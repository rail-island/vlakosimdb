#include <winlib.h>
#pragma hdrstop

#include "w32app.h"
#include "w32dci.h"

namespace wlib {

static int returncode=0;
static bool quit_message_loop=false;

void loop_exit() {
  quit_message_loop=true;
}
void loop_exited() {
  quit_message_loop=false;
}

void messageloop() {
  MSG msg;
  while (GetMessage(&msg,0,0,0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (quit_message_loop) return;
  }
  returncode=msg.wParam;
}

}

WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  using namespace wlib;
  char buf[0x200];
  GetModuleFileName(0,buf,sizeof(buf));
  basedir=buf;basedir--;
  W_init(hInstance);
  wmain();
  return returncode;
}

