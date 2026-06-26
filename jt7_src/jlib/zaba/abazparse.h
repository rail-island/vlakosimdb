#ifndef abazparseH
#define abazparseH

#include <com/ptrto.h>


class P_Module;
P_Module *abazparse(string text);

enum NameSpace {ns_sys,ns_this,ns_locals};

//class JumpInfo;

extern int generate_int();
extern string compilingmodule;

struct ZBCFileRec {
  MEMSTROUT *code;
//  int last_label;
  ZBCFileRec() {code=NULL;}
//  int getuninum() {return ++last_label;}
};

struct CodeWriter {
  int getlabel() {return generate_int();}
  void label(int value) {getout()<<"@"<<value<<":\n";}
  void cm(string name,int parcnt) {getout()<<"cm "<<name<<" "<<parcnt<<"\n";}
  void xcm(string name,int parcnt) {getout()<<"xcm "<<name<<" "<<parcnt<<"\n";}
  void pushnum(double value) {getout()<<"pushnum "<<ftos(value)<<"\n";}
  void pushstr(string value) {getout()<<"pushstr ";getout().writecstring(value.c_str(),'"');getout()<<"\n";}
  void pushthis() {getout()<<"pushthis\n";}
  void pushnull() {getout()<<"pushnull\n";}
  void pushsys() {getout()<<"pushsys\n";}
  void pushlocals() {getout()<<"pushlocals\n";}
  void pushparam(int param) {getout()<<"pushparam "<<param<<"\n";}
  void pop() {getout()<<"pop\n";}
  void polypop(int value) {if (value==0) return;getout()<<"polypop "<<value<<"\n";}
  void condnotjmp(int label) {getout()<<"condnotjmp "<<label<<"\n";}
  void condjmp(int label) {getout()<<"condjmp "<<label<<"\n";}
  void jx(int label) {getout()<<"jx "<<label<<"\n";}
  void jnx(int label) {getout()<<"jnx "<<label<<"\n";}
  void jmp(int label) {getout()<<"jmp "<<label<<"\n";}
  void initprop(string name) {getout()<<"initprop "<<name<<"\n";}
  void getprop(string name) {getout()<<"getprop "<<name<<"\n";}
  void setprop(string name) {getout()<<"setprop "<<name<<"\n";}
  void assignprop(string name) {getout()<<"assignprop "<<name<<"\n";}
  void deleteprops(int cnt) {if (cnt==0) return;getout()<<"deleteprops "<<cnt<<"\n";}
  void dup(int value) {getout()<<"dup "<<value<<"\n";}
  void move(int src,int dest) {getout()<<"move "<<src<<" "<<dest<<"\n";}
  void xchg(int value1,int value2) {getout()<<"xchg "<<value1<<" "<<value2<<"\n";}
  void find(string flags,string value) {getout()<<"find "<<flags<<" "<<value<<"\n";}
  void doret(int cnt=1) {getout()<<"doret "<<cnt<<"\n";}
  void end() {pushnull();doret();getout()<<"end\n";}
  void join() {getout()<<"join\n";}
  void yield() {getout()<<"yield\n";}
  void pushtry(int label) {getout()<<"pushtry "<<label<<"\n";}
  void poptry() {getout()<<"poptry\n";}
  void co(string name) {getout()<<"co "<<name<<"\n";}
  void callnear(int label) {getout()<<"callnear "<<label<<"\n";}
  void retnear() {getout()<<"retnear\n";}
  void raise() {getout()<<"raise\n";}
  void xraise() {getout()<<"xraise\n";}
  void comment(STRING text) {getout()<<";"<<text<<"\n";}
  void fadd() {getout()<<"fadd\n";}
  void fsub() {getout()<<"fsub\n";}
  void fmul() {getout()<<"fmul\n";}
  void fdiv() {getout()<<"fdiv\n";}
  void idiv() {getout()<<"idiv\n";}
  void imod() {getout()<<"imod\n";}
  void feq() {getout()<<"feq\n";}
  void fneq() {getout()<<"fneq\n";}
  void flt() {getout()<<"flt\n";}
  void fgt() {getout()<<"fgt\n";}
  void flet() {getout()<<"flet\n";}
  void fget() {getout()<<"fget\n";}
  void faddc(double c) {getout()<<"faddc "<<ftos(c)<<"\n";}
  void fmulc(double c) {getout()<<"fmulc "<<ftos(c)<<"\n";}
  void cat(int cnt) {getout()<<"cat "<<cnt<<"\n";}
  void testeq() {getout()<<"testeq\n";}

  void op(string name);//operace v zavislosti na direct
  void uop(string name);//unarni operace v zavislosti na direct
  void joinnames(NameSpace importer,NameSpace imported) {pushnames(imported);pushnames(importer);join();polypop(2);}
  void findlocal(string name) {find("011",name);}
  void pushnames(NameSpace ns);
  void deletelocals(int cnt) {if (cnt==0) return;pushlocals();deleteprops(cnt);pop();}
//  void makejump(JumpInfo &ji);

  void begin(string name,int minpars,int maxpars) {getout()<<";------------------------\nsub "<<name<<" "<<minpars<<" "<<maxpars<<"\ncode\n";}
  void callthisvoidmethod(string name) {cm(name,0);pop();}
  bool getdirect() {return direct;}
  void setdirect(bool value) {direct=value;}

  CodeWriter(ZBCFileRec *zfr):zfr(zfr) {direct=false;}
private:
  ZBCFileRec *zfr;
  bool direct;
protected:
  STROUT &getout() {return *zfr->code;}
};

struct MethodToWrite {
  virtual void write_method(CodeWriter &cw)=0;
};

struct IntfWriter {
  void endintf() {getout()<<"end\n";}
  void initproc(string name,int priority) {getout()<<"initproc="<<name<<","<<priority<<"\n";}
  void autocreate() {getout()<<"autocreate=1\n";}
  void beginmtds() {getout()<<"begin\n";}
  void include(string intf) {getout()<<"include="<<intf<<"\n";}
  void creatoras(string intf) {getout()<<"creatoras="<<intf<<"\n";}
  void creator(string creator) {getout()<<"creator="<<creator<<"\n";}
  void uselib(string name) {getout()<<"uselib="<<name<<"\n";}
  void usemod(string name) {getout()<<"usemod="<<name<<"\n";}
  void require(string name) {getout()<<"require="<<name<<"\n";}

  CodeWriter &sub() {return cw;}
  void begin(string name) {getout()<<";=========================\ninterface "<<name<<"\n";}

  IntfWriter(ZBCFileRec *zfr):zfr(zfr),cw(zfr) {}
  CodeWriter cw;
  ZBCFileRec *zfr;
  STROUT &getout() {return *zfr->code;}
};

class ZBCWriter;
struct ClassToWrite {
  virtual void write_class(ZBCWriter &zw)=0;
};

struct ZBCWriter {
  void save(string file) {savetextfile(file.c_str(),getcode().c_str());}
  ZBCWriter():iw(&zfr) {zfr.code=&code;}
  IntfWriter &intf() {return iw;}

  IntfWriter iw;
  ZBCFileRec zfr;
  MEMSTROUT code;
  STROUT &getout() {return code;}
  string getcode() {return code.str();}
};

class P_Node;
struct ForEachAction {
  virtual void work(P_Node *node,P_Node *parent)=0;
};

class P_Defer;class P_Index;class P_DefOp;class P_Id;class P_Call;
class P_Function;class P_DefOpDef;class P_IndexOpDef;

struct P_Node {
  P_Node *parent;
  P_Node() {parent=NULL;}
  virtual ~P_Node() {}
  virtual P_Defer *isdefer() {return NULL;}
  virtual P_Index *isindex() {return NULL;}
  virtual P_DefOp *isdefop() {return NULL;}
  virtual P_Function *isfunction() {return NULL;}
  virtual P_Id *isid() {return NULL;}
  virtual P_Call *iscall() {return NULL;}
  virtual P_DefOpDef *isdefopdef() {return NULL;}
  virtual P_IndexOpDef *isindexopdef() {return NULL;}
  virtual string getstringvalue() {return "";}
  virtual bool isdirectpush(NameSpace &ns) {return false;}
  virtual void foreach(ForEachAction &a) {}
  virtual void getextramethods(TCntVec<MethodToWrite*> &mtds) {}
  virtual P_Function *findinnerfunc(string name,int *depth) {if (parent) return parent->findinnerfunc(name,depth);return NULL;}

  void fillparents();
};

struct P_Expr:P_Node {
  virtual void writecode(CodeWriter &cw)=0;
};

struct P_ParExpr:P_Expr {
  PtrTo<P_Expr> expr;
  void writecode(CodeWriter &cw) {expr->writecode(cw);}
  P_ParExpr(P_Expr *aexpr) {expr=aexpr;}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct JumpMaker {
  virtual void basepop(int cnt) {}
  virtual void trypop(int cnt) {}
  virtual void varpop(int cnt) {}
  virtual void callnear(int label) {}
  virtual void searched(int label) {}
};

struct BreakJumpMaker:JumpMaker {
  CodeWriter *cw;
  BreakJumpMaker(CodeWriter *cw):cw(cw) {}
  virtual void basepop(int cnt) {cw->polypop(cnt);}
  virtual void trypop(int cnt) {for(int i=0;i<cnt;i++) cw->poptry();}
  virtual void varpop(int cnt) {cw->deletelocals(cnt);}
  virtual void callnear(int label) {cw->callnear(label);}
  virtual void searched(int label) {cw->jmp(label);}
};

struct ReturnJumpMaker:JumpMaker {
  CodeWriter *cw;
  ReturnJumpMaker(CodeWriter *cw):cw(cw) {}
  virtual void callnear(int label) {cw->callnear(label);}
};


/*
struct StdJumpInfo:JumpInfo {
  int label;
  int trydhi;
  int basedhi;
  int vardhi;
  StdJumpInfo() {label=-1;trydhi=0;basedhi=0;vardhi=0;}

  void basepop(int cnt) {basedhi+=cnt;}
  void trypop(int cnt) {trydhi+=cnt;}
  void varpop(int cnt) {vardhi+=cnt;}
  void searched(int alabel) {label=alabel;}
};
*/


enum JumpType {jt_none,jt_break,jt_continue,jt_return};

struct Jump {
  CodeWriter *cw;
  JumpType type;
  string label;
  Jump(CodeWriter *acw) {cw=acw;type=jt_none;}
  bool iscontinue(string alabel) {return type==jt_continue && label==alabel;}
  bool isbreak(string alabel) {return type==jt_break && label==alabel;}
};

struct P_Id:P_Expr {
  string value;
  P_Id(string avalue) {value=avalue;}
  void writecode(CodeWriter &cw) {cw.pushlocals();cw.findlocal(value);cw.getprop(value);}
  P_Id *isid() {return this;}
  virtual string getstringvalue() {return value;}
};

struct P_Command:P_Node {
//  int stackhi;
//  int trystackhi;
//  int varstackhi;
  P_Command *comparent;
  string label;
  int breaklab;
  int getbreak() {if (breaklab<0) breaklab=generate_int();return breaklab;}

//  P_Command() {stackhi=trystackhi=varstackhi=0;parent=NULL;}
  P_Command() {comparent=NULL;breaklab=-1;}
  void setlabel(P_Id *alabel) {label=alabel->value;delete alabel;}
  void writefoot(CodeWriter &cw);

  virtual void writecode(CodeWriter &cw)=0;

  virtual int varstackdhi() {return 0;}
  virtual void countstackhis() {}

  virtual void makejump(Jump &jump,JumpMaker &jm,P_Node *child);
};

struct P_Directly:P_Command {
  PtrTo<P_Command> command;
  bool direct;
  P_Directly(bool adirect,P_Command *acommand) {direct=adirect;command=acommand;}
  void foreach(ForEachAction &a) {a.work(command,this);}
  void writecode(CodeWriter &cw);
  int varstackdhi() {return command->varstackdhi();}
};

struct P_DirectCommand:P_Command {
  bool direct;
  P_DirectCommand(bool adirect) {direct=adirect;}
  void writecode(CodeWriter &cw) {cw.setdirect(direct);}
};

struct P_SelectElse:P_Node {
  PtrTo<P_Command> command;
  P_SelectElse(P_Command *acommand) {command=acommand;}
  void foreach(ForEachAction &a) {a.work(command,this);}
};

struct P_SelectItem:P_Node {
  PtrTo<P_Expr> expr;
  PtrTo<P_Command> command;
  P_SelectItem(P_Expr *aexpr,P_Command *acommand) {expr=aexpr;command=acommand;}
  void writecode(CodeWriter &cw,int end);
  void foreach(ForEachAction &a) {a.work(command,this);a.work(expr,this);}
};

struct P_SelectItems:P_Node {
  IObjArray<P_SelectItem> items;
  void additem(P_SelectItem *item) {items.add(item);}
  int count() {return items.count;}
  void foreach(ForEachAction &a) {for(int i=0;i<items.count;i++) a.work(items[i],this);}
};

struct P_Select:P_Command {
  PtrTo<P_Expr> expr;
  PtrTo<P_SelectItems> items;
  PtrTo<P_SelectElse> selectelse;
  P_Select(P_Expr *aexpr,P_SelectItems *aitems,P_SelectElse *aselectelse) {expr=aexpr;items=aitems;selectelse=aselectelse;}
  void writecode(CodeWriter &cw);
  void makejump(Jump &jump,JumpMaker &jm,P_Node *child) {jm.basepop(1);P_Command::makejump(jump,jm,child);}
  void foreach(ForEachAction &a) {a.work(expr,this);a.work(items,this);if (selectelse) a.work(selectelse,this);}
};

struct P_InspectElse:P_Node {
  PtrTo<P_Command> command;
  P_InspectElse(P_Command *acommand) {command=acommand;}
  void foreach(ForEachAction &a) {a.work(command,this);}
};

struct P_InspectItem:P_Node {
  PtrTo<P_Expr> expr;
  PtrTo<P_Command> command;
  P_InspectItem(P_Expr *aexpr,P_Command *acommand) {expr=aexpr;command=acommand;}
  void writecode(CodeWriter &cw,int end);
  void foreach(ForEachAction &a) {a.work(command,this);a.work(expr,this);}
};

struct P_InspectItems:P_Node {
  IObjArray<P_InspectItem> items;
  void additem(P_InspectItem *item) {items.add(item);}
  int count() {return items.count;}
  void foreach(ForEachAction &a) {for(int i=0;i<items.count;i++) a.work(items[i],this);}
};

struct P_Inspect:P_Command {
  PtrTo<P_Expr> expr;
  PtrTo<P_InspectItems> items;
  PtrTo<P_InspectElse> inspectelse;
  P_Inspect(P_Expr *aexpr,P_InspectItems *aitems,P_InspectElse *ainspectelse) {expr=aexpr;items=aitems;inspectelse=ainspectelse;}
  void writecode(CodeWriter &cw);
  void makejump(Jump &jump,JumpMaker &jm,P_Node *child) {jm.basepop(1);P_Command::makejump(jump,jm,child);}
  void foreach(ForEachAction &a) {a.work(expr,this);a.work(items,this);if (inspectelse) a.work(inspectelse,this);}
};

class P_SwitchCommand;class P_SwitchCase;class P_SwitchDefault;
struct P_SwitchItem:P_Node {
  virtual P_SwitchDefault *isdefault() {return NULL;}
  virtual P_SwitchCase *iscase() {return NULL;}
  virtual P_SwitchCommand *iscommand() {return NULL;}
  virtual void setparent(P_Command *sw) {}
};

struct P_SwitchCommand:P_SwitchItem {
  PtrTo<P_Command> command;
  P_SwitchCommand(P_Command *acommand) {command=acommand;}
  P_SwitchCommand *iscommand() {return this;}
  void setparent(P_Command *sw) {command->comparent=sw;}
  void foreach(ForEachAction &a) {a.work(command,this);}
};

struct P_SwitchCase:P_SwitchItem {
  PtrTo<P_Expr> expr;
  P_SwitchCase(P_Expr *aexpr) {expr=aexpr;}
  P_SwitchCase *iscase() {return this;}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_SwitchDefault:P_SwitchItem {
  P_SwitchDefault *isdefault() {return this;}
};

struct P_SwitchItems:P_Node {
  IObjArray<P_SwitchItem> items;
  P_SwitchItems() {wasdef=false;}
  bool wasdef;
  void additem(P_SwitchItem *item);
  int count() {return items.count;}
  P_SwitchItem *item(int index) {return items[index];}
  void setparent(P_Command *sw) {for(int i=0;i<items.count;i++) items[i]->setparent(sw);}
  void foreach(ForEachAction &a) {for(int i=0;i<items.count;i++) a.work(items[i],this);}
};

struct P_Switch:P_Command {
  PtrTo<P_Expr> expr;
  PtrTo<P_SwitchItems> items;
  P_Switch(P_Expr *aexpr,P_SwitchItems *aitems) {expr=aexpr;items=aitems;items->setparent(this);}
  void writecode(CodeWriter &cw);
  void makejump(Jump &jump,JumpMaker &jm,P_Node *child);
  void foreach(ForEachAction &a) {a.work(expr,this);a.work(items,this);}
};


struct P_VarExpr:P_Node {
  virtual void writecode(CodeWriter &cw)=0;
  virtual int varstackdhi() {return 0;}
};

struct P_PushThis:P_Expr {
  void writecode(CodeWriter &cw) {cw.pushthis();}
  bool isdirectpush(NameSpace &ns) {ns=ns_this;return true;}
};

struct P_PushSys:P_Expr {
  void writecode(CodeWriter &cw) {cw.pushsys();}
  bool isdirectpush(NameSpace &ns) {ns=ns_sys;return true;}
};

struct P_PushFalse:P_Expr {
  void writecode(CodeWriter &cw) {cw.pushnull();}
};

struct P_PushTrue:P_Expr {
  void writecode(CodeWriter &cw) {cw.pushsys();}
};
struct P_PushNull:P_Expr {
  void writecode(CodeWriter &cw) {cw.pushnull();}
};


struct P_IdVarExpr:P_VarExpr {
  string id;
  PtrTo<P_Expr> expr;
  P_IdVarExpr(P_Id *aid,P_Expr *aexpr) {expr=aexpr;id=aid->value;delete aid;}
  void writecode(CodeWriter &cw) {expr->writecode(cw);cw.pushlocals();cw.initprop(id);cw.pop();}
  int varstackdhi() {return 1;}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_If:P_Command {
  PtrTo<P_VarExpr> expr;
  PtrTo<P_Command> command;
  P_If(P_VarExpr *aexpr,P_Command *acommand) {expr=aexpr;command=acommand;command->comparent=this;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(expr,this);a.work(command,this);}
};

struct P_IfElse:P_Command {
  PtrTo<P_VarExpr> expr;
  PtrTo<P_Command> command1,command2;
  P_IfElse(P_VarExpr *aexpr,P_Command *acommand1,P_Command *acommand2) {expr=aexpr;command1=acommand1;command2=acommand2;command1->comparent=this;command2->comparent=this;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(expr,this);a.work(command1,this);a.work(command2,this);}
};


struct P_ExprCom:P_Command {
  PtrTo<P_Expr> expr;
  P_ExprCom(P_Expr *aexpr) {expr=aexpr;}
  void writecode(CodeWriter &cw) {expr->writecode(cw);cw.pop();writefoot(cw);}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_Commands:P_Node {
  IObjArray<P_Command> coms;
  void addcommand(P_Command *com) {coms.add(com);}
  void setparent(P_Command *aparent) {for(int i=0;i<coms.count;i++) coms[i]->comparent=aparent;}
  int varstackdhi();
  void writecode(CodeWriter &cw) {for(int i=0;i<coms.count;i++) coms[i]->writecode(cw);}
  P_Commands() {}
  void foreach(ForEachAction &a) {for(int i=0;i<coms.count;i++) a.work(coms[i],this);}
};

struct P_Block:P_Command {
  PtrTo<P_Commands> coms;
  void writecode(CodeWriter &cw);
  P_Block(P_Commands *acoms) {coms=acoms;coms->setparent(this);}

  int varstackdhi() {return 0;}
  void makejump(Jump &jump,JumpMaker &jm,P_Node *child);
  void foreach(ForEachAction &a) {a.work(coms,this);}
};

struct P_Number:P_Expr {
  double value;
  P_Number(double avalue) {value=avalue;}
  void writecode(CodeWriter &cw) {cw.pushnum(value);}
};

struct P_String:P_Expr {
  string value;
  P_String(string avalue) {value=avalue;}
  void writecode(CodeWriter &cw) {cw.pushstr(value);}
  virtual string getstringvalue() {return value;}
};

struct P_RegExp:P_Expr {
  string value;
  P_RegExp(string avalue) {value=avalue;}
  void writecode(CodeWriter &cw);
};

struct P_Operator:P_Expr {
  PtrTo<P_Expr> left,right;
  string type;
  P_Operator(P_Expr *aleft,P_Expr *aright,string atype) {left=aleft;right=aright;type=atype;}
  void writecode(CodeWriter &cw) {right->writecode(cw);left->writecode(cw);cw.op(type);}
  void foreach(ForEachAction &a) {a.work(left,this);a.work(right,this);}
};

struct P_Is:P_Expr {
  PtrTo<P_Expr> left,right;
  string type;
  P_Is(P_Expr *aleft,P_Expr *aright) {left=aleft;right=aright;}
  void writecode(CodeWriter &cw) {right->writecode(cw);left->writecode(cw);cw.cm("isInstanceOf",1);}
  void foreach(ForEachAction &a) {a.work(left,this);a.work(right,this);}
};

struct P_TestEq:P_Expr {
  PtrTo<P_Expr> left,right;
  P_TestEq(P_Expr *aleft,P_Expr *aright) {left=aleft;right=aright;}
  void writecode(CodeWriter &cw) {right->writecode(cw);left->writecode(cw);cw.testeq();}
  void foreach(ForEachAction &a) {a.work(left,this);a.work(right,this);}
};

struct P_Minus:P_Expr {
  PtrTo<P_Expr> expr;
  P_Minus(P_Expr *aexpr) {expr=aexpr;}
  void writecode(CodeWriter &cw) {expr->writecode(cw);cw.cm("op:minus",0);}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_Delegate:P_Expr {
  string name;
  PtrTo<P_Expr> expr;
  P_Delegate(P_Id *aname,P_Expr *aexpr) {name=aname->value;delete aname;expr=aexpr;}
  void writecode(CodeWriter &cw) {cw.pushstr(name);expr->writecode(cw);cw.pushsys();cw.cm("createDelegate",2);}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_StoreItem:P_Node {
  string name;
  PtrTo<P_Expr> expr;
  P_StoreItem(P_Node *aname,P_Expr *aexpr) {expr=aexpr;name=aname->getstringvalue();delete aname;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_StoreItems:P_Node {
  IObjArray<P_StoreItem> items;
  void additem(P_StoreItem *item) {items.add(item);}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {for(int i=0;i<items.count;i++) a.work(items[i],this);}
};

struct P_NewStore:P_Expr {
  PtrTo<P_StoreItems> items;
  P_NewStore(P_StoreItems *aitems) {items=aitems;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(items,this);}
};

struct P_SimpleVarExpr:P_VarExpr {
  PtrTo<P_Expr> expr;
  P_SimpleVarExpr(P_Expr *aexpr) {expr=aexpr;}
  void writecode(CodeWriter &cw) {expr->writecode(cw);}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_Return:P_Command {
  void makefinally(CodeWriter &cw);
};

struct P_RetVoid:P_Return {
  void writecode(CodeWriter &cw) {makefinally(cw);cw.pushnull();cw.doret();}
};

struct P_RetValue:P_Return {
  PtrTo<P_Expr> expr;
  P_RetValue(P_Expr *aexpr) {expr=aexpr;}
  void writecode(CodeWriter &cw) {makefinally(cw);expr->writecode(cw);cw.doret();writefoot(cw);}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_YieldVoid:P_Command {
  void writecode(CodeWriter &cw) {cw.pushnull();cw.yield();}
};

struct P_YieldValue:P_Command {
  PtrTo<P_Expr> expr;
  P_YieldValue(P_Expr *aexpr) {expr=aexpr;}
  void writecode(CodeWriter &cw) {expr->writecode(cw);cw.yield();}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};


struct P_Throw:P_Command {
  PtrTo<P_Expr> expr;
  void writecode(CodeWriter &cw) {expr->writecode(cw);cw.raise();writefoot(cw);}
  P_Throw(P_Expr *aexpr) {expr=aexpr;}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_BreakContinue:P_Command {
  string label;
  void writecode(CodeWriter &cw);
  virtual JumpType getjumptype()=0;
  P_BreakContinue(P_Id *alabel) {if (alabel) {label=alabel->value;delete alabel;} }
};

struct P_Break:P_BreakContinue {
  JumpType getjumptype() {return jt_break;}
  P_Break(P_Id *alabel=NULL):P_BreakContinue(alabel) {}
};

struct P_Continue:P_BreakContinue {
  JumpType getjumptype() {return jt_continue;}
  P_Continue(P_Id *alabel=NULL):P_BreakContinue(alabel) {}
};

struct P_For:P_Command {
  PtrTo<P_VarExpr> init;
  PtrTo<P_Expr> cond;
  PtrTo<P_Expr> inc;
  PtrTo<P_Command> body;
  P_For(P_VarExpr *ainit,P_Expr *acond,P_Expr *ainc,P_Command *abody) {
    init=ainit;cond=acond;inc=ainc;body=abody;body->comparent=this;
    contlab=-1;
  }
  void writecode(CodeWriter &cw);
  void makejump(Jump &jump,JumpMaker &jm,P_Node *child);
  void foreach(ForEachAction &a) {a.work(init,this);a.work(cond,this);a.work(inc,this);a.work(body,this);}
  int contlab;
};

struct P_TryCatch:P_Command {
  PtrTo<P_Command> safeblock,catchblock;
  string name;
  P_TryCatch(P_Command *asafeblock,P_Id *aname,P_Command *acatchblock) {
    safeblock=asafeblock;name=aname->value;delete aname;catchblock=acatchblock;
    safeblock->comparent=this;catchblock->comparent=this;
  }
  void writecode(CodeWriter &cw);
  void makejump(Jump &jump,JumpMaker &jm,P_Node *child);
  void foreach(ForEachAction &a) {a.work(safeblock,this);a.work(catchblock,this);}
};

struct P_TryFinally:P_Command {
  PtrTo<P_Command> safeblock,finallyblock;
  int nearlabel;
  P_TryFinally(P_Command *asafeblock,P_Command *afinallyblock) {
    safeblock=asafeblock;finallyblock=afinallyblock;
    safeblock->comparent=this;finallyblock->comparent=this;
    nearlabel=generate_int();
  }
  void writecode(CodeWriter &cw);
  void makejump(Jump &jump,JumpMaker &jm,P_Node *child);
  void foreach(ForEachAction &a) {a.work(safeblock,this);a.work(finallyblock,this);}
};


struct P_ForEach:P_Command {
  string id;
  PtrTo<P_Command> body;
  PtrTo<P_Expr> expr;
  P_ForEach(P_Id *aid,P_Expr *aexpr,P_Command *abody) {
    id=aid->value;delete aid;expr=aexpr;body=abody;body->comparent=this;
    contlab=-1;
  }
  void writecode(CodeWriter &cw);
  void makejump(Jump &jump,JumpMaker &jm,P_Node *child);
  int contlab;
  void foreach(ForEachAction &a) {a.work(expr,this);a.work(body,this);}
};

struct P_While:P_Command {
  PtrTo<P_VarExpr> cond;
  PtrTo<P_Command> body;
  P_While(P_VarExpr *acond,P_Command *abody) {
    cond=acond;body=abody;body->comparent=this;
    contlab=-1;
  }
  void makejump(Jump &jump,JumpMaker &jm,P_Node *child);
  void writecode(CodeWriter &cw);
  int contlab;
  void foreach(ForEachAction &a) {a.work(cond,this);a.work(body,this);}
};

struct P_DoWhile:P_Command {
  PtrTo<P_Expr> cond;
  PtrTo<P_Command> body;
  P_DoWhile(P_Expr *acond,P_Command *abody) {
    cond=acond;body=abody;body->comparent=this;
    contlab=-1;
  }
  void makejump(Jump &jump,JumpMaker &jm,P_Node *child);
  void writecode(CodeWriter &cw);
  int contlab;
  void foreach(ForEachAction &a) {a.work(cond,this);a.work(body,this);}
};

struct P_Defer:P_Expr {
  string name;
  PtrTo<P_Expr> obj;
  P_Defer(P_Expr *aobj,P_Id *aname) {obj=aobj;name=aname->value;delete aname;}
  void writecode(CodeWriter &cw);
  P_Defer *isdefer() {return this;}
  void foreach(ForEachAction &a) {a.work(obj,this);}
};

struct P_Params:P_Node {
  IObjArray<P_Expr> params;
  void addparam(P_Expr *param) {params.add(param);}
  P_Params() {}
  P_Params(P_Expr *param) {params.add(param);}
  void writecode(CodeWriter &cw);
  int count() {return params.count;}
  void foreach(ForEachAction &a) {for(int i=0;i<params.count;i++) a.work(params[i],this);}
};

struct P_Call:P_Expr {
  PtrTo<P_Expr> fn;
  PtrTo<P_Params> params;
  P_Call(P_Expr *afn,P_Params *aparams) {fn=afn;params=aparams;}
  void writecode(CodeWriter &cw);
  P_Call *iscall() {return this;}
  void foreach(ForEachAction &a) {a.work(fn,this);a.work(params,this);}
};

struct P_InlineArray:P_Expr {
  PtrTo<P_Params> params;
  P_InlineArray(P_Params *aparams) {params=aparams;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(params,this);}
};

struct P_Index:P_Expr {
  PtrTo<P_Expr> array;
  PtrTo<P_Params> params;
  P_Index(P_Expr *aarray,P_Params *aparams) {array=aarray;params=aparams;}
  void writecode(CodeWriter &cw);
  P_Index *isindex() {return this;}
  void foreach(ForEachAction &a) {a.work(array,this);a.work(params,this);}
};

struct P_New:P_Expr {
  PtrTo<P_Expr> cl;
  PtrTo<P_Params> params;
  P_New(P_Expr *acl,P_Params *aparams) {cl=acl;params=aparams;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(cl,this);a.work(params,this);}
};

struct P_Detach:P_Expr {
  string name;
  PtrTo<P_Expr> obj;
  PtrTo<P_Params> params;
  P_Detach(P_Expr *aobj,P_Id *aname,P_Params *aparams) {obj=aobj;params=aparams;name=aname->value;delete aname;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(obj,this);a.work(params,this);}
};

struct P_UnaryModify:P_Expr {
  virtual bool isprefix()=0;
  string type;
  PtrTo<P_Expr> expr;
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_PreUn:P_UnaryModify {
  P_PreUn(P_Expr *aexpr,string atype) {expr=aexpr;type=atype;}
  bool isprefix() {return true;}
};

struct P_PostUn:P_UnaryModify {
  P_PostUn(P_Expr *aexpr,string atype) {expr=aexpr;type=atype;}
  bool isprefix() {return false;}
};

struct P_DefOp:P_Expr {
  PtrTo<P_Expr> expr;
  string type;
  string prop;
  P_DefOp(P_Expr *aexpr,P_Node *aprop,string atype) {expr=aexpr;type=atype;prop=aprop->getstringvalue();delete aprop;}
  void writecode(CodeWriter &cw);
  P_DefOp *isdefop() {return this;}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_CommonComExpr:P_Expr,MethodToWrite {
  string mtdid;
  void writecode(CodeWriter &cw);
  void writejoins(CodeWriter &cw);
  void getextramethods(TCntVec<MethodToWrite*> &mtds) {mtds.add(this);}
  P_CommonComExpr() {mtdid=string("@inline")+itos(generate_int())+":"+compilingmodule;}
};

struct P_ComExpr:P_CommonComExpr {
  PtrTo<P_Command> command;
  P_ComExpr(P_Command *acommand) {command=acommand;}
  void foreach(ForEachAction &a) {a.work(command,this);}
  void write_method(CodeWriter &cw);
};

struct P_RetComExpr:P_CommonComExpr {
  PtrTo<P_Expr> expr;
  P_RetComExpr(P_Expr *aexpr) {expr=aexpr;}
  void foreach(ForEachAction &a) {a.work(expr,this);}
  void write_method(CodeWriter &cw);
};

struct P_CondExpr:P_Expr {
  PtrTo<P_Expr> cond,yes,no;
  P_CondExpr(P_Expr *acond,P_Expr *ayes,P_Expr *ano) {cond=acond;yes=ayes;no=ano;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(cond,this);a.work(yes,this);a.work(no,this);}
};

struct P_And:P_Expr {
  PtrTo<P_Expr> left,right;
  P_And(P_Expr *aleft,P_Expr *aright) {left=aleft;right=aright;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(left,this);a.work(right,this);}
};

struct P_Or:P_Expr {
  PtrTo<P_Expr> left,right;
  P_Or(P_Expr *aleft,P_Expr *aright) {left=aleft;right=aright;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(left,this);a.work(right,this);}
};

struct P_Not:P_Expr {
  PtrTo<P_Expr> expr;
  P_Not(P_Expr *aexpr) {expr=aexpr;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_Assign:P_Expr {
  PtrTo<P_Expr> left,right;
  P_Assign(P_Expr *aleft,P_Expr *aright) {left=aleft;right=aright;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(left,this);a.work(right,this);}
};

struct P_AssignOp:P_Expr {
  PtrTo<P_Expr> left,right;
  string type;
  P_AssignOp(P_Expr *aleft,P_Expr *aright,string atype) {left=aleft;right=aright;type=atype;}
  void writecode(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(left,this);a.work(right,this);}
};

struct P_VarDef:P_Node {
  string name;
  virtual void writecode(CodeWriter &cw,NameSpace ns)=0;
};

struct P_VarDefInit:P_VarDef {
  PtrTo<P_Expr> expr;
  P_VarDefInit(P_Id *aname,P_Expr *aexpr) {name=aname->value;delete aname;expr=aexpr;}
  void writecode(CodeWriter &cw,NameSpace ns) {expr->writecode(cw);cw.pushnames(ns);cw.initprop(name);cw.polypop(2);}
  void foreach(ForEachAction &a) {a.work(expr,this);}
};

struct P_VarDefVoid:P_VarDef {
  P_VarDefVoid(P_Id *aname) {name=aname->value;delete aname;}
  void writecode(CodeWriter &cw,NameSpace ns) {cw.pushnull();cw.pushnames(ns);cw.initprop(name);cw.polypop(2);}
};

struct P_VarDefs:P_Node {
  IObjArray<P_VarDef> vardefs;
  void addvardef(P_VarDef *value) {vardefs.add(value);}
  P_VarDefs(P_VarDef *vardef) {vardefs.add(vardef);}
  void writecode(CodeWriter &cw,NameSpace ns) {for(int i=0;i<vardefs.count;i++) vardefs[i]->writecode(cw,ns);}
  int count() {return vardefs.count;}
  P_VarDef *item(int index) {return vardefs[index];}
  void foreach(ForEachAction &a) {for(int i=0;i<vardefs.count;i++) a.work(vardefs[i],this);}
};

struct P_VarCommand:P_Command {
  PtrTo<P_VarDefs> defs;
  P_VarCommand(P_VarDefs *adefs) {defs=adefs;}
  void writecode(CodeWriter &cw) {defs->writecode(cw,ns_locals);}
  int varstackdhi() {return defs->vardefs.count;}
  void foreach(ForEachAction &a) {a.work(defs,this);}
};

struct P_ModuleElem:P_Node {
  virtual void writeinit(CodeWriter &cw) {}
  virtual void writemethods(IntfWriter &zw) {}
  virtual void getsubclasses(TCntVec<ClassToWrite*> &classes) {}
  virtual void createclasscode(CodeWriter &cw) {}
  virtual void makeislistcode(CodeWriter &cw) {}
  virtual void writeintfprops(IntfWriter &zw) {}
};

struct P_IncludeModule:P_ModuleElem {
  string name;
  P_IncludeModule(P_Id *aid) {name=aid->value;delete aid;}
  void writeinit(CodeWriter &cw);
  void writeintfprops(IntfWriter &zw) {zw.include(name+".module.includable");}
};

struct P_Constructor;class P_ClassElems;class P_Class;
struct P_ClassElem:P_ModuleElem {
  P_ClassElems *elems;
  P_ClassElem() {elems=NULL;}
//  virtual void writeinit(CodeWriter &cw) {}
//  virtual void writemethods(IntfWriter &zw) {}
  virtual void writestaticmethods(IntfWriter &zw) {}
//  virtual void getsubclasses(TCntVec<ClassToWrite*> &classes) {}
  virtual P_Constructor *isconstructor() {return NULL;}
};

struct P_ElemCommand:P_ClassElem {
  PtrTo<P_Command> command;
  P_ElemCommand(P_Command *acommand) {command=acommand;}
  void writeinit(CodeWriter &cw) {command->writecode(cw);}
  void foreach(ForEachAction &a) {a.work(command,this);}
};

struct P_FldDef:P_ClassElem {
  PtrTo<P_VarDefs> defs;
  P_FldDef(P_VarDefs *adefs) {defs=adefs;}
  void writeinit(CodeWriter &cw) {defs->writecode(cw,ns_this);}
  void foreach(ForEachAction &a) {a.work(defs,this);}
};

struct P_Argument:P_Node {
  int index;string name;
  P_Argument(P_Id *aname) {name=aname->value;delete aname;index=-1;}
  void writedef(CodeWriter &cw);
  P_Argument *clone() {P_Argument *res=new P_Argument(new P_Id(name));res->index=index;return res;}
};

struct P_ArgList:P_Node {
  IObjArray<P_Argument> args;
  void addarg(P_Argument *arg) {arg->index=args.count;args.add(arg);}
  P_ArgList() {}
  void writedefs(CodeWriter &cw);
  int mincnt() {return args.count;}
  int maxcnt() {return args.count;}
  int realcnt() {return args.count;}
  void foreach(ForEachAction &a) {for(int i=0;i<args.count;i++) a.work(args[i],this);}
  P_ArgList *clone() {P_ArgList *res=new P_ArgList();for(int i=0;i<args.count;i++) res->args.add(args[i]->clone());return res; }
};

class P_Function;
struct P_FuncElems:P_Node {
  PtrTo<P_Block> block;
  IObjArray<P_Function> fns;
  P_FuncElems() {block=new P_Block(new P_Commands());}
  void addcommand(P_Command *com) {block->coms->addcommand(com);}
  void addfunc(P_Function *func) {fns.add(func);}
  void foreach(ForEachAction &a);
  void getextramethods(TCntVec<MethodToWrite*> &mtds);
};

class P_FuncElems;
//MethodToWrite jen pro vnitrni fce, ostatni jsou psano pomoci writemethods
struct P_Function:P_ClassElem,MethodToWrite {
  string name;
  string getfullname() {if (v_fullname!="") return v_fullname;makefullname();return v_fullname;}
  P_Function *funcparent() {return parent->parent->isfunction();}
  PtrTo<P_ArgList> args;
  PtrTo<P_FuncElems> elems;
  P_Function(P_Id *aname,P_ArgList *aargs,P_FuncElems *aelems) {
    name=aname->value;delete aname;args=aargs;elems=aelems;
  }
  P_Function *isfunction() {return this;}
  void writemethods(IntfWriter &zw) {writecode(zw.sub());}
  void writestaticmethods(IntfWriter &zw) {}
  void foreach(ForEachAction &a) {a.work(args,this);a.work(elems,this);}
  void write_method(CodeWriter &cw);
  P_Function *findinnerfunc(string name,int *depth);
private:
  void writecode(CodeWriter &zw);
  string v_fullname;
  void makefullname() {v_fullname=name;P_Function *p=funcparent();if (p) v_fullname=p->getfullname()+"::"+v_fullname;}
};

inline P_Function *makefn0(string funcname,P_FuncElems *elems) {
  P_ArgList *args=new P_ArgList();
  return new P_Function(new P_Id(funcname),args,elems);
}
inline P_Function *makefn1(string funcname,P_Id *argname,P_FuncElems *elems) {
  P_ArgList *args=new P_ArgList();
  args->addarg(new P_Argument(argname));
  return new P_Function(new P_Id(funcname),args,elems);
}
inline P_Function *makefn2(string funcname,P_Id *argname1,P_Id *argname2,P_FuncElems *elems) {
  P_ArgList *args=new P_ArgList();
  args->addarg(new P_Argument(argname1));
  args->addarg(new P_Argument(argname2));
  return new P_Function(new P_Id(funcname),args,elems);
}


class P_ModuleElems;
struct P_OperGet:P_Node {
  P_FuncElems *elems;
  P_OperGet(P_FuncElems *aelems) {elems=aelems;}
};
typedef P_OperGet P_OperSet;
struct P_DefOpDef:P_ClassElem {
  string type;
  PtrTo<P_OperGet> get;
  PtrTo<P_OperSet> set;
  PtrTo<P_Id> parname;
  P_DefOpDef(string atype,P_Id *aparname,P_OperGet *aget,P_OperSet *aset) {get=aget;set=aset;parname=aparname;type=atype;}
  P_DefOpDef *isdefopdef() {return this;}
  void addelems(P_ModuleElems *elems);
};

struct P_IndexOpDef:P_ClassElem {
  PtrTo<P_OperGet> get;
  PtrTo<P_OperSet> set;
  PtrTo<P_ArgList> arglist;
  P_IndexOpDef(P_ArgList *aarglist,P_OperGet *aget,P_OperSet *aset) {get=aget;set=aset;arglist=aarglist;}
  P_IndexOpDef *isindexopdef() {return this;}
  void addelems(P_ModuleElems *elems);
};

struct P_Import:P_ClassElem {
  PtrTo<P_Expr> expr;
  P_Import(P_Expr *aexpr) {expr=aexpr;}
  void foreach(ForEachAction &a) {a.work(expr,this);}
  void writeinit(CodeWriter &cw) {expr->writecode(cw);cw.pushthis();cw.join();cw.polypop(2);}
};

struct P_Readonly:P_ClassElem {
  PtrTo<P_VarDefs> vardefs;
  P_Readonly(P_VarDefs *avardefs) {vardefs=avardefs;}
  void writemethods(IntfWriter &zw);
  void writeinit(CodeWriter &cw);
  void foreach(ForEachAction &a) {a.work(vardefs,this);}
};

struct P_Readwrite:P_Readonly {
  P_Readwrite(P_VarDefs *avardefs):P_Readonly(avardefs) {}
  void writemethods(IntfWriter &zw);
};

struct P_GetPropSpec:P_Node {
  PtrTo<P_Block> block;
  P_GetPropSpec(P_Block *ablock=NULL) {block=ablock;}
  void writemethod(CodeWriter &cw,string name);
  void foreach(ForEachAction &a) {if (block) a.work(block,this);}
};

struct P_SetPropSpec:P_Node {
  PtrTo<P_Block> block;
  P_SetPropSpec(P_Block *ablock=NULL) {block=ablock;}
  void writemethod(CodeWriter &cw,string name);
  void foreach(ForEachAction &a) {if (block) a.work(block,this);}
};

struct P_Property:P_ClassElem {
  string name;
  PtrTo<P_GetPropSpec> get;
  PtrTo<P_SetPropSpec> set;
  P_Property(P_Id *aid,P_GetPropSpec *aget,P_SetPropSpec *aset) {
    name=aid->value;delete aid;get=aget;set=aset;
  }
  void writemethods(IntfWriter &zw);
  void foreach(ForEachAction &a) {a.work(get,this);a.work(set,this);}
};

class P_ClassElems;
struct P_Constructor:P_ClassElem {
  PtrTo<P_ArgList> args;
  PtrTo<P_Command> code;
  P_Constructor(P_ArgList *aargs,P_Command *acode) {
    args=aargs;code=acode;
  }
  P_Constructor *isconstructor() {return this;}
  void writeinstanceinit(IntfWriter &iw,P_ClassElems *elems,P_Class *cl);
  void writeclassinit(IntfWriter &iw,P_ClassElems *elems);
  void foreach(ForEachAction &a) {a.work(args,this);a.work(code,this);}
};

struct P_Ancestor:P_Node {
  string id;
  P_Ancestor(P_Id *aid) {if (aid) {id=aid->value;delete aid;}}
};

struct P_ModuleElems:P_Node {
  IObjArray<P_ModuleElem> elems;
  int count() {return elems.count;}
  P_ModuleElems() {}
  void addelem(P_ModuleElem *elem) {
    if (P_DefOpDef *dod=elem->isdefopdef()) dod->addelems(this);
    else if (P_IndexOpDef *iod=elem->isindexopdef()) iod->addelems(this);
    else elems.add(elem);
  }
  void writeinit(CodeWriter &cw);
  void writemethods(IntfWriter &zw);
  void getsubclasses(TCntVec<ClassToWrite*> &classes);
  P_ModuleElem *item(int index) {return elems[index];}
  void foreach(ForEachAction &a) {for(int i=0;i<elems.count;i++) a.work(elems[i],this);}
  void findmethods(TCntVec<MethodToWrite*> &mtds);
  void writeintfprops(IntfWriter &zw) {for(int i=0;i<elems.count;i++) elems[i]->writeintfprops(zw);}
};

struct P_ClassElems:P_ModuleElems {
//  IObjArray<P_ClassElem> elems;
  P_Class *clazz;
  P_ClassElems() {clazz=NULL;}
//  int count() {return elems.count;}
  void addelem(P_ClassElem *elem) {P_ModuleElems::addelem(elem);elem->elems=this;}
//  void writeinit(CodeWriter &cw);
//  void writemethods(IntfWriter &zw);
  void writestaticmethods(IntfWriter &zw);
  P_ClassElem *item(int index) {return (P_ClassElem*)elems[index];}
//  void getsubclasses(TCntVec<ClassToWrite*> &classes);
};

/*
struct CommonClass:ClassToWrite {
};
*/

struct P_Class:P_ClassElem,ClassToWrite {
  string name;
  string super;
  PtrTo<P_Ancestor> ancestor;
  PtrTo<P_ClassElems> elems;
  P_Class(P_Id *aname,P_Ancestor *aancestor,P_ClassElems *aelems) {
    name=aname->value;delete aname;ancestor=aancestor;elems=aelems;
    elems->clazz=this;
    super="Object";
    if (ancestor->id!="") super=ancestor->id;
    if (name=="Object") super="";
  }
  void write_class(ZBCWriter &zw);
  void getsubclasses(TCntVec<ClassToWrite*> &classes) {
    classes.add(this);elems->getsubclasses(classes);
  }
  void createclasscode(CodeWriter &cw);//this je parent namespace
  void foreach(ForEachAction &a) {a.work(elems,this);a.work(ancestor,this);}
  void makeislistcode(CodeWriter &cw);

/*  P_Class(P_Id *aname,P_Ancestor *aancestor,P_ClassElems *aelems):CommonClass(aname,aancestor,aelems) {
    aelems->clazz=this;
  }*/
};

/*
struct P_SubClass:P_ClassElem,CommonClass {
  void getsubclasses(TCntVec<ClassToWrite*> &classes) {classes.add(this);}
  P_SubClass(P_Id *aname,P_Ancestor *aancestor,P_ClassElems *aelems):CommonClass(aname,aancestor,aelems) {}
};
*/

struct P_Usings:P_Node {
  Strings libs,mods,reqmods;
  void uselib(P_String *lib) {libs.add(lib->value);delete lib;}
  void usemod(P_String *mod) {mods.add(mod->value);delete mod;}
  void reqmod(P_Id *mod) {reqmods.add(mod->value);delete mod;}
  void write(IntfWriter iw);
};

struct P_ModuleName:P_Node {
  string name;
  int initlevel;
  P_ModuleName(P_Id *aname,int ainitlevel) {if (aname) {name=aname->value;delete aname;}initlevel=ainitlevel;compilingmodule=name;}
};

struct P_Module:P_Node {
  PtrTo<P_ModuleName> name;
  PtrTo<P_ModuleElems> elems;
  PtrTo<P_Usings> usings;
  P_Module(P_ModuleName *aname,P_Usings *ausings,P_ModuleElems *aelems) {elems=aelems;name=aname;usings=ausings;}
  void write(ZBCWriter &zw,string name0);//provede pripravy a zapise kod modulu
  void foreach(ForEachAction &a) {a.work(name,this);a.work(elems,this);}
  //je volano hned po vyvoreni P_Module
};

#endif
