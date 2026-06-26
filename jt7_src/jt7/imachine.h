#ifndef imachineH
#define imachineH

#include "iobject.h" 

namespace jtyc {
class jtCab;class jtSmoke;

struct jtMachine:jtModelObject {
  double len;
  double d1,d2;//vzdalenost 1 a druhe napravy od pocatku vozidla
//  float diam;//horni odhad velikosti
  bool isdefault;

//  jtCab *cab;
//  bool owncab;
//  jtSmoke *smoke;
//  bool ownsmoke;
  RefBase<jtCab> cab;
  RefBase<jtSmoke> smoke;

  float smoke_y,smoke_x;
//  PtrTo<jtParticle> smoke;
  gloBoundBox bound;

  double power;//v kilowattech
  double maxspeed;//v km/h
  double weight;//v tunach

  jtMachine() {len=d1=d2=power=maxspeed=weight=0;isdefault=false;}
  ~jtMachine();

  void load(PersistentObject *data,XMLNode xml);
  void render(RenderContext &rc) {model->render(rc);}
};

struct jtAircraft:jtModelObject {
  bool isdefault;
  float v_start;
  float start_len;
  float rise_hi;
  float rise_len;

  float lower_hi;
  float lower_len;
  float v_land;
  float land_len;

  float rise_angle;
  float lower_angle;

  float start_angling;
  float land_angling;

  float speed;//cestovni rychlost

  float air_radius;//polomer oblouku pri letu

  void load(PersistentObject *data,XMLNode xml);
  void render(RenderContext &rc) {model->render(rc);}
  jtAircraft() {isdefault=false;v_start=0;start_len=0;rise_hi=0;rise_len=0;}
};


extern float MSF;//machinesizefactor
extern float ASF;//aircraftsizefactor

}

#endif
