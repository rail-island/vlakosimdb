#include <jlib.h>
#pragma hdrstop

#include "gmotocar.h"
#include "mdijskr.h"
#include "gameu.h"
#include "gimages.h"
#include "gview.h"

namespace jtyc {

float Motocar::getspeed() {
  return traj->speed/beop.motocar_speed_koef;
}

bool Motocar::init(RoadItem *ri,int dir) {
  exists=true;
  return true;
}

Motocar::Motocar() {
  visible=false;
  waitremain=0;
  tonextchange=0;
  exists=false;
  traj=new MotoTraj(this);
//  way=new MotoWay(this);
  setname(lang_text_ch("$Bus 1"));
  game->mobiles->items.add(this);
  settype("");
  created();
  stopped=true;
}

bool Motocar::setdest(RoadItem *ri) {
  bool res=traj->setdest(ri);
  changed();
  return res;
}

void Motocar::start(MWayElem ri) {
  traj->start(ri);
  stopped=false;
  exists=true;
}

string Motocar::getstatestr() {
  if (stopped) return "stopped";
  if (waitremain>0) return "waiting";
  if (traj->isblocked()) return "blocked";
  return "go";
}

void Motocar::wait(float secs) {
  bool was=waitremain>0;
  waitremain+=secs;
  if (was) changed();
}

void Motocar::stop() {
  stopped=true;
  changed();
}

bool Motocar::reverse() {
  bool res=true;
  return res;
}

bool Motocar::ismouseon(float x,float y) {
  fpoint3d pt=traj->count(type->getlen()/2);
  ZPoint za=game->view->ltop(fpoint3d(pt.x,pt.z,pt.y));
  if (fdistance(za,fpoint(x,y))<=beop.max_motocar_click_distance*game->view->maptype.scale) return true;
  return false;
}

void Motocar::run(double deltatime) {
  if (stopped) return;
  if (!exists) return;
  bool waswaiting=waitremain>0;
  if (waitremain>0) {
    waitremain-=deltatime;
    if (waitremain<0) {
      deltatime+=waitremain;
      waitremain=0;
    } else return;
  }
  if (waswaiting) changed();

  try {
    traj->run(deltatime,engine);
  } catch (DEST_REACHED) {
    event("destreached");
    changed();
  }

/*
  retry:
  if (!stopped || getmanual())
  try {
    way->iterate(deltatime,engine);
    if (tonextchange<=0) changed();
    tonextchange-=deltatime;
  } catch (DEST_REACHED) {
    event("destreached");
    changed();
  } catch (NO_DEST) {
    event("nodest");
    changed();
  }
*/
}

void Motocar::unstop() {
  if (!exists) return;

  if (!stopped) return;
  stopped=false;
  changed();
}

void Motocar::countengine(MEngine &eng) {
  eng.hi_koef=0.0001;
  eng.power=type->power*1;
  eng.weight=type->weight*15;
  eng.maxspeed=type->maxspeed*beop.motocar_speed_koef;
  eng.repair();
}

void Motocar::save(XMLNode xml) {
  Mobile::save(xml);
  xml.setname("motocar");
  if (stopped) xml.setvar("stopped",stopped);
  xml.setvar("waitremain",waitremain);
  xml.setvar("exists",exists);
  xml.setvar("type",type->name);
  traj->save(xml.forcesub("traj"));
  game->saving=xml;
  event("save");
}
Motocar::Motocar(XMLNode xml):Mobile(xml) {
  traj=new MotoTraj(this);
  visible=false;
  type=gimages->motocar0;
  waitremain=0;
  tonextchange=0;
  load(xml);
}

void Motocar::settype(string value) {
  type=gimages->findmotocar(value);
  traj->setlen(type->getlen());
  countengine(engine);
}

void Motocar::load(XMLNode xml) {
  loadxml=xml;
  waitremain=xml.fvar("waitremain");
  stopped=xml.ivar("stopped");
  settype(xml.svar("type"));
  if (xml.svar("exists")=="") exists=true;else exists=xml.ivar("exists");
  traj->load(xml["traj"]);
}

bool Motocar::position(fvector3d *res,fvector3d *diff) {
  *res=traj->count(0);
  return true;
}

bool Motocar::getmanual() {
//  if (way) return way->getmanual();
  return false;
}
void Motocar::setmanual(bool value) {
//  if (way) way->setmanual(value);
}

void Motocar::octalize() {
  fpoint3d pt=traj->count(type->getlen()/2);
  gloBoundBox bnd=type->bound;
  bnd.extendrotate();
  bnd.translate(pt.x,pt.z,pt.y);
  float a[3]={bnd.x1,bnd.y1,bnd.z1},b[3]={bnd.x2,bnd.y2,bnd.z2};
  gocttree->add(otItem(this,0),a,b,true);
}

void Motocar::ot_render(int id) {
  visible=true;
}

void Motocar::clearvisibility() {
  visible=false;
}

void Motocar::changed() {
  Mobile::changed();
  tonextchange=randpause(2);
}

void Motocar::render(RenderContext &rc) {
  if (!visible) return;
/*
  point3d<float> a,b,mid;
  a=traj->count(type->d1);
  b=traj->count(type->d2);
  mid=traj->count(type->len/2);

  fvector3d e1=b-a;
  if (e1.size()<epsilon) return;
  fvector3d e2(xyzrec<float>(agvector<float>(e1.xy()).getnormal(),0));

  e2=e2.size1();
  e1=e1.size1();
  vector3d<float> e3=(e1%e2).size1();
  matrix3d<float> mat;
  mat.tobase(e1,e2,e3);//a je matice prechodu od konanicke baze k bazi aktualniho natoceni

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  glTranslatef(a.x,a.z,a.y);
  glMultiplySwapYZ(mat);

  type->render(rc);
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
*/
  type->render(rc,*traj);
}

void Motocar::ignorecross() {
  traj->ignorecross();
}

void Motocar::destroy() {
  Mobile::destroy();
  traj->kill();
  traj=NULL;
}


}

