#ifndef gmobileH
#define gmobileH

#include "game0.h"

namespace jtyc {

struct Mobile:GObject {
//  RefBase<Linka> line;
  bool exist;
//  string name;
  Mobile():GObject() {
//    line=aline;
    exist=true;
  }
//  Mobile(BINREAD &fr):GObject(fr) {
//    fr>>line>>exist;
//  }
//  void save(BINWRITE &fw) {
//    GObject::save(fw);
//    fw<<line<<exist;
//  }
  bool match(STRING aname) {return getname()==aname;}
  virtual bool position(fvector3d *res,fvector3d *diff=NULL)=0;
//  string getname() {return name;}
  void watch();
  void drive();
//  void setname(string value,bool addnum=false);
  virtual void run(double deltatime)=0;
  virtual void render(RenderContext &rc)=0;
  virtual void rendertransparent(RenderContext &rc) {}
//  virtual void crash();
//  virtual void click(float x,float y) {}
  virtual bool ismouseon(float x,float y) {return false;}
  virtual void save(XMLNode xml) {xml.setvar("name",getname());}
  Mobile(XMLNode xml) {exist=true;setname(xml.svar("name"));}
  virtual jtCab *getcab() {return NULL;}
  virtual bool getmanual() {return false;}
  virtual void setmanual(bool value) {}
  virtual void drivekeydown(int key) {}
  virtual void wait(float secs) {}
  virtual void octalize() {}
  virtual void delize() {}
  virtual void clearvisibility() {}
  virtual void beforerun() {}
  virtual Train *totrain() {return NULL;}
  virtual XMLNode getcabprops() {return XMLNode::create();}
//  bool ot_isdynamic() {return true;}
};

struct Mobiles:GInstance {
  TCntVec<Mobile*> items;
//  TRCArray<Mobile> items;
//  TCntVec<Mobile*> crashed;

  void render(RenderContext &rc);
  void rendertransparent(RenderContext &rc);
//  void redrawstatic(View *v) {}
//  Mobiles(BINREAD &fr) {fr>>items;game=frgame(fr);}
//  void save(BINWRITE &fw) {fw<<items;}
  void run(double deltatime) {
    for(int i=0;i<items.count;i++) items[i]->run(deltatime);
  }
  void beforerun() {
    for(int i=0;i<items.count;i++) items[i]->beforerun();
  }
  Mobile *search(float x,float y) {
    for(int i=0;i<items.count;i++) if (items[i]->ismouseon(x,y)) return items[i];
    return NULL;
  }
  Mobile *search(string name) {
    return items.psearchv(name.c_str());
  }
  void notifyremove(GInstance *obj);
//  void click(float x,float y) {for(int i=0;i<items.count;i++) items[i]->click(x,y);}
  Mobiles();
  void clearall();
  void init();
  void load(Archive &ar);
  void save(Archive &ar);
  void xmlsave(XMLNode xml);
  void xmlload(XMLNode xml);
  void octalize();
  void delize();
  void clearvisibility();
};



}

#endif
