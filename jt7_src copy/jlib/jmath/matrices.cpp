#include <jlib.h>
#pragma hdrstop

#include "matrices.h"

namespace jmath {

template<class REAL> string MATRIX<REAL>::mstr() {
  ostringstream os;
  MATRIX<REAL> &a=*this;
  for(int i=1;i<=lines;i++) {
    os<<"[";
    for(int j=1;j<=cols;j++) {
      write(a[i][j],os);
      if (j<cols) os<<",";
    }
    os<<"]";
    if (i<lines) os<<",";
  }
  return os.str();
}
template<class REAL> const char *MATRIX<REAL>::debugstr() {
  static char buf[0x100];
  strcpy(buf,mstr().c_str());
  return buf;
}

template<class REAL> void MATRIX<REAL>::exchange(int A,int B) {
  MATRIXLINE<REAL> a=line(A),b=line(B);
  for(int i=1;i<=cols;i++) {
    REAL pom=a[i];a[i]=b[i];b[i]=pom;
  }
}

template<class REAL> void MATRIX<REAL>::gem(VECTOR<REAL> &b,STROUT *f) {
  xassert(b.cnt==lines);
  MATRIX<REAL> &a=*this;
  for(int i=1;i<=lines;i++) {
    for(int j=i;j<=lines;j++) if (nozero(a[i][j])) {exchange(i,j);b.exchange(i,j);break;}
    for(int j=i+1;j<=lines;j++) {
      REAL k=-a[j][i]/a[i][i];
      add(j,k,i);
      b.add(j,k,i);
    }
    if (f) *f<<str()<<b.str()<<"\n============================================\n";
  }
}

template<class REAL> MATRIX<REAL>::MATRIX(const char *src) {
  STDSTRANAL q(src);
  q>>lines>>cols;
  src=q.act;
  buf=new REAL[lines*cols];
  MATRIX<REAL> &a=*this;
  for(int i=1;i<=lines;i++)
    for(int j=1;j<=cols;j++) src=read(a[i][j],src);
}

template<class REAL> void MATRIX<REAL>::solve(VECTOR<REAL> &b,VECTOR<REAL> &x) {
  xassert(lines==cols);
  xassert(b.cnt==lines);
  xassert(x.cnt==lines);
  MATRIX<REAL> &a=*this;
  for(int i=lines;i>=1;i--) {
    REAL s=0;
    for(int j=i+1;j<=cols;j++) s+=a[i][j]*x[j];
    REAL u=b[i]-s;
    x[i]=u/a[i][i];
  }
}

template<class REAL> string MATRIX<REAL>::str() {
  MEMSTROUT os;
  MATRIX<REAL> &a=*this;
  os<<lines<<" "<<cols<<"\n";
  for(int i=1;i<=lines;i++) {
    for(int j=1;j<=cols;j++) write(a[i][j],os),os<<" ";
    os<<"\n";
  }
  return os.str();
}

}
#pragma package(smart_init)



