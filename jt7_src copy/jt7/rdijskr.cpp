#include <jlib.h>
#pragma hdrstop

#include "rdijskr.h"
#include "grrmap.h"
#include "gtrain.h"
#include "gameu.h"
#include "grrstat.h"
#include "gview.h"

namespace jtyc {

const double r_epsilon=0.01;

void TrainWay::init(TCntVec<RRSPOJ> &start) {
  realtraj->init(start);
}

void RTrajectory::init(TCntVec<RRSPOJ> &start) {
  len=0;
  for(int i=0;i<start.count;i++) items.addlast(Item(start[i],len)),len+=start[i].len();
  for(int i=0;i<items.count;i++) items[i].ri->trainin(way->train);
//  updatepos();
  t=len;
}

bool TrainWay::reverse() {
  realtraj->speed=0;
  realtraj->reverse();
  actitem=0;
  items.clear();
  if (getmanual()) return true;
  realtraj->no_way=!findway(RRSPOJ(),realtraj->items.last().end());
  return !realtraj->no_way;
}

void TrainWay::destroy() {
  realtraj->kill();
  realtraj=NULL;
}

void TrainWay::iterate(double deltatime,REngine &engine) {
/*  double Fmax=engine.power,force=Fmax;
  if (engine.power<speed*Fmax) force=engine.power/speed;
  speed=speed+force/engine.weight-engine.weight*speed*F_t-speed*traindz()*weight;*/
  if (isreversed) realtraj->reverse();
  isreversed=false;
  if (realtraj->no_way) {
    nowaytime+=deltatime;
    if (nowaytime>nowaytesttime) refindway(1);
  }
  try {
    realtraj->move(deltatime,engine);
  } catch (DEST_REACHED) {
    if (headingtodepo) {
      train->stopped=true;
      headingtodepo=false;
    }
    throw;
  }
//  while (realtraj.finditem(NULL,realtraj.t)<0) next();
//  realtraj.trim(trainlen);
}

void RTrajectory::clear() {
  while (items.count>0) items.first().ri->trainout(way->train),items.erasefirst();
}

void RTrajectory::ignoresems() {
  if (waitto) {
    waitto->trainover(),waitto=NULL,breakdist=-1;
  }
}

double RTrajectory::countdspeed(double force,double deltatime,REngine &engine) {
  return beop.train_force_koef*force/engine.weight*deltatime-speed*speed*beop.train_air_koef_a*deltatime-speed*beop.train_air_koef_b*deltatime-engine.weight*speed*beop.train_F_t*deltatime-speed*traindz()*engine.weight*engine.hi_koef*deltatime/trainlen;
}

static int vrtoc(Vrchol v) {
  switch (v.data) {
    case vr_a:return 0;
    case vr_1:return 1;
    case vr_b:return 2;
    case vr_2:return 3;
    case vr_c:return 4;
    case vr_3:return 5;
    case vr_d:return 6;
    case vr_4:return 7;
  }
  throw JError("Unknown v");
}

static Vrchol ctovr(int c) {
  Vrchol vrs[8]={Vrchol(vr_a),Vrchol(vr_1),Vrchol(vr_b),Vrchol(vr_2),Vrchol(vr_c),Vrchol(vr_3),Vrchol(vr_d),Vrchol(vr_4)};
  if (c<0 || c>7) throw JError("Unknown v");
  return vrs[c];
}

static int dirtodvr(int dir) {
  switch (dir) {
    case 0:return 4;
    case 1:return 3;
    case -1:return 5;
    case 2:return 2;
    case -2:return 6;
  }
  return 0;
}

static Spojnice selectnext(int dir,Vrchol beg,int cnt,Spojnice tts[8]) {
//  if (cnt0) return tts[0];
  if (!beg.valid()) return Spojnice();
  if (cnt==0) Spojnice();
  if (cnt==1) return tts[0];
  bool ends[8];
  int d=dirtodvr(dir);
  //na nejblizsi odbocky se neda odbocit
  for(int i=0;i<8;i++) ends[i]=false;
  for(int i=0;i<cnt;i++) ends[vrtoc(tts[i].end())]=true;
  int begi=vrtoc(beg);
  if (ends[(begi+d)%8]) return Spojnice(beg,ctovr((begi+d)%8));
  #define try_end(idx) if (ends[(begi+d+idx)%8]) return Spojnice(beg,ctovr((begi+d+idx)%8));

  if (dir<0) {
    try_end(2) try_end(1) try_end(0) try_end(7) try_end(6)
  }
  if (dir>0) {
    try_end(6) try_end(7) try_end(0) try_end(1) try_end(2)
  }
  if (dir==0) {
    try_end(0) try_end(1) try_end(7) try_end(2) try_end(6)
  }

  #undef try_end
  return Spojnice();
}

void RTrajectory::beforerun() {
  waitgroup=NULL;
  goimpuls=false;
}

void WaitGroup::findbest() {
  if (trains.count==1) {
    trains[0]->way->realtraj->goimpuls=true;
    return;
  }

  TCntVec<int> frees;

  for(int i=0;i<trains.count;i++) {
    RTrajectory *traj=trains[i]->way->realtraj;
    DijRMap *map=game->rrmap->dijmap;
    map->preparedijskr();
    map->addactive(traj->items.last().end().dijvr());
    DijRVr *end=traj->way->rundijskr();
//    traj->way->dest->reset();
//    map->dijskr(RDestCallBack(traj->way->dest));
    frees.add(map->freecango(end,traj->trainlen));
  }
  int maxfreeidx=-1;
  int maxfree=0;
  //hledame ostre maximum
  for(int i=0;i<trains.count;i++) {
    if (frees[i]>maxfree) maxfree=frees[i];
  }
  for(int i=0;i<trains.count;i++) {
    if (frees[i]==maxfree) {
      if (maxfreeidx<0) maxfreeidx=i;
      else {
        maxfreeidx=-1;
        break;
      }
    }
  }
  if (maxfreeidx>=0) {
    trains[maxfreeidx]->way->realtraj->goimpuls=true;
    return;
  }

  int maxpriidx=-1;
  for(int i=0;i<trains.count;i++) {
    if (maxpriidx<0 || trains[i]->way->realtraj->actpri>trains[maxpriidx]->way->realtraj->actpri) maxpriidx=i;
  }
  if (maxpriidx>=0) trains[maxpriidx]->way->realtraj->goimpuls=true;
}

void WaitGroup::add(Train *t) {
  trains.add(t);
  t->way->realtraj->actpri++;
}

void RTrajectory::findwaitgroup() {
  waitgroup=new WaitGroup();
  waitgroup->add(way->train);
  TCntVec<Sem_Rec> sems;
  waitto->sem_get_neightboors(sems,items.last().tt.end());
  for(int i=0;i<game->mobiles->items.count;i++) if (Train *t=game->mobiles->items[i]->totrain()) {
    if (RItem *w=t->way->realtraj->waitto) {
      for(int j=0;j<sems.count;j++) if (sems[j].ri==w) {
        if (t->way->realtraj->items.last().tt.end()==sems[j].in) {
          if (t->way->realtraj->waitgroup!=waitgroup) {
            waitgroup->add(t);
            t->way->realtraj->waitgroup=waitgroup;
          }
        }
      }
    }
  }
  waitgroup->findbest();
}

bool TrainWay::wantbreak() {
  if (gotox>=0) return false;
  return true;
}

void RTrajectory::move(double deltatime,REngine &engine) {
  double maxsp=1000;
  for(int i=0;i<items.count;i++) {
    double sp=items[i].tt.maxspeed();
    if (sp>0 && sp<maxsp) maxsp=sp;
  }
  maxsp*=beop.train_speed_koef;

  if (manual) {
    double pk=game->view->getpower();
    if (pk>0) {
      double Fmax=engine.power*pk,force=Fmax;
      if (engine.power*pk<speed*Fmax) force=engine.power*pk/speed;
      speed+=countdspeed(force,deltatime,engine);
    }
    if (pk<0) {
      speed+=beop.train_speed_koef*deltatime*pk*beop.train_break_koef;
    }
    if (speed>maxsp) speed=maxsp;
    if (speed>engine.maxspeed) speed=engine.maxspeed;
    if (speed<0) speed=0;
    t+=deltatime*speed;
    while (t>len) {//musime brat dalsi RItem
      RRSPOJ last=items.last();
      RRVRCH vrs[8];
      float lens[8];
      Spojnice tts[8];
      RRVRCH beg=last.end().beside();
      int cnt=0;
      if (beg.valid()) cnt=beg.neightboors(vrs,lens,tts,true);
      //vyber pokracovani dle view-getdirection()
      Spojnice next=selectnext(game->view->getdirection(),beg.v,cnt,tts);

//      RRSPOJ next=selectnext(game->view->getdirection(),cnt,vrs,tts);
      if (next.valid()) add(RRSPOJ(beg.ri,next));
//      if (cnt>0) add(RRSPOJ(vrs[0].ri,tts[0]));
      else {
        t=len;
        speed=0;
        break;
      }
    }
    trim();
    return;
  }

  if (no_way) return;
  double oldt=t;
  if (breakdist>=0) {//brzdime
    if (breakdist>0) {
      if (!breakingtodest || way->wantbreak()) speed-=(deltatime*speed*speed)/(2*breakdist);
    } else {
      if (!breakingtodest || way->wantbreak()) speed=0;
    }
    if (speed<beop.min_train_speed) speed=beop.min_train_speed;
    double ds=deltatime*speed;
    xassert(ds>=0);
    if (ds>breakdist) ds=breakdist;
    t+=ds;breakdist-=ds;
    xassert(breakdist>=0);
  }
  if (waitto) {
    if (waitto->isfree(items.last().tt.beg())) {//musi se najit skupina
      if (!waitgroup) findwaitgroup();
      if (goimpuls) waitto->trainover(),waitto=NULL,breakdist=-1;
    }
  }
  if (breakingtodest) {
    if (fabs(breakdist)<=r_epsilon) {
      breakdist=-1;breakingtodest=false;
      if (way->wantbreak()) speed=0;
      throw DEST_REACHED();
    }
  }
  if (!waitto && ! breakingtodest) {
    double Fmax=engine.power,force=Fmax;
    if (engine.power<speed*Fmax) force=engine.power/speed;
//    speed=speed+force/engine.weight-engine.weight*speed*F_t-speed*traindz()*engine.weight*engine.hi_koef;
//    speed=speed+deltatime*2;

//    speed=speed+force/engine.weight*deltatime-engine.weight*speed*F_t*deltatime-speed*traindz()*engine.weight*engine.hi_koef*deltatime/trainlen;
    speed+=countdspeed(force,deltatime,engine);
    if (speed>engine.maxspeed) speed=engine.maxspeed;

    if (speed>maxsp) speed=maxsp;

    if (speed<beop.min_train_speed) speed=beop.min_train_speed;
    t+=deltatime*speed;
  }
  while (t>len) {
    bool last=false;
    RRSPOJ next;
    try {
      next=way->next(last);
    } catch (...) {
      t=oldt;
      throw;
    }
    if (next.valid()) add(next);
    else {
      t=len;//zastavime se na len
      return;
    }

    if (last) {
//      if (headingtodepo) {
//        headingindepo=true;
//      } else {
      if (t>len) t=len;
      breakdist=len-t;
      breakingtodest=true;
//      }
    }
  }
//  if (headingtodepo && headingindepo && items.count<=1) reacheddepo();
  trim();
}

//void RTrajectory::initloaded() {
//  for(int i=0;i<items.count;i++) items[i]->
//}

bool RTrajectory::canredraw() {
  for(int i=0;i<items.count;i++)
    if (items[i].ri->visible)
      return true;
  return false;
}

/*
void RTrajectory::reacheddepo() {
  headingtodepo->trainin(way->train);
  headingtodepo=NULL;headingindepo=false;
  while (items.count>0) items.erasefirst();
  way->reacheddepo();
  t=0;
}
*/

/*
void TrainWay::reacheddepo() {
  train->reacheddepo();
}
*/

void RTrajectory::add(const RRSPOJ &rr) {
  if (items.count>=items.SIZE) trim();
  items.addlast(Item(rr,len));
  xassert(rr.ri);
  len+=rr.len();
  rr.ri->trainin(way->train);
//  if (rr.ri->hassem()) way->tmpitems.clear();//penalizace uz neni treba
  if (! rr.ri->isfree(rr.tt.beg())) {
    waitto=rr.ri;double d2=rr.ri->breakdist(rr.tt.beg());
    double max=len-rr.len()+d2;
    actpri=way->train->getpriority();
    if (t>max) t=max,speed=0;else breakdist=max-t;
  } else rr.ri->trainover();
}

double RTrajectory::traindz() {
  if (items.count>=2) {
    double dlast=len-t,dfirst=len-dlast-trainlen;
    double res=0;
    for(int i=1;i<items.count-1;i++) res+=items[i].ri->getdz(items[i].tt);
    res+=dfirst*items.first().ri->getdz(items.first().tt);
    res+=dlast*items.last().ri->getdz(items.last().tt);
    return res;
  } else if (items.count>=1) return items.first().ri->getdz(items.first().tt);
  else return 0;
}

bool TrainWay::findway(const RRSPOJ &tt0,RRVRCH pos,WayInfo *info) {
  if (!dest) throw NO_DEST();
  actitem=0;items.clear();if (tt0.valid()) items.add(new WayItem(tt0,false));
  DijRMap *map=game->rrmap->dijmap;

//  int st0=clock();
  map->preparedijskr();
//  int ct=clock()-st0;

  map->addactive(pos.dijvr());

  DijRVr *end=rundijskr();
  if (end) {
    map->_findway(end,items,tmpitems,info);
    return true;
  }
/*
  if (headingtodepo) {
    realtraj->no_way=!findwaytodepo();
    return !realtraj->no_way;
  } else {
    dest->reset();
    map->dijskr(RDestCallBack(dest));
  }
  if (dest->reached>0) {
    map->_findway(dest,items,tmpitems,info);
    return true;
  }
*/
  return false;
}

struct DepoCallBack:DijCallBack {
  TCntVec<DijRVr*> vrs;
  TCntVec<RStation*> visited;
  bool isend() {return end;}
  void reached(DijRVr *vr) {
    if (vr->vr.ri->type==RItem::rst) {
      RstRItem *ri=(RstRItem*)vr->vr.ri;
      if (ri->rstation->isdepo) {
        if (visited.search(ri->rstation)<0) {
          ri->rstation->getdijvrs(vrs);
          visited.add(ri->rstation);
        }
      }
    }
    if (vrs.search(vr)>=0) {
      end=vr;return;
    }
  }
  DijRVr *end;
  DepoCallBack() {end=NULL;}
};

struct RStationCallBack:DijCallBack {
  TCntVec<DijRVr*> vrs;
  RStation *station;
  DijRVr *end;

  bool isend() {return end;}
  void reached(DijRVr *vr) {
    if (vrs.search(vr)>=0) end=vr;
  }
  RStationCallBack(RStation *astation) {
    end=NULL;
    station=astation;
    station->getdijvrs(vrs);
  }
};

struct XYCallBack:DijCallBack {
  bool isend() {return end;}
  void reached(DijRVr *vr) {
    RItem *ri=vr->vr.ri;
    if (ri->x==x && ri->y==y) {
      end=vr;
    }
  }
  DijRVr *end;
  int x,y;
  XYCallBack(int x,int y):x(x),y(y) {end=NULL;}
};


DijRVr *TrainWay::rundijskr() {
  DijRMap *map=game->rrmap->dijmap;
  if (headingtodepo) {
    DepoCallBack dcb;
    map->dijskr(dcb);
    return dcb.end;
  } else if (gotox>=0 && gotoy>=0) {
    XYCallBack xycb(gotox,gotoy);
    map->dijskr(xycb);
    return xycb.end;
  } else if (dest) {
    RStationCallBack rcb(dest);
    map->dijskr(rcb);
    return rcb.end;
  }
  return NULL;
}


/*
void RTrajectory::todepo(RDepo *depo) {
  headingtodepo=depo;
}
*/

/*
bool TrainWay::findwaytodepo() {
  actitem=0;items.clear();
  DijRMap *map=game->rrmap->dijmap;
  map->preparedijskr();
  map->addactive(realtraj->items.last().end().dijvr());
  DepoCallBack dcb;
  map->dijskr(dcb);
  if (dcb.res) {
//    dest=dcb.res->dest();
    RDestination *dst=dcb.res->dest();
    map->_findway(dest,items,tmpitems);
//    destmode="depo";
    delete dst;
//    realtraj->todepo(dcb.res);
    return true;
  }
  return false;
}
*/

bool TrainWay::todepo() {
  if (getmanual()) return false;
  cleardests();
  headingtodepo=true;
  headingtodepo=refindway(1);
  return headingtodepo;
//  headingtodepo=findwaytodepo();
//  return headingtodepo;
}

bool TrainWay::gotoxy(int x,int y) {
  if (getmanual()) return false;
  cleardests();
  gotox=x;gotoy=y;
  bool res=refindway(1);
  if (!res) gotox=gotoy=-1;
  return res;
}

void TrainWay::notodepo() {
  if (headingtodepo) {
    headingtodepo=false;
    refindway(1);
  }
}

void TrainWay::setmanual(bool value) {
  items.clear();
//  tmpitems.clear();
  realtraj->setmanual(value);
}

void RTrajectory::setmanual(bool value) {
  if (manual!=value) {
    waitto=NULL;breakri=NULL;breakdist=-1;
  }
  manual=value;
}

void TrainWay::findway(const RRSPOJ &tt0,WayInfo *info) {
  realtraj->no_way=!findway(tt0,realtraj->items.last().end(),info);
}

RRSPOJ TrainWay::next(bool &last) {
  retry:
  if (actitem<items.count) {
    last=items[actitem]->last;
    xassert(items[actitem]->tt.valid());
    return items[actitem++]->tt;
  } else {
    findway(RRSPOJ());
    if (items.count==0) {
      realtraj->no_way=true;
      return RRSPOJ();
    }
    goto retry;
  }
}

point3d<float> TrainWay::count(double dt,fvector3d *diff) {
  point3d<float> res(0,0,0);
  if (realtraj->count(res,dt,diff)) return res;
  throw TRAIN_NOT_VISIBLE();
}

/*
bool TrainWay::refindway(const RRSPOJ &tt0,const RRVRCH &pos) {
  bool res=findway(tt0,pos);
  realtraj->no_way=!res;
  return res;
}
*/

bool TrainWay::refindway(int reversemode) {
  nowaytime=0;
  nowaytesttime=randpause(2);
  realtraj->no_way=false;
  if (reversemode==0) {
    if (!findway(RRSPOJ(),realtraj->items.last().end())) {
      realtraj->no_way=true;
      return false;
    }
  } else if (reversemode==1) {
    if (!findway(RRSPOJ(),realtraj->items.last().end())) {
      if (!findway(RRSPOJ(),realtraj->reversevr())) {
        realtraj->no_way=true;
        return false;
      }
      isreversed=true;
    }
  } else {
    WayInfo w1,w2;
    if (!findway(RRSPOJ(),realtraj->items.last().end(),&w1)) w1.len=-1;
    if (!findway(RRSPOJ(),realtraj->reversevr(),&w2)) w2.len=-1;
    if (w1.len<0 && w2.len<0) {
      realtraj->no_way=true;
      return false;
    }
    if (w1.len<0) w1.len=w2.len+1;
    if (w2.len<0) w2.len=w1.len+1;
    if (w1.len<w2.len) {
      findway(RRSPOJ(),realtraj->items.last().end());
    } else {
      findway(RRSPOJ(),realtraj->reversevr());
      isreversed=true;
    }
  }
  return true;
}

void TrainWay::cleardests() {
  headingtodepo=false;
  gotox=gotoy=-1;
}

bool TrainWay::setdest(RStation *adest) {
  cleardests();
  dest=adest;
//  int st=clock();
  bool res=refindway(2);
//  int st2=clock()-st;
//  float sec=st2/(float)CLK_TCK;
  return res;
}
/*
bool TrainWay::start(const RRSPOJ &tt0,RDestination *adest,const RRVRCH &pos) {
  dest=adest;
  bool res=findway(tt0,pos);
  realtraj->no_way=!res;
  return res;
}
*/

void RTrajectory::updatepos() {
  len=0;
  for(int i=0;i<items.count;i++) items[i].pos=len,len+=items[i].len();
}
void RTrajectory::trim() {
  double dlast=len-t;
  double need=dlast+trainlen;
  while (items.count>0 && len-items.first().len()>need) {
    RRSPOJ tt=items.first();
    tt.ri->trainout(way->train);
    len-=tt.len();t-=tt.len();
    items.erasefirst();
  }
  updatepos();
}
void RTrajectory::destroy() {
  GInstance::destroy();
  for(int i=0;i<items.count;i++) items[i].ri->trainout(way->train);
  for(int i=0;i<items.count;i++) items[i].ri->updatesems();
  while (items.count>0) items.erasefirst();
}
void RTrajectory::reverse() {
//  t-=(len-trainlen);
  t=trainlen+len-t;
  for(int i=0;i<items.count;i++) items[i].tt.data=-items[i].tt.data;
  items.reverse();
  updatepos();
}

int RTrajectory::finditem(double *beg,double t) {
  if (t>len) return -1;
  if (t<0) throw TRAIN_NOT_VISIBLE();
  double act=0;
  for(int i=0;i<items.count;i++) {
    double act2=act+items[i].len();
    if (t>=act and t<=act2) {
      if (beg) *beg=act;
      return i;
    }
    act=act2;
  }
  return -1;
}

void RTrajectory::notifyremove(GInstance *obj) {
  GInstance::notifyremove(obj);
  if (obj==waitto) waitto=NULL;
  if (obj==breakri) breakri=NULL;
//  if (obj==headingtodepo) headingtodepo=NULL;
}

void TrainWay::notifyremove(GInstance *obj) {
  GInstance::notifyremove(obj);
  for(int i=0;i<items.count;i++) if (items[i]->tt.ri==obj) {
    items.eraselast(items.count-i);
    break;
  }
  tmpitems.notifyremove(obj);
  if (dest==obj) dest=NULL;
/*
  if (dest && dest->notifyremove(obj)) {
    dest=NULL;
  }
*/
}

bool RTrajectory::_count(point3d<float> &res,double t,fvector3d *diff) {
  double beg;
  int idx=finditem(&beg,t);
  if (idx>=0) {
    Item &it=items[idx];
    res=point3d<float>(it.countpos(t-beg,diff).translate(it.ri->x,it.ri->y,it.ri->rail->train_y));
    return true;
  }
  return false;
}

void TrainWay::save(XMLNode xml) {
  xml.setvar("actitem",actitem);
  if (headingtodepo) xml.setvar("headingtodepo",headingtodepo);
  if (dest) xml.setvar("destination",dest->getname());
  if (gotox>=0) xml.setvar("gotox",gotox),xml.setvar("gotoy",gotoy);
  XMLNode its=xml.add("items");
  for(int i=0;i<items.count;i++) {
    items[i]->save(its.add("item"));
  }
  realtraj->save(xml.add("realtraj"));
  if (isreversed) xml.setvar("isreversed",isreversed);
  tmpitems.save(xml.add("tmpitems"));
}
void TrainWay::load(XMLNode xml) {
  actitem=xml.ivar("actitem");
  GObject *obj=game->findobj(xml.svar("destination"));
  if (obj) dest=dynamic_cast<RStation*>(obj);// dest=obj->loadrdest(xml);
  XMLNode its=xml["items"];
  for(int i=0;i<its.count();i++) {
    items.add(new WayItem(its[i]));
  }
  if (xml.svar("gotox")!="") gotox=xml.ivar("gotox"),gotoy=xml.ivar("gotoy");
  headingtodepo=xml.ivar("headingtodepo");
  realtraj->load(xml["realtraj"]);
  isreversed=xml.ivar("isreversed");
  tmpitems.load(xml["tmpitems"]);
}

void RTrajectory::save(XMLNode xml) {
  xml.setvar("len",len);
  xml.setvar("t",t);
  xml.setvar("speed",speed);
  xml.setvar("no_way",no_way);
  xml.setvar("actpri",actpri);
//  if (headingtodepo) xml.setvar("todepo",headingtodepo->getname());
  xml.setvar("breakdist",breakdist);
  if (breakri) xml.setvar("breakri",breakri->saveid);
//  xml.setvar("trainlen",trainlen);
  xml.setvar("breakingtodest",breakingtodest);
  if (waitto) xml.setvar("waitto",waitto->saveid);

  XMLNode its=xml.add("items");
  for(int i=0;i<items.count;i++) {
    items[i].save(its.add("item"));
  }
}
void RTrajectory::load(XMLNode xml) {
  len=xml.fvar("len");
  t=xml.fvar("t");
  speed=xml.fvar("speed");
  no_way=xml.ivar("no_way");
  actpri=xml.ivar("actpri");
//  if (xml.isvar("todepo")) headingtodepo=dynamic_cast<RDepo*>(game->findobj(xml.svar("todepo")));
  breakdist=xml.fvar("breakdist");
  if (xml.isvar("breakri")) breakri=(RItem*)game->rrmap->finditemid(xml.ivar("breakri"));
//  trainlen=xml.fvar("trainlen");
  breakingtodest=xml.ivar("breakingtodest");
  if (xml.isvar("waitto")) waitto=(RItem*)game->rrmap->finditemid(xml.ivar("waitto"));

  XMLNode its=xml["items"];
  for(int i=0;i<its.count();i++) {
    Item it;
    it.load(its[i]);
    items.addlast(it);
  }
}

void TempPenaltyItems::clear() {
  for(int i=0;i<ritems.count;i++) ritems[i]->temp_penalty=0;
  ritems.clear();
}

void TempPenaltyItems::notifyremove(GInstance *obj) {
  for(int i=0;i<ritems.count;i++) if (ritems[i]==obj) {
    clear();
    return;
  }
}

void TempPenaltyItems::add(RItem *ri) {
  ri->temp_penalty=5;
  ritems.add(ri);
}

void TempPenaltyItems::save(XMLNode xml) {
  bstring its;
  for(int i=0;i<ritems.count;i++) its<<itos(ritems[i]->saveid)<<";";
  xml.setvar("items",its);
}

void TempPenaltyItems::load(XMLNode xml) {
  for(lineiter it=xml.svar("items").getlines(';');*it;it++) {
    RItem *ri=dynamic_cast<RItem*>(game->rrmap->finditemid(atoi(*it)));
    if (!ri) throw JError("Error loading train way");
    ritems.add(ri);
  }
}




}

//#pragma package(smart_init)
