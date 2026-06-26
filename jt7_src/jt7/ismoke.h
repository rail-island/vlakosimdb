#ifndef ismokeH
#define ismokeH

#include "iobject.h"
#include "gparticle.h"
#include "gshapesmoke.h"

namespace jtyc {

class SmokeInstance;

struct jtSmoke:jtDataObject {
  PtrTo<ParticleData> particle;
  PtrTo<ShapeSmokeData> shape;

  void load(PersistentObject *data,XMLNode xml);
  SmokeInstance *create();
  jtSmoke() {}
};

struct SmokeInstance:RCObject {
  virtual void render(RenderContext &rc) {}
  //parametry:rychlost a poloha zdroje koure
  virtual void iterate(double deltatime,fpoint3d pos,fvector3d v) {}
  virtual void translate(fvector3d d) {}
};


struct ParticleSmokeInstance:SmokeInstance {
  ParticleEngine eng;
  float impulstime;
  bool started;

  void render(RenderContext &rc);
  //parametry:rychlost a poloha zdroje koure
  void iterate(double deltatime,fpoint3d pos,fvector3d v);
  void translate(fvector3d d);

  ParticleSmokeInstance() {impulstime=0;started=false;}
};

struct ShapeSmokeInstance:SmokeInstance {
  ShapeSmoke eng;

  void render(RenderContext &rc) {eng.render(rc);}
  void iterate(double deltatime,fpoint3d pos,fvector3d v) {eng.iterate(deltatime,pos,v);}
  void translate(fvector3d d) {eng.translate(d);}

  ShapeSmokeInstance() {}
};


}

#endif
