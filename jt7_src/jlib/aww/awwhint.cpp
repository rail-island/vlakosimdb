#include <winlib.h>
#pragma hdrstop

#include "awwhint.h"
#include "awwsystem.h"
#include <win/w32gr.h>
#include <gr/comgr.h>

namespace aww {

const int MAX_HINT_WIDTH=200;

static LRESULT CALLBACK awwHintWindow_wndproc(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam) {
  void *awwwin=GetProp(hwin,"awwHintWindow_object");
  if (awwwin) return ((awwHintWindow*)awwwin)->wndproc(hwin,msg,wparam,lparam);
  return DefWindowProc(hwin,msg,wparam,lparam);
}

LRESULT awwHintWindow::wndproc(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam) {
  switch (msg) {
    case WM_PAINT:{
      PAINTSTRUCT pc;
      BeginPaint(hwin,&pc);
      wlib::WinDC wdc(pc.hdc);
      SetBkMode(pc.hdc,TRANSPARENT);
      wdc.setfont(GetStockObject(ANSI_VAR_FONT));
      wdc.setbrush(gr::clnyellow);
      Rectangle(pc.hdc,1,1,w-1,h-1);
      RECT r;r.left=r.top=1;r.right=w;r.bottom=h;
      DrawText(pc.hdc,text.c_str(),text.length(),&r,DT_WORDBREAK|DT_NOPREFIX|DT_LEFT);
      EndPaint(hwin,&pc);
    }
  }
  return DefWindowProc(hwin,msg,wparam,lparam);
}

void awwHintWindow::show(awwControl *parent,string atext) {
  hide();
  text=atext;
  HDC dc=GetDC(0);
  wlib::WinDC wdc(dc);
  wdc.setfont(GetStockObject(ANSI_VAR_FONT));
  RECT r;
  r.top=r.bottom=r.right=r.left=0;
  POINT pt;pt.x=pt.y=0;
  ClientToScreen(parent->win,&pt);
  pt.y+=parent->getsize().h;
  r.right=MAX_HINT_WIDTH;
  DrawText(dc,text.c_str(),text.length(),&r,DT_CALCRECT|DT_WORDBREAK|DT_NOPREFIX|DT_LEFT);
  ReleaseDC(0,dc);
  w=r.right+3;h=r.bottom+3;
  win=CreateWindowEx(0,"awwHintWindow","awwHintWindow",WS_POPUP,
      pt.x,pt.y,w,h,system->win,NULL,W_hInstance,NULL);
  SetProp(win,"awwHintWindow_object",this);
  ShowWindow(win,SW_SHOWNOACTIVATE);
  UpdateWindow(win);
//  win=CreateWindowEx( );
}
void awwHintWindow::hide() {
  if (win) DestroyWindow(win);
  win=NULL;
}

awwHintWindow::awwHintWindow(awwSystem *system):system(system) {
  registerwindowclass("awwHintWindow",awwHintWindow_wndproc);
  win=NULL;
}

}

