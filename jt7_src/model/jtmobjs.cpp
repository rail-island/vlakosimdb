#include <jlib.h>
#pragma hdrstop

#include "jtmobjs.h"
#include "ismoke.h"


#include <zaba/zbrt.h>
#include <zaba/zbint.h>
#include <aws/awssystem.h>
#include <aws/awspaint.h>
#include <glo/glotextures.h>

namespace zaba {
#include "jtmlib/jtmobjs.h"
}


namespace zaba {
  extern aws::System *awssys;
}

namespace jtyc {
using namespace zaba;
using namespace aws;
using namespace glo;


zbObject *newmodelvariables(Runtime *rt) {
  return new zbModelVariables(new gloVariableList(),rt);
}

zbObject *zbjtStation::complete(int trc,int trl) {
  return new zbStationInstance(getlink()->complete(trc,trl),rt);
}

zbObject *zbjtBridge::complete(int length) {
  return new zbBridgeInstance(getlink()->complete(length),rt);
}

zbObject *zbjtSmoke::create() {
  return new zbSmokeInstance(getlink()->create(),rt);
}


zbObject *loadjtobject(zbPersistentObject *obj,Runtime *rt) {

  PersistentObject *po=obj->getlink();
  MainXMLNode xml;
  po->loadxml("index.xml",xml);
#define load_func(objname,jtClass) \
  if (xml.getname()==#objname) {\
    jtClass *res=new jtClass();\
    res->load(po,xml);\
    zbObject *zbres=new zb##jtClass(res,rt);\
    return zbres;\
  }

load_func(loco,jtMachine)
load_func(busstop,jtBusStop)
load_func(motocar,jtMotocar)
load_func(bridge,jtBridge)
load_func(tunnel,jtTunnel)
load_func(rstation,jtStation)
load_func(building,jtBuilding)
load_func(cab,jtCab)
load_func(aircraft,jtAircraft)
load_func(smoke,jtSmoke)
#undef load_func
  return createVoid(rt);
}

void zbSmokeInstance::render(zbRenderContext *rc) {
  if (rc->rcref->rc) obj->render(*rc->rcref->rc);
}

void zbBridgeInstance::render(float x,float y,float z,zbRenderContext *rc,int dir) {
  if (rc->rcref->rc) obj->render(x,y,z,*rc->rcref->rc,dir);
}

void zbStationInstance::render(float x,float y,float z,zbRenderContext *rc,int dir) {
  if (rc->rcref->rc) obj->render(x,y,z,*rc->rcref->rc,dir);
}

void zbjtCab::render(zbRenderContext *zbrc,zbXMLNode *xml) {
  if (RenderContext *rc=zbrc->getlink()) getlink()->render(*rc,xml->cvalue());
}

zbObject *zbjtObject::getdata() {
  if (object->zipobj) {
    zbPersistentObject *res=new zbPersistentObject(object->zipobj,rt);
    return res;
  }
  return createVoid(rt);
}

}

namespace zaba {
using namespace jtyc;

#include "jtmlib/jtmobjs.cc"
}


