#ifndef bmpwrapH
#define bmpwrapH

//#delbeg [ABAZ]

#include <gr/comgr.h>

namespace zbuf {

struct PixelFormat {
  int rmask,gmask,bmask,amask;
  int bits;
  void init(int bits,bool redFF);
  PixelFormat() {init(16,true);}
};

struct PixelMap {
  virtual ~PixelMap() {}
  virtual int bits()=0;
  virtual void *buffer()=0;
  virtual int getw()=0;
  virtual int geth()=0;

  virtual int pitch()=0;//length of scanline in bytes
//  virtual int rmask()=0;
//  virtual int gmask()=0;
//  virtual int bmask()=0;
};

struct Bitmap:PixelMap {
  virtual ~Bitmap() {}
  virtual void clear()=0;
  virtual void clear(int color)=0;
  virtual void putpixel(int x,int y,int color)=0;
  virtual void rectangle(int x1,int y1,int x2,int y2,int color)=0;
  virtual void fillrect(int x1,int y1,int x2,int y2,int color)=0;
  virtual void blit(Bitmap *src,int dx,int dy)=0;
  virtual void line(int x1,int y1,int x2,int y2,int color)=0;
  virtual void drawellipse(int x1,int y1,int x2,int y2,int color)=0;
  virtual void fillellipse(int x1,int y1,int x2,int y2,int color)=0;

  virtual void assign(Bitmap *src)=0;
  virtual void reflectx()=0;
  virtual void reflecty()=0;
  virtual int bits()=0;//vraci barevnou hloubku
  virtual void *buffer()=0;
  virtual int getw()=0;
  virtual int geth()=0;
  virtual void copyrect(Bitmap *src,int destx,int desty,int srcx,int srcy,int w,int h)=0;
  virtual void transparentcopyrect(Bitmap *src,int destx,int desty,int srcx,int srcy,int w,int h)=0;
//  void copyrect(Bitmap *src,int destx,int desty,int srcx,int srcy,int w,int h) {}
//  void transparentcopyrect(Bitmap *src,int destx,int desty,int srcx,int srcy,int w,int h) {}
  //bere ohled na pruhlednost src
  virtual void move(int dx,int dy)=0;//posunuti bitmapy
  virtual void settransparentcolor(int color)=0;
  virtual int getpixel(int x,int y)=0;
  virtual int rgbtoint(gr::RGBColor rgb)=0;
  virtual gr::RGBColor inttorgb(int color)=0;
  virtual gr::Palette *getpalette() {return NULL;}
  virtual void setpalette(gr::Palette *palette) {delete palette;}
};

Bitmap *creatergbbitmap(int w,int h,int bits,bool transparent0,int align=1,bool reversey=false);


}

//#delend [ABAZ]

#endif
