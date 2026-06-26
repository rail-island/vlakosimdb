#include <jlib.h>
#pragma hdrstop

#include "zbintf.h"
#include "zbint.h"
#include "zbrt.h"

namespace zaba {

InternalMethod::~InternalMethod() {
  delete code;
}

void NativeMethod::load(Analyser &a) {
  zbc_jmpemptylines(a);
  a.readto(name,0x100," ",false);
  xcheckstrlen(name);
  char nname[0x100];
  a.jsp();
  a.readline(nname);
  xcheckstrlen(nname);
  sub=rt->searchsub(nname);
  if (!sub) a.error(fmtstr("Unknown native sub:'%s'",nname));
}

bool NativeMethod::call(zbObject *thiz,Interpreter *ir,int parcnt) {
  return sub(ir,parcnt);
}

void InternalMethod::load(Analyser &a) {
  a.readto(name,0x100," ",false);
  a.jsp();if (! a.is_digit()) a.error("Integer expected");minpars=a.readint();
  a.jsp();if (a.is_digit()) maxpars=a.readint();else maxpars=minpars;
  a.roll();
  code=new Code(rt);
  code->load(a);
}

bool InternalMethod::call(zbObject *thiz,Interpreter *ir,int parcnt) {
  if (parcnt<minpars ||parcnt>maxpars) return false;
  ir->internalcall(thiz,code->get1(),parcnt,&info);
  return true;
}

bool Interface::makecreator() {
  if (creator) return false;
  if (creatoras=="") {
    creator=rt->getstdcreator();
    return true;
  }
  Interface *cr=rt->searchintf(creatoras);
  if (cr->creator) {
    creator=cr->creator;
    return true;
  }
  return false;
}

bool Interface::hasincluded(Interface *intf) {
  if (intf==this) return true;
  for(int i=0;i<included.count;i++) if (included[i]->hasincluded(intf)) return true;
  return false;
}

void Interface::addmethods(Array<Method*> &items) {
  for(int i=0;i<items.count;i++) {
    bool add=true;
    for(int j=0;j<mtds.count;j++) if (eqstr(items[i]->name,mtds[j]->name)) {
      add=false;
      break;
    }
    if (add) mtds.add(items[i]);
  }
}

void Interface::makeinclude() {
  for(int i=0;i<includednames.count;i++) {
    Interface *inc=rt->searchintf(includednames[i]);
    included.add(inc);
  }
  for(int i=0;i<included.count;i++) {
    bool allreadyincluded=false;
    for(int j=0;j<included.count;j++)
      if (i!=j)
        if (included[j]->hasincluded(included[i])) allreadyincluded=true;
    if (!allreadyincluded) addmethods(included[i]->mtds);
  }
}

bool Interface::includesonly(Array<Interface*> &what) {
  for(int i=0;i<includednames.count;i++) if (what.psearchi(includednames[i])<0) return false;
  return true;
}

bool Interface::trycallmethod(const char *name,Interpreter *ir,int parcnt) {
  zbObject *thiz=ir->getfnthis(parcnt);
  for(int i=0;i<mtds.count;i++) {
    Method *mtd=mtds[i];
    if (eqstr(mtd->name,name))
      if (mtd->call(thiz,ir,parcnt)) return true;
  }
  return false;
}


Interface::Interface(Runtime *rt):rt(rt) {
  autocreate=false;
  initialized=false;
  loaded=false;
  *name=0;
}

/*
bool Interface::requires_ok(int initlevel) {
  for(int i=0;i<requires.count;i++) {
    Interface *intf=rt->searchintf(requires[i]);
    if (intf->initialized_level<initlevel && intf->initialized_level<intf->max_init_level)
      return false;
  }
  return true;
}
*/

zbObject *Interface::create() {
  zbObject *res=creator->createInstance(rt,name);
  for(int i=0;i<initprocs.count;i++) {
    rt->addinitproc(res,initprocs[i].name,initprocs[i].priority);
  }
  return res;
}

struct P_Prop {
  char name[0x100];
  char value[0x1000];
  void parse(Analyser &a) {
    a.readto(name,0x100,"=",false);
    a.f_char('=');
    a.readto(value,0x1000,"\n",false);
    a.f_char('\n');
    trimr(value);
  }
  bool operator ==(const char *s) {return eqstr(name,s);}
};

void Interface::load(Analyser &a) {
  if (loaded) throw JError(fmtstr("Interface allready loaded:%s",name));
  loaded=true;
  STRLIST ips;
  zbc_jmpemptylines(a);
  while (!a.begins("begin")) {
    P_Prop p;
    p.parse(a);
    if (p=="initproc") ips.add(p.value);
    if (p=="creator") creator=rt->searchcreator(p.value);
    if (p=="creatoras") creatoras=p.value;
    if (p=="include") includednames.add(p.value);
    if (p=="autocreate") autocreate=atoi(p.value);
    if (p=="uselib") rt->uselib(p.value);
    if (p=="usemod") rt->usemod(p.value);
    if (p=="require") requireintfs.add(p.value);
    zbc_jmpemptylines(a);
  }
  zbc_jmpemptylines(a);
  a.f_jump("begin");a.roll();
  for(;;) {
    zbc_jmpemptylines(a);
    if (a.begins("end")) {a.roll();goto endfor;}
    if (a.begjmp("sub")) {
      InternalMethod *mtd=new InternalMethod(rt);
      mtds.add(mtd);
      a.jsp();
      mtd->load(a);
    } else if (a.begjmp("native")) {
      NativeMethod *mtd=new NativeMethod(rt);
      mtds.add(mtd);
      a.jsp();
      mtd->load(a);
    } else if (a.seekch(';')) a.roll();
    else a.error("end,native or sub expected");
  }
  endfor:
  for(int i=0;i<ips.count;i++) {
    char ip[0x1000];
    strcpy(ip,ips[i]);
    char *carka=strchr(ip,',');
    if (!carka) throw JError(fmtstr("Bad initproc format:%s",ip));
    *carka=0;
    InitProcDef &ipd=initprocs.add();
    ipd.intf=this;
    strncpy(ipd.name,ip,0xFF);ipd.name[0xFF]=0;
    ipd.priority=atoi(carka+1);
  }
}

}


