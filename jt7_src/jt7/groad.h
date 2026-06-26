#ifndef groadH
#define groadH

#include "game0.h"
#include "gtrack.h"
#include "gmtcom.h"

namespace jtyc {

typedef void (*RoadItemFunc)(void *data,RoadItem *ri);
RoadItem *loadroaditemref(XMLNode xml);

enum RoadBuildType {rbt_change,rbt_direct,rbt_fixed,rbt_plane};

RoadBuildType roadmodefromstr(string mode);

struct RoadItem:TrackItem {
  RoadItem *ns,*nj,*nv,*nz;//sousedi
  bool nov,noz,nos,noj;//blokovani smeru - u bus stop

  jtRoad *road;

  enum Type {tunnel,surface,bridge};
  RoadItem(XMLNode xml,Type type);
  RoadItem(int x,int y,float z,Type type);
  ~RoadItem();
  virtual string getxmlname()=0;
  void xmlsave(XMLNode xml);
  void render(RenderContext &rc);
  bool isstreight() {return (ns && nj && !nv && !nz) || (nv && nz && !ns && !nj);}
  float getdz(MSpojnice mt) {
    if (mt==MSpojnice(mt_13)) return -dzy;
    if (mt==MSpojnice(-mt_13)) return dzy;
    if (mt==MSpojnice(mt_24)) return -dzx;
    if (mt==MSpojnice(-mt_24)) return dzx;
    return 0;
  }

  //pro dijskr:
  double value;
  bool final,active;
  RoadItem *origin;
  MSpojnice used[4];
  int usecnt;

  bool allow(MSpojnice mt);
  bool addused(MSpojnice mt);
  void removeused(MSpojnice mt);

  void initdij() {value=0x100000;final=active=false;origin=NULL;}
};

struct SurRoadItem:RoadItem {
  SurRoadItem(XMLNode xml);
  SurRoadItem(int x,int y,float z);
  ~SurRoadItem();
  void xmlsave(XMLNode xml);
  void render(RenderContext &rc);
  void countvisible() {visible=visiblesurface();}
  virtual string getxmlname() {return "surface";}
};

struct BridgeRoadItem:RoadItem {
  TrackBridge *bridge;
  BridgeItemInstance *bridgeitem;
  bool edge;

  void setbridge(TrackBridge *value) {bridge=value;}
  void setedge(bool value) {edge=value;}
  void setbridgeitem(BridgeItemInstance *value) {bridgeitem=value;}

  BridgeRoadItem(XMLNode xml):RoadItem(xml,RoadItem::bridge) {edge=false;bridge=NULL;bridgeitem=NULL;}
  BridgeRoadItem(TrackBridge *abridge,int ax,int ay,float zcoord,bool vertical,bool aedge):RoadItem(ax,ay,zcoord,RoadItem::bridge) {
    edge=aedge;bridge=abridge;
    bridgeitem=NULL;
  }
  ~BridgeRoadItem();
  void countvisible();
  void xmlsave(XMLNode xml);
  void render(RenderContext &rc);
  virtual string getxmlname() {return "bridge";}
};

struct TunnelRoadItem:RoadItem {
  TrackTunnel *tunnel;
  bool edge;

  void settunnel(TrackTunnel *value) {tunnel=value;}
  void setedge(bool value) {edge=value;}

  TunnelRoadItem(TrackTunnel *atunnel,int ax,int ay,float zcoord,bool vertical,bool aedge):RoadItem(ax,ay,zcoord,RoadItem::tunnel) {
    edge=aedge;tunnel=atunnel;
  }
  TunnelRoadItem(XMLNode xml):RoadItem(xml,RoadItem::tunnel) {edge=false;tunnel=NULL;}

  ~TunnelRoadItem();
  void countvisible();
  void xmlsave(XMLNode xml);
  void render(RenderContext &rc);
  virtual string getxmlname() {return "tunnel";}
};

struct RoadMap:TrackMap {
  bool modified;
  RoadMap() {modified=true;}

  void load(Archive &ar);
  void save(Archive &ar);

  void foreach(RoadItemFunc rif,void *data);
  void render(RenderContext &rc);

  void xmlload(XMLNode xml);
  void xmlsave(XMLNode xml);

  void modify() {modified=true;}
  void wantok() {if (modified) modified=false,recount();}
  void recount();
  RoadItem *build(int x,int y,float dhi,RoadBuildType type);

  void erase(int x,int y);
  void countvisible();

  void setroadtype(int x,int y,string type);

  RoadItem *at(int x,int y,int type) {return (RoadItem*)TrackMap::at(x,y,type);}
  RoadItem *first_at(int x,int y) {return (RoadItem*)TrackMap::first_at(x,y);}
};

}

#endif
