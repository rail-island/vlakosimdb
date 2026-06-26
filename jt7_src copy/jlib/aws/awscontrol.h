#ifndef awscontrolH
#define awscontrolH

#include "awscomponent.h"
#include "awsmodels.h"

namespace aws {

class RealControl;class RealEdit;class RealListBox;class RealPackedList;class RealButton;
class System;class RealListControl;class RealPlace;class RealPageSheet;class RealPageControl;
class RealChildControl;class RealParentControl;class Layout;class RealLabel;
class RealCheckBox;class RealRadioGroup;class RealGroupBox;class RealScrollBox;
class RealMemo;class RealWrapLabel;class Font;class RealTrackBar;

class ParentControl;

//enum SaveType {st_variable,st_child};

struct MouseWheelEvent:NotifyEvent {
  int x,y,delta;
  ShiftState shift;
};

struct Margins {
  int left,top,right,bottom;
};

struct Control:Component {
  virtual ~Control();
  Control(Component *owner,System *system);
  void setpos(Point pos);
  void setsize(Size size);
  virtual void setpos(Point pos,Size size);
  void setpos(int x,int y,int w,int h) {setpos(Point(x,y),Size(w,h));}
  void setpos(int x,int y) {setpos(Point(x,y));}
  void setsize(int w,int h) {setsize(Size(w,h));}
  Point getpos();
  Point getscreenpos();
  Size getsize();
  virtual void setparent(ParentControl *parent);
  ParentControl *getparent();
  Font *getfont();
  void setfocus();
  void setfont(Font *font);
  void beginupdate();
  bool wantanswer();
  void endupdate();
  property_i(hint,string)
  property_i(cursor,Cursor)
  //sdp=System-depended-property
  void setsdp(string name,string value);
  string getsdp(string name);
  bool supportsdp(string name);
  void show();
  void hide();
  Rect getclient();
  void centerto(Rect r);
  virtual void getmargins(Margins &m);
  virtual Size getbestsize();
  virtual void autolayout() {setsize(getbestsize());}
  virtual void clearchilds() {}
  string gettag() {return tag;}
  void settag(string value) {tag=value;}

  property_i(enabled,bool)
  property_i(visible,bool)

  RealControl *getreal() {return realcontrol;}
public:
  virtual void onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  virtual void onmouseup(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  virtual void onmousemove(int x,int y,ShiftState shift,awsEvent &event);
  virtual void onmousewheel(int x,int y,int delta,ShiftState shift,awsEvent &event);
  virtual void onkeydown(awsKey key,awsEvent &event);
  virtual void onkeyup(awsKey key,awsEvent &event);
  virtual void onchar(char ch,awsEvent &event);
  virtual void focused(awsEvent &event);
  virtual void unfocused(awsEvent &event);
  virtual void onmouseleave(awsEvent &event);
  virtual void onmouseenter(awsEvent &event);
  virtual void onsize(int w,int h);
public:
  string getname() {return name;}
  void setname(string value) {name=value;}
  void loadprops(XMLNode xml,Form *form);
//  static SaveType loadsavetype(XMLNode xml,SaveType def);
//  static STRING getxmlsavedprop(XMLNode xml,STRING xmlname,SaveType st);
//  static void setxmlsavedprop(XMLNode xml,STRING xmlname,SaveType st,STRING value);
protected:
  void notifyremove(Component *component);
  void setreal(RealControl *real) {realcontrol=real;}
//  void getprop(STRING name,esObject &res);
//private:
  RealControl *realcontrol;
  string name;
  string tag;
public:
  EventValue ev_mouseup,ev_mousedown,ev_mousemove,ev_focus,ev_killfocus,ev_keyup,ev_keydown,
  ev_mouseleave,ev_mouseenter,ev_resize,ev_mousewheel,ev_char;
};

struct ChildControl:Control {
  ChildControl(Component *owner,System *system):Control(owner,system) {}

  RealChildControl *getreal() {return (RealChildControl *)realcontrol;}
};

struct ParentControl:Control {
  ParentControl(Component *owner,System *system):Control(owner,system) {layout=NULL;denyloadchilds=false;}

  RealParentControl *getreal() {return (RealParentControl *)realcontrol;}

  void getchilds(TCntVec<Control*> &childs);
  void setlayout(Layout *layout);
  void loadprops(XMLNode xml,Form *form);
  void loadlayoutchild(XMLNode xml,Control *child);
  virtual Rect getlayoutrect() {return getclient();}
  void autolayout();
  void loaddata(XMLNode xml);
  void savedata(XMLNode xml);
  void relayout();
  void clearchilds();
private:
  Layout *layout;
protected:
  bool denyloadchilds;
  void onsize(int w,int h);
  ~ParentControl();
};

struct Edit:ChildControl {
  Edit(Component *owner,System *system);
  property_i(text,string)
  property_i(modified,bool)

  RealEdit *getreal() {return (RealEdit *)realcontrol;}
  void loadprops(XMLNode xml,Form *form);
  void selectall();
  void dochange() {donotify("onchange",ev_change.get());}

  void autolayout();

  void loaddata(XMLNode xml) {ChildControl::loaddata(xml);if (isroot(xml)) settext(xml.getvalue(getroot(xml)));}
  void savedata(XMLNode xml) {ChildControl::savedata(xml);if (isroot(xml)) xml.setvalue(getroot(xml),gettext());}

  EventValue ev_change;
};

struct Label:ChildControl,LabelImplementation {
  Label(Component *owner,System *system);
  property_i(text,string)

  RealLabel *getreal() {return (RealLabel *)realcontrol;}
  void loadprops(XMLNode xml,Form *form);

//  string xmlname;
  string model;
  void changed_model();
protected:
  void loaddata(XMLNode xml) {ChildControl::loaddata(xml);if (getroot(xml).isvalue()) settext(xml.getvalue(getroot(xml)));}
};

struct CheckBox:ChildControl {
  CheckBox(Component *owner,System *system);
  property_i(text,string)
  property_i(checked,bool)
  RealCheckBox *getreal() {return (RealCheckBox *)realcontrol;}
  void loadprops(XMLNode xml,Form *form);
  void autolayout();

  void loaddata(XMLNode xml) {ChildControl::loaddata(xml);if (isroot(xml)) setchecked(xml.getvalue(getroot(xml)).ivalue());}
  void savedata(XMLNode xml) {ChildControl::savedata(xml);if (isroot(xml)) xml.setvalue(getroot(xml),itos(getchecked()));}

  void doclick() {donotify("onclick",ev_click.get());}
  EventValue ev_click;
};

struct RadioGroup:ChildControl {
  RadioGroup(Component *owner,System *system);
  property_i(text,string)
  property_i(cur,int)
  property_i(items,string)

  RealRadioGroup *getreal() {return (RealRadioGroup *)realcontrol;}
  void loadprops(XMLNode xml,Form *form);

  void loaddata(XMLNode xml) {ChildControl::loaddata(xml);if (isroot(xml)) setcur(xml.getvalue(getroot(xml)).ivalue());}
  void savedata(XMLNode xml) {ChildControl::savedata(xml);if (isroot(xml)) xml.setvalue(getroot(xml),itos(getcur()));}

  void dochange() {donotify("onchange",ev_change.get());}
  EventValue ev_change;
};

struct GroupBox:ParentControl {
  GroupBox(Component *owner,System *system);
  property_i(text,string)

  RealGroupBox *getreal() {return (RealGroupBox *)realcontrol;}
  void loadprops(XMLNode xml,Form *form);
//  void autolayout();
};


struct ListControl:ChildControl,ListImplementation {
  ListControl(Component *owner,System *system):ChildControl(owner,system) {}// {reallist=NULL;}
  property_i(items,string)
  property_i(cur,int)

  RealListControl *getreal() {return (RealListControl *)realcontrol;}

  void loadprops(XMLNode xml,Form *form);

  EventValue ev_change;
  void dochange() {donotify("onchange",ev_change.get());}

  virtual void onchange(awsEvent &event) {if (getmodel()) getmodel()->lm_setcur(getcur());dochange();}

  void updatemodelitems();
  void updatemodelcur() {if (getmodel()) setcur(getmodel()->lm_getcur());}
  void changed_model() {updatemodelitems();}
  void changed_model_cur() {updatemodelcur();}

//  string xmlname;
  string saveas;
  string loaditems;

//  void callmethod(STRING name,esObject &res,esPars &pars) {}
//  void getprop(STRING name,esObject &res);
  void loaddata(XMLNode xml);
  void savedata(XMLNode xml);
};

struct ListBox:ListControl {
  object_i(ListBox)
  property_i(multiselect,bool)
  ///indexy oddelene ';'
  property_i(selected,string)
  void loadprops(XMLNode xml,Form *form);
};

struct PackedList:ListControl {
  PackedList(Component *owner,System *system);
  property_i(dropdowncount,int)

  RealPackedList *getreal() {return (RealPackedList *)realcontrol;}
//  RealPackedList *getreal() {return realpackedlist;}
private:
//  RealPackedList *realpackedlist;
//protected:
  //void setreal(RealPackedList *real);
};

struct TrackBar:ChildControl {
  TrackBar(Component *owner,System *system);

  RealTrackBar *getreal() {return (RealTrackBar *)realcontrol;}
  void loadprops(XMLNode xml,Form *form);

  property_i(min,int)
  property_i(max,int)
  property_i(cur,int)
  property_i(vertical,bool)
  property_i(frequency,int)


  void loaddata(XMLNode xml) {ChildControl::loaddata(xml);if (isroot(xml)) setcur(xml.getvalue(getroot(xml)).ivalue());}
  void savedata(XMLNode xml) {ChildControl::savedata(xml);if (isroot(xml)) xml.setvalue(getroot(xml),itos(getcur()));}

  EventValue ev_change;
  void dochange() {donotify("onchange",ev_change.get());}
};

struct Button:ChildControl {
  Button(Component *owner,System *system);
  property_i(text,string)
  void autolayout();

  RealButton *getreal() {return (RealButton *)realcontrol;}
  void loadprops(XMLNode xml,Form *form);
  EventValue ev_click;
  virtual void doclick() {donotify("onclick",ev_click.get());}
};

struct Place:ParentControl {
  Place(Component *owner,System *system);

  RealPlace *getreal() {return (RealPlace *)realcontrol;}
protected:
  //void setreal(RealPlace *real);
//private:
//  RealPlace *realplace;
};

class PageControl;
struct PageSheet:ParentControl {
  PageSheet(PageControl *owner);
  property_i(text,string)
  RealPageSheet *getreal() {return (RealPageSheet *)realcontrol;}
  void loadprops(XMLNode xml,Form *form);
};

struct PageControl:ChildControl {
  PageControl(Component *owner,System *system);
  RealPageControl *getreal() {return (RealPageControl *)realcontrol;}
  void loadprops(XMLNode xml,Form *form);
  void loaddata(XMLNode xml);
  void savedata(XMLNode xml);
  property_i(cur,int)
  void getpages(TCntVec<PageSheet*> &pages);

  void dochange() {donotify("onchange",ev_change.get());}
  EventValue ev_change;
};

struct ScrollBox:ParentControl {
  ScrollBox(Component *owner,System *system);
  RealScrollBox *getreal() {return (RealScrollBox *)realcontrol;}
  property_i(scrollpos,Point)

  bool scrollinview(const Rect &r);
  bool scrollinview(Control *ctrl) {return scrollinview(Rect(ctrl->getpos(),ctrl->getsize()));}

  EventValue ev_vscroll,ev_hscroll;
  virtual void dovscroll() {donotify("onvscroll",ev_vscroll.get());}
  virtual void dohscroll() {donotify("onhscroll",ev_hscroll.get());}
};

struct Memo:ChildControl {
  object_i(Memo)
  property_i(text,string)
  property_i(cur,int)
  property_i(modified,bool)
  void loadprops(XMLNode xml,Form *form);
  void dochange() {donotify("onchange",ev_change.get());}

//  string xmlname;
protected:
  void loaddata(XMLNode xml) {ChildControl::loaddata(xml);settext(xml.getvalue(getroot(xml)));}
  void savedata(XMLNode xml) {ChildControl::savedata(xml);xml.setvalue(getroot(xml),gettext());}
//void loaddata(XMLNode xml) {ChildControl::loaddata(xml);if (xmlname!="") settext(xml[xmlname].gettext());}
//void savedata(XMLNode xml) {ChildControl::savedata(xml);if (xmlname!="") xml.forcesub(xmlname).settext(gettext());}
  EventValue ev_change;
};

struct WrapLabel:ChildControl,LabelImplementation {
  object_i(WrapLabel)
  property_i(text,string)
  void loadprops(XMLNode xml,Form *form);

//  string xmlname;
  string model;
  void changed_model();
protected:
//  void loaddata(XMLNode xml) {ChildControl::loaddata(xml);if (xmlname!="") settext(xml[xmlname].gettext());}
};

}

#endif
