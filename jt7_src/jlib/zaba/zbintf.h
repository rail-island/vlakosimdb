#ifndef zbintfH
#define zbintfH

#include "zbcode.h"
#include "zbdefs.h"
#include <com/rcobj.h>

namespace zaba {

class Interpreter;class Code;class Runtime;

//typedef zbObject * (*Native_Sub)(Interpreter *ir,int argc,zbObject **argv);
typedef bool (*Native_Sub)(Interpreter *ir,int parcnt);

/*
struct MethodId {
  STRING name;
  int parcnt;
  MethodId(STRING name,int parcnt):name(name),parcnt(parcnt) {}
//  bool operator ==(const MethodId &right) {return name==right.name && parcnt==right.parcnt;}
};
*/
struct FnInfo {
  const char *name;
};

struct Method:RCObject {
  char name[0x100];
  FnInfo info;
  bool match(STRING aname) {return aname==name;}
//  int minpars,maxpars;
  Runtime *rt;
//  bool match(const MethodId &mtd) {return mtd.name==name && mtd.parcnt>=minpars && mtd.parcnt<=maxpars;}
  Method(Runtime *rt):rt(rt) {*name=0;info.name=name;}//minpars=maxpars=0;}
  virtual bool call(zbObject *thiz,Interpreter *ir,int parcnt)=0;
  //vraci, zda byla metoda zavolana
  virtual ~Method() {}
//protected:
//  void loadhdr(Analyser &a);
};

struct InternalMethod:Method {
  Code *code;
  int minpars,maxpars;

  InternalMethod(Runtime *rt):Method(rt) {code=NULL;minpars=maxpars=0;}
  ~InternalMethod();
  void load(Analyser &a);
  bool call(zbObject *thiz,Interpreter *ir,int parcnt);
};

struct NativeMethod:Method {
  Native_Sub sub;
  void load(Analyser &a);
  bool call(zbObject *thiz,Interpreter *ir,int parcnt);
  NativeMethod(Runtime *rt):Method(rt) {sub=NULL;}
};

//enum InitLevel {it_system,it_library,it_user};

//typedef int InitLevel;
enum InitLevel {it_system=0,it_library=16,it_user=32,it_program=48,it_max=256};

struct InitProcDef {
  char name[0x100];
  int priority;
  Interface *intf;
};

/**
Interface:soubor metod(jen metod)
typicky pro kazdou tridu jsou 2 Interface trida.class a trida.instance
*/
struct Interface {
  char name[0x100];

  Runtime *rt;
  TRCArray<Method> mtds;

  bool initialized;
  bool autocreate;
  bool loaded;

//  void xcm(const char *name,Interpreter *ir,int parcnt);
//  void cm(const char *name,Interpreter *ir,int parcnt);
  bool trycallmethod(const char *name,Interpreter *ir,int parcnt);

  void load(Analyser &a);
  Interface(Runtime *rt);
  bool match(STRING aname) {return aname==name;}
//  bool initialize(int step);
//  bool runinit();//je pusteno, az pokud jsou spusteny vsechny inicializace s mensim initlevel
  //vrati, zda uz neni potreba inicializovat( zda jsou inicializovany vsechny preqizity,nebo
  //neni autocreate
  zbObject *create();
//  Interface *ancestor;
//  InitLevel initlevel;

  ///vrati, zda inkluduje jen interface dane polem what
  bool includesonly(Array<Interface*> &what);
  //provede includovani metod, vsechny interfacy, na ktere se provadi, uz jsou takto upravene
  void makeinclude();
  ///pokusi se najit creator (z pole creatoras), vrati, zda ho tato funkce nasla
  bool makecreator();
  bool hascreator() {return creator;}
  bool hasincluded(Interface *intf);

  bool requires(STRING name) {return requireintfs.search(name)>=0;}
private:
  void addmethods(Array<Method*> &items);
  RefBase<InstanceCreator> creator;
  TCntVec<InitProcDef> initprocs;
  STRLIST includednames;
  STRLIST requireintfs;
  TCntVec<Interface*> included;
  string creatoras;
//  STRLIST ancestors;
//  STRLIST preqisites;
//  TCntVec<Interface*> ans;
//  TCntVec<Interface*> preqs;
//  void initcreator();
//  void linkpreqs();
//  void linkancestors();
//  void loadautocreate(Analyser &a);
//  void runinit();
};

}

#endif
