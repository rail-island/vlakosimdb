#ifndef awsproplistH
#define awsproplistH

#include "awscontrol.h"
#include "awspaint.h"

namespace aws {

class PropertyList;class PropertyEdit;
struct PropertyListInner:PaintControl {
  PropertyList *proplist;
  PropertyListInner(PropertyList *owner,System *system);
  void updatesize();
  VarList vars;
  int itemhi,valleft,wi;
  void onpaint(Canvas &cnv,awsEvent &event);
  void onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  void showedit(int line);
  void nextprop() {if (editline<vars.count-1) showedit(editline+1);}
  void prevprop() {if (editline>0) showedit(editline-1);}
  void cancel();
  void ok();
  void save();
private:
  PropertyEdit *edit;
  int editline;
  friend class PropertyList;
};

struct PropertyEdit:Edit {
  PropertyListInner *proplist;
  PropertyEdit(PropertyListInner *owner,System *system);
  void unfocused(awsEvent &event) {proplist->showedit(-1);}
  void onkeydown(awsKey key,awsEvent &event);
};

struct PropertyList:ScrollBox {
  PropertyList(Component *owner,System *system);
  void setvars(VarList *vars,bool sort);
  void getvars(VarList *vars) {vars->assign(inner->vars);}
  void update() {inner->editline=-1;inner->updatesize();inner->repaint();}
  VarList &_vars() {return inner->vars;}
  EventValue ev_change;
  void ok() {inner->ok();}
  void cancel() {inner->cancel();}
  void clear() {inner->vars.clear();update();}
  bool getmodified() {return modified;}
  void setmodified(bool value) {modified=value;}
  void modify() {modified=true;}
  void addvar(string name,string value,bool sort);
  void deleteactvar();
private:
  bool modified;
  friend class PropertyListInner;
  PropertyListInner *inner;
protected:
  void loadprops(XMLNode xml,Form *form);
  void dochange() {donotify("onchange",ev_change.get());}
  void onsize(int w,int h) {inner->updatesize();}
};

}

#endif
