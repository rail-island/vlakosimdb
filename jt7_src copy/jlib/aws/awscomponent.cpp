#include <jlib.h>
#pragma hdrstop

#include "awscomponent.h"
#include "awssystem.h"
#include <gram/gmanal.h>

namespace aws {

rtti_define(Component)
rtti_define(Canvas)
rtti_define(Control)
rtti_define(Window)
rtti_define(Color);
rtti_define(PaintEvent);
rtti_define(Key);
rtti_define(CharEvent);
rtti_define(MouseEvent);
rtti_define(MouseMoveEvent);
rtti_define(KeyEvent);
rtti_define(MouseButton);
rtti_define(ShiftState);

void EventHandler::on_defaulthandler(BaseEvent &event) {
  switch (event.type) {
    case et_notify:on_notify((NotifyEvent&)event);break;
    case et_mousedown:on_mousedown((MouseEvent&)event);break;
    case et_mouseup:on_mouseup((MouseEvent&)event);break;
    case et_mousemove:on_mousemove((MouseMoveEvent&)event);break;
    case et_mouseenter:on_mouseenter((NotifyEvent&)event);break;
    case et_mouseleave:on_mouseleave((NotifyEvent&)event);break;
    case et_keydown:on_keydown((KeyEvent&)event);break;
    case et_keyup:on_keyup((KeyEvent&)event);break;
    case et_paint:on_paint((PaintEvent&)event);break;
    case et_command:on_command((CommandEvent&)event);break;
//    case et_call:on_call((CallEvent&)event);break;
  }
  if (event.value) on_do(event);
}

void awsHaveProps::setprop(awsProp *prop) {
  deleteprop(prop->name);
  props.add(prop);
}

void awsHaveProps::deleteprop(STRING name) {
  int idx=props.psearchi(name);
  if (idx>=0) props.erase(idx);
}

/*
void MenuBar::loadprops(XMLNode xml,Form *form) {
  Component::loadprops(xml,form);
}
*/

string Component::getrootasstr(XMLNode xml) {
  if (root.begins("?")) {
    gram::Analyser a(root.c_str());
    a.f_char('?');
    string val;
    if (*a=='>') {
      a++;val=xml.getname();
    } else {
      string var=a.readto(">",true);
      val=xml.svar(var);
    }
    while (*a) {
      string vari=a.readto("=",true);
      string vali=a.readto(";",true);
      if (vari==val) return vali;
    }
    return "";
  }
  return root;
}

void Component::loadprops(XMLNode xml,Form *form) {
//  ev_command.load(xml,"oncommand");
/*for(int i=0;i<xml.vars.count;i++) {
    VarRecord *r=xml.vars[i];
    if (r->name.begins("on")) events.setvar(r->name,r->value);
  }
*/
  root=xml.svar("root");
  for(int i=0;i<xml.varcount();i++) if (xml.getvarname(i).begins("sdp."))
    setsdp(xml.getvarname(i).c_str()+4,xml.getvar(i));
}
void Component::do_dispatch(BaseEvent &event) {
  for(int i=0;i<handlers.count;i++) handlers[i]->on_defaulthandler(event);
  for(int i=0;i<props.count;i++) props[i]->doevent(event);
}

void Component::docommand(string command) {
  CommandEvent ev;
  EventValue val;val.setvalue(command);
  ev.value=val.get();// ev_command.get();
  ev.type=et_command;ev.name="oncommand";ev.event=NULL;
  ev.command=command;
  do_event(ev);
}
void Component::donotify(const char *name,EventValue *value) {
  NotifyEvent ev;
  donotify(ev,name,value);
}
void Component::donotify(NotifyEvent &ev,const char *name,EventValue *value) {
  ev.type=et_notify;ev.value=value;ev.name=name;
  ev.event=NULL;
  do_event(ev);
}


void Component::notifyremove(Component *obj) {
  slaves.quickremove(obj);
  for(int i=0;i<slaves.count;i++) slaves[i]->notifyremove(obj);
}

void Component::destroy() {
  if (system) system->destroycomponent(this);
  else delete this;
}

Component::~Component() {
  while (slaves.count>0) delete slaves[0];
  if (system) system->notifyremove(this);
}

Component::Component(Component *owner,System *system):owner(owner),system(system) {
  if (owner) owner->addslave(this);
}

void Component::foreach(void *param,void (*proc)(void *param,Component *comp)) {
  proc(param,this);
  for(int i=0;i<slaves.count;i++) slaves[i]->foreach(param,proc);
}

void init_aws_rtti() {
  using namespace rtti;
  rtti_impl(Component)
  rtti_impl(Canvas)
  rtti_impl(Control)
  rtti_impl(Window)
  rtti_impl(Color);
  rtti_impl(PaintEvent);
  rtti_impl(Key);
  rtti_impl(CharEvent);
  rtti_impl(MouseEvent);
  rtti_impl(MouseMoveEvent);
  rtti_impl(KeyEvent);
  rtti_impl(ShiftState);
  rtti_impl(MouseButton);

  ComponentClass->init_ptr("Component");
  CanvasClass->init_ptr("Canvas");
  ControlClass->init_ptr("Control");
  WindowClass->init_ptr("Window");
  ColorClass->init_prim<int>("Color");
  KeyEventClass->init_prim<awsKey>("Key");
  PaintEventClass->init_rec<PaintEvent>("PaintEvent");
  CharEventClass->init_rec<CharEvent>("CharEvent");
  MouseEventClass->init_rec<MouseEvent>("MouseEvent");
  MouseMoveEventClass->init_rec<MouseMoveEvent>("MouseMoveEvent");
  KeyEventClass->init_rec<KeyEvent>("KeyEvent");
  ShiftStateClass->init_prim<ShiftState>("ShiftState");
  MouseButtonClass->init_prim<MouseButton>("MouseButton");

  PaintEventClass->init_rec<PaintEvent>("PaintEvent");

  rtti_def_fld(PaintEvent,canvas,Canvas);
  rtti_def_fld(CharEvent,ch,Char);
  rtti_def_fld(KeyEvent,key,Key);

  rtti_def_fld(MouseEvent,x,Int);
  rtti_def_fld(MouseEvent,y,Int);
  rtti_def_fld(MouseEvent,shift,ShiftState);
  rtti_def_fld(MouseEvent,button,MouseButton);

  rtti_def_fld(MouseMoveEvent,x,Int);
  rtti_def_fld(MouseMoveEvent,shift,ShiftState);
  rtti_def_fld(MouseMoveEvent,y,Int);
}

}


