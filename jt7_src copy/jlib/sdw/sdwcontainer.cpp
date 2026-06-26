#include <jlib.h>
#pragma hdrstop

#include "sdwcontainer.h"
#include "sdwcontrol.h"

namespace sdw {

void sdwContainer::bringtofront(sdwControl *ctrl) {
  int idx=ctrls.search(ctrl);
  if (idx>=0 && idx<ctrls.count-1) ctrls.exchange(idx,ctrls.count-1);
}

void sdwContainer::notifyremove(sdwControl* ctrl) {
  ctrls.remove(ctrl);
  if (ctrl==mousefocus) mousefocus=NULL;
  if (ctrl==mousedownctrl) mousedownctrl=NULL;
}

void sdwContainer::mouseout() {
  if (mousefocus) {
    mousefocus->mouseleave();
    mousefocus=NULL;
  }
}

void sdwContainer::notifyadd(sdwControl* ctrl) {
  if (ctrls.search(ctrl)<0) ctrls.add(ctrl);
}

void sdwContainer::paint(sdwCanvas cnv) {
  SDL_Rect sclip=cnv.getclip();
  Rect clip=Rect(sclip.x,sclip.y,sclip.w,sclip.h);
  for(int i=0;i<ctrls.count;i++) {
    sdwControl *ctrl=ctrls[i];
    if (ctrl->visible && iscollision(Rect(ctrl->x,ctrl->y,ctrl->w,ctrl->h),clip))
      ctrl->paint(cnv);
  }
}

sdwControl *sdwContainer::ctrlat(int x,int y) {
  for(int i=ctrls.count-1;i>=0;i--) if (ctrls[i]->visible && ctrls[i]->contain(x,y)) return ctrls[i];
  return NULL;
}
/*
void sdwContainer::mousebutton(int x,int y,MouseButton button,ShiftState shift,bool down) {
  if (!down && mousedown) {
    mousedown->mouseup(x-mousedown->x,y-mousedown->y,button,shift);
    mousedown=NULL;
  } else {
    sdwControl *at=ctrlat(x,y);
    if (at) {
      if (down) at->mousedown(x-at->x,y-at->y,button,shift),mousedown=at;
    }
  }
}
*/
void sdwContainer::mouseup(int x,int y,MouseButton button,ShiftState shift) {
  if (mousedownctrl) {
    mousedownctrl->mouseup(x-mousedownctrl->x,y-mousedownctrl->y,button,shift);
    mousedownctrl=NULL;
  }
}

bool sdwContainer::mousedown(int x,int y,MouseButton button,ShiftState shift) {
  sdwControl *at=ctrlat(x,y);
  if (at) {
    at->mousedown(x-at->x,y-at->y,button,shift),mousedownctrl=at;
    return true;
  }
  return false;
}

sdwContainer::sdwContainer() {
  mousefocus=NULL;mousedownctrl=NULL;
}

void sdwContainer::mousemove(int x,int y,ShiftState shift) {
  if (mousedownctrl) mousedownctrl->mousemove(x-mousedownctrl->x,y-mousedownctrl->y,shift);
  sdwControl *at=ctrlat(x,y);
  if (at!=mousefocus) {
    if (mousefocus) mousefocus->mouseleave();
    mousefocus=at;
    if (mousefocus) mousefocus->mouseenter();
  }
  if (at && !mousedownctrl) at->mousemove(x-at->x,y-at->y,shift);
}


}


