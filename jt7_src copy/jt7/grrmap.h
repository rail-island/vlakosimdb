#ifndef grrmapH
#define grrmapH

#include "game0.h"
#include "grrcom.h"
#include "gsemaphore.h"
#include "gtrack.h"
#include <com/dvslist.h>

namespace jtyc {

//class RailItem;

typedef void (*RItemFunc)(void *data,RItem *ri);
RItem *loadritemref(XMLNode xml);

struct RInfo {
  int vrcnt;
  unsigned char vrtoindex[vr_cnt+1];//poradi vrcholu
  int begin;//vyuziva dijrrmap, je to index do jejiho pole vrcholu
  unsigned char indextovr[vr_cnt];

  //inicializuje pomoci RItem
  void update(RItem *ri);
  int operator [](Vrchol v) {return vrtoindex[v.data];}
  Vrchol operator [](int index) {return Vrchol(indextovr[index]);}
};

struct RItem:TrackItem {
//  enum Type {tunnel,surface,rst,bridge,idepo,odepo};
  enum Type {tunnel,surface,rst,bridge};
  static const char *typetoname(Type type);
//  Type type;
  jtRail *rail;
//  RItem *next;//dalsi RItem na tomto policku
  bool visited;//pomocna data
  int trains;//pocet vlaku zde
//  int dir;//0:obousmera trat,1:ve smeru spojnice,-1:v protismeru spojnice
//  bool visible;
//  int saveid;//uziva se pri ukladani vlaku

  RInfo info;

  int flags;

/*
  bool buldozere() {
//    destroy();
    exist=false;
    return true;
  }
*/

//  bool exist;

  //docasna penalizace, vlak ji nastavi, pokud si vybere cestu tudy
  rdist_t temp_penalty;

  RItem(int ax,int ay,float z,Type atype);
  float getz(Vrchol v);
  RItem(XMLNode xml,Type atype);
  virtual void xmlsave(XMLNode xml);
  virtual ~RItem();
//  virtual void destroy() {}
  //pokud ma semafor, nastavuje, zda je na strane vecholu v volno
  Spojnice spojnice();//pokud je jen jedna spojnice,vrati ji,pokud vice,vrati 0

  virtual void trainin(Train *t) {trains++;if (trains>1 && !hassem()) crash();}
  virtual void trainout(Train *t) {trains--;}
  virtual bool istrain() {return trains;}
  virtual bool isfree(Vrchol v) {return true;}//vraci, zda je volno,pokud prijedu z vrcholu v
  virtual double breakdist(Vrchol v) {return 0;}//pokud not isfree(v), vraci, jak daleko muze jet po teto RItem
  virtual bool hassem() {return false;}//vrati,zda ma semafor(nebo neco,co oddeluje obe strany)
  virtual bool dijskr_hassem2() {return false;}//vrati, zda ma obousmerny semafor, uziva dijskr
  virtual void setfree(bool isfree,Vrchol v) {}
  virtual bool isblocked() {return false;}//viz gsemaphore.h/Semaghore
  virtual void trainover() {}//je volano, pokud vlak projede semafor
  virtual void render(RenderContext &rc) {}
  virtual rdist_t dijskr_penalty(Spojnice tt) {return temp_penalty;}
  virtual bool canhavecol() {return false;}
  virtual void sem_get_neightboors(TCntVec<Sem_Rec> &res,Vrchol v) {}
  virtual int getsemdir() {return 0;}//vrati semdir, pokud hassem()

//  virtual void afterloadlink() {}
  void clearmobiles() {trains=0;}

  void updatesems() {Semaphore::updatesems(this);}
  float getdz(Spojnice tt) {
    if (tt==Spojnice(tt_13)) return -dzy;
    if (tt==Spojnice(-tt_13)) return dzy;
    if (tt==Spojnice(tt_24)) return -dzx;
    if (tt==Spojnice(-tt_24)) return dzx;
    return 0;
  }
  void updateinfo(bool modifymap=true) {info.update(this);}
  void notifyremove(GInstance *obj) {if (obj==next) next=NULL;GInstance::notifyremove(obj);}
  //vrati, zda sem muze pridat vlak, tj. zda se vsemi smery vzdy drive narazi na semafor nez na vlak
  bool canaddtrain();

//  RItem *next_ri;//nastavuje RRMap::createdritem, ukazatel do spojaku vsech RItem
protected:
  void crash() {}
  void std_draw(RenderContext &rc);
//  void draw_spojnice(RailItem *step,Spojnice tt,RenderContext &rc);
  void draw_spojnice(Spojnice tt,RenderContext &rc);
};

struct SurRItem:RItem {
  SurRItem(int ax,int ay,float z);
  ~SurRItem();
  Semaphore *semaphore;
  bool surblocked;
  RailBlock *rb;
  void surblock();
  void surunblock();
//  RefBase<SqBlock> block;
//  RefBase<Semaphore> semaphore;
//  SurRItem(BINREAD &fr):RItem(fr,surface) {fr>>block>>semaphore;}
//  void save(BINWRITE &fw) {RItem::save(fw);fw<<block<<semaphore;}
  void destroy() {}
  bool hassem() {return semaphore;}
  void setfree(bool isfree,Vrchol v);
//  void redrawstatic(View *v,ZBitmap *bmp);
//  void redraw(View *v,ZBitmap *bmp);
  void trainin(Train *t) {RItem::trainin(t);if (semaphore) semaphore->trainin();}
  void trainout(Train *t) {RItem::trainout(t);if (semaphore) semaphore->trainout();}
  bool isfree(Vrchol v) {return semaphore?semaphore->isfree(v):true;}
  bool isblocked() {return semaphore?semaphore->isblocked():false;}
  void trainover() {if (semaphore) semaphore->trainover();}
  double breakdist(Vrchol v) {return semaphore?semaphore->tt.len()/2:0;}
  void notifyremove(GInstance *obj) {if (obj==semaphore) semaphore=NULL;RItem::notifyremove(obj);}
  void run(double deltatime);
  int getsemdir();
  rdist_t dijskr_penalty(Spojnice tt);
  void clearmobiles();
  void render(RenderContext &rc);
  bool dijskr_hassem2();
  void xmlsave(XMLNode xml);
  SurRItem(XMLNode xml);
  void blocksurmap(Spojnice tt);
  void countvisible() {visible=visiblesurface();}
  bool canhavecol() {return !semaphore && spojnice().valid();}
  void sem_get_neightboors(TCntVec<Sem_Rec> &res,Vrchol v);
};

struct BridgeRItem:RItem {
  TrackBridge *bridge;
  BridgeItemInstance *bridgeitem;
  bool edge;

  void setbridge(TrackBridge *value) {bridge=value;}
  void setedge(bool value) {edge=value;}
  void setbridgeitem(BridgeItemInstance *value) {bridgeitem=value;}

  BridgeRItem(TrackBridge *abridge,int ax,int ay,float zcoord,bool vertical,bool aedge):RItem(ax,ay,zcoord,RItem::bridge) {
    flags=Spojnice(vertical?tt_13:tt_24).flag();updateinfo();edge=aedge;bridge=abridge;
    bridgeitem=NULL;
  }
  BridgeRItem(XMLNode xml):RItem(xml,RItem::bridge) {edge=false;bridge=NULL;bridgeitem=NULL;}
//  void xmlsave(XMLNode xml);
//  BridgeRItem(BINREAD &fr):RItem(fr,bridge) {}
//  void save(BINWRITE &fw) {RItem::save(fw);}
  void render(RenderContext &rc);
  void countvisible();
};

struct TunnelRItem:RItem {
  TrackTunnel *tunnel;
  bool edge;

  void settunnel(TrackTunnel *value) {tunnel=value;}
  void setedge(bool value) {edge=value;}

  TunnelRItem(TrackTunnel *atunnel,int ax,int ay,float zcoord,bool vertical,bool aedge):RItem(ax,ay,zcoord,RItem::tunnel) {
    flags=Spojnice(vertical?tt_13:tt_24).flag();updateinfo();edge=aedge;tunnel=atunnel;
  }
  TunnelRItem(XMLNode xml):RItem(xml,RItem::tunnel) {edge=false;tunnel=NULL;}
//  TunnelRItem(XMLNode xml):RItem(xml,tunnel) {edge=xml.ivar("edge");}
//  void xmlsave(XMLNode xml) {RItem::xmlsave(xml);xml.setvar("edge",edge);}
  void countvisible();
  void render(RenderContext &rc);
};

/*
struct InnerDepoRItem:RItem {
//  BackRef<RDepo> depo;
  RDepo *depo;
  InnerDepoRItem(int ax,int ay,float z,bool vertical,RDepo *adepo):RItem(ax,ay,z,idepo) {
    flags=Spojnice(vertical?tt_13:tt_24).flag();depo=adepo;updateinfo();
  }
//  InnerDepoRItem(BINREAD &fr):RItem(fr,idepo) {fr>>depo;}
//  void save(BINWRITE &fw) {RItem::save(fw);fw<<depo;}
  InnerDepoRItem(XMLNode xml):RItem(xml,idepo) {depo=NULL;}//pak musi depo priradit ukazatel
//  void xmlsave(XMLNode xml) {RItem::xmlsave(xml);
  bool hassem() {return true;}
  void setfree(bool isfree,Vrchol v);
  void countvisible() {visible=false;}
  void render(RenderContext &rc);
};
struct OuterDepoRItem:RItem {
//  BackRef<RDepo> depo;
  RDepo *depo;
  string outtrainname;
  Train *outtrain;//vlak vyjizdejici z depa
  OuterDepoRItem(int ax,int ay,float z,bool vertical,RDepo *adepo):RItem(ax,ay,z,odepo) {
    flags=Spojnice(vertical?tt_13:tt_24).flag();depo=adepo;outtrain=NULL;updateinfo();
  }
//  OuterDepoRItem(BINREAD &fr):RItem(fr,odepo) {fr>>depo;}
//  void save(BINWRITE &fw) {RItem::save(fw);fw<<depo;}
  void afterloadlink();
  OuterDepoRItem(XMLNode xml):RItem(xml,odepo) {depo=NULL;outtrain=NULL;outtrainname=xml.svar("outtrain");}
  void xmlsave(XMLNode xml);
  void trainin(Train *t) {RItem::trainin(t);updatesems();}
  void trainout(Train *t) {RItem::trainout(t);if (outtrain==t) {outtrain=NULL;trains--;updatesems();}}
//  void trainout(Train *t) {RItem::trainout(t);updatesems();}
//  void setouttrain(Train *t) {outtrain=t;trains++;updatesems();}
  void setouttrain(Train *t) {outtrain=t;trains++;updatesems();}
  void countvisible() {visible=visiblesurface();}
  void notifyremove(GInstance *obj);
  void render(RenderContext &rc);
};
*/

struct RstRItem:RItem {
  int track;
//  BackRef<RStation> rstation;
  RStation *rstation;
  RstRItem(int ax,int ay,float z,bool vertical,int atrack,RStation *astation):RItem(ax,ay,z,rst) {
    flags=Spojnice(vertical?tt_13:tt_24).flag();updateinfo();track=atrack;rstation=astation;
  }
  RstRItem(XMLNode xml):RItem(xml,rst) {rstation=NULL;track=xml.ivar("track");}
  void xmlsave(XMLNode xml);
//  void save(BINWRITE &fw) {RItem::save(fw);fw<<track<<rstation;}
//  void draw(View *v,ZBitmap *bmp);
  void trainin(Train *t);
  void trainout(Train *t);
  void countvisible() {visible=visiblesurface();}
  void render(RenderContext &rc);
};


enum BuildType {bt_change,bt_direct,bt_fixed};

class DijRMap;
struct RRMap:TrackMap {
//  int W,H;
//  TMatrix<RItem*> map;

//  RItem *map
//  OWNER<RefMatrix<RItem> > map;
//  DVSList ritems;
  RRMap();
//  RRMap(BINREAD &fr);
//  RRMap(XMLNode xml);
//  void save(BINWRITE &fw);
  void redraw(View *v);
  void redrawstatic(View *v);
  void build(int x,int y,Spojnice tt,float dhi,BuildType bt);//postavi trat na zemi
  //pokud (dhi), tak se snazi, aby byla o dhi nahoru
  bool valid(int x,int y) {return x>=0 and y>=0 and x<W and y<H;}
//  RItem *build(int x,int y,RItem::Type type,Spojnice tt,int h);//postavi obecny kus trate
//  void build(RItem *ri) {addri(ri->x,ri->y,ri);}//postavi obecny kus trate
  //(ve vzduchu/v zemi),vyrobi zvlastni ritem
  RItem *at(int x,int y,int type) {return (RItem*)TrackMap::at(x,y,type);}
  RItem *first_at(int x,int y) {return (RItem*)TrackMap::first_at(x,y);}
//  void addri(RItem *ri) {addri(ri->x,ri->y,ri);}
//  void erase(RItem *ri);
  void buldozere(int x,int y);
  void foreach(RItemFunc rif,void *data);
//  void updatesems(RItem *beg) {Semaphore::updatesems(beg);}
  DijRMap *dijmap;
  void modify();
  void countvisible();
  void render(RenderContext &rc);
//  void init();
  void erase(int x,int y);
//  void clearmobiles();

//  void erasedritem(RItem *item);
//  void createdritem(RItem *item);

  void save(Archive &ar);
  void load(Archive &ar);
  void setrailtype(int x,int y,string type);//for bridge,tunnel:set the edge square for whole object

//  void afterloadlink();

  void storerrtemplate(int x0,int y0,int w,int h,XMLNode xml);
  void userrtemplate(int x0,int y0,int dir8,XMLNode xml);
  void drawrrtemplate(int x0,int y0,int dir8,XMLNode xml);

  void drawmap(int minx,int miny,int maxx,int maxy);
  void drawmapitem(int x,int y,int flags,int *semdir=NULL,bool maphi=false);

//  RItem *findritemid(int id);

//  RItem *ri_list;
private:
//  static const int newitemcount=0x20;
//  RItem *newitems[newitemcount];
//  int newcnt;

  void xmlsave(XMLNode xml);
  void xmlload(XMLNode xml);

//  void doinit(int aw,int ah);

//  void addri(int x,int y,RItem *ri);//prida na zacatek spojoveho seznamu na pozici x,y

  void _build(int x,int y,Spojnice tt,float dhi,BuildType bt);
  RItem *addnew(int x,int y,Spojnice tt,float dhi,BuildType bt);
//  RItem *addnewdenyhi(int x,int y,Spojnice tt);
  void addtoex(SurRItem *ri,int x,int y,Spojnice tt);
//  RItem *addnewfixed(HiMap *hm,int x,int y,Spojnice tt,float dhi);
  //postavi zeleznici, prida k existujici
};

struct RRVRCH {//jednoznacne identifikuje misto na koleji(ctverecek a vrchol)
//  BackRef<RItem> ri;
  RItem *ri;
  Vrchol v;
  RRVRCH direct() {return RRVRCH(ri,v.direct());}
  RRVRCH(RItem *ari,Vrchol av) {ri=ari;v=av;}
  RRVRCH() {}
  int neightboors(RRVRCH buf[],rdist_t lens[]=NULL,Spojnice tts[]=NULL,bool ignoredir=false) const;
  //zapise vsechny sousedy do buf,vrati jejich pocet
  //do lens da vzdalenosti sousedu,vzdalenost je delka trasy pres policko [x,y]
  //(vrchol v bere jako zacatek)
  RRVRCH beside();
//  void save(BINWRITE &fw) {
//    fw<<v.data<<ri;
//  }
//  void load(BINREAD &fr) {
//    fr>>v.data>>ri;
//  }
  bool valid() const {return ri && v.data!=vr_none;}
  DijRVr *dijvr();//musi byt spravne updatovano dijmap
  void eprint() {
    if (ri) eprintf("[%d,%d],vrchol %d\n",ri->x,ri->y,v.data);
    else eputs("Invalid RRVRCH\n");
  }
};
inline bool operator ==(const RRVRCH &v1,const RRVRCH &v2) {return v1.ri==v2.ri and v1.v==v2.v;}
inline bool operator !=(const RRVRCH &v1,const RRVRCH &v2) {return v1.ri!=v2.ri or v1.v!=v2.v;}

//BINWRITE &operator <<(BINWRITE &fw,RRVRCH &v) {v.save(fw);return fw;}
//BINREAD &operator >>(BINREAD &fr,RRVRCH &v) {v.load(fr);return fr;}

struct RRSPOJ {//jednoznacne identifikuje spojnici(ctverecek+spojnice)
  Spojnice tt;
  RItem* ri;
//  RefTo<RItem> ri;
  RRVRCH beg() {return RRVRCH(ri,tt.beg());}
  RRVRCH end() {return RRVRCH(ri,tt.end());}
  RRSPOJ(RItem *ari,Spojnice att) {ri=ari;tt=att;}
  RRSPOJ() {}
//  void save(BINWRITE &fw) {
//    fw<<tt.data<<ri;
//  }
//  void load(BINREAD &fr) {
//    fr>>tt.data>>ri;
//  }
  bool valid() const {return ri and tt.data!=tt_none;}
  double len() const {
    double tl=tt.len();
    int d=max(abs(ri->dzx),abs(ri->dzy));
//    return sqrt(tl*tl+d*d*frac_z_sq*frac_z_sq);
    return sqrt(tl*tl+d*d);
  }
  point3d<float> countpos(double t,fvector3d *diff=NULL);
  void save(XMLNode xml) {
    tt.save(xml);
    xml.setvar("rid",ri->saveid);
  }
  void load(XMLNode xml) {
    tt.load(xml);
    ri=loadritemref(xml);
  }
};
inline bool operator ==(const RRSPOJ &s1,const RRSPOJ &s2) {return s1.ri==s2.ri and s1.tt==s2.tt;}
inline bool operator !=(const RRSPOJ &s1,const RRSPOJ &s2) {return s1.ri!=s2.ri or s1.tt!=s2.tt;}

extern float max_tt_leap;
extern float rr_build_mhd;


}

#endif
