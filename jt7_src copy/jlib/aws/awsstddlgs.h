#ifndef awsstddlgsH
#define awsstddlgsH

#include <ext/xmlnode.h>
#include "awspaint.h"
#include "awswindow.h"

namespace aws {

class System;

int msgbox(System *system,string title,string msg,string buttons);
bool inputquery(System *system,string title,string msg,string &res);
inline string inputbox(System *system,string title,string msg,string def) {
  string res=def;
  if (!inputquery(system,title,msg,res)) res="";
  return res;
}

bool runxmldialog(System *system,XMLNode dialog,XMLNode data);
bool settingsdialog(System *system,XMLNode settings,XMLNode data);

class WaitWindow;
struct WaitControl:PaintControl {
  WaitControl(WaitWindow *owner,System *system);
  void onpaint(Canvas &cnv,awsEvent &event);
private:
  WaitWindow *win;
};

struct WaitWindow:Window {
  WaitWindow(Component *owner,System *system);
  string getmsg() {return msg;}
  void setmsg(string value);
  override bool canbemain() {return false;}
  void show();
private:
  friend class WaitControl;
  WaitControl *ctrl;
  string appname;
  string msg;
  int bgcolor;
  int framecolor;
  int framewi;
  int msgcolor;
  int appcolor;
};

}

#endif
