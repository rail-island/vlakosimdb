//---------------------------------------------------------------------------

//0 je uzivana pro pruhlednou barvu

#ifndef bytebmpH
#define bytebmpH

#include <com/com0.h>
#include <math.h>
#include <jmath/geom3d.h>
#include <gr/comgr.h>
#include <gr/comcnv.h>
#include <jmath/analgeom.h>

namespace zbuf {
using namespace jmath;

inline uchar logictopal16(gr::LOGICCOLOR color) {
  if (color.l==0) return 16;
  if (color.l<16) return color.l;
  return 0;
}
//const char *colornames[]={"black",

template<class COLOR,class REAL> struct ZBMP;

template<class COLOR,class REAL> struct PIXEL {
  REAL x,y;
  COLOR color;
  void set(REAL ax,REAL ay,COLOR acolor) {x=ax;y=ay;color=acolor;}
};

//template<class COLOR,class REAL> class PIXELARRAY;

template<class COLOR> struct BYTEBMP {
  COLOR *buf;
  COLOR transparent;

  COLOR **scanline;
  bool reversey;
  int W,H;
  int alignedW;
  int size;//>=W*H(==W*H+diry po zoravnani)  
  BYTEBMP(int w,int h,COLOR transparentcolor,bool reversey=false,int alignelements=1);
  ~BYTEBMP();
  int repairblitsize(int src0,int dest0,int srcsize,int destsize,int size)
    {return max(0,min(size,min(destsize-dest0,srcsize-src0)));}
  void clear() {clear(transparent);}
  void clear(COLOR color)
    {if (buf) fillarray(buf,size,color);}
  void putpixel(int x,int y,COLOR color) {if (x>=0 and x<W and y>=0 and y<H) scanline[y][x]=color;}
  COLOR getpixel(int x,int y) {if (x>=0 and x<W and y>=0 and y<H) return scanline[y][x];return 0;}

  void fillrect(int x1,int y1,int x2,int y2,COLOR color);
  void rectangle(int x1,int y1,int x2,int y2,COLOR color);
  void line(int x1,int y1,int x2,int y2,COLOR color);
  void line(XYREC a,XYREC b,COLOR color) {line(a.x,a.y,b.x,b.y,color);}
  void fline(double x1,double y1,double x2,double y2,COLOR color);
  void fline(XYREC a,XYREC b,COLOR color) {fline(a.x,b.y,b.x,b.y,color);}
  void circle(int mx,int my,int radius,COLOR color,double start=0,double stop=2*M_PI);
  //nasledujici fce jsou "odflaknuty", proto pracuji pomalu
  void fcircle(double mx,double my,double radius,COLOR color,double start=0,double stop=2*M_PI) {
    fellipse(mx,my,radius,radius,color,start,stop);
  }
  void fellipse(double mx,double my,double radiusx,double radiusy,COLOR color,double start=0,double stop=2*M_PI);
  void fillellipse(int x1,int y1,int x2,int y2,COLOR color);
//  void ellipse(int x1,int y1,int x2,int y2,COLOR color);
  void drawpolygon(const XYREC *pts,int count,COLOR color,bool onlypolyline=false);
  void fillpolygon(const XYREC *pts,int count,COLOR color);
//  template<class REAL> friend class PIXELARRAY<COLOR,REAL>;
  void transp();//transponuje bitmapu(neni-li ctvercova,prohodi W a H)
  void reflectx();//zrcadleni podel osy x(kolem y)
  void reflecty();//zrcadleni podel osy y(kolem x)
  void blit(BYTEBMP &src,int dx,int dy);
  void assign(BYTEBMP<COLOR> &srv);
  void copyrect(const BYTEBMP<COLOR> &src,int destx,int desty,int srcx,int srcy,int w,int h);
  void transparentcopyrect(const BYTEBMP<COLOR> &src,int destx,int desty,int srcx,int srcy,int w,int h);
  template<class INDEXCOLOR> void transparentcopyrectpalette(const BYTEBMP<INDEXCOLOR> &src,int destx,int desty,int srcx,int srcy,int w,int h,COLOR colors[]);
protected:
  //tyto dve fce pomocne,volany z line
  void line_de_x(int x1,int y1,int x2,int y2,COLOR color);//x1<x2
  void line_de_y(int x1,int y1,int x2,int y2,COLOR color);//y1<y2
};

template<class COLOR> struct BMPCANVAS:gr::CANVAS {
  typedef COLOR (*Flogictonative)(gr::LOGICCOLOR logic);

  BYTEBMP<COLOR> *bmp;
  BMPCANVAS(BYTEBMP<COLOR> *abmp,Flogictonative alogictofg,Flogictonative alogictobg) {
    bmp=abmp;logictofg=alogictofg;logictobg=alogictobg;
  }
  mutable COLOR fgcolor,bgcolor;
  Flogictonative logictofg,logictobg;

  void _line(int x1,int y1,int x2,int y2) const {bmp->line(x1,y1,x2,y2,fgcolor);}
  void _rectangle(int x1,int y1,int x2,int y2) const {bmp->fillrect(x1,y1,x2,y2,bgcolor);bmp->rectangle(x1,y1,x2,y2,fgcolor);}
  void _fillrectangle(int x1,int y1,int x2,int y2) const {bmp->fillrect(x1,y1,x2,y2,bgcolor);}
  void _drawrectangle(int x1,int y1,int x2,int y2) const {bmp->rectangle(x1,y1,x2,y2,fgcolor);}
  void _ellipse(int x1,int y1,int x2,int y2) const {
    bmp->fillellipse(x1,y1,x2,y2,bgcolor);
    bmp->fellipse((x1+x2)/2.0,(y1+y2)/2.0,fabs(x2-x1)/2.0,fabs(y2-y1)/2.0,fgcolor);
  }
  void _drawellipse(int x1,int y1,int x2,int y2) const {
    bmp->fellipse((x1+x2)/2.0,(y1+y2)/2.0,fabs(x2-x1)/2.0,fabs(y2-y1)/2.0,fgcolor);
  }
  void _fillellipse(int x1,int y1,int x2,int y2) const {
    bmp->fillellipse(x1,y1,x2,y2,bgcolor);
  }
  void _setfgcolor(gr::LOGICCOLOR color) const {fgcolor=logictofg(color);}
  void _setbgcolor(gr::LOGICCOLOR color) const {bgcolor=logictobg(color);}
  void _polyline(const XYREC *points,int pointcount) const {bmp->drawpolygon(points,pointcount,fgcolor,true);}
  void _polygon(const XYREC *points,int pointcount) const {
    bmp->fillpolygon(points,pointcount,bgcolor);
    bmp->drawpolygon(points,pointcount,fgcolor);
  }
  void _drawpolygon(const XYREC *points,int pointcount) const {bmp->drawpolygon(points,pointcount,fgcolor);}
  void _fillpolygon(const XYREC *points,int pointcount) const {bmp->fillpolygon(points,pointcount,bgcolor);}
};

/*
template<class COLOR,class REAL> class PIXELARRAY {
  typedef PIXEL<COLOR,REAL> pixel_t;
public:
  pixel_t *pixels;
  int count;
  PIXELARRAY(BYTEBMP<COLOR> *bmp,int x0=0,int y0=0);
  //vytvori seznam tech pixelu,kde barva!=0,
  //x0,y0 jsou souradnice v bmp,ktere budou v PIXELARRAY v pocatku
  PIXELARRAY(BYTEBMP<COLOR> *bmp,int x0,int y0,float kx,float ky);
  //nejdrive se vynasobi kx,ky a pak odecte x0,y0
  PIXELARRAY(pixel_t *data,int acount);
  template<class HI> void blittozbmp(ZBMP<COLOR,HI> *zbmp,int x0,int y0,HI z);
  template<class HI> void blittozbmp(ZBMP<COLOR,HI> *zbmp,int x0,int y0,HI z0,float kzx,float kzy,const agmatrix<float> &mat,const Projector<HI> &prj);
//  template<class HI> void blittozbmp(ZBMP<COLOR,HI> *zbmp,int x0,int y0,int ky,HI z);
  PIXELARRAY<COLOR,REAL> *clone() {return new PIXELARRAY<COLOR,REAL>(pixels,count);}
  template<class REAL2> void transform(const agtransform<REAL2> &t) {
    for(int i=0;i<count;i++) {
      gr::agpoint<REAL2> pt(pixels[i].x,pixels[i].y);
      pt=t*pt;
      pixels[i].x=pt.x;
      pixels[i].y=pt.y;
    }
  }
};
*/

}
#endif
