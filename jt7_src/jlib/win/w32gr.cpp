#include <winlib.h>
#pragma hdrstop

#include "w32gr.h"

namespace wlib {
using namespace jlib;

WStdGrObjs *stdobjs=NULL;

void WinDC::textout(int x,int y,string text) {
//  TextOut(dc,x,y,text.c_str(),text.length());
  int len=text.length();
  wchar_t *w=new wchar_t[len+1];
  MultiByteToWideChar(0,0,text.c_str(),len,w,len+1);
  TextOutW(dc,x,y,w,len);
  delete []w;
}

WinDC::WinDC(HDC dc):dc(dc),pen(dc),brush(dc),font(dc) {
  brushcolor=-1;
  pencolor=-1;
  penwi=-1;
}
WinDC::~WinDC() {
}

void WinDC::setdc(HDC adc) {
  pen.setdc(adc);brush.setdc(adc);font.setdc(adc);
  dc=adc;
}


void WinDC::cliptext(int x,int y,int x1,int y1,int x2,int y2,string text) {
  RECT r;r.left=x;r.top=y;r.right=x2;r.bottom=y2;
  DrawText(dc,text.c_str(),text.length(),&r,0);
}

WStdGrObjs::WStdGrObjs() {
  LOGBRUSH lb;
  lb.lbStyle=BS_NULL;
  lb.lbColor=clnnone;
  lb.lbHatch=0;
  clearbrush=CreateBrushIndirect(&lb);
}
WStdGrObjs::~WStdGrObjs() {
  DeleteObject(clearbrush);
}


}
#pragma package(smart_init)
