#ifndef gparticleH
#define gparticleH

#include <ext/xmlnode.h>
#include <glo/gloobject.h>

namespace jtyc {
using namespace glo;

class ParticleEngine;

struct ParticleData {
  float start_r,start_g,start_b,start_a;
  float end_r,end_g,end_b,end_a;
  float vx_min,vx_max,vy_min,vy_max,vz_min,vz_max;
  float rise_const;
//  float x0,y0,z0;
  float elem_size;
  float slow_down;
  float fade_min,fade_max;
  float interval;
  float impuls_rate;
  int count;
  bool auto_alive;
  void load(XMLNode xml);
  ParticleData();
};


struct ParticleItem {
  float life;// Particle Life
  float fade;// Fade Speed
  float x;// X Position
  float y;// Y Position
  float z;// Z Position
  float xi;// X Direction
  float yi;// Y Direction
  float zi;// Z Direction
  void init(ParticleEngine &eng);
};

struct ParticleEngine {
  ParticleData data;
  void iterate(double deltatime);
  void render(RenderContext &rc);
  void translate(float dx,float dy,float dz);
  void start();

  float x0,y0,z0;
  float vx,vy,vz;

  ParticleEngine() {x0=y0=z0=0;vx=vy=vz=0;}
  void impuls();
private:
  void init_item(ParticleItem &item);

  TCntVec<ParticleItem> particle;
};

}

#endif
