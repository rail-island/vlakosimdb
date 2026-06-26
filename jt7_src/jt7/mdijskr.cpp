#include <jlib.h>
#pragma hdrstop

#include "mdijskr.h"
#include "groad.h"
#include "gameu.h"
#include "gimages.h"
#include "gmotocar.h"

namespace jtyc {

//float motodist=0.15;
//const double min_motocar_speed=0.3,F_t=0;
//const double motocar_max_block_time=8;

static void init_dij_func(void *data,RoadItem *ri) {
  ri->initdij();
}

static bool update_soused(RoadItem *src,RoadItem *ri,jtMotocar *motocar) {
  if (!ri) return false;
  if (!ri->road->allow(motocar)) return false;
  if (ri->active || ri->final) return false;
  double newvalue=src->value+1;
  if (newvalue<ri->value) {
    ri->value=newvalue;
    ri->origin=src;
    ri->active=true;
    return true;
  }
  return false;
}

bool MotoWay::findway(RoadItem *src,RoadItem *dest,char dir0) {
  items.clear();

  RoadMap &map=*game->roadmap;
  map.wantok();
  map.foreach(init_dij_func,NULL);

  TCntVec<RoadItem*> active;
  active.add(src);src->active=true;src->value=0;
  bool first=true;
  while (active.count>0) {
    int minidx=-1;double minvalue;
    for(int i=0;i<active.count;i++) if (minidx<0 || active[i]->value<minvalue) minvalue=active[i]->value,minidx=i;
    RoadItem *actv=active[minidx];

    if (actv==dest) break;

    actv->final=true;
    active.quickerase(minidx);

    bool canv=true,canj=true,canz=true,cans=true;
    if (first) {
      if (dir0=='j') canv=canz=cans=false;
      if (dir0=='s') canv=canz=canj=false;
      if (dir0=='z') canv=canj=cans=false;
      if (dir0=='v') canj=canz=cans=false;
    }

    if (cans && update_soused(actv,actv->ns,traj->car->type)) active.add(actv->ns);
    if (canj && update_soused(actv,actv->nj,traj->car->type)) active.add(actv->nj);
    if (canv && update_soused(actv,actv->nv,traj->car->type)) active.add(actv->nv);
    if (canz && update_soused(actv,actv->nz,traj->car->type)) active.add(actv->nz);

    first=false;
  }

  if (dest->origin) {//cesta nalezena
    RoadItem *act=dest;
    RoadItem *last=NULL;
    while (act) {
      if (act->origin) {
        int dx=act->x-act->origin->x,dy=act->y-act->origin->y;
        MVrchol src=MVrchol(dx,dy).direct(),dest;
        if (last) dest=MVrchol(last->x-act->x,last->y-act->y);
        else dest=src.direct();
        items.add(MWayElem(act,MSpojnice(src,dest)));
      }
      last=act;
      act=act->origin;
    }
    items.reverse();
    return true;
  } else return false;
}

void MotoTraj::trim() {
  double dlast=trajlen-t;
  double need=dlast+carlen;
  if (t>trajlen) need=carlen;
  while (items.count>0 && trajlen-items.first().len()>need) {
    MWayElem elem=items.first();
    elem.ri->removeused(elem.mt);
    double dlen=elem.len();
    trajlen-=dlen;t-=dlen;
    items.erasefirst();
  }
}

double MWayElem::len() {
  float motodist=ri->road->motodist;
  double res=mt.len(motodist);
  if (bw) res+=bw->len;
  return res;
}

void MWayElem::save(XMLNode xml) {
  xml.setvar("mt",mt.data);
  xml.setvar("rid",ri->saveid);
}
void MWayElem::load(XMLNode xml) {
  mt.data=xml.ivar("mt");
  ri=loadroaditemref(xml);
}

void MotoTraj::save(XMLNode xml) {
  xml.setvar("t",t);
  xml.setvar("speed",speed);

  XMLNode its=xml.add("items");
  for(int i=0;i<items.count;i++) {
    items[i].save(its.add("item"));
  }

  way->save(xml.forcesub("way"));
}
void MotoTraj::load(XMLNode xml) {
  t=xml.fvar("t");
  speed=xml.fvar("speed");

  XMLNode its=xml["items"];
  for(int i=0;i<its.count();i++) {
    MWayElem elem;
    elem.load(its[i]);
    addelem(elem,true);
  }

  way->load(xml["way"]);
}


void MotoWay::save(XMLNode xml) {
  xml.setvar("actitem",actitem);
  if (dest) xml.setvar("destination",dest->saveid);
  XMLNode its=xml.add("items");
  for(int i=0;i<items.count;i++) {
    items[i].save(its.add("item"));
  }
}

void MotoWay::load(XMLNode xml) {
  actitem=xml.ivar("actitem");
  if (xml.svar("destination")!="") dest=(RoadItem*)game->roadmap->finditemid(xml.ivar("destination"));
  XMLNode its=xml["items"];
  for(int i=0;i<its.count();i++) {
    MWayElem elem;
    elem.load(its[i]);
    items.add(elem);
  }
}

void MotoTraj::ignorecross() {
  ignorencross=true;
}

void MotoTraj::destroy() {
  GInstance::destroy();
  while (items.count>0) {
    MWayElem elem=items.first();
    elem.ri->removeused(elem.mt);
    items.erasefirst();
  }
  way->kill();
  way=NULL;
}

MWayElem MotoWay::nextway(RoadItem *act) {
  if (actitem<items.count) return items[actitem++];
  if (dest) {
    char dir0=0;
    if (items.count>0) {
      MWayElem elem=items.last();
      MVrchol v=elem.mt.end();
      switch (v.data) {
        case mv_1:dir0='j';break;
        case mv_2:dir0='v';break;
        case mv_3:dir0='s';break;
        case mv_4:dir0='z';break;
      }
    }
    actitem=0;
    findway(act,dest,dir0);
    if (actitem<items.count) return items[actitem++];
  }
  return MWayElem();
}

MWayElem MotoTraj::nextway() {
  return way->nextway(items.last().ri);
}

double MotoTraj::getdz() {
  if (items.count>=2) {
    double dlast=trajlen-t,dfirst=trajlen-dlast-carlen;
    double res=0;
    for(int i=1;i<items.count-1;i++) res+=items[i].ri->getdz(items[i].mt);
    res+=dfirst*items.first().ri->getdz(items.first().mt);
    res+=dlast*items.last().ri->getdz(items.last().mt);
    return res;
  } else if (items.count>=1) return items.first().ri->getdz(items.first().mt);
  else return 0;
}

double MotoTraj::countdspeed(double force,double deltatime,MEngine &engine) {
  return beop.motocar_force_koef*force/engine.weight*deltatime-engine.weight*speed*beop.motocar_F_t*deltatime-speed*getdz()*engine.weight*engine.hi_koef*deltatime/carlen;
}

void MotoTraj::run(double deltatime,MEngine &engine) {
  double maxsp=1000;

  for(int i=0;i<items.count;i++) {
    double sp=items[i].maxspeed();
    if (sp>0 && sp<maxsp) maxsp=sp;
  }
  maxsp*=beop.motocar_speed_koef;


  if (speed<beop.min_motocar_speed) speed=beop.min_motocar_speed;
//  double ds=deltatime*speed;

  if (breakdist>=0) {
    speed-=(deltatime*speed*speed)/(2*breakdist);
  } else {
    double Fmax=engine.power,force=Fmax;
    if (engine.power<speed*Fmax) force=engine.power/speed;
    speed+=countdspeed(force,deltatime,engine);
    if (speed>engine.maxspeed) speed=engine.maxspeed;

    if (speed>maxsp) speed=maxsp;
  }

  if (speed<beop.min_motocar_speed) speed=beop.min_motocar_speed;
  t+=deltatime*speed;

  if (breakdist>=0) {
    breakdist-=deltatime*speed;
    if (breakdist<=0) breakdist=-1;
  }

  while (t>trajlen) {
    if (items.count>0 && items.last().ri==way->dest) {
      t=trajlen;
      speed=0;
      throw DEST_REACHED();
    }
    MWayElem w=nextway();
    if (w.ri) {
      if (addelem(w,false)) {
        blocktime=0;
        breakdist=-1;
        if (way->actitem>=way->items.count) breakdist=w.len();
        else {
          MWayElem next=way->items[way->actitem];
          if (!next.ri->allow(next.mt)) breakdist=next.len();
        }
      } else {
        blocktime+=deltatime;
        if (blocktime>beop.motocar_max_block_time) {
          addelem(w,true);
          blocktime=0;
        } else {
          way->actitem--;
          speed=0;
        }
        break;
      }
    } else break;
    trim();
  }
  if (t>trajlen) t=trajlen;
}

void MotoTraj::notifyremove(GInstance *obj) {
  GInstance::notifyremove(obj);
}
void MotoWay::notifyremove(GInstance *obj) {
  GInstance::notifyremove(obj);
  if (obj==dest) dest=NULL;
  for(int i=0;i<items.count;i++) if (items[i].ri==obj) {
    items.eraselast(items.count-i);
    break;
  }
}


bool MotoTraj::addelem(MWayElem elem,bool forceadd) {
  if (forceadd || ignorencross) {
    elem.ri->addused(elem.mt);
    items.addlast(elem);
    trajlen+=elem.len();
    ignorencross=false;
    return true;
  }

  if (!elem.ri->allow(elem.mt))
    return false;

  if (items.count>0) {
    if (elem==items.last()) return false;
    fpoint3d p1=items.last().count(items.last().len());
    fpoint3d p2=elem.count(0);
    double d=distance3d(p1,p2);
    if (d>epsilon) {
      elem.bw=allocbw();
      elem.bw->p1=p1;
      elem.bw->p2=p2;
      elem.bw->len=d;
      speed=0;
    }
  }
  if (!elem.ri->addused(elem.mt)) return false;
  items.addlast(elem);
  trajlen+=elem.len();
  return true;
}

fpoint3d MotoTraj::_count(float t) {
  if (items.count==0) return fpoint3d();
  if (t>trajlen) items.last().count(t-trajlen);
  if (t<0) return items[0].count(t);
  double act=0;
  for(int i=0;i<items.count;i++) {
    double act2=act+items[i].len();
    if (t>=act-epsilon && t<=act2+epsilon) return items[i].count(t-act);
    act=act2;
  }
  return fpoint3d();
}

MotoTraj::MotoTraj(Motocar *car):car(car) {
  t=0;
  carlen=0;
  blocktime=0;
  trajlen=0;
  speed=0;
  actbw=0;
  breakdist=-1;
  ignorencross=false;
  way=new MotoWay(this);
}
MotoWay::MotoWay(MotoTraj *traj):traj(traj) {
  dest=NULL;actitem=0;
}
BreakWay *MotoTraj::allocbw() {
  if (actbw==0x10) actbw=0;
  return bwbuf+actbw;
}

double MWayElem::maxspeed() {
  if (bw) return 20;
  return 200;
}

fpoint3d MWayElem::count(double t) {
  double len0=0;
  if (bw) {
    len0=bw->len;
    if (t>=0 && t<=bw->len) {
      fvector3d d=bw->p2-bw->p1;
      return bw->p1+d*t;
    }
  }
  fpoint p2=mt.pos(t-len0,ri->road->motodist);
  return fpoint3d(p2.x+ri->x,p2.y+ri->y,ri->za+p2.x*ri->dzx+p2.y*ri->dzy+ri->road->moto_y);
}

bool MotoTraj::setdest(RoadItem *ri) {
  way->dest=ri;
  return true;
}

void MotoTraj::start(MWayElem ri) {
  addelem(ri,true);
}


}

