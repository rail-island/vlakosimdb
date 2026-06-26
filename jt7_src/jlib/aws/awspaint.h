#ifndef awspaintH
#define awspaintH

#include "awscontrol.h"
#include "awsgr.h"

namespace aws {
using namespace gr;
class RealPaintControl;class RealScrolledPainter;


struct Picture:RCObject {
  virtual Canvas getcanvas()=0;
  virtual void releasecanvas()=0;
//  virtual void releasecanvas(Canvas &cnv)=0;
  virtual bool dodraw(BltDrawOpts &o,RealCanvas *cnv) {return false;}
  virtual bool dostretch(StretchDrawOpts &o,RealCanvas *cnv) {return false;}
//  virtual void transparentblit(Canvas cnv,int transparentcolor)=0;
  virtual int getw()=0;
  virtual void setw(int w)=0;
  virtual int geth()=0;
  virtual void seth(int h)=0;
  virtual int getbits()=0;//bits<=0: nespecifikovano (podle obrazovky)
  virtual void setbits(int bits)=0;
  virtual void load(string file)=0;
  virtual void save(string file)=0;
  virtual void hflip()=0;
  virtual void vflip()=0;
  virtual void transp()=0;
  virtual void invert()=0;

  void blit(Canvas cnv) {cnv.picture(0,0,this);}
  void bitblt(Canvas cnv,const Rect &src,STRING rop,int rop_arg=0) {cnv.blt(0,0,src,this,rop,rop_arg);}
  void stretchblt(Canvas cnv,const Rect &dest,const Rect &src,STRING rop,int rop_arg=0)
    {cnv.stretch(dest,src,this,rop,rop_arg);}
//  bool trydraw(RealCanvas *cnv,int x,int y,STRING rop,int rop_arg);
//  bool trystretch(RealCanvas *cnv,int x,int y,STRING rop,int rop_arg);
};

struct Paintable {
  virtual void onpaint(Canvas &cnv,awsEvent &event)=0;
};

struct PaintControl:ParentControl,Paintable {
  virtual void onpaint(Canvas &cnv,awsEvent &event);
  void repaint(const Rect &r);
  void repaint();
  object_i(PaintControl)
  void loadprops(XMLNode xml,Form *form);
  property_i(doublebuffer,int)
  property_i(erasebkgnd,bool)
public:
  EventValue ev_paint;
};

struct ScrolledPainter:ChildControl,Paintable {
  virtual void onpaint(Canvas &cnv,awsEvent &event);
  void repaint(const Rect &r);
  void repaint();
  object_i(ScrolledPainter)
  void loadprops(XMLNode xml,Form *form);
  bool scrollinview(const Rect &r);

  EventValue ev_vscroll,ev_hscroll;
  property_i(scrollpos,Point)
  property_i(insize,Size)
  property_i(doublebuffer,int)
  property_i(caretpos,Point)
  property_i(caretsize,int) //<0:caret hidden
  virtual void dovscroll() {donotify("onvscroll",ev_vscroll.get());}
  virtual void dohscroll() {donotify("onhscroll",ev_hscroll.get());}
  void onmousewheel(int x,int y,int delta,ShiftState shift,awsEvent &event);
public:
  EventValue ev_paint;
};

/*
//#delbeg [ABAZ]
struct awsCanvas:CANVAS {
  awsCanvas(Canvas *cnv):cnv(cnv) {fg=clnblack;bg=clnwhite;}
  Canvas *cnv;
  mutable int fg,bg;
protected:
  void _line(int x1,int y1,int x2,int y2) const {cnv->fg(fg).line(x1,y1,x2,y2);}
  void _rectangle(int x1,int y1,int x2,int y2) const {cnv->fg(fg).bg(bg).rect(x1,y1,x2,y2);}
  void _fillrectangle(int x1,int y1,int x2,int y2) const {cnv->fg(clnnone).bg(bg).rect(x1,y1,x2,y2);}
  void _drawrectangle(int x1,int y1,int x2,int y2) const {cnv->fg(fg).bg(clnnone).rect(x1,y1,x2,y2);}
  void _ellipse(int x1,int y1,int x2,int y2) const {cnv->fg(fg).bg(bg).ellipse(x1,y1,x2,y2);}
  void _drawellipse(int x1,int y1,int x2,int y2) const {cnv->fg(clnnone).bg(bg).ellipse(x1,y1,x2,y2);}
  void _fillellipse(int x1,int y1,int x2,int y2) const {cnv->fg(fg).bg(bg).ellipse(x1,y1,x2,y2);}
  void _setfgcolor(LOGICCOLOR color) const {fg=color.getnative();}
  void _setbgcolor(LOGICCOLOR color) const {bg=color.getnative();}
  void _textout(int x,int y,const char *text) const {cnv->text(x,y,text);}
  int _texthi(const char *text) const {return cnv->texthi(text);}
  int _textwi(const char *text) const {return cnv->textwi(text);}
};
//#delend [ABAZ]
*/

void init_aws_paint_rtti();

}

#endif
