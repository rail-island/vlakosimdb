#ifndef awsnullctrlH
#define awsnullctrlH

#include "awscontrol.h"

namespace aws {

class RealNullControl;

struct NullControl:Control {//neviditelny ovladaci prvek (ale ma rodice...)
  RealNullControl *getreal() {return (RealNullControl*)Control::getreal();}

  NullControl(Component *owner,System *system);
};

/*
class RealPolyControl;

struct PolyControl {
  RealPolyControl *getreal() {return (RealNullControl*)Control::getreal();}

  PolyControl(Component *owner,System *system);
protected:
  void addsubctrl(Control *slave);
};
*/

}

#endif
