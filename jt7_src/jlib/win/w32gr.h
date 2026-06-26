#ifndef w32grH
#define w32grH

#include <gr/comgr.h>
//#include <ext/resman.h>

namespace wlib {
using namespace jlib;
using namespace gr;


struct WStdGrObjs {
  HBRUSH clearbrush;
  WStdGrObjs();
  ~WStdGrObjs();
};

struct WDCRes {
  HANDLE saved,aloc;
  HDC dc;
  WDCRes(HDC dc):dc(dc) {saved=aloc=NULL;}
  void select(HANDLE obj) {
    HANDLE old=SelectObject(dc,obj);
    if (!saved) saved=old;
  }
  void selectnew(HANDLE obj) {
    if (aloc) {
      SelectObject(dc,saved);
      DeleteObject(aloc);
    }
    aloc=obj;
    select(obj);
  }
  ~WDCRes() {
    setdc(NULL);
  }
  void setdc(HDC adc) {
    if (saved) SelectObject(dc,saved);
    if (aloc) DeleteObject(aloc);
    saved=aloc=NULL;
    dc=adc;
  }
};

extern WStdGrObjs *stdobjs;

struct WinDC {
  WinDC(HDC dc=NULL);
  ~WinDC();
  void setpen(HANDLE obj) {pen.select(obj);}
  void setfont(HANDLE obj) {font.select(obj);}
  void setbrush(HANDLE obj) {brush.select(obj);}
  void updatepen() {
    pen.selectnew(CreatePen((pencolor==clnnone)?PS_NULL:PS_SOLID,penwi,pencolor));
  }
  void setpenwi(int wi) {
    if (wi==penwi) return;
    penwi=wi;
    updatepen();
  }
  void setpen(int color) {
    if (color==pencolor) return;
    pencolor=color;
    updatepen();
  }
  void setbrush(int color) {
    if (brushcolor==color) return;
    brushcolor=color;
    if (brushcolor==clnnone) brush.select(stdobjs->clearbrush);
    else brush.selectnew(CreateSolidBrush(brushcolor));
  }
  void line(int x1,int y1,int x2,int y2) {
    MoveToEx(dc,x1,y1,NULL);LineTo(dc,x2,y2);
  }
  void rect(int x1,int y1,int x2,int y2) {
    Rectangle(dc,x1,y1,x2,y2);
  }
  void ellipse(int x1,int y1,int x2,int y2) {
    Ellipse(dc,x1,y1,x2,y2);
  }
  void polygon(POINT *pt,int count) {
    Polygon(dc,pt,count);
  }
  void drawpixel(int x,int y,int color) {
    SetPixel(dc,x,y,color);
  }
  int getpixel(int x,int y) {
    return GetPixel(dc,x,y);
  }
  void floodfill(int x,int y,string mode,int color) {
    if (mode=="border") ExtFloodFill(dc,x,y,color,FLOODFILLBORDER);
    if (mode=="surface") ExtFloodFill(dc,x,y,color,FLOODFILLSURFACE);
  }
  int gettextwi(string text) {
    SIZE s;
    if (GetTextExtentPoint(dc,text.c_str(),text.length(),&s)) return s.cx;
    return -1;
  }
  int gettexthi(string text) {
    SIZE s;
    if (GetTextExtentPoint(dc,text.c_str(),text.length(),&s)) return s.cy;
    return -1;
  }
  void textout(int x,int y,string text);
  void cliptext(int x,int y,int x1,int y1,int x2,int y2,string text);
  HDC getdc() {return dc;}
  void setdc(HDC dc);
  void setfontcolor(int color) {SetTextColor(dc,color);}
private:
  WDCRes pen,brush,font;
  HDC dc;
  int pencolor,penwi,brushcolor;
};


}
#endif
