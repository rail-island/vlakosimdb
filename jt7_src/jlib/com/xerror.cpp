#include <jlib.h>
#pragma hdrstop

#include "xerror.h"

namespace jlib {

void (*showXErrorMessage)(const char *msg)=NULL;

void XError::initfmt(const char *format,...) {
  va_list args;
  char s[0x1000];
  va_start(args,format);
  vsprintf(s,format,args);
  va_end(args);
  xcheckstrlen(s);
  msg=s;
}

string XError::exceptname() {
//  return typeid(*this).name();
  return "???";
}

void XError::handle() {
  XErrorMessage(exceptname()+"\n"+formatmessage()+"\n(tento popis je v souboru stderr)");
  eprintf("%s\n%s\n--------------------------\n\n",exceptname().c_str(),formatmessage().c_str());
}

string XIndexError::formatmessage() {
  return fmtstr("index=%d,count=%d;\n%s",index,count,msg.c_str());
}

}

//#pragma package(smart_init)
