#include <jlib.h>
#pragma hdrstop

#include "grrmap.h"
#include "gview.h"
#include "grrcom.h"
#include "gameu.h"
#include "gvolatile.h"
#include "ghimap.h"
#include "gsurmap.h"
#include "gsemaphore.h"
#include "grrstat.h"
#include "grdijmap.h"
#include "gmobile.h"
#include "gtrain.h"
#include "grtubus.h"
#include <opengl.h>

namespace jtyc {

float max_tt_leap;
float rr_build_mhd;

RItem *loadritemref(XMLNode xml) {
  return (RItem*)game->rrmap->finditemid(xml.ivar("rid"));
}

bool RItem::canaddtrain() {
  return free_sem_square(this);
}

static RItem *loadRItem(XMLNode xml) {
  if (xml.hasname("surface")) return new SurRItem(xml);
//  if (xml.hasname("idepo")) return new InnerDepoRItem(xml);
//  if (xml.hasname("odepo")) return new OuterDepoRItem(xml);
  if (xml.hasname("rst")) return new RstRItem(xml);
  if (xml.hasname("bridge")) return new BridgeRItem(xml);
  if (xml.hasname("tunnel")) return new TunnelRItem(xml);
  return NULL;
//  throw JError("Unknown RItem type:"+xml.getname());
}

void RInfo::update(RItem *ri) {
  bool isvr[vr_cnt+1];vrcnt=0;
  for(int i=0;i<=vr_cnt;i++) isvr[i]=false;
  for(int i=0;i<vr_cnt;i++) indextovr[i]=0;
  for(int i=1;i<=tt_cnt;i++) {
    Spojnice tt(i);
    if (tt.flag()&ri->flags) {
      Vrchol v;
      v=tt.beg();
      if (not isvr[v.data]) {
        isvr[v.data]=true;
        indextovr[vrcnt++]=v.data;
      }
      v=tt.end();
      if (not isvr[v.data]) {
        isvr[v.data]=true;
        indextovr[vrcnt++]=v.data;
      }
    }
  }
  for(int i=0;i<=vr_cnt;i++) vrtoindex[i]=-1;
  for(int i=0;i<vrcnt;i++) if (indextovr[i]) vrtoindex[indextovr[i]]=i;
}

RItem::RItem(int ax,int ay,float z,Type atype):TrackItem(ax,ay,z,atype,game->rrmap) {
//  x=ax;y=ay;za=z;dzx=0;dzy=0;type=atype;exist=true;
  trains=0;
//  dir=0;
  flags=0;
  rail=gimages->rail0;
//  next=NULL;saveid=-1;
  temp_penalty=0;
//  game->rrmap->createdritem(this);
  updateinfo(false);
}

RItem::RItem(XMLNode xml,Type atype):TrackItem(xml,atype,game->rrmap) {
//  type=atype;
//  exist=true;
//  x=xml.ivar("x");
//  y=xml.ivar("y");
//  za=xml.fvar("za");
//  dzx=xml.fvar("dzx");
//  dzy=xml.fvar("dzy");
  flags=xml.ivar("flags");
//  dir=xml.ivar("dir");
  temp_penalty=xml.fvar("temp_penalty");
//  saveid=xml.ivar("saveid");
  trains=xml.ivar("trains");
  rail=gimages->findrail(xml.svar("rail"));
//  next=NULL;
//  game->rrmap->createdritem(this);
  updateinfo(false);
}
void RItem::xmlsave(XMLNode xml) {
  TrackItem::xmlsave(xml);
  xml.setname(typetoname(type));
/*
  xml.setvar("x",x);
  xml.setvar("y",y);
  xml.setvar("za",za);
  xml.setvar("dzx",dzx);
  xml.setvar("dzy",dzy);
  xml.setvar("saveid",saveid);
*/
  xml.setvar("flags",flags);
  if (rail!=gimages->rail0) xml.setvar("rail",rail->name); 
//  if (dir) xml.setvar("dir",dir);
  if (trains) xml.setvar("trains",trains);
  if (temp_penalty) xml.setvar("temp_penalty",temp_penalty);
}
const char *RItem::typetoname(Type type) {
  switch (type) {
    case tunnel:return "tunnel";
    case surface:return "surface";
    case rst:return "rst";
    case bridge:return "bridge";
//    case idepo:return "idepo";
//    case odepo:return "odepo";
  }
  throw JError("Unknown RItem type");
}

void TunnelRItem::countvisible() {
//  visible=edge && visiblesurface();
  visible=tunnel->isvisible();
}

void TunnelRItem::render(RenderContext &rc) {
//  if (edge || game->view->type) std_draw(rc);
  std_draw(rc);
}

/*
void OuterDepoRItem::notifyremove(GInstance *obj) {
  RItem::notifyremove(obj);
  if (obj==outtrain) outtrain=NULL;
}

void OuterDepoRItem::afterloadlink() {
  outtrain=dynamic_cast<Train*>(game->findobj(outtrainname));
}

void OuterDepoRItem::xmlsave(XMLNode xml) {
  RItem::xmlsave(xml);
  if (outtrain) xml.setvar("trainout",outtrain->getname());
}
*/

int SurRItem::getsemdir() {
  if (semaphore) return semaphore->dir;
  return 0; 
}

void RstRItem::xmlsave(XMLNode xml) {
  RItem::xmlsave(xml);
  xml.setvar("track",track);
}

/*
void RRMap::erasedritem(RItem *item) {
  map.at(item->x,item->y)=unlink_from_unidir_list(map(item->x,item->y),item);

  RItem **act=&ri_list;
  while (*act && *act!=item) act=&(*act)->next_ri;
  *act=(*act)->next_ri;
/*  RItem **first=&map.at(item->x,item->y);
  while (*first!=item) first=&(*first)->next;
  *first=(*first)->next;*/
//}


void SurRItem::run(double deltatime) {
  if (semaphore) semaphore->run(deltatime);
}

RItem::~RItem() {
//  game->rrmap->erasedritem(this);
}

static void filltubus(TrackTubus *tubus,jtRail *rail) {
  for(int i=0;i<tubus->items.count;i++) ((RItem*)tubus->items[i])->rail=rail;
}

void RRMap::setrailtype(int x,int y,string type) {
  RItem *ri=(RItem*)map.at(x,y);
  jtRail *rail=gimages->findrail(type);
  while (ri) {
    if (BridgeRItem *b=dynamic_cast<BridgeRItem*>(ri)) {
      if (b->edge) filltubus(b->bridge,rail);
    } else if (TunnelRItem *t=dynamic_cast<TunnelRItem*>(ri)) {
      if (t->edge) filltubus(t->tunnel,rail);
    } else ri->rail=rail;
    ri=(RItem*)ri->next;
  }
}

/*
RItem *RRMap::first_at(int x,int y) {
  if (x>=0 && x<map.w && y>=0 && y<map.h) return map.at(x,y);
  return NULL;
}
*/

void RRMap::foreach(RItemFunc rif,void *data) {
  RItem *ri=(RItem*)ri_list;
  while (ri) rif(data,ri),ri=(RItem*)ri->next_ri;
//  int size=map.size;
//  for(int i=0;i<size;i++) {
//    RItem *ri=map.at(i);
//    while (ri) rif(data,ri),ri=ri->next;
//  }
}
/*
void RRMap::createdritem(RItem *ri) {
//  ritems.addlast(ri);
  RItem *next=first_at(ri->x,ri->y);
  setri(ri->x,ri->y,ri);
  ri->next=next;
  modify();

  ri->next_ri=ri_list;
  ri_list=ri;
}
*/

float RItem::getz(Vrchol v) {
  float res=za;
  if (v.dx()>0) res+=dzx;
  if (v.dy()>0) res+=dzy;
  return res;
}

void SurRItem::render(RenderContext &rc) {
  std_draw(rc);
  if (semaphore) semaphore->render(rc);
}

/*
void InnerDepoRItem::render(RenderContext &rc) {

}

void OuterDepoRItem::render(RenderContext &rc) {
  std_draw(rc);
}
*/

void BridgeRItem::render(RenderContext &rc) {
  std_draw(rc);
}

void RstRItem::render(RenderContext &rc) {
  std_draw(rc);
}

void render_func(void *data,RItem *ri) {
  RenderContext &rc=*(RenderContext*)data;
  if (ri->visible) ri->render(rc);
}

void RRMap::render(RenderContext &rc) {
  rc.push();
  rc.setculling(false);
  rc.settexture2d(true);
  rc.setalphatest(true);
//  glEnable(GL_TEXTURE_2D);
//  glEnable(GL_ALPHA_TEST);
//  glAlphaFunc(GL_GEQUAL,0.5);
//  glColor4f(0.6,0.6,0.6,1);
//  glBegin(GL_QUADS);
//  tt_bmps[tt_13].draw(0,1,4,0,0,0,0);
  countvisible();

  foreach(render_func,&rc);
/*
  for(int y=game->himap->boundsqy1;y<=game->himap->boundsqy2;y++) {
    for(int x=game->himap->boundsqx1;x<=game->himap->boundsqx2;x++) {
      RItem *ri=map(x,y);
      while (ri) {
        if (ri->visible) ri->render(rc);
        ri=ri->next;
      }
    }
  }
*/
/*
  for(int i=0;i<map.size;i++) {
    RItem *ri=map[i];
    while (ri) {
      if (ri->visible) ri->render(rc);
      ri=ri->next;
    }
//    if () map[i]->render(rc);
  }
*/
//  glEnd();
//  glDisable(GL_TEXTURE_2D);
//  glDisable(GL_ALPHA_TEST);
  rc.pop();
}


RRMap::RRMap() {
//  newcnt=0;
  ri_list=NULL;
//  map=new RefMatrix<RItem>(W,H);
  dijmap=new DijRMap(this);
}

void RRMap::erase(int x,int y) {
  RItem *ri=at(x,y,RItem::surface);
  if (ri) {
    if (ri->trains!=0) game->seterror("$Track-blocked");else delete ri;
    modify();
  }
}

void RRMap::addtoex(SurRItem *ri,int x,int y,Spojnice tt) {
//  ri->flags|=tt.flag();
  HiMap *hm=game->himap;

  if (game->surmap->blockcount(x,y)>1) throw SqBlocked(x,y);
  //1 blokovani je od RItem, ostatni prekazeji
  float h1=hm->hiat(x,y),h2=hm->hiat(x+1,y),h3=hm->hiat(x+1,y+1),h4=hm->hiat(x,y+1);
  float d1=fabs(h1-h2),d2=fabs(h2-h3),d3=fabs(h3-h4);
  if (d1>epsilon || d2>epsilon || d3>epsilon) throw SqBlocked(x,y);

  ri->flags|=tt.flag();
  ri->updateinfo();
  ri->surblock();

  modify();

}


/*
RItem *RRMap::at(int x,int y,int type) {
  RItem *act=first_at(x,y);
  while (act and act->type!=type) act=act->next;
  return act;
}
*/

void BridgeRItem::countvisible() {
//  if (game->view->type) {
//    visible=game->view->type->visible(fvector3d(x,za,y));
//  } else {
//    ZPoint a=game->view->ltop(fvector3d(x,za,y));
//    visible=game->view->nearvisible(a,1);
//  }
//  visible=true;
//  visible=false;
  if (bridgeitem) visible=bridgeitem->visible;else visible=true;
}


void SurRItem::xmlsave(XMLNode xml) {
  RItem::xmlsave(xml);
  if (semaphore) semaphore->xmlsave(xml.add());
}
SurRItem::SurRItem(int ax,int ay,float z):RItem(ax,ay,z,surface) {
  semaphore=NULL;
  game->himap->block(x,y,2,2);
  surblocked=false;
  rb=NULL;
}
SurRItem::~SurRItem() {
  if (!game->destroying) {
    surunblock();
    game->himap->unblock(x,y,2,2);
    if (semaphore) delete semaphore;
  }
}
void SurRItem::surblock() {
  surunblock();
  if (flags==tt_24 || flags==-tt_24) {
    rb=new RailBlock(x,y,false,za,dzx,dzy);
  } else if (flags==tt_13 || flags==-tt_13) {
    rb=new RailBlock(x,y,true,za,dzx,dzy);
  } else game->surmap->block(x,y);
  surblocked=true;
}

void SurRItem::surunblock() {
  if (surblocked) {
    surblocked=false;
    if (rb) {
      delete rb;
      rb=NULL;
    } else game->surmap->unblock(x,y);
  }
}
SurRItem::SurRItem(XMLNode xml):RItem(xml,surface) {
  semaphore=NULL;
  surblocked=false;
  game->himap->block(x,y,2,2);
  if (xml.searchsub("semaphore")>=0) semaphore=new Semaphore(this,xml["semaphore"]);
  rb=NULL;
  surblock();
}


void RstRItem::trainin(Train *t) {
  RItem::trainin(t);
  rstation->trainin(track);
}
void RstRItem::trainout(Train *t) {
  RItem::trainout(t);
  rstation->trainout(track);
}

void RRMap::modify() {
  TrackMap::modify();
  dijmap->modified=true;
}
/*
void InnerDepoRItem::setfree(bool isfree,Vrchol v) {
  depo->setfree(isfree);
}
*/

static Spojnice count_spojnice(int flags) {
  Spojnice res(0);
  for(int i=1;i<=tt_cnt;i++) {
    Spojnice tt(i);
    if (tt.flag()&flags) {
      if (res.valid()) return Spojnice(0);
      res=tt;
    }
  }
  return res;
}

Spojnice RItem::spojnice() {
  return count_spojnice(flags);
}



void SurRItem::setfree(bool isfree,Vrchol v) {
  semaphore->setfree(isfree,v);
}

DijRVr *RRVRCH::dijvr() {
  if (!ri) return NULL;
  DijRMap *map=game->rrmap->dijmap;
  int index=ri->info.begin+ri->info[v];
  xcheckindex(index,map->items.count);
  DijRVr *res=map->items.buf+index;
//  if (res) res->vr.eprint();
  return res;
}

bool SurRItem::dijskr_hassem2() {
  if (semaphore && semaphore->dir==0) return true;
  return false;
}

RRVRCH RRVRCH::beside() {
  xassert(ri);
  RItem *vedle=game->rrmap->first_at(ri->x+v.dx(),ri->y+v.dy());
  Vrchol dir=v.direct();
  float z=ri->getz(v);
  if (vedle) {
    while (vedle) {
      if (fabs(vedle->getz(dir)-z)<epsilon) return RRVRCH(vedle,dir);
      vedle=(RItem*)vedle->next;
    }
  }
  return RRVRCH(0,Vrchol(vr_none));
}

int RRVRCH::neightboors(RRVRCH buf[],rdist_t lens[],Spojnice tts[],bool ignoredir) const {
  if (not valid()) return 0;
  RRVRCH *w=buf;rdist_t *wl=lens;Spojnice *ttw=tts;
//  RRMap *map=ri->map;
  for(int i=-tt_cnt;i<=tt_cnt;i++) {
    Spojnice tt(i);
    if (tt.flag()&ri->flags && (ignoredir || ri->getsemdir()*i<=0)) {
      Vrchol beg=tt.beg();
      if (beg==v) {
        Vrchol end=tt.end();
        w->ri=ri;
        w->v=end;
        if (lens) *wl++=tt.len();
        if (tts) *ttw++=tt;
        w++;
      }
    }
  }
  return w-buf;
}

rdist_t SurRItem::dijskr_penalty(Spojnice tt) {
  rdist_t res=RItem::dijskr_penalty(tt);
  if (semaphore) res+=semaphore->dijskr_penalty(tt);
  return res;
}


void RRMap::build(int x,int y,Spojnice tt,float dhi,BuildType bt) {
  HiMapBackup bak(game->himap);
  bak.storemap();
//  HiMap hm;
//  hm.assign(*game->himap);
//  RItem *res=NULL;
  try {
    _build(x,y,tt,dhi,bt);
  } catch (const HiBlocked &hb) {
    bak.restoremap();
    game->addvolatile(new BadVr(hb.x,hb.y));
    game->seterror("$Cannot-build-track");
    return;
  } catch (const SqBlocked &sb) {
    bak.restoremap();
    game->addvolatile(new BadSq(sb.x,sb.y));
    game->seterror("$Cannot-build-track");
    return;
  }
//  game->himap->assign(hm);
}

inline void makeabcd(HiMap *hm,HItem *&a,HItem *&b,HItem *&c,HItem *&d,Vrchol vr) {
  int dx=vr.dx(),dy=vr.dy();
  if (dx<0) b=c=0;if (dx>0) a=d=0;
  if (dy<0) d=c=0;if (dy>0) a=b=0;
}

RItem *RRMap::addnew(int x,int y,Spojnice tt,float dhi,BuildType bt) {
  HiMap *hm=game->himap;

  bool planar=tt.test(tt_13) || tt.test(tt_24);

  HItem *a=hm->hat(x,y),*b=hm->hat(x+1,y),*c=hm->hat(x+1,y+1),*d=hm->hat(x,y+1);

  HItem *ia=a,*ib=b,*ic=c,*id=d;
  makeabcd(hm,ia,ib,ic,id,tt.beg());
  //vynulovane vrcholy na zacatku
  HItem *oa=a,*ob=b,*oc=c,*od=d;
  makeabcd(hm,oa,ob,oc,od,tt.end());
  //vynulovane vrcholy na konci

  HItem *ih[4],**ihw=ih;
  if (ia) *ihw++=ia;
  if (ib) *ihw++=ib;
  if (ic) *ihw++=ic;
  if (id) *ihw++=id;

  HItem *oh[4],**ohw=oh;
  if (oa) *ohw++=oa;
  if (ob) *ohw++=ob;
  if (oc) *ohw++=oc;
  if (od) *ohw++=od;

  if (planar) {
    xassert(ihw-ih==2);xassert(ohw-oh==2);
    float in_hi=(ih[0]->hi+ih[1]->hi)/2.0;
    float out_hi=(oh[0]->hi+oh[1]->hi)/2.0;
    Interval out_i1,out_i2;
    hm->makeinterval(oh[0]->x,oh[0]->y,out_i1);
    hm->makeinterval(oh[1]->x,oh[1]->y,out_i2);
    float out_minh=max(out_i1.lo,out_i2.lo),out_maxh=min(out_i1.hi,out_i2.hi);

    Interval in_i1,in_i2;
    hm->makeinterval(ih[0]->x,ih[0]->y,in_i1);
    hm->makeinterval(ih[1]->x,ih[1]->y,in_i2);
    float in_minh=max(in_i1.lo,in_i2.lo),in_maxh=min(in_i1.hi,in_i2.hi);

    if (bt==bt_change) {
      if (in_hi<in_minh) in_hi=in_minh;if (in_hi>in_maxh) in_hi=in_maxh;
      if (out_hi<out_minh) out_hi=out_minh;if (out_hi>out_maxh) out_hi=out_maxh;
      if (fabs(out_hi-in_hi)>max_tt_leap) {
        if (out_hi>in_hi) out_hi=in_hi+max_tt_leap;
        else out_hi=in_hi-max_tt_leap;
      }
//      if (out_hi<out_minh && out_hi>out_maxh && out_minh>out_maxh) throw SqBlocked(x,y);
      if (out_hi<out_minh || out_hi>out_maxh) throw SqBlocked(x,y);
      if (in_hi<in_minh || in_hi>in_maxh) throw SqBlocked(x,y);
    } else if (bt==bt_direct) {
      out_hi=in_hi+dhi;
      if (out_hi<out_minh) out_hi=out_minh;
      if (out_hi>out_maxh) out_hi=out_maxh;
      if (fabs(out_hi-in_hi)>max_tt_leap) {
        if (out_hi>in_hi) out_hi=in_hi+max_tt_leap;
        else out_hi=in_hi-max_tt_leap;
      }
      if (out_hi<out_minh || out_hi>out_maxh) throw SqBlocked(x,y);
    } else if (bt==bt_fixed) {
      if (fabs(ih[0]->hi-ih[1]->hi)>epsilon) throw HiBlocked(ih[0]->x,ih[0]->y);
      if (fabs(oh[0]->hi-oh[1]->hi)>epsilon) throw HiBlocked(ih[0]->x,ih[0]->y);
      if (fabs(out_hi-in_hi)>max_tt_leap+epsilon) throw SqBlocked(x,y);
    }
    hm->sethi(ih[0]->x,ih[0]->y,in_hi);
    hm->sethi(ih[1]->x,ih[1]->y,in_hi);
    hm->sethi(oh[0]->x,oh[0]->y,out_hi);
    hm->sethi(oh[1]->x,oh[1]->y,out_hi);
//    hm->repairrect(x,y,2,2,rr_build_mhd);

    SurRItem *ri=new SurRItem(x,y,tt.data>0?out_hi:in_hi);
    switch (tt.data) {
      case tt_24:ri->dzx=in_hi-out_hi;break;
      case -tt_24:ri->dzx=out_hi-in_hi;break;
      case tt_13:ri->dzy=in_hi-out_hi;break;
      case -tt_13:ri->dzy=out_hi-in_hi;break;
    }
    ri->flags=tt.flag();
    ri->updateinfo();
    modify();
    ri->surblock();
    return ri;
  } else {
    float h=ih[0]->hi;
    if (ihw>=ih+2) h=(h+ih[1]->hi)/2;
    if (bt==bt_fixed) {
      if (fabs(h-a->hi)>epsilon) throw SqBlocked(x,y);
      if (fabs(h-b->hi)>epsilon) throw SqBlocked(x,y);
      if (fabs(h-c->hi)>epsilon) throw SqBlocked(x,y);
      if (fabs(h-d->hi)>epsilon) throw SqBlocked(x,y);
    }
    hm->sethi(x,y,2,2,h);
//    hm->repairrect(x,y,2,2,rr_build_mhd);

    SurRItem *ri=new SurRItem(x,y,h);
    ri->flags=tt.flag();
    ri->updateinfo();
    modify();
    ri->surblock();
    return ri;
  }

}

void SurRItem::clearmobiles() {
  RItem::clearmobiles();
  if (semaphore) semaphore->clearmobiles();
}

void RRMap::save(Archive &ar) {
  TrackMap::save(ar);
//  lastsaveid=1;
//  foreach(setsaveid,NULL);
  MainXMLNode xml;
  xmlsave(xml);
  ar.savexml("rrmap.xml",xml);
}

point3d<float> RRSPOJ::countpos(double t,fvector3d *diff) {
  t=t/len()*tt.len();
  fpoint sqp=tt.pos(t);
  float z=ri->za+sqp.x*ri->dzx+sqp.y*ri->dzy;
  if (diff) {
    fvector d2;
    d2=tt.diff(t);
    diff->x=d2.x;
    diff->y=d2.y;
    diff->z=0;
  }
  return point3d<float>(sqp,z);
}


void RRMap::xmlload(XMLNode xml) {
  for(int i=0;i<xml.count();i++) {
    loadRItem(xml[i]);
//    new SurRItem(xml[i]);
  }
  modify();
}

static void xml_save(void *XML,RItem *ri) {
  XMLNode xml=*(XMLNode*)XML;
//  if (ri->type==RItem::surface) ((SurRItem*)(ri))->xmlsave(xml.add());
  ri->xmlsave(xml.add());
}

void RRMap::xmlsave(XMLNode xml) {
  xml.setname("rrmap");
  foreach(xml_save,&xml);
}

void RRMap::load(Archive &ar) {
  MainXMLNode xml;
  ar.loadxml("rrmap.xml",xml);
  xmlload(xml);
}

void RRMap::storerrtemplate(int x0,int y0,int w,int h,XMLNode xml) {
  xml.setvar("w",w);
  xml.setvar("h",h);
  for(int y=0;y<h;y++) for(int x=0;x<w;x++) {
    if (RItem *ri=at(x+x0,y+y0,RItem::surface)) {
      SurRItem *si=(SurRItem*)ri;
      XMLNode it=xml.add("ritem");
      it.setvar("x",x);it.setvar("y",y);
      it.setvar("flags",ri->flags);
      if (si->semaphore) {
        XMLNode sx=it.add("semaphore");
        sx.setvar("type",si->semaphore->gettype()->name);
        sx.setvar("dir",si->semaphore->dir);
        sx.setvar("tt",si->semaphore->tt.data);
      }
    }
  }
}

static Vrchol transp_v(Vrchol v,int dir8) {
  int dx0=v.dx(),dy0=v.dy(),dx=dx0,dy=dy0;
  switch (dir8) {
    case 0:dx=dx0;dy=dy0;break;
    case 1:dx=-dx0;dy=dy0;break;
    case 2:dx=-dx0;dy=-dy0;break;
    case 3:dx=dx0;dy=-dy0;break;

    case 4:dx=dy0;dy=dx0;break;
    case 5:dx=-dy0;dy=dx0;break;
    case 6:dx=-dy0;dy=-dx0;break;
    case 7:dx=dy0;dy=-dx0;break;
  }
  return Vrchol(dx,dy);
}

static Spojnice transp_tt(Spojnice tt,int dir8) {
  Vrchol beg=transp_v(tt.beg(),dir8),end=transp_v(tt.end(),dir8);
  return Spojnice(beg,end);
}

struct RRTplItem {
  int x,y,flags;
  jtSemaphore *semtype;
  int semdir;
  Spojnice semtt;
};

struct RRTemplate {
  TCntVec<RRTplItem> items;
  int w,h;
  void load(XMLNode xml,int dir8);
};

void RRTemplate::load(XMLNode xml,int dir8) {
  w=xml.ivar("w"),h=xml.ivar("h");
  for(int i=0;i<xml.count();i++) {
    int rx=0,ry=0;
    XMLNode it=xml[i];
    int x=xml[i].ivar("x"),y=xml[i].ivar("y"),flags0=xml[i].ivar("flags");
    switch (dir8) {
      case 0:rx=x;ry=y;break;
      case 1:rx=w-x-1;ry=y;break;
      case 2:rx=w-x-1;ry=h-y-1;break;
      case 3:rx=x;ry=h-y-1;break;

      case 4:rx=y;ry=x;break;
      case 5:rx=h-y-1;ry=x;break;
      case 6:rx=h-y-1;ry=w-x-1;break;
      case 7:rx=y;ry=w-x-1;break;
    }
    int flags=0;
    for(int tt=1;tt<=tt_cnt;tt++) if (Spojnice(tt).flag()&flags0) {
//      Spojnice tt(ti);
//      Vrchol beg=transp_v(tt.beg(),dir8),end=transp_v(tt.end(),dir8);
      flags|=transp_tt(Spojnice(tt),dir8).flag();
    }
    RRTplItem &item=items.add();
    item.x=rx;item.y=ry;item.flags=flags;
    item.semtype=NULL;
    if (it.searchsub("semaphore")>=0) {
      item.semtype=gimages->findsemaphore(it["semaphore"].svar("type"));
      item.semdir=it["semaphore"].ivar("dir");
      item.semtt=transp_tt(Spojnice(it["semaphore"].ivar("tt")),dir8);
//      item.semtt.data=abs(item.semtt.data);
//      if (dir8==2 || dir8==3 || dir8==4 || dir8==7) item.semdir=-item.semdir;
      if (item.semtt.data<0) {
        item.semtt.data=-item.semtt.data;
        item.semdir=-item.semdir;
      }
      if (dir8==1 || dir8==3 || dir8==4 || dir8==6) item.semdir=-item.semdir;
      int tt=item.semtt.data;

      switch (dir8) {
        case 1:if (tt==tt_34 || tt==tt_23) item.semdir=-item.semdir;break;
        case 2:if (tt==tt_14 || tt==tt_23 || tt==tt_ab || tt==tt_cd) item.semdir=-item.semdir;break;
        case 3:if (tt==tt_12 || tt==tt_23 || tt==tt_ab || tt==tt_cd) item.semdir=-item.semdir;break;
        case 4:if (tt==tt_14 || tt==tt_23 || tt==tt_ab || tt==tt_bc) item.semdir=-item.semdir;break;
        case 5:if (tt==tt_12 || tt==tt_23 || tt==tt_ab || tt==tt_ad) item.semdir=-item.semdir;break;
        case 6:if (tt==tt_ab || tt==tt_ad) item.semdir=-item.semdir;break;
        case 7:if (tt==tt_23 || tt==tt_34 || tt==tt_ab || tt==tt_bc) item.semdir=-item.semdir;break;
      }

    }
  }
  if (dir8>=4) exchange(w,h);
}

void RRMap::userrtemplate(int x0,int y0,int dir8,XMLNode xml) {
  RRTemplate tpl;
  tpl.load(xml,dir8);
  bool *test=new bool[(tpl.w+1)*(tpl.h+1)];
  fillarray(test,(tpl.w+1)*(tpl.h+1),false);
  for(int i=0;i<tpl.items.count;i++) {
    int x=tpl.items[i].x,y=tpl.items[i].y;
    test[y*(tpl.w+1)+x]=true;
    test[y*(tpl.w+1)+x+1]=true;
    test[(y+1)*(tpl.w+1)+x]=true;
    test[(y+1)*(tpl.w+1)+x+1]=true;
  }
  float hi=game->himap->getfreehi(x0,y0,tpl.w,tpl.h,test);
  if (hi<0) return;

  for(int y=0;y<=tpl.h;y++) for(int x=0;x<=tpl.w;x++) if (test[y*(tpl.w+1)+x]) game->himap->sethi(x0+x,y0+y,hi);
  delete []test;

  for(int i=0;i<tpl.items.count;i++) {
    RRTplItem &it=tpl.items[i];
    SurRItem *ri=new SurRItem(x0+it.x,y0+it.y,hi);
    ri->flags=it.flags;

    ri->updateinfo();
    modify();
    ri->surblock();

    if (it.semtype) ri->semaphore=new Semaphore(ri,it.semtt,it.semdir,it.semtype);

    ri->updatesems();
  }
}

static void _glrect(float x1,float y1,float x2,float y2) {
  glVertex3f(x1,0,y1);
  glVertex3f(x2,0,y1);
  glVertex3f(x2,0,y2);
  glVertex3f(x1,0,y2);
}

static void _hglrect(float x1,float y1,float x2,float y2) {
  glVertex3f(x1,game->himap->interpolatehi(x1,y1),y1);
  glVertex3f(x2,game->himap->interpolatehi(x2,y1),y1);
  glVertex3f(x2,game->himap->interpolatehi(x2,y2),y2);
  glVertex3f(x1,game->himap->interpolatehi(x1,y2),y2);
}

void RRMap::drawmapitem(int x,int y,int flags,int *semdir,bool maphi) {
  if (!game->view->customrc) return;
  RenderContext &rc=*game->view->customrc;

  rc.push();
  rc.setblending(false);
  {
    float scale=game->view->carttype.scale;
    if (scale<0.01) glLineWidth(1);
    else if (scale<0.05) glLineWidth(2);
    else glLineWidth(3);
  }

  glColor4f(0,0,0,1);
  glBegin(GL_LINES);

  for(int i=1;i<=tt_cnt;i++) {
    Spojnice tt(i);
    if (!(flags&tt.flag())) continue;
    if (maphi) {
      float bx=x+0.5+0.5*tt.beg().dx(),by=y+0.5+0.5*tt.beg().dy();
      float ex=x+0.5+0.5*tt.end().dx(),ey=y+0.5+0.5*tt.end().dy();
      glVertex3f(bx,game->himap->interpolatehi(bx,by),by);
      glVertex3f(ex,game->himap->interpolatehi(ex,ey),ey);
    } else {
      glVertex3f(x+0.5+0.5*tt.beg().dx(),0,y+0.5+0.5*tt.beg().dy());
      glVertex3f(x+0.5+0.5*tt.end().dx(),0,y+0.5+0.5*tt.end().dy());
    }
  }
  glEnd();
  glLineWidth(1);

  if (semdir) {
    Spojnice tt=count_spojnice(flags);
    if (tt.valid()) {
      float bx=x+0.5+0.5*tt.beg().dx(),by=y+0.5+0.5*tt.beg().dy();
      float ex=x+0.5+0.5*tt.end().dx(),ey=y+0.5+0.5*tt.end().dy();
      fpoint bp=fpoint(bx,by),ep=fpoint(ex,ey),mid=(fvector)(bp+ep)*0.5f;
      fvector v=ep-bp,n=v.getnormal().size1(),nsm=n*0.2f;
      fpoint pos=mid+nsm,neg=mid-nsm;
      switch (tt.data) {
//        case tt_14:case tt_ad:case tt_ab:case tt_bc:exchange(pos,neg);
        case tt_23:case tt_ad:case tt_cd:case tt_bc:exchange(pos,neg);
      }
      float qs2=0.05;
      glColor4f(1,0,0,1);
      glBegin(GL_QUADS);
      if (*semdir>=0) {
        if (maphi) _hglrect(pos.x-qs2,pos.y-qs2,pos.x+qs2,pos.y+qs2);
        else _glrect(pos.x-qs2,pos.y-qs2,pos.x+qs2,pos.y+qs2);
      }
      if (*semdir<=0) {
        if (maphi) _hglrect(neg.x-qs2,neg.y-qs2,neg.x+qs2,neg.y+qs2);
        else _glrect(neg.x-qs2,neg.y-qs2,neg.x+qs2,neg.y+qs2);
      }
      glEnd();
    }
  }

  rc.pop();
}

void RRMap::drawmap(int minx,int miny,int maxx,int maxy) {
  for(RItem *ri=(RItem*)ri_list;ri;ri=(RItem*)ri->next_ri) {
    if (ri->x<minx || ri->x>=maxx || ri->y<=miny || ri->y>=maxy) continue;
    int dir=ri->getsemdir(),*pdir=ri->hassem()?&dir:NULL;
    drawmapitem(ri->x,ri->y,ri->flags,pdir);
  }
}

void RRMap::drawrrtemplate(int x0,int y0,int dir8,XMLNode xml) {
  RRTemplate tpl;
  tpl.load(xml,dir8);
  for(int i=0;i<tpl.items.count;i++) {
    RRTplItem &it=tpl.items[i];
    int dir=it.semdir,*pdir=it.semtype?&dir:NULL;
    drawmapitem(it.x+x0,it.y+y0,it.flags,pdir,true);
  }
}


void RRMap::_build(int x,int y,Spojnice tt,float dhi,BuildType bt) {
  RItem *it=at(x,y,RItem::surface);
  if (it) addtoex((SurRItem*)it,x,y,tt);
  else {
    if (!game->surmap->freesq(x,y)) throw SqBlocked(x,y);
    addnew(x,y,tt,dhi,bt);
  }
//  game->view->staticchanged();
//  if (newitemcount<MAXNEWITEMS) newitems[newitemcount++]=it;else
}

void SurRItem::sem_get_neightboors(TCntVec<Sem_Rec> &res,Vrchol v) {
  if (semaphore) semaphore->get_neightboors(res,v);
}

static void count_visible(void *,RItem *ri) {
  ri->countvisible();
}
void RRMap::countvisible() {
  foreach(count_visible,0);
}


void RItem::draw_spojnice(Spojnice tt,RenderContext &rc) {
//  RRTexture &bmp=tt.bmp();
  HiMap *hm=game->himap;
//  float ha=hm->hiat(x,y),hb=hm->hiat(x+1,y),hc=hm->hiat(x+1,y+1),hd=hm->hiat(x,y+1);
  float ha=za,hb=za+dzx,hc=za+dzx+dzy,hd=za+dzy;
  rail->render(this,tt.data,rc);
//  draw_rail(step,x,y,ha,hb,hc,hd,tt,rc);
//  bmp.draw(step,x,y,ha,hb,hc,hd,rc);
}

void RItem::std_draw(RenderContext &rc) {
//  jtRail *r=gimages->rail0;
//  for(int step=0;step<r->steps();step++) {
    for(int i=1;i<=tt_cnt;i++) {
//      if (flags&Spojnice(i).flag()) draw_spojnice(r->step(step),Spojnice(i),rc);
      if (flags&Spojnice(i).flag()) draw_spojnice(Spojnice(i),rc);
    }
//  }
}


}

//#pragma package(smart_init)
