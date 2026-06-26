#include <jlib.h>
#pragma hdrstop

#include "polynoms.h"

namespace jmath {

template<class REAL> POLYNOM<REAL> power(const POLYNOM<REAL> &a,int k) {
  POLYNOM<REAL> res("1 1");
  for(int i=0;i<k;i++) res*=a;
  return res;
}

template<class REAL> string POLYNOM<REAL>::pstr(const char *variable) {
  ostringstream os;
  bool writed=false;
  for(int i=cnt-1;i>=0;i--) if (nozero(buf[i])) {
    REAL x=buf[i];
    if (x>0 and writed) os<<"+";
    write(buf[i],os);
    if (i>0) os<<"*"<<variable;
    if (i>1) os<<"^"<<i;
    writed=true;
  }
  return os.str();
}

template<class REAL> POLYNOM<REAL> POLYNOM<REAL>::linear(REAL a0,REAL a1) {
  POLYNOM<REAL> res(2);
  res[0]=a0;
  res[1]=a1;
  return res;
}

template<class REAL> POLYNOM<REAL> operator +(const POLYNOM<REAL> &a,const POLYNOM<REAL> &b) {
  POLYNOM<REAL> res(max(a.cnt,b.cnt));
  if (a.cnt<b.cnt) {
    for(int i=0;i<a.cnt;i++) res[i]=a[i]+b[i];
    for(int i=a.cnt;i<b.cnt;i++) res[i]=b[i];
  } else {
    for(int i=0;i<b.cnt;i++) res[i]=a[i]+b[i];
    for(int i=b.cnt;i<a.cnt;i++) res[i]=a[i];
  }
  return res;
}

template<class REAL> POLYNOM<REAL> operator -(const POLYNOM<REAL> &a) {
  POLYNOM<REAL> res(a.cnt);
  for(int i=0;i<a.cnt;i++) res[i]=-a[i];
  return res;
}

template<class REAL> POLYNOM<REAL> operator *(const POLYNOM<REAL> &a,const POLYNOM<REAL> &b) {
  POLYNOM<REAL> res(a.DEG()+b.DEG()+1);
  for(int i=0;i<a.cnt;i++)
    for(int j=0;j<b.cnt;j++)
      res[i+j]+=a[i]*b[j];
  return res;
}


}
#pragma package(smart_init)
