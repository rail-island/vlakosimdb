#include <jlib.h>
#pragma hdrstop

#include "awslayout.h"

namespace aws {

void Layout1::work() {
  Rect r=owner->getlayoutrect();
  TCntVec<Control*> childs;
  owner->getchilds(childs);
  if (childs.count>0) childs[0]->setpos(r.a.x,r.a.y,r.b.x,r.b.y);
}

void GridLayout::work() {
  if (cols<0 or rows<0) return;
  Rect r=owner->getlayoutrect();
  Size s=r.size();
  int xs=s.w/cols,ys=s.h/rows;
  TCntVec<Control*> childs;
  owner->getchilds(childs);
  int act=0;
  for(int x=0;x<cols;x++)
    for(int y=0;y<rows;y++,act++) {
      if (act>=childs.count) return;
      childs[act]->setpos(r.left()+x*xs,r.top()+y*ys,xs,ys);
    }
}

void HFlowLayout::loadprops(XMLNode xml) {
  Layout::loadprops(xml);
  hcenter=xml.ivar(gethcenter());
  vcenter=xml.ivar(getvcenter());
  left=xml.ivar(getlefts());
  right=xml.ivar(getrights());
  top=xml.ivar(gettops());
  bottom=xml.ivar(getbottoms());
}

void HFlowLayout::wrap() {
//  actx=getx(r.a);maxy=basey=maxy;
  lines.add(actline);
  actline=new Line();
}

void HFlowLayout::loadchild(XMLNode xml,Control *ctrl) {

}

bool HFlowLayout::Line::addctrl(Control *ctrl,HFlowLayout *layout) {
  int ischild=childs.count>0?1:0;
  Size cs=ctrl->getsize();
  if (linew+layout->hjump*ischild+layout->getw(cs)>layout->maxw) return false;
  linew+=layout->hjump*ischild+layout->getw(cs);
  childs.add(ctrl);
  if (layout->geth(cs)>lineh) lineh=layout->geth(cs);
  return true;
}

void HFlowLayout::Line::work(HFlowLayout *layout,int x0) {
  if (childs.count==0) return;
  int startx=0,dx=0;
  if (layout->hcenter) {
    int mezwi=(layout->maxw-linew)/childs.count;
    startx=mezwi/2,dx=mezwi;
  }
  int actx=startx+layout->left;
  for(int i=0;i<childs.count;i++) {
    Size cs=childs[i]->getsize();
    layout->setpos(childs[i],actx+x0,layout->acty);
    actx+=layout->getw(cs)+dx;
  }
}

void HFlowLayout::setposes() {
  if (lines.count==0) return;
  int starty=0,dy=0;
  int hsum=0;
  for(int i=0;i<lines.count;i++) hsum+=lines[i]->lineh;
  if (vcenter) {
    int mezhi=(geth(layrec.size())-top-bottom-hsum)/lines.count;
    starty=mezhi/2;dy=mezhi;
  }
  acty=starty+top;
  acty+=gety(layrec.a);
  for(int i=0;i<lines.count;i++) {
    Line *l=lines[i];
    l->work(this,getx(layrec.a));
    acty+=dy;
    acty+=l->lineh;
  }
}

void HFlowLayout::work() {
  layrec=owner->getlayoutrect();
  maxw=getw(layrec.size())-left-right;
  TCntVec<Control*> childs;
  actline=new Line();
  owner->getchilds(childs);
  for(int i=0;i<childs.count;i++) {
    if (!actline->addctrl(childs[i],this)) wrap(),actline->addctrl(childs[i],this);
  }
  if (actline->childs.count>0) lines.add(actline);else delete actline;
  setposes();
  lines.clear();
}


void VListLayout::_work(int size) {
  if (size<0) return;
  int sizesum=0,allcnt=0;
  for(int j=0;j<items.count;j++) {
    Item &i=items[j];
    if (i.ctrl->getvisible()) {
      if (!i.all) {
        if (i.rel) i.realsize=i.size*size/100;
        else i.realsize=i.size;
        sizesum+=i.realsize;
      } else i.realsize=0,allcnt++;
    } else i.realsize=0;
  }
  int dsize=size-sizesum;
  for(int j=0;j<items.count;j++) {
    Item &i=items[j];
    if (i.all && i.ctrl->getvisible()) i.realsize=dsize/allcnt;
  }
  int realsum=0;
  for(int i=0;i<items.count;i++) realsum+=items[i].realsize;
  if (realsum>size) {
    float d=(float)size/(float)realsum;
    for(int i=0;i<items.count;i++) items[i].realsize*=d;
  }
}

void VListLayout::loadchild(XMLNode xml,Control *ctrl) {
  string size=xml.svar(sizename);
  if (ctrl) {
    Item &i=items.add();
    i.ctrl=ctrl;
    i.size=0;
    i.rel=false;
    i.all=false;
    if (size=="" or size=="*") i.all=true;
    else {
      if (size.ends("%")) i.rel=true;
      i.size=atoi(size.c_str());
      if (size=="fixed") i.size=xml.ivar(gethletter());
    }
  }
}

void VListLayout::work() {
  Rect r=owner->getlayoutrect();
  Size s=r.size();
  _work(geth(s));
  int act=gety(r.a);
  for(int i=0;i<items.count;i++) {
    if (items[i].ctrl) {
      setpos(items[i].ctrl,getx(r.a),act,getw(s),items[i].realsize);
      act+=items[i].realsize;
    }
  }
}

void AutoLayout::work() {
  Rect r=owner->getlayoutrect();

  const int placeleft=20,placetop=20,placedx=30,placedy=30;
  const int maxhi=r.size().h;

  TCntVec<Control*> childs;
  owner->getchilds(childs);
  for(int i=0;i<childs.count;i++) childs[i]->autolayout();

  int left=placeleft;
  int acty=placetop;
  int actw=0;
  int reachedh=0;
  int w=0;

  for(int i=0;i<childs.count;i++) {
    Size cs=childs[i]->getsize();
    if (acty+cs.h>maxhi) {
      left+=actw+placedx;
      w+=actw+placedx;
      actw=0;
      if (acty>reachedh) reachedh=acty;acty=placetop;
    }
    childs[i]->setpos(left,acty);
    acty+=cs.h;acty+=placedy;
    if (cs.w>actw) actw=cs.w;
  }
  if (acty>reachedh) reachedh=acty;
  w+=actw+placedx;
  int h=reachedh;
}


}


