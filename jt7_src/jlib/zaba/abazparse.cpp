#include <jlib.h>
#pragma hdrstop

#include "abazparse.h"
#include <gram/gmanal.h>
#include <ext/regexintf.h> 
#include <typeinfo>

//#define retype(type,expr) ( (P_##type*) (expr) )
//#define retype(type,expr) ( &dynamic_cast<P_##type&>(*(expr)) )

string compilingmodule;

template <class T> T *p_retype(P_Node *node) {
  if (node==NULL) return NULL;
  T *res=dynamic_cast<T*>(node);
  if (!res) {
    printf("Want %s,is %s\n",typeid(T).name(),typeid(*node).name());
    throw 0;
  } else return res;
}

#define retype(type,expr) (p_retype<P_##type>(expr))

/*
void CodeWriter::makejump(JumpMaker &jm) {
  if (ji.label<0) throw JError("");
  comment("jump begin");
  for(int i=0;i<ji.trydhi;i++) poptry();
  polypop(ji.basedhi);
  deletelocals(ji.vardhi);
  jmp(ji.label);
  comment("jump end");
}
*/

void P_Directly::writecode(CodeWriter &cw) {
  bool saved=cw.getdirect();
  cw.setdirect(direct);
  command->writecode(cw);
  cw.setdirect(saved);
}

void CodeWriter::uop(string name) {
  if (direct) {
    if (name=="inc") faddc(1);
    else if (name=="dec") faddc(-1);
    else cm("op:"+name,0);
  } else {
    cm("op:"+name,0);
  }
}

void CodeWriter::op(string name) {
  if (direct) {
    if (name=="add") fadd();
    else if (name=="sub") fsub();
    else if (name=="mul") fmul();
    else if (name=="div") fdiv();
    else if (name=="mod") imod();
    else if (name=="eq") feq();
    else if (name=="neq") fneq();
    else if (name=="gt") fgt();
    else if (name=="lt") flt();
    else if (name=="get") fget();
    else if (name=="let") flet();
    else if (name=="cat") cat(2);
    else cm("op:"+name,1);
  } else {
    if (name=="cat") cat(2);
    else cm("op:"+name,1);
  }
}

void CodeWriter::pushnames(NameSpace ns) {
  switch (ns) {
    case ns_sys:pushsys();break;
    case ns_locals:pushlocals();break;
    case ns_this:pushthis();break;
  }
}

void yyerror(const char *msg);
void syntaxerror(const char *msg);

void P_Block::makejump(Jump &jump,JumpMaker &jm,P_Node *child) {
  int varhi=0;
  for(int i=0;i<coms->coms.count;i++) {
    if (coms->coms[i]==child) break;
    varhi+=coms->coms[i]->varstackdhi();
  }
  jm.varpop(varhi);
  P_Command::makejump(jump,jm,child);
}

void P_For::makejump(Jump &jump,JumpMaker &jm,P_Node *child) {
  if (jump.iscontinue(label)) {jm.searched(contlab);return;}
  if (jump.type==jt_break && child==body) jm.varpop(init->varstackdhi());
  if (jump.isbreak(label)) {jm.searched(getbreak());return;}
  P_Command::makejump(jump,jm,child);
}
void P_ForEach::makejump(Jump &jump,JumpMaker &jm,P_Node *child) {
  if (jump.iscontinue(label)) {jm.searched(contlab);return;}
  if (jump.type==jt_break && child==body) jm.varpop(1),jm.basepop(1);//navic deklarovana promenna
  if (jump.isbreak(label)) {jm.searched(getbreak());return;}
  P_Command::makejump(jump,jm,child);
}
void P_While::makejump(Jump &jump,JumpMaker &jm,P_Node *child) {
  if (jump.iscontinue(label)) {jm.searched(contlab);return;}
  if (jump.isbreak(label)) {jm.searched(getbreak());return;}
  P_Command::makejump(jump,jm,child);
}
void P_DoWhile::makejump(Jump &jump,JumpMaker &jm,P_Node *child) {
  if (jump.iscontinue(label)) {jm.searched(contlab);return;}
  if (jump.isbreak(label)) {jm.searched(getbreak());return;}
  P_Command::makejump(jump,jm,child);
}
void P_BreakContinue::writecode(CodeWriter &cw) {
  Jump jmp(&cw);
  BreakJumpMaker jm(&cw);
  jmp.type=getjumptype();
  jmp.label=label;
  makejump(jmp,jm,this);
  writefoot(cw);
}
void P_If::writecode(CodeWriter &cw) {
  expr->writecode(cw);
  int l=cw.getlabel();
  cw.condnotjmp(l);
  command->writecode(cw);
  cw.label(l);
  cw.deletelocals(expr->varstackdhi());
  writefoot(cw);
}
void P_IfElse::writecode(CodeWriter &cw) {
  expr->writecode(cw);
  int l1=cw.getlabel();
  cw.condnotjmp(l1);
  command1->writecode(cw);
  int l2=cw.getlabel();
  cw.deletelocals(expr->varstackdhi());
  cw.jmp(l2);
  cw.label(l1);
  cw.deletelocals(expr->varstackdhi());
  command2->writecode(cw);
  cw.label(l2);
  writefoot(cw);
}

void P_SelectItem::writecode(CodeWriter &cw,int end) {
  int next=cw.getlabel();
  expr->writecode(cw);
  cw.dup(2);
  cw.cm("op:eq",1);
  cw.condnotjmp(next);
  command->writecode(cw);
  cw.jmp(end);
  cw.label(next);
}
void P_Select::writecode(CodeWriter &cw) {
  expr->writecode(cw);
  int end=cw.getlabel();
  for(int i=0;i<items->count();i++) items->items[i]->writecode(cw,end);
  if (selectelse) selectelse->command->writecode(cw);
  cw.label(end);
  cw.pop();
  writefoot(cw);
}

void P_InspectItem::writecode(CodeWriter &cw,int end) {
  int next=cw.getlabel();
  expr->writecode(cw);
  cw.dup(2);
  cw.cm("isInstanceOf",1);
  cw.condnotjmp(next);
  command->writecode(cw);
  cw.jmp(end);
  cw.label(next);
}
void P_Inspect::writecode(CodeWriter &cw) {
  expr->writecode(cw);
  int end=cw.getlabel();
  for(int i=0;i<items->count();i++) items->items[i]->writecode(cw,end);
  if (inspectelse) inspectelse->command->writecode(cw);
  cw.label(end);
  cw.pop();
  writefoot(cw);
}

void P_SwitchItems::additem(P_SwitchItem *item) {
  if (wasdef && item->iscase()) syntaxerror("Unexpected case");
  items.add(item);
  if (item->isdefault()) wasdef=true;
}
void P_Switch::writecode(CodeWriter &cw) {
  expr->writecode(cw);
  int next=cw.getlabel();
  cw.jmp(next);
  for(int i=0;i<items->count();i++) {
    P_SwitchItem *item=items->item(i);
    if (P_SwitchCommand *com=item->iscommand()) {
      com->command->writecode(cw);
    } else if (P_SwitchCase *cas=item->iscase()) {
      int l=cw.getlabel();
      cw.jmp(l);
      cw.label(next);
      next=cw.getlabel();
      cas->expr->writecode(cw);
      cw.dup(2);
      cw.cm("op:eq",1);
      cw.condnotjmp(next);
      cw.label(l);
    } else if (P_SwitchDefault *def=item->isdefault()) {
      cw.label(next);
      next=-1;
    }
  }
  if (next>=0) cw.label(next);
  cw.pop();
  writefoot(cw);
}
void P_Switch::makejump(Jump &jump,JumpMaker &jm,P_Node *child) {
  jm.basepop(1);
  if (jump.isbreak(label)) {jm.searched(getbreak());return;}
  P_Command::makejump(jump,jm,child);
}

void P_For::writecode(CodeWriter &cw) {
  contlab=cw.getlabel();
  int l1=cw.getlabel(),l2=cw.getlabel();
  init->writecode(cw);
  cw.pop();
  cw.label(l1);
  cond->writecode(cw);
  cw.condnotjmp(l2);
  body->writecode(cw);
  cw.label(contlab);//continue
  inc->writecode(cw);
  cw.pop();
  cw.jmp(l1);
  cw.label(l2);
  cw.deletelocals(init->varstackdhi());
  writefoot(cw);
}
void P_ForEach::writecode(CodeWriter &cw) {
  contlab=cw.getlabel();
  int l1=cw.getlabel(),l2=cw.getlabel();
  expr->writecode(cw);
  cw.cm("getIterator",0);
  cw.label(l1);
  cw.dup(1);
  cw.cm("getValue",0);
  cw.dup(1);
  cw.condnotjmp(l2);
  cw.pushlocals();
  cw.initprop(id);
  cw.polypop(2);
  body->writecode(cw);
  cw.label(contlab);//continue
  cw.dup(1);
  cw.cm("moveNext",0);
  cw.pop();
  cw.deletelocals(1);
  cw.jmp(l1);
  cw.label(l2);
  cw.polypop(2);
  writefoot(cw);
}
void P_While::writecode(CodeWriter &cw) {
  contlab=cw.getlabel();
  int l1=cw.getlabel(),l2=cw.getlabel();
  cw.label(contlab);
  cw.label(l1);
  cond->writecode(cw);
  cw.condnotjmp(l2);
  body->writecode(cw);
  cw.jmp(l1);
  cw.label(l2);
  writefoot(cw);
}
void P_DoWhile::writecode(CodeWriter &cw) {
  contlab=cw.getlabel();
  int l1=cw.getlabel();
  cw.label(contlab);
  cw.label(l1);
  body->writecode(cw);
  cond->writecode(cw);
  cw.condjmp(l1);
  writefoot(cw);
}
void P_Params::writecode(CodeWriter &cw) {
  for(int i=0;i<params.count;i++) params[i]->writecode(cw);
}
void P_Call::writecode(CodeWriter &cw) {
  if (P_Defer *def=fn->isdefer()) {
    params->writecode(cw);
    def->obj->writecode(cw);
    cw.cm(def->name,params->count());
  } else if (P_Id *id=fn->isid()) {
    int depth=0;
    P_Function *f=findinnerfunc(id->value,&depth);
    if (f) {
      params->writecode(cw);
      cw.pushlocals();
      for(int i=1;i<depth;i++) cw.getprop("@funcparent");
      cw.pushthis();
      cw.cm(f->getfullname(),params->count()+1);
    } else {
      params->writecode(cw);
      int l=cw.getlabel();
      cw.pushthis();
      cw.xcm(id->value,params->count());
      cw.jnx(l);
      cw.pop();
      id->writecode(cw);
      cw.cm("op:call",params->count());
      cw.label(l);
    }
  } else {
    params->writecode(cw);
    fn->writecode(cw);
    cw.cm("op:call",params->count());
  }
}
void P_InlineArray::writecode(CodeWriter &cw) {
  params->writecode(cw);
  cw.pushsys();
  cw.cm("createArray",params->count());
}
void P_Defer::writecode(CodeWriter &cw) {
  NameSpace ns;
  if (obj->isdirectpush(ns)) {
    cw.pushnames(ns);
    cw.getprop(name);
  } else {
    obj->writecode(cw);
    cw.cm("get:"+name,0);
  }
}
void P_Assign::writecode(CodeWriter &cw) {
  if (P_Defer *def=left->isdefer()) {
    NameSpace ns;
    if (def->obj->isdirectpush(ns)) {
      right->writecode(cw);
      cw.pushnames(ns);
      cw.setprop(def->name);
      cw.pop();
    } else {
      right->writecode(cw);
      def->obj->writecode(cw);
      cw.cm("set:"+def->name,1);
    }
  } else if (P_Index *idx=left->isindex()) {
    idx->params->writecode(cw);
    right->writecode(cw);
    idx->array->writecode(cw);
    cw.cm("op:set:index",idx->params->count()+1);
  } else if (P_DefOp *dfo=left->isdefop()) {
    cw.pushstr(dfo->prop);
    right->writecode(cw);
    dfo->expr->writecode(cw);
    cw.cm("op:set:"+dfo->type,2);
  } else if (P_Id *id=left->isid()) {
    right->writecode(cw);
    cw.pushlocals();
    cw.findlocal(id->value);
    cw.assignprop(id->value);
    cw.pop();
  } else yyerror("Assign to not lvalue");
}
void P_AssignOp::writecode(CodeWriter &cw) {
  if (P_Id *id=left->isid()) {
    cw.pushlocals(); //&& locs
    cw.findlocal(id->value); //&& names
    cw.dup(1); //&& names names
    cw.getprop(id->value); //&& names prop
    right->writecode(cw);
    cw.xchg(1,2);
    cw.op(type);
//    cw.cm("op:"+type,1);
    cw.xchg(1,2);
    cw.assignprop(id->value);
    cw.pop();
  } else if (P_Defer *def=left->isdefer()) {
    NameSpace ns;
    if (def->obj->isdirectpush(ns)) {
      right->writecode(cw);
      cw.pushnames(ns);
      cw.getprop(def->name);
      cw.op(type);
//      cw.cm("op:"+type,1);
      cw.pushnames(ns);
      cw.assignprop(def->name);
      cw.pop();
    } else {
      def->obj->writecode(cw);
      cw.dup(1);
      cw.cm("get:"+def->name,0);
      right->writecode(cw);
      cw.xchg(1,2);
      cw.op(type);
//      cw.cm("op:"+type,1);
      cw.xchg(1,2);
      cw.cm("set:"+def->name,1);
    }
  } else if (P_Index *idx=left->isindex()) {
    idx->params->writecode(cw);
    idx->array->writecode(cw);
    cw.dup(2);
    cw.dup(2);
    cw.cm("op:get:index",idx->params->count());
    right->writecode(cw);
    cw.xchg(1,2);
    cw.op(type);
//    cw.cm("op:"+type,1);
    cw.xchg(1,2);
    cw.cm("op:set:index",idx->params->count()+1);
  } else if (P_DefOp *dfo=left->isdefop()) {
    cw.pushstr(dfo->prop);
    dfo->expr->writecode(cw);
    cw.dup(2);
    cw.dup(2);
    cw.cm("op:get:"+dfo->type,1);
    right->writecode(cw);
    cw.xchg(1,2);
    cw.op(type);
//    cw.cm("op:"+type,1);
    cw.xchg(1,2);
    cw.cm("op:set:"+dfo->type,2);
  } else yyerror("Assign to not lvalue");
}
void P_UnaryModify::writecode(CodeWriter &cw) {
  if (P_Defer *def=expr->isdefer()) {
    NameSpace ns;
    if (def->obj->isdirectpush(ns)) {
      cw.pushnames(ns);
      cw.getprop(def->name);
      if (!isprefix()) cw.dup(1);
      cw.uop(type);
//      cw.cm("op:"+type,0);
      if (isprefix()) cw.dup(1);
      cw.pushnames(ns);
      cw.assignprop(def->name);
      cw.polypop(2);
      throw 0;
    } else {
      def->obj->writecode(cw);
      cw.dup(1);
      cw.cm("get:"+def->name,0);
      if (!isprefix()) cw.dup(1);
      cw.uop(type);
//      cw.cm("op:"+type,0);
      if (isprefix()) cw.dup(1);
      cw.xchg(2,3);
      cw.xchg(1,2);
      cw.cm("set:"+def->name,1);
      cw.pop();
    }
  } else if (P_Id *id=expr->isid()) {
    if (isprefix()) {
      cw.pushlocals();
      cw.findlocal(id->value);
      cw.dup(1);
      cw.getprop(id->value);
      cw.uop(type);
//      cw.cm("op:"+type,0);
      cw.xchg(1,2);
      cw.assignprop(id->value);
      cw.pop();
    } else {
      cw.pushlocals();
      cw.findlocal(id->value);
      cw.dup(1);
      cw.getprop(id->value);
      cw.dup(1);
      cw.uop(type);
//      cw.cm("op:"+type,0);
      cw.xchg(2,3);
      cw.xchg(1,2);
      cw.assignprop(id->value);
      cw.polypop(2);
    }
  } else if (P_DefOp *dfo=expr->isdefop()) {
    //eg. x->a++
    dfo->expr->writecode(cw);//&& x
    cw.pushstr(dfo->prop);//&& x "a"
    cw.dup(1);//&& x "a" "a"
    cw.dup(3);//&& x "a" "a" x
    cw.cm("op:get:"+dfo->type,1);//&& x "a" x->a
    if (!isprefix()) cw.dup(1);
    cw.uop(type);
//    cw.cm("op:"+type,0);
    if (isprefix()) cw.dup(1);
    cw.xchg(2,4);
    cw.xchg(1,2);
    cw.cm("op:set:"+dfo->type,2);
    cw.pop();
  } else if (P_Index *idx=expr->isindex()) {
    idx->params->writecode(cw);
    idx->array->writecode(cw);
    cw.dup(2);
    cw.dup(2);
    cw.cm("op:get:index",idx->params->count());
    if (!isprefix()) cw.dup(1);
    cw.uop(type);
//    cw.cm("op:"+type,0);
    if (isprefix()) cw.dup(1);
    cw.move(2,4);
    cw.xchg(1,2);
    cw.cm("op:set:index",idx->params->count()+1);
    cw.pop();
  } else yyerror("Assign to not lvalue");
}
void P_CondExpr::writecode(CodeWriter &cw) {
  int l1=cw.getlabel(),l2=cw.getlabel();
  cond->writecode(cw);
  cw.condnotjmp(l1);
  yes->writecode(cw);
  cw.jmp(l2);
  cw.label(l1);
  no->writecode(cw);
  cw.label(l2);
}
void P_Or::writecode(CodeWriter &cw) {
  int l1=cw.getlabel(),l2=cw.getlabel();
  left->writecode(cw);
  cw.condjmp(l1);
  right->writecode(cw);
  cw.condjmp(l1);
  cw.pushnull();
  cw.jmp(l2);
  cw.label(l1);
  cw.pushsys();
  cw.label(l2);
}
void P_And::writecode(CodeWriter &cw) {
  int l1=cw.getlabel(),l2=cw.getlabel();
  left->writecode(cw);
  cw.condnotjmp(l1);
  right->writecode(cw);
  cw.condnotjmp(l1);
  cw.pushsys();
  cw.jmp(l2);
  cw.label(l1);
  cw.pushnull();
  cw.label(l2);
}
void P_Not::writecode(CodeWriter &cw) {
  int l1=cw.getlabel(),l2=cw.getlabel();
  expr->writecode(cw);
  cw.condjmp(l1);
  cw.pushsys();
  cw.jmp(l2);
  cw.label(l1);
  cw.pushnull();
  cw.label(l2);
}

void P_Index::writecode(CodeWriter &cw) {
  params->writecode(cw);
  array->writecode(cw);
  cw.cm("op:get:index",params->count());
}
void P_DefOp::writecode(CodeWriter &cw) {
  cw.pushstr(prop);
  expr->writecode(cw);
  cw.cm("op:get:"+type,1);
}
void P_New::writecode(CodeWriter &cw) {
  params->writecode(cw);
  cl->writecode(cw);
  cw.cm("op:new",params->count());
}
void P_Detach::writecode(CodeWriter &cw) {
  obj->writecode(cw);
  cw.pushstr(name);
  params->writecode(cw);
  cw.pushsys();
  cw.cm("createInterruptFunction",params->count()+2);
}
void P_NewStore::writecode(CodeWriter &cw) {
  cw.pushsys();
  cw.getprop("Store");
  cw.cm("op:new",0);
  items->writecode(cw);
}
void P_StoreItems::writecode(CodeWriter &cw) {
  for(int i=0;i<items.count;i++) items[i]->writecode(cw);
}
void P_StoreItem::writecode(CodeWriter &cw) {
  expr->writecode(cw);
  cw.xchg(1,2);
  cw.setprop(name);
  cw.xchg(1,2);
  cw.pop();
}

int P_Commands::varstackdhi() {
  int res=0;
  for(int i=0;i<coms.count;i++) res+=coms[i]->varstackdhi();
  return res;
}

void P_Block::writecode(CodeWriter &cw) {
  bool direct=cw.getdirect();
  int delcnt=coms->varstackdhi();
  coms->writecode(cw);
  cw.deletelocals(delcnt);
  writefoot(cw);
  cw.setdirect(direct);
}

void P_ModuleElems::writeinit(CodeWriter &cw) {
  for(int i=0;i<elems.count;i++) elems[i]->writeinit(cw);
}

void P_ModuleElems::getsubclasses(TCntVec<ClassToWrite*> &classes) {
  for(int i=0;i<elems.count;i++) elems[i]->getsubclasses(classes);
}

struct FindMethodAction:ForEachAction {
  TCntVec<MethodToWrite*> *mtds;
  void work(P_Node *node,P_Node *parent) {
    node->getextramethods(*mtds);
    node->foreach(*this);
  }
};

void P_RegExp::writecode(CodeWriter &cw) {
  cw.pushstr(value);
  cw.pushsys();
  cw.getprop("RegExp");
  cw.cm("op:new",1);
}

void P_CommonComExpr::writecode(CodeWriter &cw) {
  cw.pushlocals();
  cw.pushthis();
  cw.pushstr(mtdid);
  cw.pushsys();
  cw.cm("createInlineMethod",3);
}

void P_CommonComExpr::writejoins(CodeWriter &cw) {
  cw.joinnames(ns_locals,ns_this);
  cw.pushparam(0);
  cw.pushlocals();
  cw.join();
  cw.polypop(2);
  cw.pushparam(1);
  cw.pushlocals();
  cw.join();
  cw.polypop(2);
}

void P_ComExpr::write_method(CodeWriter &cw) {
  xassert(mtdid!="");
  cw.begin(mtdid,2,2);
  writejoins(cw);
  command->writecode(cw);
  cw.end();
}

void P_Return::makefinally(CodeWriter &cw) {
  ReturnJumpMaker rjm(&cw);
  Jump jump(&cw);
  jump.type=jt_return;
  makejump(jump,rjm,this);
}

void P_RetComExpr::write_method(CodeWriter &cw) {
  xassert(mtdid!="");
  cw.begin(mtdid,2,2);
  writejoins(cw);
  expr->writecode(cw);
  cw.doret(1);
  cw.end();
}


void P_ModuleElems::findmethods(TCntVec<MethodToWrite*> &mtds) {
  FindMethodAction fma;
  fma.mtds=&mtds;
  foreach(fma);
}

void P_Usings::write(IntfWriter iw) {
  for(int i=0;i<mods.count;i++) iw.usemod(mods[i]);
  for(int i=0;i<libs.count;i++) iw.uselib(libs[i]);
  for(int i=0;i<reqmods.count;i++) iw.require(reqmods[i]+".module");
}

void P_Class::makeislistcode(CodeWriter &cw) {
  cw.pushsys();
  cw.getprop(name);
  cw.pushsys();
  cw.cm("createIsList",1);
  cw.pop();
}

void P_Module::write(ZBCWriter &zw,string name0) {
  string name=name0;
  if (this->name->name!="") name=this->name->name;
  int ilevel=this->name->initlevel;
  IntfWriter &mod=zw.intf();
  if (ilevel>=0) mod.begin(name+".module");else mod.begin(name+".module.includable");
  if (ilevel>=0) {
    mod.initproc("@init",ilevel);
    mod.initproc("@registermodule",1);
    mod.initproc("@makeislist",3);
  }
  mod.initproc("@createclasses",2);
  mod.autocreate();
  usings->write(mod);
  elems->writeintfprops(mod);
  mod.beginmtds();
  CodeWriter &cw=mod.sub();
  if (ilevel>=0) {
    cw.begin("@registermodule",0,0);
    cw.pushthis();
    cw.pushsys();
    cw.setprop(name);
    cw.polypop(2);
    cw.end();
  }

  if (ilevel>=0) cw.begin("@init",0,0);
  else cw.begin("@init:"+name,0,0);
  cw.joinnames(ns_this,ns_sys);
  cw.joinnames(ns_locals,ns_this);
  elems->writeinit(cw);
  cw.end();

  cw.begin("@makeislist",0,0);
  for(int i=0;i<elems->elems.count;i++) elems->elems[i]->makeislistcode(cw);
  cw.end();

  cw.begin("@createclasses",0,0);
  for(int i=0;i<elems->elems.count;i++) elems->elems[i]->createclasscode(cw);
  cw.end();


  elems->writemethods(mod);

  TCntVec<MethodToWrite*> mtds;elems->findmethods(mtds);
  for(int i=0;i<mtds.count;i++) mtds[i]->write_method(cw);

  mod.endintf();

  TCntVec<ClassToWrite*> classes;
  elems->getsubclasses(classes);
  for(int i=0;i<classes.count;i++) classes[i]->write_class(zw);
}

void P_IncludeModule::writeinit(CodeWriter &cw) {
  cw.pushthis();
  cw.cm("@init:"+name,0);
  cw.pop();
}

void P_Class::createclasscode(CodeWriter &cw) {
  cw.co(name+".class");
  cw.pushsys(); //&& class|sys
  cw.setprop(name);
  cw.pop();//&& class
  cw.pushthis();//&& class this
  cw.xchg(1,2);//&& this class
  cw.setprop("@module");

  cw.xchg(1,2);//&& class this
  cw.pop();//&& class
  cw.cm("@createclasses",0);
  cw.pop();

  cw.pushstr(name);
  cw.pushsys();
  cw.getprop(name);
  cw.setprop("@classname");
  cw.polypop(2);

  cw.pushstr(super);
  cw.pushsys();
  cw.getprop(name);
  cw.setprop("@supername");
  cw.polypop(2);

//  cw.polypop(2);
}


void P_Constructor::writeinstanceinit(IntfWriter &iw,P_ClassElems *elems,P_Class *cl) {
  CodeWriter &cw=iw.sub();
  cw.begin("@constructor",args->mincnt(),args->maxcnt());
  cw.joinnames(ns_locals,ns_this);
//  elems->writeinit(cw);
  cw.pushthis();
  cw.xcm("@init_instance:"+cl->name,0);
  int lab=cw.getlabel();
  cw.jnx(lab);//nutne kvuli vycisteni xreg
  cw.label(lab);
  cw.pop();
  args->writedefs(cw);
  code->writecode(cw);
  cw.pushthis();
  cw.doret();
  cw.end();
}

void P_Constructor::writeclassinit(IntfWriter &iw,P_ClassElems *elems) {
  CodeWriter &cw=iw.sub();
  cw.begin("op:new",args->mincnt(),args->maxcnt());
  cw.co(elems->clazz->name+".instance");
  cw.dup(2);
  cw.getprop("@module");
  cw.xchg(1,2);
  cw.join();
  cw.xchg(1,3);
  cw.polypop(2);
  cw.cm("@constructor",args->mincnt());
  cw.doret();
  cw.end();
}

void P_Class::write_class(ZBCWriter &zw) {
  IntfWriter &inst=zw.intf();
  inst.begin(name+".instance");
  if (super!="") {
    inst.include(super+".instance");
    inst.creatoras(super+".instance");
  }
  inst.beginmtds();

  {
    CodeWriter &cw=inst.sub();
    cw.begin("getClass",0,0);
    cw.pushsys();
    cw.getprop(name);
    cw.doret();
    cw.end();
  }

  {
    CodeWriter &cw=inst.sub();
    cw.begin("@init_instance:"+name,0,0);
    if (super!="" && super!="Object") {
      cw.pushthis();
      cw.xcm("@init_instance:"+super,0);
      int lab=cw.getlabel();
      cw.jnx(lab);//nutne kvuli vycisteni xreg
      cw.label(lab);
      cw.pop();
    }
    cw.joinnames(ns_locals,ns_this);
    elems->writeinit(cw);
    cw.pushnull();
    cw.doret();
    cw.end();
  }

  elems->writemethods(inst);
  for(int i=0;i<elems->count();i++) if (P_Constructor *con=elems->item(i)->isconstructor()) {
    con->writeinstanceinit(inst,elems,this);
  }

  CodeWriter &icw=inst.sub();
  TCntVec<MethodToWrite*> mtds;elems->findmethods(mtds);
  for(int i=0;i<mtds.count;i++) mtds[i]->write_method(icw);

  inst.endintf();

  IntfWriter &cl=zw.intf();
  cl.begin(name+".class");
  cl.beginmtds();

  CodeWriter &ccw=cl.sub();
  ccw.begin("@createclasses",0,0);
  for(int i=0;i<elems->count();i++) elems->item(i)->createclasscode(ccw);
  ccw.end();

  for(int i=0;i<elems->count();i++) if (P_Constructor *con=elems->item(i)->isconstructor()) {
    con->writeclassinit(cl,elems);
  }
  cl.endintf();
}

void P_ClassElems::writestaticmethods(IntfWriter &zw) {
  for(int i=0;i<elems.count;i++) item(i)->writestaticmethods(zw);
}

void P_ModuleElems::writemethods(IntfWriter &zw) {
  for(int i=0;i<elems.count;i++) elems[i]->writemethods(zw);
}

void P_Function::writecode(CodeWriter &cw) {
  cw.begin(getfullname(),args->mincnt(),args->maxcnt());
  cw.joinnames(ns_locals,ns_this);
  args->writedefs(cw);
  elems->block->writecode(cw);
  cw.end();
}

//vnitrni funkce
void P_Function::write_method(CodeWriter &cw) {
  cw.begin(getfullname(),args->mincnt()+1,args->maxcnt()+1);
  cw.joinnames(ns_locals,ns_this);
  cw.pushparam(args->realcnt());
  cw.pushlocals();
  cw.join();
  cw.setprop("@funcparent");
  cw.polypop(2);
  args->writedefs(cw);
  elems->block->writecode(cw);
  cw.end();
}


void P_ArgList::writedefs(CodeWriter &cw) {
  for(int i=0;i<args.count;i++) args[i]->writedef(cw);
}

void P_Argument::writedef(CodeWriter &cw) {
  cw.pushparam(index);
  cw.pushlocals();
  cw.initprop(name);
  cw.polypop(2);
}

void P_TryCatch::makejump(Jump &jump,JumpMaker &jm,P_Node *child) {
  if (child==safeblock) {
    jm.trypop(1);
  }
  P_Command::makejump(jump,jm,child);
}
void P_TryCatch::writecode(CodeWriter &cw) {
  int l1=cw.getlabel(),l2=cw.getlabel();
  cw.pushtry(l1);
  safeblock->writecode(cw);
  cw.poptry();
  cw.jmp(l2);
  cw.label(l1);
  cw.pushlocals();
  cw.initprop(name);
  cw.polypop(2);
  catchblock->writecode(cw);
  cw.deletelocals(1);
  cw.label(l2);
}

void P_TryFinally::writecode(CodeWriter &cw) {
  int catchl=cw.getlabel(),end=cw.getlabel();
  cw.pushtry(catchl);
  safeblock->writecode(cw);

  //normalni pruchod
  cw.poptry();
  cw.callnear(nearlabel);
  cw.jmp(end);

  //vyjimka (catch-blok)
  cw.label(catchl);
  cw.callnear(nearlabel);
  cw.raise();

  //finally blok
  cw.label(nearlabel);
  finallyblock->writecode(cw);
  cw.retnear();

  cw.label(end);
}

void P_TryFinally::makejump(Jump &jump,JumpMaker &jm,P_Node *child) {
  xassert(nearlabel>0);
  if (child==safeblock) jm.callnear(nearlabel);
  P_Command::makejump(jump,jm,child);
}

void P_Readonly::writemethods(IntfWriter &zw) {
  for(int i=0;i<vardefs->count();i++) {
    P_VarDef *vd=vardefs->item(i);
    CodeWriter &cw=zw.sub();
    cw.begin("get:"+vd->name,0,0);
    cw.pushthis();
    cw.getprop(vd->name);
    cw.doret();
    cw.end();
  }
}

void P_Readwrite::writemethods(IntfWriter &zw) {
  P_Readonly::writemethods(zw);
  for(int i=0;i<vardefs->count();i++) {
    P_VarDef *vd=vardefs->item(i);
    CodeWriter &cw=zw.sub();
    cw.begin("set:"+vd->name,1,1);
    cw.pushparam(0);
    cw.pushthis();
    cw.setprop(vd->name);
    cw.pop();
    cw.doret();
    cw.end();
  }
}

void P_GetPropSpec::writemethod(CodeWriter &cw,string name) {
  if (!block) return;
  cw.begin("get:"+name,0,0);
  cw.joinnames(ns_locals,ns_this);
  block->writecode(cw);
  cw.end();
}

void P_SetPropSpec::writemethod(CodeWriter &cw,string name) {
  if (!block) return;
  cw.begin("set:"+name,1,1);
  cw.joinnames(ns_locals,ns_this);
  cw.pushparam(0);
  cw.pushlocals();
  cw.setprop("value");
  cw.polypop(2);
  block->writecode(cw);
  cw.end();
}

void P_Property::writemethods(IntfWriter &zw) {
  CodeWriter &cw=zw.sub();
  get->writemethod(cw,name);
  set->writemethod(cw,name);
}

void P_Readonly::writeinit(CodeWriter &cw) {
  vardefs->writecode(cw,ns_this);
}

void P_Command::writefoot(CodeWriter &cw) {
  if (breaklab>=0) cw.label(breaklab);
}

void P_Command::makejump(Jump &jump,JumpMaker &jm,P_Node *child) {
  if (label!="" && jump.isbreak(label)) {
    if (breaklab<0) breaklab=jump.cw->getlabel();
    jm.searched(breaklab);
  } else if (comparent) {
    comparent->makejump(jump,jm,this);
  } else if (jump.type==jt_break || jump.type==jt_continue) {
    throw JError("Label not found:"+jump.label);
  }
}

struct FillParentsAction:ForEachAction {
  void work(P_Node *node,P_Node *parent) {
    node->parent=parent;
    node->foreach(*this);
  }
};

void P_Node::fillparents() {
  FillParentsAction fpa;
  foreach(fpa);
}

void P_FuncElems::foreach(ForEachAction &a) {
  a.work(block,this);
  for(int i=0;i<fns.count;i++) a.work(fns[i],this);
}
void P_FuncElems::getextramethods(TCntVec<MethodToWrite*> &mtds) {
  for(int i=0;i<fns.count;i++) mtds.add(fns[i]);
}

P_Function *P_Function::findinnerfunc(string name,int *depth) {
  (*depth)++;
  for(int i=0;i<elems->fns.count;i++) if (elems->fns[i]->name==name) return elems->fns[i];
  return P_ClassElem::findinnerfunc(name,depth);
}

void P_DefOpDef::addelems(P_ModuleElems *elems) {
  string par=parname->value;
  if (get) elems->addelem(makefn1("op:get:"+type,new P_Id(par),get->elems));
  if (set) elems->addelem(makefn2("op:set:"+type,new P_Id(par),new P_Id("value"),set->elems));
  delete this;
}

void P_IndexOpDef::addelems(P_ModuleElems *elems) {
  if (get) {
    elems->addelem(new P_Function(new P_Id("op:get:index"),arglist->clone(),get->elems));
  }
  if (set) {
    P_ArgList *sargs=arglist->clone();
    sargs->addarg(new P_Argument(new P_Id("value")));
    elems->addelem(new P_Function(new P_Id("op:set:index"),sargs,set->elems));
  }
  delete this;
};


int yyparse();
void skip();

void stop_error(STRING msg) {
  throw JError(msg);
}

static const char *pos;
static const char *text;
//static bool maybeop=false;
static P_Module *lex_result;

void syntaxerror(const char *msg) {
  CommonParseError pe;
  pe.init(text,msg,pos-text,"");
  throw pe;
}

void yyerror(const char *msg) {
}

extern int yydebug;

int last_label=0;
int generate_int() {return ++last_label;} 


//#define YYDEBUG 1

P_Module *abazparse(string atext) {
  compilingmodule="";
  yydebug=1;
  last_label=0;
  text=pos=atext.c_str();
  skip();
  lex_result=NULL;
  if (yyparse()) {
    stop_error("File compilation failed");
  }
  if (lex_result) lex_result->fillparents();
  return lex_result;
}

#define YYSTYPE P_Node *

#include "abazcmp.cc"

static string readcstring(char end) {
  bstring res;
  while (*pos && *pos!=end) {
    if (*pos=='\\') {
      pos++;
      if (*pos=='n') res<<'\n';
      else res<<*pos;
      if (*pos) pos++;
    } else res<<*pos++;
  }
  if (*pos) pos++;
  return res;
}

#define mac_iswhite(c) ((c)==' ' || (c)=='\n' || (c)=='\r' || (c)=='\t')
#define mac_isidf(c) ((c)=='_' || ((c)>='a' && (c)<='z') || ((c)>='A' && (c)<='Z') || ((c)>='0' && (c)<='9'))
#define mac_isidfbeg(c) ((c)=='_' || ((c)>='a' && (c)<='z') || ((c)>='A' && (c)<='Z'))

#define m_keyword(str,id) {\
  int len=strlen(str);\
  if (strbegcmp(str,pos) && !mac_isidf(pos[len])) {pos+=len;skip();return id;}\
}

#define m_oper(str,id) {\
  int len=strlen(str);\
  if (strbegcmp(str,pos)) {pos+=len;skip();return id;}\
}


void jmpwhite() {
  while (mac_iswhite(*pos)) pos++;
}

static void skip() {
  retry:
  jmpwhite();
  if (*pos=='/') {
    if (pos[1]=='*') {
      while (*pos && (*pos!='*' || pos[1]!='/')) pos++;
      if (*pos) pos+=2;
      goto retry;
    }
    if (pos[1]=='/') {
      while (*pos && *pos!='\n') pos++;
      if (*pos) pos++;
      goto retry;
    }
  }
}

int yylex() {
  skip();
  retry:
  if (!*pos) return 0;
  char c=*pos;
//  if (c>='0' && c<='9' || (c=='-' && pos[1]>='0' && pos[1]<='9')) {
  if (c>='0' && c<='9') {
//    if (c=='-' && !op) goto no_number;//je to operator
    char *end;
    double res=strtod(pos,&end);
    pos=end;
    yylval=new P_Number(res);
    skip();
    return tNUMBER;
  }
//  no_number:
  if (c=='\'' || c=='"') {
    char end=c;
    pos++;
    string res=readcstring(end);
    yylval=new P_String(res);
    skip();
    return tSTRING;
  }
  if (c=='@' && (pos[1]=='"' || pos[1]=='\'')) {
    pos++;
    char end=*pos;
    pos++;
    bstring res;
    while (*pos && *pos!=end) res<<*pos++;
    if (*pos) pos++;
    yylval=new P_String(res);
    skip();
    return tSTRING;
  }
  if (c=='@' && (pos[1]=='/' || pos[1]=='%')) {
    pos++;
    char end=*pos;
    pos++;
    bstring res;
    while (*pos && *pos!=end) {
      if (*pos=='\\' && pos[1]) res<<*pos++,res<<*pos++;
/*      if (*pos=='\\' && pos[1]=='n') res<<'\n',pos+=2;
      if (*pos=='\\' && pos[1]=='t') res<<'\t',pos+=2;
      if (*pos=='\\' && pos[1]=='r') res<<'\r',pos+=2;*/
      else res<<*pos++;
    }
    if (*pos) pos++;
    yylval=new P_RegExp(makeexregex(res));
    skip();
    return tREGEXP;
  }

  m_keyword("if",kIF)
  m_keyword("else",kELSE)
  m_keyword("do",kDO)
  m_keyword("while",kWHILE)
  m_keyword("function",kFUNCTION)
  m_keyword("for",kFOR)
  m_keyword("in",kIN)
  m_keyword("var",kVAR)
  m_keyword("new",kNEW)
  m_keyword("module",kMODULE)
  m_keyword("system",kSYSTEM)
  m_keyword("this",kTHIS)
  m_keyword("return",kRETURN)
  m_keyword("try",kTRY)
  m_keyword("catch",kCATCH)
  m_keyword("finally",kFINALLY)
  m_keyword("class",kCLASS)
  m_keyword("readonly",kREADONLY)
  m_keyword("readwrite",kREADWRITE)
  m_keyword("extends",kEXTENDS)
  m_keyword("break",kBREAK)
  m_keyword("continue",kCONTINUE)
  m_keyword("constructor",kCONSTRUCTOR)
  m_keyword("property",kPROPERTY)
  m_keyword("get",kGET)
  m_keyword("set",kSET)
  m_keyword("library",kLIBRARY)
  m_keyword("program",kPROGRAM)
  m_keyword("select",kSELECT)
  m_keyword("switch",kSWITCH)
  m_keyword("case",kCASE)
  m_keyword("default",kDEFAULT)
  m_keyword("delegate",kDELEGATE)
  m_keyword("false",kFALSE)
  m_keyword("true",kTRUE)
  m_keyword("null",kNULL)
  m_keyword("throw",kTHROW)
  m_keyword("import",kIMPORT)
  m_keyword("using",kUSING)
  m_keyword("operator",kOPERATOR)
  m_keyword("directly",kDIRECTLY)
  m_keyword("indirectly",kINDIRECTLY)
  m_keyword("option",kOPTION)
  m_keyword("requires",kREQUIRES)
  m_keyword("yield",kYIELD)
  m_keyword("detach",kDETACH)
  m_keyword("is",kIS)
  m_keyword("inspect",kINSPECT)
  m_keyword("include",kINCLUDE)
  m_keyword("includable",kINCLUDABLE)

  m_oper("++",kINC)
  m_oper("--",kDEC)
  m_oper("-/",kGETELEM)
  m_oper("->",kGETPROP)
  m_oper(">=",kGET)
  m_oper("<=",kLET)
  m_oper(">",kGT)
  m_oper("<",kLT)
  m_oper("===",kTESTEQ)
  m_oper("==",kEQ)
  m_oper("!=",kNEQ)
  m_oper("+=",kADDASSIGN)
  m_oper("-=",kSUBASSIGN)
  m_oper("*=",kMULASSIGN)
  m_oper("/=",kDIVASSIGN)
  m_oper("%=",kMODASSIGN)
  m_oper("&&",kAND)
  m_oper("||",kOR)
  m_oper("!",kNOT)

  if (mac_isidfbeg(c)) {
    bstring res;
    while (mac_isidf(c)) {
      res<<c;pos++;c=*pos;
    }
    yylval=new P_Id(res);
    skip();
    return tID;
  }
  return *pos++;
}


#pragma package(smart_init)
