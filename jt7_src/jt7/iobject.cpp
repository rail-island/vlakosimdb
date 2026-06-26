#include <jlib.h>
#pragma hdrstop

#include "iobject.h"

namespace jtyc {

extern bool ignore_visibility=false;

glo::gloModel *jt_loadmodel(PersistentObject *obj) {
  MainXMLNode xml;
  obj->loadxml("index.xml",xml);
  glo::gloModel *res=new glo::gloModel();
  res->load(xml,obj);
  float dx=xml.fvar("dx"),dy=xml.fvar("dy"),dz=xml.fvar("dz");
  if (dx || dy || dz) res->translate(dx,dy,dz);
  return res;
}

glo::gloModel *jt_loadmodel(PersistentObject *parent,string file) {
  PersistentObject *obj=parent->openchildpath(file,true);
  if (obj) return jt_loadmodel(obj);
  return new glo::gloModel();
}



}

