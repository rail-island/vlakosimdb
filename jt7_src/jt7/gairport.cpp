#include <jlib.h>
#pragma hdrstop

#include "gairport.h"
#include "gview.h"
#include "gameu.h"
#include "ghimap.h"
#include "gaircraft.h"
#include "gsurmap.h"

namespace jtyc {

Airport::Airport(int x,int y,int w,int h,bool ver,int len,int wi,float z):Station(x,y,w,h) {
  setname(lang_text_ch("$Airport 1"));
  this->vertical=ver;this->len=len;this->wi=wi;this->z=z;
  aircrafts=0;
  created();
}

void Airport::countpos(float t,fvector3d *pos,float *alfa) {
  if (vertical) {
    pos->x=x+wi/2.0;
    pos->y=y+t;
    pos->z=z;
    *alfa=90;
  } else {
    pos->x=x+t;
    pos->y=y+wi/2.0;
    pos->z=z;
    *alfa=0;
  }
}

void Airport::findway(ATrajectory &traj,float rad,fvector3d pos,float alfa,float land_len) {
  fpoint cur(pos.x,pos.y);
  fvector3d lpos;float a2=0;
  countpos(len-land_len,&lpos,&a2);
  traj.count(pos.xy(),alfa/180*M_PI,lpos.xy(),a2/180*M_PI,rad);
}

void Airport::render(RenderContext &rc) {
  rc.setculling(false);
  bool l=rc.setlighting(false);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glDisable(GL_TEXTURE_2D);
  glTranslatef(x,z+0.01,y);
  glColor3f(0,0,0);
  glBegin(GL_QUADS);
  glVertex3f(0,0,0);
  glVertex3f(w,0,0);
  glVertex3f(w,0,h);
  glVertex3f(0,0,h);
  glEnd();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  rc.setlighting(l);
}

void Airport::xmlsave(XMLNode xml) {
  Station::xmlsave(xml);
  xml.setname("Airport");
  xml.setvar("vertical",vertical);
  xml.setvar("z",z);
  xml.setvar("wi",wi);
  xml.setvar("len",len);
  xml.setvar("aircrafts",aircrafts);
}

Airport::Airport(XMLNode xml):Station(xml) {
  vertical=xml.ivar("vertical");
  z=xml.fvar("z");//createfreetrack();
  wi=xml.ivar("wi");
  len=xml.ivar("len");
  aircrafts=xml.ivar("aircrafts");
  created();
}

void Airport::created() {
  game->surmap->block(x,y,w,h);
  game->himap->block(x,y,w+1,h+1);
}

void Airport::destroy() {
  game->surmap->unblock(x,y,w,h);
  game->himap->unblock(x,y,w+1,h+1);
  Station::destroy();
}

void Airport::rendercaptions(RenderContext &rc) {
  rendertitle(rc,getname(),x,y,z);
}



}

