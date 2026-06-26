#include <jlib.h>
#pragma hdrstop

#include "sdwcontrol.h"
#include "sdwsystem.h"
#include "sdwwindow.h"
#include "sdwpaint.h"

namespace sdw {

sdwControl::sdwControl(Control *actrl,sdwSystem *asys) {
  ctrl=actrl;sys=asys;x=y=w=h=0;parent=NULL;
  parenttoreturn=NULL;window=NULL;
  sys->createdcontrol(this);
  font=dynamic_cast<sdwCommonFont*>(sys->createfont());
  visible=true;
}

sdwControl::~sdwControl() {
  sys->removedcontrol(this);
}

void sdwControl::sdwnotifyremove(sdwControl *ctrl) {
  if (ctrl==window) window=NULL;
}

void sdwControl::setfocus() {
  if (window) window->setfocus(this);
}

sdwControl *getsdwreal(Component *obj) {
  if (Control *ctrl=dynamic_cast<Control*>(obj))
    if (sdwControl *sdc=dynamic_cast<sdwControl*>(ctrl->getreal()))
      return sdc;
  return NULL;
}

void sdwControl::changedpos() {
  if (parent) parent->repaint();
  onsize();
}

void sdwControl::repaint() {
  if (parent) {
    parent->repaint(Rect(x,y,w,h));
  }
}
void sdwControl::repaint(const Rect &r) {
  if (parent) parent->repaint(r+Point(x,y));
}


void sdwControl::removechild(sdwControl *ctrl) {
  childs.notifyremove(ctrl);
  childschanged();
  repaint();
}
void sdwControl::addchild(sdwControl *ctrl) {
  childs.notifyadd(ctrl);
  childschanged();
  repaint();
}

void sdwControl::setparent(sdwControl *value) {
  parenttoreturn=NULL;window=NULL;
  if (parent) parent->removechild(this);
  sdwControl *p=value->getinner();
  if (p==this) parent=value;else parent=p;
  if (parent) parent->addchild(this),window=parent->getwindow();
}

Font *sdwControl::getfont() {
  return font;
}
void sdwControl::setfont(Font *value) {
  font=dynamic_cast<sdwCommonFont*>(value);
  if (!font) font=dynamic_cast<sdwCommonFont*>(sys->createfont());
}

void sdwControl::draw(sdwCanvas canvas) {
  childs.paint(canvas);
}

void sdwControl::paint(sdwCanvas canvas) {
  SDL_Rect r=canvas.getclip();
  sdwCanvas my=sdwCanvas(canvas,x,y);
  my.bound(SDLRect(0,0,w,h));
  draw(my);
  canvas.setclip(r);
}

void sdwControl::keydown(awsKey key) {
  awsEvent ev;
  if (ctrl) ctrl->onkeydown(key,ev);
}
void sdwControl::keyup(awsKey key) {
  awsEvent ev;
  if (ctrl) ctrl->onkeyup(key,ev);
}
void sdwControl::mousedown(int x,int y,MouseButton but,ShiftState shift) {
  awsEvent ev;
  if (ctrl) ctrl->onmousedown(x,y,but,shift,ev);
  childs.mousedown(x,y,but,shift);
}
void sdwControl::mouseup(int x,int y,MouseButton but,ShiftState shift) {
  awsEvent ev;
  if (ctrl) ctrl->onmouseup(x,y,but,shift,ev);
  childs.mouseup(x,y,but,shift);
}
void sdwControl::mousemove(int x,int y,ShiftState shift) {
  awsEvent ev;
  if (ctrl) ctrl->onmousemove(x,y,shift,ev);
  childs.mousemove(x,y,shift);
}
void sdwControl::mouseenter() {
  awsEvent ev;
  if (ctrl) ctrl->onmouseenter(ev);
}
void sdwControl::mouseleave() {
  awsEvent ev;
  if (ctrl) ctrl->onmouseleave(ev);
  childs.mouseout();
}
void sdwControl::onsize() {
  awsEvent ev;
  if (ctrl) ctrl->onsize(w,h);
}
void sdwControl::getchilds(TCntVec<Control*> &childs) {
  for(int i=0;i<this->childs.ctrls.count;i++)
    childs.add(this->childs.ctrls[i]->ctrl);
}


}

