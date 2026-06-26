#include <jlib.h>
#pragma hdrstop

#include "rasterfont.h"
//#include "bytebmp.cpp"
#include "zbufwrap.h"

namespace zbuf {

void RasterMask::draw(Bitmap *dest,int x0,int y0,int color) {
  int *act=(int*)map;
  register int actflag=1;
  register int flags=*act;

  for(int y=0;y<H;y++) for(int x=0;x<W;x++) {
    if (flags&actflag) dest->putpixel(x0+x,y0+y,color);
    actflag<<=1;
    if (!actflag) actflag=1,act++,flags=*act;
  }
}

void RasterMask::draw(ZBitmap *dest,int x0,int y0,float z,int color) {
  int *act=(int*)map;
  register int actflag=1;
  register int flags=*act;

  for(int y=0;y<H;y++) for(int x=0;x<W;x++) {
    if (flags&actflag) dest->putpixel(x0+x,y0+y,z,color);
    actflag<<=1;
    if (!actflag) actflag=1,act++,flags=*act;
  }
}

int RasterFont::text(ZBitmap *dest,int x0,int y0,float z,int color,const char *text) {
  int wi=0;
  for(const char *c=text;*c;c++) {
    if (types[uchar(*c)]) types[uchar(*c)]->draw(dest,x0+wi,y0,z,color),wi+=wis[uchar(*c)];
  }
  return wi;
}
int RasterFont::text(Bitmap *dest,int x0,int y0,int color,const char *text) {
  int wi=0;
  for(const char *c=text;*c;c++) {
    if (types[uchar(*c)]) types[uchar(*c)]->draw(dest,x0+wi,y0,color),wi+=wis[uchar(*c)];
  }
  return wi;
}
int RasterFont::textwi(const char *text) {
  int wi=0;
  for(const char *c=text;*c;c++) {
    if (types[uchar(*c)]) wi+=wis[uchar(*c)];
  }
  return wi;
}

void RasterMask::save(BINWRITE &fw) {
  fw<<W<<H<<size;
  fw.writedata(map,size);
}
RasterMask::RasterMask(BINREAD &fr) {
  fr>>W>>H>>size;
  map=malloc(size);
  fr.readdata(map,size);
}

RasterFont::RasterFont(BINREAD &fr) {
  fr>>hi;fr.readdata(wis,sizeof(wis));
  for(int i=0;i<0x100;i++) {
    bool ismap;fr>>ismap;
    if (ismap) types[i]=new RasterMask(fr);else types[i]=NULL;
  }
}
void RasterFont::save(BINWRITE &fw) {
  fw<<hi;fw.writedata(wis,sizeof(wis));
  for(int i=0;i<0x100;i++) {
    bool ismap=types[i];fw<<ismap;
    if (ismap) types[i]->save(fw);
  }
}

}

#pragma package(smart_init)
