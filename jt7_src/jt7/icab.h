#ifndef icabH
#define icabH

#include "iobject.h"

namespace jtyc {

struct CabItem {
  virtual ~CabItem() {}
  virtual void render(RenderContext &rc,XMLNode xml) {}
};

struct CabCircle:CabItem {
  RefBase<Texture> tex;
  float startangle,endangle,startvalue,endvalue;
  float x,y;
  float radius,wi;
  XMLPath root;
  void render(RenderContext &rc,XMLNode xml);
  CabCircle(XMLNode xml,PersistentObject *data);
};

struct jtCab:jtDataObject {
  bool isdefault;
  RefBase<Texture> bg;
  float fovy,hi;

  float x,y,w,h;

  IObjArray<CabItem> items;

  void load(PersistentObject *data,XMLNode xml);
  void render(RenderContext &rc,XMLNode xml);
  jtCab() {isdefault=false;fovy=45;hi=0.1;x=y=w=h=0;}
private:
  bool bind(RenderContext &rc,Texture *tex);
};


}

#endif
