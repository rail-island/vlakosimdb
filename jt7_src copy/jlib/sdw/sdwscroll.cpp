#include <jlib.h>
#pragma hdrstop

#include "sdwscroll.h"
#include "sdwsystem.h"

namespace sdw {

sdwScrollControl::sdwScrollControl(Control *ctrl,sdwSystem *sys):sdwControl(ctrl,sys) {
  dx=dy=0;vo=sys->vo;inw=inh=0;w=h=100;
  ishor=isver=0;horscrolling=verscrolling=false;
  dclientx=vo->scrollboxborder;
  dclienty=vo->scrollboxborder;
  mouseinclient=false;
}

bool sdwScrollControl::isclient(int x,int y) {
  return x>=vo->scrollboxborder && x<w-isver-vo->scrollboxborder
      && y>=vo->scrollboxborder && y<h-ishor-vo->scrollboxborder;
}

void sdwScrollControl::mousedown(int x,int y,MouseButton but,ShiftState shift) {
  if (isclient(x,y)) {
    sdwControl::mousedown(x-dclientx+dx,y-dclienty+dy,but,shift);
    mouseinclient=true;
  }
  if (ishor && y>h-ishor) {
    if (y>h-ishor && x>=pleft && x<pleft+hscrlen) {
      mouseorig=x;horscrolling=true;
    }
  } else if (isver && x>w-isver) {
    if (x>w-isver && y>=ptop && y<ptop+vscrlen) {
      mouseorig=y;verscrolling=true;
    }
  }
}
void sdwScrollControl::mouseup(int x,int y,MouseButton but,ShiftState shift) {
  if (mouseinclient) {
    sdwControl::mouseup(x-dclientx+dx,y-dclienty+dy,but,shift);
    mouseinclient=false;
  }
  horscrolling=false;
  verscrolling=false;
}
void sdwScrollControl::mousemove(int x,int y,ShiftState shift) {
  if (mouseinclient || isclient(x,y)) {
    sdwControl::mousemove(x-dclientx+dx,y-dclienty+dy,shift);
  }
  if (horscrolling) {
    dx+=(kx*(x-mouseorig));
    mouseorig=x;
    updatedscrollpos();
    repaint();
  }
  if (verscrolling) {
    dy+=(ky*(y-mouseorig));
    mouseorig=y;
    updatedscrollpos();
    repaint();
  }
}

void sdwScrollControl::updatedinsize() {
  if (inw+2*vo->scrollboxborder+vo->scrollbarsize>w) ishor=vo->scrollbarsize;else ishor=0;
  if (inh+2*vo->scrollboxborder+vo->scrollbarsize>h) isver=vo->scrollbarsize;else isver=0;
  clientw=w-isver-2*vo->scrollboxborder;
  clienth=h-ishor-2*vo->scrollboxborder;
  if (ishor) {
    hscrlen=clientw-2*vo->scrollbarborder;
    kx=(float)inw/(float)clientw;
    hscrsize=hscrlen/kx;
  }
  if (isver) {
    vscrlen=clienth-2*vo->scrollbarborder;
    ky=(float)inh/(float)clienth;
    vscrsize=vscrlen/ky;
  }
  updatedscrollpos();
}

void sdwScrollControl::onsize() {
  sdwControl::onsize();
  placew=w-2*vo->scrollbarborder-2*vo->scrollboxborder;
  placeh=h-2*vo->scrollbarborder-2*vo->scrollboxborder;
  updatedinsize();
}

void sdwScrollControl::childschanged() {
  inw=0,inh=0;
  for(int i=0;i<childs.ctrls.count;i++) {
    sdwControl *child=childs.ctrls[i];
    if (child->x+child->w>=inw) inw=child->x+child->w+1;
    if (child->y+child->h>=inh) inh=child->y+child->h+1;
  }
  updatedinsize();
}

void sdwScrollControl::updatedscrollpos() {
  if (ishor) {
    if (dx<0) dx=0;
    if (dx/kx+hscrsize>hscrlen) dx=(hscrlen-hscrsize)*kx;
    pleft=dx/kx+vo->scrollbarborder+vo->scrollboxborder;
  } else dx=0;
  if (isver) {
    if (dy<0) dy=0;
    if (dy/ky+vscrsize>vscrlen) dy=(vscrlen-vscrsize)*ky;
    ptop=dy/ky+vo->scrollbarborder+vo->scrollboxborder;
  } else dy=0;
}

void sdwScrollControl::draw(sdwCanvas canvas) {
  canvas.frect(0,0,w-1,h-1,vo->clscrollboxbg);
  SDL_Rect clip=canvas.getclip();
  canvas.bound(SDLRect(vo->scrollbarborder,vo->scrollbarborder,
                 w-2*vo->scrollbarborder-isver,h-2*vo->scrollbarborder-ishor));
  sdwControl::draw(sdwCanvas(canvas,-dx+vo->scrollbarborder,-dy+vo->scrollbarborder));
  canvas.setclip(clip);
  if (ishor) {
    canvas.frect(vo->scrollbarborder,h-vo->scrollbarborder-ishor,
      w-vo->scrollbarborder-1-isver,h-vo->scrollbarborder,vo->clscrollbarbg);
    canvas.frect(pleft,h-vo->scrollbarborder-ishor+vo->scrollboxborder,
      pleft+hscrsize,h-vo->scrollboxborder-vo->scrollbarborder-1,vo->clscrollbar);
  }
  if (isver) {
    canvas.frect(w-vo->scrollbarborder-ishor,vo->scrollbarborder,
      w-vo->scrollbarborder,h-vo->scrollbarborder-1-ishor,vo->clscrollbarbg);
    canvas.frect(w-vo->scrollbarborder-isver+vo->scrollboxborder,ptop,
      w-vo->scrollboxborder-vo->scrollbarborder-1,ptop+vscrsize,vo->clscrollbar);
  }
}

void sdwScrolledPainterInner::dopaint(Canvas cnv) {
  awsEvent ev;
  main->scrpaint->onpaint(cnv,ev);
}

sdwScrolledPainterInner::sdwScrolledPainterInner(sdwScrolledPainter *amain,sdwSystem *sys):sdwPaintControl(NULL,sys) {
  main=amain;
}

static void makevis(int &pos,int len,int beg,int end) {
  if (end>pos+len) pos=end-len;
  if (beg<pos) pos=beg;
}

bool sdwScrolledPainter::scrollinview(const Rect &r) {
  makevis(dx,clientw,r.a.x,r.b.x);
  makevis(dy,clienth,r.a.y,r.b.y);
  updatedscrollpos();
  repaint();
}

sdwScrolledPainter::sdwScrolledPainter(ScrolledPainter *ctrl,sdwSystem *sys):sdwScrollControl(ctrl,sys) {
  scrpaint=ctrl;
  inner=new sdwScrolledPainterInner(this,sys);
  inner->setparent(this);
}


}

