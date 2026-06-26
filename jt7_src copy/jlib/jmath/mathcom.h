#ifndef mathcomH
#define mathcomH

namespace jmath {

const float floatepsilon=1.0E-5;
const double doubleepsilon=1.0E-7;
const long double longdoubleepsilon=1.0E-8;

struct FRACTION {
  int p,q;
  void simplify();
  FRACTION(int a,int b) {p=a;q=b;simplify();}
  FRACTION(int a) {p=a;q=1;}
  FRACTION() {p=0;q=1;}
};

struct mstring {
  string expr;
  explicit mstring(string aexpr) {expr=aexpr;}
  mstring() {}
  mstring(int) {expr="0";}
  void operator +=(mstring v);
  void operator -=(mstring v);
  void operator *=(mstring v);
  void operator /=(mstring v);
};
inline mstring operator +(mstring left,mstring right) {
  string res;
  res<<"("<<left.expr<<"+"<<right.expr<<")";
  return mstring(res);
}
inline mstring operator -(mstring left,mstring right) {
  string res;
  res<<"("<<left.expr<<"-"<<right.expr<<")";
  return mstring(res);
}
inline mstring operator *(mstring left,mstring right) {
  string res;
  res<<"("<<left.expr<<"*"<<right.expr<<")";
  return mstring(res);
}
inline mstring operator /(mstring left,mstring right) {
  string res;
  res<<"("<<left.expr<<"/"<<right.expr<<")";
  return mstring(res);
}
mstring operator -(mstring arg) {
  string res;
  res<<"-("<<arg.expr<<")";
  return mstring(res);
}
inline void mstring::operator +=(mstring v) {expr=(*this+v).expr;}
inline void mstring::operator -=(mstring v) {expr=(*this-v).expr;}
inline void mstring::operator *=(mstring v) {expr=(*this*v).expr;}
inline void mstring::operator /=(mstring v) {expr=(*this/v).expr;}

bool nozero(float x) {return fabs(x)>floatepsilon;}
bool nozero(double x) {return fabs(x)>doubleepsilon;}
bool nozero(long double x) {return fabs(x)>longdoubleepsilon;}
bool nozero(FRACTION x) {return x.p;}
bool nozero(mstring ms) {return true;}

int NSD(int a,int b);

inline FRACTION operator+(FRACTION a,FRACTION b) {return FRACTION(a.p*b.q+b.p*a.q,a.q*b.q);}
inline FRACTION operator-(FRACTION a,FRACTION b) {return FRACTION(a.p*b.q-b.p*a.q,a.q*b.q);}
inline FRACTION operator-(FRACTION a) {return FRACTION(-a.p,a.q);}
inline FRACTION operator*(FRACTION a,FRACTION b) {return FRACTION(a.p*b.p,a.q*b.q);}
inline FRACTION operator/(FRACTION a,FRACTION b) {return FRACTION(a.p*b.q,a.q*b.p);}

inline void operator +=(FRACTION &a,FRACTION f) {a=a+f;}
inline void operator -=(FRACTION &a,FRACTION f) {a=a-f;}
inline void operator *=(FRACTION &a,FRACTION f) {a=a*f;}
inline void operator /=(FRACTION &a,FRACTION f) {a=a/f;}

inline bool operator ==(const FRACTION &a,const FRACTION &b) {return a.p*b.q==a.q*b.p;}
inline bool operator !=(const FRACTION &a,const FRACTION &b) {return a.p*b.q!=a.q*b.p;}
inline bool operator >(const FRACTION &a,const FRACTION &b) {return a.p*b.q>a.q*b.p;}
inline bool operator <(const FRACTION &a,const FRACTION &b) {return a.p*b.q<a.q*b.p;}
inline bool operator >=(const FRACTION &a,const FRACTION &b) {return a.p*b.q>=a.q*b.p;}
inline bool operator <=(const FRACTION &a,const FRACTION &b) {return a.p*b.q<=a.q*b.p;}


string fractostr(FRACTION f) {return itos(f.p)+"/"+itos(f.q);}

char *read(float &val,const char *c);
char *read(double &val,const char *c);
char *read(long double &val,const char *c);
char *read(FRACTION &val,const char *c);

void write(float val,STROUT &f) {if (nozero(val)) f<<val;else f<<"0";}
void write(double val,STROUT &f) {if (nozero(val)) f<<val;else f<<"0";}
void write(long double val,STROUT &f) {if (nozero(val)) f<<double(val);else f<<"0";}
void write(FRACTION val,STROUT &f) {f<<val.p<<"/"<<val.q;}
void write(mstring val,STROUT &f) {f<<val.expr
;}

template<class REAL> inline REAL sgn(REAL x) {if (x<0) return -1;if (x>0) return 1;return 0;}

}

#endif
