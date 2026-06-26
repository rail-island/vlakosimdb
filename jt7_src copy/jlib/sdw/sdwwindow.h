#ifndef sdwwindowH
#define sdwwindowH

#include "sdwcontrol.h"
#include "sdwplace.h"
#include "sdwmenu.h"

namespace sdw {
class sdwMenuItem;class ViewOpts;

struct sdwWindow:sdwControl {
  sdwWindow(Window *ctrl,sdwSystem *sys);
  ~sdwWindow();
  void draw(sdwCanvas canvas);
  void show();
  void showmodal();
  void repaint();
  void repaint(const Rect &r);
  void onsize();
  sdwControl *getinner() {return inner;}
  string gettext() {return title;}
  void settext(string value) {title=value;repaint();}
  void mousedown(int x,int y,MouseButton but,ShiftState shift);
  void mouseup(int x,int y,MouseButton but,ShiftState shift);
  void mousemove(int x,int y,ShiftState shift);
  void mouseleave();
  void changedpos();
  void setmenu(MenuItem *menu);
  void notifyremove(Component *obj);
  Rect getclient() {return Rect(0,0,inner->w,inner->h);}
  void getchilds(TCntVec<Control*> &childs) {inner->getchilds(childs);}
  sdwWindow *getwindow() {return this;}
  void setfocus(sdwControl *ctrl);
  void sdwnotifyremove(sdwControl *ctrl);
  string getstyle() {return "";}
  void setstyle(string value) {}
  Size getclientsize() {return Size(inner->w,inner->h);}
  void setclientsize(Size value);
  void setstate(WindowState value) {state=value;}
  WindowState getstate() {return state;}
  void setposition(WindowPosition value) {position=value;}
  WindowPosition getposition() {return position;}
  void close();
  void denyclose() {deniedclose=false;}
  void bringtofront();
  SDL_Rect getrect() {return SDLRect(x,y,w,h);}
private:
  void resizeinner();
  int menuleft();
  int menutop();
  void paintmenuifneeded() {if (menubar.wantrepaint) repaint();}

  ViewOpts *vo;
  Window *win;
  sdwPlace *inner;
  sdwMenuItem *menu;
  sdwControl *focused;
  sdwMenuBar menubar;
  string title;
  WindowState state;
  WindowPosition position;
  bool deniedclose;
  bool moving;
  int movorigx,movorigy;
  friend class sdwSystem;
};



}

#endif
