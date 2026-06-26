#include <jlib.h>
#pragma hdrstop

#include "jmath.h"
#include "matrices.cpp"
#include "polynoms.cpp"
#include "vectors.cpp"

namespace jmath {

#define real(CLASSNAME) \
template struct CLASSNAME<double>;\
template struct CLASSNAME<long double>;\
template struct CLASSNAME<float>;\
template struct CLASSNAME<FRACTION>;
real(MATRIX);
real(VECTOR);
real(POLYNOM);
#undef real


}

#pragma package(smart_init)
