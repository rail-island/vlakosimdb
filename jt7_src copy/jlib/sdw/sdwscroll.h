#ifndef sdwscrollH
#define sdwscrollH

#include "sdwcontrol.h"
#include "sdwpaint.h"
#include <aws/awspaint.h>

namespace sdw {
class ViewOpts;

struct sdwScrollControl:sdwControl {
  sdwScrollControl(Control *ctrl,sdwSystem *sys);
  void draw(sdwCanvas canvas);
  void childschanged();
  void setinsize(int aw,int ah) {inw=aw;inh=ah;updatedinsize();}
  void updatedinsize();
  void onsize();
  void mousedown(int x,int y,MouseButton but,ShiftState shift);
  void mouseup(int x,int y,MouseButton but,ShiftState shift);
  void mousemove(int x,int y,ShiftState shift);
  void updatedscrollpos();
  bool isclient(int x,int y);
  void setscrollpos(int x,int y) {if (ishor) dx=x;if (isver) dy=y;updatedscrollpos();} 

  float kx,ky;//>1
  int dx,dy;
  int inw,inh;
  int placeh,placew;
  int ishor,isver;//sirky soupatek:0|const
  int hscrsize,vscrsize;//delky soupatek:0|const
  int hscrlen,vscrlen;//delka, ktera je dostupna pro skrolovani
  int pleft,ptop;//pozice posouvatka
  int mouseorig;
  int dclientx,dclienty;
  int clientw,clienth;
  bool horscrolling,verscrolling;
  bool mouseinclient;
  ViewOpts *vo;
};

struct sdwScrollBox:sdwScrollControl {
  sdwScrollBox(ScrollBox *ctrl,sdwSystem *sys):sdwScrollControl(ctrl,sys) {}
};

class sdwScrolledPainter;
struct sdwScrolledPainterInner:sdwPaintControl {
  sdwScrolledPainterInner(sdwScrolledPainter *amain,sdwSystem *sys);
  sdwScrolledPainter *main;
  void dopaint(Canvas cnv);
};

struct sdwScrolledPainter:sdwScrollControl {
  sdwScrolledPainter(ScrolledPainter *ctrl,sdwSystem *sys);
  bool scrollinview(const Rect &r);
  Point getscrollpos() {return Point(dx,dy);}
  void setscrollpos(Point value) {sdwScrollControl::setscrollpos(value.x,value.y);}
  Size getinsize() {return inner->getsize();}//{return Size(inw,inh);}
  void setinsize(Size value) {inner->setpos(Point(0,0),value);childschanged();}// sdwScrollControl::setinsize(value.w,value.h);}
//  property_a(caretpos,Point)
//  property_a(caretsize,int)
  int getdoublebuffer() {return 1;}
  void setdoublebuffer(int value) {}


  sdwScrolledPainterInner *inner;
  ScrolledPainter *scrpaint;
};

}

#endif
