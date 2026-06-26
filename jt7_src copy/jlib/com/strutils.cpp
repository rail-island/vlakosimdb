#include <jlib.h>
#pragma hdrstop

#include "strutils.h"

namespace jlib {

void CHARANAL::jumpoverfirst(const char *s) {
  char *p=strstr(act,s);
  if (p) act=p+strlen(s);else act=strend(act);
}

namespace str {

bool readto(const char *&c,char *w,char end,int bufsize) {
  if (bufsize) *w=0;
  while (*c and bufsize and *c!=end) *w++=*c++,bufsize--;
  if (bufsize) *w++=0;else w[-1]=0;
  if (*c==end) {c++;return true;}
  return false;
}

bool readto(char *&c,char **buf,char end) {
  const char *beg=c;
  while (*c and *c!=end) c++;
  *buf=challoc(c-beg+1);memmove(*buf,beg,c-beg);(*buf)[c-beg]=0;
  if (*c==end) {c++;return true;}
  return false;
}


}

}

//#pragma package(smart_init)
