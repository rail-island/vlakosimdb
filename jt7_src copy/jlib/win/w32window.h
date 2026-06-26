#ifndef w32windowH
#define w32windowH

#include "w32dci.h"
#include "w32menu.h"

namespace wlib {

class WinControl;

inline string gettextfromwindow(WControl win) {
  string res;
  int len=win.gettextlen();
  char *buf=res.reservebuffer(len);
  win.gettext(buf,len);
  res.updatelen();
  return res;
}


struct CommonWindow {
  HWND gethandle() {return win.handle;}
  ~CommonWindow() {
    destroywnd();
  }
  CommonWindow() {menu=NULL;}
  LRESULT wndproc(UINT msg,WPARAM wparam,LPARAM lparam);
  void setmenu(MenuBar *amenu) {menu=amenu;}
  WWindow getwin() {return win;}
  void settext(const char *text) {caption=text;if (win.handle) win.settext(text);}
  int settimer(int id,float secs);//returns timer id
  void killtimer(int timer) {if (timer) KillTimer(win.handle,timer);}
  int getid() {return actual_id;}
  void close() {win.close();}
protected:
  void wanthandle() {if (!win.handle) createwindow();}
  void createwindow();
  void destroywnd();
  WWindow win;
  MenuBar *menu;
  string caption;
  virtual HWND createwnd()=0;
  virtual void repaint(HDC dc,PAINTSTRUCT &pi) {}
  virtual void resized(int w,int h) {}
  virtual void ontimer(int id) {}
  virtual void onmousedown(int x,int y,int button) {}
  virtual void onmouseup(int x,int y,int button) {}
  virtual void onclose() {PostQuitMessage(0);}
  virtual bool onkeydown(int key) {return false;}//vraci, zda bylo zpracovano
  IObjArray<WinControl> childs;
  int actual_id;
};

struct MaxWindow:CommonWindow {//window over whole screen
  virtual void onclose() {PostQuitMessage(0);}
  void show();
protected:
  HWND createwnd();
};

struct ModalDialog:CommonWindow {
  int w,h;
  void showmodal(CommonWindow *parent);
  void onclose();
protected:
  HWND createwnd();
  CommonWindow *parent;
};

struct WinControl {
  WinControl(CommonWindow *parent);
  void wanthandle() {if (!ctrl.handle) createwindow();}
  ~WinControl() {destroywnd();}
  void recreatewnd() {destroywnd();createwindow();}
  virtual void destroywnd() {ctrl.destroy();}
  int x,y,w,h;
  virtual void command() {}
  bool start;
  HWND gethandle() {return ctrl.handle;}
protected:
  CommonWindow *parent;
  virtual HWND createwnd()=0;
  WControl ctrl;
  void createwindow();
  void updatepos();
};

struct NotifyEvent {
  virtual void onclick(WinControl *sender)=0;
};

struct EditControl:WinControl {
  EditControl(CommonWindow *parent):WinControl(parent) {}
  HWND createwnd();
  string gettext() {if (ctrl.handle) return gettextfromwindow(ctrl);return text;}
  void settext(string s) {text=s;if (ctrl.handle) ctrl.settext(s.c_str());}
protected:
  string text;
  void destroywnd() {text=gettext();WinControl::destroywnd();}
};
struct LabelControl:WinControl {
  LabelControl(CommonWindow *parent):WinControl(parent) {}
  HWND createwnd();
  void settext(string s) {text=s;if (ctrl.handle) ctrl.settext(s.c_str());}
protected:
  string text;
};
struct ButtonControl:WinControl {
  HWND createwnd();
  ButtonControl(CommonWindow *parent):WinControl(parent) {onclick=NULL;}
  void settext(string s) {text=s;if (ctrl.handle) ctrl.settext(s.c_str());}
  void command() {if (onclick) onclick->onclick(this);}
  NotifyEvent *onclick;
protected:
  string text;
};

}

#endif
