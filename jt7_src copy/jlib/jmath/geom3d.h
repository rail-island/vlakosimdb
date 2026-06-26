#ifndef geom3dH
#define geom3dH

#include "analgeom.h"

namespace jlib {class XMLItem;}

namespace jmath {
//template<class COLOR,class HI> struct ZBMP;

const int intkoef_MOV=10;
const double intkoef_DIV=1<<intkoef_MOV;

struct intkoef {
  int value;
  intkoef(double v) {value=v*intkoef_DIV;}
  intkoef(int k) {value=k<<intkoef_MOV;}
  intkoef(int k,int) {value=k;}
  intkoef() {value=intkoef_DIV;}
  operator double () const {return value/intkoef_DIV;}
};
inline int operator * (int a,intkoef k) {return (a*k.value)>>intkoef_MOV;}
inline intkoef operator * (intkoef a,intkoef b) {return intkoef((a.value*b.value)>>intkoef_MOV,0);}

template<class REAL> struct xyzrec {
  REAL x,y,z;
  xyzrec(REAL ax,REAL ay,REAL az) {x=ax;y=ay;z=az;};
  template<class REAL2> xyzrec(const agxyrec<REAL2> &XY,REAL Z) {x=XY.x;y=XY.y;z=Z;}
  agxyrec<REAL> xy() const {return agxyrec<REAL>(x,y);}
  xyzrec() {x=y=z=0;}
  void domultiply(REAL kx,REAL ky,REAL kz) {x*=kx;y*=ky;z*=kz;}
  void save(jlib::XMLItem &xml);
  void load(jlib::XMLItem &xml);
  REAL &coord(int i) {if (i) {if (i-1) return z;return y;}return x;}
  //vrati i-tou souradnici,i=0..2
};

template<class REAL> struct vector3d:xyzrec<REAL> {
  vector3d(REAL x,REAL y,REAL z):xyzrec<REAL>(x,y,z) {}
  vector3d(const xyzrec<REAL> &src):xyzrec<REAL>(src) {}
  REAL size() const {return sqrt(x*x+y*y+z*z);}
  vector3d<REAL> size1() const {REAL s=size();return vector3d<REAL>(x/s,y/s,z/s);}
  vector3d<REAL> operator *(REAL k) const {return vector3d<REAL>(x*k,y*k,z*k);}
  vector3d<REAL> operator /(REAL k) const {return vector3d<REAL>(x/k,y/k,z/k);}
  vector3d() {}
};
typedef vector3d<float> fvector3d;

template<class REAL> struct point3d:xyzrec<REAL> {
  point3d(REAL x,REAL y,REAL z):xyzrec<REAL>(x,y,z) {}
  point3d(const xyzrec<REAL> &src):xyzrec<REAL>(src) {}
  point3d() {}
  template<class REAL2> point3d(const agxyrec<REAL2> &XY,REAL Z):xyzrec<REAL>(XY,Z) {}
  point3d<REAL> translate(REAL dx,REAL dy,REAL dz) const {return point3d<REAL>(x+dx,y+dy,z+dz);}
};
typedef point3d<float> fpoint3d;

template<class REAL> struct matrix3d;
template<class REAL> struct base3d {
  vector3d<REAL> v1,v2,v3;
  base3d(const vector3d<REAL> &w1,const vector3d<REAL> &w2,const vector3d<REAL> &w3):v1(w1),v2(w2),v3(w3) {}
  void transform(const matrix3d<REAL> &m);
};
template<class REAL> inline base3d<REAL> normalbase() {return base3d<REAL>(vector3d<REAL>(1,0,0),vector3d<REAL>(0,1,0),vector3d<REAL>(0,0,1));}

template<class REAL> struct matrix3d {
  REAL a11,a12,a13,a21,a22,a23,a31,a32,a33;
  void set(REAL num) {a11=a12=a13=a21=a22=a23=a31=a32=a33=num;}
  matrix3d(REAL init=1) {set(0);a11=a22=a33=init;}
  matrix3d(const agmatrix<REAL> &src) {set(0);a11=src.a11;a12=src.a12;a21=src.a21;a22=src.a22;a33=1;}
  agmatrix<REAL> to2d() {return agmatrix<REAL>(a11,a12,a21,a22);}
  void dorotatez(double angle) {
    REAL c=cos(angle),s=sin(angle);
    a11=c;a12=-s;a13=0;
    a21=s;a22=c;a23=0;
    a31=0;a32=0;a33=1;
  }
  void dorotatey(double angle) {
    REAL c=cos(angle),s=sin(angle);
    a11=c;a12=0;a13=s;
    a21=0;a22=1;a23=0;
    a31=-s;a32=0;a33=c;
  }
  void dorotatex(double angle) {
    REAL c=cos(angle),s=sin(angle);
    a11=1;a12=0;a13=0;
    a21=0;a22=c;a23=-s;
    a31=0;a32=s;a33=c;
  }
  void dorotate(fvector3d v,float angle);
/*  void doreflection(REAL rx,REAL ry,REAL rz) {
    a11=rx;a12=0;a13=0;
    a21=0;a22=ry;s23=0;
    a31=0;a32=0;a33=rz;
  }*/
  void tobase(const vector3d<REAL> &v1,const vector3d<REAL> &v2,const vector3d<REAL> &v3) {
    tobase(normalbase<REAL>(),base3d<REAL>(v1,v2,v3));
  }
  //matice bude matici prechodu od kanonicke baze k bazi (v1,v2,v3)
  void tobase(const base3d<REAL> b1,const base3d<REAL> b2);
  //matice bude matici prechodu od b1 k b2
  REAL determinant() {return a33*a11*a22-a33*a12*a21-a13*a31*a22-a23*a11*a32+a23*a12*a31+a13*a21*a32;}
  void load(jlib::XMLItem &xml);
  void save(jlib::XMLItem &xml);
};
typedef matrix3d<float> fmatrix3d;

template<class REAL> struct transform3d {
  matrix3d<REAL> m;
  vector3d<REAL> b;
  transform3d(const matrix3d<REAL> &m,const vector3d<REAL> &b) {this->m=m;this->b=b;}
  transform3d(const matrix3d<REAL> &m) {this->m=m;this->b=vector3d(0,0,0);}
  transform3d() {}
  void save(jlib::XMLItem &xml);
  void load(jlib::XMLItem &xml) {m.load(xml);b.load(xml);}
//  point3d<REAL> apply(const point3d<REAL> &pt);
};
template<class REAL> inline point3d<REAL> operator *(const matrix3d<REAL> &a,point3d<REAL> v) {
  return point3d<REAL>(
    a.a11*v.x+a.a12*v.y+a.a13*v.z,
    a.a21*v.x+a.a22*v.y+a.a23*v.z,
    a.a31*v.x+a.a32*v.y+a.a33*v.z
  );
}
template<class REAL> inline vector3d<REAL> operator *(const matrix3d<REAL> &a,vector3d<REAL> v) {
  return vector3d<REAL>(
    a.a11*v.x+a.a12*v.y+a.a13*v.z,
    a.a21*v.x+a.a22*v.y+a.a23*v.z,
    a.a31*v.x+a.a32*v.y+a.a33*v.z
  );
}

template<class REAL> inline matrix3d<REAL> operator *(const matrix3d<REAL> &a,const matrix3d<REAL> &b) {
  matrix3d<REAL> res;

  res.a11=a.a11*b.a11+a.a12*b.a21+a.a13*b.a31;
  res.a12=a.a11*b.a12+a.a12*b.a22+a.a13*b.a32;
  res.a13=a.a11*b.a13+a.a12*b.a23+a.a13*b.a33;

  res.a21=a.a21*b.a11+a.a22*b.a21+a.a23*b.a31;
  res.a22=a.a21*b.a12+a.a22*b.a22+a.a23*b.a32;
  res.a23=a.a21*b.a13+a.a22*b.a23+a.a23*b.a33;

  res.a31=a.a31*b.a11+a.a32*b.a21+a.a33*b.a31;
  res.a32=a.a31*b.a12+a.a32*b.a22+a.a33*b.a32;
  res.a33=a.a31*b.a13+a.a32*b.a23+a.a33*b.a33;

  return res;
}

template<class REAL> inline vector3d<REAL> operator %(const vector3d<REAL> &a,const vector3d<REAL> &b) {//vektorovy soucin
  return vector3d<REAL>(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);
}
template<class REAL> inline REAL operator *(const vector3d<REAL> &a,const vector3d<REAL> &b) {//skalarni soucin
  return a.x*b.x+a.y*b.y+a.z*b.z;
}
template<class REAL> inline REAL operator /(const vector3d<REAL> &a,const vector3d<REAL> &b)//orientovany kosinus udchylky
{ return (a*b)/(a.size()*b.size());}

template<class REAL> inline vector3d<REAL> operator -(const vector3d<REAL> &v) {return vector3d<REAL>(-v.x,-v.y,-v.z);}
template<class REAL> inline vector3d<REAL> operator +(const vector3d<REAL> &a,const vector3d<REAL> &b) {return vector3d<REAL>(a.x+b.x,a.y+b.y,a.z+b.z);}
template<class REAL> inline xyzrec<REAL> operator +(const xyzrec<REAL> &a,const xyzrec<REAL> &b) {return xyzrec<REAL>(a.x+b.x,a.y+b.y,a.z+b.z);}
template<class REAL> inline vector3d<REAL> operator -(const vector3d<REAL> &a,const vector3d<REAL> &b) {return vector3d<REAL>(a.x-b.x,a.y-b.y,a.z-b.z);}
template<class REAL> inline vector3d<REAL> operator -(const point3d<REAL> &a,const point3d<REAL> &b) {return vector3d<REAL>(a.x-b.x,a.y-b.y,a.z-b.z);}
template<class REAL> inline vector3d<REAL> operator *(const vector3d<REAL> &a,REAL k) {return vector3d<REAL>(k*a.x,k*a.y,k*a.z);}
template<class REAL> inline point3d<REAL> operator +(const point3d<REAL> &a,const vector3d<REAL> &b) {return point3d<REAL>(a.x+b.x,a.y+b.y,a.z+b.z);}
template<class REAL> inline point3d<REAL> operator -(const point3d<REAL> &a,const vector3d<REAL> &b) {return point3d<REAL>(a.x-b.x,a.y-b.y,a.z-b.z);}

template<class REAL> inline vector3d<REAL> middle(const point3d<REAL> &a,const point3d<REAL> &b) {
  return point3d<REAL>((a.x+b.x)/2,(a.y+b.y)/2,(a.z+b.z)/2);
}
template<class REAL> inline point3d<REAL> operator *(const transform3d<REAL> &t,const point3d<REAL> &b) {
  return t.m*b+t.b;
}
template<class REAL> inline transform3d<REAL> operator *(const transform3d<REAL> &a,const transform3d<REAL> &b) {
  return transform3d<REAL>(a.m*b.m,a.m*b.b+a.b);
}


template<class REAL> inline void base3d<REAL>::transform(const matrix3d<REAL> &m) {
  v1=m*v1;v2=m*v2;v3=m*v3;
}

template<class REAL> struct plane3d {//rovina
  point3d<REAL> a;
  vector3d<REAL> t;
};
template<class REAL> struct implicitplane3d {//rovina zadana obecnou rovnici
  REAL a,b,c,d;
};

template<class HI> struct XYHREC {
  int x,y;
  HI z;
  XYHREC(XYREC apt,HI ahi) {x=apt.x;y=apt.y;z=ahi;}
  XYHREC(int ax,int ay,HI ahi) {x=ax;y=ay;z=ahi;}
  template<class REAL> XYHREC(const xyzrec<REAL> &xyz) {x=xyz.x;y=xyz.y;z=xyz.z;}
  template<class REAL> xyzrec<REAL> xyz() {return xyzrec<REAL>z,y,z);}
  XYHREC() {x=0;y=0;z=0;}
};
template<class HI> XYHREC<HI> operator +(const XYHREC<HI> &a,const XYHREC<HI> &b) {
  return XYHREC<HI>(a.x+b.x,a.y+b.y,a.z+b.z);
}
template<class HI> XYHREC<HI> operator /(const XYHREC<HI> &a,float k) {
  return XYHREC<HI>(a.x/k,a.y/k,a.z/k);
}

template<class HI> struct Projector {
  float ky,kz;
  Projector(double seeangle) {ky=sin(seeangle);kz=cos(seeangle);}
  Projector(float aky,float akz) {ky=aky;kz=akz;}
  template<class REAL> XYHREC<HI> operator () (const point3d<REAL> &pt) const {
    return XYHREC<HI>(pt.x,pt.y*ky-kz*pt.z,pt.z);
  }
};

/*template<class REAL,class agnum> inline agxyrec<agnum> operator *(const agmatrix<agnum> &a,const agxyrec<agnum> &v) {
  return agxyrec<agnum>(a.a11*v.x+a.a12*v.y,a.a21*v.x+a.a22*v.y);
}

template<class REAL,class agnum> inline agtransform<agnum> operator *(const agtransform<agnum> &a,const xyzrev<REAL> &v) {
  return agtransform<agnum>(a.a*b.a,a.a*b.d+a.d);
}*/
struct REMAP {
  int map[3];
  REMAP(int x,int y,int z) {map[0]=x;map[1]=y;map[2]=z;}
  int operator [](int index) const {return map[index];}
  REMAP inv() {
    REMAP res;
    res.map[map[0]]=0;
    res.map[map[1]]=1;
    res.map[map[2]]=2;
    return res;
  }
private:
  REMAP() {}
};

template<class REAL> struct complane {
  vector3d<REAL> n;
  REAL d;
  complane() {d=0;}
  complane(const point3d<REAL> &a,const point3d<REAL> &b,const point3d<REAL> &c) {
    vector3d<REAL> s1=b-a,s2=b-c;
    n=(s1%s2).size1();
    d=-n.x*a.x-n.y*a.y-n.z*a.z;
  }
  REAL operator () (const point3d<REAL> &pt) {return n.x*pt.x+n.y*pt.y+n.z*pt.z+d;}
  void want(const point3d<REAL> &pt) {if (operator()(pt)<0) reverse();}
  void reverse() {n=-n;d=-d;}
};
typedef complane<float> fcomplane;

typedef void (*VoxelFunction)(float &x,float &y,float &z,void *data);

template<class T,class S> void updatemmxy(const T &pt,S &minx,S &miny,S &maxx,S &maxy) {
  if (pt.x<minx) minx=pt.x;
  if (pt.y<miny) miny=pt.y;
  if (pt.x>maxx) maxx=pt.x;
  if (pt.y>maxy) maxy=pt.y;
}

template<class REAL> inline REAL distance3d(const xyzrec<REAL> &a,const xyzrec<REAL> &b) {
  REAL dx=a.x-b.x,dy=a.y-b.y,dz=a.z-b.z;
  return sqrt(dx*dx+dy*dy+dz*dz);
}

}

#endif
