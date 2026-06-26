#ifndef jtmobjsH
#define jtmobjsH

#include <zaba/zbrt.h>
#include <zaba/zbint.h>
#include <zaba/zbobj.h>
#include <aws/awsglview.h>
#include <aws/awsglmodelview.h>
#include <zaba/zbaws.h>
#include <zaba/zbext.h>
#include <glo/glovars.h>
#include "istatic.h"
#include "imachine.h"
#include "imotocar.h"
#include "ismoke.h"
#include "icab.h"

namespace jtyc {
using namespace zaba;

class zbjtBuilding;class zbBusStop;class zbRenderContext;


struct zbModelVariables:zbObject {
  RefBase<gloVariableList> vars;
  gloVariableList *getlink() {return vars;}
  zbModelVariables(gloVariableList *avars,Runtime *rt,STRING intf="ModelVariables.instance"):zbObject(rt,intf) {vars=avars;}
};

struct zbjtObject:zbObject {
  RefBase<jtDataObject> object;
  jtDataObject *getlink() {return object;}
  virtual string getname() {return getlink()->getname();}
  virtual string gettitle() {return getlink()->gettitle();}
  string getauthor() {return getlink()->author;}
  string getpath() {return getlink()->path;}
  zbjtObject(jtDataObject *obj,Runtime *rt,STRING intf="jtObject.instance"):zbObject(rt,intf) {object=obj;}
  string tostring() {return getname();}
  zbObject *getdata();
};

struct zbjtBuilding:zbjtObject {
//  jtBuilding *type;
  jtBuilding *getlink() {return (jtBuilding*)zbjtObject::getlink();}
//  string getname() {return getlink()->getname();}
  zbjtBuilding(jtBuilding *obj,Runtime *rt,STRING intf="jtBuilding.instance"):zbjtObject(obj,rt,intf) {}
  int getW() {return getlink()->w;}
  int getH() {return getlink()->h;}
};

struct zbjtMachine:zbjtObject {
  jtMachine *getlink() {return (jtMachine*)zbjtObject::getlink();}
  double getmaxSpeed() {return getlink()->maxspeed;}
  double getweight() {return getlink()->weight;}
  double getpower() {return getlink()->power;}
  zbjtMachine(jtMachine *obj,Runtime *rt,STRING intf="jtMachine.instance"):zbjtObject(obj,rt,intf) {}
};

struct zbjtMotocar:zbjtObject {
  jtMotocar *getlink() {return (jtMotocar*)zbjtObject::getlink();}
  double getmaxSpeed() {return getlink()->maxspeed;}
  double getweight() {return getlink()->weight;}
  double getpower() {return getlink()->power;}
  zbjtMotocar(jtMotocar *obj,Runtime *rt,STRING intf="jtMotocar.instance"):zbjtObject(obj,rt,intf) {}
};

struct zbBridgeInstance:zbObject {
  RefBase<BridgeInstance> obj;

  BridgeInstance *getlink() {return obj;}
  zbBridgeInstance(BridgeInstance *obj,Runtime *rt,STRING intf="BridgeInstance.instance"):zbObject(rt,intf),obj(obj) {}
  void render(float x,float y,float z,zbRenderContext *rc,int dir);
};

struct zbStationInstance:zbObject {
  RefBase<StationInstance> obj;

  StationInstance *getlink() {return obj;}
  zbStationInstance(StationInstance *obj,Runtime *rt,STRING intf="StationInstance.instance"):zbObject(rt,intf),obj(obj) {}
  void render(float x,float y,float z,zbRenderContext *rc,int dir);
};

struct zbRenderContext:zbObject {
  RefBase<RenderContextReference> rcref;
  RenderContext *getlink() {return rcref->rc;}
  zbRenderContext(RenderContextReference *rc,Runtime *rt,STRING intf="RenderContext.instance"):zbObject(rt,intf) {rcref=rc;}
};

struct zbjtSmoke:zbjtObject {
  zbjtSmoke(jtSmoke *obj,Runtime *rt,STRING intf="jtSmoke.instance"):zbjtObject(obj,rt,intf) {}
  zbObject *create();
  jtSmoke *getlink() {return (jtSmoke*)zbjtObject::getlink();}
};
struct zbSmokeInstance:zbObject {
  RefBase<SmokeInstance> obj;

  SmokeInstance *getlink() {return obj;}
  zbSmokeInstance(SmokeInstance *obj,Runtime *rt,STRING intf="SmokeInstance.instance"):zbObject(rt,intf),obj(obj) {}
  void render(zbRenderContext *rc);
  void translate(float x,float y,float z) {getlink()->translate(fvector3d(x,y,z));}
  void iterate(float deltatime,float x,float y,float z,float vx,float vy,float vz) {getlink()->iterate(deltatime,fpoint3d(x,y,z),fvector3d(vx,vy,vz));}
};


struct zbjtBridge:zbjtObject {
  zbjtBridge(jtBridge *obj,Runtime *rt,STRING intf="jtBridge.instance"):zbjtObject(obj,rt,intf) {}
  zbObject *complete(int length);
  jtBridge *getlink() {return (jtBridge*)zbjtObject::getlink();}
};
struct zbjtTunnel:zbjtObject {
  zbjtTunnel(jtTunnel *obj,Runtime *rt,STRING intf="jtTunnel.instance"):zbjtObject(obj,rt,intf) {}
};

struct zbjtDepo:zbjtObject {
  zbjtDepo(jtDepo *obj,Runtime *rt,STRING intf="jtDepo.instance"):zbjtObject(obj,rt,intf) {}
};
struct zbjtStation:zbjtObject {
  zbjtStation(jtStation *obj,Runtime *rt,STRING intf="jtStation.instance"):zbjtObject(obj,rt,intf) {}
  jtStation *getlink() {return (jtStation*)zbjtObject::getlink();}
  zbObject *complete(int trc,int trl);
};
struct zbjtBusStop:zbjtObject {
  zbjtBusStop(jtBusStop *obj,Runtime *rt,STRING intf="jtBusStop.instance"):zbjtObject(obj,rt,intf) {}
};
struct zbjtAircraft:zbjtObject {
  zbjtAircraft(jtAircraft *obj,Runtime *rt,STRING intf="jtAircraft.instance"):zbjtObject(obj,rt,intf) {}
};
struct zbjtCab:zbjtObject {
  zbjtCab(jtCab *obj,Runtime *rt,STRING intf="jtCab.instance"):zbjtObject(obj,rt,intf) {}
  jtCab *getlink() {return (jtCab*)zbjtObject::getlink();}
  void render(zbRenderContext *rc,zbXMLNode *xml);
};



}

#endif
