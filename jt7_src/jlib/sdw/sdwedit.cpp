#include <jlib.h>
#pragma hdrstop

#include "sdwedit.h"
#include "sdwsystem.h"

namespace sdw {

sdwEdit::sdwEdit(Edit *ctrl,sdwSystem *sys):sdwControl(ctrl,sys) {
  edit=ctrl;vo=sys->vo;dx=0;textwi=0;virtualwi=0;curpos=0;
}

SDL_Rect sdwEdit::getcaret() {
  int x=vo->edittextwi(text.copy(0,curpos))-dx;
//  return SDLRect(vo->editborder+x,vo->editborder,vo->editcaretwi,vo->edithi-vo->editborder);
  return SDLRect(vo->editborder+x,0,vo->editcaretwi,vo->edithi);
}

void sdwEdit::curposchanged() {
  sys->caretreset();
}

void sdwEdit::onsize() {
  h=vo->edithi;
  sdwControl::onsize();
}

void sdwEdit::mousedown(int x,int y,MouseButton but,ShiftState shift) {
  sdwControl::mousedown(x,y,but,shift);
  setfocus();
}
void sdwEdit::mouseup(int x,int y,MouseButton but,ShiftState shift) {
  sdwControl::mouseup(x,y,but,shift);
}
void sdwEdit::mousemove(int x,int y,ShiftState shift) {
  sdwControl::mousemove(x,y,shift);
}


void sdwEdit::textchanged() {
  textwi=vo->edittextwi(text);
  virtualwi=textwi+vo->edittextwi(" ");
  repaint();
}

void sdwEdit::keydown(awsKey key) {
  if (key.type==kt_left && curpos>0) curpos--,curposchanged();
  if (key.type==kt_right && curpos<text.length()) curpos++,curposchanged();
  if (key.type==kt_home) curpos=0,curposchanged();
  if (key.type==kt_end) curpos=text.length(),curposchanged();
  if (key.type==kt_char) {
    int len=text.length();
    text.insert(curpos,string(key.ch));
    if (text.length()>len) curpos++;
    textchanged();
  }
  if (key.type==kt_delete) {
    if (curpos<text.length()) text.erase(curpos,1);
    textchanged();
  }
  if (key.type==kt_bs) {
    if (curpos>0) curpos--,text.erase(curpos,1);
    textchanged();
  }
}

void sdwEdit::draw(sdwCanvas cnv) {
  cnv.rect(0,0,w-1,h-1,vo->cleditframe);
  cnv.frect(vo->editborder-1,vo->editborder-1,w-vo->editborder,h-vo->editborder,vo->cleditbg);
  cnv.text(vo->editborder-dx,vo->editborder,text,vo->cledittext,font);
}


}

