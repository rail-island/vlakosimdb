#include <winlib.h>
#pragma hdrstop

#include "awwfont.h"
#include "awwsystem.h"
#include "awwpaint.h"

namespace aww {

WinFont::WinFont(WinFontMan *man,const LOGFONT &font) {
  handle=NULL;
  own=true;
  manager=man;
  manager->add(this);
  lf=font;
  hashcode=fonthash(lf);
//  memcpy(&lf,&font,sizeof(font));
}

WinFont::WinFont(WinFontMan *man,HFONT ahandle) {
  handle=ahandle;
  own=false;
//  own=aown;
//  if (!handle) handle=GetStockObject(ANSI_VAR_FONT);
  update();
  manager=man;
  manager->add(this);
}
WinFont::~WinFont() {
  manager->erased(this);
  if (own) DeleteObject(handle);
}

awwFont::awwFont(WinFont *wf,awwSystem *system):wf(wf),system(system) {
  pixelsperinch=system->logpixelsy;color=0;
}

void awwFont::setwf(LOGFONT &f) {
  wf=system->fontman.search(f,xorhash(&f,sizeof(f)));
  if (!wf) wf=new WinFont(&system->fontman,f);
}

bool awwFont::edit() {
  CHOOSEFONT cf;
  LOGFONT lf;
  lf=wf->lf;
//  memcpy(&lf,&wf->lf,sizeof(LOGFONT));
//  GetObject(wf->handle,sizeof(LOGFONT),&lf);
  zerorec(cf);
  cf.lStructSize=sizeof(cf);
  cf.lpLogFont=&lf;
  cf.rgbColors=getcolor();
  cf.Flags=CF_INITTOLOGFONTSTRUCT|CF_BOTH|CF_EFFECTS;
  if (ChooseFont(&cf)) {
    setwf(lf);
    setcolor(cf.rgbColors);
    return true;
  }
  return false;
}

int WinFont::getheight() {
  TEXTMETRIC metric;
  HDC dc=GetDC(0);
  HFONT oldfont=SelectObject(dc,handle);
  GetTextMetrics(dc, &metric);
  SelectObject(dc,oldfont);
  ReleaseDC(0,dc);
  return metric.tmHeight;
}

int WinFont::getwidth(string text) {
  TEXTMETRIC metric;
  HDC dc=GetDC(0);
  HFONT oldfont=SelectObject(dc,handle);
  SIZE size;
  GetTextExtentPoint(dc,text.c_str(),text.length(),&size);
  SelectObject(dc,oldfont);
  ReleaseDC(0,dc);
  return size.cx;
}

int WinFont::getheight(RealCanvas *cnv) {
  if (awwCanvas *wc=dynamic_cast<awwCanvas*>(cnv->getasker())) {
    TEXTMETRIC metric;
    HFONT oldfont=SelectObject(wc->wdc.getdc(),handle);
    GetTextMetrics(wc->wdc.getdc(), &metric);
    SelectObject(wc->wdc.getdc(),oldfont);
    return metric.tmHeight;
  }
  return 0;
}
int WinFont::getwidth(RealCanvas *cnv,string text) {
  if (awwCanvas *wc=dynamic_cast<awwCanvas*>(cnv->getasker())) {
    HFONT oldfont=SelectObject(wc->wdc.getdc(),handle);
    SIZE size;
    GetTextExtentPoint(wc->wdc.getdc(),text.c_str(),text.length(),&size);
    SelectObject(wc->wdc.getdc(),oldfont);
    return size.cx;
  }
  return 0;
}


}

