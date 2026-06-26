#ifndef gtrackH
#define gtrackH

#include "game0.h"

namespace jtyc {

typedef void (*TrackItemFunc)(void *data,TrackItem *ri);

struct TrackItem:GInstance {
  int type;
  TrackItem *next;
  TrackItem *next_ri;
  int x,y;
  float dzx,dzy;//sklon ve zmeru osy x a y
  float za;//vyska bodu A
  int saveid;
  TrackMap *map;
  bool visible;

  TrackItem(int ax,int ay,float z,int atype,TrackMap *amap);
  TrackItem(XMLNode xml,int atype,TrackMap *amap);
  virtual ~TrackItem();

  void xmlsave(XMLNode xml);
  bool visiblesurface();
  virtual void countvisible() {visible=false;}

  virtual void afterloadlink() {}
  virtual void clearmobiles() {}
  virtual void run(double deltatime) {}

  virtual void settunnel(TrackTunnel *value) {}
  virtual void setbridge(TrackBridge *value) {}
  virtual void setedge(bool value) {}
  virtual void setbridgeitem(BridgeItemInstance *value) {}

//  TrackItem() {type=-1;next=NULL;next_ri=NULL;}
};


struct TrackMap {
  TMatrix<TrackItem*> map;
  int W,H;
  TrackItem *ri_list;
  TrackMap() {W=H=0;ri_list=NULL;}

  TrackItem *finditemid(int id);
  void eraseditem(TrackItem *item);
  void setri(int x,int y,TrackItem *ri) {map.set(x,y,ri);}//nastavi
  void createditem(TrackItem *item);
  void init();
  void doinit(int aw,int ah);
  void save(Archive &ar);
  void foreach(TrackItemFunc rif,void *data);
  virtual void modify() {}
  void afterloadlink();
  void clearmobiles();
  void run(double deltatime);
  TrackItem *at(int x,int y,int type);
  TrackItem *first_at(int x,int y);
};

}

#endif
