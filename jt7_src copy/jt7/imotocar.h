#ifndef imotocarH
#define imotocarH

#include "iobject.h"
#include "atrajectory.h"

namespace jtyc {
using namespace glo;

class jtMotocar;class jtCab;

struct interval {
  float a,b;
  interval *next;
  interval(float a,float b,interval *next):a(a),b(b),next(next) {}
};

struct MT_Common {
  jtMotocar *mcar;
  MT_Common(jtMotocar *mcar):mcar(mcar) {}
  virtual void render(RenderContext &rc,AbstractTrajectory &traj)=0;
  virtual bool load(XMLNode xml,gloModel *model,PersistentObject *data)=0;
  virtual float getlen()=0;

  //vyrenderuje model (pozice zadana budy a,b), vrati tranmsformaci, kterou se to renderuje
  void render_solid(RenderContext &rc,gloModel *model,fpoint3d a,fpoint3d b,transform3d<float> &tr);
};

struct MD_PasData {//data tykajici se jednoho pasu (texture, texcoords)
  float zd_t1,zd_t2,zd_s1,zd_s2;
  float zi_t1,zi_t2,zi_s1,zi_s2;
  float yd_t1,yd_t2,yd_s1,yd_s2;

  float pas_wi,pas_y1,pas_y2;

  RefBase<Texture> zdec,zinc,ydec;

  void load(XMLNode xml,PersistentObject *data,float beg,float end);
  //beg1 je x-ova pizice zacatku pasu pri tarnsformaci t1, end2 je x-ova pozice konce pasu pri t2
  void render(RenderContext &rc,const transform3d<float> &t1,const transform3d<float> &t2,float beg1,float end2);
};

struct MT_PasCommon:MT_Common {
  virtual bool dopasload(XMLNode xml,gloModel *model,PersistentObject *data,gloPolyObject *poly,interval *list)=0;
  bool load(XMLNode xml,gloModel *model,PersistentObject *data);

  MT_PasCommon(jtMotocar *mcar):MT_Common(mcar) {}
};

struct MT_Bus2c:MT_PasCommon {//kloubovy autobus s jednim kloubem
  MD_PasData pas;

  PtrTo<gloModel> head,tail;
  float headlen,spacelen,taillen;//headlen+spacelen+taillen=delka puvodniho modelu

  float head_d1,head_d2,tail_d1,tail_d2;

  float pas_begx,pas_endx;
  
  MT_Bus2c(jtMotocar *mcar):MT_PasCommon(mcar) {}
  bool dopasload(XMLNode xml,gloModel *model,PersistentObject *data,gloPolyObject *poly,interval *list);
  void render(RenderContext &rc,AbstractTrajectory &traj);
  float getlen() {return headlen+spacelen+taillen;}
  void makehead();
  void maketail(float tailx);
};

struct MT_PasTram:MT_PasCommon {
  MT_PasTram(jtMotocar *mcar):MT_PasCommon(mcar) {}

  void makemember(gloModel *model,float &d1,float &d2);
};

struct MT_Tram2c:MT_PasTram {//kloubovy autobus s jednim kloubem
  MD_PasData pas;

  PtrTo<gloModel> head,tail;
  float headlen,spacelen,taillen;//headlen+spacelen+taillen=delka puvodniho modelu

  float head_d1,head_d2,tail_d1,tail_d2;

  float pas_begx,pas_endx;

  MT_Tram2c(jtMotocar *mcar):MT_PasTram(mcar) {}
  bool dopasload(XMLNode xml,gloModel *model,PersistentObject *data,gloPolyObject *poly,interval *list);
  void render(RenderContext &rc,AbstractTrajectory &traj);
  float getlen() {return headlen+spacelen+taillen;}
};

struct MT_Tram3c:MT_PasTram {//kloubovy autobus s jednim kloubem
  MD_PasData pas1,pas2;

  float mem1,spa1,mem2,spa2,mem3;//delky clanku tramvaje

  float beg1,end1,beg2,end2,beg3,end3;//hranice useku

  float m1_d1,m1_d2,m2_d1,m2_d2,m3_d1,m3_d2;

  PtrTo<gloModel> m1,m2,m3;

  float pasbeg1,pasend1,pasbeg2,pasend2;

  MT_Tram3c(jtMotocar *mcar):MT_PasTram(mcar) {}

  bool dopasload(XMLNode xml,gloModel *model,PersistentObject *data,gloPolyObject *poly,interval *list);
  void render(RenderContext &rc,AbstractTrajectory &traj);
  float getlen() {return mem1+spa1+mem2+spa2+mem3;}
};

struct MT_Solid:MT_Common {//jednolite vozidlo
  double len;
  double d1,d2;//vzdalenost 1 a druhe napravy od pocatku vozidla

  PtrTo<gloModel> model;
  void render(RenderContext &rc,AbstractTrajectory &traj);
  bool load(XMLNode xml,gloModel *model,PersistentObject *data);
  float getlen() {return len;}

  MT_Solid(jtMotocar *mcar):MT_Common(mcar) {len=d1=d2=0;}
};

struct MT_Tram2v:MT_Common {//jednolite vozidlo
  PtrTo<MT_Solid> solid;
  float space;

  void render(RenderContext &rc,AbstractTrajectory &traj);
  bool load(XMLNode xml,gloModel *model,PersistentObject *data);
  float getlen() {return 2*solid->len+space;}

  MT_Tram2v(jtMotocar *mcar):MT_Common(mcar) {}
};


struct jtMotocar:jtDataObject {
  bool isdefault;
  string roadstyle;

  PtrTo<MT_Common> obj;

  jtCab *cab;
  bool owncab;
  gloBoundBox bound;

  double power;//v kilowattech
  double maxspeed;//v km/h
  double weight;//v tunach

  jtMotocar() {power=maxspeed=weight=0;isdefault=false;cab=NULL;owncab=NULL;}
  ~jtMotocar();

  void load(PersistentObject *data,XMLNode xml);
  void render(RenderContext &rc,AbstractTrajectory &traj);
  float getlen() {return obj->getlen();}
};

extern float CSF;//carsizefactor


}

#endif
