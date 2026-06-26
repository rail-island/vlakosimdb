#include <jlib.h>
#pragma hdrstop

#include "gltools.h"
#include <opengl.h>

namespace jtyc {

void glMapCrook(bool vertical,float dy) {
  if (vertical) {
    float matrix[16]={
      1,0,0,0,
      0,1,0,0,
      0,dy,1,0,
      0,0,0,1
    };
    glMultMatrixf(matrix);
  } else {
    float matrix[16]={
      1,dy,0,0,
      0,1,0,0,
      0,0,1,0,
      0,0,0,1
    };
    glMultMatrixf(matrix);
  }
}

void glMultiply(fmatrix3d &m) {
  float a[16]={
    m.a11,m.a21,m.a31,0,
    m.a12,m.a22,m.a32,0,
    m.a13,m.a23,m.a33,0,
    0    ,0    ,0    ,1
  };
  glMultMatrixf(a);
}

void glMultiplySwapYZ(fmatrix3d &m) {
  float a[16]={
    m.a11,m.a31,m.a21,0,
    m.a13,m.a33,m.a23,0,
    m.a12,m.a32,m.a22,0,
    0    ,0    ,0    ,1
  };
  glMultMatrixf(a);
}



}


