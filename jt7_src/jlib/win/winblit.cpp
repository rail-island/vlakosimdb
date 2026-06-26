#include <winlib.h>
#pragma hdrstop

//#delbeg [ABAZ]

#include "winblit.h"

namespace wlib {

void blitbmp(HDC destdc,zbuf::Bitmap *src,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth) {
  BITMAPINFO bi;bi.bmiHeader.biSize=sizeof(bi.bmiHeader);
  int w=src->getw(),h=src->geth();
  bi.bmiHeader.biWidth=w;bi.bmiHeader.biHeight=h;
  bi.bmiHeader.biPlanes=1;bi.bmiHeader.biBitCount=src->bits();
  bi.bmiHeader.biCompression=BI_RGB;bi.bmiHeader.biSizeImage=0;
  bi.bmiHeader.biXPelsPerMeter=0;
  bi.bmiHeader.biYPelsPerMeter=0;
  bi.bmiHeader.biClrUsed=0;
  bi.bmiHeader.biClrImportant==0;

  SetDIBitsToDevice(destdc,xdest,ydest,rectw,recth,xsrc,ysrc,0,h,src->buffer(),&bi,DIB_RGB_COLORS);
//  SetDIBitsToDevice(destdc,xdest,ydest,rectw,recth,xsrc,ysrc,0,recth,src->buffer(),&bi,DIB_RGB_COLORS);
//  SetDIBitsToDevice(destdc,xdest,ydest,rectw,recth,xsrc,ysrc,(h-ysrc)-recth,h,src->buffer(),&bi,DIB_RGB_COLORS);
}
void blitpalbmp(HDC destdc,zbuf::Bitmap *src,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth,gr::Palette *pal) {
  BITMAPINFO *pbi=(BITMAPINFO*)malloc(sizeof(BITMAPINFO)+sizeof(RGBQUAD)*0x100),&bi=*pbi;
  bi.bmiHeader.biSize=sizeof(bi.bmiHeader);
  pal->copyto(bi.bmiColors,true);
  int w=src->getw(),h=src->geth();
  bi.bmiHeader.biWidth=w;bi.bmiHeader.biHeight=h;
  bi.bmiHeader.biPlanes=1;bi.bmiHeader.biBitCount=src->bits();
  bi.bmiHeader.biCompression=BI_RGB;bi.bmiHeader.biSizeImage=0;
  bi.bmiHeader.biXPelsPerMeter=0;
  bi.bmiHeader.biYPelsPerMeter=0;
  bi.bmiHeader.biClrUsed=0;
  bi.bmiHeader.biClrImportant==0;

  SetDIBitsToDevice(destdc,xdest,ydest,rectw,recth,xsrc,ysrc,0,h,src->buffer(),&bi,DIB_RGB_COLORS);
  free(pbi);
}



void readrgbbmp(HDC srcdc,HBITMAP srcbmp,zbuf::Bitmap *dest,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth) {
  BITMAPINFO bi;
  bi.bmiHeader.biSize=sizeof(bi.bmiHeader);
  int w=dest->getw(),h=dest->geth();
  bi.bmiHeader.biWidth=w;bi.bmiHeader.biHeight=h;
  bi.bmiHeader.biPlanes=1;bi.bmiHeader.biBitCount=dest->bits();
  bi.bmiHeader.biCompression=BI_RGB;bi.bmiHeader.biSizeImage=0;
  bi.bmiHeader.biXPelsPerMeter=0;
  bi.bmiHeader.biYPelsPerMeter=0;
  bi.bmiHeader.biClrUsed=0;
  bi.bmiHeader.biClrImportant==0;
  GetDIBits(srcdc,srcbmp,0,recth,dest->buffer(),&bi,DIB_RGB_COLORS);
}
void readrgbbmp(HDC srcdc,zbuf::Bitmap *dest,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth) {
  HBITMAP bmp=CreateCompatibleBitmap(srcdc,rectw,recth);
  HDC memdc=CreateCompatibleDC(srcdc);
  HBITMAP oldbmp=SelectObject(memdc,bmp);
  BitBlt(memdc,0,0,rectw,recth,srcdc,xsrc,ysrc,SRCCOPY);
  readrgbbmp(memdc,bmp,dest,xdest,ydest,xsrc,ysrc,rectw,recth);
  SelectObject(memdc,oldbmp);
  DeleteObject(bmp);
  DeleteDC(memdc);
}

void readpalbmp(HDC srcdc,HBITMAP srcbmp,zbuf::Bitmap *dest,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth,gr::Palette *pal) {
  BITMAPINFO *pbi=(BITMAPINFO*)malloc(sizeof(BITMAPINFO)+sizeof(RGBQUAD)*0x100),&bi=*pbi;
  bi.bmiHeader.biSize=sizeof(bi.bmiHeader);
  pal->copyto(bi.bmiColors,true);
  int w=dest->getw(),h=dest->geth();
  bi.bmiHeader.biWidth=w;bi.bmiHeader.biHeight=h;
  bi.bmiHeader.biPlanes=1;bi.bmiHeader.biBitCount=dest->bits();
  bi.bmiHeader.biCompression=BI_RGB;bi.bmiHeader.biSizeImage=0;
  bi.bmiHeader.biXPelsPerMeter=0;
  bi.bmiHeader.biYPelsPerMeter=0;
  bi.bmiHeader.biClrUsed=0;
  bi.bmiHeader.biClrImportant==0;
  GetDIBits(srcdc,srcbmp,0,recth,dest->buffer(),&bi,DIB_PAL_COLORS);
  free(pbi);
}
void readpalbmp(HDC srcdc,zbuf::Bitmap *dest,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth,gr::Palette *pal) {
  HBITMAP bmp=CreateCompatibleBitmap(srcdc,rectw,recth);
  HDC memdc=CreateCompatibleDC(srcdc);
  HBITMAP oldbmp=SelectObject(memdc,bmp);
  BitBlt(memdc,0,0,rectw,recth,srcdc,xsrc,ysrc,SRCCOPY);
  readrgbbmp(memdc,bmp,dest,xdest,ydest,xsrc,ysrc,rectw,recth);
  SelectObject(memdc,oldbmp);
  DeleteObject(bmp);
  DeleteDC(memdc);
}


void blitbmp(HDC dest,int w,int h,int bits,void *data) {
  BITMAPINFO bi;bi.bmiHeader.biSize=sizeof(bi.bmiHeader);
  bi.bmiHeader.biWidth=w;bi.bmiHeader.biHeight=h;
  bi.bmiHeader.biPlanes=1;bi.bmiHeader.biBitCount=bits;
  bi.bmiHeader.biCompression=BI_RGB;bi.bmiHeader.biSizeImage=0;
  bi.bmiHeader.biXPelsPerMeter=0;
  bi.bmiHeader.biYPelsPerMeter=0;
  bi.bmiHeader.biClrUsed=0;
  bi.bmiHeader.biClrImportant==0;

  SetDIBitsToDevice(dest,0,0,w,h,0,0,0,h,data,&bi,DIB_RGB_COLORS);
}

/*
void blitbmp(HDC dest,zbuf::ZBitmap *src) {
  blitbmp(dest,src->getw(),src->geth(),src->bits(),src->buffer());
}
*/
void blitbmp(HDC dest,zbuf::PixelMap *src) {
  blitbmp(dest,src->getw(),src->geth(),src->bits(),src->buffer());
}


}

//#delend [ABAZ]


//#pragma package(smart_init)
