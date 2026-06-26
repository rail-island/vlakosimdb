#ifndef sdwradiogroupH
#define sdwradiogroupH

#include "sdwcontrol.h"

namespace sdw {
class ViewOpts;

struct sdwRadioGroup:sdwControl {
  sdwRadioGroup(RadioGroup *ctrl,sdwSystem *sys);
  void draw(sdwCanvas cnv);
  void setitems(string value);
  string getitems() {return items.gettext('\n');}
  int getcur() {return cur;}
  void setcur(int value) {cur=value;}
  string gettext() {return text;}
  void settext(string value) {text=value;}
  void mousedown(int x,int y,MouseButton but,ShiftState shift);
  void mouseup(int x,int y,MouseButton but,ShiftState shift);
  void mousemove(int x,int y,ShiftState shift);

  RadioGroup *radio;
  ViewOpts *vo;
  Strings items;
  int cur;
  string text;
};


}

#endif
