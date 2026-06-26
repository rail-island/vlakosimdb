#ifndef metacnvH
#define metacnvH

#include "comgr.h"
#include "comcnv.h"
#include <com/strms.h>
#include <com/bms.h>

namespace gr {
using namespace jlib;


class METACANVAS:public CANVAS {
  static const int LINE=1,RECTANGLE=2,FILLRECTANGLE=3,DRAWRECTANGLE=4,
    ELLIPSE=5,DRAWELLIPSE=6,SETFGCOLOR=7,SETBGCOLOR=8,FILLELLIPSE=9,
    POLYLINE=10,POLYGON=11,DPOLYGON=12,FPOLYGON=13,DRAWTEXT=14,SETFONTINDEX=15,
    SETFONTTYPE=16,DRAWVARTEXT=18,SETLINESTYLE=19,SETRESFONTINDEX=20;
//trida,ktera si pamatuje, co na ni bylo kresleno,
//pak to dokaze reprodukovat
  mutable MEMORYSTREAM data;
//  mutable BLOCKMEMORYSTREAM data;
  void loadfont(const CANVAS &cnv);
  void loadfonttable(const CANVAS &cnv);
protected:
  const CANVAS *tcnv;

  void _vartextout(int x,int y,int index) const;
  void _line(int x1,int y1,int x2,int y2) const;
  void _rectangle(int x1,int y1,int x2,int y2) const;
  void _fillrectangle(int x1,int y1,int x2,int y2) const;
  void _drawrectangle(int x1,int y1,int x2,int y2) const;
  void _ellipse(int x1,int y1,int x2,int y2) const;
  void _drawellipse(int x1,int y1,int x2,int y2) const;
  void _fillellipse(int x1,int y1,int x2,int y2) const;
  void _setfgcolor(LOGICCOLOR color) const;
  void _setbgcolor(LOGICCOLOR color) const;
  void _polyline(const XYREC *points,int pointcount) const {_poly(points,pointcount,POLYLINE);}
  void _polygon(const XYREC *points,int pointcount) const {_poly(points,pointcount,POLYGON);}
  void _drawpolygon(const XYREC *points,int pointcount) const {_poly(points,pointcount,DPOLYGON);}
  void _fillpolygon(const XYREC *points,int pointcount) const {_poly(points,pointcount,FPOLYGON);}
  void _textout(int x,int y,const char *text) const;
  void _linestyle(int style) const;

  void _setfontindex(int index) const;
  void _setresfont(int index,void *handle) const;
  void _setfonttype(const FONT &fnt) const;

  int _texthi(const char *text) const {xassert(tcnv);return tcnv->texthi(text);}
  int _textwi(const char *text) const {xassert(tcnv);return tcnv->textwi(text);}

  void _poly(const XYREC *points,int pointcount,int shape) const;
public:
  void log(const char *file) {MEMSTROUT mso;LOGCANVAS log(&mso);draw(log);mso.save(file);}
  void save(BINWRITE &fw);
  void load(BINREAD &fr);
  void clear();
  void draw(const CANVAS &cnv);
  void save(const char *file) {BINWFILE fw(file);save(fw);}
  void load(const char *file) {BINRFILE fr(file);load(fr);}
  void assign(METACANVAS &src);
  METACANVAS(const CANVAS *targetcanvas=NULL) {tcnv=targetcanvas;metavariables=NULL;}//tcnv slouzi k volani funkci texthi,textwi
  //vyuziva se jen pri zapisu
  ~METACANVAS() {}
  void settargetcnv(const CANVAS *atarget) {tcnv=atarget;}
  //predem neznamy text,index je index do metavariables

  STRLIST *metavariables;
};


}
#endif
