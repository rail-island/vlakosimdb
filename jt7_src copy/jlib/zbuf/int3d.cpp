#include <jlib.h>
#pragma hdrstop

#include "int3d.h"

//---------------------------------------------------------------------------

intmat3d operator *(intmat3d a,intmat3d b) {
  intmat3d res;

  res.a11=(a.a11*b.a11+a.a12*b.a21+a.a13*b.a31)>>10;
  res.a12=(a.a11*b.a12+a.a12*b.a22+a.a13*b.a32)>>10;
  res.a13=(a.a11*b.a13+a.a12*b.a23+a.a13*b.a33)>>10;

  res.a21=(a.a21*b.a11+a.a22*b.a21+a.a23*b.a31)>>10;
  res.a22=(a.a21*b.a12+a.a22*b.a22+a.a23*b.a32)>>10;
  res.a23=(a.a21*b.a13+a.a22*b.a23+a.a23*b.a33)>>10;

  res.a31=(a.a31*b.a11+a.a32*b.a21+a.a33*b.a31)>>10;
  res.a32=(a.a31*b.a12+a.a32*b.a22+a.a33*b.a32)>>10;
  res.a33=(a.a31*b.a13+a.a32*b.a23+a.a33*b.a33)>>10;

  return res;
}


#pragma package(smart_init)
