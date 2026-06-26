#ifndef awwglviewH
#define awwglviewH

#include "awwstdctrls.h"
#include <aws/awsglview.h>

namespace aww {

struct awwGLView:awwPlace,RealGLView {
  awwGLView(GLView *glview);
  virtual void onpaintgl(Canvas &cnv,awsEvent &event) {}
  void createwindow();
  void ondestroywnd();
  void onpaint(awwMessage &msg);
  void onsize(awwMessage &msg,int w,int h);
  void dorender();
  void render();
  RealControl *getrealctrl() {RealPlace *pl=this;return pl;}//dvojita konverze=oklamani C++

  GLView *glv;
  HDC  hdc;
  HGLRC  hglrc;  void wantdc();};

}

#endif
