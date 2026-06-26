#include <winlib.h>
#pragma hdrstop

#include "awwobject.h"

namespace aww {

ATOM awwObject_Atom;

void init_awwobject_atom() {
  char name[0x100];
//  sprintf(name,"awwObject_%d",time(NULL));
  int pid=GetCurrentProcessId();
  sprintf(name,"awwObject_%d",pid);
  awwObject_Atom=GlobalAddAtom(name);
}

void delete_awwobject_atom() {
  GlobalDeleteAtom(awwObject_Atom);
}

string awwObject::getwndtext() {
  string res;
  int len=win.gettextlen();
  char *buf=res.reservebuffer(len);
  win.gettext(buf,len+1);
  res.updatelen();
  return res;
}

void awwObject::defproc(awwMessage &msg) {
  if (!msg.handled) {
    if (defwndproc) msg.res=CallWindowProc(
    #ifdef __BORLANDC__
    (FARPROC)
    #else
    (WNDPROC)
    #endif
    defwndproc,msg.hwin,msg.msg,msg.wparam,msg.lparam);
    else msg.res=DefWindowProc(msg.hwin,msg.msg,msg.wparam,msg.lparam);
    msg.handled=true;
  }
}


}


