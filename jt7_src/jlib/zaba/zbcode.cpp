#include <jlib.h>
#pragma hdrstop

#include "zbcode.h"

namespace zaba {

#define _foriter for(Instruction *in=code.buf;in->com!=Coms::END;in+=comlens[in->com])

void Code::addins(int instruction,Instruction p1,Instruction p2,Instruction p3) {
  int res=code.count;
  _add(instruction);
  int pc=compars[instruction];
  if (pc>0) _add(p1);pc--;
  if (pc>0) _add(p2);pc--;
  if (pc>0) _add(p3);pc--;
}

void Code::_idfp(Analyser &a) {
  a.jsp();
  char name[0x100];
  if (a.is_white() || *a==0) a.error("Identifier expected");
  a.readto(name,0x100," \n",false);
  trimr(name);
  strs.add(name);
  _add(strs.last());
}
void Code::_strp(Analyser &a) {
  a.jsp();a.f_char('"');
  string s=a.readcstring();
  zbObject *obj=createString(s,rt);
  objs.add(obj);
  _add(obj);
}
void Code::_intp(Analyser &a) {
  a.jsp();if (*a=='@') a++;
  if (! a.is_digit()) a.error("Integer expected");
  _add(a.readint());
}
void Code::_flgp(Analyser &a) {
  a.jsp();
  int res=0;
  int flag=1;
  while (*a=='0' || *a=='1') {
    if (*a=='1') res|=flag;
    flag*=2;
    a++;
  }
  _add(res);
}

void Code::_nump(Analyser &a) {
  if (! a.is_digit() && *a!='-' && *a!='.') a.error("Number expected");
  zbObject *obj=createNumber(a.readdouble(),rt);
  objs.add(obj);
  _add(obj);
}

void Code::_floatp(Analyser &a) {
  if (! a.is_digit() && *a!='-' && *a!='.') a.error("Number expected");
  number_t *d=new number_t(a.readdouble());
  numbers.add(d);
  _add(d);
}


struct Label_T {
  int label;
  int dest;
};

void Code::linklabels() {
  TCntVec<Label_T> labels;
  Instruction *w=code.buf;
  for(Instruction *ip=code.buf;ip->com!=Coms::END;) {
    Instruction *next=ip+comlens[ip->com];
    if (ip->com==Coms::label) {
      Label_T &lab=labels.add();
      lab.label=ip[1].i;
      lab.dest=w-code.buf;
      ip=next;
    } else {
      while (ip<next) *w++=*ip++;
    }
  }
  *w++=Coms::END;
  code.count=w-code.buf;
  _foriter {
    switch (in[0].com) {
      case Coms::condjmp:case Coms::condnotjmp:case Coms::jmp:case Coms::jx:case Coms::jnx:
      case Coms::pushtry:case Coms::callnear:{
        bool found=false;
        for(int i=0;i<labels.count;i++) if (labels[i].label==in[1].i) {
          in[1].i=labels[i].dest-(in-code.buf);
          found=true;
          break;
        }
        if (!found) throw JError("Label not found:"+itos(in[1].i));
      }
    }
  }
}

void Code::load(Analyser &a) {
  if (!a.begjmp("code")) a.error("code expected");
  a.roll();
  while (*a) {
    if (*a==';') a.roll();//comment
    else if (a.begins("end")) {
      a.roll();break;
    } else if (a.seekch()) {
      a.roll();
    } else if (*a=='@') {
      a++;
      addins(Coms::label,a.readint());
      a.roll();
    } else {
      char ins[0x100];
      a.readidf(ins);
      a.jsp();
      STRING in=ins;
      if (in=="cm") _add(Coms::cm),_idfp(a),_intp(a);
      else if (in=="xcm") _add(Coms::xcm),_idfp(a),_intp(a);
      else if (in=="setprop") _add(Coms::setprop),_idfp(a);
      else if (in=="initprop") _add(Coms::initprop),_idfp(a);
      else if (in=="assignprop") _add(Coms::assignprop),_idfp(a);
      else if (in=="getprop") _add(Coms::getprop),_idfp(a);
      else if (in=="xgetprop") _add(Coms::xgetprop),_idfp(a);
      else if (in=="deleteprops") _add(Coms::deleteprops),_intp(a);
      else if (in=="jmp") _add(Coms::jmp),_intp(a);
      else if (in=="condjmp") _add(Coms::condjmp),_intp(a);
      else if (in=="condnotjmp") _add(Coms::condnotjmp),_intp(a);
      else if (in=="pushtry") _add(Coms::pushtry),_intp(a);
      else if (in=="poptry") _add(Coms::poptry);
      else if (in=="doret") _add(Coms::doret),_intp(a);
      else if (in=="pushnum") _add(Coms::pushobj),_nump(a);
      else if (in=="pushstr") _add(Coms::pushobj),_strp(a);
      else if (in=="pushthis") _add(Coms::pushthis);
      else if (in=="pushsys") _add(Coms::pushsys);
      else if (in=="pushnull") _add(Coms::pushnull);
      else if (in=="pushlocals") _add(Coms::pushlocals);
      else if (in=="pushparam") _add(Coms::pushparam),_intp(a);
      else if (in=="raise") _add(Coms::raise);
      else if (in=="xraise") _add(Coms::xraise);
      else if (in=="co") _add(Coms::co),_idfp(a);
      else if (in=="jx") _add(Coms::jx),_intp(a);
      else if (in=="jnx") _add(Coms::jnx),_intp(a);
      else if (in=="pop") _add(Coms::pop);
      else if (in=="xchg") _add(Coms::xchg),_intp(a),_intp(a);
      else if (in=="polypop") _add(Coms::polypop),_intp(a);
      else if (in=="dup") _add(Coms::dup),_intp(a);
      else if (in=="move") _add(Coms::move),_intp(a),_intp(a);
      else if (in=="find") _add(Coms::find),_flgp(a),_idfp(a);
      else if (in=="join") _add(Coms::join);
      else if (in=="callnear") _add(Coms::callnear),_intp(a);
      else if (in=="retnear") _add(Coms::retnear);
      else if (in=="fadd") _add(Coms::fadd);
      else if (in=="fmul") _add(Coms::fmul);
      else if (in=="fsub") _add(Coms::fsub);
      else if (in=="fdiv") _add(Coms::fdiv);
      else if (in=="idiv") _add(Coms::idiv);
      else if (in=="imod") _add(Coms::imod);
      else if (in=="feq") _add(Coms::feq);
      else if (in=="fneq") _add(Coms::fneq);
      else if (in=="flt") _add(Coms::flt);
      else if (in=="fgt") _add(Coms::fgt);
      else if (in=="flet") _add(Coms::flet);
      else if (in=="fget") _add(Coms::fget);
      else if (in=="faddc") _add(Coms::faddc),_floatp(a);
      else if (in=="fmulc") _add(Coms::fmulc),_floatp(a);
      else if (in=="cat") _add(Coms::cat),_intp(a);
      else if (in=="testeq") _add(Coms::testeq);
      else if (in=="yield") _add(Coms::yield);
      else a.error(fmtstr("Unknown instruction:%s",in));
      a.roll();
    }
  }
  _add(Coms::END);
  linklabels();
}



}



