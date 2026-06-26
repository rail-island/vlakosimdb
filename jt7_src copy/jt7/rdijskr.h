#ifndef rdijskrH
#define rdijskrH

#include "grrcom.h"
#include "grrmap.h"
//#include "dijskr.h"
#include <com/carray.h>
#include "grdijmap.h"

namespace jtyc {

struct TempPenaltyItems {
  TCntVec<RItem*> ritems;
  ~TempPenaltyItems() {clear();}
  void clear();
  void notifyremove(GInstance *obj);
  void add(RItem *ri);//prida RItem a penalizuje
  void save(XMLNode xml);
  void load(XMLNode xml);
};

class DijRVr;
struct REngine:CommonEngine {
};

struct WaitGroup:RCObject {
  TCntVec<Train*> trains;
  void findbest();//najde vlak, ktery je nejlepsi, aby vyrazil, nastavi mu goimpusl na true
  void add(Train *t);
};

class TrainWay;
struct RTrajectory:GInstance {
  double len;double t;double speed;
  bool no_way;

//  RDepo *headingtodepo;
//  bool headingindepo;//zda se pohybuje po depo

  bool manual;//manualni rizeni, nekouka na semafory a na dest
  RefBase<WaitGroup> waitgroup;
  bool goimpuls;
  int actpri;//priorita, zvysuje se kazdym pridanim do waitgroup 

  double  breakdist;//vzdalenost do mista, kde musim zabrzdit
  RItem *breakri;
  TrainWay *way;double trainlen;
  bool breakingtodest;//brdeni pred cilem

  struct Item:RRSPOJ {
    bool match(RItem *ari) {return ri==ari;}
    void save(XMLNode xml) {RRSPOJ::save(xml);xml.setvar("pos",pos);}
    void load(XMLNode xml) {RRSPOJ::load(xml);pos=xml.fvar("pos");}
    double pos;
    Item(const RRSPOJ &rr,double apos):RRSPOJ(rr) {pos=apos;}
    Item() {}
  };
  CycleArray<Item,6> items;
  RItem *waitto;

  double traindz();
  void add(const RRSPOJ &rr);
  void trim();//vymaze casti z trajektorie,ktere jiz nejsou potreba
  RRVRCH reversevr() {//musi byt volano po trim
    return items.count>0?items[0].beg():RRVRCH();
  }
  void reverse();//otoci vlak, pocita, ze bylo udelano trim
  int finditem(double *beg,double t);
  bool _count(point3d<float> &res,double t,fvector3d *diff);
  bool count(point3d<float> &res,double dt,fvector3d *diff) {return _count(res,t-dt,diff);}
//  void save(BINWRITE &fw);
//  void load(BINREAD &fw);
  void updatepos();

  ///zavola loaded_istrain pro vsechny soucasti trajektorie
  // void initloaded();

  void destroy();
  RTrajectory(TrainWay *away) {
    len=0;t=0;speed=0;breakdist=-1;way=away;trainlen=1;
    breakingtodest=false;//headingtodepo=NULL;headingindepo=false;
    no_way=true;waitto=NULL;breakri=NULL;manual=false;actpri=0;goimpuls=false;
  }
//  void todepo(RDepo *depo);

  void move(double deltatime,REngine &engine);
  void settrainlen(double alen) {trainlen=alen;}
  void clear();
  void ignoresems();
  void stop() {speed=0;}
//  void reacheddepo();
  void setmanual(bool value);
  bool getmanual() {return manual;}
  void init(TCntVec<RRSPOJ> &start);

  void beforerun();
  void findwaitgroup();

  bool canredraw();
  void notifyremove(GInstance *obj);

  void save(XMLNode xml);
  void load(XMLNode xml);

  double countdspeed(double force,double deltatime,REngine &engine);
};


//struct NO_WAY {};
struct NO_DEST {};
struct TRAIN_NOT_VISIBLE {};

/*struct RWays {
  int findto(const RRVRCH &pos,RDestination *dest,bool overfirstsq);//vrati, kolik cest uspesne naslo
  IObjArray<RWay> trajs;
  RRSPOJ findway(RDestination *dest,bool &last);
  int actitem;//kolikata item z selected ways je aktualni
};*/

struct TrainWay:GObject {
//stara se kompletne o cestu vlaku
  IObjArray<WayItem> items;
  TempPenaltyItems tmpitems;
  Train *train;
  int actitem;//index do items
  double nowaytime;//cas od posledniho pokusu o refindway
  double nowaytesttime;//interval, v jakem se zkousi najit cesta
//  string destmode;//""==setdest(),"depo","xy"

  bool headingtodepo;
  int gotox,gotoy;//waypoint, gotox==-1:neni

  RTrajectory *realtraj;//skutecna trajektorie-vybirana z ways

  TrainWay(Train *atrain) {actitem=0;train=atrain;realtraj=new RTrajectory(this);isreversed=false;nowaytime=0;nowaytesttime=randpause(2);headingtodepo=false;dest=NULL;gotox=gotoy=-1;}
//  TrainWay(BINREAD &fr):GObject(fr),realtraj(this) {}
//  void trim() {realtraj.trim(trainlen);}
  bool setdest(RStation *adest);//throws NO_WAY
//  bool start(const RRSPOJ &tt0,RDestination *adest,const RRVRCH &pos);//throws NO_WAY
  //tt0 muzi byt not valid(),je to prvni cast ceste
  point3d<float> count(double dt,fvector3d *diff=NULL);//throws NO_WAY,DEST_REACHED
  //dt je vzdalenost pocitaneho bodu od predku vlaku
//  fvector countdiff(double t);
//  void save(BINWRITE &fw) {}
//  void load(BINREAD &fr) {}
//  void move(double dt) {realtraj.t+=dt;}
//  void next();//spocita dalsi kus cesty
  void iterate(double deltatime,REngine &engine);
  RRSPOJ next(bool &last);//spocita dalsi kus cesty
  void settrainlen(double alen) {realtraj->settrainlen(alen);}
  void clear() {realtraj->clear();}
  void ignoresems() {realtraj->ignoresems();}
  bool todepo();
  void notodepo();
  void stop() {realtraj->stop();}
  bool reverse();
  bool canredraw() {return realtraj->canredraw();}
//  bool hasposition() {return realtraj->items.count>0;}
//  void reacheddepo();
  void notifyremove(GInstance *obj);
//  bool refindway(const RRSPOJ &tt0,const RRVRCH &pos);
  ///najde novou cestu z aktualniho umisteni, muze otocit pripadne vlak
  ///reversemode:0-neotaci se vubec,1-otaci se,kdyz jinak nejde,2-otaci se,pokud je to kratsi
  bool refindway(int reversemode);
  void destroy();
  void setmanual(bool value);
  bool getmanual() {return realtraj->getmanual();}
  void beforerun() {realtraj->beforerun();}
//  bool findwaytodepo();
  bool gotoxy(int x,int y);
  void cleardests();

  //pusti dijskr, vrati, kam dojelo, predpoklada pripravene dijmap (aktivni vrcholy, volano preparedijskr())
  //zavisi na heidingtodepo..., podle toho zvoly spravny DijCallBack 
  DijRVr *rundijskr();

  void init(TCntVec<RRSPOJ> &start);

  void save(XMLNode xml);
  void load(XMLNode xml);

  RStation *dest;

//  OWNER<RDestination> dest;
  bool isreversed;
  void findway(const RRSPOJ &tt0,WayInfo *info=NULL);
  bool findway(const RRSPOJ &tt0,RRVRCH pos,WayInfo *info=NULL);

  //vrati, zda se do cile musi brzdit
  bool wantbreak();
};

}

#endif
