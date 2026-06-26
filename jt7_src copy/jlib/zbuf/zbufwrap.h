#ifndef zbufwrapH
#define zbufwrapH

#include <jmath/geom3d.h>
#include <gr/comgr.h>
#include "bmpwrap.h"
#include <com/mzip.h>

namespace zbuf {
using namespace jmath;

class ZBitmap;class Voxels;class Bitmap;

struct Voxels {
  virtual ~Voxels() {}
  virtual void blittozbmp(ZBitmap *bmp,int x0,int y0,float z0,float ky,float kz)=0;
  virtual void blittozbmp(ZBitmap *bmp,int x0,int y0,float z0,const matrix3d<float> &m,float ky,float kz)=0;
  virtual void translate(float dx,float dy,float dz)=0;
  virtual Voxels *clone()=0;
  virtual void load(BINREAD &fr)=0;
  virtual void fromzbmp(ZBitmap *src,int x0,int y0,float z0,float kx,float ky)=0;
  virtual point3d<float> mincoords()=0;
  virtual point3d<float> maxcoords()=0;
  virtual void reflectx(float x0)=0;
  virtual void reflecty(float y0)=0;
  virtual void reflectz(float z0)=0;
  virtual int bits()=0;//vraci barevnou hloubku
  virtual bool frontlines()=0;
  //vraci, zda se u kazdeho voxelu pamatuje, zda vznikl nakreslenim cary
  virtual void transp(REMAP map)=0;
  inline void load(string file) {
    GZREAD fr(file.c_str());
    load(fr);
  }
  virtual void foreach(VoxelFunction vf,void *data)=0;
  virtual float diam()=0;
};

struct ZBitmap:PixelMap {
  virtual ~ZBitmap() {}
  virtual void clear()=0;
  virtual void clear(int color)=0;
  virtual void clear(int color,float hi)=0;
  virtual void putpixel(int x,int y,float z,int color)=0;
  virtual void rectangle(int x1,int y1,int x2,int y2,float z,int color)=0;
  virtual void fillrect(int x1,int y1,int x2,int y2,float z,int color)=0;
  virtual void blit(ZBitmap *src,int dx,int dy,float dz)=0;
  virtual void line(int x1,int y1,float z1,int x2,int y2,float z2,int color)=0;
  virtual void line(const XYHREC<float> &a,const XYHREC<float> &b,int color)=0;
  virtual void assign(ZBitmap *src)=0;
  virtual void copyrect(ZBitmap *src,int destx,int desty,int srcx,int srcy,int w,int h)=0;
  virtual void reflectx()=0;
  virtual void reflecty()=0;
  virtual void triangle(const XYHREC<float> &a,const XYHREC<float> &b,const XYHREC<float> &c,int color)=0;
  virtual int bits()=0;//vraci barevnou hloubku
  virtual void *buffer()=0;
  virtual int getw()=0;
  virtual int geth()=0;
  virtual void copyto(Bitmap *dest)=0;
  virtual void minmaxcoords(XYREC &min,XYREC &max)=0;
  virtual void move(int dx,int dy)=0;//posunuti bitmapy
  virtual void clearrect(int x1,int y1,int x2,int y2)=0;
  virtual void clearrect(int x1,int y1,int x2,int y2,int color)=0;
  virtual void settransparentcolor(int color)=0;
  virtual void drawellipse(int x1,int y1,int x2,int y2,float z,int color)=0;
  virtual void fillellipse(int x1,int y1,int x2,int y2,float z,int color)=0;
};


ZBitmap *creatergbzbitmap(int w,int h,int bits,int align=1,bool reversey=false);
//bits==16,32;align:pocet bytu, na ktery se zarovna kazda radka
Voxels *createvoxels(int bits,bool lines);
inline Voxels *createvoxels(string file,int bits,bool lines) {
  Voxels *res=createvoxels(bits,lines);
  res->load(file);
  return res;
}

//extern Bitmap *loadpicturefromfile(const char *file);

}

#endif
