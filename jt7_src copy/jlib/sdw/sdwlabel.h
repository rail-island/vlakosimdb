#ifndef sdwlabelH
#define sdwlabelH

#include "sdwcontrol.h"

namespace sdw {
class sdwSystem;

struct sdwLabel:sdwControl {
  void draw(sdwCanvas canvas);
  sdwLabel(Label *ctrl,sdwSystem *sys);
  void settext(string value) {text=value;updatesize();}
  void updatesize();
  string gettext() {return text;}
  Label *label;
  string text;
};

struct sdwWrapLabel:sdwControl {
  void draw(sdwCanvas canvas);
  sdwWrapLabel(WrapLabel *ctrl,sdwSystem *sys);
  void settext(string value) {text=value;repaint();}
  void updatesize();
  string gettext() {return text;}
  WrapLabel *label;
  string text;
};

}

#endif
