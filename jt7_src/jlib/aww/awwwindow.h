#ifndef awwwindowH
#define awwwindowH

#include "awwcontrol.h"
//#include <win/w32menu.h>
#include <aws/awswindow.h>
#include "awwmenu.h"

namespace aww {

struct awwWindow:awwControl {
//  void maximize() {wanthandle();visible=true;WWindow(win).maximize();}
//  void minimize() {wanthandle();visible=true;WWindow(win).minimize();}
//  void normalize() {wanthandle();visible=true;WWindow(win).normalize();}
  void close();
  void onclose(awwMessage &msg);
  awwWindow(Window *ctrl);
  void createparams(CreateParams &pars);
  void setmenu(aws::MenuItem *menu);
  void onsize(awwMessage &msg,int w,int h);

  void setstate(WindowState astate) {state=astate;if (win) updatestate();}
  WindowState getstate() {updatemystate();return state;}
  void setposition(WindowPosition aposition) {position=aposition;}
  WindowPosition getposition() {return position;}
  string getstyle() {return style;}
  void setstyle(string value) {style=value;if (win) recreatewnd();}

//  void defaultshow();
  string gettext() {return text;}
  void settext(string atext) {text=atext;if (win) win.settext(text.c_str());}
  void denyclose() {_denyclose=true;}
  void minimizewin(bool minimizethis=false);
private:
  WindowState state;WindowPosition position;
  awwControl *focused_ctrl;
  string text;
  string style;
  awwMenuItem *menu;
  bool _denyclose;
//  wlib::winMenuBar *wmenu;
//  aws::MenuBar *menu;
//  bool defaultpos;
  void updatemenu();
  void updatemystate();
  void updatestate();
protected:
  void oncommand(awwMessage &msg,int com,int nitify);
  void onkeydown(awwMessage &msg,awsKey key);
  void notifyremove(Component *obj);
  void childonkeydown(awwMessage &msg,awsKey key) {onkeydown(msg,key);}
  void createwindow();
  void showmodal();
  Size getclientsize();
  void setclientsize(Size size);
  void selectedfocus(awwControl *ctrl) {if (ctrl) focused_ctrl=ctrl;}
protected:
  void show();
  bool isclientsize;
  Size getframesize();
  aws::Window *awswin;
  awwWindow *modalparent;//okno, ktere je timto zablokovano kvuli modalnosti
};

}

#endif
