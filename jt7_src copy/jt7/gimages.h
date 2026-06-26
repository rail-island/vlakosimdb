#ifndef gimagesH
#define gimagesH

#include "game0.h"
#include <glo/gloobject.h>
#include <ext/persistentobject.h>
#include "galias.h"
#include "iobject.h"

namespace jtyc {
using namespace glo;

class jtMotocar;class jtTunnel;class jtBusStop;


struct jtSemaphore:jtModelObject {
  float gradtime;
  bool isdefault;
  jtSemaphore() {gradtime=0;isdefault=false;}

  void load(PersistentObject *data,XMLNode xml);
};

struct jtTree {
  bool isdefault;

  bool match(string aname) {return aname==getname();}
  virtual string getname()=0;
  virtual string gettitle()=0;
  virtual float getw()=0;
  virtual float geth()=0;

  virtual void addref()=0;
  virtual void release()=0;

  virtual void render(RenderContext &rc)=0;
  virtual ~jtTree() {}
  jtTree() {isdefault=true;}
};

struct jtSimpleTree:jtDataObject,jtTree {
  RefBase<Texture> texture;
  float w,h;

  jtSimpleTree() {w=h=1;}
  virtual string getname() {return name;}
  virtual string gettitle() {return title;}
  void render(RenderContext &rc);
  void load(PersistentObject *data,XMLNode xml);
  float getw() {return w;}
  float geth() {return h;}
  void addref() {jtDataObject::addref();}
  void release() {jtDataObject::release();}
};

struct TrackTT {
  virtual void render(TrackItem *ri,int tt,RenderContext &rc)=0;
};

struct TT_Texture {
  RefBase<Texture> texture;
  float r,g,b;
  bool transparent;
  void load(PersistentObject *data,XMLNode xml,string prefix);
  void bind(RenderContext &rc);
};

struct StdTrackTT:TrackTT {
  TT_Texture tex;

  float hi;
  StdTrackTT(PersistentObject *data,XMLNode xml);

  void render(TrackItem *ri,int tt,RenderContext &rc);
  virtual void dorender(TrackItem *ri,int tt,RenderContext &rc)=0;
};

struct ModelRailTT:TrackTT {
  PtrTo<glo::gloModel> model;
  ModelRailTT(PersistentObject *data,XMLNode xml);
  float gauge2;
  fpoint3d lastpos;//updated by render

  void render(TrackItem *ri,int tt,RenderContext &rc);
};

struct ColsRailTT:ModelRailTT {
  int frequency;

  TT_Texture wiretex;
  float wire_y;
  float wire_linewi;

  jtRail *parent;

  ColsRailTT(PersistentObject *data,XMLNode xml,jtRail *parent);
  void render(TrackItem *ri,int tt,RenderContext &rc);
};

struct RailRailTT:StdTrackTT {
  float gauge2;
  float quality;
  float w;

  void dorender(TrackItem *ri,int tt,RenderContext &rc);
  RailRailTT(PersistentObject *data,XMLNode xml);
};
struct SleeperRailTT:StdTrackTT {
  float len,wi,cover;

  void dorender(TrackItem *ri,int tt,RenderContext &rc);
  SleeperRailTT(PersistentObject *data,XMLNode xml);
};
struct GravelRailTT:StdTrackTT {
  float quality;
  float w;

  void dorender(TrackItem *ri,int tt,RenderContext &rc);
  GravelRailTT(PersistentObject *data,XMLNode xml);
};
struct WireRailTT:StdTrackTT {
  float quality;
  float linewi;
  float gauge2;

  void dorender(TrackItem *ri,int tt,RenderContext &rc);
  WireRailTT(PersistentObject *data,XMLNode xml);
};
struct VWireRailTT:StdTrackTT {
  float linewi;
  float y1,y2;

  void dorender(TrackItem *ri,int tt,RenderContext &rc);
  VWireRailTT(PersistentObject *data,XMLNode xml);
};


struct GravelRoadTT:StdTrackTT {
  float d1,d2,d3;

  void dorender(TrackItem *ri,int tt,RenderContext &rc);
  GravelRoadTT(PersistentObject *data,XMLNode xml);
};

struct MidlineRoadTT:StdTrackTT {
  float w;

  void dorender(TrackItem *ri,int tt,RenderContext &rc);
  MidlineRoadTT(PersistentObject *data,XMLNode xml);
};

struct TramRailRoadTT:StdTrackTT {
  float gauge2,w,dist;

  void dorender(TrackItem *ri,int tt,RenderContext &rc);
  TramRailRoadTT(jtRoad *road,PersistentObject *data,XMLNode xml);
};

struct WireRoadTT:StdTrackTT {
  float gauge2,w,dist;

  void dorender(TrackItem *ri,int tt,RenderContext &rc);
  WireRoadTT(jtRoad *road,PersistentObject *data,XMLNode xml);
};

struct jtWay:jtDataObject {
  IObjArray<TrackTT> items;
  bool isdefault;

  jtWay() {isdefault=true;}
  virtual TrackTT *loadtrack(PersistentObject *data,XMLNode xml)=0;
  void render(TrackItem *ri,int tt,RenderContext &rc);
  void load(PersistentObject *data,XMLNode xml);
};

struct jtRail:jtWay {
  float wire_top,wire_bot,train_y;
  jtRail() {wire_top=wire_bot=train_y=0;}
  TrackTT *loadtrack(PersistentObject *data,XMLNode xml);
  void load(PersistentObject *data,XMLNode xml);
};

struct jtRoad:jtWay {
  float motodist;
  float moto_y;

  bool haswire,hastramrail;

  Strings allowstyles;

  bool allow(jtMotocar *motocar);//zda sem muze motocar
  jtRoad() {motodist=0.2;haswire=hastramrail=false;moto_y=0;}
  TrackTT *loadtrack(PersistentObject *data,XMLNode xml);
  void load(PersistentObject *data,XMLNode xml);
};

struct jtTerrain:jtDataObject {
  RefBase<Texture> texture;
  bool isdefault;

  jtTerrain() {isdefault=false;}
  void load(PersistentObject *data,XMLNode xml);
  void bind(RenderContext &rc);
};

struct Images {
//  Machines machines;
  TRCArray<jtMachine> machines;
  TRCArray<jtMotocar> motocars;
//  IObjArray<jtCommonBuilding> buildings;
  TRCArray<jtBridge> bridges;
  TRCArray<jtTunnel> tunnels;
  TRCArray<jtStation> stations;
  TRCArray<jtStation> depos;
  TRCArray<jtSemaphore> sems;
  TRCArray<jtBuilding> blds;
  TRCArray<jtTree> trees;
  TRCArray<jtRail> rails;
  TRCArray<jtTerrain> terrains;
  TRCArray<jtCab> cabs;
  TRCArray<jtAircraft> aircrafts;
  TRCArray<jtBusStop> busstops;
  TRCArray<jtRoad> roads;

  RefBase<jtStation> station0;
  RefBase<jtStation> depo0;
  RefBase<jtCab> cab0;
  RefBase<jtSemaphore> sem0;
//  PtrTo<gloModel> greensem,redsem;
  RefBase<jtBridge> bridge0;
  RefBase<jtTunnel> tunnel0;
  RefBase<jtMachine> machine0;
  RefBase<jtMotocar> motocar0;
  RefBase<jtTree> tree0;
  RefBase<jtRail> rail0;
  RefBase<jtRoad> road0;
  RefBase<jtTerrain> terrain0,water;
  RefBase<jtAircraft> aircraft0;
  RefBase<jtBusStop> busstop0;

  jtBridge *findbridge(string name) {jtBridge *res=bridges.psearchv(name);return res?res:(jtBridge*)bridge0;}
  jtTunnel *findtunnel(string name) {jtTunnel *res=tunnels.psearchv(name);return res?res:(jtTunnel*)tunnel0;}
  jtTree *findtree(string name) {jtTree *res=trees.psearchv(name);return res?res:(jtTree*)tree0;}
  jtTree *searchtree(string name) {jtTree *res=trees.psearchv(name);return res;}
  jtStation *findstation(string name) {jtStation *res=stations.psearchv(name);return res?res:(jtStation*)station0;}
  jtBusStop *findbusstop(string name) {jtBusStop *res=busstops.psearchv(name);return res?res:(jtBusStop*)busstop0;}
//  jtMachine *findmachine(string name) {jtMachine *res=machines.searchvalue(name);return res?res:machine0;}
  jtMachine *findmachine(string name);
  jtMachine *searchmachine(string name) {jtMachine *res=machines.psearchv(name);return res;}
  jtMotocar *searchmotocar(string name) {jtMotocar *res=motocars.psearchv(name);return res;}
//  jtMotocar *findmotocar(string name) {jtMotocar *res=motocars.searchvalue(name);return res?res:motocar0;}
  jtMotocar *findmotocar(string name);
  jtSemaphore *findsemaphore(string name) {jtSemaphore *res=sems.psearchv(name);return res?res:(jtSemaphore*)sem0;}
  jtStation *finddepo(string name) {jtStation *res=depos.psearchv(name);return res?res:(jtStation*)depo0;}
//  jtBuilding *findbuilding(string name) {jtBuilding *res=blds.searchvalue(name);return res;}
  jtBuilding *findbuilding(string name);
  jtTerrain *findterrain(string name) {jtTerrain *res=terrains.psearchv(name);return res?res:(jtTerrain*)terrain0;}
  jtRail *findrail(string name) {jtRail *res=rails.psearchv(name);return res?res:(jtRail*)rail0;}
  jtRoad *findroad(string name) {jtRoad *res=roads.psearchv(name);return res?res:(jtRoad*)road0;}
  jtAircraft *findaircraft(string name) {jtAircraft *res=aircrafts.psearchv(name);return res?res:(jtAircraft*)aircraft0;}
//  void load(XMLNode xml);
//  void loadmachines(XMLNode xml);
//  void loadstatics(XMLNode xml);
  Images();
//  Images() {station0=NULL;depo0=NULL;sem0=NULL;bridge0=NULL;tunnel0=NULL;machine0=NULL;tree0=NULL;terrain0=NULL;rail0=NULL;cab0=NULL;aircraft0=NULL;busstop0=NULL;motocar0=NULL;}
  ~Images();
//  Images(XMLNode xml) {load(xml);}

//  void viewchanged();
  void load(PersistentObject *obj);

  Aliases als;
};


extern Images *gimages;

void load_images();
inline void want_images() {if (!gimages) load_images();}

}

#endif
