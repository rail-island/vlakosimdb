#include <jlib.h>
#pragma hdrstop

#include "awspaint.h"
#include "awscontrol.h"
#include "awssystem.h"
#include "awsreal.h"
#include "awslayout.h"
#include "awsform.h"

namespace aws {

object_d(PaintControl,ParentControl)
property_d(PaintControl,doublebuffer,int)
property_d(PaintControl,erasebkgnd,bool)
object_d(ScrolledPainter,ChildControl)
property_d(ScrolledPainter,scrollpos,Point)
property_d(ScrolledPainter,insize,Size)
property_d(ScrolledPainter,doublebuffer,int)
property_d(ScrolledPainter,caretpos,Point)
property_d(ScrolledPainter,caretsize,int)


void PaintControl::onpaint(Canvas &cnv,awsEvent &event) {
  PaintEvent ev;
  ev.type=et_paint;ev.name="onpaint";ev.value=ev_paint.get();ev.event=&event;
  ev.canvas=&cnv;
  do_event(ev);
}
void PaintControl::loadprops(XMLNode xml,Form *form) {
  ParentControl::loadprops(xml,form);
  ev_paint.load(xml,"onpaint");
  if (xml.isvar("doublebuffer")) setdoublebuffer(xml.ivar("doublebuffer"));
  if (xml.validvar("erasebkgnd")) seterasebkgnd(xml.ivar("erasebkgnd"));
}

void ScrolledPainter::onpaint(Canvas &cnv,awsEvent &event) {
  PaintEvent ev;
  ev.type=et_paint;ev.name="onpaint";ev.value=ev_paint.get();ev.event=&event;
  ev.canvas=&cnv;
  do_event(ev);
}
void ScrolledPainter::loadprops(XMLNode xml,Form *form) {
  ChildControl::loadprops(xml,form);
  ev_paint.load(xml,"onpaint");
  if (xml.isvar("doublebuffer")) setdoublebuffer(xml.ivar("doublebuffer"));
//  if (xml.validvar("erasebkgnd")) seterasebkgnd(xml.ivar("erasebkgnd"));
}
bool ScrolledPainter::scrollinview(const Rect &r) {
  return getreal()->scrollinview(r);
}
void ScrolledPainter::repaint(const Rect &r) {getreal()->repaint(r);}
void ScrolledPainter::repaint() {getreal()->repaint();}

void ScrolledPainter::onmousewheel(int x,int y,int delta,ShiftState shift,awsEvent &event) {
  ChildControl::onmousewheel(x,y,delta,shift,event);
  Point pos=getscrollpos();pos.y-=delta;
  setscrollpos(pos);
}

void PaintControl::repaint(const Rect &r) {getreal()->repaint(r);}
void PaintControl::repaint() {getreal()->repaint();}

void Canvas::centertext(const Rect &r,string text,bool centerx,bool centery,bool clip) {
  int x1=r.a.x,y1=r.a.y,x2=r.b.x,y2=r.b.y;
  int w=textwi(text),h=texthi(text);
  makeless(x1,x2);makeless(y1,y2);
  int rw=x2-x1,rh=y2-y1;
  if (clip) cnv->cliptext(dx+x1+centerx*(rw-w)/2,dy+y1+centery*(rh-h)/2,dx+r.a.x,dy+r.a.y,dx+r.b.x,dy+r.b.y,text);
  else cnv->drawtext(dx+x1+centerx*(rw-w)/2,dy+y1+centery*(rh-h)/2,text);
}

/*
void Picture::blit(Canvas cnv) {

}
void Picture::bitblt(Canvas cnv,Rect src,STRING rop,int rop_arg=0) {
}
void Picture::stretchblt(Canvas cnv,Rect dest,Rect src,STRING rop,int rop_arg=0) {
}
bool Picture::trydraw(RealCanvas *cnv,int x,int y,STRING rop,int rop_arg) {
  if (dodraw(cnv,x,y,rop,rop_arg)) return true;
  if (cnv->drawpicture(x,y,this,rop,rop_arg)) return true;
  return false;
}
bool Picture::trystretch(RealCanvas *cnv,int x,int y,STRING rop,int rop_arg) {
  if (dostretch(cnv,x,y,rop,rop_arg)) return true;
  if (cnv->drawpicture(x,y,this,rop,rop_arg)) return true;
  return false;
}
*/


void init_aws_paint_rtti() {
}

}


