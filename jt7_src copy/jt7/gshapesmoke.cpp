#include <jlib.h>
#pragma hdrstop

#include "gshapesmoke.h"

namespace jtyc {

void ShapeSmokeData::load(XMLNode xml) {
  vrcnt=xml.ivar("vrcnt");
  slow_down=xml.fvar("slow_down");
  fade=xml.fvar("fade");
  interval=xml.fvar("interval");
  yspeed=xml.fvar("yspeed");
}

ShapeSmokeData::ShapeSmokeData() {
  vrcnt=0;
  slow_down=0;
  fade=0;
  interval=0;
  yspeed=0;
}

void ShapeSmoke::load(ShapeSmokeData &data) {
  vrcnt=data.vrcnt;
  if (vrcnt>0) vrs=new SmokeVertex[vrcnt];
  this->data=data;
}

void ShapeSmoke::translate(fvector3d v) {
  for(int i=0;i<vrcnt;i++) vrs[i].x+=v.x,vrs[i].y+=v.y,vrs[i].z+=v.z;
}

void ShapeSmoke::iterate(double deltatime,fpoint3d pos,fvector3d v) {
  impulstime+=deltatime;
  if (impulstime>data.interval) {
    if (vrcnt>0) {
      memmove(vrs+1,vrs,(vrcnt-1)*sizeof(SmokeVertex));
      vrs[0].x=pos.x;
      vrs[0].y=pos.y;
      vrs[0].z=pos.z;
      vrs[0].xi=v.x;
      vrs[0].yi=v.y+data.yspeed;
      vrs[0].zi=v.z;
    }
    impulstime=0;
  }
  _iterate(deltatime);
}

void ShapeSmoke::_iterate(double deltatime) {
  float vk=pow(data.slow_down,deltatime);
  for(int i=0;i<vrcnt;i++) {
    vrs[i].x+=vrs[i].xi*deltatime;
    vrs[i].y+=vrs[i].yi*deltatime;
    vrs[i].z+=vrs[i].zi*deltatime;

    vrs[i].xi*=vk;
    vrs[i].yi*=vk;
    vrs[i].zi*=vk;

    vrs[i].life-=data.fade*deltatime;
  }
}

void ShapeSmoke::render(RenderContext &rc) {
  glColor4f(1,0,0,1);
  glBegin(GL_LINE_STRIP);
  for(int i=0;i<vrcnt;i++) {
    glVertex3f(vrs[i].x,vrs[i].y,vrs[i].z);
  }
  glEnd();
}

}

