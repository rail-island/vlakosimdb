#ifndef awwobjectH
#define awwobjectH

#include <win/w32dci.h>
#include <aws/awscontrol.h>

namespace aww {

extern ATOM awwObject_Atom;

struct CreateParams {
  int style,exstyle;
  HWND parent;
  char classname[0x100];
  char windowname[0x100];
  HMENU menu;
  int cid;
  int x,y,w,h;
};

struct awwMessage:aws::awsEvent {
  UINT msg;
  WPARAM wparam;
  LPARAM lparam;
  HWND hwin;
  bool handled;
  awwMessage(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam):hwin(hwin),msg(msg),wparam(wparam),lparam(lparam) {
    handled=false;
    res=0;
  }
  LRESULT res;
  void done() {handled=true;}
private:
  awwMessage(const awwMessage&);
};

struct awwObject {
  WControl win;
  ~awwObject() {destroywnd();}
  awwObject() {defwndproc=NULL;}
  virtual LRESULT wndproc(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam) {return DefWindowProc(hwin,msg,wparam,lparam);}
  virtual void destroywnd() {win.destroy();}
  string getwndtext();
  virtual void defproc(awwMessage &msg);
  WNDPROC defwndproc;
};

//inline awwObject *objfromhandle(HWND handle) {return (awwObject*)WWindow(handle).getprop("awwObject");}
//inline void objtohandle(HWND handle,awwObject *obj) {WWindow(handle).setprop("awwObject",obj);}

inline awwObject *objfromhandle(HWND handle) {return (awwObject*)WWindow(handle).getprop(awwObject_Atom);}
inline void objtohandle(HWND handle,awwObject *obj) {WWindow(handle).setprop(awwObject_Atom,obj);}

inline aws::Rect convertrect(const RECT &r) {return aws::Rect(aws::Point(r.left,r.top),aws::Point(r.right,r.bottom));}
inline void convertrect(const aws::Rect &r,RECT *out) {out->left=r.a.x;out->right=r.b.x;out->top=r.a.y;out->bottom=r.b.y;}

void init_awwobject_atom();
void delete_awwobject_atom();

}

#endif
