//---------------------------------------------------------------------------

#ifndef zbmpH
#define zbmpH

#include <com/com0.h>
#include <jmath/geom3d.h>

//0 je uzivana pro pruhlednou barvu

namespace zbuf {
using namespace jlib;
using namespace jmath;

inline int repairblitsize(int src0,int dest0,int srcsize,int destsize,int size) {
  return max(0,min(size,min(destsize-dest0,srcsize-src0)));
}

template<class COLOR> struct TEXTURECOLORER {
  COLOR *color,*buf;
  operator COLOR () {
    if (*color) return *color++;
    color=buf;
    return *color;
  }
  TEXTURECOLORER(COLOR *abuf) {buf=abuf;color=buf;}
};

template<class COLOR,class HI> struct ZBMP {
  static const int MAX=0xFF;
  int W,H;
  COLOR transparent;
  HI minimalhi;
  COLOR *buf;
  HI *zbuf;
  COLOR **scanline;
  HI **zscanline;
  int *lineofs;
  int size;
  int alignedW;
  bool reversey;

  ZBMP(int w,int h,COLOR transparentcolor,HI minimalhi,bool reversey,int alignelements);
  ~ZBMP();
  void clear(COLOR color,HI height)
    {if (W>0 and H>0) {fillarray(buf,W*H,color);fillarray(zbuf,W*H,height);}}
  void clear() {clear(transparent,minimalhi);}
//-----------------------------------------
//funkce neprepocitavajici souradnice
  void putpixel(int x,int y,HI z,COLOR color) {
    if (x<0 or x>=W or y<0 or y>=H) return;
    int ofs=lineofs[y]+x;
    if (z>zbuf[ofs]) buf[ofs]=color,zbuf[ofs]=z;
  }
  void rectangle(int x1,int y1,int x2,int y2,HI z,COLOR color);
  void fillrect(int x1,int y1,int x2,int y2,HI z,COLOR color);
  void clearrect(int x1,int y1,int x2,int y2,COLOR color);
  void blit(ZBMP &src,int dx,int dy,HI dz);
  void line(int x1,int y1,HI z1,int x2,int y2,HI z2,COLOR color);
  void line(const XYHREC<HI> &a,const XYHREC<HI> &b,COLOR color) {line(a.x,a.y,a.z,b.x,b.y,b.z,color);}
  void minmaxcoords(XYREC &min,XYREC &max);
//  void xline(int y,int x1,int x2,int z1,int z2,COLOR color);//vodorovna cara,x1<x2
//  void yline(int x,int y1,int y2,int z1,int z2,COLOR color);//svisla cara,y1<y2
//  void fillrect(int x1,int y1,int x2,int y2,int z,COLOR color);
//  void line(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color);
  void fellipse(double mx,double my,double radiusx,double radiusy,HI z,COLOR color,double start=0,double stop=2*M_PI);
  void drawellipse(int x1,int y1,int x2,int y2,HI z,COLOR color) {fellipse((x1+x2)/2.0,(y1+y2)/2.0,abs(x1-x2)/2.0,abs(y1-y2)/2.0,z,color);}
  void fillellipse(int x1,int y1,int x2,int y2,HI z,COLOR color);
protected:
  //tyto dve fce pomocne,volany z line
//  void line_de_x(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color);//x1<x2
//  void line_de_y(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color);//y1<y2
public:
  void assign(const ZBMP<COLOR,HI> &src);
  void copyrect(const ZBMP<COLOR,HI> &src,int destx,int desty,int srcx,int srcy,int w,int h);//preste kopiruje, nekouka na hlobky
  void reflectx();
  void reflecty();
  void move(int dx,int dy);
  template<class COLORER> void xlefttriangle(int x1,int x2,int y1,int y2,int y3,HI z1,HI z2,HI z3,COLORER brush);
//  predpoklad:y1<=y2 and x1<=x2
  /*Vystvetleni:
     A:y1,z1


     z=imz      C:y3,z3

     B:y2,z2
     x1         x2

     C muze byt i nad urovni A,i pod urovni B
  */
  template<class COLORER> void xrighttriangle(int x1,int x2,int y1,int y2,int y3,HI z1,HI z2,HI z3,COLORER brush);
//  void xrighttriangle(int x1,int x2,int y1,int y2,int y3,HI z1,HI z2,HI z3,COLOR color);
  template<class COLORER> void triangle(XYHREC<HI> a,XYHREC<HI> b,XYHREC<HI> c,COLORER brush);

//  void xlefttriangle(int x1,int x2,int y1,int y2,int y3,int z1,int z2,int z3,COLOR color);
  //predpoklad:y1<=y2 and x1<=x2
  /*Vystvetleni:
     A:y1,z1


                C:y3,z3

     B:y2,z2
     x1         x2

     C muze byt i nad urovni A,i pod urovni B
  */
//  void xrighttriangle(int x1,int x2,int y1,int y2,int y3,int z1,int z2,int z3,COLOR color);
  //analogicky,ale usecka AB je napravo od C,cislovani stejne(napr:C:y3,z3),x1<=x2,y1<=y2
//  void polygon(int pts[],int pointscount,int dzx,int dzy,point3d<int> fix,COLOR color);
  //nakresli KONVEXNI polygon,dzx je sklon polygonu ve smeru osy x * 1024,
  //dzy ve smeru u,fix je bud,ktery lezi v polygonu
  //pts je pole int:x,y(x1,y1,x2,y2,x3,y3,...,xn,yn],n==pointscount
};


}
#endif
