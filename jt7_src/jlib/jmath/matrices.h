#ifndef matricesH
#define matricesH

#include "mathcom.h"
#include "vectors.h"

namespace jmath {

template<class REAL> struct MATRIXLINE {
  REAL *p;int cnt;
  REAL &operator [] (int index) {xcheckindex(index-1,cnt);return p[index-1];}
  MATRIXLINE(REAL *abuf,int acnt) {p=abuf;cnt=acnt;}
  void add(MATRIXLINE src,REAL koef) {
    for(int i=0;i<cnt;i++) p[i]+=src.p[i]*koef;
  }
};


template<class REAL> struct MATRIX {
  int lines,cols;
  REAL *buf;
  MATRIX(int alines,int acols) {lines=alines;cols=acols;buf=new REAL[lines*cols];fillarray(buf,lines*cols,REAL(0));}
  ~MATRIX() {delete []buf;}
  MATRIXLINE<REAL> operator [] (int index) {xcheckindex(index-1,lines);return MATRIXLINE<REAL>(buf+(index-1)*cols,cols);}
  MATRIXLINE<REAL> line(int index) {xcheckindex(index-1,lines);return MATRIXLINE<REAL>(buf+(index-1)*cols,cols);}

  MATRIX(const char *src);
  MATRIX(const MATRIX<REAL> &src) {
    lines=src.lines;cols=src.cols;
    buf=new REAL[lines*cols];memmove(buf,src.buf,sizeof(REAL)*liens*cols))
  }
  void operator =(const MATRIX<REAL> &src) {
    delete []buf;
    lines=src.lines;cols=src.cols;
    buf=new REAL[lines*cols];memmove(buf,src.buf,sizeof(REAL)*liens*cols))
  }

  void exchange(int A,int B);//vymeni radky A a B
  void gem(VECTOR<REAL> &b,STROUT *f=NULL);//Gaussova eliminacni metoda,b:vektor pravych stran

  void add(int dest,REAL koef,int src) {line(dest).add(line(src),koef);}

  string str();

  void solve(VECTOR<REAL> &b,VECTOR<REAL> &x);//musi byt pouzito na horni trujuhelnikovou matici,
  //b je vektor pravych stran,x je reseni

  string mstr();
  const char *debugstr();
};


}
#endif

