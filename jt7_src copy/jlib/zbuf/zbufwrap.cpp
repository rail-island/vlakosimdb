#include <jlib.h>
#pragma hdrstop

#pragma option -O2
#pragma option -Ov
#pragma option -Oi
#pragma option -Og
#pragma instristic
#pragma option -r
#pragma option -ff
#pragma option -vi

#include "zbufwrap.h"
#include "zbmp.cpp"
#include "voxels.cpp"
#include "bytebmp.cpp"
#include <gr/comgr.h>

namespace zbuf {
using namespace gr;


inline void portvoxeltonativevoxel(void *,PortVoxel &pv,VOXEL<int,float> &vox) {
  vox.color=gr::reversergb(gr::makergb(pv.r,pv.g,pv.b));
  vox.pt.x=pv.x;
  vox.pt.y=pv.y;
  vox.pt.z=pv.z;
}
inline void portvoxeltonativevoxel_line(void *,PortVoxel &pv,VOXEL<COLORANDLINE<int>,float> &vox) {
  vox.color.color=gr::reversergb(gr::makergb(pv.r,pv.g,pv.b));
  vox.pt.x=pv.x;
  vox.pt.y=pv.y;
  vox.pt.z=pv.z;
  vox.color.isline=pv.flags;
}
inline void nativevoxeltoportvoxel(void *,PortVoxel &pv,VOXEL<COLORANDLINE<int>,float> &vox) {
  pv.x=vox.pt.x;
  pv.y=vox.pt.y;
  pv.z=vox.pt.z;
  pv.flags=vox.color.isline;
  int bgr=gr::reversergb(vox.color.color);
  pv.r=gr::nativervalue(bgr);
  pv.g=gr::nativegvalue(bgr);
  pv.b=gr::nativebvalue(bgr);
}
inline void nativevoxeltoportvoxel(void *,PortVoxel &pv,VOXEL<int,float> &vox) {
  pv.x=vox.pt.x;
  pv.y=vox.pt.y;
  pv.z=vox.pt.z;
  int bgr=gr::reversergb(vox.color);
  pv.r=gr::nativervalue(bgr);
  pv.g=gr::nativegvalue(bgr);
  pv.b=gr::nativebvalue(bgr);
}




inline void portvoxeltonativevoxel(void *,PortVoxel &pv,VOXEL<short,float> &vox) {
  vox.color=RGBColor(pv.r,pv.g,pv.b).torevint16();
  vox.pt.x=pv.x;
  vox.pt.y=pv.y;
  vox.pt.z=pv.z;
}
inline void portvoxeltonativevoxel_line(void *,PortVoxel &pv,VOXEL<COLORANDLINE<short>,float> &vox) {
  vox.color.color=RGBColor(pv.r,pv.g,pv.b).torevint16();
  vox.pt.x=pv.x;
  vox.pt.y=pv.y;
  vox.pt.z=pv.z;
  vox.color.isline=pv.flags;
}
inline void nativevoxeltoportvoxel(void *,PortVoxel &pv,VOXEL<COLORANDLINE<short>,float> &vox) {
  xassert(0);
  pv.x=vox.pt.x;
  pv.y=vox.pt.y;
  pv.z=vox.pt.z;
  pv.flags=vox.color.isline;
  int bgr=gr::reversergb(vox.color.color);
  pv.r=gr::nativervalue(bgr);
  pv.g=gr::nativegvalue(bgr);
  pv.b=gr::nativebvalue(bgr);
}
inline void nativevoxeltoportvoxel(void *,PortVoxel &pv,VOXEL<short,float> &vox) {
  xassert(0);
  pv.x=vox.pt.x;
  pv.y=vox.pt.y;
  pv.z=vox.pt.z;
  int bgr=gr::reversergb(vox.color);
  pv.r=gr::nativervalue(bgr);
  pv.g=gr::nativegvalue(bgr);
  pv.b=gr::nativebvalue(bgr);
}

template<class T> struct BaseNativeZBitmap:ZBitmap {
  ZBMP<T,float> bmp;
  BaseNativeZBitmap(int w,int h,int transparent,int align,bool reversey):bmp(w,h,transparent,-1E10f,reversey,align) {}
//  ~BaseNativeZBitmap() {}
  void clear() {bmp.clear();}
  void clear(int color) {bmp.clear(color,bmp.minimalhi);}
  void clear(int color,float hi) {bmp.clear(color,hi);}
  void putpixel(int x,int y,float z,int color) {bmp.putpixel(x,y,z,color);}
  void rectangle(int x1,int y1,int x2,int y2,float z,int color) {bmp.rectangle(x1,y1,x2,y2,z,color);}
  void fillrect(int x1,int y1,int x2,int y2,float z,int color) {bmp.fillrect(x1,y1,x2,y2,z,color);}
  void blit(ZBitmap *src,int dx,int dy,float dz) {
    xassert(typeid(*src)==typeid(*this));
    bmp.blit(((BaseNativeZBitmap<T>*)src)->bmp,dx,dy,dz);
  }
  void line(int x1,int y1,float z1,int x2,int y2,float z2,int color) {bmp.line(x1,y1,z1,x2,y2,z2,color);}
  void line(const XYHREC<float> &a,const XYHREC<float> &b,int color) {bmp.line(a,b,color);}
  void assign(ZBitmap *src) {
    xassert(typeid(*src)==typeid(*this));
    bmp.assign(((BaseNativeZBitmap<T>*)src)->bmp);
  }
  void copyrect(ZBitmap *src,int destx,int desty,int srcx,int srcy,int w,int h) {
    xassert(typeid(*src)==typeid(*this));
    bmp.copyrect(((BaseNativeZBitmap<T>*)src)->bmp,destx,desty,srcx,srcy,w,h);
  }
  void reflectx() {bmp.reflectx();}
  void reflecty() {bmp.reflecty();}
  void triangle(const XYHREC<float> &a,const XYHREC<float> &b,const XYHREC<float> &c,int color) {
    bmp.triangle(a,b,c,color);
  }
  void *buffer() {return bmp.buf;}
  int getw() {return bmp.W;}
  int geth() {return bmp.H;}
  void minmaxcoords(XYREC &min,XYREC &max) {bmp.minmaxcoords(min,max);}
  void move(int dx,int dy) {bmp.move(dx,dy);}
  void clearrect(int x1,int y1,int x2,int y2) {bmp.clearrect(x1,y1,x2,y2,bmp.transparent);}
  void clearrect(int x1,int y1,int x2,int y2,int color) {bmp.clearrect(x1,y1,x2,y2,color);}
  void settransparentcolor(int color) {bmp.transparent=color;}
  void drawellipse(int x1,int y1,int x2,int y2,float z,int color) {bmp.drawellipse(x1,y1,x2,y2,z,color);}
  void fillellipse(int x1,int y1,int x2,int y2,float z,int color) {bmp.fillellipse(x1,y1,x2,y2,z,color);}
  void copyto(Bitmap *dest) {
    xassert(dest->bits()==bits());// typeid(*src)==typeid(BaseNativeZBitmap<T>));
    xassert(bmp.W==dest->getw());
    xassert(bmp.H==dest->geth());
//    memmove(  bmp.buf,((BaseNativeZBitmap<T>*)src)->bmp.buf,bmp.W*bmp.H*sizeof(T));
    memmove(dest->buffer(),bmp.buf,bmp.W*bmp.H*sizeof(T));
  }
  int pitch() {return bmp.alignedW*sizeof(T);}
};

struct NativeZBitmap32:BaseNativeZBitmap<int> {
  int bits() {return 32;}
//  int rmask() {return 0xFF;}
//  int gmask() {return 0xFF00;}
//  int bmask() {return 0xFF0000;}
  NativeZBitmap32(int w,int h,int align,bool reversey):BaseNativeZBitmap<int>(w,h,clnnone,align/4,reversey) {}
};
struct NativeZBitmap16:BaseNativeZBitmap<short> {
  int bits() {return 16;}
//  int rmask() {return 0x1F;}
//  int gmask() {return 0x1F<<5;}
//  int bmask() {return 0x1F<<10;}
  NativeZBitmap16(int w,int h,int align,bool reversey):BaseNativeZBitmap<short>(w,h,clnone16bit,align/2,reversey) {}
};


template<class VOXS> struct BaseVoxels:Voxels {
  VOXS voxs;
  ~BaseVoxels() {}
  BaseVoxels():voxs(jsystem->tempblock()) {}
  BaseVoxels(const BaseVoxels &src):voxs(src.voxs) {}
  void translate(float dx,float dy,float dz) {voxs.translate(dx,dy,dz);}
  point3d<float> mincoords() {return voxs.mincoords();}
  point3d<float> maxcoords() {return voxs.maxcoords();}
  void reflectx(float x0) {voxs.reflectx(x0);}
  void reflecty(float y0) {voxs.reflecty(y0);}
  void reflectz(float z0) {voxs.reflectz(z0);}
  void transp(REMAP map) {voxs.transp(map);}
  void foreach(VoxelFunction vf,void *data) {voxs.foreach(vf,data);}
  float diam() {return voxs.diam();}
};

typedef BaseVoxels<VOXELARRAY<int,float> > Base_32;
typedef BaseVoxels<VOXELARRAY<COLORANDLINE<int>,float> > LBase_32;

typedef BaseVoxels<VOXELARRAY<short,float> > Base_16;
typedef BaseVoxels<VOXELARRAY<COLORANDLINE<short>,float> > LBase_16;

struct NativeVoxels32:Base_32 {
  void blittozbmp(ZBitmap *bmp,int x0,int y0,float z0,float ky,float kz) {
    xassert(typeid(*bmp)==typeid(NativeZBitmap32));
    voxs.blittozbmp<float,float>(&((NativeZBitmap32*)bmp)->bmp,x0,y0,z0,ky,kz);
  }
  void blittozbmp(ZBitmap *bmp,int x0,int y0,float z0,const matrix3d<float> &m,float ky,float kz) {
    xassert(typeid(*bmp)==typeid(NativeZBitmap32));
    voxs.blittozbmp<float,float>(&((NativeZBitmap32*)bmp)->bmp,x0,y0,z0,m,ky,kz);
  }
  void load(BINREAD &fr) {voxs.load(fr,portvoxeltonativevoxel,0);}
  void fromzbmp(ZBitmap *src,int x0,int y0,float z0,float kx,float ky) {
    xassert(typeid(*src)==typeid(NativeZBitmap32));
    voxs.fromzbmp(&((NativeZBitmap32*)src)->bmp,x0,y0,z0,kx,ky);
  }

  NativeVoxels32() {}
  NativeVoxels32(const NativeVoxels32 &src):Base_32(src) {}
  Voxels *clone() {return new NativeVoxels32(*this);}
  int bits() {return 32;}
  bool frontlines() {return false;}
};

struct NativeVoxels16:Base_16 {
  void blittozbmp(ZBitmap *bmp,int x0,int y0,float z0,float ky,float kz) {
    xassert(typeid(*bmp)==typeid(NativeZBitmap16));
    voxs.blittozbmp<float,float>(&((NativeZBitmap16*)bmp)->bmp,x0,y0,z0,ky,kz);
  }
  void blittozbmp(ZBitmap *bmp,int x0,int y0,float z0,const matrix3d<float> &m,float ky,float kz) {
    xassert(typeid(*bmp)==typeid(NativeZBitmap16));
    voxs.blittozbmp<float,float>(&((NativeZBitmap16*)bmp)->bmp,x0,y0,z0,m,ky,kz);
  }
  void load(BINREAD &fr) {voxs.load(fr,portvoxeltonativevoxel,0);}
  void fromzbmp(ZBitmap *src,int x0,int y0,float z0,float kx,float ky) {
    xassert(typeid(*src)==typeid(NativeZBitmap16));
    voxs.fromzbmp(&((NativeZBitmap16*)src)->bmp,x0,y0,z0,kx,ky);
  }

  NativeVoxels16() {}
  NativeVoxels16(const NativeVoxels16 &src):Base_16(src) {}
  Voxels *clone() {return new NativeVoxels16(*this);}
  int bits() {return 16;}
  bool frontlines() {return false;}
};


struct LineNativeVoxels32:LBase_32 {
  void load(BINREAD &fr) {voxs.load(fr,portvoxeltonativevoxel_line,0);}
  void fromzbmp(ZBitmap *src,int x0,int y0,float z0,float kx,float ky) {xabstract;}
  void blittozbmp(ZBitmap *bmp,int x0,int y0,float z0,float ky,float kz) {
    xassert(typeid(*bmp)==typeid(NativeZBitmap32));
    voxs.lineblittozbmp<float,float>(&((NativeZBitmap32*)bmp)->bmp,x0,y0,z0,ky,kz);
  }
  void blittozbmp(ZBitmap *bmp,int x0,int y0,float z0,const matrix3d<float> &m,float ky,float kz) {
    xassert(typeid(*bmp)==typeid(NativeZBitmap32));
    voxs.lineblittozbmp<float,float>(&((NativeZBitmap32*)bmp)->bmp,x0,y0,z0,m,ky,kz);
  }

  LineNativeVoxels32() {}
  LineNativeVoxels32(const LineNativeVoxels32 &src):LBase_32(src) {}
  Voxels *clone() {return new LineNativeVoxels32(*this);}
  int bits() {return 32;}
  bool frontlines() {return true;}
};

struct LineNativeVoxels16:LBase_16 {
  void load(BINREAD &fr) {voxs.load(fr,portvoxeltonativevoxel_line,0);}
  void fromzbmp(ZBitmap *src,int x0,int y0,float z0,float kx,float ky) {xabstract;}
  void blittozbmp(ZBitmap *bmp,int x0,int y0,float z0,float ky,float kz) {
    xassert(typeid(*bmp)==typeid(NativeZBitmap16));
    voxs.lineblittozbmp<float,float>(&((NativeZBitmap16*)bmp)->bmp,x0,y0,z0,ky,kz);
  }
  void blittozbmp(ZBitmap *bmp,int x0,int y0,float z0,const matrix3d<float> &m,float ky,float kz) {
    xassert(typeid(*bmp)==typeid(NativeZBitmap16));
    voxs.lineblittozbmp<float,float>(&((NativeZBitmap16*)bmp)->bmp,x0,y0,z0,m,ky,kz);
  }

  LineNativeVoxels16() {}
  LineNativeVoxels16(const LineNativeVoxels16 &src):LBase_16(src) {}
  Voxels *clone() {return new LineNativeVoxels16(*this);}
  int bits() {return 16;}
  bool frontlines() {return true;}
};


ZBitmap *creatergbzbitmap(int w,int h,int bits,int align,bool reversey) {
  switch (bits) {
    case 32:return new NativeZBitmap32(w,h,align,reversey);
    case 16:return new NativeZBitmap16(w,h,align,reversey);
  }
  return NULL;
}
Voxels *createvoxels(int bits,bool lines) {
  switch (bits) {
    case 32:if (lines) return new LineNativeVoxels32();return new NativeVoxels32();
    case 16:if (lines) return new LineNativeVoxels16();return new NativeVoxels16();
  }
  return NULL;
}


}

//#pragma package(smart_init)


