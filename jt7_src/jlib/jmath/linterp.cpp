#include <jlib.h>
#pragma hdrstop

#include "linterp.h"

namespace jmath {

template<class REAL> POLYNOM<REAL> lagreange_interpolation(int n,REAL x[],REAL fx[]) {
  POLYNOM<REAL> res;
  for(int i=0;i<=n;i++) {
    POLYNOM<REAL> p=POLYNOM<REAL>::linear(1,0);//knstantni jednicka
    for(int j=0;j<=n;j++) if (i!=j) p*=POLYNOM<REAL>::linear(-x[j],1);//linearni faktor (x-x[j])
    REAL q=1;
    for(int j=0;j<=n;j++) if (i!=j) q*=(x[i]-x[j]);
    res+=p*fx[i]/q;
  }
  return res;
}

template<class REAL> string maple_lagreange_interpolation(int n,int a,int b,REAL (*f)(REAL)) {
  REAL *x=new REAL[n+1],*fx=new REAL[n+1];
  REAL h=REAL(b-a)/REAL(n);
  for(int i=0;i<=n;i++) x[i]=REAL(a)+i*h,fx[i]=f(x[i]);
  POLYNOM<REAL> res=lagreange_interpolation(n,x,fx);

  delete []x;delete []fx;
  return res.pstr();
}


}

#pragma package(smart_init)
