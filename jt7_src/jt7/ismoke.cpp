#include <jlib.h>
#pragma hdrstop

#include "ismoke.h"

namespace jtyc {

SmokeInstance *jtSmoke::create() {
  if (particle) {
    ParticleSmokeInstance *res=new ParticleSmokeInstance();
    res->eng.data=*particle;
    return res;
  }
  if (shape) {
    ShapeSmokeInstance *res=new ShapeSmokeInstance();
    res->eng.load(*shape);
    return res;
  }
  return new SmokeInstance();
}

void ParticleSmokeInstance::iterate(double deltatime,fpoint3d pos,fvector3d v) {
  impulstime+=deltatime;
  if (impulstime>eng.data.interval || !started) {
    eng.x0=pos.x;
    eng.y0=pos.y;
    eng.z0=pos.z;
    eng.vx=v.x;
    eng.vy=v.y;
    eng.vz=v.z;
    if (!started) {
      eng.start();
      started=true;
    }
    eng.impuls();
    impulstime=0;
  }
  eng.iterate(deltatime);
}

void ParticleSmokeInstance::render(RenderContext &rc) {
  eng.render(rc);
}

void jtSmoke::load(PersistentObject *data,XMLNode xml) {
  string type=xml.svar("type");
  if (type=="particle") {
    particle=new ParticleData();
    particle->load(xml);
  }
  if (type=="shape") {
    shape=new ShapeSmokeData();
    shape->load(xml);
  }
}

void ParticleSmokeInstance::translate(fvector3d d) {
  eng.translate(d.x,d.y,d.z);
}


}

