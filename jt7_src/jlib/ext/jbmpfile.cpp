#include <jlib.h>
#pragma hdrstop

#include "jbmpfile.h"

namespace jlib {

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef long LONG;
typedef unsigned char BYTE;

struct RGBQUAD {
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
};

struct BITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
};

struct BITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
};


void BMPData::load(string file) {
  clear();
  BINRFILE fr(file.c_str());
  if (fr.eof) throw JError("File couldn't be open:"+file);

  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;

  //nelze nacist najednou kvuli zarovnani struktury v C++ 
  fr.rr(bfh.bfType).rr(bfh.bfSize).rr(bfh.bfReserved1).rr(bfh.bfReserved2).rr(bfh.bfOffBits);
  fr.rr(bih.biSize).rr(bih.biWidth).rr(bih.biHeight).rr(bih.biPlanes).rr(bih.biBitCount).rr(bih.biCompression).rr(bih.biSizeImage)
    .rr(bih.biXPelsPerMeter).rr(bih.biYPelsPerMeter).rr(bih.biClrUsed).rr(bih.biClrImportant);

  w=bih.biWidth;
  h=bih.biHeight;
  bits=bih.biBitCount;
  pallen=0;
  if (bits==1) pallen=2;
  if (bits==4) pallen=16;
  if (bits==8) pallen=256;
  if (bih.biClrUsed) pallen=bih.biClrUsed;
  if (pallen) palete=(bmpRGB*)malloc(pallen*sizeof(RGBQUAD));
  if (pallen) fr.readdata(palete,pallen*sizeof(RGBQUAD));
  for(int i=0;i<pallen;i++) exchange(palete[i].r,palete[i].b);
  data=fr.readtoeof();
  linelen=w*bits;
  while (linelen%32) linelen++;
  linelen/=8;
}

int BMPData::_getpixel(int x,int y) {
  unsigned char *start=(unsigned char *)data+(h-y-1)*linelen;
  switch (bits) {
    case 1:return (start[x/8]&(1<<(x%8)))?1:0;
    case 4:if (x%2) return start[x/2]&0xF;return start[x/2]>>4;
    case 8:return start[x];
    case 16:return ((short*)(start))[x];
    case 24:return (*(int*)(start+3*x))&0xFFFFFF;
    case 32:return ((int*)(start))[x];
  }
  return -1;
}

bmpRGB BMPData::_getcolor(int x,int y) {
  switch (bits) {
    case 1:return palete[_getpixel(x,y)];
    case 4:return palete[_getpixel(x,y)];
    case 8:return palete[_getpixel(x,y)];
    case 16:{
      int c16=_getpixel(x,y);
      return bmpRGB(((c16&(0x1F)<<5)>>5)<<3,((c16&(0x1F)<<10)>>10)<<3,(c16&0x1F)<<3,0);
    };
    case 24:case 32:{
      int c24=_getpixel(x,y);
//      return bmpRGB(c24&0xFF,(c24&0xFF00)>>8,(c24&0xFF0000)>>16,0);
      return bmpRGB((c24&0xFF0000)>>16,(c24&0xFF00)>>8,c24&0xFF,0);
    }
  }
  return bmpRGB();
}

void BMPData::fill32(int *colors,bool reversey) {
  int *act=colors;
  for(int y=0;y<h;y++) {
    int acty=y;
    if (reversey) acty=h-y-1;
    for(int x=0;x<w;x++,act++) {
      bmpRGB color=_getcolor(x,acty);
      *act=color.r | (color.g<<8) | (color.b<<16);
    }
  }
}


}

