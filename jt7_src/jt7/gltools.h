#ifndef gltoolsH
#define gltoolsH

#include <jmath/geom3d.h>

namespace jtyc {

using namespace jmath;

///zkoseni
void glMapCrook(bool vertical,float dy);
void glMultiply(fmatrix3d &m);
void glMultiplySwapYZ(fmatrix3d &m);

}

#endif
