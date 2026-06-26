#ifndef awwcontrolH
#define awwcontrolH

#include <aws/awsreal.h>
#include <win/w32dci.h>
#include <win/w32keys.h>
#include "awwobject.h"

namespace aww {
using namespace aws;
struct awwSystem;class awwFont;

class awwControl;class awwObject;class awwWindow;

/*
struct WinChild {
  awwControl *wc;
  Control *ac;
};
*/

struct awwControl:awwObject,RealComplex {
//  WControl win;
  ~awwControl();
  awwWindow *parentwin;
  LRESULT wndproc(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam);
  void destroywnd();
private:
  int actual_id;
protected:
  awwControl *parent;
  bool visible,enabled;
  awwFont *font;
  int x,y,w,h;
  bool updating;
  bool firstcreate;
  string hint;
  Cursor cursor;
  TCntVec<awwControl*> childs;
//  TCntVec<WinChild> childs;
protected:
  awwControl(Control *ctrl);
  awwSystem *system;Control *ctrl;
  int getid() {return actual_id;}

  void notifyremove(Component *comp);
  void wanthandle() {if (!win.handle) createwindow();}
  virtual void createwindow();
  virtual void createparams(CreateParams &pars);
  virtual HWND createwnd();

  virtual bool specialwndproc() {return false;}

//  virtual void defproc(awwMessage &msg);

  void beginupdate() {updating=true;}
  bool wantanswer() {return win and not updating;}
  void endupdate() {updating=false;recreatewnd();}

  void setsdp(string name,string value) {}
  string getsdp(string name) {return "";}
  bool supportsdp(string name) {return false;}
  HCURSOR savedcursor;
//  HWND tooltipwin;
public:
  virtual void recreatewnd() {updatemypos();destroywnd();createwindow();}


  virtual void onmousedown(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift);
  virtual void onmouseup(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift);
  virtual void onmousemove(awwMessage &msg,int x,int y,ShiftState shift);
  virtual void ontimer(awwMessage &msg,int id) {}
  virtual void oncommand(awwMessage &msg,int id,int notifycode) {}
  virtual void onclose(awwMessage &msg) {}
  virtual void onkeydown(awwMessage &msg,awsKey key) {if (ctrl) ctrl->onkeydown(key,msg);awwControl::childonkeydown(msg,key);}
  virtual void onkeyup(awwMessage &msg,awsKey key) {if (ctrl) ctrl->onkeyup(key,msg);}
  virtual void onpaint(awwMessage &msg) {}
  virtual void onsize(awwMessage &msg,int w,int h);
  virtual void onhscroll(awwMessage &msg,int code,int pos,HWND scroll) {}
  virtual void onvscroll(awwMessage &msg,int code,int pos,HWND scroll) {}
  virtual void onsetfocus(awwMessage &msg) {if (ctrl) ctrl->focused(msg);}
  virtual void onkillfocus(awwMessage &msg) {if (ctrl) ctrl->unfocused(msg);}
  virtual void onerasebkgnd(awwMessage &msg) {}
  virtual void resizedchild() {}
  virtual void onmousewheel(awwMessage &msg,int x,int y,int delta,ShiftState shift) {if (ctrl) ctrl->onmousewheel(x,y,delta,shift,msg);}
  virtual void ondestroywnd();
  virtual void onchar(awwMessage &msg,char ch) {if (ctrl) ctrl->onchar(ch,msg);}

  virtual int getmousedx() {return 0;}
  virtual int getmousedy() {return 0;}

  void onmouseleave();
  void onmouseenter();
//  virtual void onselchange(awwMessage &msg) {}
  virtual void childonkeydown(awwMessage &msg,awsKey key) {if (parent) parent->childonkeydown(msg,key);}
  void recreatechilds();
protected:
  void show() {visible=true;if (win) win.show();}// wanthandle();win.show();win.update();}
  void hide() {visible=false;if (win) win.hide();}
//  void setpos(Point pos) {x=pos.x;y=pos.y;updatepos();}
  ParentControl *getparent();
  bool isvisible() {return visible;}
  bool getvisible() {return visible;}
  void setvisible(bool value) {if (value) show();else hide();}
  bool getenabled() {return enabled;}
  void setenabled(bool value) {if (enabled!=value) {enabled=value;if (win) EnableWindow(win,enabled);}}
  Font *getfont();
  void setfont(Font *afont);
  void getchilds(TCntVec<Control*> &childs) {childs.clear();for(int i=0;i<this->childs.count;i++) childs.add(this->childs[i]->ctrl);}
  Rect getclient();
  void setparent(ParentControl *parent);
  void repaint(const Rect &r) {xabstract;}
  void repaint() {xabstract;}
  void sethint(string value) {hint=value;}
  string gethint() {return hint;}
  void selectall() {if (win) SendMessage(win,EM_SETSEL,0,-1);}
  virtual void selectedfocus(awwControl *ctrl) {if (parent) parent->selectedfocus(ctrl);}
protected:
  virtual void updatemypos();
  virtual void updatepos() {setwindowpos(x,y,w,h);}
  void removechild(awwControl *ctrl);
  virtual void addchild(awwControl *ctrl);
  bool getmodified() {return win.getmodified();}
  void setmodified(bool value) {win.setmodified(value);}
  void updatecursor();
  void checkcursor();
public:
  void setparent(awwControl *parent);
  void setpos(Point pos,Size size) {x=pos.x;y=pos.y;w=size.w;h=size.h;updatepos();}
  Point getpos() {updatemypos();return Point(x,y);}
  Point getscreenpos() {POINT pt;pt.x=pt.y=0;ClientToScreen(win,&pt);return Point(pt.x,pt.y);}
  Size getsize() {updatemypos();return Size(w,h);}
  void setsize(Size size) {w=size.w;h=size.h;updatepos();}
  void setwindowpos(int x,int y,int w,int h) {SetWindowPos(win,NULL,x,y,w,h,SWP_NOZORDER);}
  void setfocus() {if (win) SetFocus(win);else if (parent) parent->selectedfocus(this);}
  Cursor getcursor() {return cursor;}
  void setcursor(Cursor value) {cursor=value;updatecursor();}
};

inline awwControl *ctrlfromhandle(HWND handle) {return (awwControl*)objfromhandle(handle);}


}

#endif
