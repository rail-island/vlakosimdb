#include <winlib.h>
#pragma hdrstop

#include "awwwindow.h"
#include "awwsystem.h"

namespace aww {

void awwWindow::close() {
  if (system->processedwin==this) system->processedwin=NULL;
  if (win) WWindow(win).close();
}

void awwWindow::onclose(awwMessage &msg) {
  _denyclose=false;
  if (system->processedwin==this) system->processedwin=NULL;
  awwControl::onclose(msg);
  awswin->onclose();
  if (_denyclose) {
    msg.handled=true;
    return;
  }
  destroywnd();
  visible=false;
  system->closedwindow((Window*)ctrl);
  focused_ctrl=NULL;
//  awswin->onclose();
}

void awwWindow::updatemystate() {
  WINDOWPLACEMENT wp;
  wp.length=sizeof(wp);
  if (GetWindowPlacement(win,&wp)) {
    if (wp.showCmd==SW_SHOWNORMAL or wp.showCmd==SW_SHOW or wp.showCmd==SW_RESTORE) state=ws_normal;
    if (wp.showCmd==SW_SHOWMAXIMIZED or wp.showCmd==SW_MAXIMIZE) state=ws_maximized;
    if (wp.showCmd==SW_SHOWMINIMIZED or wp.showCmd==SW_MINIMIZE) state=ws_minimized;
  }
}

void awwWindow::minimizewin(bool minimizethis) {
  if (system->getmainwindow()==awswin) system->minimize(),minimizethis=false;
  else if (modalparent) {
    modalparent->minimizewin(true);
    minimizethis=false;
  }
  if (minimizethis) ShowWindow(win,SW_MINIMIZE);
}

void awwWindow::onsize(awwMessage &msg,int w,int h) {
  awwControl::onsize(msg,w,h);
  if (msg.wparam==SIZE_MINIMIZED) {
    minimizewin();
  }
  if (msg.wparam==SIZE_RESTORED) {
    SetActiveWindow(win);
  }
}

void awwWindow::show() {
  wanthandle();awwControl::show();
  SetActiveWindow(win);
}

/*
void awwWindow::defaultshow() {
  if (win) win.show();
  else {
    defaultpos=true;
    show();
    defaultpos=false;
  }
}
*/

void awwWindow::oncommand(awwMessage &msg,int com,int notify) {
  awwControl::oncommand(msg,com,notify);
  if (menu and menu->runcommand(com)) msg.handled=true;
}
void awwWindow::onkeydown(awwMessage &msg,awsKey key) {
  awwControl::onkeydown(msg,key);
  if (menu and menu->keydown(key)) msg.handled=true;
}

void awwWindow::notifyremove(Component *obj) {
  awwControl::notifyremove(obj);
  if ((awwMenuItem*)obj==menu) menu=NULL;
}
void awwWindow::setmenu(aws::MenuItem *amenu) {
  menu=dynamic_cast<awwMenuItem*>(amenu);
  updatemenu();
/*  menu=amenu;
  awwMenuBar *wm=dynamic_cast<awwMenuBar*>(menu);
  if (wm) wmenu=&wm->menu;else menu=NULL;
  updatemenu();*/
}
void awwWindow::updatemenu() {
  if (menu and win) SetMenu(win,menu->gethandle());
}
void awwWindow::updatestate() {
  if (visible and win) {
    if (state==ws_normal) ShowWindow(win,SW_RESTORE);
    if (state==ws_maximized) ShowWindow(win,SW_MAXIMIZE);
    if (state==ws_minimized) ShowWindow(win,SW_MINIMIZE);
  }
}
void awwWindow::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  strcpy(pars.classname,"awwWindow");
  strcpy(pars.windowname,text.c_str());
  if (style=="simple") {
    pars.style=WS_POPUP|WS_VISIBLE;
  } else if (style=="tool") {
    pars.exstyle|=WS_EX_TOOLWINDOW;
  } else {
    pars.style=WS_OVERLAPPEDWINDOW;
  }
  pars.exstyle|=WS_EX_CONTROLPARENT;
  if (firstcreate) {
    if (position==wp_default) pars.x=pars.y=pars.w=pars.h=CW_USEDEFAULT;
    if (position==wp_centered) {
      int xs=GetSystemMetrics(SM_CXMAXIMIZED),ys=GetSystemMetrics(SM_CYMAXIMIZED);
      pars.x=xs/2-w/2;pars.y=ys/2-h/2;
    }
  }
  pars.cid=0;
  pars.menu=menu?menu->gethandle():NULL;
  pars.parent=system->gethandle();
}
void awwWindow::createwindow() {
  awwControl::createwindow();
  system->createdwindow((Window*)ctrl);
  updatestate();
  if (isclientsize) {
    setsize(getsize()+getframesize());
  }
  if (focused_ctrl) focused_ctrl->setfocus(),focused_ctrl=NULL;
}

Size awwWindow::getclientsize() {
  if (win) {
    RECT r;
    GetClientRect(win,&r);
    return Size(r.right-r.left+1,r.bottom-r.top+1);
  }
  return getsize();
}
Size awwWindow::getframesize() {
  if (win) {
    Size cs=getclientsize(),s=getsize();
    return Size(s.w-cs.w,s.h-cs.h);
  }
  return Size(0,0);
}

void awwWindow::setclientsize(Size size) {
  if (win) {
    Size cs=getclientsize(),s=getsize();
    setpos(getpos(),Size(size.w+s.w-cs.w,size.h+s.h-cs.h));
  } else {
    isclientsize=true;
    w=size.w;h=size.h;
  }
}

awwWindow::awwWindow(Window *ctrl):awwControl(ctrl),awswin(ctrl) {
  defwndproc=NULL;isclientsize=false;
  menu=NULL;//defaultpos=false;
  position=wp_default;state=ws_normal;
  focused_ctrl=NULL;
  modalparent=NULL;
}

void awwWindow::showmodal() {
  HWND dis=NULL;
  HWND focusctrl=GetFocus();
  awwControl *fc=dynamic_cast<awwControl*>(objfromhandle(focusctrl));
  modalparent=system->processedwin;
  if (system->processedwin) dis=system->processedwin->win;
  if (dis) EnableWindow(dis,false);
  show();
  while (visible) system->answer();
  modalparent=NULL;
  if (dis) {
    EnableWindow(dis,true),SetActiveWindow(dis);
  }
  if (fc) fc->setfocus();
}



}


