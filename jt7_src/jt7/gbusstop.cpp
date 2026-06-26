#include <jlib.h>
#pragma hdrstop

#include "gbusstop.h"
#include "gview.h"
#include "gameu.h"
#include "groad.h"
#include "gmotocar.h"
#include "ghimap.h"
#include "mdijskr.h"
#include "gsurmap.h"
#include "gimages.h"
#include "istatic.h"

namespace jtyc {

MWayElem BusStop::getstartelem() {
  if (vertical) return MWayElem(ri,MSpojnice(mt_13));
  return MWayElem(ri,MSpojnice(mt_24));
}
void BusStop::clearvisibility() {
  visible=false;
}
bool BusStop::canremove() {
  return true;
}
void BusStop::dooctalize() {
  type->octalize(this,x,y,z,gocttree);
}
void BusStop::ot_render(int id) {
  visible=true;
}
void BusStop::dodelize() {
  gocttree->removegroup(this);
}

BusStop::BusStop(int x,int y,RoadItem *ri,bool ver,float z):Station(x,y,1,1) {
  vertical=ver;
  this->z=z;
  setname(lang_text_ch("$Bus-stop 1"));
  type=gimages->busstop0;
  this->ri=ri;
//  type=NULL;
  created();
}

BusStop::BusStop(XMLNode xml):Station(xml) {
  vertical=xml.ivar("vertical");
  z=xml.fvar("z");
  ri=dynamic_cast<RoadItem*>(game->roadmap->finditemid(xml.ivar("ri")));
  type=gimages->findbusstop(xml.svar("type"));
  created();
}

void BusStop::created() {
  game->surmap->block(x,y,1,1);
  game->himap->block(x,y,2,2);
  if (vertical) {
    ri->nov=ri->noz=true;
  } else {
    ri->nos=ri->noj=true;
  }
}

void BusStop::notifyremove(GInstance *obj) {
  Station::notifyremove(obj);
}

string BusStop::gettype() {
  return type->name;
}

void BusStop::xmlsave(XMLNode xml) {
  Station::xmlsave(xml);
  xml.setname("BusStop");
  xml.setvar("vertical",vertical);
  xml.setvar("z",z);
  xml.setvar("ri",ri->saveid);
  xml.setvar("type",type->name);
}

void BusStop::settype(string value) {
  delize();
  type=gimages->findbusstop(value);
}

void BusStop::destroy() {
//  delete ri;
//  ri=NULL;
  ri->nos=ri->noj=ri->noz=ri->nov=false;
  game->surmap->unblock(x,y,1,1);
  game->himap->unblock(x,y,2,2);
  game->roadmap->modify();
  Station::destroy();
}

void BusStop::rendercaptions(RenderContext &rc) {
  if (!visible) return;
  rendertitle(rc,getname(),x,y,z);
}

void BusStop::render(RenderContext &rc) {
  if (!visible) return;
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(x,z,y);
  type->render(rc,vertical);
  glPopMatrix();
}

}

