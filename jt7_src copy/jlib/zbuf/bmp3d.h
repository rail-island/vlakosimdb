#ifndef bmp3dH
#define bmp3dH

#include <jmath/geom3d.h>
#include "bytebmp.h"

namespace zbuf {
template<class COLOR,class REAL> struct VOXELARRAY;
template<class COLOR,class HI> struct ZBMP;

struct VPOINT {
  int crds[3];
  int &x(const REMAP &map) {return crds[map[0]];}
  int &y(const REMAP &map) {return crds[map[1]];}
  int &z(const REMAP &map) {return crds[map[2]];}
  VPOINT(int x,int y,int z) {crds[0]=x;crds[1]=y;crds[2]=z;}
};

template<class COLOR> struct BMP3D {
  VPOINT bmpsize;
  COLOR transparent;
  int &xs,&ys,&zs;
  int bufsize;
  COLOR *buf;
  COLOR **xscanline;
  COLOR ***scanline;
  BMP3D(int xsize,int ysize,int zsize,COLOR transparentcolor);
  ~BMP3D() {delete []buf;delete []scanline;delete []xscanline;}
  COLOR &at(int x,int y,int z) {return scanline[z][y][x];}
  void fillbox(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color);
  void fillbox(VPOINT p1,VPOINT p2,REMAP map,COLOR color) {fillbox(p1.x(map),p1.y(map),p1.z(map),p2.x(map),p2.y(map),p2.z(map),color);}
  void drawframe(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color);
  template<class REAL> VOXELARRAY<COLOR,REAL> *shape(REAL dx,REAL dy,REAL dz,double koef);//vyrobi jen obrys(ty voxely, ktere maji aspon jednoho pruhledneho souseda
  template<class REAL> VOXELARRAY<COLOR,REAL> *qualityshape(int quality);
  //vyrobi jen obrys(ty voxely, ktere maji aspon jednoho pruhledneho souseda do vzdalenosti quality
  COLOR vget(int x,int y,int z) {if (x>=0 and y>=0 and z>=0 and x<xs and y<ys and z<zs) return at(x,y,z);return transparent;}
  COLOR vget(VPOINT pt,REMAP map) {return vget(pt.x(map),pt.y(map),pt.z(map));}
  //pokud jsou souradnice v bitmape, vrati barvu voxelu, jinak pruhlednou
  void vset(int x,int y,int z,COLOR color) {if (x>=0 and y>=0 and z>=0 and x<xs and y<ys and z<zs) at(x,y,z)=color;}
  void vset(VPOINT pt,REMAP map,COLOR color) {vset(pt.x(map),pt.y(map),pt.z(map),color);}
  bool hastransparent(int x,int y,int z,int maxdist);//zjisti, zda v okoli [x,y,z] je pruhledny bod
  //ve vzdalenosti maxdist
  bool hastransparent(int x,int y,int z) {
    return vget(x+1,y,z)==transparent or vget(x-1,y,z)==transparent
           or vget(x,y+1,z)==transparent or vget(x,y-1,z)==transparent
           or vget(x,y,z+1)==transparent or vget(x,y,z-1)==transparent;
  }
  void zcut(BYTEBMP<COLOR> &bmp,int dx,int dy);//posune o dx doprava vuci bmp
  //wcut udela prunik uktualni bitmapy a telesem vzniklym posunem nbmp ve smeru osy w
  //v bmp je jedina vyznamovova barva transparent-pruhledna,ostatni nepruhledna
  void zcut(BYTEBMP<COLOR> &bmp,int dx,int dy,REMAP map);
  void zcolor(BYTEBMP<COLOR> &bmp,int dx,int dy,REMAP map,int numcolor);
  void zcolorback(BYTEBMP<COLOR> &bmp,int dx,int dy,REMAP map,int numcolor);
  void clear(COLOR color=transparent) {
    for(int i=0;i<bufsize;i++) buf[i]=color;
  }
  void fillaround(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color);
  void fillaround(VPOINT p1,VPOINT p2,REMAP map,COLOR color) {fillaround(p1.x(map),p1.y(map),p1.z(map),p2.x(map),p2.y(map),p2.z(map),color);}
};

template<class COLOR,class REAL> struct VOXEL {
  point3d<REAL> pt;
  COLOR color;
  VOXEL(REAL x,REAL y,REAL z,COLOR acolor):pt(x,y,z) {color=acolor;}
  VOXEL(const point3d<REAL> &apt,COLOR acolor):pt(apt) {color=acolor;}
  VOXEL():pt(0,0,0) {color=0;}
};
template<class COLOR,class REAL> inline VOXEL<COLOR,REAL> operator *(const transform3d<REAL> &t,const VOXEL<COLOR,REAL> &v) {
  return VOXEL<COLOR,REAL>(t*v.pt,v.color);
}
template<class COLOR> struct COLORANDLINE {
  COLOR color;
  bool isline;
  COLORANDLINE(COLOR acolor,bool aisline) {color=acolor;isline=aisline;}
  operator int () {return color;}
  COLORANDLINE(COLOR acolor) {isline=false;color=acolor;}
  COLORANDLINE() {isline=false;color=0;}
};
template<class COLOR> inline operator ==(const COLORANDLINE<COLOR> &a,const COLORANDLINE<COLOR> &b) {return a.color==b.color;}

}

#endif
