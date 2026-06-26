#ifndef awschecklistH
#define awschecklistH

#include "awspaint.h"

namespace aws {

rtti_intf(CheckedEvent)

struct CheckedEvent:NotifyEvent {
  int line;int checked;
  CheckedEvent(int aline,bool achecked) {line=aline;checked=achecked;classtype=CheckedEventClass;}
};

struct CheckList:ScrolledPainter {
  CheckList(Component *owner,System *system);
  void onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  void onpaint(Canvas &cnv,awsEvent &event);
  void setitems(string value);
  void onkeydown(awsKey key,awsEvent &event);
  string getitems();
  void onsize(int w,int h);
  int getcur() {return cur;}
  void setcur(int value);
  //0|1 oddelene ';' (1:zaskrtnute,0:prazdne)
  string getchecked();
  void setchecked(string value);
  void loadprops(XMLNode xml,Form *form);
  bool getvisiblechecks() {return drawchecks;}
  void setvisiblechecks(bool value) {drawchecks=value;repaint();}
  void loaddata(XMLNode xml);
  void savedata(XMLNode xml);
private:
  void _check(int line);
  Rect currect() {return Rect(Point(0,cur*linehi),Size(0,linehi));}
  void updateinsize();
  void dochange() {donotify("onchange",ev_change.get());}
  void docheck(int line,bool value) {CheckedEvent ev(line,value);donotify(ev,"oncheck",ev_check.get());}

  Strings items;
  int cur;
  bool drawchecks;
  int linehi;
  TCntVec<bool> checked;
  EventValue ev_change,ev_check;
};

void init_aws_checklist_rtti();

}

#endif
