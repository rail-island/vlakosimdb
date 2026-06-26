#ifndef istaticH
#define istaticH

#include "iobject.h"
#include <glo/gloocttree.h>

namespace jtyc {

class jtCompletable;
void jt_transformate(int dir,int w,int h);
///backtracking
void jt_complete(Array<jtCompletable> &avail,TCntVec<int> &res,int cnt);
void jt_renderbuilding(RenderContext &rc,float dx,float dy,float dz,int w,int h,int dir,glo::gloModel *model);

struct jtCommonBuilding:jtModelObject {
  int w,h;
  gloBoundBox bound;
//  float diam;//horni odhad

  jtCommonBuilding() {w=h=0;}

  void loadmodel(PersistentObject *data,string file);
  virtual void render(RenderContext &rc,int dir);
};

struct jtCompletable {
  int min,max,cnt,id;
  int bmin,bmax,emin,emax;
  jtCompletable() {cnt=0;min=max=-1;bmin=bmax=emin=emax=-1;id=-1;}
  jtCompletable(XMLNode xml);
};

class jtStation;class jtStationItem;class StationInstance;


struct CompletableItemBuildingItem {
  PtrTo<glo::gloModel> model;
  gloBoundBox bound;
  int w,h;
  float dx,dy,dz;
  int dir;
  CompletableItemBuildingItem(int w,int h):w(w),h(h) {dx=dy=dz=0;dir=0;}
  void load(PersistentObject *data,XMLNode xml);
  void render(RenderContext &rc,int dir,int w,int h);
};

struct jtCompletableItemBuilding:jtCommonBuilding {
  gloBoundBox bound;
  void loaditem(int w,int h,PersistentObject *data,XMLNode xml);
  void render(RenderContext &rc,int dir);
  IObjArray<CompletableItemBuildingItem> models;
};


struct jtBuilding:jtCompletableItemBuilding {
  void load(PersistentObject *data,XMLNode xml);
};

struct jtStationPeron:jtDataObject {
  void load(PersistentObject *data,XMLNode xml);

  jtStationPeron(jtStation *parent):parent(parent) {}

  jtStation *parent;
  int trc;

  TCntVec<jtStationItem*> items;
  TCntVec<jtCompletable> items_c;
};

struct jtStationItem:jtCompletableItemBuilding {
//  jtStationTrack *track;
  //h==trc,w==trl
  void load(PersistentObject *data,XMLNode xml);

  int trc,trl;
};

struct jtStation:jtDataObject {
//  IObjArray<jtCompletable> perons,tracks;
//  IObjArray<jtStationTrack> perons,tracks;
  bool isdefault;
  bool isdepo;

  IObjArray<jtStationItem> items;
  TCntVec<jtCompletable> items_c;

  IObjArray<jtStationPeron> perons;
  TCntVec<jtCompletable> perons_c;

  jtStation() {isdefault=false;isdepo=false;}
  void load(PersistentObject *data,XMLNode xml);

  StationInstance *complete(int trc,int trl);
//  jtStationItem *finditem(string name,int trc);
};

typedef jtStation jtDepo;

struct StationItemInstance {
  jtStationItem *item;
//  gloBoundBox bound;
  bool visible;
  StationItemInstance(jtStationItem *aitem) {item=aitem;}
};

struct PeronInstance {
  jtStationPeron *peron;
  void clearvisibility() {for(int i=0;i<items.count;i++) items[i]->visible=false;}
  IObjArray<StationItemInstance> items;
//  TCntVec<jtStationItem*> items;
};

struct StationInstance:RCObject {
  int trc,trl;
  void render(float x,float y,float z,RenderContext &rc,int dir);
  void clearvisibility() {for(int i=0;i<perons.count;i++) perons[i]->clearvisibility();}
  void octalize(void *obj,int x,int y,float z,int dir,OctTree *ot);
  void ot_render(int id);
  IObjArray<PeronInstance> perons;
};

struct jtBridgeItem:jtCompletableItemBuilding {
  int cnt;
  void load(PersistentObject *data,XMLNode xml);
  jtBridgeItem() {cnt=0;w=h=1;}
};

struct BridgeItemInstance {
  jtBridgeItem *item;
  bool visible;
  BridgeItemInstance(jtBridgeItem *aitem) {item=aitem;}
};

struct BridgeInstance:RCObject {
  IObjArray<BridgeItemInstance> items;
  void render(float x,float y,float z,RenderContext &rc,bool vertical);
};

struct jtBridge:jtDataObject {
  IObjArray<jtBridgeItem> items;
  TCntVec<jtCompletable> items_c;
  bool isdefault;
//  string name;
  void load(PersistentObject *data,XMLNode xml);
  BridgeInstance *complete(int len);
  jtBridge() {isdefault=false;}
};

struct jtTunnel:jtCommonBuilding {
  RefBase<Texture> top,bot;
  float topscale,botscale;
  bool isdefault;
  float horspace,verspace;
  jtTunnel() {w=1;h=1;horspace=verspace=0;isdefault=false;topscale=1;botscale=1;}

  void load(PersistentObject *data,XMLNode xml);
};

struct jtBusStop:jtModelObject {
  gloBoundBox bound;
  bool isdefault;
  jtBusStop() {isdefault=false;}

  void load(PersistentObject *data,XMLNode xml);
  void render(RenderContext &rc,bool vertical);
  void octalize(void *obj,float x,float y,float z,OctTree *ot);
};

struct jtAirport:jtDataObject {
  bool isdefault;
  int w,h;
  jtAirport() {isdefault=false;w=h=0;}
};

gloBoundBox bldrotatebound(const gloBoundBox &box,int dir,int w,int h);

extern float BSF;//buildingsizefactor

}

#endif
