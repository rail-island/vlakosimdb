#ifndef jwswinsysH
#define jwswinsysH

#include <jws/jws.h>
#include <jws/jwssystem.h>

namespace wlib {

struct WinSystem:jws::System {
  bool processmousemove,dorepainting;
  void (*onidle)();
  bool _terminated;
  bool _wantrepaint;

  void initflags() {processmousemove=true;dorepainting=true;onidle=NULL;_terminated=false;_wantrepaint=false;}
  void repaint();
  void exit();
  WinSystem(void *hInstance);
  HDC hmaindc;
  HWND hmainwin;
  int wndproc(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam);
  void messageloop();
  void displaychanged();
  jws::BitmapCanvas bmpcnv;
};

extern WinSystem *jwswinsys;

}

void jws_init();
void jws_done();

#endif
