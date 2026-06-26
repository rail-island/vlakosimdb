#include <jlib.h>
#pragma hdrstop

#include "subcnv.h"

namespace gr {

SUBCANVAS::SUBCANVAS(const CANVAS &cnv,int ax0,int ay0,double akx,double aky) {
  if (const SUBCANVAS *sub=dynamic_cast<const SUBCANVAS*>(&cnv)) {
    parent=sub->parent;
    x0=sub->kx*ax0+sub->x0;y0=sub->ky*ay0+sub->y0;
    kx=akx*sub->kx;ky=aky*sub->ky;
    invkx=1/kx;invky=1/ky;
  } else {
    kx=akx;ky=aky;
    invkx=1/kx;invky=1/ky;
    x0=ax0;y0=ay0;
    parent=&cnv;
  }
  variables=cnv.variables;
}

FIXSUBCANVAS::FIXSUBCANVAS(const CANVAS &cnv,int ax0,int ay0,double akx,double aky) {
  kx=akx;ky=aky;
  x0=ax0;y0=ay0;
  parent=&cnv;
  invkx=1/kx;invky=1/ky;
}

void FIXSUBCANVAS::fromrect(const ABREC &src,const ABREC &dest) {
  kx=(double)dest.xsize()/(double)src.xsize();
  ky=(double)dest.ysize()/(double)src.ysize();
  XYREC t=dest.topleft(),s=src.topleft();
  x0=t.x-kx*s.x;
  y0=t.y-ky*s.y;
  invkx=1/kx;invky=1/ky;
}

void FIXSUBCANVAS::eqfromrect(const ABREC &src,const ABREC &dest) {
  float akx=(double)dest.xsize()/(double)src.xsize();
  float aky=(double)dest.ysize()/(double)src.ysize();
  kx=ky=min(akx,aky);
  XYREC t=dest.topleft(),s=src.topleft();
  x0=t.x-kx*s.x;
  y0=t.y-ky*s.y;
  invkx=1/kx;invky=1/ky;
  if (akx>aky) {//posuneme ve smeru osy x
    x0+=dest.xsize()/2-kx*src.xsize()/2;
  } else {
    y0+=dest.ysize()/2-ky*src.ysize()/2;
  }
}

#define runpolyfunction(func) {\
  XYREC *mypts=new XYREC[pointcount];\
  for(int i=0;i<pointcount;i++) mypts[i]=_ltop(points[i]);\
  parent->func(mypts,pointcount);\
  delete []mypts;\
}

void COMSUBCANVAS::_polyline(const XYREC *points,int pointcount) const {
  runpolyfunction(polyline);
}
void COMSUBCANVAS::_polygon(const XYREC *points,int pointcount) const {
  runpolyfunction(polygon);
}
void COMSUBCANVAS::_drawpolygon(const XYREC *points,int pointcount) const {
  runpolyfunction(dpolygon);
}
void COMSUBCANVAS::_fillpolygon(const XYREC *points,int pointcount) const {
  runpolyfunction(fpolygon);
}

}

//#pragma package(smart_init)
