#ifndef sdwcontrolH
#define sdwcontrolH

#include <sdl/sdl.h>
#include <aws/awsreal.h>
#include "sdwcanvas.h"
#include "sdwcontainer.h"
#include <com/ptrto.h> 

namespace sdw {
using namespace aws;
class sdwSystem;class sdwControl;class sdwWindow;class sdwCommonFont;

sdwControl *getsdwreal(Component *obj);

struct sdwControl:RealComplex {
  Control *ctrl;
  sdwSystem *sys;
  sdwControl *parent;
  int x,y,w,h;

  void setpos(Point pos,Size size) {x=pos.x;y=pos.y;w=size.w;h=size.h;changedpos();}
  Point getpos() {return Point(x,y);}
  Point getscreenpos() {return Point();}
  Size getsize() {return Size(w,h);}
  void setparent(ParentControl *value) {setparent(getsdwreal(value));parenttoreturn=value;}
  ParentControl *getparent() {return parenttoreturn;}
  void repaint();
  void repaint(const Rect &r);
  void setfocus();

  void setparent(sdwControl *value);

  sdwControl(Control *actrl,sdwSystem *asys);
  ~sdwControl();
  virtual void keydown(awsKey key);
  virtual void keyup(awsKey key);
  virtual void mousedown(int x,int y,MouseButton but,ShiftState shift);
  virtual void mouseup(int x,int y,MouseButton but,ShiftState shift);
  virtual void mousemove(int x,int y,ShiftState shift);
  virtual void mouseenter();
  virtual void mouseleave();
  virtual void onsize();
  virtual sdwControl *getinner() {return this;}
  void notifyremove(Component *obj) {if (obj==ctrl) ctrl=NULL;}
  void paint(sdwCanvas canvas);//call this
  virtual void draw(sdwCanvas canvas);//override this
  bool contain(int px,int py) {return px>=x && py>=y && px<x+w && py<y+h;}
  void changedpos();
  virtual void removechild(sdwControl *ctrl);
  virtual void addchild(sdwControl *ctrl);
  virtual void childschanged() {}
  Rect getclient() {return Rect(0,0,w,h);}
  void getchilds(TCntVec<Control*> &childs);
  virtual sdwWindow *getwindow() {return window;}
  virtual void sdwnotifyremove(sdwControl *ctrl);
  virtual SDL_Rect getcaret() {return SDLRect(0,0,0,0);}
  Point abspos() {Point res=Point(x,y);if (parent) res=res+parent->abspos();return res;}
  string gethint() {return hint;}
  void sethint(string value) {hint=value;}
  Font *getfont();
  void setfont(Font *font);
  bool getvisible() {return visible;}
  void setvisible(bool value) {visible=value;if (parent) parent->repaint();} 

  sdwContainer childs;
  ParentControl *parenttoreturn;
  sdwWindow *window;
  string hint;
  bool visible;
  PtrTo<sdwCommonFont> font;
};


}

#endif
