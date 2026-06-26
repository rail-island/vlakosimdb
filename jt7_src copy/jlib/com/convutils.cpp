#include <jlib.h>
#pragma hdrstop

#include "convutils.h"

namespace jlib {

int anytoi(char *&c,char beg,char end) {
  if (*c=='-') {c++;return -1;}
  int res=0;
  int mul=end-beg+1;
  while (*c>=beg && *c<=end) res*=mul,res+=*c-beg,c++;
  return res;
}

void itoany(char *&w,int num,char beg,char end) {
  if (num<0) {*w++='-';return;}
  int mul=end-beg+1;
  if (num==0) {*w++=beg;return;}
  char pb[0x100],*pw=pb;
  while (num>0) {
    *pw++=(num%mul)+beg;
    num/=mul;
  }
  while (--pw>=pb) *w++=*pw;
}


}



