#ifndef zbintH
#define zbintH

#include <gram/gmanal.h>
#include "zbcode.h"
#include "zbobj.h"
#include "zbdefs.h"

namespace zaba {
using namespace gram;

#ifdef __BORLANDC__
#define ZBINCALL __fastcall
#else
#define ZBINCALL
#endif

class Runtime;class FnInfo;class TryFrame;

struct CallFrame {
  Instruction *ret;
  zbObject *thiz;
  zbInstance *locals;
  FnInfo *info;
//  Namespace vars;
  zbObject **basew;
  TryFrame *tryw;
  void getvar(Interpreter *ir,STRING name);
  void setvar(Interpreter *ir,STRING name,zbObject *value);
  void release() {locals->vars.vars.clear();locals->subnames.clear();locals->release();thiz->release();}
};

struct TryFrame {
  zbObject **basew;
  CallFrame *callw;
  Instruction *except;
};

struct XFailItem {
  zbObject *object;
  const char *name;
  const char *type;//method,prop
  int parcnt;
  zbObject **pars;
};

struct Interpreter {
/*
  zbObject *basestack[0x1000],**basew;
  CallFrame callstack[0x400],*callw;
  TryFrame trystack[0x200],*tryw;
  XFailItem xstack[0x100],*xw;
*/

  zbObject **basestack,**basew;
  CallFrame *callstack,*callw;
  TryFrame *trystack,*tryw;
  XFailItem *xstack,*xw;

  Instruction *ip;Runtime *rt;
  Interpreter();
  ~Interpreter();

  zbObject **getparams(int parcnt) {return basew-parcnt-1;}
  zbObject *getfnthis(int parcnt) {return basew[-1];}
  void setfnresult(zbObject *res,int parcnt);
  void dopop(int cnt=1);
  ///vyvolani vyjimky, mohou volat jen nativni metody
  void nativeerror(string msg);
  void nativebegin() {bnativeerror=false;}
  bool isnativeerror() {return bnativeerror;}
public:
//  zbObject *right() {return basew[-1];}
//  template<class T> T *tright() {return dynamic_cast<T*>(basew[-1]);}
//  template<class T> T *tleft() {return dynamic_cast<T*>(basew[-2]);}
//  void binopres(zbObject *res) {basew--;(*basew)->release();basew[-1]->release();basew[-1]=res;}
//  void xnomtd(const char *name) {xreg=1;xerror=name;}
  void raise_error(zbObject *e);//e je platny odkaz, o ktery se funkce postara
  string getstacktrace();
  void xnomtd(const char *name,int parcnt);
  void xnoprop(const char *name);
  void xok() {xreg=0;xw=xstack;}//xerror="";}
  zbObject *gettop() {if (basew>basestack) return basew[-1];return NULL;}
//  const char *isxerror() {return xreg?xerror:NULL;}
  bool isxerror() {return xreg;}
  void interpret();
  void _interpret();
//  void init(Instruction *aip) {ip=aip;xreg=0;;basew=basestack;}
  void internalcall(zbObject *athiz,Instruction *aip,int parcnt,FnInfo *info);
  void push(zbObject *obj) {*basew++=obj->addref();}
  void _push(zbObject *obj) {*basew++=obj;}
//  void internalerror(STRING err) {error(err);}
  string getxerror();
  void throwxerror() {throw InterpretException(getxerror());}
//  void raisexerror() {xraise();}
  void getstate(XMLNode xml);
  void internalerror(string msg);
  static string nomtdmsg(zbObject *thiz,STRING name,int parcnt) {return fmtstr("XError:%s,method=%s,parcnt=%d\n",thiz->debugstring().c_str(),name,parcnt);}
//  string nomtdmsg(zbObject *thiz,STRING name,int parcnt) {return fmtstr("XError:%s,method=%s,parcnt=%d\nCallStack:\n%s",thiz->debugstring().c_str(),name,parcnt,getstacktrace().c_str());}
  ///vrati popis, kde vsude se hledaji jmena pomoci find z obj
  static string searchin(zbObject *obj,zbObject *base);
  string posinfo() {return "\nCallStack:\n"+getstacktrace();}
  void runinterrupted();
  
  Instruction *savedip;//vyuzije se pri yield
  bool stopnow;//pokud je true, interpret se pred prvni instrukci zastavi a ulozi ip do savedip
protected:
  void raisenoprop(zbObject *obj,STRING name);

  bool bnativeerror;
  int xreg;
//  const char *xerror;
  void initinstructions();
  void doraise();//zmensi basestack,callstack pri volani raise
//*********************************************************
//                     Instrukce
//*********************************************************
  void ZBINCALL empty();
  void ZBINCALL cm();
  void ZBINCALL xcm();
  void ZBINCALL pushobj();
  void ZBINCALL pop();
  void ZBINCALL doret();
  void ZBINCALL co();
//  void ZBINCALL getlocal();
//  void ZBINCALL setlocal();
  void ZBINCALL find();
  void ZBINCALL join();
  void ZBINCALL getprop();
  void ZBINCALL xgetprop();
  void ZBINCALL setprop();
  void ZBINCALL assignprop();
  void ZBINCALL initprop();
  void ZBINCALL jx();
  void ZBINCALL jnx();
  void ZBINCALL pushthis();
  void ZBINCALL pushnull();
  void ZBINCALL pushsys();
  void ZBINCALL pushlocals();
  void ZBINCALL pushparam();
  void ZBINCALL raise();
  void ZBINCALL xraise();
  void ZBINCALL xchg();
  void ZBINCALL polypop();
  void ZBINCALL condjmp();
  void ZBINCALL jmp();
  void ZBINCALL condnotjmp();
  void ZBINCALL dup();
  void ZBINCALL deleteprops();
  void ZBINCALL pushtry();
  void ZBINCALL poptry();
  void ZBINCALL move();
  void ZBINCALL callnear();
  void ZBINCALL retnear();
  void ZBINCALL fadd();
  void ZBINCALL fmul();
  void ZBINCALL fdiv();
  void ZBINCALL fsub();
  void ZBINCALL idiv();
  void ZBINCALL imod();
  void ZBINCALL cat();
  void ZBINCALL testeq();
  void ZBINCALL feq();
  void ZBINCALL fneq();
  void ZBINCALL flt();
  void ZBINCALL fgt();
  void ZBINCALL flet();
  void ZBINCALL fget();
  void ZBINCALL faddc();
  void ZBINCALL fmulc();
  void ZBINCALL yield();
};

}

#endif
