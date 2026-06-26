#ifndef comcnvH
#define comcnvH

#include "comgr.h"
#include "fonts.h"

namespace gr {

const int line_solid=0,line_dash=1,line_dot=2,line_dashdot=3;

class CANVAS {
  void __setresfont(const FONT &f) const {int index;void *handle=fontresource->findres(f,&index);_setresfont(index,handle);}
protected:
  //_tvar nakresli tvar s obvodem vyplneny,_filltvar jen vyplni,_drawtvar jen nakresli obrys
  virtual void _line(int x1,int y1,int x2,int y2) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _rectangle(int x1,int y1,int x2,int y2) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _fillrectangle(int x1,int y1,int x2,int y2) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _drawrectangle(int x1,int y1,int x2,int y2) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _ellipse(int x1,int y1,int x2,int y2) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _drawellipse(int x1,int y1,int x2,int y2) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _fillellipse(int x1,int y1,int x2,int y2) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _setfgcolor(LOGICCOLOR color) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _setbgcolor(LOGICCOLOR color) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _polyline(const XYREC *points,int pointcount) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _polygon(const XYREC *points,int pointcount) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _drawpolygon(const XYREC *points,int pointcount) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _fillpolygon(const XYREC *points,int pointcount) const {throw ECANVAS_Operation_Not_Supported();}

  virtual void _pencopy() const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _penxor() const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _penxnor() const {throw ECANVAS_Operation_Not_Supported();}

  virtual void _linestyle(int style) const {throw ECANVAS_Operation_Not_Supported();}

  virtual void _textout(int x,int y,const char *text) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _vartextout(int x,int y,int index) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _setfonttype(const FONT &fnt) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _setfontindex(int index) const {throw ECANVAS_Operation_Not_Supported();}
  virtual void _setresfont(int index,void *handle) const {throw ECANVAS_Operation_Not_Supported();}

  virtual int _texthi(const char *text) const {throw ECANVAS_Operation_Not_Supported();}
  virtual int _textwi(const char *text) const {throw ECANVAS_Operation_Not_Supported();}
public:
  const CANVAS &line(const XYREC &a,const XYREC &b) const {_line(a.x,a.y,b.x,b.y);return *this;}

  const CANVAS &line(int x1,int y1,int x2,int y2) const {_line(x1,y1,x2,y2);return *this;}
  const CANVAS &Line(int x,int y,int dx,int dy) const {_line(x,y,x+dx,y+dy);return *this;}
  const CANVAS &linehor(int y,int x1,int x2) const {_line(x1,y,x2,y);return *this;}
  const CANVAS &linever(int x,int y1,int y2) const {_line(x,y1,x,y2);return *this;}
  const CANVAS &Linehor(int y,int x,int dx) const {_line(x,y,x+dx,y);return *this;}
  const CANVAS &Linever(int x,int y,int dy) const {_line(x,y,x,y+dy);return *this;}

  const CANVAS &rect(int x1,int y1,int x2,int y2) const {_rectangle(x1,y1,x2,y2);return *this;}
  const CANVAS &frect(int x1,int y1,int x2,int y2) const {_fillrectangle(x1,y1,x2,y2);return *this;}
  const CANVAS &drect(int x1,int y1,int x2,int y2) const {_drawrectangle(x1,y1,x2,y2);return *this;}

  const CANVAS &Rect(int x,int y,int w,int h) const {_rectangle(x,y,x+w,y+h);return *this;}
  const CANVAS &fRect(int x,int y,int w,int h) const {_fillrectangle(x,y,x+w,y+h);return *this;}
  const CANVAS &dRect(int x,int y,int w,int h) const {_drawrectangle(x,y,x+w,y+h);return *this;}

  const CANVAS &ell(int x1,int y1,int x2,int y2) const {_ellipse(x1,y1,x2,y2);return *this;}
  const CANVAS &dell(int x1,int y1,int x2,int y2) const {_drawellipse(x1,y1,x2,y2);return *this;}
  const CANVAS &fell(int x1,int y1,int x2,int y2) const {_fillellipse(x1,y1,x2,y2);return *this;}

  const CANVAS &Ell(int x,int y,int w,int h) const {_ellipse(x,y,x+w,y+h);return *this;}
  const CANVAS &dEll(int x,int y,int w,int h) const {_drawellipse(x,y,x+w,y+h);return *this;}
  const CANVAS &fEll(int x,int y,int w,int h) const {_fillellipse(x,y,x+w,y+h);return *this;}

  const CANVAS &fg(LOGICCOLOR color) const {_setfgcolor(color);return *this;}
  const CANVAS &bg(LOGICCOLOR color) const {_setbgcolor(color);return *this;}

  const CANVAS &cross(int x,int y,int size2) const {return extcross(x,y,size2,size2);}
  const CANVAS &extcross(int x,int y,int sizehor,int sizever) const {linehor(y,x-sizehor,x+sizehor);linever(x,y-sizever,y+sizever);return *this;}
  const CANVAS &cross(int x1,int y1,int x2,int y2) const {linehor((y1+y2)/2,x1,x2);linever((x1+x2)/2,y1,y2);return *this;}

  const CANVAS &polyline(const XYREC *points,int pointcount) const {_polyline(points,pointcount);return *this;}

  const CANVAS &polygon(const XYREC *points,int pointcount) const {_polygon(points,pointcount);return *this;}
  const CANVAS &dpolygon(const XYREC *points,int pointcount) const {_drawpolygon(points,pointcount);return *this;}
  const CANVAS &fpolygon(const XYREC *points,int pointcount) const {_fillpolygon(points,pointcount);return *this;}

  const CANVAS &pencopy() const {_pencopy();return *this;}
  const CANVAS &penxor() const {_penxor();return *this;}
  const CANVAS &penxnor() const {_penxnor();return *this;}

  const CANVAS &linestyle(int style) const {_linestyle(style);return *this;}

  const CANVAS &font(int index) const {fontsizes=NULL;_setfontindex(index);return *this;}
  const CANVAS &resfont(int index,void *handle) const {fontsizes=NULL;_setresfont(index,handle);return *this;}
  const CANVAS &font(const FONT &fnt) const {fontsizes=NULL;if (fontresource) __setresfont(fnt);else _setfonttype(fnt);return *this;}

  const CANVAS &text(int x,int y,const char *text) const {_textout(x,y,text);return *this;}
  const CANVAS &vartext(int x,int y,int index) const {if (variables) _textout(x,y,variables->item(index));else _vartextout(x,y,index);return *this;}

  int textwi(const char *text) const {if (fontsizes) return fontsizes->textwi(text);return _textwi(text);}
  int texthi(const char *text) const {if (fontsizes) return fontsizes->hi;return _texthi(text);}

  CANVAS() {fontsizes=NULL;variables=NULL;fontresource=NULL;}
  mutable FONTSIZES *fontsizes;
  mutable STRLIST *variables;
  mutable FONTRESOURCE *fontresource;
  //kdyz neni NULL,tak pri kazdem volani font(const FONT &) se zavola _resfontindex,
  //pokud font jeste neni ve fontresources, prida se tam
};

class NULLCANVAS:public CANVAS {
protected:
  void _line(int x1,int y1,int x2,int y2) const {}
  void _rectangle(int x1,int y1,int x2,int y2) const {}
  void _fillrectangle(int x1,int y1,int x2,int y2) const {}
  void _drawrectangle(int x1,int y1,int x2,int y2) const {}
  void _ellipse(int x1,int y1,int x2,int y2) const {}
  void _drawellipse(int x1,int y1,int x2,int y2) const {}
  void _fillellipse(int x1,int y1,int x2,int y2) const {}
  void _setfgcolor(LOGICCOLOR color) const {}
  void _setbgcolor(LOGICCOLOR color) const {}
  void _polyline(const XYREC *points,int pointcount) const {}
  void _polygon(const XYREC *points,int pointcount) const {}
  void _drawpolygon(const XYREC *points,int pointcount) const {}
  void _fillpolygon(const XYREC *points,int pointcount) const {}
  void _pencopy() const {}
  void _penxor() const {}
  void _penxnor() const {}
  void _linestyle(int style) const {}
  void _textout(int x,int y,const char *text) const {};
  void _setfonttype(const FONT &fnt) const {}
  void _setfontindex(int index) const {}
  void _resfontindex(int index) const {}
};

struct LOGCANVAS:CANVAS {//hlavne pro ladici ucely,vypisuje akce,ktere provadi
  STROUT *s;
  const char *space,*linesep;
  void _line(int x1,int y1,int x2,int y2) const {*s<<"line"<<space<<x1<<space<<y1<<space<<x2<<space<<y2<<linesep;}
  void _rectangle(int x1,int y1,int x2,int y2) const {*s<<"rectangle"<<space<<x1<<space<<y1<<space<<x2<<space<<y2<<linesep;}
  void _fillrectangle(int x1,int y1,int x2,int y2) const {*s<<"fillrectangle"<<space<<x1<<space<<y1<<space<<x2<<space<<y2<<linesep;}
  void _drawrectangle(int x1,int y1,int x2,int y2) const {*s<<"drawrectangle"<<space<<x1<<space<<y1<<space<<x2<<space<<y2<<linesep;}
  void _ellipse(int x1,int y1,int x2,int y2) const {*s<<"ellipse"<<space<<x1<<space<<y1<<space<<x2<<space<<y2<<linesep;}
  void _drawellipse(int x1,int y1,int x2,int y2) const {*s<<"drawellipse"<<space<<x1<<space<<y1<<space<<x2<<space<<y2<<linesep;}
  void _fillellipse(int x1,int y1,int x2,int y2) const {*s<<"fillellipse"<<space<<x1<<space<<y1<<space<<x2<<space<<y2<<linesep;}
  void _setfgcolor(LOGICCOLOR color) const {*s<<"fgcolor"<<space<<color.l<<linesep;}
  void _setbgcolor(LOGICCOLOR color) const {*s<<"bgcolor"<<space<<color.l<<linesep;}
  void _polyline(const XYREC *points,int pointcount) const {*s<<"polyline"<<linesep;}
  void _polygon(const XYREC *points,int pointcount) const {*s<<"polygon"<<linesep;}
  void _drawpolygon(const XYREC *points,int pointcount) const {*s<<"drawpolygon"<<linesep;}
  void _fillpolygon(const XYREC *points,int pointcount) const {*s<<"fillpolygon"<<linesep;}
  void _textout(int x,int y,const char *text) const {*s<<"textout"<<space<<x<<space<<y<<space<<text<<linesep;}
  void _setfonttype(const FONT &fnt) const {*s<<"setfonttype"<<space<<fnt.name<<space<<fnt.style<<space<<fnt.height<<space<<fnt.width<<linesep;}
  void _setfontindex(int index) const {*s<<"setfontindex"<<space<<index<<linesep;}
  void _setresfont(int index,void *handle) const {*s<<"resfontindex"<<space<<index<<space<<int(handle)<<linesep;}
  void _linestyle(int style) const {*s<<"linestyle"<<space<<style<<linesep;}
  int texthi(const char *text) const {return 1;}
  int textwi(const char *text) const {return 1;}
  LOGCANVAS(STROUT *out) {linesep="\n";space=" ";s=out;}
};


extern NULLCANVAS nullcnv;

}

//---------------------------------------------------------------------------
#endif
