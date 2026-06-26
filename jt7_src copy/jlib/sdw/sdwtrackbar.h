#ifndef sdwtrackbarH
#define sdwtrackbarH

#include "sdwcontrol.h"

namespace sdw {

struct sdwTrackBar:sdwControl {
  sdwTrackBar(TrackBar *ctrl,sdwSystem *sys);
  void draw(sdwCanvas cnv);
  void mousedown(int x,int y,MouseButton but,ShiftState shift);
  void mouseup(int x,int y,MouseButton but,ShiftState shift);
  void mousemove(int x,int y,ShiftState shift);

  int getmin() {return min;}
  void setmin(int value) {min=value;}
  int getmax() {return max;}
  void setmax(int value) {max=value;}
  int getcur() {return cur;}
  void setcur(int value);
  int getfrequency() {return freq;}
  void setfrequency(int value) {freq=value;}
  bool getvertical() {return vertical;}
  void setvertical(bool value) {vertical=value;}

  TrackBar *bar;
  int min,max,cur,freq;
  bool vertical;
  ViewOpts *vo;
private:
  bool changing;
};


}

#endif
