#ifndef grdijmapH
#define grdijmapH

#include "grrmap.h"
//#include <com/jset.h>

namespace jtyc {

struct DijRVr {
  RRVRCH vr;
  RRVRCH beside;

  struct Soused {
    DijRVr *vr;
    rdist_t len;
    Spojnice tt;
  };

  Soused sousedi[8];//seznam sousedu
  int souscnt;

//  Set32 targets;//ktere cile se daji dosahnout

  //pro DijRMap::dijskr:
  double value;
  int steps;
  bool final;
  bool active;
  DijRVr *src;

  //vyuziva DijRMap::_findway
//  int waycnt;
  double dist;//vzdalenost do cile

//  DijRVr *next[8];
//  int nextcnt;
//  void addnext(DijRVr *anext) {next[nextcnt++]=anext;}//vetve stromecku
  //next[i]->src==this pro 0<=i<nextcnt
//  DijRVr *next;//next->src==this
//  DijRVr *alternext;//pri rozvetveni stro

  rdist_t countdist(int idx);

  void initdij() {value=1E10;steps=-1;final=false;src=NULL;active=false;}//targets.clear();waycnt=0;nextcnt=0;}
  void construct() {new(JP_VOID(this)) DijRVr();}
};

/*
struct RDestination {
  struct Item {
    RRVRCH end;
    int data;//napr. cislo koleje
    DijRVr *reached;
    Item(RRVRCH aend,int adata) {end=aend;data=adata;reached=NULL;}
    Item() {reached=NULL;data=0;}
  };
//  TObjVec<Item> items;
  TCntVec<DijRVr*> ends;
  virtual bool _accept(int data)=0;
  bool accept(int index) {return _accept(items[index].data);}
  bool isend() {
    return reached>0;
//    return reached==items.count;
  }
  void reach(const RRVRCH &aend,DijRVr *vr) {
    for(int i=0;i<items.count;i++) if (items[i].end==aend && !items[i].reached) {
      items[i].reached=vr;
//      vr->targets<<i;
      reached++;
      return;
    }
  }
  void reset() {
    for(int i=0;i<items.count;i++) items[i].reached=NULL;
    reached=0;
  }
  DijRVr *getend() {
    for(int i=0;i<items.count;i++) if (DijRVr *res=items[i].reached) return res;
    return NULL;
  }
  virtual void xmlsave(XMLNode xml)=0;
  ///vrati, zda prestane byt platne
  virtual bool notifyremove(GInstance *obj) {return false;}
  int reached;
};
*/

struct DijCallBack {
  virtual bool isend()=0;
  virtual void reached(DijRVr *vr)=0;
};

/*
struct RDestCallBack:DijCallBack {
  RDestination *dest;
  RDestCallBack(RDestination *adest) {dest=adest;}
  bool isend();
  void reached(DijRVr *vr);
};
*/

struct WayItem {
  RRSPOJ tt;
  bool last;
  WayItem(const RRSPOJ &att,bool alast):tt(att) {last=alast;}
  void save(XMLNode xml) {if (last) xml.setvar("last",1);tt.save(xml);}
  void load(XMLNode xml) {last=xml.ivar("last");tt.load(xml);}
  WayItem(XMLNode xml) {load(xml);}
};

struct WayInfo {
  rdist_t len;
  WayInfo() {len=-1;}
};

struct DijRMap:GInstance {
  TCntVec<DijRVr> items;
  bool modified;
  TCntVec<DijRVr*> active,final;//vlastne lokalni v dijskr, ale aby se nemuselo znova porad alokovat
  RRMap *map;
  DijRMap(RRMap *amap) {map=amap;modified=true;}
  void update();

  void dijskr(DijCallBack &dcb);
  void preparedijskr();
  void addactive(DijRVr *v,double value0=0);

  void _findway(DijRVr *end,IObjArray<WayItem> &way,TempPenaltyItems &tmpitems,WayInfo *info=NULL);
  //vyhleda usek cesty k nejblizsimu vetveni(pripadne pokud je vetveni moc blizko,
  //rozhodne, kterou cestou se da
  //predpoklada, ze dijskr uz je hotovy

  int freecango(DijRVr *end,rdist_t trainlen);
  //zjisti, zda vlak ma volny zacatek cesty
  //pokud na prvnim semaforu (nepocita se ten, na ktery ceka) je cervena,vrati 0
  //pokud na prvnim je zelena, ale nemuze volne projet tak, aby se cely vesel za prvni semafor, vrati 1
  //jinak vrati 2 (muze volne projet a nezablokuje semavfor)

  int actvr;//index do items aktualniho vr,kam se zapisuje
};


}

#endif
