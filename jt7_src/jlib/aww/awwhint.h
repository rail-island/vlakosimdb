#ifndef awwhintH
#define awwhintH

#include "awwcontrol.h"

namespace aww {

/*
struct awwHintWindow:awwPaintControl {
  awwHintWindow():awwPaintControl(NULL) {}
  void createparams(CreateParams &pars);
  void settext(string value);
private:
  string text;
};
*/

class awwSystem;

struct awwHintWindow {
  awwHintWindow(awwSystem *system);
  void show(awwControl *parent,string text);
  void hide();
  LRESULT wndproc(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam);
private:
  awwSystem *system;
  HWND win;
  string text;
  int w,h;
};


}

#endif
