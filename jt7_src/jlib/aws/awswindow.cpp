#include <jlib.h>
#pragma hdrstop

#include "awswindow.h"
#include "awssystem.h"
#include "awsreal.h"
#include "awsform.h"

namespace aws {

object_d(Window,ParentControl)
property_d(Window,state,WindowState)
property_d(Window,position,WindowPosition)
property_d(Window,clientsize,Size)
property_d(Window,style,string)

string Window::gettext() {return getreal()->gettext();}
void Window::settext(string value) {getreal()->settext(lang_text_ch(value));}
//property_d(Window,text,string)

void Window::onkeydown(awsKey key,awsEvent &event) {
  if (key==kt_esc && closeonesc) close();
  if (key==kt_enter) donotify("onok",ev_ok.get());
  ParentControl::onkeydown(key,event);
}

void Window::loadprops(XMLNode xml,Form *form) {
  ParentControl::loadprops(xml,form);
  if (xml.isvar("state")) {
    string state=xml.svar("state");
    if (state=="max" or state=="maximized") setstate(ws_maximized);
    if (state=="min" or state=="minimized") setstate(ws_minimized);
    if (state=="normal") setstate(ws_normal);
  }
  if (xml.isvar("position")) {
    string pos=xml.svar("position");
    if (pos=="designed" or pos=="normal") setposition(wp_normal);
    if (pos=="default" or pos=="system") setposition(wp_default);
    if (pos=="center" or pos=="centered") setposition(wp_centered);
  }
  autoload=xml.svar("autoload");
  setstyle(xml.svar("style"));
  settext(xml.svar("text"));
  Size s=getclientsize();
  if (xml.isvar("clientw")) s.w=xml.ivar("clientw");
  if (xml.isvar("clienth")) s.h=xml.ivar("clienth");
  getreal()->setclientsize(s);
  ev_close.load(xml,"onclose");
  ev_ok.load(xml,"onok");
  if (xml.validvar("menu")) {
//    MenuItem *menu=new MenuItem(form,system);
//    menu->loadprops(system->getrep()->search(xml.svar("menu")),form);
    XMLNode mx=system->findrep(xml.svar("menu"));
    MenuItem *menu=system->createmenuitem(form);
    menu->loadprops(mx,form);
//    menu->addhandler(form);
    setmenu(menu);
  }
  closeonesc=xml.ivar("closeonesc");
  if (autoload!="") loaddata(system->getopt().getat(XMLPath(autoload)));
}
void Window::showmodal() {
  getreal()->showmodal();
}
void Window::close() {
  getreal()->close();
}
void Window::denyclose() {
  getreal()->denyclose();
}
void Window::onclose() {
  donotify("onclose",ev_close.get());
  if (autoload!="") savedata(system->getopt().getat(XMLPath(autoload)));
}


/*
void Window::show() {
  realwin->show();
}

void Window::maximize() {
  realwin->maximize();
}
void Window::minimize() {
  realwin->minimize();
}
*/
void Window::setmenu(MenuItem *menu) {
  getreal()->setmenu(menu);
}


}


