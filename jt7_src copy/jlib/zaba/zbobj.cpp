#include <jlib.h>
#pragma hdrstop

#include "zbobj.h"
#include "zbint.h"
#include "zbintf.h"
#include "zbrt.h"
#include <typeinfo>

namespace zaba {

#ifdef OBJCOUNTING
ObjCounter objcounter;

ObjCounter::~ObjCounter() {
  eputs("\n\nNo released objects\n===================\n");
  for(int i=0;i<count;i++) eputs(typeid(*item(i)).name()),eputs(" : "),ewrite(item(i),"");
}

int zbObject::global_count=0;
#endif

ZabaObject callMethod(zbObject *obj,STRING name,zbObject **pars,int parcnt) {
  Interpreter ir;
  ir.rt=obj->rt;
  for(int i=0;i<parcnt;i++) ir.push(pars[i]);
  ir.push(obj);
  if (obj->trycallmethod(&ir,name,parcnt)) {
    ir.interpret();
    if (zbObject *res=ir.gettop()) return res;
  } else throw JError(Interpreter::nomtdmsg(obj,name,parcnt));
  return NULL;
}

string zbObject::debugstring() {
  return tostring()+":"+intf->name+":"+typeid(*this).name()+";"+itos((int)this);
}

/*
zbObject *zbObject::find(int flags,const char *id) {
  if (flags & ff_firstmethod) {
    if (flags & ff_method) {
      if (Method *m=intf->mtds.psearchv(id)) {
        return this;
      }
    }
    if (flags & ff_prop) {
      if (Method *m=intf->mtds.psearchv(id)) {
        return this;
      }
    }
  } else {
    if (flags & ff_prop) {
      if (Method *m=intf->mtds.psearchv(id)) {
        return this;
      }
    }
    if (flags & ff_method) {
      if (Method *m=intf->mtds.psearchv(id)) {
        return this;
      }
    }
  }
  return NULL;
}
*/

zbObject *zbObject::find(int flags,const char *id,zbObject *mainnames) {
  if (flags & ff_method) {
    if (Method *m=intf->mtds.psearchv(id)) return this;
  }
  return NULL;
}

bool zbInstance::joinnames(zbObject *dest) {
/*  if ((int)dest==12316400) {
    jsystem->showinfo("join-test");
    return false;
  }*/
  if (dest->reachable(this))
    return false;
  subnames.add(dest);return true;
}

bool zbInstance::reachable(zbObject *dest) {
  if (dest==this)
    return true;
  for(int i=0;i<subnames.count;i++) if (subnames[i]->reachable(dest)) return true;
  return false;
}

zbObject *zbInstance::find(int flags,const char *id,zbObject *mainnames) {
  if (mainnames==this)
    return NULL;
  if (!mainnames) mainnames=this;
  if (flags & ff_method) {
    if (Method *m=intf->mtds.psearchv(id)) return this;
  } else {
    if (vars.hasvar(id)) return this;
  }
  if (flags & ff_search) {
    for(int i=0;i<subnames.count;i++)
      if (zbObject *r=subnames[i]->find(flags,id,mainnames)) return r;
  }
  return NULL;
}

/*
zbObject *zbInstance::find(int flags,const char *id) {
  zbObject *res=zbObject::find(flags,id);
  if (res) return res;
  if (flags & ff_search) {
    for(int i=0;i<subnames.count;i++)
      if (zbObject *r=subnames[i]->find(flags,id)) return r;
  }
  return NULL;
}
*/

void zbObject::dbgsave(XMLNode xml) {
  char buf[0x10000],buf2[0x10000];
  strcpy(buf,tostring().c_str());
  char *c=buf,*w=buf2;
  while (*c) {
    if (*c=='\n') *w++='\\',*w++='n';else *w++=*c;
    c++;
  }
  *w=0;
  xml.setvar("tostring",buf2);
  xml.setvar("refcnt",refcnt);
  xml.setvar("typestr",typestr());
  xml.setvar("ctype",typeid(*this).name());
  xml.setvar("interface",intf->name);
}

void Namespace::dbgsave(XMLNode xml) {
  for(int i=0;i<vars.count;i++) {
    XMLNode var=xml.add("var");
    var.setvar("name",vars[i]->name);
    vars[i]->value->dbgsave(var.add("value"));
  }
}

void Namespace::print(STROUT &fw) {
  for(int i=0;i<vars.count;i++) fw<<vars[i]->name<<"=",fw.writecstring(vars[i]->value->tostring().c_str()),fw<<"\n";
}

void Namespace::setvar(STRING name,zbObject *value) {
  NamedVar *var=vars.searchvalue(name);
  if (var) {
    var->value->release();
    var->value=value->addref();
    return;
  }
  var=new NamedVar(value->addref(),name);
  vars.add(var);
}

bool Namespace::initvar(STRING name,zbObject *value) {
  NamedVar *var=vars.searchvalue(name);
  if (var) return false;
  var=new NamedVar(value->addref(),name);
  vars.add(var);
  return true;
}

bool Namespace::assignvar(STRING name,zbObject *value) {
  NamedVar *var=vars.searchvalue(name);
  if (var) {
    var->value->release();
    var->value=value->addref();
    return true;
  }
  return false;
}


void ewrite(zbObject *obj,const char *prefix) {
  char buf[0x10000],buf2[0x10000];
  strcpy(buf,obj->tostring().c_str());
  char *c=buf,*w=buf2;
  while (*c) {
    if (*c=='\n') *w++='\\',*w++='n';else *w++=*c;
    c++;
  }
  *w=0;
  if (obj) eprintf("%s%s:%s[%d](%s),{%s}\n",prefix,obj->typestr().c_str(),buf2,obj->_refcnt(),typeid(*obj).name(),
  obj->intf?obj->intf->name:"");
  else eprintf("%sNULL\n",prefix);
}

zbObject::~zbObject() {
  #ifdef OBJECTCHECK
  CHECKOBJ
  objcheck=DELETEDOBJ;
  #endif
  #ifdef OBJCOUNTING
  global_count--;
  objcounter.remove(this);
  #endif
}

bool zbObject::tobool() {
  return this!=rt->getvoid();
}

zbObject::zbObject(Runtime *rt,STRING intfname):rt(rt) {
  refcnt=1;
  intf=rt->searchintf(intfname);
  #ifdef OBJCOUNTING
  global_count++;
  objcounter.add(this);
  #endif
  refcnt=1;
  #ifdef OBJECTCHECK
  objcheck=ACTIVEOBJ;
  #endif
  rt->getgc()->add(this);
}

void zbObject::destroy() {
  #ifdef INTERPRETDEBUG
  eprintf("~%s() : ",typeid(*this).name());ewrite(this,"");
  #endif
  delete this;
}

bool zbObject::trycallmethod(Interpreter *in,STRING name,int parcnt) {
  CHECKOBJ
  return intf->trycallmethod(name,in,parcnt);
}

/*
void zbObject::cm(Interpreter *in,STRING name,int parcnt) {
  CHECKOBJ
  intf->cm(name,in,parcnt);
}

void zbObject::xcm(Interpreter *in,STRING name,int parcnt) {
  CHECKOBJ
  intf->xcm(name,in,parcnt);
}
*/

void Namespace::gc_clear() {
  for(int i=0;i<vars.count;i++) vars[i]->gc_clear();
  vars.clear();
}
void Namespace::enum_refs_and_add_inrefs() {
  for(int i=0;i<vars.count;i++) vars[i]->value->gc_add_in_ref();
}
void Namespace::enum_refs_and_validate() {
  for(int i=0;i<vars.count;i++) vars[i]->value->gc_validate();
}


}










