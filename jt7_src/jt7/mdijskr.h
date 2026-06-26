#ifndef mdijskrH
#define mdijskrH

#include "game0.h"
#include "gmtcom.h"
#include <com/carray.h>

namespace jtyc {
class MotoTraj;class Motocar;

struct MEngine:CommonEngine {
};

struct BreakWay {
  double len;
  fpoint3d p1,p2;
};

struct MWayElem {
  RoadItem *ri;
  MSpojnice mt;
  BreakWay *bw;//pokud pred timto elem je usek, ktery neni zachycen v elems, jejde se po primce
  MWayElem(RoadItem *ri,MSpojnice mt):ri(ri),mt(mt) {bw=NULL;}
  MWayElem() {ri=NULL;bw=NULL;}
  fpoint3d count(double t);
  double len();
  double maxspeed();
  void save(XMLNode xml);
  void load(XMLNode xml);
};

inline bool operator ==(const MWayElem &e1,const MWayElem &e2) {return e1.ri==e2.ri && e1.mt==e2.mt;}

struct MotoWay:GInstance {
  TCntVec<MWayElem> items;
  int actitem;
  RoadItem *dest;
  MotoTraj *traj;
  MotoWay(MotoTraj *traj);

  //dir0 je smer, kam jde prvni cesta, 0:kamkoliv, 's'|'v'|'z'|'j'
  bool findway(RoadItem *src,RoadItem *dest,char dir0);
  MWayElem nextway(RoadItem *act);
  void notifyremove(GInstance *obj);

  void save(XMLNode xml);
  void load(XMLNode xml);
};

struct MotoTraj:GInstance,AbstractTrajectory {
  double carlen;
  double trajlen;
  double speed;

  MotoWay *way;
  Motocar *car;

  CycleArray<MWayElem,6> items;
  double t;//parametr vuci aktualni

  MWayElem nextway();
  void destroy();
  void trim();
  void setlen(double value) {carlen=value;}
  void run(double deltatime,MEngine &engine);
  fpoint3d at_count_pos(float dt) {return count(dt);}
  fpoint3d count(float dt) {return _count(t-dt);}
  bool setdest(RoadItem *ri);
  void start(MWayElem ri);
  double countdspeed(double force,double deltatime,MEngine &engine);
  double getdz();
  bool addelem(MWayElem elem,bool forceadd);
  void notifyremove(GInstance *obj);
  void ignorecross();

  MotoTraj(Motocar *car);

  void save(XMLNode xml);
  void load(XMLNode xml);

  bool isblocked() {return blocktime>0;}
private:
  fpoint3d _count(float t);
  BreakWay bwbuf[0x10];
  BreakWay *allocbw();
  float blocktime;//jek dlouho uz ceka
  int actbw;
  bool ignorencross;
  float breakdist;
};

}

#endif
