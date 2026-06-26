#include <jlib.h>
#pragma hdrstop

#include "gmobile.h"
#include "gtrain.h"
#include "gmotocar.h"
#include "gaircraft.h"
#include "gameu.h"

namespace jtyc {

static Mobile *loadmobile(XMLNode xml) {
  if (xml.hasname("train")) return new Train(xml);
  if (xml.hasname("motocar")) return new Motocar(xml);
  if (xml.hasname("aircraft")) return new Aircraft(xml);
  return NULL;
}

void Mobile::drive() {
  game->setdrive(this);
}

void Mobile::watch() {
  game->setwatch(this);
}

/*
void Mobile::setname(string value,bool addnum) {
  string base=value;
  int num=0;if (addnum) num++;
  if (addnum) value=base+"1";
  while (game->mobiles->search(value)) {
    num++;
    value=base+itos(num);
  }
  name=value;
}
*/

void Mobiles::notifyremove(GInstance *obj) {
  GInstance::notifyremove(obj);
  items.quickremove((Mobile*)obj);
//  if (Mobile *mob=dynamic_cast<Mobile*>(obj)) items.quickremove(mob);
}

/*
void Mobile::crash() {
  RefBase<Mobile> I=this;//aby nebyl volan destruktor do konce teto fce
  game->mobiles->items.remove(I);
  exist=false;
}
*/
/*Mobile *createmobile(int type,BINREAD &fr) {
  switch (type) {
    case Train::TYPE:return new Train(fr);
  }
  return NULL;
}*/

void Mobiles::render(RenderContext &rc) {
  for(int i=0;i<items.count;i++) items[i]->render(rc);
}
void Mobiles::rendertransparent(RenderContext &rc) {
  for(int i=0;i<items.count;i++) items[i]->rendertransparent(rc);
}
void Mobiles::octalize() {
  for(int i=0;i<items.count;i++) items[i]->octalize();
}
void Mobiles::delize() {
  for(int i=0;i<items.count;i++) items[i]->delize();
}
void Mobiles::clearvisibility() {
  for(int i=0;i<items.count;i++) items[i]->clearvisibility();
}

Mobiles::Mobiles() {
}

void Mobiles::init() {
}

void Mobiles::load(Archive &ar) {
  if (ar.findfile("mobiles.xml")<0) return;
  MainXMLNode xml;
  ar.loadxml("mobiles.xml",xml);
  xmlload(xml);
}

void Mobiles::save(Archive &ar) {
  MainXMLNode xml;
  xml.setname("mobiles");
  xmlsave(xml);
  ar.savexml("mobiles.xml",xml);
}

void Mobiles::xmlsave(XMLNode xml) {
  for(int i=0;i<items.count;i++) items[i]->save(xml.add());
}

void Mobiles::xmlload(XMLNode xml) {
  for(int i=0;i<xml.count();i++) if (Mobile *mob=loadmobile(xml[i])) items.add(mob);
}

void Mobiles::clearall() {
  while (items.count>0) items[0]->kill();
}


//Mobiles::Mobiles(BINREAD &fr) {
//  game=frgame(fr);
//  loadarray(fr,items,createmobile);
//}


}

//#pragma package(smart_init)
