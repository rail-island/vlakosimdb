#ifndef sdweditH
#define sdweditH

#include "sdwcontrol.h"

namespace sdw {
class ViewOpts;

struct sdwEdit:sdwControl {
  sdwEdit(Edit *ctrl,sdwSystem *sys);
  string gettext() {return text;}
  void settext(string value) {text=value;textchanged();}
  void draw(sdwCanvas cnv);
  void textchanged();
  void keydown(awsKey key);
  void curposchanged();
  void onsize();
  void mousedown(int x,int y,MouseButton but,ShiftState shift);
  void mouseup(int x,int y,MouseButton but,ShiftState shift);
  void mousemove(int x,int y,ShiftState shift);
  SDL_Rect getcaret();

  Edit *edit;
  ViewOpts *vo;
  string text;
  int textwi,virtualwi;
  int dx;
  int curpos;
};



}

#endif
