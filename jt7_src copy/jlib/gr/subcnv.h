#ifndef subcnvH
#define subcnvH

#include "comgr.h"
#include "comcnv.h"

namespace gr {

class COMSUBCANVAS:public CANVAS {//implementuje metody SUBCANVASU
protected:
  XYREC _ltop(const XYREC &p) const {return XYREC(p.x*kx+x0,p.y*ky+y0);}

  const CANVAS *parent;
  double invkx,invky;
  int x0,y0;
  double kx,ky;
  COMSUBCANVAS() {}

  void _line(int x1,int y1,int x2,int y2) const {parent->line(x1*kx+x0,y1*ky+y0,x2*kx+x0,y2*ky+y0);};
  void _rectangle(int x1,int y1,int x2,int y2) const  {parent->rect(x1*kx+x0,y1*ky+y0,x2*kx+x0,y2*ky+y0);}
  void _fillrectangle(int x1,int y1,int x2,int y2) const  {parent->frect(x1*kx+x0,y1*ky+y0,x2*kx+x0,y2*ky+y0);}
  void _drawrectangle(int x1,int y1,int x2,int y2) const {parent->drect(x1*kx+x0,y1*ky+y0,x2*kx+x0,y2*ky+y0);}
  void _ellipse(int x1,int y1,int x2,int y2) const {parent->ell(x1*kx+x0,y1*ky+y0,x2*kx+x0,y2*ky+y0);}
  void _drawellipse(int x1,int y1,int x2,int y2) const {parent->dell(x1*kx+x0,y1*ky+y0,x2*kx+x0,y2*ky+y0);}
  void _fillellipse(int x1,int y1,int x2,int y2) const {parent->fell(x1*kx+x0,y1*ky+y0,x2*kx+x0,y2*ky+y0);}
  void _setfgcolor(LOGICCOLOR color) const {parent->fg(color);}
  void _setbgcolor(LOGICCOLOR color) const {parent->bg(color);}

  void _polyline(const XYREC *points,int pointcount) const;
  void _polygon(const XYREC *points,int pointcount) const;
  void _drawpolygon(const XYREC *points,int pointcount) const;
  void _fillpolygon(const XYREC *points,int pointcount) const;
  void _textout(int x,int y,const char *text) const {parent->text(x*kx+x0,y*ky+y0,text);}
  void _vartextout(int x,int y,int index) const {parent->vartext(x*kx+x0,y*ky+y0,index);}
  void _setfontindex(int index) const {parent->font(index);}
  void _setfonttype(const FONT &fnt) const {parent->font(FONT(fnt.name,fnt.style,fnt.color,fnt.height*ky,fnt.width*kx));}
  void _setresfont(int index,void *handle) const {parent->resfont(index,handle);}

  void _linestyle(int style) const {parent->linestyle(style);}

  virtual void _pencopy() const {parent->pencopy();}
  virtual void _penxor() const {parent->penxor();}
  virtual void _penxnor() const {parent->penxnor();}
public:

  const CANVAS &getparent() const {return *parent;}

  int _texthi(const char *text) const {return parent->texthi(text)*invky;}
  int _textwi(const char *text) const {return parent->textwi(text)*invkx;}

};

struct SUBCANVAS:COMSUBCANVAS {
  SUBCANVAS(const CANVAS &cnv,int ax0,int ay0,double akx=1,double aky=1);
};

struct FIXSUBCANVAS:COMSUBCANVAS {//ma promenlive hodnoty x0,y0,kx,ky
  //tato trida umoznuje meneni x0,y0,kx,ky behem zivota objektu
  //(ale pokud je parent opet SUBCANVAS,vykreslovani je pak pomalejsi,
  //pokud neni,pak ma stejnou fci jako SUBCANVAS)
  void fromrect(const ABREC &src,const ABREC &dest);
  //upravi kx,ky,x0,y0 tak, aby se obdelnik src zobrazil na obdelnik dest na rodicovskem platne

  void eqfromrect(const ABREC &src,const ABREC &dest);
  //upravi kx,ky,x0,y0 tak, aby se obdelnik src zobrazil na obdelnik dest na rodicovskem platne
  //kx se bude rovnat ky,src bude vystredovano v dest

  int xltop(int x) const {return x*kx+x0;}
  int yltop(int y) const {return y*ky+y0;}

  int xptol(int x) const {return (x-x0)*invkx;}
  int yptol(int y) const {return (y-y0)*invky;}

  XYREC ltop(const XYREC p) const {return _ltop(p);}

  FIXSUBCANVAS(const CANVAS &cnv,int ax0,int ay0,double akx=1,double aky=1);

  void set0(int x,int y) {x0=x;y0=y;}
  void setkoef(double k) {kx=k;ky=k;}
  double getkoef() {return (kx+ky)/2;}
  int getx0() {return x0;}
  int gety0() {return y0;}
};

}

#endif
