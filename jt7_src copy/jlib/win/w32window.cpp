#include <winlib.h>
#pragma hdrstop

#include "w32window.h"
#include "w32app.h"
#include "w32keys.h"

namespace wlib {

static bool WINDOWCONTROL_CLASS_REGISTERED=false;
static ATOM WINDOWCONTROL_ATOM;
const char WINDOWCONTROL_PROP[]="CommonWindowClass_Object";

int CommonWindow::settimer(int id,float secs) {
  return SetTimer(win.handle,id,secs*1000,NULL);
}

LRESULT CommonWindow::wndproc(UINT msg,WPARAM wparam,LPARAM lparam) {
  switch (msg) {
    case WM_CLOSE:onclose();break;
    case WM_COMMAND:{
      int com=LOWORD(wparam);
      if (menu) menu->runcommand(com);
      if (com>0 and com<=childs.count) childs[com-1]->command();
    } break;
    case WM_KEYDOWN:case WM_SYSKEYDOWN:{
      int key=virtkeytokey(wparam,lparam);
      if (menu and menu->keydown(key)) {
      } else if (onkeydown(key)) {
      } else return DefWindowProc(win.handle,msg,wparam,lparam);
    } break;
    case WM_PAINT:{
      HDC dc;PAINTSTRUCT pi;
      dc=BeginPaint(win.handle,&pi);
      repaint(dc,pi);
      EndPaint(win.handle,&pi);
    } break;
    case WM_SIZE:{
      resized(LOWORD(lparam),HIWORD(lparam));
    } break;
    case WM_TIMER:{
      if (lparam) return DefWindowProc(win.handle,msg,wparam,lparam);
      else ontimer(wparam);
    } break;
    case WM_LBUTTONDOWN:{
      onmousedown(LOWORD(lparam),HIWORD(lparam),0);
    } break;
    case WM_RBUTTONDOWN:{
      onmousedown(LOWORD(lparam),HIWORD(lparam),1);
    } break;
    case WM_LBUTTONUP:{
      onmouseup(LOWORD(lparam),HIWORD(lparam),0);
    } break;
    case WM_RBUTTONUP:{
      onmouseup(LOWORD(lparam),HIWORD(lparam),1);
    } break;
    default:return DefWindowProc(win.handle,msg,wparam,lparam);
  }
  return 0;
}

static LRESULT CALLBACK window_control_wndproc(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam) {
  CommonWindow *win=(CommonWindow*)WWindow(hwin).getprop(WINDOWCONTROL_PROP);
  return win?win->wndproc(msg,wparam,lparam):DefWindowProc(hwin,msg,wparam,lparam);
}


static void register_window_control_class() {
  WINDOWCONTROL_CLASS_REGISTERED=true;
  registerwindowclass("CommonWindowClass",window_control_wndproc);
  WINDOWCONTROL_ATOM=GlobalAddAtom(WINDOWCONTROL_PROP);
}

void MaxWindow::show() {
  wanthandle();
  win.maxshow();
  win.update();
}

void CommonWindow::createwindow() {
  if (not WINDOWCONTROL_CLASS_REGISTERED) register_window_control_class();
  win.handle=createwnd();
  win.setprop(WINDOWCONTROL_PROP,this);
  if (menu) SetMenu(win.handle,menu->getmenu());
  actual_id=1;
  for(int i=0;i<childs.count;i++) childs[i]->recreatewnd(),actual_id++;
  for(int i=0;i<childs.count;i++) if (childs[i]->start) SetFocus(childs[i]->gethandle());
}

HWND MaxWindow::createwnd() {
  return CreateWindow("CommonWindowClass",caption.c_str(),0,//WS_OVERLAPPEDWINDOW
  CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,W_hInstance,NULL);
//  win.setprop(MAKEINTATOM(WINDOWCONTROL_ATOM),this);
}
HWND ModalDialog::createwnd() {
  WCanvas cnv(GetDC(0));
  int x=cnv.getw()/2-w/2,y=cnv.geth()/2-h/2;
  return CreateWindow("CommonWindowClass",caption.c_str(),WS_BORDER|WS_SYSMENU|WS_CAPTION,
  x,y,w,h,NULL,NULL,W_hInstance,NULL);
  cnv.release(0);
}

void ModalDialog::showmodal(CommonWindow *aparent) {
  parent=aparent;
  EnableWindow(parent->gethandle(),false);
  wanthandle();
  win.show();
  messageloop();
  loop_exited();
}
void ModalDialog::onclose() {
  EnableWindow(parent->gethandle(),true);
  destroywnd();
  loop_exit();
}
void CommonWindow::destroywnd() {
  for(int i=0;i<childs.count;i++) childs[i]->destroywnd();
  win.destroy();
}

WinControl::WinControl(CommonWindow *parent):parent(parent) {
  x=y=w=h=0;start=false;
}

HWND EditControl::createwnd() {
  WEdit res(parent->getwin(),x,y,w,parent->getid());
  res.settext(text.c_str());
  return res.handle;
}
HWND LabelControl::createwnd() {
  WLabel res(parent->getwin(),x,y,w,h);
  res.settext(text.c_str());
  return res.handle;
}
HWND ButtonControl::createwnd() {
  WButton res(parent->getwin(),x,y,w,h,parent->getid());
  res.settext(text.c_str());
  return res.handle;
}

void WinControl::createwindow() {
  ctrl.handle=createwnd();
}

void WinControl::updatepos() {
  SetWindowPos(ctrl.handle,NULL,x,y,w,h,SWP_NOZORDER);
}


}


