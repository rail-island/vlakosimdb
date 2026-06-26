#include <jlib.h>
#pragma hdrstop

#include "zbss.h"
#include <gram/ssmain.h>
#include "zbint.h"
#include "zbrt.h"

namespace zaba {
using namespace ss;

void ssRunner::addglobal(STRING name,zbObject *obj) {
  ssm.globals->setvar(name,zbtoss(obj));
}

string ssRunner::zbtoss(zbObject *obj) {
  if (zbString *s=dynamic_cast<zbString*>(obj)) return s->value;
  if (zbNumber *n=dynamic_cast<zbNumber*>(obj)) return ftos(n->value);
  return ssm.objtostr(ssObject(obj,"zbo"));
}

zbObject *ssRunner::sstozb(string obj) {
  ssObject sso=ssm.strtoobj(obj);
  if (sso.istype("zbo")) return ((zbObject*)(sso.data))->addref();
  return createString(obj,rt);
}

string ssRunner::callmethod(ssObject obj,STRING name,Strings &pars,Module *module) {
  zbObject *zo=(zbObject*)obj.data;
  string res;
  if (!obj.istype("zbo")) zo=NULL;
  if (!zo || !trycallmethod(zo,name,pars,module,res)) res=ss::Library::callmethod(obj,name,pars,module);
  return res;
}
string ssRunner::getprop(ssObject obj,STRING name,Module *module) {
  zbObject *zo=(zbObject*)obj.data;
  Strings pars;
  string res;
  if (!obj.istype("zbo")) zo=NULL;
  if (!zo || !trycallmethod(zo,string("get:")+name,pars,module,res)) res=ss::Library::callmethod(obj,name,pars,module);
  return res;
}
string ssRunner::getvar(STRING name,Module *module) {
  if (data) {
    zbObject *res=data->getvar(name);
    if (res) {
      string r=zbtoss(res);
      res->release();
      return r;
    }
  }
  return ss::Library::getvar(name,module);
}

bool ssRunner::trycallmethod(zbObject *obj,STRING name,Strings &pars,ss::Module *module,string &res) {
  zaba::Interpreter ir;
  ir.rt=rt;
  for(int i=0;i<pars.count;i++) ir._push(sstozb(pars[i]));
  ir.push(obj);
  if (handler->trycallmethod(&ir,name,pars.count)) {
    ir.interpret();
    if (zbObject *obj=ir.gettop()) res=zbtoss(obj);else res="";
    return true;
  }
  return false;
}

string ssRunner::callfn(STRING name,Strings &pars,Module *module) {
  string res;
  if (!trycallmethod(handler,name,pars,module,res)) res=ss::Library::callfn(name,pars,module);
  return res;
}

void ssRunner::run(STRING com) {
  ss::Runtime ssrt(&ssm);
  ssrt.a=com;
  ssrt.run();
}

void ssRunner::deleteobject(ss::ssObject obj) {
  if (obj.istype("zbo")) ((zbObject*)obj.data)->release();
/*
  if (obj.istype("zbo")) {
    zbObject *zbo=((zbObject*)obj.data);
    eputs(zbo->debugstring().c_str());
    ((zbObject*)obj.data)->release();
  }
*/
}
void ssRunner::createobject(ss::ssObject obj) {
  if (obj.istype("zbo")) ((zbObject*)obj.data)->addref();
}


}


