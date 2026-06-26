#ifndef ssdefsH
#define ssdefsH

#include <ext/xmlnode.h>

namespace ss {

struct SubDef:RCObject {
  char name[0x100];
  bool match(STRING aname) {return aname==name;}
  virtual string call(Strings &pars)=0;
  virtual ~SubDef() {}
  SubDef() {inmodule=false;}
private:
  bool inmodule;
  friend class Module;
};

struct FldDef {
  string obj,fld;
  bool valid;
  operator bool () {return valid;}
  FldDef(bool valid):valid(valid) {}
  FldDef(string obj,string fld):obj(obj),fld(fld),valid(true) {}
};

struct ssObject {
  void *data;
  const char *type;
  bool operator ==(const ssObject &b) const {
    return data==b.data && eqstr(type,b.type);
  }
  bool istype(STRING atype) {return type && atype==type;}
  ssObject() {data=NULL;type=0;}
  ssObject(void *data,const char *type):data(data),type(type) {}
};

struct ssObjectItem {
  ssObject obj;
  int check;
};

class Module;
struct Library {
  static ssObject xmltoobj(XMLNode xml);
  static ssObject varstoobj(XMLNode xml);
  virtual string callmethod(ssObject obj,STRING name,Strings &pars,Module *module);
  virtual string getprop(ssObject obj,STRING name,Module *module);
  virtual string callfn(STRING name,Strings &pars,Module *module);
  virtual string getvar(STRING name,Module *module) {return "";}
  virtual void foreach(ssObject obj,Strings &items,Module *module);
  virtual void deleteobject(ssObject obj);
  virtual void createobject(ssObject obj) {}
  //volano z objtostr(), pokud dany objekt je nevo vyrvaren
};

struct Module {
//  IObjArray<SubDef> subs;
  TCntVec<ssObjectItem> objs;
  RefBase<VarList> globals;
  VarList *locals;
  Library *lib;

  Module() {globals=new VarList();locals=globals;actcheck=0x3F870A;lib=NULL;}
  ~Module() {clearobjs();}

  string objtostr(ssObject obj);
  ssObject strtoobj(string str);
  string callmethod(string obj,string name,Strings &pars);
  string getprop(string obj,string name);
  string callfn(string name,Strings &pars);
  string callsub(string name,Strings &pars);
  string getvarvalue(string name);
  string getvalue(string obj);
  string callobj(string obj,Strings &pars);
  string callfunc(string name,Strings &pars);
  bool hassub(string name) {return subs.psearchv(name);}
  void assignvar(string varname,string value);
  void doassign(string lvalue,string value);
  FldDef splitfld(string obj);
  bool isref(string obj) {return obj.begins("#[ref]");}
  string reffromvar(string obj) {return obj.c_str()+6;}
  void foreach(string obj,Strings &items);
  void clearobjs();
  void addsub(SubDef *sub);
  bool tobool(string val) {return val!="" && val!="0";}
protected:
  int actcheck;
  TRCArray<SubDef> subs;
};

struct DefLocals {
  VarList *oldl;Module *mod;
  DefLocals(VarList *newl,Module *mod):mod(mod) {oldl=mod->locals;mod->locals=newl;}
  ~DefLocals() {mod->locals=oldl;}
};

#define DEF_LOCALS VarList locs;DefLocals def_locs(&locs,module);

}

#endif
