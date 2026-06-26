#include <jlib.h>
#pragma hdrstop

#include "sdwradiogroup.h"
#include "sdwsystem.h"

namespace sdw {

sdwRadioGroup::sdwRadioGroup(RadioGroup *ctrl,sdwSystem *sys):sdwControl(ctrl,sys) {
  radio=ctrl;w=200;h=200;cur=0;vo=sys->vo;
}

void sdwRadioGroup::mousedown(int x,int y,MouseButton but,ShiftState shift) {
  sdwControl::mousedown(x,y,but,shift);
  if (items.count==0) return;
  int clh=h-vo->radiotitlehi-vo->radiobotspace;
  int itemh=clh/items.count;
  int y0=vo->radiotitlehi;
  int cli=(y-y0)/itemh;
  if (cli>=0 && cli<items.count) {
    cur=cli;
    repaint();
    radio->dochange();
  }
}
void sdwRadioGroup::mouseup(int x,int y,MouseButton but,ShiftState shift) {
  sdwControl::mouseup(x,y,but,shift);
}
void sdwRadioGroup::mousemove(int x,int y,ShiftState shift) {
  sdwControl::mousemove(x,y,shift);
}

void sdwRadioGroup::setitems(string value) {
  items.fromtext(value,'\n');cur=0;
  for(int i=0;i<items.count;i++) items[i]=lang_text_ch(items[i]);
  repaint();
}

void sdwRadioGroup::draw(sdwCanvas cnv) {
  cnv.frect(0,0,w-1,h-1,vo->clbtnface);
  cnv.rect(1,vo->radiotitlehi/2,w-2,h-2,vo->clradioframe);
  cnv.frect(vo->radiotitleleft,0,vo->radiotitleleft+vo->radiotextwi(text),vo->radiotitlehi,vo->clbtnface);
  cnv.text(vo->radiotitleleft,1,text,vo->clradiotext,sys->deffont);
  if (items.count==0) return;
  int clh=h-vo->radiotitlehi-vo->radiobotspace;
  int itemh=clh/items.count;
  int acty=vo->radiotitlehi;
  for(int i=0;i<items.count;i++) {
    cnv.drawradio(vo->radiohborder+vo->radioradius+vo->radiospaceleft,acty+itemh/2,i==cur,vo);
    cnv.text(vo->radiohborder+2*vo->radioradius+vo->radiospaceleft+vo->radiospaceright,
      acty+itemh/2-vo->radiofonthi/2,items[i],vo->clradiotext,sys->deffont);
    acty+=itemh;
  }
}


}


