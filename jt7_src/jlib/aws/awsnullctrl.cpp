#include <jlib.h>
#pragma hdrstop

#include "awsnullctrl.h"
#include "awsreal.h"

namespace aws {

struct RealNullControl:RealControl {
  RealNullControl() {parent=NULL;}

  virtual ~RealNullControl() {}
  virtual void notifyremove(Component *obj) {if (obj==parent) parent=NULL;}
  virtual void show() {}
  virtual void hide() {}
  virtual bool isvisible() {return false;}
  virtual void setpos(Point pos,Size size) {}
  virtual Point getpos() {return Point(-1,-1);}
  virtual Point getscreenpos() {return Point(-1,-1);}
  virtual Size getsize() {return Size(0,0);}
  virtual void setparent(ParentControl *aparent) {parent=aparent;}
  virtual ParentControl *getparent() {return parent;}
  virtual Font *getfont() {return NULL;}
  virtual void setfont(Font *font) {}
  virtual void beginupdate() {}
  virtual bool wantanswer() {return false;}
  virtual void endupdate() {}
  virtual Rect getclient() {return Rect(-1,-1,0,0);}
  virtual void getmargins(Margins &m) {m.left=m.right=m.top=m.bottom=0;}
  virtual Size getbestsize() {return getsize();}
  virtual void sethint(string value) {}
  virtual string gethint() {return "";}

  virtual void setsdp(string name,string value) {}
  virtual string getsdp(string name) {return "";}
  virtual bool supportsdp(string name) {return false;}
  virtual void setfocus() {}
private:
  ParentControl *parent;
};

/*
struct RealPolyControl:RealNullControl {
  TCntVec<Control*> subs;
  Point pos;
  Size size;

  void notifyremove(Component *obj) {
    RealNullControl::notifyremove(obj);
    subs.quickremove(dynamic_cast<Control*>(obj));
  }
  void addsub(Control *slave) {subs.add(slave);}

  Point getpos() {return pos;}
  Point getscreenpos() {return Point(-1,-1);}
  Size getsize() {return size;}
  void setparent(ParentControl *aparent) {
    RealNullControl::setparent(aparent);
    for(int i=0;i<subs.count;i++) subs[i]->setparent(aparent);
  }

};

void PolyControl::addsubctrl(Control *slave) {
  getreal()->addsub(slave);
}

PolyControl::PolyControl(Component *owner,System *system):Control(owner,system) {
  setreal(new RealPolyControl());
}
*/

NullControl::NullControl(Component *owner,System *system):Control(owner,system) {
  setreal(new RealNullControl());
}

}


