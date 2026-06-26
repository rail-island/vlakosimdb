#include <jlib.h>
#pragma hdrsitop

#include "zbint.h"
#include "zbrt.h"
#include <typeinfo>

//#define INTERPRETDEBUG

#define INTCHECK

#ifdef ZBDEBUG
#define INTCHECK
#endif

#ifdef INTCHECK
#define checkstack(pos) xassert(pos>0);xassert(pos<=basew-basestack);
#else
#define checkstack(pos)
#endif

namespace zaba {

static void ZBINCALL (Interpreter::*instructions[Coms::comcnt])();
static bool int_inited=false;

void Interpreter::getstate(XMLNode xml) {
  xml.setname("interpret_state");
  string callstack=getstacktrace();callstack.replaceall("\n",";");
  xml.setvar("callstack",callstack);
  xml.setvar("tryhi",tryw-trystack);

  XMLNode locs=xml.add("locals");callw[-1].locals->vars.dbgsave(locs);
  if (zbInstance *inst=dynamic_cast<zbInstance*>(callw[-1].thiz)) {
    XMLNode flds=xml.add("this.fields");inst->vars.dbgsave(flds);
  }
  XMLNode stack=xml.add("basestack");
  int ss=basew-basestack;
  for(int i=ss;i>=1;i--) basew[-i]->dbgsave(stack.add("item"));
  if (tryw>trystack) {
    int idx=tryw[-1].basew-basestack;
    stack[idx-1].setname(stack[idx-1].getname()+".trytop");
  }
}


string Interpreter::getstacktrace() {
  CallFrame *cw=callw-1;
  bstring res;
  while (cw>=callstack) {
    res<<cw->thiz->intf->name<<" ... "<<cw->info->name<<"\n";
    cw--;
  }
  return res;
}

void Interpreter::nativeerror(string msg) {
  zbException *e=new zbException(rt);
  e->setmsg(msg);
  bnativeerror=true;
  raise_error(e);
}
void Interpreter::internalerror(string msg) {
  zbException *e=new zbException(rt);
  e->setmsg(msg);
  raise_error(e);
}

Interpreter::Interpreter() {

  basestack=new zbObject*[0x1000];
  callstack=new CallFrame[0x400];
  trystack=new TryFrame[0x200];
  xstack=new XFailItem[0x100];

  stopnow=false;
/*
  basestack=new zbObject*[0x2000];
  callstack=new CallFrame[0x800];
  trystack=new TryFrame[0x400];
  xstack=new XFailItem[0x200];
*/

  basew=basestack;ip=NULL;initinstructions();xreg=0;rt=NULL;
  callw=callstack;tryw=trystack;xw=xstack;//xerror="";
  savedip=NULL;
}

Interpreter::~Interpreter() {
  while (basew>basestack) (*--basew)->release();
  while (callw>callstack) (*--callw).release();

  delete []basestack;
  delete []callstack;
  delete []trystack;
  delete []xstack;
}

void Interpreter::initinstructions() {
  if (int_inited) return;
  int_inited=true;
  for(int i=0;i<Coms::comcnt;i++) instructions[i]=&Interpreter::empty;
  #define ins(_I) instructions[Coms::_I]=&Interpreter::_I;
  ins(pop) ins(cm) ins(xcm) ins(doret) ins(pushobj) ins(co)
  ins(getprop) ins(setprop) ins(assignprop) ins(initprop) ins(xgetprop)
  ins(pushthis) ins(pushnull) ins(pushsys) ins(jx) ins(jnx)
  ins(raise) ins(xchg) ins(polypop) ins(pushlocals) ins(pushparam) ins(xraise)
  ins(condjmp) ins(condnotjmp) ins(jmp) ins(dup) ins(deleteprops)
  ins(pushtry) ins(poptry) ins(move) ins(find) ins(join)
  ins(callnear) ins(retnear)
  ins(fadd) ins(fmul) ins(fsub) ins(fdiv) ins(idiv) ins(imod) ins(cat)
  ins(testeq)
  ins(feq) ins(fneq) ins(flt) ins(fgt) ins(flet) ins(fget) ins(faddc) ins(fmulc)
  ins(yield)
//  ins(getlocal) ins(setlocal)
  #undef ins
}

/*
void CallFrame::getvar(Interpreter *ir,STRING name) {
  zbObject *res=vars.findvar(name);
  if (res) ir->push(res),ir->xok();
  else ir->xnomtd();
}
void CallFrame::setvar(Interpreter *ir,STRING name,zbObject *value) {
  vars.setvar(name,value);
  ir->dopop();
}
*/

void Interpreter::xnoprop(const char *name) {
  xreg=1;
  xw->object=basew[-1];
  xw->name=name;
  xw->type="property";
  xw->parcnt=0;
  xw->pars=NULL;
  xw++;
}

void ZBINCALL Interpreter::move() {
  int i=ip[1].i,j=ip[2].i;
  if (i>j) {
    zbObject *pom=basew[-i];
    memmove(basew-i,basew-i+1,(i-j)*sizeof(zbObject *));
    basew[-j]=pom;
  }
  if (i<j) {
    zbObject *pom=basew[-i];
    memmove(basew-j+1,basew-j,(j-i)*sizeof(zbObject *));
    basew[-j]=pom;
  }
  ip+=3;
}

void Interpreter::xnomtd(const char *name,int parcnt) {
  xreg=1;
  xw->object=getfnthis(parcnt);
  xw->name=name;
  xw->type="method";
  xw->parcnt=parcnt;
  xw->pars=getparams(parcnt);
  xw++;
//  xreg=1;xerror=name;
}

void Interpreter::dopop(int cnt) {
  #ifdef INTCHECK
  xassert(basew-basestack>=cnt);
  #endif
  for(int i=0;i<cnt;i++) (*--basew)->release();
}

void Interpreter::setfnresult(zbObject *res,int parcnt) {
  dopop(parcnt+1);
  *basew++=res;
}

void Interpreter::internalcall(zbObject *athiz,Instruction *aip,int parcnt,FnInfo *info) {
  callw->thiz=athiz->addref();
  callw->ret=ip;
  callw->tryw=tryw;
  callw->basew=basew-parcnt-1;
  callw->locals=new zbInstance(rt,"");
  callw->info=info;
  callw++;
  ip=aip;
}

string Interpreter::searchin(zbObject *obj,zbObject *base) {
  if (base==obj) return "";
  if (base==NULL) base=obj;
  MEMSTROUT res;
  res<<obj->debugstring()<<"\n";
  if (zbInstance *in=dynamic_cast<zbInstance*>(obj)) {
    for(int i=0;i<in->subnames.count;i++) res<<searchin(in->subnames[i],base);
  }
  return res.str();
}

void Interpreter::raisenoprop(zbObject *obj,STRING name) {
  string msg=fmtstr("Property not found:%s,Searched in:\n%s",name.c_str(),searchin(obj,NULL).c_str());
  internalerror(msg);
}

void ZBINCALL Interpreter::find() {
  int flg=ip[1].i;
  const char *s=ip[2].s;
  zbObject *obj=basew[-1]->find(flg,s,NULL);
  if (obj) {
    zbObject *old=basew[-1];
    basew[-1]=obj->addref();
    old->release();
    ip+=3;
  } else {
    raisenoprop(basew[-1],s);
  }
}


void ZBINCALL Interpreter::pushobj() {
  *basew++=ip[1].obj->addref();
  ip+=2;
}
void ZBINCALL Interpreter::pop() {
  #ifdef INTCHECK
  xassert(basew>basestack);
  #endif
  (*--basew)->release();
  ip+=1;
}

void ZBINCALL Interpreter::join() {
  if (basew[-1]==basew[-2]) {
    internalerror("Join the same names:\n"+basew[-1]->debugstring());
  }
  if (basew[-1]->joinnames(basew[-2])) {
    ip++;
  } else {
    internalerror("Join names error or cycle:\nImported:\n"+Interpreter::searchin(basew[-2],NULL)+"Importing:\n"+
    basew[-1]->debugstring()+"\nLocals:"+callw[-1].locals->debugstring()+"\nThis:"+callw[-1].thiz->debugstring());
  }
}

void ZBINCALL Interpreter::polypop() {
  dopop(ip[1].i);
  ip+=2;
}
void ZBINCALL Interpreter::pushthis() {
  *basew++=callw[-1].thiz->addref();
  ip++;
}
void ZBINCALL Interpreter::pushnull() {
  *basew++=rt->getvoid()->addref();
  ip++;
}
void ZBINCALL Interpreter::pushsys() {
  *basew++=rt->getsys()->addref();
  ip++;
}
void ZBINCALL Interpreter::pushparam() {
  *basew++=callw[-1].basew[ip[1].i]->addref();
  ip+=2;
}
void ZBINCALL Interpreter::dup() {
  checkstack(ip[1].i);
  *basew++=basew[-ip[1].i]->addref();
  ip+=2;
}
void ZBINCALL Interpreter::pushlocals() {
  *basew++=callw[-1].locals->addref();// rt->getsys()->addref();
  ip++;
}
void ZBINCALL Interpreter::xchg() {
  checkstack(ip[1].i);checkstack(ip[2].i);
  exchange(basew[-ip[1].i],basew[-ip[2].i]);
  ip+=3;
}
/*
void ZBINCALL Interpreter::getlocal() {
  callw[-1].getvar(this,ip[1].s);
  ip+=2;
}
void ZBINCALL Interpreter::setlocal() {
  callw[-1].setvar(this,ip[1].s,basew[-1]);
  ip+=2;
}
*/
void ZBINCALL Interpreter::deleteprops() {
  if (!basew[-1]->deleteprops(ip[1].i)) {
    internalerror("Properties cannot be deleted");
  }
  ip+=2;
}
void ZBINCALL Interpreter::getprop() {
  checkstack(1);
  zbObject *res=basew[-1]->getprop(ip[1].s);
  if (res) {
    zbObject *old=basew[-1];
    basew[-1]=res->addref();
    old->release();
    ip+=2;
  } else {
    raisenoprop(basew[-1],ip[1].s);
  }
/*  if (res) {basew[-1]->release();basew[-1]=res;xok();}//new zbNumber(rt,128);xok();}
  else xnoprop(ip[1].s);
  ip+=2;*/
}
void ZBINCALL Interpreter::xgetprop() {
  checkstack(1);
  zbObject *res=basew[-1]->getprop(ip[1].s);
  if (res) {
    zbObject *old=basew[-1];
    basew[-1]=res->addref();
    old->release();
    ip+=2;
    xok();
  } else {
    xnoprop(ip[1].s);
    ip+=2;
  }
}
void ZBINCALL Interpreter::setprop() {
  checkstack(2);
  if (basew[-1]->setprop(ip[1].s,basew[-2])) {
    ip+=2;
  } else internalerror(fmtstr("Property cannot be set:%s",ip[1].s));
}
void ZBINCALL Interpreter::assignprop() {
  checkstack(2);
  if (basew[-1]->assignprop(ip[1].s,basew[-2])) {
    ip+=2;
  } else internalerror(fmtstr("Property cannot be assigned:%s",ip[1].s));
}
void ZBINCALL Interpreter::initprop() {
  checkstack(2);
  if (basew[-1]->initprop(ip[1].s,basew[-2])) {
    ip+=2;
  } else internalerror(fmtstr("Property cannot be initialized:%s",ip[1].s));
}


void ZBINCALL Interpreter::doret() {
//  while (tryw>trystack && tryw->callw==callw) tryw--;
  int cnt=ip[1].i;
  callw--;
  tryw=callw->tryw;
  for(zbObject **obj=callw->basew;obj<basew-cnt;obj++) (*obj)->release();
  for(int i=0;i<cnt;i++) callw->basew[i]=basew[i-cnt];
  basew=callw->basew+cnt;
//  while (basew>callw->basew) (*--basew)->release();
  callw->release();
  ip=callw->ret;
}

void ZBINCALL Interpreter::yield() {
  ip++;
  savedip=ip;
  ip=NULL;
}

void Interpreter::runinterrupted() {
  ip=savedip;
  interpret();
}

void ZBINCALL Interpreter::pushtry() {
  tryw->basew=basew;
  tryw->callw=callw;
  tryw->except=ip+ip[1].i;
  tryw++;
  ip+=2;
}
void ZBINCALL Interpreter::poptry() {
  #ifdef INTCHECK
  xassert(tryw>trystack);
  #endif
  tryw--;
  ip++;
}

void ZBINCALL Interpreter::co() {
  *basew++=rt->createinstance(ip[1].s);
  ip+=2;
}

void ZBINCALL Interpreter::callnear() {
  *basew++=new zbCodeAddress(rt,ip+2);
  ip+=ip[1].i;
}
void ZBINCALL Interpreter::retnear() {
  Instruction *ret=basew[-1]->getcodeaddress();
  if (!ret) internalerror("retnear:bad stack");
  ip=ret;
  (*--basew)->release();
}

void ZBINCALL Interpreter::fadd() {
  float res=basew[-1]->tofloat()+basew[-2]->tofloat();
  (*--basew)->release();basew[-1]->release();
  basew[-1]=new zbNumber(res,rt);
  ip++;
}
void ZBINCALL Interpreter::fmul() {
  float res=basew[-1]->tofloat()*basew[-2]->tofloat();
  (*--basew)->release();basew[-1]->release();
  basew[-1]=new zbNumber(res,rt);
  ip++;
}
void ZBINCALL Interpreter::fdiv() {
  double d1=basew[-1]->tofloat(),d2=basew[-2]->tofloat();
  if (fabs(d2)<0.0000001) {
    internalerror("fdiv:Divide by zero");
    return;
  }
  double res=d1/d2;
  (*--basew)->release();basew[-1]->release();
  basew[-1]=new zbNumber(res,rt);
  ip++;
}
void ZBINCALL Interpreter::fsub() {
  float res=basew[-1]->tofloat()-basew[-2]->tofloat();
  (*--basew)->release();basew[-1]->release();
  basew[-1]=new zbNumber(res,rt);
  ip++;
}
void ZBINCALL Interpreter::idiv() {
  int d1=basew[-1]->toint(),d2=basew[-2]->toint();
  if (d2==0) {
    internalerror("idiv:Divide by zero");
    return;
  }
  double res=d1/d2;
  (*--basew)->release();basew[-1]->release();
  basew[-1]=new zbNumber(res,rt);
  ip++;
}
void ZBINCALL Interpreter::imod() {
  int d1=basew[-1]->toint(),d2=basew[-2]->toint();
  if (d2==0) {
    internalerror("imod:Divide by zero");
    return;
  }
  double res=d1%d2;
  (*--basew)->release();basew[-1]->release();
  basew[-1]=new zbNumber(res,rt);
  ip++;
}
void ZBINCALL Interpreter::cat() {
  int cnt=ip[1].i;
  MEMSTROUT res;
  for(int i=1;i<=cnt;i++) res<<basew[-i]->tostring();
  for(int i=1;i<=cnt;i++) basew[-i]->release();
  basew-=cnt;
  *basew++=new zbString(res.str(),rt);
  ip+=2;
}
void ZBINCALL Interpreter::testeq() {
  bool res=basew[-1]==basew[-2];
  (*--basew)->release();basew[-1]->release();
  if (res) basew[-1]=rt->getsys()->addref();else basew[-1]=rt->getvoid()->addref();
  ip++;
}
void ZBINCALL Interpreter::feq() {
  bool res=basew[-1]->tofloat()==basew[-2]->tofloat();
  (*--basew)->release();basew[-1]->release();
  if (res) basew[-1]=rt->getsys()->addref();else basew[-1]=rt->getvoid()->addref();
  ip++;
}
void ZBINCALL Interpreter::fneq() {
  bool res=basew[-1]->tofloat()==basew[-2]->tofloat();
  (*--basew)->release();basew[-1]->release();
  if (res) basew[-1]=rt->getsys()->addref();else basew[-1]=rt->getvoid()->addref();
  ip++;
}
void ZBINCALL Interpreter::flt() {
  bool res=basew[-1]->tofloat()<basew[-2]->tofloat();
  (*--basew)->release();basew[-1]->release();
  if (res) basew[-1]=rt->getsys()->addref();else basew[-1]=rt->getvoid()->addref();
  ip++;
}
void ZBINCALL Interpreter::fgt() {
  bool res=basew[-1]->tofloat()>basew[-2]->tofloat();
  (*--basew)->release();basew[-1]->release();
  if (res) basew[-1]=rt->getsys()->addref();else basew[-1]=rt->getvoid()->addref();
  ip++;
}
void ZBINCALL Interpreter::flet() {
  bool res=basew[-1]->tofloat()<=basew[-2]->tofloat();
  (*--basew)->release();basew[-1]->release();
  if (res) basew[-1]=rt->getsys()->addref();else basew[-1]=rt->getvoid()->addref();
  ip++;
}
void ZBINCALL Interpreter::fget() {
  bool res=basew[-1]->tofloat()>=basew[-2]->tofloat();
  (*--basew)->release();basew[-1]->release();
  if (res) basew[-1]=rt->getsys()->addref();else basew[-1]=rt->getvoid()->addref();
  ip++;
}
void ZBINCALL Interpreter::faddc() {
  number_t res=basew[-1]->tofloat()+(*ip[1].pnum);
  basew[-1]->release();
  basew[-1]=new zbNumber(res,rt);
  ip+=2;
}
void ZBINCALL Interpreter::fmulc() {
  number_t res=basew[-1]->tofloat()*(*ip[1].pnum);
  basew[-1]->release();
  basew[-1]=new zbNumber(res,rt);
  ip+=2;
}


void ZBINCALL Interpreter::cm() {
  #ifdef INTCHECK
  xassert(basew-basestack>ip[2].i);
  #endif
  int parcnt=ip[2].i;
  const char *name=ip[1].s;
  ip+=3;
  if (basew[-1]->trycallmethod(this,name,parcnt)) {
  } else {
    zbObject *thiz=getfnthis(parcnt);
    internalerror(nomtdmsg(thiz,name,parcnt));
  }
}
void ZBINCALL Interpreter::xcm() {
  #ifdef INTCHECK
  xassert(basew-basestack>ip[2].i);
  #endif
  int parcnt=ip[2].i;
  const char *name=ip[1].s;
  ip+=3;
  if (basew[-1]->trycallmethod(this,name,parcnt)) {
    xok();
  } else {
    xnomtd(name,parcnt);
  }
}
void ZBINCALL Interpreter::jx() {
  if (xreg) ip+=ip[1].i;else ip+=2,xw=xstack,xreg=0;
  xreg=0;
}
void ZBINCALL Interpreter::jnx() {
  if (!xreg) ip+=ip[1].i,xw=xstack,xreg=0;else ip+=2;
  xreg=0;
}
void ZBINCALL Interpreter::condjmp() {
  if (basew[-1]!=rt->getvoid()) ip+=ip[1].i;else ip+=2;
  dopop();
}
void ZBINCALL Interpreter::condnotjmp() {
  if (basew[-1]==rt->getvoid()) ip+=ip[1].i;else ip+=2;
  dopop();
}
void ZBINCALL Interpreter::jmp() {
  ip+=ip[1].i;
}

string Interpreter::getxerror() {
  MEMSTROUT res;
  res<<"XError:symbol not found\n";
  while (xw>xstack) {
    xw--;
    res<<"interface="<<xw->object->intf->name<<";";
    xw->object->addref();xw->object->release();
    res<<"type="<<typeid(*xw->object).name()<<";";
    res<<xw->type<<"="<<xw->name<<"\n";
  }
  res<<"Arguments("<<xw->parcnt<<")\n";
  for(int i=0;i<xw->parcnt;i++) res<<xw->pars[i]->debugstring()<<"\n";
  return res.str();
}

void Interpreter::raise_error(zbObject *e) {
  if (tryw>trystack) {
    doraise();
    *basew++=e;
  } else {
    string s=e->tostring();
    e->release();
    throw InterpretException(s);
  }
}

void Interpreter::doraise() {
  tryw--;
  while (callw>tryw->callw) callw--,callw->release();
  ip=tryw->except;
  while (basew>tryw->basew) {
    if (eqstr(basew[-1]->intf->name,"ArrayIterator.instance")) {
      xthrowmsg(XError,("Chyba"));
    }
    (*--basew)->release();
  }
}
void ZBINCALL Interpreter::raise() {
  if (tryw>trystack) {
    zbObject *e=*--basew;
    doraise();
    *basew++=e;
  } else throw InterpretException(basew[-1]->tostring());
}
void ZBINCALL Interpreter::xraise() {
  if (tryw>trystack) {
    zbException *e=new zbException(rt);
    e->setmsg(getxerror());
    doraise();
    *basew++=e;
  } else throwxerror();
}

void ZBINCALL Interpreter::empty() {
  xthrowmsg(XError,("Calling of empty instruction:%s",comnames[ip[0].i]));
}

void Interpreter::_interpret() {
  if (stopnow) {
    savedip=ip;
    ip=NULL;
    stopnow=false;
    return;
  }
  #ifdef INTERPRETDEBUG
  Instruction *startip=ip;
  #endif
  while (ip) {
    #ifdef INTERPRETDEBUG
    eprintf("\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:%d:%s",ip-startip,comnames[ip->com]);
    switch (ip->com) {
//      case Coms::getlocal:case Coms::setlocal:
      case Coms::getprop:case Coms::setprop:
        case Coms::cm:case Coms::co:
          eprintf(" %s",ip[1].s);
        break;
      case Coms::jx:case Coms::jnx:
          eprintf(" %d",ip[1].i);
        break;
    }
    eputs("\n");
    #endif

//    void ZBINCALL (Interpreter::*act)()=instructions[ip[0].com];
//    int s=sizeof(act);
//    (this->*(act))();

    (this->*(instructions[ip[0].com]))();

    #ifdef INTERPRETDEBUG
    if (!ip) break;
    MEMSTROUT mso;
//    callw[-1].vars.print(mso);
    callw[-1].locals->vars.print(mso);
    eputs("Locals:\n");
    eputs(mso.str().c_str());
    if (zbInstance *zi=dynamic_cast<zbInstance*>(callw[-1].thiz)) {
      mso.clear();
      zi->vars.print(mso);
      eputs("Fields:\n");
      eputs(mso.str().c_str());
    }
//    ((zbInstance*)callw[-1].thiz)->vars.print(mso);


    int ss=basew-basestack;
    eprintf("****STACK****[%d]\n",ss);
    for(int i=ss;i>=1;i--) {
      char *prefix="  ";
//      if (basew-i==callw->result_w) prefix="> ";
      ewrite(basew[-i],prefix);
    }

    eprintf("TryStack:%d",tryw-trystack);
    #endif
  }
}

void Interpreter::interpret() {
  try {
    _interpret();
  } catch (JException &e) {
    throw JError(e.getmsg()+posinfo());
  }
}


}


