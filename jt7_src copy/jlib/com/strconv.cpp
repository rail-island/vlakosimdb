#include <jlib.h>
#pragma hdrstop
#include "strconv.h"

namespace jlib {

StrConv& StrConv::operator >>(void (*convproc)(char *changedbuf)) {
  char *b=strnew(str.c_str());
  convproc(b);
  str=b;
  free(b);
  return *this;
}

}
//#pragma package(smart_init)
