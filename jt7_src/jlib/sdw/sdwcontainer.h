#ifndef sdwcontainerH
#define sdwcontainerH

#include "sdwcanvas.h"
#include <aws/awsdefs.h>
#include <aws/awscontrol.h>

namespace sdw {
using namespace aws;

class sdwControl;

struct sdwContainer {
  void paint(sdwCanvas cnv);
  void notifyremove(sdwControl* ctrl);
  void notifyadd(sdwControl* ctrl);
  bool mousedown(int x,int y,MouseButton button,ShiftState shift);
  void mouseup(int x,int y,MouseButton button,ShiftState shift);
  void mousemove(int x,int y,ShiftState shift);
  void mouseout();
  void bringtofront(sdwControl *ctrl);
  sdwControl *ctrlat(int x,int y);
  sdwContainer();

  TCntVec<sdwControl*> ctrls;
  sdwControl *mousefocus;
  sdwControl *mousedownctrl;
};

}

#endif
