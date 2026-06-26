#include <jlib.h>
#pragma hdrstop

#include "sdwtrackbar.h"
#include "sdwopts.h"
#include "sdwsystem.h"

namespace sdw {

sdwTrackBar::sdwTrackBar(TrackBar *ctrl,sdwSystem *sys):sdwControl(ctrl,sys) {
  bar=ctrl;vo=sys->vo;
  min=max=cur=0;
  vertical=false;
  freq=1;
  changing=false;
}

void sdwTrackBar::setcur(int value) {
  cur=value;
  if (value<min) value=min;
  if (value>max) value=max;
  repaint();
}

void sdwTrackBar::draw(sdwCanvas cnv) {
  cnv.frect(0,0,w-1,h-1,vo->cltrackbarbg);
  if (min>=max || cur<min || cur>max) return;
  float pos=(float)(cur-min)/(max-min);
  if (vertical) {
    cnv.frect(0,pos*h-vo->trackbarsize/2,w-1,pos*h+vo->trackbarsize/2,vo->cltrackbardragger);
  } else {
    cnv.frect(pos*w-vo->trackbarsize/2,0,pos*w+vo->trackbarsize/2,h-1,vo->cltrackbardragger);
  }
}

void sdwTrackBar::mousedown(int x,int y,MouseButton but,ShiftState shift) {
  sdwControl::mousedown(x,y,but,shift);
  if (min>=max || cur<min || cur>max) return;
  float pos=(float)(cur-min)/(max-min);
  if (vertical) {
    if (abs(y-pos*h)<=vo->trackbarsize/2) changing=true;
  } else {
    if (abs(x-pos*w)<=vo->trackbarsize/2) changing=true;
  }
}
void sdwTrackBar::mouseup(int x,int y,MouseButton but,ShiftState shift) {
  sdwControl::mouseup(x,y,but,shift);
  changing=false;
}
void sdwTrackBar::mousemove(int x,int y,ShiftState shift) {
  sdwControl::mousemove(x,y,shift);
  if (changing) {
    float pos;
    if (vertical) pos=(float)y/h;else pos=(float)x/w;
    cur=min+pos*(max-min);
    if (cur<min) cur=min;
    if (cur>max) cur=max;
    bar->dochange();
    repaint();
  }
}



}

