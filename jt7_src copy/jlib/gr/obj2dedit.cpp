#include <jlib.h>
#pragma hdrstop

#include "obj2dedit.h"
#include <jmath/analgeom.cpp>

namespace gr {
using namespace jmath;

void GRStdState::setselected(Array<GRObject*> &selected) {
  this->selected.assign(selected);
  editor=NULL;
  if (selected.count==1) editor=selected[0]->editstate(this);
  onlistchng(param,this);
}

void GRStdState::mousedown(BUTTON bt,GREditor *sender,int x,int y,int shift) {
  begin:
  if (editor and editor->mousedown(bt,sender,x,y,shift)) return;
  //kliknuti zpracoval editor objeku-neni uz pro me
  GRObject *act=obj->findobjat(XYREC(mouserealx,mouserealy),t->ltop(),t);
  if (act) {
    int index=selected.search(act);
    if (shift&ss_ctrl) {
      editor=NULL;
      if (index>=0) selected.erase(index);else selected.add(act);
    } else {
      if (index<0) {
        selected.clear();selected.add(act);
        editor=act->editstate(this);
        goto begin;
      } else {
        moving=new Moving(x,y);
        if (selected.count==1) editor=act->editstate(this);
      }
    }
  } else selected.clear(),editor=NULL;
  onlistchng(param,this);
}
void GRStdState::mousemove(GREditor *sender,int x,int y,int shift) {
  if (moving) {
    int dx=x-moving->lastx,dy=y-moving->lasty;
    for(int i=0;i<selected.count;i++) selected[i]->translate(dx,dy);
    moving=new Moving(x,y);
  } else if (editor) editor->mousemove(sender,x,y,shift);
}
void GRStdState::mouseup(BUTTON bt,GREditor *sender,int x,int y,int shift) {
  if (moving) moving=NULL;
  else if (editor) editor->mouseup(bt,sender,x,y,shift);
}

void GRStdState::drawselectedobj(GRObject *obj,CANVAS &cnv,CANVAS &orig) {
  TCntVec<XYREC> pts;
  obj->getselectpoints(pts);
  for(int i=0;i<pts.count;i++) t->ltop(pts[i].x,pts[i].y);
  for(int i=0;i<pts.count;i++) {
    XYREC &pt=pts[i];
    orig.bg(lcblack).frect(pt.x-2,pt.y-2,pt.x+2,pt.y+2);
  }
}

void GRStdState::redraw(GREditor *sender,CANVAS &cnv,CANVAS &orig) {
  obj->draw(cnv);
  for(int i=0;i<selected.count;i++) drawselectedobj(selected[i],cnv,orig);
  if (editor) editor->redraw(sender,cnv,orig);
}

int GREditPoly::nearline() {
  int minidx=-1;float mind=0;
  int cnt=grp->pts.count;
  if (cnt>0 and cnt<=2) return 0;
  for(int i=0;i<cnt;i++) {
    XYREC a=lpt1(i),b=lpt2(i);
    state->ltop(a);state->ltop(b);
    agline<int> line(a,b);
    float actd=line.pdistfromline(agpoint<int>(state->mouserealx,state->mouserealy));
    if (minidx<0 or actd<mind) mind=actd,minidx=i;
  }
  return minidx;
}

void GRCreatorState::cancel(GREditor *sender) {
  delete created;created=NULL;
  sender->setstate(new GRStdState(obj,this));
}

void GRCreatorState::create(GREditor *sender) {
  obj->items.add(created);obj->modified=true;created=NULL;
  if (oncreate) oncreate(param,this);
  sender->repaint();
  sender->setstate(new GRStdState(obj,this));
}
void GR2Creator::redraw(GREditor *sender,CANVAS &cnv,CANVAS &orig) {
  obj->draw(cnv);
  if (down) gr2->draw(cnv);
}
void GRPolyCreator::redraw(GREditor *sender,CANVAS &cnv,CANVAS &orig) {
  obj->draw(cnv);
  grp->draw(cnv);
}
void GRMiddle::mousedown(BUTTON bt,GREditor *sender,int x,int y,int shift) {
  if (bt==left) {
    obj->setmiddle(x,y);
    if (done) done(param,this);
    sender->setstate(new GRStdState(obj,this));
  }
}


}
#pragma package(smart_init)
