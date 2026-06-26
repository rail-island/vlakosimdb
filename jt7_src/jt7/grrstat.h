#ifndef grrstatH
#define grrstatH

#include "game0.h"
#include "gstatic.h"
#include "grrmap.h"
#include <com/carray.h>
#include "gimages.h"
#include "istatic.h"
//#include "railroadu.h"

namespace jtyc {

class RLinka;class RDestination;

struct RStation:Station {
//  static const int TYPE=1;
  float z;
  int dir;
//  bool vertical;
//  enum Type {horizontal,vertical};
//  Type type;
  TCntVec<RstRItem*> ritems;
//  IObjArray<RstRItem> ritems;
//  TRCArray<RstRItem> ritems;
  int trc,trl;//pocet koleji,delka nastupiste
  bool isdepo;

  void redraw(View *v) {}
  void redrawstatic(View *v);
//  void clickon(int dx,int dy);
//  OWNER<StaticBmp> bmp;
  RStation(int x,int y,int adir,int atrc,int atrl,int w,int h,float z,bool isdepo);
//  void save(BINWRITE &fw);
//  RDestination *dest();
//  void viewchanged() {bmp->viewchanged();}
//  void buldozere(int dx,int dy);
  void xmlsave(XMLNode xml);
  RStation(XMLNode xml);
//  bool isfree(int track) {return isfreetrack[track]==0;}
//  ~RStation() {delete []isfreetrack;}
  void trainin(int track) {}//isfreetrack[track]++;}
  void trainout(int track) {}//isfreetrack[track]--;}
  jtStation *complex;
  void setcomplex(jtStation *acomplex);
  void notifyremove(GInstance *obj);
  void destroy();
  void render(RenderContext &rc);
  void rendercaptions(RenderContext &rc);
  string gettype() {return complex->name;}
  void settype(string value);
//  RDestination *loadrdest(XMLNode xml) {return dest();}
  bool canremove();
  void dooctalize();
  void dodelize();
  void clearvisibility();
  void ot_render(int id);
  bool getstarttrack(TCntVec<RRSPOJ> &start,float minlen);
  RRSPOJ getsp(int track,int item);//vratin spojnici v zavislosti na dir
  //prida do vrs konecne vrs
  void getdijvrs(TCntVec<DijRVr*> &vrs);
private:
//  TMatrix<RRSPOJ> rmat;//indexovano trc,trl
  bool partvisible;
//  bool isdepo;
  RefBase<StationInstance> instance;
//  TCntVec<StationItem*> items;
//  int *isfreetrack;
  void created(bool createritems);
//  void createfreetrack() {isfreetrack=new int[trc];for(int i=0;i<trc;i++) isfreetrack[i]=0;}

//  string gettext() {return string("Nadrazi ")+name;}
//  jws::PolyForm *createform() {return createrstationform(this);}
};

/*struct DepoTrains:jws::ListIntf {
  RDepo *depo;
  DepoTrains(RDepo *depo):depo(depo) {}
  int getcount();
};*/
/*
struct RDepo:Static {
//  OWNER<StaticBmp> bmp;
//  friend class DepoTrains;
//  DepoTrains trains;
  int dir;

  InnerDepoRItem *rri;
  OuterDepoRItem *rro;
  TCntVec<Train*> trains;
  TCntVec<Train*> waiting;

  void clearmobiles() {isfree=true;trains.clear();waiting.clear();}

//  TRCArray<Train> trains;

//  TRCArray<Train> waiting;
//  CycleArray<RefBase<Train>,8> waiting;
//  CycleArray<8,int> waiting;

  RRVRCH startv;float z;bool vertical;
  bool isfree;

  RDepo(int x,int y,int adir,int w,int h,float z);
//  RDepo(BINREAD &fr);
//  void save(BINWRITE &fw);
//  void redraw(View *v) {}
//  void redrawstatic(View *v);
//  void viewchanged() {bmp->viewchanged();}
  void clickon(int dx,int dy) {}
  void buldozere(int dx,int dy);

  RRSPOJ trainstartspoj();//spojnice, ze ktere startuji vlaky
  RRVRCH trainstartvr();//vrchol, ze ktere startuji vlaky (predava se do trainway.start())

  void setfree(bool aisfree) {isfree=aisfree;}
  RDestination *dest();

  void starttrain(Train *train);
  void run(double deltatime);
  RDestination *loadrdest(XMLNode xml) {return dest();}
  void destroy();

  Spojnice outspoj();
  void xmlsave(XMLNode xml);
  RDepo(XMLNode xml);
  void trainin(Train *t);
  void render(RenderContext &rc);

//  jws::PolyForm *createform();
//  string gettext() {return "Depo";}
//  int getcount() {return trains.count;}
//  string stringat(int index);
  void gettrains(TCntVec<Train*> &value);

  string gettype() {return "";}
  void settype(string value);
  void afterloadlink();
  void ot_render(int id);
  void clearvisibility();
  void dooctalize();
  void dodelize();

  bool canremove();
private:
  jtDepo *type;
  bool visible;
  void created(bool createritems);
  Strings loadedtrains,loadedwaiting;
  Train *findtrain(string name);
};
*/

}

#endif
