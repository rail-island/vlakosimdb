#include <jlib.h>
#pragma hdrstop

#include "gtrack.h"

#include "gview.h"
#include "gameu.h"
#include "gvolatile.h"
#include "ghimap.h"
#include "gsurmap.h"
#include "gmobile.h"
#include <opengl.h>

namespace jtyc {

void TrackMap::eraseditem(TrackItem *item) {
  map.at(item->x,item->y)=unlink_from_unidir_list(map(item->x,item->y),item);

  TrackItem **act=&ri_list;
  while (*act && *act!=item) act=&(*act)->next_ri;
  *act=(*act)->next_ri;
}

void TrackMap::createditem(TrackItem *ri) {
  TrackItem *next=first_at(ri->x,ri->y);
  setri(ri->x,ri->y,ri);
  ri->next=next;
  modify();

  ri->next_ri=ri_list;
  ri_list=ri;
}

void TrackMap::init() {
  doinit(game->W,game->H);
}

void TrackMap::doinit(int aw,int ah) {
  W=aw;H=ah;
  map.init(W,H);
  map.fill(NULL);
}

bool TrackItem::visiblesurface() {
  return game->himap->faceat(x,y).octvisible;
}

TrackItem *TrackMap::at(int x,int y,int type) {
  TrackItem *act=first_at(x,y);
  while (act && act->type!=type) act=act->next;
  return act;
}

TrackItem *TrackMap::first_at(int x,int y) {
  if (x>=0 && x<map.w && y>=0 && y<map.h) return map.at(x,y);
  return NULL;
}

TrackItem::TrackItem(int ax,int ay,float z,int atype,TrackMap *amap) {
  x=ax;y=ay;za=z;dzx=0;dzy=0;type=atype;
  map=amap;
  next=NULL;saveid=-1;
  visible=false;
  map->createditem(this);
}

TrackItem::TrackItem(XMLNode xml,int atype,TrackMap *amap) {
  type=atype;
  map=amap;
  visible=false;
  x=xml.ivar("x");
  y=xml.ivar("y");
  za=xml.fvar("za");
  dzx=xml.fvar("dzx");
  dzy=xml.fvar("dzy");
  saveid=xml.ivar("saveid");
  next=NULL;
  map->createditem(this);
}

TrackItem::~TrackItem() {
  map->eraseditem(this);
}

void TrackItem::xmlsave(XMLNode xml) {
  xml.setvar("x",x);
  xml.setvar("y",y);
  xml.setvar("za",za);
  xml.setvar("dzx",dzx);
  xml.setvar("dzy",dzy);
  xml.setvar("saveid",saveid);
}

TrackItem *TrackMap::finditemid(int id) {
  int size=map.size;
  for(int i=0;i<size;i++) {
    TrackItem *ri=map.at(i);
    while (ri) {
      if (ri->saveid==id) return ri;
      ri=ri->next;
    }
  }
  throw JError("TrackMap load error:TrackItem id doesn't exists");
//  return NULL;
}

static int lastsaveid;

static void setsaveid(void *,TrackItem *ri) {
  ri->saveid=lastsaveid++;
}

void TrackMap::save(Archive &ar) {
  lastsaveid=1;
  foreach(setsaveid,NULL);
}

void TrackMap::foreach(TrackItemFunc rif,void *data) {
  TrackItem *ri=ri_list;
  while (ri) rif(data,ri),ri=ri->next_ri;
}

static void f_afterloadlink(void *,TrackItem *ri) {
  ri->afterloadlink();
}
void TrackMap::afterloadlink() {
  foreach(f_afterloadlink,NULL);
}

static void f_clearmobiles(void *,TrackItem *ri) {
  ri->clearmobiles();
}
void TrackMap::clearmobiles() {
  foreach(f_clearmobiles,NULL);
}
static void run_func(void *dt,TrackItem *ri) {
  ri->run(*(double*)dt);
}
void TrackMap::run(double deltatime) {
  foreach(run_func,&deltatime);
}




}


