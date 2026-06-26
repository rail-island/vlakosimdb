#include <jlib.h>
#pragma hdrstop

//#delbeg [ABAZ]

#include "bmpwrap.h"

#pragma option -O2
#pragma option -Ov
#pragma option -Oi
#pragma option -Og
#pragma instristic
#pragma option -r
#pragma option -ff
#pragma option -vi

#include "bmpwrap.h"
#include "bytebmp.cpp"
#include <gr/comgr.h>

namespace zbuf {
using namespace gr;


void PixelFormat::init(int bits,bool redFF) {
  amask=0;
  if (bits==16) {
    rmask=0x1F;gmask=0x1F<<5;bmask=0x1F<<10;
  }
  if (bits==32) {
    rmask=0xFF0000;gmask=0xFF00;bmask=0xFF;
    if (redFF) exchange(rmask,bmask);
  }
}

template<class T> struct BaseNativeBitmap:Bitmap {
  BYTEBMP<T> bmp;
  BaseNativeBitmap(int w,int h,int transparent,int align,bool reversey):bmp(w,h,transparent,reversey,align) {}
  ~BaseNativeBitmap() {}
  void clear() {bmp.clear();}
  void clear(int color) {bmp.clear(color);}
  void putpixel(int x,int y,int color) {return bmp.putpixel(x,y,color);}
  int getpixel(int x,int y) {return bmp.getpixel(x,y);}
  void rectangle(int x1,int y1,int x2,int y2,int color) {bmp.rectangle(x1,y1,x2,y2,color);}
  void fillrect(int x1,int y1,int x2,int y2,int color) {bmp.fillrect(x1,y1,x2,y2,color);}
  void settransparentcolor(int color) {bmp.transparent=color;}
  void blit(Bitmap *src,int dx,int dy) {
    xassert(typeid(*src)==typeid(*this));
    bmp.blit(((BaseNativeBitmap<T>*)src)->bmp,dx,dy);
  }
  void line(int x1,int y1,int x2,int y2,int color) {bmp.line(x1,y1,x2,y2,color);}
  void assign(Bitmap *src) {
    xassert(typeid(*src)==typeid(*this));
    bmp.assign(((BaseNativeBitmap<T>*)src)->bmp);
  }
/*  void assign(ZBitmap *src) {
    xassert(src->bits()==bits());// typeid(*src)==typeid(BaseNativeZBitmap<T>));
    xassert(bmp.W==src->getw());
    xassert(bmp.H==src->geth());
    memmove(bmp.buf,((BaseNativeZBitmap<T>*)src)->bmp.buf,bmp.W*bmp.H*sizeof(T));
  }*/
  void copyrect(Bitmap *src,int destx,int desty,int srcx,int srcy,int w,int h) {
    xassert(typeid(*src)==typeid(*this));
    bmp.copyrect(((BaseNativeBitmap<T>*)src)->bmp,destx,desty,srcx,srcy,w,h);
  }
  void transparentcopyrect(Bitmap *src,int destx,int desty,int srcx,int srcy,int w,int h) {
    xassert(typeid(*src)==typeid(*this));
    BaseNativeBitmap<T> *s=(BaseNativeBitmap<T>*)src;
    bmp.transparentcopyrect(s->bmp,destx,desty,srcx,srcy,w,h);
  }
  void reflectx() {bmp.reflectx();}
  void reflecty() {bmp.reflecty();}
  void *buffer() {return bmp.buf;}
  int getw() {return bmp.W;}
  int geth() {return bmp.H;}
  void drawellipse(int x1,int y1,int x2,int y2,int color) {bmp.fellipse(x1,y1,x2,y2,color);}
  void fillellipse(int x1,int y1,int x2,int y2,int color) {bmp.fillellipse(x1,y1,x2,y2,color);}
  void move(int dx,int dy) {}//bmp.move(dx,dy);}
  int pitch() {return bmp.alignedW*sizeof(T);}
};
struct NativeBitmap16:BaseNativeBitmap<short> {
  int bits() {return 16;}
  NativeBitmap16(int w,int h,int transparent,int align,bool reversey):BaseNativeBitmap<short>(w,h,transparent,align/2,reversey) {}
  int rgbtoint(RGBColor rgb) {
    return rgb.torevint16();
  }
  virtual RGBColor inttorgb(int color) {
    xabstract;
//    RGBColor res;
//    res.fromrevint16(color);
//    return res;
  }
//  int rmask() {return 0x1F;}
//  int gmask() {return 0x1F<<5;}
//  int bmask() {return 0x1F<<10;}
};
struct NativeBitmap8:BaseNativeBitmap<unsigned char> {
  int bits() {return 8;}
  Palette *palette;
  NativeBitmap8(int w,int h,int transparent,int align,bool reversey):BaseNativeBitmap<unsigned char>(w,h,transparent,align,reversey) {
    palette=NULL;
  }
  int rgbtoint(RGBColor rgb) {
    xabstract;
  }
  virtual RGBColor inttorgb(int color) {
    xabstract;
  }
  ~NativeBitmap8() {delete palette;}
  gr::Palette *getpalette() {return palette;}
  void setpalette(Palette *pal) {
    delete palette;
    palette=pal;
  }
//  int rmask() {return 0;}
//  int gmask() {return 0;}
//  int bmask() {return 0;}
};
struct NativeBitmap32:BaseNativeBitmap<int> {
  int bits() {return 32;}
  NativeBitmap32(int w,int h,int transparent,int align,bool reversey):BaseNativeBitmap<int>(w,h,transparent,align/4,reversey) {}
  int rgbtoint(RGBColor rgb) {
    return rgb.torevint();
  }
  virtual RGBColor inttorgb(int color) {
    RGBColor res;
    res.fromrevint(color);
    return res;
  }
  void transparentcopyrect(Bitmap *src,int destx,int desty,int srcx,int srcy,int w,int h) {
    if (typeid(*src)==typeid(NativeBitmap8)) {
      NativeBitmap8 *s=(NativeBitmap8*)src;
      int pal[0x100];
      s->palette->fillrevint(pal);
      bmp.transparentcopyrectpalette(s->bmp,destx,desty,srcx,srcy,w,h,pal);
    } else BaseNativeBitmap<int>::transparentcopyrect(src,destx,desty,srcx,srcy,w,h);
  }
//  int rmask() {return 0xFF;}
//  int gmask() {return 0xFF00;}
//  int bmask() {return 0xFF0000;}
};



Bitmap *creatergbbitmap(int w,int h,int bits,bool transparent0,int align,bool reversey) {
  switch (bits) {
    case 32:return new NativeBitmap32(w,h,(not transparent0)*clnnone,align,reversey);
    case 16:return new NativeBitmap16(w,h,(not transparent0)*clnone16bit,align,reversey);
    case 8:return new NativeBitmap8(w,h,(not transparent0)*clnone8bit,align,reversey);
  }
  return NULL;
}


}

//#delend [ABAZ]

