#include <jlib.h>
#pragma hdrstop

#include "sdwwindow.h"
#include "sdwsystem.h"
#include <sdl/sdl.h>

namespace sdw {

sdwWindow::sdwWindow(Window *ctrl,sdwSystem *sys):sdwControl(ctrl,sys),menubar(sys) {
  win=ctrl;vo=sys->vo;
  position=wp_default;state=ws_normal;
  focused=NULL;
  w=300;h=200;
  inner=new sdwPlace(ctrl,sys);
  inner->setparent(this);
  moving=false;visible=false;
}

int sdwWindow::menuleft() {return vo->winhborder;}
int sdwWindow::menutop() {return vo->wintopborder+vo->wintitlehi;}

void sdwWindow::setfocus(sdwControl *ctrl) {
  focused=ctrl;
  sys->changedfocus();
}

void sdwWindow::sdwnotifyremove(sdwControl *ctrl) {
  sdwControl::sdwnotifyremove(ctrl);
  if (ctrl==focused) focused=NULL;
}

void sdwWindow::setmenu(MenuItem *value) {
  menu=dynamic_cast<sdwMenuItem*>(value);
  menubar.setmenu(menu);
  resizeinner();
  repaint();
}

void sdwWindow::notifyremove(Component *obj) {
  sdwControl::notifyremove(obj);
  menubar.notifyremove(dynamic_cast<sdwMenuItem*>(obj));
  if (obj==menu) menu=NULL;
}

void sdwWindow::onsize() {
  sdwControl::onsize();
  resizeinner();
  menubar.resize(w-2*vo->winhborder,h-vo->wintopborder-vo->winbotborder);
}

void sdwWindow::resizeinner() {
  int menuhi=menubar.gethi();
  inner->setpos(
    Point(vo->winhborder,vo->wintitlehi+vo->wintopborder+menuhi),
    Size(w-2*vo->winhborder,h-vo->wintitlehi-vo->winbotborder-menuhi)
  );
}
void sdwWindow::setclientsize(Size value) {
  int menuhi=menubar.gethi();
  w=value.w+2*vo->winhborder;
  h=value.h+vo->wintitlehi+vo->wintopborder+menuhi+vo->winbotborder;
  onsize();
}
void sdwWindow::mouseleave() {
  sdwControl::mouseleave();
  menubar.mouseleave();
  paintmenuifneeded();
}
void sdwWindow::bringtofront() {
  sys->bringtofront(this);
}
void sdwWindow::mousedown(int x,int y,MouseButton but,ShiftState shift) {
  sdwControl::mousedown(x,y,but,shift);
  bringtofront();
  menubar.mousedown(x-menuleft(),y-menutop(),but);
  if (y<vo->wintitlehi) moving=true,movorigx=x,movorigy=y;
  paintmenuifneeded();
  sys->activatewin(this);
  int cx1=w-vo->winbuttonsize-2*vo->winbuttonborder,cy1=vo->winbuttonborder,
      cx2=w-vo->winbuttonborder,cy2=vo->wintitlehi-1;
  if (x>=cx1 && x<=cx2 && y>=cy1 && y<=cy2) close();    
}
void sdwWindow::mouseup(int x,int y,MouseButton but,ShiftState shift) {
  sdwControl::mouseup(x,y,but,shift);
  menubar.mouseup(x-menuleft(),y-menutop(),but);
  moving=false;
  paintmenuifneeded();
}
void sdwWindow::mousemove(int x,int y,ShiftState shift) {
  sdwControl::mousemove(x,y,shift);
  menubar.mousemove(x-menuleft(),y-menutop());
  if (moving) {
    this->x+=x-movorigx;
    this->y+=y-movorigy;
    changedpos();
  }
  paintmenuifneeded();
}

sdwWindow::~sdwWindow() {
  sys->removedwindow(this);
  delete inner;
}

void sdwWindow::changedpos() {
  sdwControl::changedpos();
  sys->invalidate();
}

void sdwWindow::draw(sdwCanvas canvas) {
  canvas.frect(0,0,w-1,h-1,vo->clwinbgcolor);
  canvas.frect(vo->winhborder,vo->wintopborder,w-1-2*vo->winhborder,vo->wintitlehi-1,vo->clwintitlebg);
  canvas.text(vo->wintitleleft,vo->wintopborder,title,vo->clwintitletext,font);
  int cx1=w-vo->winbuttonsize-2*vo->winbuttonborder,cy1=vo->winbuttonborder,
      cx2=w-vo->winbuttonborder,cy2=vo->wintitlehi-1;
  canvas.frect(cx1,cy1,cx2,cy2,vo->clwinbuttonbg);
  canvas.line(cx1,cy1,cx2,cy2,vo->clwinclosebut);
  canvas.line(cx1,cy2,cx2,cy1,vo->clwinclosebut);
  sdwControl::draw(canvas);
  menubar.paint(sdwCanvas(canvas,menuleft(),menutop()));
}

void sdwWindow::show() {
  visible=true;
  if (position==wp_centered) {
    x=sys->getw()/2-w/2;
    y=sys->geth()/2-h/2;
  }
  resizeinner();
  repaint();
  onsize();
}

void sdwWindow::showmodal() {
  show();
  while (visible) sys->answer();
}

void sdwWindow::close() {
  deniedclose=false;
  if (win) win->onclose();
  if (deniedclose) return;
  visible=false;
  sys->invalidate(Rect(x,y,w,h));
}

void sdwWindow::repaint() {
  sys->invalidate(Rect(x,y,w,h));
}

void sdwWindow::repaint(const Rect &r) {
  sys->invalidate(r+Point(x,y));
}


}

