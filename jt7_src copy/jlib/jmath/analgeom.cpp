#include <jlib.h>
#pragma hdrstop

#include "analgeom.h"

namespace jmath {
using namespace jlib;
const double epsilon=0.001;


template<class agnum> double agline<agnum>::pdist(const agpoint<agnum> &pt) const {
  double a,b,c;
  makeimplicitfunc(a,b,c);
  return fabs(a*pt.x+b*pt.y+c)/sqrt(a*a+b*b);
}

template<class agnum> void agline<agnum>::makeimplicitfunc(double &a,double &b,double &c) const {
  a=d.y;
  b=-d.x;
  c=this->a.y*d.x-this->a.x*d.y;
}

template<class agnum> double agline<agnum>::pdistfromline(const agpoint<agnum> &pt) const {
  if (fsize()<epsilon) return (a-pt).fsize();
  double d=fsize(),d1=getvert().pdist(pt),d2=getvertb().pdist(pt);
  if (d1<d2) {//pt je blize bodu a nez b
    if (d2>d) return agvector<agnum>(pt,a).fsize();
  } else {
    if (d1>d) return agvector<agnum>(pt,getb()).fsize();
  }
  return pdist(pt);
}

template<class agnum> bool agline<agnum>::compoint(const agline<agnum> &l,agpoint<agnum> &pt,agnum *arg) const {
  double jmenovatel=l.d.x*d.y-l.d.y*d.x;
  if (fabs(jmenovatel)<epsilon) return false;
  double s=(a.x*d.y-a.y*d.x+l.a.y*d.x-l.a.x*d.y)/jmenovatel;
  if (arg) *arg=s;
  pt.x=l.a.x+l.d.x*s;
  pt.y=l.a.y+l.d.y*s;
  return true;
}


}
#pragma package(smart_init)

