#ifndef int3dH
#define int3dH

namespace zbuf {

typedef int intkoef;//realne cislo nasobene 1024(<<10), odrezana desetinna cast nasobku
//(tedy priblizna presnost na tri desetinna mista)
typedef int intkoef2;//realne cislo <<20

struct int3dpoint//struktura pro bod vyjadreny celymi cisly
{
  int x,y,z;
  int3dpoint(int ax,int ay,int az) {x=ax;y=ay;z=az;};
};

struct int3dvector {//struktura pro vektor vyjadreny trunc(realnymi cisly*1024)
  intkoef x,y,z;
  int3dvector(intkoef ax,intkoef ay,intkoef az) {x=ax;y=ay;z=az;};
  float size() {return sqrt(x*x/1024.0+y*y/1024.0+z*z/1024.0);}
};

struct int2dpoint {
  int x,y;
  int2dpoint(int ax,int ay) {x=ax;y=ay;}
};

template<class REAL> struct vector3d {//struktura pro vektor
  REAL x,y,z;
  vector3d(REAL ax,REAL ay,REAL az) {x=ax;y=ay;z=az;};
  REAL size() {return sqrt(x*x+y*y+z*z);}
};

struct intmat3d {
  intkoef a11,a12,a13,a21,a22,a23,a31,a32,a33;
  void set(int num) {a11=num;a12=num;a13=num;a21=num;a22=num;a23=num;a31=num;a32=num;a33=num;}
  intmat3d() {set(0);}
};

inline int3dpoint operator *(intmat3d &a,int3dpoint v) {
  return int3dpoint(
    (a.a11*v.x+a.a12*v.y+a.a13*v.z)>>10,
    (a.a21*v.x+a.a22*v.y+a.a23*v.z)>>10,
    (a.a31*v.x+a.a32*v.y+a.a33*v.z)>>10
  );
}

intmat3d operator *(intmat3d a,intmat3d b);


//int3dvector
inline int3dvector operator %(int3dvector a,int3dvector b) {//vektorovy soucin
  return int3dvector(
    (a.y*b.z-a.z*b.y)>>10,
    (a.z*b.x-a.x*b.z)>>10,
    (a.x*b.y-a.y*b.x)>>10
  );
}
inline intkoef2 i2scalarproduct(int3dvector a,int3dvector b)//skalarni soucin<<20
{ return a.x*b.x+a.y*b.y+a.z*b.z;}
inline float operator *(int3dvector a,int3dvector b)//skalarni soucin
{ return (a.x*b.x+a.y*b.y+a.z*b.z)/1024.0/1024.0;}
inline float operator /(int3dvector a,int3dvector b)//orientovany kosinus udchylky
{ return (a*b)/(a.size()*b.size());}
inline int3dvector makeintvector(int x,int y,int z) {return int3dvector(x<<10,y<<10,z<<10);}


//--------------------------------------
//real3dvector
template<class REAL> inline REAL operator *(vector3d<REAL> a,vector3d<REAL> b)
{ return a.x*b.x+a.y*b.y+a.z*b.z;}//skalarni soucin
template<class REAL> inline REAL operator /(vector3d<REAL> a,vector3d<REAL> b)
{ return (a*b)/(a.size()*b.size());}//orientovany kosinus udchylky
template<class REAL> inline vector3d<REAL> operator %(vector3d<REAL> a,vector3d<REAL> b) {//vektorovy soucin
  return vector3d<REAL>(
    a.y*b.z-a.z*b.y,
    a.z*b.x-a.x*b.z,
    a.x*b.y-a.y*b.x
  );
}

//spolecne operatory
template<class VEC> inline VEC operator -(VEC v) {return VEC(-v.x,-v.y,-v.z);}
template<class VEC> inline VEC operator +(VEC a,VEC b) {return VEC(a.x+b.x,a.y+b.y,a.z+b.z);}
template<class VEC> inline VEC operator -(VEC a,VEC b) {return VEC(a.x-b.x,a.y-b.y,a.z-b.z);}

}

#endif
