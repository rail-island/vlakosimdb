#ifndef gshapesmokeH
#define gshapesmokeH

#include <ext/xmlnode.h>
#include <glo/gloobject.h>

namespace jtyc {
using namespace glo;

struct ShapeSmokeData {
  int vrcnt;
  float slow_down;
  float fade;
  float interval;
  float yspeed;

  void load(XMLNode xml);
  ShapeSmokeData();
};

struct SmokeVertex {
  float x,y,z;
  float xi,yi,zi;
  float life;
  bool valid;
  SmokeVertex() {xi=yi=zi=x=y=z=life=0;valid=false;}
};


struct ShapeSmoke {
  SmokeVertex *vrs;
  int vrcnt;

  ShapeSmokeData data;

  void iterate(double deltatime,fpoint3d pos,fvector3d v);
  void translate(fvector3d v);
  void load(ShapeSmokeData &data);
  void render(RenderContext &rc);
  ShapeSmoke() {vrs=NULL;vrcnt=0;impulstime=0;}
  ~ShapeSmoke() {if (vrs) delete []vrs;}
private:
  void _iterate(double deltatime);
  float impulstime;
};

}

#endif
