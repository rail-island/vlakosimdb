#ifndef awscomponentH
#define awscomponentH

#include <ext/xmlnode.h>
#include "awsdefs.h"
//#include <es/esall.h>
#include <com/jreflect.h>

#define property_i(name,type) type get##name();void set##name(type value);

#define property_d(object,name,type) \
type object::get##name() {return getreal()->get##name();}\
void object::set##name(type value) {getreal()->set##name(value);}

#define property_a(name,type) virtual type get##name() {xabstract;} virtual void set##name(type value) {xabstract;}

#define object_i(object)   object(Component *owner,System *system);Real##object *getreal() {return (Real##object *)realcontrol;}
#define object_d(object,ancestor) object::object(Component *owner,System *system):ancestor(owner,system) {setreal(system->create##object(this));}

namespace aws {
class System;class Component;class Form;

rtti_intf(Component);
rtti_intf(Canvas);
rtti_intf(Control);
rtti_intf(Window);
rtti_intf(Color);
rtti_intf(Key);
rtti_intf(ShiftState);
rtti_intf(MouseButton);
rtti_intf(PaintEvent);
rtti_intf(CharEvent);
rtti_intf(MouseEvent);
rtti_intf(MouseMoveEvent);
rtti_intf(KeyEvent);


struct EventValue {
  string value;//button1click...
  void load(XMLNode xml,const char *name) {value=xml.svar(name);}
  void setvalue(string avalue) {value=avalue;}
  virtual bool valid() {return value!="";}
  EventValue *get() {return valid()?this:NULL;}
private:
  void operator &();
};
enum EventType {et_notify,et_custom,et_mousedown,et_mouseup,et_keydown,et_keyup,et_mousemove,
                et_mouseenter,et_mouseleave,et_paint,et_command};//,et_call};
enum MouseButton {mb_left,mb_right,mb_middle};

struct awsEvent {
  virtual void done() {}
};

struct BaseEvent {
  rtti::Class *classtype;
  EventValue *value;
  Component *sender;
  EventType type;
  awsEvent *event;
  const char *name;//onclick...
  bool isvalue(const char *name) {return value && value->value==name;}
  bool operator ==(const char *name) {return isvalue(name);}
  bool istype(const char *aname) {return eqstr(name,aname);}
  bool operator () (const char *aname) {return istype(aname);}
  bool operator () (Component *asender) {return sender==asender;}
  bool operator () (Component *asender,const char *aname) {return sender==asender && eqstr(name,aname);}
  STRING getvalue() {return value?value->value.c_str():"";}
  bool iscall() {return strchr(getvalue(),'(');}
  BaseEvent() {classtype=NULL;}
};
struct MouseMoveEvent:BaseEvent {
  int x,y;
  ShiftState shift;
  MouseMoveEvent() {classtype=MouseMoveEventClass;}
};
struct MouseEvent:BaseEvent {
  int x,y;
  MouseButton button;
  ShiftState shift;
  MouseEvent() {classtype=MouseEventClass;}
};
struct KeyEvent:BaseEvent {
  awsKey key;
  KeyEvent() {classtype=KeyEventClass;}
};
class Canvas;
struct PaintEvent:BaseEvent {
  Canvas *canvas;
  PaintEvent() {classtype=PaintEventClass;}
};
struct CommandEvent:BaseEvent {
  string command;
};
struct NotifyEvent:BaseEvent {
  NotifyEvent() {type=et_notify;}
};
struct CharEvent:NotifyEvent {
  char ch;
  CharEvent() {classtype=CharEventClass;}
};
/*struct CallEvent:BaseEvent {
  STRING fnname;
  esPars pars;
  CallEvent():fnname("") {}
  bool operator ==(const char *name_type) {return pars.isfn(fnname,name_type);}
  int ipar(int index) {return pars[index].ivalue;}
  void done() {}
//private:
//  bool handled;
};
*/


struct EventHandler {
  virtual void on_notify(NotifyEvent &event) {}
  virtual void on_mousedown(MouseEvent &event) {}
  virtual void on_mouseup(MouseEvent &event) {}
  virtual void on_mousemove(MouseMoveEvent &event) {}
  virtual void on_mouseleave(NotifyEvent &event) {}
  virtual void on_mouseenter(NotifyEvent &event) {}
  virtual void on_keydown(KeyEvent &event) {}
  virtual void on_keyup(KeyEvent &event) {}
  virtual void on_paint(PaintEvent &event) {}
  virtual void on_command(CommandEvent &event) {}
  virtual void on_do(BaseEvent &event) {}//je pusten, pokud evemt.value!=NULL
//  virtual void on_call(CallEvent &ev) {}
  virtual void on_defaulthandler(BaseEvent &event);
};

struct awsProp {
  EventHandler *handler;
  char name[0x20];
  awsProp(STRING aname) {strcpy(name,aname);handler=NULL;}
  bool match(STRING aname) {return aname==name;}
  void doevent(BaseEvent &event) {if (handler) handler->on_defaulthandler(event);}

  virtual ~awsProp() {}
};

struct awsHaveProps {
  void setprop(awsProp *prop);
  void deleteprop(STRING name);
  awsProp *searchprop(STRING name) {return props.psearchv(name);}
protected:
  IObjArray<awsProp> props;
};

struct Component:awsHaveProps {
  System *system;
  virtual ~Component();
  Component(Component *owner,System *system);
  void addhandler(EventHandler *handler) {if (handlers.search(handler)<0) handlers.add(handler);}
  void removehandler(EventHandler *handler) {handlers.remove(handler);}
  void foreach(void *param,void (*proc)(void *param,Component *comp));
  int slavecnt() {return slaves.count;}
  Component *getslave(int index) {return slaves[index];}
  virtual void setsdp(string name,string value) {}
  virtual string getsdp(string name) {return "";}
  virtual bool supportsdp(string name) {return false;}
  void destroy();
protected:
  Component *owner;
  void addslave(Component *obj) {slaves.add(obj);}
  virtual void notifyremove(Component *obj);
  TCntVec<Component*> slaves;
  TCntVec<EventHandler*> handlers;
//  EventValue ev_command;
//  VarList events;
public:
  ///vybere root, pokud zacina ?, vybira se dle hodnoty atributu v xml
  XMLPath getroot(XMLNode xml) {return XMLPath(getrootasstr(xml));}
  XMLPath getroot() {return XMLPath(root);}
  string getrootasstr(XMLNode xml);
  bool isroot(XMLNode xml) {return getrootasstr(xml)!="";}
  void setroot(string aroot) {root=aroot;}
  void do_event(BaseEvent &event) {event.sender=this;do_dispatch(event);}
  void do_dispatch(BaseEvent &event);
//  void setevent(string name,string value) {events.setvar(name,value);}
//  void doevent(string name,string value);
//  void doevent(string name) {doevent(name,events.findvalue(name));}
  virtual void loadprops(XMLNode xml,Form *form);
  void docommand(string command);//do_event(CommandEvent)
  void donotify(const char *name,EventValue *value);
  void donotify(NotifyEvent &event,const char *name,EventValue *value);
  virtual void loaddata(XMLNode xml) {}
  virtual void savedata(XMLNode xml) {}
//  virtual void callmethod(STRING name,esObject &res,esPars &pars) {}
//  virtual void getprop(STRING name,esObject &res) {}
private:
  string root;
};


/*
struct MenuBar:Component {
  MenuBar(Component *owner,System *system):Component(owner,system) {}
  void loadprops(XMLNode xml,Form *form);
//  virtual void sethandler(EventHandler *handler)=0;
};
*/


struct awsError:JException {
  string msg;
  string getmsg() {return msg;}
  awsError(string msg):msg(msg) {}
};

struct Timer:Component {
  EventValue ev_timer;
  void ontimer() {donotify("ontimer",ev_timer.get());}
  virtual void setinterval(float secs)=0;
  virtual float getinterval()=0;
  virtual void stop()=0;
  virtual void restart()=0;
  virtual bool isrunning()=0;
  Timer(Component *owner,System *system):Component(owner,system) {}
  virtual ~Timer() {}
};

struct Console:Component {
  virtual void print(string what)=0;
  virtual string readln()=0;
  virtual void wait()=0;
  virtual void close()=0;
  Console(Component *owner,System *system):Component(owner,system) {}
};

struct OpenDialog {
  //if (multiselect) return "";
  virtual string getfile()=0;
  virtual void setfile(string value)=0;
  virtual bool getmultiselect()=0;
  virtual void setmultiselect(bool value)=0;
  virtual bool execute()=0;
  //files:file\nfile\n...
  virtual string getfiles()=0;
  virtual string getinitialdir()=0;
  virtual void setinitialdir(string value)=0;
  //filter:"Text files~*.txt|All files~*.*"
  virtual string getfilter()=0;
  virtual void setfilter(string value)=0;
};

struct SaveDialog {
  virtual string getfile()=0;
  virtual void setfile(string value)=0;
  virtual bool execute()=0;
  virtual string getinitialdir()=0;
  virtual void setinitialdir(string value)=0;
  virtual string getfilter()=0;
  virtual void setfilter(string value)=0;
};

struct ColorDialog {
  virtual bool execute()=0;
  virtual int getcolor()=0;
  virtual void setcolor(int color)=0;
};

void init_aws_rtti();

}

#endif
