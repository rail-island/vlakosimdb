#include <winlib.h>
#pragma hdrstop

#include "awwcontrol.h"
#include "awwsystem.h"
#include <win/w32keys.h>
#include "commctrl.h"
#include "awwwindow.h"
#include "awwpaint.h"
#include "awwfont.h"
#include "awwhint.h"
#include <typeinfo>

namespace aww {

static ShiftState keystoshift(WPARAM keys) {
  ShiftState res;
  if (keys&MK_CONTROL) res|=ks_ctrl;
  if (keys&MK_SHIFT) res|=ks_shift;
  return res;
}

void awwControl::onmouseleave() {
  system->hidehint(this);
  if (savedcursor) {
    SetCursor(savedcursor);
    savedcursor=NULL;
  }
  awsEvent event;if (ctrl) ctrl->onmouseleave(event);
}
void awwControl::onmouseenter() {
  if (hint!="") system->showhint(hint,this);
  updatecursor();
  awsEvent event;if (ctrl) ctrl->onmouseenter(event);
}
void awwControl::updatecursor() {
  if (!win) return;
//  if (cursor!=cr_default) {
  savedcursor=GetCursor();
  SetCursor(system->findcursor(cursor));
// }
//  SetClassLong(win,GCL_HCURSOR,(int)LoadCursor(W_hInstance,"HANDGRAB"));
//  SetCursor(LoadCursor(W_hInstance,"HANDGRAB"));
//  SetCursor(LoadCursor(W_hInstance,IDC_CROSS));
}

awwControl::awwControl(Control *ctrl):ctrl(ctrl) {
  defwndproc=DefWindowProc;
//  tooltipwin=NULL;
  cursor=cr_default;savedcursor=NULL;
  system=(awwSystem*)ctrl->system;
  parent=NULL;visible=enabled=true;
  font=(awwFont*)system->createfont();
  updating=false;firstcreate=true;
  x=y=0;
  w=h=100;
}

void awwControl::destroywnd() {
  for(int i=0;i<childs.count;i++) childs[i]->destroywnd();
  awwObject::destroywnd();parentwin=NULL;
}

void awwControl::onsize(awwMessage &msg,int w,int h) {
  ctrl->onsize(w,h);
  if (parent) parent->resizedchild();
}


void awwControl::updatemypos() {
  if (!win) return;
  RECT r;
  GetWindowRect(win,&r);
  POINT p1,p2;p1.x=r.left;p1.y=r.top;p2.x=r.right;p2.y=r.bottom;
  if (GetWindowLong(win,GWL_STYLE)&WS_CHILD) {
    HWND parenthandle=(HWND)GetWindowLong(win,GWL_HWNDPARENT);
    ScreenToClient(parenthandle,&p1);
    ScreenToClient(parenthandle,&p2);
  }
  x=p1.x;w=p2.x-p1.x;
  y=p1.y;h=p2.y-p1.y;
}
Rect awwControl::getclient() {
  if (!win) return Rect(Point(0,0),Size(w,h));
  RECT r;
  GetClientRect(win,&r);
  return Rect(Point(r.left,r.top),Point(r.right,r.bottom));
}

void awwControl::notifyremove(Component *comp) {
  for(int i=0;i<childs.count;i++) if (childs[i]->ctrl==comp) {
    childs.quickerase(i);
    break;
  }
  if (parent && parent->ctrl==comp) parent=NULL;
/*  if (awwControl* ctrl=dynamic_cast<awwControl*>(comp)) {
    childs.quickremove(ctrl);
    if (ctrl==parent) parent=NULL;
  }*/
/*  if (Control* awsctrl=dynamic_cast<Control*>(comp)) {
    if (awwControl *wc=dynamic_cast<awwControl*>(awsctrl->getreal())) {
      childs.quickremove(wc);
      if (wc==parent) parent=NULL;
    }
  }*/
}

void awwControl::createparams(CreateParams &pars) {
  pars.cid=getid();
  if (parent) pars.parent=parent->win;
  if (visible) pars.style|=WS_VISIBLE;
//  pars.style|=CS_HREDRAW|CS_VREDRAW;
  pars.x=x;pars.y=y;pars.w=w;pars.h=h;
}

void awwControl::ondestroywnd() {
  updatemypos();
  win=NULL;
}

void awwControl::checkcursor() {
  if (!win) return;
  HCURSOR want=system->findcursor(cursor);
  if (want!=GetCursor()) SetCursor(system->findcursor(cursor));
}

void awwControl::onmousedown(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift) {
  system->shiftstate=shift;
  if (ctrl) ctrl->onmousedown(getmousedx()+x,getmousedy()+y,button,shift,msg);
  system->shiftstate=ShiftState();
//  checkcursor();
}
void awwControl::onmouseup(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift) {
  system->shiftstate=shift;
  if (ctrl) ctrl->onmouseup(getmousedx()+x,getmousedy()+y,button,shift,msg);
  system->shiftstate=ShiftState();
//  checkcursor();
}
void awwControl::onmousemove(awwMessage &msg,int x,int y,ShiftState shift) {
  system->shiftstate=shift;
  if (ctrl) ctrl->onmousemove(getmousedx()+x,getmousedy()+y,shift,msg);
  system->shiftstate=ShiftState();
}

LRESULT awwControl::wndproc(HWND hwin,UINT nmsg,WPARAM wparam,LPARAM lparam) {
  typedef signed short MOUSEPOS;
  system->processedwin=parentwin;
  awwMessage msg(hwin,nmsg,wparam,lparam);
  switch (nmsg) {
    case WM_CLOSE:onclose(msg);break;
    case WM_KEYDOWN:case WM_SYSKEYDOWN:{
      awsKey key=wlib::virtkeytokey(wparam,lparam);
//      int key=wlib::virtkeytokey(wparam,lparam);
      onkeydown(msg,key);
    } break;
    case WM_KEYUP:case WM_SYSKEYUP:{
      awsKey key=wlib::virtkeytokey(wparam,lparam);
//      int key=wlib::virtkeytokey(wparam,lparam);
      onkeyup(msg,key);
    } break;
    case WM_PAINT:{
      onpaint(msg);
    } break;
    case WM_SIZE:{
      defproc(msg);
      onsize(msg,LOWORD(lparam),HIWORD(lparam));
      UpdateWindow(win);
    } break;
    case WM_TIMER:{
      ontimer(msg,wparam);
    } break;
    case WM_LBUTTONDOWN:{
      onmousedown(msg,(MOUSEPOS)LOWORD(lparam),(MOUSEPOS)HIWORD(lparam),mb_left,keystoshift(wparam));
    } break;
    case WM_RBUTTONDOWN:{
      onmousedown(msg,(MOUSEPOS)LOWORD(lparam),(MOUSEPOS)HIWORD(lparam),mb_right,keystoshift(wparam));
    } break;
    case WM_MBUTTONDOWN:{
      onmousedown(msg,(MOUSEPOS)LOWORD(lparam),(MOUSEPOS)HIWORD(lparam),mb_middle,keystoshift(wparam));
    } break;
    case WM_LBUTTONUP:{
      onmouseup(msg,(MOUSEPOS)LOWORD(lparam),(MOUSEPOS)HIWORD(lparam),mb_left,keystoshift(wparam));
    } break;
    case WM_RBUTTONUP:{
      onmouseup(msg,(MOUSEPOS)LOWORD(lparam),(MOUSEPOS)HIWORD(lparam),mb_right,keystoshift(wparam));
    } break;
    case WM_MBUTTONUP:{
      onmouseup(msg,(MOUSEPOS)LOWORD(lparam),(MOUSEPOS)HIWORD(lparam),mb_middle,keystoshift(wparam));
    } break;
    case WM_MOUSEWHEEL:{
      onmousewheel(msg,(short)LOWORD(lparam),(short)HIWORD(lparam),(short)HIWORD(wparam),keystoshift(LOWORD(wparam)));
    } break;
    case WM_HSCROLL:{
      onhscroll(msg,LOWORD(wparam),HIWORD(wparam),(HWND)lparam);
      if (awwObject *obj=objfromhandle((HWND)lparam))
        if (awwControl *ctrl=dynamic_cast<awwControl*>(obj))
          ctrl->onhscroll(msg,LOWORD(wparam),HIWORD(wparam),(HWND)lparam);
    } break;
    case WM_VSCROLL:{
      onvscroll(msg,LOWORD(wparam),HIWORD(wparam),(HWND)lparam);
      if (awwObject *obj=objfromhandle((HWND)lparam))
        if (awwControl *ctrl=dynamic_cast<awwControl*>(obj))
          ctrl->onvscroll(msg,LOWORD(wparam),HIWORD(wparam),(HWND)lparam);
    } break;
    case WM_SETFOCUS: {
      onsetfocus(msg);
    } break;
    case WM_KILLFOCUS: {
      onkillfocus(msg);
    } break;
    case WM_MOUSEMOVE: {
      onmousemove(msg,(MOUSEPOS)LOWORD(lparam),(MOUSEPOS)HIWORD(lparam),keystoshift(wparam));
    } break;
    case WM_COMMAND: {
      if (awwControl *ctrl=ctrlfromhandle((HWND)lparam)) ctrl->oncommand(msg,LOWORD(wparam),HIWORD(wparam));
      else oncommand(msg,LOWORD(wparam),HIWORD(wparam));
    } break;
    case WM_ERASEBKGND: {
      onerasebkgnd(msg);
    } break;
    case WM_DESTROY: {
      ondestroywnd();
    } break;
    case WM_CHAR: {
      char ch=(char)wparam;
      if ((unsigned char)ch>=' ') onchar(msg,ch);
    } break;
    case WM_SETCURSOR: {
      HWND wnd=(HWND)wparam;
      int hittest=LOWORD(lparam);
      if (hittest==HTCLIENT)
        if (awwObject *obj=objfromhandle(wnd))
          if (awwControl *ctrl=dynamic_cast<awwControl*>(obj)) {
            if (ctrl->cursor!=cr_default) return 1;
          }
    } break;
//    case WM_COMMAND:{
//      oncommand(msg,LOWORD(wparam));
//    } break;
/*
    case TCN_SELCHANGE:{
      NMHDR *hdr=(NMHDR*)lparam;
      if (awwControl *ctrl=ctrlfromhandle(hdr->hwndFrom)) ctrl->onselchange(msg);
    }*/
  }
  defproc(msg);
  system->processedwin=NULL;
  return msg.res;
}

void awwControl::recreatechilds() {
  actual_id=1;
  for(int i=0;i<childs.count;i++) childs[i]->recreatewnd(),actual_id++;
}
void awwControl::createwindow() {
  awwControl *act=this;
  while (typeid(*act)!=typeid(awwWindow)) act=act->parent;
  parentwin=dynamic_cast<awwWindow*>(act);

  win.handle=createwnd();
  checkw32(win);
  objtohandle(win,this);
  recreatechilds();

  if (specialwndproc()) {
    defwndproc=(WINDOWPROC)GetWindowLong(win,GWL_WNDPROC);
    SetWindowLong(win,GWL_WNDPROC,(LONG)system->stdwndproc);
  }
  SendMessage(win.handle,WM_SETFONT,(WPARAM)font->gethandle(),MAKELPARAM(true,0));
  if (!enabled) EnableWindow(win,enabled);
  updatemypos();
  ctrl->onsize(w,h);
  firstcreate=false;
//  tooltipwin=DoCreateTool NULL;
//  for(int i=0;i<childs.count;i++) if (childs[i]->start) SetFocus(childs[i]->gethandle());
}
awwControl::~awwControl() {
  delete font;
}

Font *awwControl::getfont() {
  return font;
}
void awwControl::setfont(Font *afont) {
  font->assign(afont);
  if (win) SendMessage(win.handle,WM_SETFONT,(WPARAM)font->gethandle(),MAKELPARAM(true,0));
}


HWND awwControl::createwnd() {
  CreateParams pars;
  zerorec(pars);
  createparams(pars);
  return CreateWindowEx(pars.exstyle,pars.classname,pars.windowname,pars.style,
    pars.x,pars.y,pars.w,pars.h,pars.parent,
    pars.menu?pars.menu:(HMENU)pars.cid,W_hInstance,NULL);
//  return CreateWindow("CommonWindowClass",caption.c_str(),0,//WS_OVERLAPPEDWINDOW
//  CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,W_hInstance,NULL);
}

void awwControl::setparent(ParentControl *aparent) {
  if (aparent) setparent(dynamic_cast<awwControl*>(aparent->getreal()));
  else setparent((awwControl*)NULL);
}
void awwControl::setparent(awwControl *aparent) {
  if (parent) parent->removechild(this);
  parent=aparent;
  if (parent) parent->addchild(this);
}

ParentControl *awwControl::getparent() {
  return parent?dynamic_cast<ParentControl*>(parent->ctrl):NULL;
}
void awwControl::removechild(awwControl *ctrl) {
  ctrl->destroywnd();
  childs.quickremove(ctrl);
}
void awwControl::addchild(awwControl *ctrl) {
  if (childs.search(ctrl)<0) {
    childs.add(ctrl);
    if (win) ctrl->recreatewnd();
  }
}



}


