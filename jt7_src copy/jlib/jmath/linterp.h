#ifndef linterpH
#define linterpH

#include "mathcom.h"
#include "polynoms.h"

namespace jmath {

template<class REAL> POLYNOM<REAL> lagreange_interpolation(int n,REAL x[],REAL fx[]);

template<class REAL> string maple_lagreange_interpolation(int n,int a,int b,REAL (*f)(REAL));

}

#endif
