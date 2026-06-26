#include <jlib.h>
#pragma hdrstop

#include "ssmain.h"
#include <ext/cmstrdata.h>
#include <ext/xmlfile.h>

namespace ss {

struct ReturnValue {
  string val;
  ReturnValue(string val):val(val) {}
};

string FnDef::call(Strings &pars) {
  DEF_LOCALS
//  Analyser last=rt->a;
  Runtime rt(module);
  rt.a=body;
//  rt->a=body;
  for(int i=0;i<min(pars.count,args.count);i++) locs.setvar(args[i],pars[i]);
  try {
    rt.runblock();
  } catch (ReturnValue &rv) {
//    rt.a=last;
    return rv.val;
  }
//  rt->a=last;
  return locs.findvalue("result");
}

string Runtime::readidf() {
  char idf[0x100],*w=idf;
  while (a.is_idf() || *a=='$') *w++=*a,a++;*w=0;
  return idf;
}

string Runtime::ssexprhi() {
  a.jcw();
  if (*a=='(') {
    a++;
    string res=evalexpr();
    a.jcw();
    a.f_char(')');
    return res;
  } else if (a.is_idfbeg() || *a=='$') {
    return string("#[ref]")+readidf();
  } else if (a.is_digit() || *a=='-') {
    char num[0x100];
    a.readdouble(num);
    return string(num);
  } else if (*a=='"') {
    a++;
    char str[0x1000];
    a.readcstring(str);
    return string(str);
  } else if (*a=='\'') {
    a++;
    char str[0x1000];
    a.readcstring(str,'\'');
    return string(str);
  } else {
    a.error();
  }
  return string("");
}


string Runtime::ssexprdefer() {
  string res=ssexprhi();
  a.jcw();
  for(;;) {
    if (*a=='.') {
      a++;a.jcw();
      string right=readidf();
      res=module->getvalue(res);
      res=string("#[fld]")+right+"."+res;
    } else if (*a=='[') {
      a++;a.jcw();
      Strings pars;
      while (*a!=']') {
        pars.add(module->getvalue(evalexpr()));
        a.jcw();
        if (*a==',') a++,a.jcw();
      }
      if (*a) a++;a.jcw();
      res=module->callmethod(module->getvalue(res),"getindex",pars);
    } else if (*a=='(') {
      a++;a.jcw();
      Strings pars;
      while (*a!=')') {
        pars.add(module->getvalue(evalexpr()));
        a.jcw();
        if (*a==',') a++,a.jcw();
      }
      if (*a) a++;a.jcw();
      res=module->callobj(res,pars);
    } else break;
  }
  a.jcw();
  if (a.begjmp("++")) {
    string oldval=module->getvalue(res);
    string newval=ftos(oldval.fvalue()+1);
    module->doassign(res,newval);
    res=oldval;
  }
  return res;
}

string Runtime::ssexprmul() {
  string res=ssexprdefer();
  for(;;) {
    a.jcw();
    if (*a=='*') {
      a++;a.jcw();
      string right=ssexprdefer();
      res=ftos(atof(module->getvalue(res).c_str())*atof(module->getvalue(right).c_str()));
    } else if (*a=='/') {
      a++;a.jcw();
      string right=ssexprdefer();
      res=ftos(atof(module->getvalue(res).c_str())*atof(module->getvalue(right).c_str()));
    } else break;
  }
  return res;
}

string Runtime::ssexpradd() {
  string res=ssexprmul();
  for(;;) {
    a.jcw();
    if (*a=='+') {
      a++;a.jcw();
      string right=ssexprmul();
      res=ftos(atof(module->getvalue(res).c_str())+atof(module->getvalue(right).c_str()));
    } else if (*a=='-') {
      a++;a.jcw();
      string right=ssexprmul();
      res=ftos(atof(module->getvalue(res).c_str())*atof(module->getvalue(right).c_str()));
    } else if (*a=='&' && a[1]!='&') {
      a++;a.jcw();
      string right=ssexprmul();
      res=module->getvalue(res)+module->getvalue(right);
    } else break;
  }
  return res;
}

string Runtime::ssexprrel() {
  string res=ssexpradd();
  for(;;) {
    a.jcw();
    if (a.begjmp("<=")) {
      a.jcw();
      string right=ssexpradd();
      bool r=atof(module->getvalue(res).c_str())<=atof(module->getvalue(right).c_str());
      res=itos(r);
    } else if (a.begjmp(">=")) {
      a.jcw();
      string right=ssexpradd();
      bool r=atof(module->getvalue(res).c_str())>=atof(module->getvalue(right).c_str());
      res=itos(r);
    } else if (a.begjmp("<")) {
      a.jcw();
      string right=ssexpradd();
      bool r=atof(module->getvalue(res).c_str())<atof(module->getvalue(right).c_str());
      res=itos(r);
    } else if (a.begjmp(">")) {
      a.jcw();
      string right=ssexpradd();
      bool r=atof(module->getvalue(res).c_str())>atof(module->getvalue(right).c_str());
      res=itos(r);
    } else if (a.begjmp("==")) {
      a.jcw();
      string right=ssexpradd();
      bool r=module->getvalue(res)==module->getvalue(right);
      res=itos(r);
    } else if (a.begjmp("!=")) {
      a.jcw();
      string right=ssexpradd();
      bool r=module->getvalue(res)!=module->getvalue(right);
      res=itos(r);
    } else break;
  }
  return res;
}

string Runtime::ssexprlog() {
  bool neg=false;
  a.jcw();
  if (a.begjmp("!") || a.jmpidf("not"))
    neg=true;
  string res=ssexprrel();
  for(;;) {
    a.jcw();
    if (a.jmpidf("and") || a.begjmp("&&")) {
      a.jcw();
      string right=ssexprrel();
      bool r=module->tobool(module->getvalue(res)) && module->tobool(module->getvalue(right));
      res=itos(r);
    } else if (a.jmpidf("or") || a.begjmp("||")) {
      a.jcw();
      string right=ssexprrel();
      bool r=module->tobool(module->getvalue(res)) || module->tobool(module->getvalue(right));
      res=itos(r);
    } else break;
  }
  if (neg) res=itos(! module->tobool(module->getvalue(res)));
  return res;
}

string Runtime::evalexpr() {
  return ssexprlog();
}

void Runtime::run(char endch) {
//  DEFLOCALS(module)
  while (a.jcw(),*a && *a!=endch) runcom();
}

void Runtime::runcoms() {
  while (a.jcw(),*a && *a!='}') runcom();
}

void Runtime::jumpblock() {
  int level=0;
  a.jcw();a.f_char('{');a.jcw();
  while (*a) {
    if (*a=='{') {
      level++;
      a++;
    } else if (*a=='}') {
      a++;
      if (level==0) return;
      level--;
    } else if (*a=='"') {
      a++;
      a.jumpcstring();
    } else if (*a=='\'') {
      a++;
      a.jumpcstring('\'');
    } else a++;
    a.jcw();
  }
}

void Runtime::runblock() {
  a.jcw();a.f_char('{');a.jcw();
  runcoms();
  a.jcw();
  a.f_char('}');
  a.jcw();
}

void Runtime::runfor() {
  a.jcw();
  char name[0x40];
  a.f_readidf(name);
  a.jcw();a.f_char('=');
  string start=evalexpr();
  a.jcw();if (!a.jmpidf("to")) a.error("'to' expected");
  string stop=evalexpr();
  a.jcw();
  Analyser beg=a;
  double startf=atof(start.c_str()),stopf=atof(stop.c_str()),actf=startf;
  while (actf<=stopf) {
    module->locals->setvar(name,ftos(actf));
    runblock();
    a=beg;
    actf+=1;
  }
  jumpblock();
}

bool Runtime::evalcond(bool brackets) {
  if (brackets) a.jcw(),a.f_char('(');
  string cond=evalexpr();
  if (brackets) a.jcw(),a.f_char(')');
  return module->tobool(module->getvalue(cond));
}

void FnDef::load(Analyser &a) {
  a.jcw();a.f_readidf(name);
  a.jcw();a.f_char('(');
  a.jcw();
  while (*a && *a!=')') {
    char arg[0x100];
    a.f_readidf(arg);
    args.add(arg);
    a.jcw();
    if (*a==',') a++,a.jcw();
  }
  a.f_char(')');
  a.jcw();body=a;
}

void Runtime::runcom() {
  a.jcw();
  if (a.jmpidf("while")) {
    Analyser beg=a;
    for(;;) {
      a=beg;
      if (evalcond()) {
        runblock();
      } else {
        jumpblock();
        break;
      }
    }
  } else if (a.jmpidf("do")) {
    Analyser beg=a;
    while (*a) {
      a=beg;
      runblock();
      a.jcw();
      if (!evalcond()) break;
    }
    a.jcw();if (*a==';') a++,a.jcw();
  } else if (a.jmpidf("for")) {
    runfor();
  } else if (a.jmpidf("sub")) {
    RefBase<FnDef> sub=new FnDef(module);
    sub->load(a);
    module->addsub(sub);
//    if (module->subs.searchtype(sub->name)<0) module->subs.add(sub);else delete sub;
    jumpblock();
  } else if (a.jmpidf("if")) {
    bool act=evalcond(),was=act;
    doblock(act);
    for(;;) {
      a.jcw();
      if (a.jmpidf("elif")) {
        act=evalcond();
      } else if (a.jmpidf("else")) {
        doblock( ! was);
        break;
      } else break;
      doblock(act && ! was);
      was = was || act;
    }
  } else if (a.jmpidf("return")) {
    a.jcw();
    if (*a==';' || *a=='}') throw ReturnValue(module->locals->findvalue("result"));
    string res=evalexpr();
    throw ReturnValue(module->getvalue(res));
  } else if (*a=='{') {
    runblock();
  } else {
    string expr=evalexpr();
    a.jcw();
    if (*a=='=') {
      a++;a.jcw();
      string right=evalexpr();
      module->doassign(expr,module->getvalue(right));
    }
  }
  a.jcw();
  if (*a==';') a++,a.jcw();
}


}


