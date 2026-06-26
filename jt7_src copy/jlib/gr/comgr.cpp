#include <jlib.h>
#pragma hdrstop

#include "comgr.h"

namespace gr {
using namespace jlib;

STDCOLORS logiccolors;

void Palette::copyfrom(const void *src,bool swaprgb) {
  memcpy(colors,src,sizeof(RGBColor)*size);
  if (swaprgb) for(int i=0;i<size;i++) exchange(colors[i].r,colors[i].b);
}
void Palette::copyto(void *dest,bool swaprgb) {
  memcpy(dest,colors,sizeof(RGBColor)*size);
  if (swaprgb) {
    RGBColor *c=(RGBColor*)dest;
    for(int i=0;i<size;i++) exchange(c[i].r,c[i].b);
  }
}

int LOGICCOLOR::getnative() {
  switch (l) {
    case lcnative:return native;

    case lcRGB:return makergb(r,g,b);
    case lcnone:return clnnone;

    case lcwblack:return clnblack;
    case lcwmaroon:return clnmaroon;
    case lcwgreen:return clngreen;
    case lcwolive:return clnolive;
    case lcwnavy:return clnnavy;
    case lcwpurple:return clnpurple;
    case lcwteal:return clnteal;
    case lcwgray:return clngray;

    case lcwsilver:return clnsilver;
    case lcwred:return clnred;
    case lcwlime:return clnlime;
    case lcwyellow:return clnyellow;
    case lcwblue:return clnblue;
    case lcwfuchsia:return clnfuchsia;
    case lcwaqua:return clnaqua;
    case lcwwhite:return clnwhite;

  }
  if (l<logiccolors.count) {
    LOGICCOLOR rgb=logiccolors[l];
    return makergb(rgb.r,rgb.g,rgb.b);
  }
  return clnnone;
}

STDCOLORS::STDCOLORS() {
  add(LOGICCOLOR(0,0,0));
  add(LOGICCOLOR(0,0,0x80));
  add(LOGICCOLOR(0,0x80,0));
  add(LOGICCOLOR(0,0x80,0x80));
  add(LOGICCOLOR(0x80,0,0));
  add(LOGICCOLOR(0x80,0,0x80));
  add(LOGICCOLOR(0x80,0x80,0));
  add(LOGICCOLOR(0x80,0x80,0x80));

  add(LOGICCOLOR(0xC0,0xC0,0xC0));
  add(LOGICCOLOR(0,0,0xFF));
  add(LOGICCOLOR(0,0xFF,0));
  add(LOGICCOLOR(0,0xFF,0xFF));
  add(LOGICCOLOR(0xFF,0,0));
  add(LOGICCOLOR(0xFF,0,0xFF));
  add(LOGICCOLOR(0xFF,0xFF,0));
  add(LOGICCOLOR(0xFF,0xFF,0xFF));

  add(LOGICCOLOR(0,0,0));
}

#define _color_names\
  clr("Black",makergb(0,0,0))\
  clr("Maroon",makergb(0x80,0,0))\
  clr("Green",makergb(0,0x80,0))\
  clr("Olive",makergb(0x80,0x80,0))\
  clr("Navy",makergb(0,0,0x80))\
  clr("Purple",makergb(0x80,0,0x80))\
  clr("Teal",makergb(0,0x80,0x80))\
  clr("Gray",makergb(0x80,0x80,0x80))\
  clr("Silver",makergb(0xC0,0xC0,0xC0))\
  clr("Red",makergb(0xFF,0,0))\
  clr("Lime",makergb(0,0xFF,0))\
  clr("Yellow",makergb(0xFF,0xFF,0))\
  clr("Blue",makergb(0,0,0xFF))\
  clr("Fuchsia",makergb(0xFF,0,0xFF))\
  clr("Aqua",makergb(0,0xFF,0xFF))\
  clr("White",makergb(0xFF,0xFF,0xFF))\
  clr("Brown",makergb(0x80,0x40,0x00))\
  clr("Pond",makergb(0x40,0x80,0x00))\

#define unhex(_num) ( (_num>='0' and _num<='9')?(_num-'0'):toupper(_num)-'A'+10 )

int strtonativecolor(const char *name) {
  #define clr(_color,_colorref) if (upeqstr((char*)name,_color)) return _colorref;
  _color_names
  #undef clr
  if (*name=='#') return makergb(unhex(name[1])*16+unhex(name[2]),
  unhex(name[3])*16+unhex(name[4]),unhex(name[5])*16+unhex(name[6]));
  return clnnone;
}
void nativecolortostr(int color,char *res) {
  #define clr(_color,_colorref) if (_colorref==color) {strcpy(res,_color);return;}
  _color_names
  #undef clr
  *res='#';
  char p[]="0123456789ABCDEF";
  int r=nativervalue(color);
  res[1]=p[r/16];res[2]=p[r%16];
  int g=nativegvalue(color);
  res[3]=p[g/16];res[4]=p[g%16];
  int b=nativebvalue(color);
  res[5]=p[b/16];res[6]=p[b%16];
  res[7]=0;
}

string nativecolortostr(int color) {
  char res[0x100];
  nativecolortostr(color,res);
  return string(res);
}


}
//#pragma package(smart_init)


