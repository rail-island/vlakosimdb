#include <jlib.h>
#pragma hdrstop

#include "geom3d.h"
#include <ext/xmldoc.h>

namespace jmath {

template<class REAL> void xyzrec<REAL>::save(XMLItem &xml) {
  xml.name="xyz";
  xml.setvar("x",x);
  xml.setvar("y",y);
  xml.setvar("z",z);
}
template<class REAL> void xyzrec<REAL>::load(XMLItem &xml) {
  x=xml.fvars["x"];
  y=xml.fvars["y"];
  z=xml.fvars["z"];
}
template<class REAL> void transform3d<REAL>::save(jlib::XMLItem &xml) {
  m.save(xml);b.save(xml);
  xml.name="transform3d";
}



template<class REAL> void matrix3d<REAL>::load(XMLItem &xml) {
  a11=xml.fvars["a11"];
  a12=xml.fvars["a12"];
  a13=xml.fvars["a13"];
  a21=xml.fvars["a21"];
  a22=xml.fvars["a22"];
  a23=xml.fvars["a23"];
  a31=xml.fvars["a31"];
  a32=xml.fvars["a32"];
  a33=xml.fvars["a33"];
  xml.name="matrix3d";
}
template<class REAL> void matrix3d<REAL>::save(XMLItem &xml) {
  xml.vars.setvar("a11",a11);
  xml.vars.setvar("a12",a12);
  xml.vars.setvar("a13",a13);
  xml.vars.setvar("a21",a21);
  xml.vars.setvar("a22",a22);
  xml.vars.setvar("a23",a23);
  xml.vars.setvar("a31",a31);
  xml.vars.setvar("a32",a32);
  xml.vars.setvar("a33",a33);
}

template<class REAL> void matrix3d<REAL>::dorotate(fvector3d v,float angle) {
  v=v.size1();
  float c=cos(angle),s=sin(angle);

  float x=v.x,y=v.y,z=v.z;

  a11=x*x+c*(1-x*x)+0*s;
  a12=x*y+c*(0-x*y)-z*s;
  a13=x*z+c*(0-x*z)+y*s;

  a21=y*x+c*(0-y*x)+z*s;
  a22=y*y+c*(1-y*y)+0*s;
  a23=y*z+c*(0-y*z)-x*s;

  a31=z*x+c*(0-z*x)-y*s;
  a32=z*y+c*(0-z*y)+x*s;
  a33=z*z+c*(1-z*z)+0*s;
}

template<class REAL> static void solve_3d(const vector3d<REAL> &v1,const vector3d<REAL> &v2,const vector3d<REAL> &v3,const vector3d<REAL> &right,REAL &a,REAL &b,REAL &c,REAL deta) {
  REAL a11=v1.x,a21=v1.y,a31=v1.z,
       a12=v2.x,a22=v2.y,a32=v2.z,
       a13=v3.x,a23=v3.y,a33=v3.z;
  REAL b1=right.x,b2=right.y,b3=right.z;
  a=(-a12*b2*a33+a12*a23*b3-b1*a23*a32-a13*b3*a22+b1*a22*a33+a13*b2*a32)/deta;
  b=(a21*a13*b3-a21*b1*a33+b2*a11*a33-a13*b2*a31+b1*a23*a31-a23*a11*b3)/deta;
  c=-(-b3*a11*a22+b3*a12*a21+b1*a31*a22+b2*a11*a32-b2*a12*a31-b1*a21*a32)/deta;
  //spocitano knihovnou jmath(MATRIX::gem(),MATRIX::solve()) pro typ mstring
  //upraveno programem Maple
  //vysledek je totozny s pouzitim Cramerova pravidla
}

template<class REAL> void matrix3d<REAL>::tobase(const base3d<REAL> b1,const base3d<REAL> b2) {
  REAL a11=b1.v1.x,a21=b1.v1.y,a31=b1.v1.z,
       a12=b1.v2.x,a22=b1.v2.y,a32=b1.v2.z,
       a13=b1.v3.x,a23=b1.v3.y,a33=b1.v3.z;
  REAL deta=a33*a11*a22-a33*a12*a21-a13*a31*a22-a23*a11*a32+a23*a12*a31+a13*a21*a32;
  solve_3d(b1.v1,b1.v2,b1.v3,b2.v1,this->a11,this->a21,this->a31,deta);
  solve_3d(b1.v1,b1.v2,b1.v3,b2.v2,this->a12,this->a22,this->a32,deta);
  solve_3d(b1.v1,b1.v2,b1.v3,b2.v3,this->a13,this->a23,this->a33,deta);
}


}

#pragma package(smart_init)
