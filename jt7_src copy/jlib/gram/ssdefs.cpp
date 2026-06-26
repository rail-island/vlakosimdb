#include <jlib.h>
#pragma hdrstop

#include "ssdefs.h"
#include <gram/gmanal.h>
#include <ext/cmstrdata.h>
#include <ext/xmlfile.h>

namespace ss {

void Library::foreach(ssObject obj,Strings &items,Module *module) {
  if (obj.data==NULL) return;
  if (obj.istype("xml")) {
    XMLNode xml((XMLDataNode*)obj.data);
    for(int i=0;i<xml.count();i++) {
      items.add(module->objtostr(xmltoobj(xml[i])));
    }
  }
}

string Library::getprop(ssObject obj,STRING name,Module *module) {
  if (obj.data==NULL) return "";
  if (obj.istype("xml")) {
    XMLNode xml((XMLDataNode*)obj.data);
    if (name=="count") return itos(xml.count());
    if (name=="vars") return module->objtostr(varstoobj(xml));
    if (name=="name") return xml.getname();
    if (name=="text") return xml.gettext();
    if (*name.c_str()=='$') return xml.svar(name.c_str()+1);
    return xml.svar(name);
  }
  if (obj.istype("xmlvars")) {
    XMLNode xml((XMLDataNode*)obj.data);
    if (name=="count") return itos(xml.varcount());
  }
  return "";
}

string Library::callmethod(ssObject obj,STRING name,Strings &pars,Module *module) {
  if (obj.data==NULL) return "";
  if (obj.istype("xml")) {
    XMLNode xml((XMLDataNode*)obj.data);
    if (name=="getindex") {
      if (pars.count==0) return "";
      if (gram::Analyser(pars[0].c_str()).is_digit())
        return module->objtostr(xmltoobj(xml[atoi(pars[0].c_str())]));
      return module->objtostr(xmltoobj(xml[pars[0]]));
    }
    if (name=="find") {
      if (pars.count!=2) return "";
      for(int i=0;i<xml.count();i++) if (pars[1]==xml[i].svar(pars[0]))
        return module->objtostr(xmltoobj(xml[i]));
      return "";
    }
  }
  if (obj.istype("xmlvars")) {
    XMLNode xml((XMLDataNode*)obj.data);
    if (name=="getindex") {
      if (pars.count==0) return "";
      if (gram::Analyser(pars[0].c_str()).is_digit())
        return xml.getvar(atoi(pars[0].c_str()));
      return xml.svar(pars[0]);
    }
  }
  return "";
}

ssObject Library::xmltoobj(XMLNode xml) {
  ssObject res;
  res.data=xml.getnode();xml.getnode()->addref();
  res.type="xml";
  return res;
}
ssObject Library::varstoobj(XMLNode xml) {
  ssObject res;res.data=xml.getnode();
  xml.getnode()->addref();
  res.type="xmlvars";
  return res;
}
void Library::deleteobject(ssObject obj) {
  if (obj.istype("xml")) {
    XMLDataNode *xml=(XMLDataNode*)obj.data;
    xml->release();
  }
}


void Module::clearobjs() {
  if (lib) {
    for(int i=0;i<objs.count;i++) lib->deleteobject(objs[i].obj);
  }
  objs.clear();
}

string Module::getvarvalue(string name) {
  if (name.begins("$")) return globals->findvalue(name.c_str()+1);
  if (locals->isvar(name)) return locals->findvalue(name);
  if (lib) return lib->getvar(name,this);
  return string("");
}

string Library::callfn(STRING name,Strings &pars,Module *module) {
  if (name=="trim" && pars.count==1) return pars[0].gettrimed();
  return "";
}

void Module::foreach(string obj,Strings &items) {
  if (obj.begins("#[obj]")) {
    if (lib) lib->foreach(strtoobj(obj),items,this);
  } else if (isSA(obj)) decodeSA(obj,items);
}

string Module::callmethod(string obj,string name,Strings &pars) {
  if (obj.begins("#[obj]")) {
    if (lib) return lib->callmethod(strtoobj(obj),name,pars,this);
    return "";
  }
  if (obj.begins("#[Record]") && name=="set" && pars.count==2) {
    return SRsetprop(obj,pars[0],pars[1]);
  }
  if (name=="replace" && pars.count==2) {
    obj.replaceall(pars[0],pars[1]);return obj;
  }
  if (name=="trim" && pars.count==0) return obj.gettrimed();
  if (name=="filename" && pars.count==0) {
    sFile file(obj);return file.getfile()%"";
  }
  if (name=="lines" && pars.count<=1) {
    char sep='\n';
    if (pars.count>0 && pars[0].length()>0) sep=pars[0].c_str()[0];
    Strings items;
    for(lineiter it=obj.getlines(sep);*it;it++) items.add(*it);
    return createSA(items);
  }
  if (name=="xmlencode" && pars.count==0) {
    return xmlencode(obj);
  }
  if (name=="htmlencode" && pars.count==0) {
    return htmlencode(obj);
  }
  return "";
}
string Module::getprop(string obj,string name) {
  if (obj.begins("#[obj]")) {
    if (lib) return lib->getprop(strtoobj(obj),name,this);
    else return "";
  }
  if (obj.begins("#[Record]")) {
    return SRgetprop(obj,name);
  }
  return "";

}

string Module::objtostr(ssObject obj) {
  int res=-1;
  for(int i=0;i<objs.count;i++) if (objs[i].obj==obj) {res=i;break;}
  if (res<0) {
    res=objs.count;
    ssObjectItem &n=objs.add();
    n.obj=obj;
    n.check=actcheck++;
    if (lib) lib->createobject(obj);
  }
  return fmtstr("#[obj]%d:%d",res,objs[res].check);
}
ssObject Module::strtoobj(string str) {
  if (! str.begins("#[obj]") ) return ssObject();
  const char *s=str.c_str()+6;
  gram::Analyser a(s);
  int idx=a.readint();
  if (*a==':') a++;
  int check=a.readint();
  if (idx<0 || idx>=objs.count) return ssObject();
  if (objs[idx].check!=check) return ssObject();
  return objs[idx].obj;
}

FldDef Module::splitfld(string obj) {
//  char buf[0x100];
  if (obj.begins("#[fld]")) {
    FldDef res(true);
    int tpos=obj.search('.');
    if (tpos<0) return res;
    res.fld=obj.copy(6,tpos-6);
    res.obj=obj.c_str()+tpos+1;
/*    strcpy(buf,obj.c_str()+6);
    char *pt=strchr(buf,'.'),*prop=buf;
    if (pt) *pt=0,prop=pt+1;*/
    return res;
//    return FldDef(buf,prop);
  }
  return false;
}

string Module::callfunc(string name,Strings &pars) {
  if (name=="alert") {
    MEMSTROUT mso;
    for(int i=0;i<pars.count;i++) mso<<pars[i]<<"\n";
    jsystem->showinfo(mso.str());
    return "";
  }
  if (name=="record") {
    VarList vars;
    for(int i=0;i<pars.count/2;i++) vars.setvar(pars[i*2],pars[i*2+1]);
    return SRcreate(vars);
  }
  if (SubDef *sub=subs.psearchv(name)) return sub->call(pars);
  return callfn(name,pars);
}

string Module::callfn(string name,Strings &pars) {
  return lib?lib->callfn(name,pars,this):string("");
}

string Module::callsub(string name,Strings &pars) {
  if (SubDef *sd=subs.psearchv(name)) return sd->call(pars);
  return string("");
}

void Module::doassign(string lvalue,string value) {
  if (isref(lvalue)) assignvar(reffromvar(lvalue),value);
}

void Module::assignvar(string varname,string value) {
  if (varname.begins("$")) globals->setvar(varname.c_str()+1,value);
  else locals->setvar(varname,value);
}

string Module::getvalue(string obj) {
  if (isref(obj)) return getvarvalue(reffromvar(obj));
  if (FldDef fld=splitfld(obj)) return getprop(fld.obj,fld.fld);
  return obj;
}

string Module::callobj(string obj,Strings &pars) {
//  char buf[0x100],*o,*f;
//  if (splitfld(obj,buf,&o,&f)) return callmethod(o,f,pars);
  if (FldDef fld=splitfld(obj)) return callmethod(fld.obj,fld.fld,pars);
  if (isref(obj)) return callfunc(reffromvar(obj),pars);
//  obj=getvalue(obj);
//  return callfunc(obj,pars);
  return "";
}

void Module::addsub(SubDef *sub) {
  if (sub->inmodule) return;
  if (SubDef *old=subs.psearchv(sub->name)) return;
  sub->inmodule=true;
  subs.add(sub);
}


}


