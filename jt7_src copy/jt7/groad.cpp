#include <jlib.h>
#pragma hdrstop

#include "groad.h"

#include "gview.h"
#include "gameu.h"
#include "gvolatile.h"
#include "ghimap.h"
#include "gsurmap.h"
#include "gmobile.h"
#include "gimages.h"
#include "grtubus.h"
#include <opengl.h>

namespace jtyc {

RoadItem *loadroaditemref(XMLNode xml) {
  return (RoadItem*)game->roadmap->finditemid(xml.ivar("rid"));
}

RoadBuildType roadmodefromstr(string mode) {
  if (mode=="change") return rbt_change;
  if (mode=="direct") return rbt_direct;
  if (mode=="fixed") return rbt_fixed;
  if (mode=="plane") return rbt_plane;
  return rbt_change;
}

RoadItem::RoadItem(XMLNode xml,Type type):TrackItem(xml,type,game->roadmap) {
  ns=nj=nv=nz=NULL;
  noj=nos=nov=noz=false;
  usecnt=0;
  game->himap->block(x,y,2,2);
  road=gimages->findroad(xml.svar("type"));
}

RoadItem::RoadItem(int x,int y,float z,Type type):TrackItem(x,y,z,type,game->roadmap) {
  ns=nj=nv=nz=NULL;
  noj=nos=nov=noz=false;
  usecnt=0;
  road=gimages->road0;
  game->himap->block(x,y,2,2);
}

RoadItem::~RoadItem() {
  if (!game->destroying) {
    game->himap->unblock(x,y,2,2);
  }
}

bool RoadItem::allow(MSpojnice mt) {
  for(int i=0;i<usecnt;i++) if (mt.cross(used[i])) return false;
  return true;
}

bool RoadItem::addused(MSpojnice mt) {
  if (usecnt==4) return false;
  used[usecnt++]=mt;
  return true;
}

void RoadItem::removeused(MSpojnice mt) {
  for(int i=0;i<usecnt;i++) if (used[i]==mt) {
    used[i]=used[usecnt-1];
    usecnt--;
    break;
  }
}

SurRoadItem::SurRoadItem(XMLNode xml):RoadItem(xml,surface) {
  game->himap->block(x,y,2,2);
}

SurRoadItem::SurRoadItem(int x,int y,float z):RoadItem(x,y,z,surface) {
  game->himap->block(x,y,2,2);
}

SurRoadItem::~SurRoadItem() {
  if (!game->destroying) {
    game->himap->unblock(x,y,2,2);
  }
}

BridgeRoadItem::~BridgeRoadItem() {
}

TunnelRoadItem::~TunnelRoadItem() {
}


void RoadItem::xmlsave(XMLNode xml) {
  xml.setname(getxmlname());
  xml.setvar("type",road->name);
  TrackItem::xmlsave(xml);
//  switch (type) {
//    case surface:xml.setvar("type","surface");break;
//  }
}

static RoadItem *loadroaditem(XMLNode xml) {
  if (xml.hasname("surface")) return new SurRoadItem(xml);
  if (xml.hasname("bridge")) return new BridgeRoadItem(xml);
  if (xml.hasname("tunnel")) return new TunnelRoadItem(xml);
  return NULL;
}


void RoadMap::foreach(RoadItemFunc rif,void *data) {
  RoadItem *ri=(RoadItem*)ri_list;
  while (ri) rif(data,ri),ri=(RoadItem*)ri->next_ri;
}

void RoadItem::render(RenderContext &rc) {
  road->render(this,0,rc);
}

void render_func(void *data,RoadItem *ri) {
  if (ri->visible) {
    RenderContext &rc=*(RenderContext*)data;
    glPushMatrix();
    glTranslatef(ri->x,ri->za,ri->y);
    ri->render(rc);
    glPopMatrix();
  }
}

void RoadMap::erase(int x,int y) {
  if (RoadItem *ri=at(x,y,RoadItem::surface)) {
    if (ri->usecnt==0) {
      delete ri;
      modify();
    }
  }
}

static void count_visible(void *,RoadItem *ri) {
  ri->countvisible();
}
void RoadMap::countvisible() {
  foreach(count_visible,0);
}

void BridgeRoadItem::countvisible() {
  if (bridgeitem) visible=bridgeitem->visible;else visible=true;
}
void TunnelRoadItem::countvisible() {
  visible=tunnel->isvisible();
}

void RoadMap::render(RenderContext &rc) {
  wantok();
  glColor4f(1,1,1,1);
//  glColor4f(0,0,0,1);
  rc.push();
  rc.setculling(false);
//  rc.settexture2d(true);
//  rc.settexture2d(false);
//  rc.setalphatest(true);
  countvisible();
  glMatrixMode(GL_MODELVIEW);
  foreach(render_func,&rc);
  rc.pop();
  glColor4f(1,1,1,1);
}

void recount_func(void *data,RoadItem *ri) {
  RoadItem *act;
  ri->ns=ri->nj=ri->nz=ri->nv=NULL;

  act=game->roadmap->first_at(ri->x-1,ri->y);
  while (act && fabs(act->za+act->dzx - ri->za)>epsilon) act=(RoadItem*)act->next;
  if (act) ri->nz=act;

  act=game->roadmap->first_at(ri->x+1,ri->y);
  while (act && fabs(act->za - ri->za-ri->dzx)>epsilon) act=(RoadItem*)act->next;
  if (act) ri->nv=act;

  act=game->roadmap->first_at(ri->x,ri->y-1);
  while (act && fabs(act->za+act->dzy - ri->za)>epsilon) act=(RoadItem*)act->next;
  if (act) ri->ns=act;

  act=game->roadmap->first_at(ri->x,ri->y+1);
  while (act && fabs(act->za - ri->za-ri->dzy)>epsilon) act=(RoadItem*)act->next;
  if (act) ri->nj=act;

  if (ri->nj) {
    if (ri->noj) ri->nj=NULL;
    else if (ri->nj->nos) ri->nj=NULL;
  }
  if (ri->ns) {
    if (ri->nos) ri->ns=NULL;
    else if (ri->ns->noj) ri->ns=NULL;
  }
  if (ri->nv) {
    if (ri->nov) ri->nv=NULL;
    else if (ri->nv->noz) ri->nv=NULL;
  }
  if (ri->nz) {
    if (ri->noz) ri->nz=NULL;
    else if (ri->nz->nov) ri->nz=NULL;
  }
}
void RoadMap::recount() {
  foreach(recount_func,NULL);
}

RoadItem *RoadMap::build(int x,int y,float dhi,RoadBuildType type) {
  #define error {game->addvolatile(new BadSq(x,y));return NULL;}

  float max_road_leap=(type==rbt_change)?fabs(dhi):0.4;

  if (at(x,y,RoadItem::surface)) return NULL;
  RoadItem *rs=at(x,y-1,RoadItem::surface);
  RoadItem *rj=at(x,y+1,RoadItem::surface);
  RoadItem *rz=at(x-1,y,RoadItem::surface);
  RoadItem *rv=at(x+1,y,RoadItem::surface);
  int cnt=0;
  if (rs) cnt++;if (rj) cnt++;if (rv) cnt++;if (rz) cnt++;
  bool mustplane=false;
  if (type==rbt_plane) mustplane=true;
  if (cnt>2) mustplane=true;
  if (cnt==2 && !mustplane) {
    float ha=game->himap->hiat(x,y);
    float hb=game->himap->hiat(x+1,y);
    float hc=game->himap->hiat(x+1,y+1);
    float hd=game->himap->hiat(x,y+1);
    if (rs && rj) {
      if (fabs(hc-hd)>epsilon || fabs(ha-hb)>epsilon) error
      if (fabs(ha-hd)>fabs(dhi)) error
      SurRoadItem *ri=new SurRoadItem(x,y,ha);
      ri->dzy=hd-ha;
      modify();
      return ri;
    }
    if (rv && rz) {
      if (fabs(ha-hd)>epsilon || fabs(hc-hb)>epsilon) error
      if (fabs(ha-hb)>fabs(dhi)) error
      SurRoadItem *ri=new SurRoadItem(x,y,ha);
      ri->dzx=hb-ha;
      modify();
      return ri;
    }
    mustplane=true;
  }
  if (cnt==1 && !mustplane) {
    HiMap *hm=game->himap;
    int index=-1;
    RoadItem *n=NULL;
    float hi=0;
    bool ver=0;
    HItem *a=hm->hat(x,y),*b=hm->hat(x+1,y),*c=hm->hat(x+1,y+1),*d=hm->hat(x,y+1);

    HItem *i1=NULL,*i2=NULL,*o1=NULL,*o2=NULL;

    if (rs) index=0,n=rs,hi=n->za,ver=true,i1=a,i2=b,o1=c,o2=d;
    if (rj) index=1,n=rj,hi=n->za+n->dzy,ver=true,i1=c,i2=d,o1=a,o2=b;
    if (rz) index=2,n=rz,hi=n->za+n->dzx,ver=false,i1=a,i2=d,o1=b,o2=c;
    if (rv) index=3,n=rv,hi=n->za,ver=false,i1=b,i2=c,o1=a,o2=d;

    if (ver) {
      if (fabs(n->dzx)>epsilon) error
    } else {
      if (fabs(n->dzy)>epsilon) error
    }

    float in_hi=(i1->hi+i2->hi)/2.0;
    float out_hi=(o1->hi+o2->hi)/2.0;
    Interval out_i1,out_i2;
    hm->makeinterval(o1->x,o1->y,out_i1);
    hm->makeinterval(o2->x,o2->y,out_i2);
    float out_minh=max(out_i1.lo,out_i2.lo),out_maxh=min(out_i1.hi,out_i2.hi);

    Interval in_i1,in_i2;
    hm->makeinterval(i1->x,i1->y,in_i1);
    hm->makeinterval(i2->x,i2->y,in_i2);
    float in_minh=max(in_i1.lo,in_i2.lo),in_maxh=min(in_i1.hi,in_i2.hi);

    if (type==rbt_change) {
      if (in_hi<in_minh) in_hi=in_minh;if (in_hi>in_maxh) in_hi=in_maxh;
      if (out_hi<out_minh) out_hi=out_minh;if (out_hi>out_maxh) out_hi=out_maxh;
      if (fabs(out_hi-in_hi)>max_road_leap) {
        if (out_hi>in_hi) out_hi=in_hi+max_road_leap;
        else out_hi=in_hi-max_road_leap;
      }
//      if (out_hi<out_minh && out_hi>out_maxh && out_minh>out_maxh) throw SqBlocked(x,y);
      if (out_hi<out_minh || out_hi>out_maxh) error
      if (in_hi<in_minh || in_hi>in_maxh) error
//    if (type==rbt_change) {

    }


    hm->sethi(i1->x,i1->y,in_hi);
    hm->sethi(i2->x,i2->y,in_hi);
    hm->sethi(o1->x,o1->y,out_hi);
    hm->sethi(o2->x,o2->y,out_hi);

    SurRoadItem *ri=new SurRoadItem(x,y,rs||rz?in_hi:out_hi);
    if (rv) ri->dzx=in_hi-out_hi;
    if (rz) ri->dzx=out_hi-in_hi;
    if (rj) ri->dzy=in_hi-out_hi;
    if (rs) ri->dzy=out_hi-in_hi;
    modify();
    return ri;
  }
  if (cnt!=1 || mustplane) {
    float hi=game->himap->getfreehi(x,y,1,1);
   if (hi<0) error
    game->himap->sethi(x,y,2,2,hi);
    SurRoadItem *ri=new SurRoadItem(x,y,hi);
    modify();
    return ri;
  }

  #undef error
}

void RoadMap::load(Archive &ar) {
  MainXMLNode xml;
  if (ar.findfile("roadmap.xml")>=0) ar.loadxml("roadmap.xml",xml);
  xmlload(xml);
}

static void filltubus(TrackTubus *tubus,jtRoad *road) {
  for(int i=0;i<tubus->items.count;i++) ((RoadItem*)tubus->items[i])->road=road;
}

void RoadMap::setroadtype(int x,int y,string type) {
  RoadItem *ri=(RoadItem*)map.at(x,y);
  jtRoad *road=gimages->findroad(type);
  while (ri) {
    if (BridgeRoadItem *b=dynamic_cast<BridgeRoadItem*>(ri)) {
      if (b->edge) filltubus(b->bridge,road);
    } else if (TunnelRoadItem *t=dynamic_cast<TunnelRoadItem*>(ri)) {
      if (t->edge) filltubus(t->tunnel,road);
    } else ri->road=road;
    ri=(RoadItem*)ri->next;
  }
}

void RoadMap::save(Archive &ar) {
  TrackMap::save(ar);
  MainXMLNode xml;
  xmlsave(xml);
  ar.savexml("roadmap.xml",xml);
}

static void xml_save(void *XML,RoadItem *ri) {
  XMLNode xml=*(XMLNode*)XML;
  ri->xmlsave(xml.add());
}

void RoadMap::xmlsave(XMLNode xml) {
  xml.setname("roadmap");
  foreach(xml_save,&xml);
}

void RoadMap::xmlload(XMLNode xml) {
  for(int i=0;i<xml.count();i++) {
    loadroaditem(xml[i]);
  }
  modify();
}



}
