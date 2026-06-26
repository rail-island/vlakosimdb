#ifndef vectorsH
#define vectorsH

#include "mathcom.h"

namespace jmath {

template<class REAL> struct VECTOR {
  REAL *buf;int cnt;
  REAL &item (int index) {xcheckindex(index-1,cnt);return buf[index-1];}
  REAL &operator [] (int index) {return item(index);}
  VECTOR(int acnt) {cnt=acnt;buf=new REAL[max(cnt,1)];fillarray(buf,cnt,REAL(0));}
  VECTOR(const char *src);
  VECTOR(const VECTOR<REAL> &src);
  ~VECTOR() {delete []buf;}
  void operator =(const VECTOR<REAL> &src);
  void exchange(int a,int b) {REAL pom=item(a);item(a)=item(b);item(b)=pom;}
  void add(int dest,REAL koef,int src) {item(dest)+=item(src)*koef;}

  string str();
};


}

#endif
