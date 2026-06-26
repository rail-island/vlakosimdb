#include <jlib.h>
#pragma hdrstop

#include "metacnv.h"
#include <com/strms.h>

namespace gr {
using namespace jlib;

struct P {
  int x,y;
  P(int X,int Y) {x=X;y=Y;}
  P() {}
};
struct R {
  int x1,y1,x2,y2;
  R(int X1,int Y1,int X2,int Y2) {x1=X1;y1=Y1;x2=X2;y2=Y2;}
  R() {}
};

void METACANVAS::assign(METACANVAS &src) {
  data.rewrite();src.data.savedata(data);
}

void METACANVAS::_setfonttype(const FONT &fnt) const {
  data<<SETFONTTYPE<<fnt;
  if (tcnv) tcnv->font(fnt);
}
void METACANVAS::_setfontindex(int index) const {
  data<<SETFONTINDEX<<index;
  if (tcnv) tcnv->font(index);
}
void METACANVAS::_setresfont(int index,void *handle) const {
  data<<SETRESFONTINDEX<<index;
  if (tcnv) tcnv->resfont(index,handle);
}
void METACANVAS::_linestyle(int style) const {
  data<<SETLINESTYLE<<style;
}
void METACANVAS::_line(int x1,int y1,int x2,int y2) const {
  data<<LINE;data.wr(R(x1,y1,x2,y2));
}
void METACANVAS::_rectangle(int x1,int y1,int x2,int y2)  const {
  data<<RECTANGLE;data.wr(R(x1,y1,x2,y2));
}
void METACANVAS::_fillrectangle(int x1,int y1,int x2,int y2) const {
  data<<FILLRECTANGLE;data.wr(R(x1,y1,x2,y2));
}
void METACANVAS::_drawrectangle(int x1,int y1,int x2,int y2) const {
  data<<DRAWRECTANGLE;data.wr(R(x1,y1,x2,y2));
}
void METACANVAS::_ellipse(int x1,int y1,int x2,int y2) const {
  data<<ELLIPSE;data.wr(R(x1,y1,x2,y2));
}
void METACANVAS::_drawellipse(int x1,int y1,int x2,int y2) const {
  data<<DRAWELLIPSE;data.wr(R(x1,y1,x2,y2));
}
void METACANVAS::_fillellipse(int x1,int y1,int x2,int y2) const {
  data<<FILLELLIPSE;data.wr(R(x1,y1,x2,y2));
}
void METACANVAS::_setfgcolor(LOGICCOLOR color) const {
  data<<SETFGCOLOR<<color;
}
void METACANVAS::_setbgcolor(LOGICCOLOR color) const {
  data<<SETBGCOLOR<<color;
}
void METACANVAS::_poly(const XYREC *points,int pointcount,int shape) const {
  data<<shape<<pointcount;
  data.writedata(points,sizeof(XYREC)*pointcount);
}
void METACANVAS::_textout(int x,int y,const char *text) const {
  data<<DRAWTEXT;data.wr(P(x,y))<<text;
}
void METACANVAS::_vartextout(int x,int y,int index) const {
  data<<DRAWVARTEXT;data.wr(P(x,y))<<index;
}

void METACANVAS::save(BINWRITE &fw) {
  data.save(fw);
}
void METACANVAS::load(BINREAD &fr) {
  data.load(fr);
}
void METACANVAS::clear() {
  data.rewrite();
}
#define polyfunc(func) \
{int cnt=data.readi();XYREC *pts=new XYREC[cnt];data.readdata(pts,sizeof(XYREC)*cnt);\
 func(pts,cnt);delete []pts;}

void METACANVAS::draw(const CANVAS &cnv) {
  SimpleExceptType(XMetaCanvas_Operation_Not_Known,XError);
  data.reset();
  string s;
  while (not data.eof) {
    int shape=data.readi();
    int i;
    LOGICCOLOR logic;
    FONT fnt;
    R r;
    P p;
    switch (shape) {
      case LINE:data.rr(r);cnv.line(r.x1,r.y1,r.x2,r.y2);break;
      case RECTANGLE:data.rr(r);cnv.rect(r.x1,r.y1,r.x2,r.y2);break;
      case FILLRECTANGLE:data.rr(r);cnv.frect(r.x1,r.y1,r.x2,r.y2);break;
      case DRAWRECTANGLE:data.rr(r);cnv.drect(r.x1,r.y1,r.x2,r.y2);break;
      case ELLIPSE:data.rr(r);cnv.ell(r.x1,r.y1,r.x2,r.y2);break;
      case DRAWELLIPSE:data.rr(r);cnv.dell(r.x1,r.y1,r.x2,r.y2);break;
      case FILLELLIPSE:data.rr(r);cnv.fell(r.x1,r.y1,r.x2,r.y2);break;
      case SETFGCOLOR:data>>logic;cnv.fg(logic);break;
      case SETBGCOLOR:data>>logic;cnv.bg(logic);break;
      case POLYLINE:polyfunc(polyline);break;
      case DPOLYGON:polyfunc(dpolygon);break;
      case FPOLYGON:polyfunc(fpolygon);break;
      case POLYGON:polyfunc(polygon);break;
      case DRAWTEXT:{
        data.rr(p)>>s;
        cnv.text(p.x,p.y,s.c_str());
      } break;
      case DRAWVARTEXT:{data.rr(p)>>i;if (metavariables) cnv.text(p.x,p.y,metavariables->item(i));else cnv.vartext(p.x,p.y,i);} break;
      case SETFONTINDEX:data>>i;cnv.font(i);break;
      case SETFONTTYPE:data>>fnt;cnv.font(fnt);break;
      case SETLINESTYLE:data>>i;cnv.linestyle(i);break;
      case SETRESFONTINDEX:data>>i;cnv.resfont(i,NULL);break;
      default:xthrowmsg(XMetaCanvas_Operation_Not_Known,("Cislo:%d",shape));
    }
  }
}

#undef polyfunc

}
//#pragma package(smart_init)
