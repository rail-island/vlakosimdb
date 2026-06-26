#ifndef w32bmpsH
#define w32bmpsH

#include <zbuf/zbufwrap.h>
#include <win/winblit.h>

namespace wlib {

struct DIBitmap {
  DIBitmap() {dib=0;palsize=0;pal=0;bmp=0;w=h=0;oldbmp=0;}
  ~DIBitmap() {done();}
  void blit(HDC dc,int x,int y);
  void palblit(HDC dc,int x,int y);//blit, ktery reaguje na zmenu palety pomoci setpalcolor
  void setpalcolor(int index,int r,int g,int b,int flags=0);
  HBITMAP getbmp() {return bmp;}
  int getcolorindex(HDC dc,int x,int y);
  HDC createdc();
  void releasedc(HDC dc);
  int getw() {return w;}
  int geth() {return h;}
  zbuf::Bitmap *tocbitmap(int bits);
  void load(const char *file);
  gr::Palette *createpalette();
  int getbits() {return bits;}
private:
  void done() {
    if (dib) GlobalFree(dib);dib=0;
    if (pal) DeleteObject(pal);pal=0;
    if (bmp) DeleteObject(bmp);bmp=0;
    w=h=0;palsize=0;
  }
  void loadbmp(const char *file);
  void loadgif(const char *file);
  void loadpcx(const char *file);
  void loaddib(HANDLE dib);
  HANDLE dib;
  int palsize;
  HPALETTE pal;
  HBITMAP bmp;
  int w,h;
  HBITMAP oldbmp;
  int bits;
};

void drawbitmaptodc(HDC dc,int x,int y,HBITMAP bmp);
HPALETTE createpalette(gr::Palette *pal);


}

#endif
