#include <winlib.h>
#pragma hdrstop

#include "w32bmps.h"
#include <bmps/bmfile.h>
#include <bmps/bmmanip.h>

extern "C" HDIB ReadPcxFile(const char *filename);
extern "C" HDIB ReadGifFile(const char *filename);

extern "C" void ErrorBox(char *msg) {
  MessageBox(0,msg,"",0);
}

namespace wlib {

void drawbitmaptodc(HDC dc,int x,int y,HBITMAP bmp) {
    SIZE size;
    HDC memdc=CreateCompatibleDC(0);
    HBITMAP oldbmp=SelectObject(memdc,bmp);

    DIBSECTION dib;
    zerorec(dib);
    GetObject(bmp, sizeof(dib), &dib);

    size.cx=dib.dsBm.bmWidth;
    size.cy=dib.dsBm.bmHeight;
    BitBlt(dc,0,0,size.cx,size.cy,memdc,0,0,SRCCOPY);
    SelectObject(memdc,oldbmp);
    DeleteDC(memdc);
}

HPALETTE createpalette(gr::Palette *pal) {
  LOGPALETTE *lp=(LOGPALETTE*)malloc(sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*0x100);
  lp->palVersion=0x300;
  lp->palNumEntries=pal->size;
  pal->copyto(lp->palPalEntry,false);
  HPALETTE res=CreatePalette(lp);
  free(lp);
  return res;
}

zbuf::Bitmap *DIBitmap::tocbitmap(int bits) {
  if (bits==24) bits=32;
  zbuf::Bitmap *res=zbuf::creatergbbitmap(w,h,bits,false,4);
  HDC mdc=createdc();
  if (bits==8 or bits==4) {
    gr::Palette *pal=createpalette();
    readpalbmp(mdc,bmp,res,0,0,0,0,w,h,pal);
    res->setpalette(pal);
  } else {
    readrgbbmp(mdc,bmp,res,0,0,0,0,w,h);
  }
  releasedc(mdc);
  return res;
}

void DIBitmap::loaddib(HDIB dib) {
  this->dib=dib;
  pal=CreateDibPalette(dib);
  bmp=DibToBitmap(dib);
  BITMAPINFO *bmi=(BITMAPINFO*)GlobalLock(dib);
  w=bmi->bmiHeader.biWidth;
  h=bmi->bmiHeader.biHeight;
  bits=bmi->bmiHeader.biBitCount;
  GlobalUnlock(dib);
}

gr::Palette *DIBitmap::createpalette() {
  BITMAPINFO *bmi=(BITMAPINFO*)GlobalLock(dib);
  int count=1<<bmi->bmiHeader.biBitCount;
  gr::Palette *res=new gr::Palette(count);
  res->copyfrom(bmi->bmiColors,true);
  GlobalUnlock(dib);
  return res;
}

void DIBitmap::loadbmp(const char *file) {
  loaddib(ReadBitmapFile(file));
}
void DIBitmap::loadgif(const char *file) {
  loaddib(ReadGifFile(file));
}
void DIBitmap::loadpcx(const char *file) {
  loaddib(ReadPcxFile(file));
}

void DIBitmap::load(const char *file) {
  sFile f(file);
  if (f.testext("bmp")) return loadbmp(file);
  if (f.testext("gif")) return loadgif(file);
  if (f.testext("pcx")) return loadpcx(file);
}

void DIBitmap::blit(HDC dc,int x,int y) {
  HDC mdc=createdc();
  BitBlt(dc,x,y,w,h,mdc,0,0,SRCCOPY);
  releasedc(mdc);
}

void DIBitmap::palblit(HDC dc,int x,int y) {
  HDC mdc=createdc();
  DrawDIBitmap(dc,x,y,dib,0);
  releasedc(mdc);
}

void DIBitmap::setpalcolor(int index,int r,int g,int b,int flags) {
  RGBQUAD rgb;
  rgb.rgbBlue=b;
  rgb.rgbGreen=g;
  rgb.rgbRed=r;
  rgb.rgbReserved=flags;
  BITMAPINFO *bmi=(BITMAPINFO*)GlobalLock(dib);
  bmi->bmiColors[index]=rgb;
  GlobalUnlock(dib);
}

int DIBitmap::getcolorindex(HDC dc,int x,int y) {
  COLORREF color=GetPixel(dc,x,y);
  int index=GetNearestPaletteIndex(pal,color);
  return index;
}

HDC DIBitmap::createdc() {
  xassert(oldbmp==0);
  HDC res=CreateCompatibleDC(0);
  oldbmp=SelectObject(res,bmp);
  return res;
}
void DIBitmap::releasedc(HDC dc) {
  SelectObject(dc,oldbmp);
  DeleteDC(dc);
  oldbmp=0;
}

/*
extern zbuf::Bitmap *zbuf::loadpicturefromfile(const char *file) {
  wlib::DIBitmap bmp;
  bmp.load(file);
  return bmp.tocbitmap(bmp.getbits());
}
*/


}





