#include <jlib.h>
#pragma hdrstop

#include "awscolortable.h"

namespace aws {

ColorTable::ColorTable(Component *owner,System *system):PaintControl(owner,system) {
  hcnt=4;vcnt=2;
  xsqsize=1;ysqsize=1;
}

void ColorTable::setcolor(int row,int col,int color) {
  for(int i=0;i<colors.count;i++) if (colors[i].row==row && colors[i].col==col) {
    colors[i].color=color;
    repaint();
    return;
  }
  ctColorDef &cf=colors.add();
  cf.row=row;cf.col=col;cf.color=color;
  repaint();
}

int ColorTable::getcolor(int row,int col) {
  for(int i=0;i<colors.count;i++)
    if (colors[i].row==row && colors[i].col==col)
      return colors[i].color;
  return clnnone;
}

void ColorTable::onsize(int w,int h) {
  PaintControl::onsize(w,h);
  recount();
}

void ColorTable::recount() {
  Size s=getsize();
  xsqsize=(float)s.w/hcnt;
  ysqsize=(float)s.h/vcnt;
  repaint();
}

void ColorTable::loadprops(XMLNode xml,Form *form) {
  PaintControl::loadprops(xml,form);
  ev_select.load(xml,"onselect");
  hcnt=xml.ivar("cols");
  vcnt=xml.ivar("rows");
  if (hcnt<1) hcnt=1;
  if (vcnt<1) vcnt=1;
  recount();
}

void ColorTable::onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  PaintControl::onmousedown(x,y,button,shift,event);
  int lx=x/xsqsize,ly=y/ysqsize;
  MouseEvent ev;
  ev.type=et_mousedown;ev.name="onselect";ev.value=ev_select.get();ev.event=&event;
  ev.x=lx;ev.y=ly;ev.button=button;ev.shift=shift;
  do_event(ev);
}

void ColorTable::drawcrect(Canvas cnv,int x,int y,int w,int h,bool pushed,int color) {
  Rect r(x,y,w,h);
  cnv.drawframe(r,pushed);
  r.inflate(-3,-3);
  cnv.bg(color).fg(clnnone);
  cnv.rect(r);
}

void ColorTable::onpaint(Canvas &cnv,awsEvent &event) {
  PaintControl::onpaint(cnv,event);
  for(int x=0;x<hcnt;x++) {
    for(int y=0;y<vcnt;y++) {
      int clr=y*hcnt+x;
      drawcrect(cnv,xsqsize*x,ysqsize*y,xsqsize,ysqsize,false,getcolor(y,x));
    }
  }
}

void ColorTable::savedata(XMLNode xml) {
  PaintControl::savedata(xml);
  if (!getroot(xml).isdeep()) return;
  XMLNode d=xml.getat(getroot(xml));
  d.clear();
  savecolors(d);
}

void ColorTable::loaddata(XMLNode xml) {
  PaintControl::loaddata(xml);
  if (getroot(xml).isdeep()) {
    XMLNode d=xml.getat(getroot(xml));
    loadcolors(d);
  }
}

void ColorTable::savecolors(XMLNode xml) {
  xml.clearsubs();
  for(int i=0;i<colors.count;i++) {
    XMLNode c=xml.add("color");
    c.setvar("row",colors[i].row);
    c.setvar("col",colors[i].col);
    c.setvar("color",gr::nativecolortostr(colors[i].color));
  }
}
void ColorTable::loadcolors(XMLNode xml) {
  colors.clear();
  for(int i=0;i<xml.count();i++) {
    ctColorDef &c=colors.add();
    c.row=xml[i].ivar("row");
    c.col=xml[i].ivar("col");
    c.color=gr::strtonativecolor(xml[i].cvar("color"));
  }
  repaint();
}


}


