#ifndef gairportH
#define gairportH

#include "gstatic.h"

namespace jtyc {

struct Airport:Station {
  Airport(int x,int y,int w,int h,bool ver,int len,int wi,float z);
  void render(RenderContext &rc);
  void xmlsave(XMLNode xml);
  Airport(XMLNode xml);
  void created();
  void destroy();
  void rendercaptions(RenderContext &rc);
  bool isfree() {return aircrafts==0;}
  void addair() {aircrafts++;}
  void freeair() {aircrafts--;}

  bool vertical;
  int len;
  int wi;
  float z;
  int aircrafts;

  float runwaylen() {return len;}
  void countpos(float t,fvector3d *pos,float *alfa);

  void addwait(Aircraft *a) {waiting.add(a);}
  void findway(ATrajectory &traj,float rad,fvector3d pos,float alfa,float land_len);

  TCntVec<Aircraft*> waiting;
};

struct Runway:GInstance {
};


}

#endif
