#include <jlib.h>
#pragma hdrstop

#include "splines.h"

namespace jmath {

template<class REAL> string maple_natural_spline(int n,int a,int b,REAL (*f)(REAL)) {
  REAL *x=new REAL[n+1],*fx=new REAL[n+1];
  REAL h=REAL(b-a)/REAL(n);
  for(int i=0;i<=n;i++) x[i]=REAL(a)+i*h,fx[i]=f(x[i]);
  TObjVec<POLYNOM<REAL> > spl;
  natural_spline(n,x,fx,spl);

  ostringstream res;
  res<<"PIECEWISE(";
  for(int i=0;i<spl.count;i++) {
    res<<"["<<spl[i].pstr();
    if (i<spl.count-1) res<<",x<"<<x[i+1];else res<<",otherwise";
    res<<"]";
    if (i<spl.count-1) res<<",";
  }
  res<<")";
  delete []x;delete []fx;
  return res.str();
}

template<class REAL> void natural_spline(int n,REAL x[],REAL fx[],TObjVec<POLYNOM<REAL> > &res) {
#define lambda(_i) (h[(_i)-1]/(h[(_i)-1]+h[_i]))
#define ny(_i) (1-lambda(_i))
#define getm(_i) ((_i)==0?REAL(0):((_i)==n?REAL(0):M[_i]))
  REAL *h=new REAL[n];
  for(int i=0;i<n;i++) h[i]=x[i+1]-x[i];
  MATRIX<REAL> a(n-1,n-1);VECTOR<REAL> b(n-1);
  a[1][1]=2;
  a[1][2]=ny(1);
  for(int i=1;i<=n-1;i++) {
    REAL c1=(fx[i+1]-fx[i])/h[i];
    REAL c2=(fx[i]-fx[i-1])/h[i-1];
    REAL k=6/(h[i-1]+h[i]);
    b[i]=k*(c1-c2);
  }
  a[n-1][n-2]=lambda(n-1);
  a[n-1][n-1]=2;
  for(int i=2;i<=n-2;i++) {
    a[i][i-1]=lambda(i);
    a[i][i]=2;
    a[i][i+1]=ny(i);
  }
  a.gem(b);
  VECTOR<REAL> M(n-1);
  a.solve(b,M);
  for(int i=0;i<n;i++) {
    POLYNOM<REAL> c1,c2,fi;
    c1=power(POLYNOM<REAL>::linear(-x[i],1),3);
    c1*=getm(i+1)/6/h[i];
    c2=power(POLYNOM<REAL>::linear(x[i+1],-1),3);
    c2*=getm(i)/6/h[i];
    REAL ai=(fx[i+1]-fx[i])/h[i]-(getm(i+1)-getm(i))/6*h[i];
    REAL bi=fx[i]-getm(i)/6*h[i]*h[i];
    fi=c1+c2+POLYNOM<REAL>::linear(-x[i],1)*ai+POLYNOM<REAL>::linear(bi,0);
    res+=fi;
  }
  delete []h;
#undef lambda
#undef ny
#undef getm
}


}
#pragma package(smart_init)
