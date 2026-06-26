#ifndef awsextctrlsH
#define awsextctrlsH

#include "awspaint.h"
#include "awswindow.h"
#include "awsnullctrl.h"

namespace aws {

class SpeedGroup;

struct SpeedButton:PaintControl {
  SpeedButton(Component *owner,System *system);
  void onmouseup(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  void onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  void onmouseleave(awsEvent &event);
  void onmouseenter(awsEvent &event);
  void onpaint(Canvas &cnv,awsEvent &event);
  string gettext() {return text;}
  void settext(string value) {text=lang_text_ch(value);repaint();}
  void loadprops(XMLNode xml,Form *form);
  void setflat(bool value) {flat=value;repaint();}
  bool getflat() {return flat;}
  Picture *getimage() {return image;}
  void setimage(Picture *aimage) {image=aimage;repaint();}
  void setgroup(SpeedGroup *value);
  void notifyremove(Component *comp);
  void setswitch(bool value) {isswitch=value;}
  bool getswitch() {return isswitch;}
  bool getswitched() {return switched;}
  void setswitched(bool value) {switched=value;repaint();}
  int getcolor() {return color;}
  void setcolor(int value) {color=value;repaint();}
private:
  bool pushed;bool mdown;bool mousein;
  bool switched;
  bool flat;bool isswitch;
  int color;
  string text;
  RefBase<Picture> image;
  SpeedGroup *group;

  void universal_paint(Canvas cnv,int state);//-1:sunken,0:flayt,1:raised
public:
  void doclick();
  EventValue ev_click;
};

struct SpeedGroup:NullControl {
  SpeedGroup(Component *owner,System *system);

  void loadprops(XMLNode xml,Form *form);
  void choose(SpeedButton *but);
  bool ischoosed(SpeedButton *but);
  int getcur();
  void setcur(int index);
private:
  SpeedButton *choosen;
  void notifyremove(Component *comp);
  TCntVec<SpeedButton*> items;
  EventValue ev_change;
};

struct Panel:PaintControl {
  Panel(Component *owner,System *system):PaintControl(owner,system) {}
  void onpaint(Canvas &cnv,awsEvent &event);
  Rect getlayoutrect() {Rect r=PaintControl::getlayoutrect();r.inflate(-2,-2);return r;}
  void getmargins(Margins &m) {m.left=m.right=m.top=m.bottom=2;}
};

struct ConsoleImpl:Window,EventHandler {
  ConsoleImpl(Component *owner,System *system);
  ~ConsoleImpl();
  void print(string what);
  string readln();
  void wait();
private:
  Memo *memo;
  bool reading,waiting;
protected:
  void on_keydown(KeyEvent &ev);
};

struct awsConsole:Console {
  ConsoleImpl *impl;
  awsConsole(Component *owner,System *system):Console(owner,system) {impl=new ConsoleImpl(this,system);}
  void notifyremove(Component *comp) {if (impl==comp) impl=NULL;Console::notifyremove(comp);}
  void print(string what) {if (impl) impl->print(what);}
  string readln() {if (impl) return impl->readln();return string("");}
  void wait() {if (impl) impl->wait();}
  void close() {if (impl) impl->close();}
};

struct LabeledEdit:Place {
  LabeledEdit(Component *owner,System *system);
  void notifyremove(Component *comp);
  void setlabel(string value) {label->settext(value);}
  string getlabel() {return label->gettext();}
  void setvalue(string value) {edit->settext(value);}
  string getvalue() {return edit->gettext();}
  void loadprops(XMLNode xml,Form *form);
  void loaddata(XMLNode xml) {edit->loaddata(xml);}
  void savedata(XMLNode xml) {edit->savedata(xml);}
private:
  Label *label;
  Edit *edit;
};

struct FontButton:Button {
  void edit();
  void doclick() {Button::doclick();edit();}
  FontButton(Component *owner,System *system);
  void loaddata(XMLNode xml);
  void savedata(XMLNode xml);
};

struct PrinterOptions:Button {
  void edit();
  void doclick() {Button::doclick();edit();}
  PrinterOptions(Component *owner,System *system);
  void loaddata(XMLNode xml) {props.assign(xml.getat(getroot(xml)));}
  void savedata(XMLNode xml) {xml.getat(getroot(xml)).assign(props);}
private:
  XMLNode props;
};

class Tree;class Panel;class SettingsNode;

struct Settings:Place,EventHandler {
  Settings(Component *owner,System *system);
  ~Settings();
  void loadprops(XMLNode xml,Form *form);
  void notifyremove(Component *comp);
  void onsize(int w,int h);
  void on_notify(NotifyEvent &ev);
  void clickednode(SettingsNode *node);
  void loaddata(XMLNode xml);
  void savedata(XMLNode xml);
  void createall();
private:
  bool bcreateall;
  Form *form;
  Tree *tree;
  Panel *panel;
  Control *visiblepage;
  XMLNode dsgn;
  XMLNode data;
  void showpage(Control *page);
  friend class SettingsNode;
  SettingsNode *main;
};

}

#endif
