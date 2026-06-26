#ifndef zbobjH
#define zbobjH

#include "zbdefs.h"
#include <ext/garbcol.h>
#include <ext/xmlnode.h>

//#define ZBDEBUG
#define OBJECTCHECK
//#define OBJCOUNTING


#ifdef ZBDEBUG
#define OBJCOUNTING
#define OBJECTCHECK
#define INTERPRETDEBUG
#endif

namespace zaba {


#ifdef OBJECTCHECK
#define CHECKOBJ xassert(this);xassert(objcheck!=DELETEDOBJ);xassert(objcheck==ACTIVEOBJ);
#else
#define CHECKOBJ
#endif

const int ACTIVEOBJ =0x34F683DE;
const int DELETEDOBJ=0x6A08B244;

#ifdef OBJCOUNTING
struct ObjCounter:TCntVec<zbObject*> {
  ~ObjCounter();
};
extern ObjCounter objcounter;
#endif

//enum FindFlags {ff_method=1,ff_prop=2,ff_firstmethod=4,ff_search=8,ff_raise=16};
enum FindFlags {ff_method=1,ff_search=2,ff_raise=4};
//pokud neni ff_method, je ff_fld


class Namespace;class Instruction;

typedef RefBase<zbObject> ZabaObject;
struct zbObject {
  GC_Data gc_data;
  Interface *intf;
  Runtime *rt;

//  void cm(Interpreter *in,STRING name,int parcnt);
//  void xcm(Interpreter *in,STRING name,int parcnt);
  //jen prevede volani na Interface::cm resp. Interface::xcm

  bool trycallmethod(Interpreter *in,STRING name,int parcnt);

  //tyto zbObject* jsou jen ukazatele, ne platne reference
  virtual zbObject *getprop(STRING name) {return NULL;}
  virtual bool setprop(STRING name,zbObject *value) {return false;}
  virtual bool initprop(STRING name,zbObject *value) {return false;}
  virtual bool assignprop(STRING name,zbObject *value) {return false;}
  virtual bool hasprop(STRING name) {return false;}
  virtual bool deleteprops(int count) {return false;}
  virtual Instruction *getcodeaddress() {return NULL;}

  virtual zbObject *find(int flags,const char *id,zbObject *mainnames);
  virtual bool joinnames(zbObject *dest) {return false;} //spojeni namespaces
  virtual bool reachable(zbObject *dest) {return false;} //spojeni namespaces

  zbObject *addref() {CHECKOBJ refcnt++;return this;}
  void release() {CHECKOBJ if (--refcnt==0) destroy();}
  zbObject(Runtime *rt,STRING intfname);
  int _refcnt() {return refcnt;}

  virtual string tostring() {return "";}
  string debugstring();
  virtual string typestr() {return "Object";}
  virtual int toint() {return 0;}
  virtual number_t tofloat() {return 0;}
  bool tobool();

  zbObject *cvalue() {return this;}

  #ifdef OBJCOUNTING
  static int global_count;
  #endif
  #ifdef OBJECTCHECK
  int objcheck;
  #endif

  void dbgsave(XMLNode xml);

public://GC Data
  virtual void enum_refs_and_add_inrefs() {}
  virtual void enum_refs_and_validate() {}
  virtual void unlink_refs() {}
  void gc_add_in_ref() {gc_data.addinref();}
  void gc_validate() {
    bool was_valid=gc_data.valid_object;
    gc_data.validate();
    if (!was_valid) enum_refs_and_validate();
  }
  void gc_release() {refcnt--;}

  void destroy();
  #ifdef REFCHECK
  int refcnt;
  #endif
private:
  #ifndef REFCHECK
  int refcnt;
  #endif
protected:
  virtual ~zbObject();
};

struct zbException:zbObject {
  zbException(Runtime *rt,STRING intf="Exception.instance"):zbObject(rt,intf) {}
  void setmsg(string amsg) {msg=amsg;}
  string tostring() {return msg;}
private:
  string msg;
};

struct NamedVar {
  char name[0x20];
  zbObject *value;
  ~NamedVar() {if (value) value->release();}
  NamedVar(zbObject *avalue,STRING aname) {value=avalue;strcpy(name,aname);}
  void gc_clear() {value->gc_release();value=NULL;}
  bool match(STRING aname) {return aname==name;}
};

struct Namespace {
  IObjArray<NamedVar> vars;
  zbObject *findvar(STRING name) {NamedVar *res=vars.searchvalue(name);if (res) return res->value;return NULL;}
  void setvar(STRING name,zbObject *value);
  bool initvar(STRING name,zbObject *value);
  bool assignvar(STRING name,zbObject *value);
  bool deleteprops(int count) {if (count>vars.count) return false;vars.eraselast(count);return true;}
  bool hasvar(STRING name) {return vars.psearchv(name);}
  void print(STROUT &fw);
  void enum_refs_and_add_inrefs();
  void gc_clear();
  void enum_refs_and_validate();
  void dbgsave(XMLNode xml);
};

struct zbInstance:zbObject {//jakakoli instance objektu vytvoreneho interpretrem
  Namespace vars;
  TRCArray<zbObject> subnames;

  zbInstance(Runtime *rt,STRING name):zbObject(rt,name) {}
  zbObject *getprop(STRING name) {return vars.findvar(name);}
  bool setprop(STRING name,zbObject *value) {vars.setvar(name,value);return true;}
  bool initprop(STRING name,zbObject *value) {return vars.initvar(name,value);}
  bool assignprop(STRING name,zbObject *value) {return vars.assignvar(name,value);}
  bool hasprop(STRING name) {return vars.hasvar(name);}
  bool deleteprops(int count) {return vars.deleteprops(count);}

  zbObject *find(int flags,const char *id,zbObject *mainnames);
  bool joinnames(zbObject *dest);
  bool reachable(zbObject *dest);
protected:
  void enum_refs_and_add_inrefs() {vars.enum_refs_and_add_inrefs();}
  void enum_refs_and_validate() {vars.enum_refs_and_validate();}
  void unlink_refs() {vars.gc_clear();}
};

struct Structure {
  zbInstance *data;
  Structure(zbInstance *data):data(data) {}
  int ivalue(STRING name,int def=0) {if (!data) return def;zbObject *res=data->getprop(name);if (res) return res->toint();return def;}
  bool bvalue(STRING name,int def=0) {if (!data) return def;zbObject *res=data->getprop(name);if (res) return res->tobool();return def;}
  int fvalue(STRING name,double def=0) {if (!data) return def;zbObject *res=data->getprop(name);if (res) return res->tofloat();return def;}
  string svalue(STRING name,string def="") {if (!data) return def;zbObject *res=data->getprop(name);if (res) return res->tostring();return def;}
  template<class T> T *get(STRING name) {if (!data) return NULL;zbObject *res=data->getprop(name);if (res) return dynamic_cast<T*>(res);return NULL;}
  void setn(STRING name,number_t value) {if (!data) return;zbObject *num=createNumber(value,data->rt);data->setprop(name,num);num->release();}
  void sets(STRING name,string value) {if (!data) return;zbObject *str=createString(value,data->rt);data->setprop(name,str);str->release();}
  void setb(STRING name,bool value) {if (!data) return;zbObject *bol=createLogical(value,data->rt);data->setprop(name,bol);bol->release();}
};

void ewrite(zbObject *obj,const char *prefix);

ZabaObject callMethod(zbObject *obj,STRING name,zbObject **pars,int parcnt);//throws JException

}

#endif
