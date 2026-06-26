#ifndef zbssH
#define zbssH

#include <gram/ssdefs.h>
#include "zbobj.h" 

namespace zaba {

struct zbssData {
  virtual zbObject *getvar(STRING name) {return NULL;}
};

struct ssRunner:ss::Library {
  zbObject *handler;
  zaba::Runtime *rt;
  ss::Module ssm;
  zbssData *data;

  void run(STRING com);
  ssRunner(zaba::Runtime *rt):rt(rt) {handler=NULL;ssm.lib=this;data=NULL;}
  string zbtoss(zbObject *obj);
  zbObject *sstozb(string obj);
  void addglobal(STRING name,zbObject *obj);
  void addglobal(STRING name,string value) {ssm.globals->setvar(name,value);}
  string getglobal(STRING name) {return ssm.globals->svar(name);}
private:
  bool trycallmethod(zbObject *obj,STRING name,Strings &pars,ss::Module *module,string &res);
protected:
  void deleteobject(ss::ssObject obj);
  void createobject(ss::ssObject obj);
  string callmethod(ss::ssObject obj,STRING name,Strings &pars,ss::Module *module);
  string getprop(ss::ssObject obj,STRING name,ss::Module *module);
  string callfn(STRING name,Strings &pars,ss::Module *module);
  string getvar(STRING name,ss::Module *module);
};

}

#endif
