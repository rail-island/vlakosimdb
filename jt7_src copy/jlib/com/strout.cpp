#include <jlib.h>
#pragma hdrstop

#include "strout.h"

namespace jlib {

string MEMSTROUT::str() {
  flushbuf();
  int size=bms.size();
  string res;
  bms.savedata(BUFWRITER(res.reservebuffer(size),size));
  res.setlen(size);
  return res;
}

void STROUT::writecstring(const char *s,char edge) {
  STROUT &fw=*this;
  fw<<edge;
  while (*s) {
    switch (*s) {
      case '\n':fw<<'\\'<<'n';break;
      case '\\':fw<<'\\'<<'\\';break;
      case '\'':fw<<'\\'<<'\'';break;
      case '"':fw<<'\\'<<'"';break;
      default:fw<<*s;break;
    }
    s++;
  }
  fw<<edge;
}

STROUT &STROUT::operator <<(QSTR str) {
  flushbuf();
  char *buf=new char[strlen(str.buf)*2+3];
  char *c=str.buf,*w=buf;
  *w++='"';
  while (*c) {*w++=*c;if (*c=='"') *w++='"';c++;}
  *w++='"';
  strm->writedata(buf,w-buf);
  delete []buf;
  return *this;
}


}

//#pragma package(smart_init)
