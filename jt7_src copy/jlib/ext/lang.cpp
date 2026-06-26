#include <jlib.h>
#pragma hdrstop

#include "lang.h"
#include <com/strutils.h>

namespace jlib {

LANG::SECT &LANG::getsect(const char *name) {
  for(int i=0;i<sects.count;i++) if (eqstr(sects[i]->name,name)) return *sects[i];
  return emptysect;
}

const char *LANG::SECT::getval(const char *name) {
  for(int i=0;i<vars.count;i++) if (eqstr(vars[i]->name,name)) return vars[i]->value;
  return "";
}

LANG::LANG(const char *buf) {
  char *c=(char*)buf;
  while (*c) {
    if (*c=='>') sects+=new SECT(c);
    c=readline(c);
  }
}

char *_loadtext_(char *c,char **val) {
  char buf[0x1000],*w=buf;
  if (strbegcmp(">>>>",c)) {
    c+=4;while (*c and not strbegcmp("<<<<",c)) *w++=*c++;
    if (strbegcmp("<<<<",c)) c+=4;
  } else {
    while (*c and *c!='\n') {
      if (*c=='/') c++,*w++='\n',jmpspaces(c);else
      if (*c=='_') c++,jmpspaces(c);else
      if (*c=='\\' and c[1]!='#') {c++;if (*c) *w++=*c++;} else
      *w++=*c++;
    }
    if (*c) c++;
  }
  *w=0;
  xcheckstrlen(buf);
  *val=strnew(buf);
  return c;
}

LANG::SECT::SECT(char *&c) {
  c++;
  str::readto(c,&name,'\n');
  trimr(name);
  jmpspaces(c);
  while (*c!='<' and *c) {
    char *naz,*val;
    str::readto(c,&naz,'=');
    c=_loadtext_(c,&val);
    vars+=new VAR(naz,val);
    jmpspaces(c);
  }
}


}

#pragma package(smart_init)
