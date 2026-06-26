#include <jlib.h>
#pragma hdrstop

#include "awstree.h"

namespace aws {

Tree::Tree(Component *owner,System *system):ScrolledPainter(owner,system) {
  main=NULL;selected=NULL;
  itemhi=15;signw=10;signh=10;signspacing=5;
  wantresize=false;
}

void Tree::loadprops(XMLNode xml,Form *form) {
  ScrolledPainter::loadprops(xml,form);
  ev_nodeclick.load(xml,"onnodeclick");
}

NodeImage::NodeImage(TreeSource *asrc,Tree *atree,NodeImage *aparent) {
  tree=atree;src=asrc;wi=hi=0;packed=true;parent=aparent;
}

void Tree::setactive(TCntVec<int> &multiindex) {
  selected=indextonode(multiindex,true);
  repaint();
}

NodeImage *Tree::indextonode(TCntVec<int> &multiindex,bool expand) {
  NodeImage *act=main;
  for(int i=0;i<multiindex.count;i++) {
    int idx=multiindex[i];
    act->update();
    if (expand) act->expand();
    act=act->subs[idx];
  }
  return act;
}

void Tree::nodetoindex(NodeImage *act,TCntVec<int> &multiindex) {
  while (act!=main) {
    xassert(act);
    multiindex.add(act->parent->subs.search(act));
    act=act->parent;
  }
  multiindex.reverse();
}

bool Tree::getactive(TCntVec<int> &multiindex) {
  if (!selected) return false;
  nodetoindex(selected,multiindex);
  return true;
}

void NodeImage::countwi(Canvas &cnv) {
  counthi(cnv);
  wi=cnv.textwi(src->gettext());
  if (!packed) {
    int maxw=0;
    for(int i=0;i<subs.count;i++) if (subs[i]->getwi(cnv)>maxw) maxw=subs[i]->getwi(cnv);
    if (wi<maxw) wi=maxw;
  }
  wi+=tree->signw+tree->signspacing;
}

void NodeImage::counthi(Canvas &cnv) {
  hi=tree->itemhi;
  if (!packed) {
    if (src->getsubs()!=subs.count) updatesubs();
    for(int i=0;i<subs.count;i++) hi+=subs[i]->gethi(cnv);
  }
}

void NodeImage::drawsign(Canvas &cnv) {
  int w=tree->signw,h=tree->signh,itemhi=tree->itemhi,midy=itemhi/2,ybeg=(itemhi-h)/2,yend=(itemhi+h)/2;
  cnv.bg(clnnone).rect(0,ybeg,w,yend);
  if (src->getsubs()>0) {
    cnv.line(0,midy,w,midy);
    if (packed) cnv.line(w/2,ybeg,w/2,yend);
  }
}

void NodeImage::draw(Canvas cnv,int x,int y) {
  this->x=x;this->y=y;
  Rect clip=cnv.clip();
  gethi(cnv);
  if ((clip.a.y<0 && clip.b.y<0) || (clip.a.y>=hi) && clip.b.y>=hi) return;
  string text=src->gettext();
  int w=tree->signw,h=tree->signh,ite,itemhi=tree->itemhi;
  int dtext=w+tree->signspacing;
  if (tree->selected==this) cnv.bg(clnaqua).fg(clnnone).rect(dtext,0,dtext+cnv.textwi(text),itemhi).fg(clnblack);
  cnv.centertext(Rect(dtext,0,0,itemhi),text,false,true,false);
  drawsign(cnv);
  if (!packed) {
    int acty=tree->itemhi,lasty=0;
    for(int i=0;i<subs.count;i++) {
      subs[i]->draw(Canvas(cnv,dtext,acty),x+dtext,y+acty);
      cnv.line(w/2,acty+itemhi/2,dtext,acty+itemhi/2);
      lasty=acty;
      acty+=subs[i]->gethi(cnv);
    }
    if (subs.count>0) cnv.line(w/2,itemhi/2+h/2,w/2,lasty+itemhi/2);
  }
}

void NodeImage::updatesubs() {
  subs.clear();
  for(int i=0;i<src->getsubs();i++) subs.add(new NodeImage(src->nodeat(i),tree,this));
  hi=wi=0;
}

void Tree::setsource(TreeSource *src) {
  delete main;
  main=new NodeImage(src,this,NULL);
  selected=main;
  wantresize=true;
//  setinsize(Size(100,main->gethi()));
  repaint();
}

void NodeImage::update() {
  if (subs.count!=src->getsubs()) updatesubs();
  for(int i=0;i<subs.count;i++) subs[i]->update();
/*  else {
    for(int i=0;i<subs.count;i++) if (!subs[i]->src->equalto(
  }*/
}

void Tree::update() {
//  TCntVec<int> active;
//  bool isa=getactive(active);
  main->update();
//  if (isa) setactive(active);
  repaint();
}

void Tree::onpaint(Canvas &cnv,awsEvent &event) {
  ScrolledPainter::onpaint(cnv,event);
  Rect clip=cnv.clip();clip.inflate(1,1);
  cnv.fg(clnnone).bg(clnltgray).rect(clip).fg(clnblack);
  if (wantresize && main) setinsize(Size(main->getwi(cnv)+signw+signspacing,main->gethi(cnv))),wantresize=false;
  if (main) main->draw(cnv,0,0);
}

void NodeImage::changed() {
  hi=wi=0;
  tree->wantresize=true;
  tree->repaint();
  if (parent) parent->changed();
}

void NodeImage::expand() {
  if (!packed) return;
  packed=false;
  changed();
  tree->scrollinview(Rect(x,y,wi,hi));
}
void NodeImage::collapse() {
  if (packed) return;
  packed=true;
  changed();
}


bool NodeImage::mevent(int x,int y,WHAT what) {
  if (x>=0 && y>=0 && y<tree->itemhi) {tree->selected=this;tree->repaint();tree->donodeclick(this);}
  if (x>=0 && x<tree->signw && y>=(tree->itemhi-tree->signh)/2 && y<(tree->itemhi+tree->signh)/2) {
    if (what==leftdown) {if (packed) expand();else collapse();return true;}
  }
  if (!packed) {
    int acty=tree->itemhi;
    for(int i=0;i<subs.count;i++) {
      if (subs[i]->mevent(x-tree->signw-tree->signspacing,y-acty,what)) return true;
      acty+=subs[i]->hi;
    }
  }
  return false;
}

NodeImage *NodeImage::getlastopened() {
  if (packed || subs.count==0) return this;
  return subs.last()->getlastopened();
}
NodeImage *NodeImage::getprev() {
  if (parent) {
    int idx=parent->subs.search(this);
    if (idx>0) return parent->subs[idx-1]->getlastopened();
    return parent;
  } else return this;
}
NodeImage *NodeImage::getnext(bool canchild) {
  if (canchild && !packed && subs.count>0) return subs[0];
  if (parent) {
    int idx=parent->subs.search(this);
    if (idx<parent->subs.count-1) return parent->subs[idx+1];
    NodeImage *res=parent->getnext(false);
    if (res) return res;
    if (canchild) return this;
    return NULL;
  }
  if (canchild) return this;
  return NULL;
}
Rect NodeImage::getsmallrect() {
  return Rect(x,y,wi,tree->itemhi);
}

void Tree::onkeydown(awsKey key,awsEvent &event) {
  ScrolledPainter::onkeydown(key,event);
  if (selected) {
    if (key==kt_up) {
      NodeImage *prev=selected->getprev();
      scrollinview(prev->getsmallrect());
      repaint(selected->getsmallrect());
      repaint(prev->getsmallrect());
//      selected->soft_repaint();
//      prev->soft_repaint();
      selected=prev;
      donodeclick(selected);
    } else if (key==kt_down) {
      NodeImage *next=selected->getnext();
      scrollinview(next->getsmallrect());
      repaint(selected->getsmallrect());
      repaint(next->getsmallrect());
//      next->soft_repaint();
//      selected->soft_repaint();
      selected=next;
      donodeclick(selected);
    } else if (key==kt_pgup) {
      NodeImage *node0=selected;
      NodeImage *last;int pghi=getsize().h;
      do {
        last=selected;
        selected=selected->getprev();
        if (node0->y-selected->y>=pghi) selected=last;
      } while (last!=selected);
      scrollinview(selected->getsmallrect());
//      repaint();
      repaint(selected->getsmallrect());
      repaint(node0->getsmallrect());
      donodeclick(selected);
    } else if (key==kt_pgdn) {
      NodeImage *node0=selected;
      NodeImage *last;int pghi=getsize().h;
      do {
        last=selected;
        selected=selected->getnext();
        if (selected->y-node0->y>=pghi) selected=last;
      } while (last!=selected);
      scrollinview(selected->getsmallrect());
      repaint(selected->getsmallrect());
      repaint(node0->getsmallrect());
      donodeclick(selected);
//      repaint();
    }
  }
}

void Tree::onchar(char ch,awsEvent &event) {
  ScrolledPainter::onchar(ch,event);
  if (selected) {
    if (ch=='+') {
       selected->expand();
    } else if (ch=='-') {
      selected->collapse();
    }
  }
}

void Tree::onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  ScrolledPainter::onmousedown(x,y,button,shift,event);
  setfocus();
  if (main) {
    if (button==0) main->mevent(x,y,NodeImage::leftdown);
    if (button==1) main->mevent(x,y,NodeImage::rightdown);
  }
}
void Tree::onmouseup(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  ScrolledPainter::onmouseup(x,y,button,shift,event);
}
void Tree::onmousemove(int x,int y,ShiftState shift,awsEvent &event) {
  ScrolledPainter::onmousemove(x,y,shift,event);
}


}


