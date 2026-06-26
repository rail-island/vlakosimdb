#ifndef splinesH
#define splinesH

#include "polynoms.h"

namespace jmath {

template<class REAL> void natural_spline(int n,REAL x[],REAL fx[],TObjVec<POLYNOM<REAL> > &res);

template<class REAL> string maple_natural_spline(int n,int a,int b,REAL (*f)(REAL));

}
#endif
