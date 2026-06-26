#ifndef polynomsH
#define polynomsH

#include "mathcom.h"
#include "vectors.h"

namespace jmath {

template<class REAL> struct POLYNOM:VECTOR<REAL> {
  POLYNOM(const char *src):VECTOR<REAL>(src) {}
  POLYNOM(int cnt=0):VECTOR<REAL>(cnt) {}
  int DEG() const {return cnt-1;}
  REAL operator[](int index) const {xcheckindex(index,cnt);return buf[index];}
  REAL &operator[](int index) {xcheckindex(index,cnt);return buf[index];}
  static POLYNOM<REAL> linear(REAL a0,REAL a1);
  string pstr(const char *variable="x");
};

template<class REAL> POLYNOM<REAL> operator +(const POLYNOM<REAL> &a,const POLYNOM<REAL> &b);
template<class REAL> POLYNOM<REAL> operator -(const POLYNOM<REAL> &a);
template<class REAL> POLYNOM<REAL> inline operator -(const POLYNOM<REAL> &a,const POLYNOM<REAL> &b) {return a+(-b);}
template<class REAL> POLYNOM<REAL> operator *(const POLYNOM<REAL> &a,const POLYNOM<REAL> &b);

template<class REAL> POLYNOM<REAL> operator *=(const POLYNOM<REAL> &a,const POLYNOM<REAL> &b);

template<class REAL> POLYNOM<REAL> power(const POLYNOM<REAL> &a,int k);//mocnina

template<class REAL> void inline operator *=(POLYNOM<REAL> &a,REAL k) {for(int i=0;i<a.cnt;i++) a[i]*=k;}
template<class REAL> void inline operator /=(POLYNOM<REAL> &a,REAL k) {for(int i=0;i<a.cnt;i++) a[i]/=k;}

template<class REAL> void inline operator *=(POLYNOM<REAL> &a,const POLYNOM<REAL> &b) {a=a*b;}
template<class REAL> void inline operator /=(POLYNOM<REAL> &a,const POLYNOM<REAL> &b) {a=a/b;}
template<class REAL> void inline operator +=(POLYNOM<REAL> &a,const POLYNOM<REAL> &b) {a=a+b;}
template<class REAL> void inline operator -=(POLYNOM<REAL> &a,const POLYNOM<REAL> &b) {a=a-b;}



template<class REAL> POLYNOM<REAL> inline operator *(const POLYNOM<REAL> &a,REAL k) {POLYNOM<REAL> res=a;res*=k;return res;}
template<class REAL> POLYNOM<REAL> inline operator /(const POLYNOM<REAL> &a,REAL k) {POLYNOM<REAL> res=a;res/=k;return res;}

}
#endif
