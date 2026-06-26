#include <jlib.h>
#pragma hdrstop

#include "mathcom.h"

namespace jmath {

char *read(float &val,const char *c) {
  STDSTRANAL q(c);q>>val;return q.act;
}
char *read(double &val,const char *c) {
  STDSTRANAL q(c);q>>val;return q.act;
}
char *read(long double &val,const char *c) {
  double v;
  STDSTRANAL q(c);q>>v;val=v;return q.act;
}
char *read(FRACTION &val,const char *c) {
  STDSTRANAL q(c);q>>val.p;
  if (q.act=="/") q>>val.q;else val.q=1;
  return q.act;
}

int NSD(int a,int b) {
  while (a>0 and b>0) {
    if (a>b) a=a-b;else b=b-a;
  }
  return max(a,b);
}

void FRACTION::simplify() {
  if (p) {
    int nsd=NSD(abs(p),abs(q));
    if (q<0) nsd=-nsd;
    p/=nsd;q/=nsd;
  } else q=1;
}

}
#pragma package(smart_init)
