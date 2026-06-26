#ifndef grtubusH
#define grtubusH

#include "game0.h"
#include "grrcom.h"
#include "gstatic.h"
#include "gimages.h"
#include "istatic.h"

namespace jtyc {

struct TrackTubus:Static {
  TrackTubus(bool avertical,int x,int y,int len,float ah0,float adhisum);
  TrackTubus(XMLNode xml,TrackMap *map);
  bool isedge(int dx,int dy) {return (dx==0 && dy==0) || (dy==h-1 && vertical) || (dx==w-1 && !vertical);}
  bool clickoccupy(int ax,int ay) {return (ax==x && ay==y) || (ax==x+w-1 && ay==y+h-1);}
  void destroy();
  void xmlsave(XMLNode xml);
  virtual bool canremove() {return true;}
  bool railcanremove();
  bool roadcanremove();
  TCntVec<TrackItem*> items;
protected:
  TCntVec<HiBlock*> hblockers;
  TCntVec<SqBlock*> sqblocks;

  bool vertical;
  float h0;int len;
  float dhisum;

  void createritems();
  void createblocks();
  virtual HiBlock *blocker(int x,int y,int index)=0;
  virtual TrackItem *createritem(int x,int y,bool vertical,bool edge,float ha)=0;
};

struct TrackBridge:TrackTubus {
  jtBridge *bridge;
  TrackBridge(bool avertical,int x,int y,int len,float ah0,float dhisum);
  TrackBridge(XMLNode xml,TrackMap *map);
  void xmlsave(XMLNode xml);
  void render(RenderContext &rc);
  string gettype() {return bridge->name;}
  void settype(string value);
  void dooctalize();
  void clearvisibility();
  void dodelize();
  void ot_render(int id);
  
  virtual string getxmlname()=0;

  void created();
protected:
  RefBase<BridgeInstance> instance;
  void createbridgeblocks();
  void createbridgeitems();
//  TrackItem *createritem(int x,int y,bool vertical,bool edge,float ha);
  HiBlock *blocker(int x,int y,int index);
};

struct TrackTunnel:TrackTubus {
  TrackTunnel(bool avertical,int x,int y,int len,float ah0,float dhisum);
  void render(RenderContext &rc);
  void destroy();
  void xmlsave(XMLNode xml);
  TrackTunnel(XMLNode xml,TrackMap *map);
  string gettype() {return tunnel->name;}
  void settype(string value);
  void clearvisibility();
  void dooctalize();
  void dodelize();
  void ot_render(int id);
  void markhiview();

  void created();
  virtual string getxmlname()=0;

  bool isvisible() {return visible;}
protected:
  jtTunnel *tunnel;
  bool visible;
  void changedtype();
  void createtunnelblocks();
  HiBlock *blocker(int x,int y,int index);
//  TrackItem *createritem(int x,int y,bool vertical,bool inedge,float ha);
};

/*
struct RTubus:Static {//predek pro RBridge a RTennel
  RTubus(bool avertical,int x,int y,int len,float ah0,float adhisum);
  RTubus(XMLNode xml);
  bool isedge(int dx,int dy) {return (dx==0 && dy==0) || (dy==h-1 && vertical) || (dx==w-1 && !vertical);}
  bool clickoccupy(int ax,int ay) {return (ax==x && ay==y) || (ax==x+w-1 && ay==y+h-1);}
  void destroy();
  void xmlsave(XMLNode xml);
  bool canremove();
  TCntVec<RItem*> items;
protected:
  TCntVec<HiBlock*> hblockers;
  TCntVec<SqBlock*> sqblocks;

  bool vertical;
  float h0;int len;
  float dhisum;

  void createritems();
  void createblocks();
  virtual HiBlock *blocker(int x,int y,int index)=0;
  virtual RItem *createritem(int x,int y,bool vertical,bool edge,float ha)=0;
};

struct RBridge:RTubus {
  jtBridge *bridge;
  RBridge(bool avertical,int x,int y,int len,float ah0,float dhisum);
  RBridge(XMLNode xml);
  void xmlsave(XMLNode xml);
  void render(RenderContext &rc);
  string gettype() {return bridge->name;}
  void settype(string value);
  void dooctalize();
  void clearvisibility();
  void dodelize();
  void ot_render(int id);
protected:
  PtrTo<BridgeInstance> instance;
  void createbridgeblocks();
  void createbridgeitems();
  RItem *createritem(int x,int y,bool vertical,bool edge,float ha);
  HiBlock *blocker(int x,int y,int index);
};

struct RTunnel:RTubus {
  RTunnel(bool avertical,int x,int y,int len,float ah0,float dhisum);
  void render(RenderContext &rc);
  void destroy();
  void xmlsave(XMLNode xml);
  RTunnel(XMLNode xml);
  string gettype() {return tunnel->name;}
  void settype(string value) {}
  void clearvisibility();
  void dooctalize();
  void dodelize();
  void ot_render(int id);
  void markhiview();
protected:
  jtTunnel *tunnel;
  friend class TunnelRItem;
  bool visible;
  void changedtype();
  void createtunnelblocks();
  HiBlock *blocker(int x,int y,int index);
  RItem *createritem(int x,int y,bool vertical,bool inedge,float ha);
};
*/

struct RBridge:TrackBridge {
  RBridge(bool avertical,int x,int y,int len,float ah0,float dhisum);
  RBridge(XMLNode xml);
  bool canremove() {return railcanremove();}
  TrackItem *createritem(int x,int y,bool vertical,bool edge,float ha);
  string getxmlname() {return "RBridge";}
};

struct RTunnel:TrackTunnel {
  RTunnel(bool avertical,int x,int y,int len,float ah0,float dhisum);
  RTunnel(XMLNode xml);
  bool canremove() {return railcanremove();}
  TrackItem *createritem(int x,int y,bool vertical,bool edge,float ha);
  string getxmlname() {return "RTunnel";}
};

struct RoadBridge:TrackBridge {
  RoadBridge(bool avertical,int x,int y,int len,float ah0,float dhisum);
  RoadBridge(XMLNode xml);
  bool canremove() {return roadcanremove();}
  TrackItem *createritem(int x,int y,bool vertical,bool edge,float ha);
  string getxmlname() {return "RoadBridge";}
};

struct RoadTunnel:TrackTunnel {
  RoadTunnel(bool avertical,int x,int y,int len,float ah0,float dhisum);
  RoadTunnel(XMLNode xml);
  bool canremove() {return roadcanremove();}
  TrackItem *createritem(int x,int y,bool vertical,bool edge,float ha);
  string getxmlname() {return "RoadTunnel";}
};

}

#endif
