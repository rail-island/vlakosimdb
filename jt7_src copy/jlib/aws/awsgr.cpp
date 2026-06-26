#include <jlib.h>
#pragma hdrstop

#include "awsgr.h"
#include "awspaint.h"
#include <typeinfo>

namespace aws {

Canvas &Canvas::polygon(const Point *pts,int count) {
  Point *newpts=new Point[count];
  for(int i=0;i<count;i++) newpts[i]=pts[i]+Point(dx,dy);
  cnv->drawpolygon(newpts,count);
  delete []newpts;
  return *this;
}
Canvas &Canvas::picture(int x,int y,Picture *picture) {
  cnv->drawpicture(x+dx,y+dy,Rect(Point(0,0),Size(picture->getw(),picture->geth())),picture,"copy",0);
  return *this;
}
Canvas &Canvas::picture(const Rect &r,Picture *picture) {
  cnv->stretchpicture(r+Point(dx,dy),Rect(Point(0,0),Size(picture->getw(),picture->geth())),picture,"copy",0);
  return *this;
}
Canvas &Canvas::blt(int x,int y,const Rect &src,Picture *picture,STRING rop,int rop_arg) {
  cnv->drawpicture(x+dx,y+dy,src,picture,rop,rop_arg);
  return *this;
}
Canvas &Canvas::stretch(const Rect &dest,const Rect &src,Picture *picture,STRING rop,int rop_arg) {
  cnv->stretchpicture(dest+Point(dx,dy),src,picture,rop,rop_arg);
  return *this;
}
static void initrandomcolor() {
  srand(123);
}
static int randomcolor() {
  int r=rand()%0x100,g=rand()%0x100,b=rand()%0x100;
  return gr::makergb(r,b,g);
}
void Canvas::drawselection(Rect r) {
  r+=Point(dx,dy);
  for(int x=r.a.x;x<=r.b.x;x++) {
    for(int y=r.a.y;y<=r.b.y;y++) {
      if (x==r.a.x || x==r.b.x || y==r.a.y || y==r.b.y) {
        pixel(x,y,randomcolor());
      }
    }
  }
}
void Canvas::randomline(int x1,int y1,int x2,int y2) {
  initrandomcolor();
  int dx=x2-x1,dy=y2-y1;
  if (dx || dy) {
    if (abs(dx)>abs(dy)) {
      if (x1>x2) exchange(x1,x2),exchange(y1,y2),dx=-dx,dy=-dy;
      double k=(double)dy/(double)dx;
      for(int x=x1;x<=x2;x++) pixel(x,y1+k*(x-x1),randomcolor());
    } else {
      if (y1>y2) exchange(x1,x2),exchange(y1,y2),dx=-dx,dy=-dy;
      double k=(double)dx/(double)dy;
      for(int y=y1;y<=y2;y++) pixel(x1+k*(y-y1),y,randomcolor());
    }
  }
}



//  void drawpicture(int x,int y,const Rect &src,Picture *picture,STRING rop,int rop_arg);
//  void stretchpicture(const Rect &dest,const Rect &src,Picture *picture,STRING rop,int rop_arg);

void RealCanvas::drawpicture(int x,int y,const Rect &src,Picture *picture,STRING rop,int rop_arg) {
  BltDrawOpts o(src,Point(x,y),rop,rop_arg);
  if (!trydrawpicture(o,picture))
    throw JError(fmtstr("Cannot draw %s on %s",typeid(*picture).name(),typeid(*this).name()));
}
void RealCanvas::stretchpicture(const Rect &dest,const Rect &src,Picture *picture,STRING rop,int rop_arg) {
  StretchDrawOpts o(src,dest,rop,rop_arg);
  if (!trystretchpicture(o,picture))
    throw JError(fmtstr("Cannot draw %s on %s",typeid(*picture).name(),typeid(*this).name()));
}

bool RealCanvas::trydrawpicture(BltDrawOpts &o,Picture *picture) {
  if (dodrawpicture(o,picture)) return true;
  if (picture->dodraw(o,this)) return true;
  return false;
}
bool RealCanvas::trystretchpicture(StretchDrawOpts &o,Picture *picture) {
  if (dostretchpicture(o,picture)) return true;
  if (picture->dostretch(o,this)) return true;
  return false;
}


bool ResizeCanvas::dodrawpicture(BltDrawOpts &o,Picture *picture) {
  Size s=Size(picture->getw(),picture->geth());
  Rect r=Rect(o.dest,s);
  StretchDrawOpts o2(o.src,convrect(r),o.rop,o.rop_arg);
  return cnv->trystretchpicture(o2,picture);
}

void ResizeCanvas::drawpolygon(const Point *pts,int count) {
  Point *newpts=new Point[count];
  for(int i=0;i<count;i++) newpts[i]=pts[i],newpts[i].x*=kx,newpts[i].y*=ky;
  cnv->drawpolygon(newpts,count);
  delete []newpts;
}

bool ResizeCanvas::dostretchpicture(StretchDrawOpts &o,Picture *picture) {
  StretchDrawOpts o2(o.src,convrect(o.dest),o.rop,o.rop_arg);
  return cnv->trystretchpicture(o2,picture);
}

void RedirectCanvas::setfont(Font *afont) {
  ask->setfont(afont);
//  draw.ref->~ProxyObject<RealCanvas>();
//  RealCanvas *cc=draw.ref->obj;
//  cc->setfont(afont);
  draw->setfont(afont);
}

void Font::save(XMLNode xml) {
  xml.setvar("name",getname());
  xml.setvar("size",getsize());
  xml.setvar("bold",getbold());
  xml.setvar("underline",getunderline());
  xml.setvar("italic",getitalic());
  xml.setvar("strikeout",getstrikeout());
  xml.setvar("color",gr::nativecolortostr(getcolor()));
}
void Font::load(XMLNode xml) {
  setname(xml.svar("name"));
  setsize(xml.ivar("size"));
  setbold(xml.ivar("bold"));
  setunderline(xml.ivar("underline"));
  setitalic(xml.ivar("italic"));
  setstrikeout(xml.ivar("strikeout"));
  string color=xml.svar("color");
  if (isdigit(color[0])) setcolor(color.ivalue());
  else if (color=="") setcolor(0);
  else setcolor(gr::strtonativecolor(color.c_str()));
}


void RealCanvas::nextband(Canvas &cnv,int hi,string type) {
  cnv.dy+=hi;
}

void MetaCanvas::replacetext(string oldtext,string newtext) {
  for(int i=0;i<items.count;i++) if (TextItem *ti=dynamic_cast<TextItem*>(items[i])) {
    if (ti->text==oldtext) ti->text=newtext;
  }
}


}

