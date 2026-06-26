#ifndef comgrH
#define comgrH

namespace gr {
using namespace jlib;

struct RGBDiff {
  short dr,dg,db;
  RGBDiff(short dr,short dg,short db):dr(dr),dg(dg),db(db) {}
};

struct RGBColor {
  unsigned char r,g,b;
  unsigned char reserved;
  int toint() {return r|(g<<8)|(b<<16);}
  RGBColor(float fr,float fg,float fb) {r=255*fr;g=255*fg;b=255*fb;reserved=0;}
  RGBColor(int r,int g,int b):r(r),g(g),b(b) {reserved=0;}
  RGBColor() {reserved=0;}
  int torevint() {return b|(g<<8)|(r<<16);}
  void fromint(int color) {r=color&0xff;g=(color&0xff00)>>8;b=(color&0xff0000)>>16;}
  void fromrevint(int color) {b=color&0xff;g=(color&0xff00)>>8;r=(color&0xff0000)>>16;}
  void operator +=(RGBDiff rgb) {r+=rgb.dr;g+=rgb.dg;b+=rgb.db;}
  int toint16() {return (r>>3)|((g>>3)<<5)|((b>>3)<<10);}
  int torevint16() {return (b>>3)|((g>>3)<<5)|((r>>3)<<10);}
};
inline bool operator ==(const RGBColor &a,const RGBColor &b) {return a.r==b.r and a.g==b.g and a.b==b.b;}
inline bool operator !=(const RGBColor &a,const RGBColor &b) {return not (a==b);}


inline RGBDiff operator -(RGBColor a,RGBColor b) {return RGBDiff(a.r-b.r,a.g-b.g,a.b-b.b);}
inline RGBDiff operator /(RGBDiff a,int k) {return RGBDiff(a.dr/k,a.dg/k,a.db/k);}

struct ABREC {
  XYREC a,b;
  ABREC(int x,int y,int w,int h):a(x,y),b(x+w,y+h) {}
  ABREC(const XYREC &p1,const XYREC &p2):a(p1),b(p2) {}
  ABREC() {}
  int xsize() const {return abs(b.x-a.x);}
  int ysize() const {return abs(b.y-a.y);}
  XYREC topleft() const {return XYREC(min(a.x,b.x),min(a.y,b.y));}
  bool has(const XYREC &p) {return p.x>=a.x and p.y>=a.y and p.x<=b.x and p.y<=b.y;}
};

const unsigned char
  lcblack=0,lcblue=1,lcgreen=2,lccyan=3,lcred=4,lcmagenta=5,lcbrown=6,lcgray=7,
  lclblack=8,lclblue=9,lclgreen=10,lclcyan=11,lclred=12,lclmagenta=13,lclbrown=14,lclgray=15;
//synonyma
const unsigned char lcyellow=lclbrown,lcwhite=lclgray,lcpurple=lcmagenta,lclpurple=lclmagenta;
const unsigned char//Windows-nazevy barev
lcwblack=lcblack,lcwmaroon=lcred,lcwgreen=lcgreen,lcwolive=lcbrown,
lcwnavy=lcblue,lcwpurple=lcmagenta,lcwteal=lccyan,lcwgray=lcgray,
lcwsilver=lclblack,lcwred=lclred,lcwlime=lclgreen,lcwyellow=lcyellow,
lcwblue=lclblue,lcwfuchsia=lclmagenta,lcwaqua=lclcyan,lcwwhite=lcwhite;

const unsigned char lcRGB=0xFF,lcnone=0xFE,lcnative=0xFD;

//Nativni barvy
const int clnblack = 0x0,clnmaroon = 0x80,clngreen = 0x8000,clnolive = 0x8080,
clnnavy = 0x800000,clnpurple = 0x800080,clnteal = 0x808000,clngray = 0x808080,
clnsilver = 0xc0c0c0,clnred = 0xff,clnlime = 0xff00,clnyellow = 0xffff,
clnblue = 0xff0000,clnfuchsia = 0xff00ff,clnaqua = 0xffff00,clnltgray = 0xc0c0c0,
clndkgray = 0x808080,clnwhite = 0xffffff,clnnone = 0x1fffffff,clndefault = 0x20000000;
const int clnbrown=0x4080;
const int clnone16bit=0xFFFF;
const int clnone8bit=0xFF;

inline RGBColor rgb_black() {return RGBColor(0,0,0);}
inline RGBColor rgb_blue() {return RGBColor(0,0,0xFF);}
inline RGBColor rgb_navy() {return RGBColor(0,0,0x80);}
inline RGBColor rgb_green() {return RGBColor(0,0x80,0);}
inline RGBColor rgb_lime() {return RGBColor(0,0xFF,0);}
inline RGBColor rgb_dkgreen() {return RGBColor(0,0x80,0);}
inline RGBColor rgb_cyan() {return RGBColor(0,0xFF,0xFF);}
inline RGBColor rgb_aqua() {return RGBColor(0,0xFF,0xFF);}
inline RGBColor rgb_dkaqua() {return RGBColor(0,0x80,0x80);}
inline RGBColor rgb_purple() {return RGBColor(0x80,0,0x80);}
inline RGBColor rgb_lpurple() {return RGBColor(0xFF,0,0xFF);}
inline RGBColor rgb_red() {return RGBColor(0xFF,0,0);}
inline RGBColor rgb_dkred() {return RGBColor(0x80,0,0);}
inline RGBColor rgb_yellow() {return RGBColor(0xFF,0xFF,0);}
inline RGBColor rgb_white() {return RGBColor(0xFF,0xFF,0xFF);}
inline RGBColor rgb_gray() {return RGBColor(0xC0,0xC0,0xC0);}
inline RGBColor rgb_brown() {return RGBColor(0x80,0x40,0);}
inline RGBColor rgb_dkgray() {return RGBColor(0x80,0x80,0x80);}

inline int reversergb(int c) {
  if (c==clnnone) return clnnone;
  return ((c&0xff)<<16)|((c&0xff00))|((c&0xff0000)>>16);
}

inline int makergb(uchar r,uchar g,uchar b) {
  return r|(g<<8)|(b<<16);
}
inline int nativervalue(int color) {
  return color&0xff;
}
inline int nativegvalue(int color) {
  return (color&0xff00)>>8;
}
inline int nativebvalue(int color) {
  return (color&0xff0000)>>16;
}

struct Palette {
  Palette(int size) {
    this->size=size;
    colors=new RGBColor[size];
  }
  ~Palette() {delete []colors;}
  Palette *clone() {
    Palette *res=new Palette(size);
    res->copyfrom(colors,false);
    return res;
  }
  void copyfrom(const void *src,bool swaprgb);
  void copyto(void *dest,bool swaprgb);
  RGBColor &operator [] (int index) {return colors[index];}
  RGBColor *colors;
  int size;
  void fillrevint(int *buf) {
    for(int i=0;i<size;i++) buf[i]=colors[i].torevint();
  }
};


struct NATIVE_COLOR {};
struct LOGICCOLOR {
  unsigned char l,r,g,b;//l je logicka hodnota,pokud==0xFF,plati hotnoty RGB
  int native;
  LOGICCOLOR(uchar R,uchar G,uchar B) {l=lcRGB;r=R;g=G;b=B;}
  LOGICCOLOR(int logiccolor) {l=logiccolor;r=0;g=0;b=0;}
  LOGICCOLOR(int anative,NATIVE_COLOR) {l=lcnative;r=g=b=0;native=anative;}
  LOGICCOLOR() {l=lcnone;r=0;g=0;b=0;}
  int toint() {
    union {struct {unsigned char l,r,g,b;} logic;int res;};
    logic.l=l;logic.r=r;logic.g=g;logic.b=b;return res;
  }
  void fromint(int src) {
    union {struct {unsigned char l,r,g,b;} logic;int res;};
    res=src;
    l=logic.l;r=logic.r;g=logic.g;b=logic.b;
  }
  int getr();int getg();int getb();
  int getnative();//nativni reprezentace barvy
  bool isnone() {return l==lcnone or (l==lcnative and native==clnnone);}
};

inline int logictonative(LOGICCOLOR fg) {
  return reversergb(fg.getnative());
};

inline bool operator ==(LOGICCOLOR a,LOGICCOLOR b) {
  return a.toint()==b.toint();
}

struct STDCOLORS:TCntVec<LOGICCOLOR> {
  STDCOLORS();
};
extern STDCOLORS logiccolors;

inline int LOGICCOLOR::getr() {
  if (l<16) return logiccolors[l].r;
  if (l==lcRGB) return r;
  if (l==lcnative) return nativervalue(native);
  return -1;
}
inline int LOGICCOLOR::getg() {
  if (l<16) return logiccolors[l].g;
  if (l==lcRGB) return g;
  if (l==lcnative) return nativegvalue(native);
  return -1;
}
inline int LOGICCOLOR::getb() {
  if (l<16) return logiccolors[l].b;
  if (l==lcRGB) return b;
  if (l==lcnative) return nativebvalue(native);
  return -1;
}


/*Tyto fce se musi predefinovat:
  void _line(int x1,int y1,int x2,int y2) const ;
  void _rectangle(int x1,int y1,int x2,int y2) const ;
  void _fillrectangle(int x1,int y1,int x2,int y2) const ;
  void _drawrectangle(int x1,int y1,int x2,int y2) const ;
  void _ellipse(int x1,int y1,int x2,int y2) const ;
  void _drawellipse(int x1,int y1,int x2,int y2) const ;
  void _fillellipse(int x1,int y1,int x2,int y2) const ;
  void _setfgcolor(LOGICCOLOR color) const ;
  void _setbgcolor(LOGICCOLOR color) const ;
  void _polyline(const XYREC *points,int pointcount) const;
  void _polygon(const XYREC *points,int pointcount) const;
  void _drawpolygon(const XYREC *points,int pointcount) const;
  void _fillpolygon(const XYREC *points,int pointcount) const;
*/
/*Tyto se mohou predefinovat:
  int texthi(const char *text) const;
  int textwi(const char *text) const;

  void _textout(int x,int y,const char *text) const;
  void _setfonttype(FONT *fnt) const;
  void _setfontindex(int index) const;
  void _setfonttable(Array<FONT*> &fonts) const;

  DEVFONT *countdevfont(FONT *fnt) const {return new DEVFONT;}
  FONTSIZES *countfontsizes(FONT *fnt) const {throw ECANVAS_Operation_Not_Supported();}
*/

struct ECANVAS:JError {
  ECANVAS():JError("Canvas error") {}
};
struct ECANVAS_Operation_Not_Supported:public ECANVAS {};

inline BINREAD &operator >>(BINREAD &fr,LOGICCOLOR &clr) {return fr.rr(clr);}
inline BINWRITE &operator <<(BINWRITE &fw,LOGICCOLOR clr) {return fw.wr(clr);}

int strtonativecolor(const char *name);
void nativecolortostr(int color,char *res);
string nativecolortostr(int color);

}


#endif
