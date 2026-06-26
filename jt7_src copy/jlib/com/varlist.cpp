#include <jlib.h>
#pragma hdrstop

#include "varlist.h"

namespace jlib {


bool VARLISTRECORD::multiline() {
  return strchr(value,'\n');
}
void VARLISTRECORD::save(STROUT &f) {
  if (multiline()) {
    f<<"["<<name<<"]\n";
    f<<value<<"\n";
    f<<"[/"<<name<<"]\n";
  } else f<<name<<"="<<value<<"\n";
}

void VARLIST::initfromqstr(const char *str) {
  STDSTRANAL q(str);
  char name[0x100],value[0x100];
  while (q.jmpspaces(),q.aredata()) {
    q.readto('=',name);
    q>>STRANAL::QSTR(value);
    if (*name) setvar(name,value);
  }
}

int VARLIST::findvari(const char *name) {
  for(int i=0;i<count;i++) if (eqstr(buf[i].name,name)) return i;
  return -1;
}

const char *VARLIST::findvar(const char *name) {
  int idx=findvari(name);
  if (idx>=0) return buf[idx].value;
  return NULL;
}

const char *VARLIST::surefindvar(const char *name) {
  SimpleExceptType(XVarList_VarNotFound,XError);
  if (const char *res=findvar(name)) return res;
  xthrowmsg(XVarList_VarNotFound,("%s",name));
}

void VARLIST::setvar(const char *name,const char *value) {
  int idx=findvari(name);
  if (idx>=0) buf[idx].setvalue(value);else addnew(PrivateVarListRec(strnew(name),strnew(value)));
}
void VARLIST::_addvar(const char *name,const char *value) {
  addnew(PrivateVarListRec(strnew(name),strnew(value)));
}

void VARLIST::save(BINWRITE &fw) {
  fw<<count;
  for(int i=0;i<count;i++) fw<<buf[i].name,fw<<buf[i].value;
}
void VARLIST::load(BINREAD &fr) {
  clear();
  int cnt=fr.readi();wantcapa(cnt);
  for(int i=0;i<cnt;i++) {
    char *name=fr.readandallocs();
    char *value=fr.readandallocs();
    addnew(PrivateVarListRec(name,value));
  }
}

}
//#pragma package(smart_init)
