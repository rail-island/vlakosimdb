#include <jlib.h>
#pragma hdrstop

#include "gmexpr.h"
#include "gmanal.h"

namespace gram {

int parseexpr(Analyser &a,IVariables &vars);

static int parseelem(Analyser &a,IVariables &vars) {
  if (a.is_digit()) return a.readint();
  if (a.is_idfbeg()) {
    char name[0x100];
    a.readidf(name);
    return vars.getivalue(name);
  }
  if (*a=='(') {
    a++;
    int res=parseexpr(a,vars);
    if (*a==')') a++;
    return res;
  }
  return 0;
}

static int parsemul(Analyser &a,IVariables &vars) {
  int res=parseelem(a,vars);
  while (*a) {
    switch (*a) {
      case '*':a++;res*=parseelem(a,vars);break;
      case '/':a++;res/=parseelem(a,vars);break;
      case '%':a++;res%=parseelem(a,vars);break;
      default:return res;
    }
  }
  return res;
}

static int parseadd(Analyser &a,IVariables &vars) {
  int res=parsemul(a,vars);
  while (*a) {
    switch (*a) {
      case '+':a++;res+=parsemul(a,vars);break;
      case '-':a++;res-=parsemul(a,vars);break;
      default:return res;
    }
  }
  return res;
}

static int parseexpr(Analyser &a,IVariables &vars) {
  return parseadd(a,vars);
}

int evalexpr(const char *expr,IVariables &vars) {
  Analyser a(expr);
  return parseexpr(a,vars);
}

}


