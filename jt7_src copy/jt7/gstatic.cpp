#include <jlib.h>
#pragma hdrstop

#include "gtrain.h"
#include "gsurmap.h"
#include "ghimap.h"
#include "gstatic.h"
#include "gview.h"
#include "grrstat.h"
#include "gameu.h"
#include "grtubus.h"
#include "gairport.h"
#include "gbusstop.h"
#include "groad.h"

namespace jtyc {

static Static *load_static(XMLNode xml) {
//  if (xml.getname()=="RDepo") return new RDepo(xml);
  if (xml.getname()=="RStation") return new RStation(xml);
  if (xml.getname()=="BusStop") return new BusStop(xml);
  if (xml.getname()=="RBridge") return new RBridge(xml);
  if (xml.getname()=="RTunnel") return new RTunnel(xml);
  if (xml.getname()=="RoadBridge") return new RoadBridge(xml);
  if (xml.getname()=="RoadTunnel") return new RoadTunnel(xml);
  if (xml.getname()=="Building") return new Building(xml);
  if (xml.getname()=="Airport") return new Airport(xml);
  return NULL;
}

void Statics::afterloadlink() {
  for(int i=0;i<items.count;i++) items[i]->afterloadlink();
}


/*
Statics::Statics(Game *agame,XMLNode xml) {
  game=agame;
  for(int i=0;i<xml.count();i++) if (Static *added=load_static(game,xml[i])) items.add(added);
}
*/

/*
void Statics::click(int x,int y) {
  for(int i=0;i<items.count;i++) {
    Static &it=*items[i];
    if (it.clickoccupy(x,y)) it.clickon(x-it.x,y-it.y);
  }
}
*/

void Statics::save(Archive &ar) {
  MainXMLNode xml;
  xml.setname("statics");
  for(int i=0;i<items.count;i++) items[i]->xmlsave(xml.add());
  ar.savexml("statics.xml",xml);
}
void Statics::load(Archive &ar) {
  MainXMLNode xml;
  ar.loadxml("statics.xml",xml);
  for(int i=0;i<xml.count();i++) {
    if (Static *added=load_static(xml[i]))
      items.add(added);
  }
}

Statics::Statics() {
}

void Statics::init() {
}

Static *Statics::search(int x,int y) {
  for(int i=0;i<items.count;i++)
    if (items[i]->clickoccupy(x,y)) return items[i];
  return NULL;
}

/*
void Statics::builddepo(int x,int y,int dir) {
  bool hor=(dir==0 || dir==2);
  int w=1,h=2;if (hor) exchange(w,h);

  bool can=true;
  float hi=game->himap->getfreehi(x,y,w,h);
  if (hi<0) can=false;
  if (can) game->himap->sethi(x,y,w+1,h+1,hi);

  if (!game->surmap->freeplane(x,y,w,h)) can=false;

//  if (!can) game->seterror("$Cannot-build-depo");
//  else items.add(new RDepo(x,y,dir,w,h,hi));

//  if (game->himap->checkplane(x,y,w,h,&h0) && game->surmap->freeplane(x,y,w,h)) {
//    filltrack(game,Spojnice(hor?tt_24:tt_13),RItem::depo,mx,my,w,h,h0);
//    items.add(new RDepo(x,y,dir,w,h,h0));
//  } else game->seterror("$Cannot-build-depo");
}
*/

Building::Building(XMLNode xml):Static(xml) {
  setname(xml.svar("name"));
  string stype=xml.svar("type");
  type=gimages->findbuilding(stype);
  if (!type) {
//    game->addloadwarning("warning:building not found:'"+stype"'\n");
  }
  dir=xml.ivar("dir");
  z=xml.fvar("z");
  createblocks();
}
void Building::xmlsave(XMLNode xml) {
  Static::xmlsave(xml);xml.setvar("name",getname());
  xml.setname("Building");
  xml.setvar("dir",dir);
  xml.setvar("z",z);
  if (type) {
    xml.setvar("type",type->name);
  }
}
void Building::render(RenderContext &rc) {
  if (!visible) return;
  if (type) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x,z,y);
    type->render(rc,dir);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
}

void Statics::notifyremove(GInstance *obj) {
  GInstance::notifyremove(obj);
//  if (Static *st=dynamic_cast<Static*>(obj))
  items.quickremove((Static*)obj);
}

void Statics::buildrstation(int x1,int y1,int x2,int y2,int dir,bool isdepo) {
  int x=min(x1,x2),w=abs(x1-x2)+1;
  int y=min(y1,y2),h=abs(y1-y2)+1;
  int trc=h,trl=w;
  if (dir%2) exchange(trc,trl);

//  float hi0=0;
  bool can=true;
  float hi=game->himap->getfreehi(x,y,w,h);
  if (hi<0) can=false;
  if (can) game->himap->sethi(x,y,w+1,h+1,hi);

  if (!game->surmap->freeplane(x,y,w,h)) can=false;

  if (!can) game->seterror("$Cannot-build-rstation");
  else items.add(new RStation(x,y,dir,trc,trl,w,h,hi,isdepo));
/*
  if (game->himap->checkplane(x,y,w,h,&hi0) && game->surmap->freeplane(x,y,w,h)) {
    items.add(new RStation(x,y,vertical,trc,trl,w,h,hi0));
  } else game->seterror("$cannot-build-rstation");
*/
}

void Statics::buildsemaphore(int x,int y,string type) {
  if (RItem *ri=game->rrmap->at(x,y,RItem::surface)) {
    jtSemaphore *sem=gimages->findsemaphore(type);
    if (! ri->spojnice().valid()) {
      game->seterror("$Cannot-build-semaphore");
      return;
    }
    SurRItem *sri=(SurRItem*)ri;
    if (sri->semaphore) {
      sri->semaphore->buildnext();
      sri->semaphore->settype(sem);
    } else sri->semaphore=new Semaphore(ri,ri->spojnice(),0,sem);
    sri->updatesems();
  }
}

static bool maketubushi(int x,int y,int len,bool ver,float dhi,float *resulthi,float *resultdhi,string dhimode) {
  HItem &a=game->himap->vertexat(x,y),&b=game->himap->vertexat(x+(ver?1:0),y+(ver?0:1)),
        &c=game->himap->vertexat(x+(ver?0:len),y+(ver?len:0)),&d=game->himap->vertexat(x+(ver?1:len),y+(ver?len:1));
  //horizontalne:
  //a            c
  //b            d
  Interval ia,ib,ic,id;
  game->himap->makeinterval(a.x,a.y,ia);
  game->himap->makeinterval(b.x,b.y,ib);
  game->himap->makeinterval(c.x,c.y,ic);
  game->himap->makeinterval(d.x,d.y,id);
  Interval r1=ia*ib,r2=ic*id;
  if (dhimode=="direct") {//musime presne o dhi
    r2.lo-=dhi,r2.hi-=dhi;
    float in_hi=((a.hi+b.hi)/2+(c.hi+d.hi)/2-dhi)/2;//prvni navrh
    Interval r=r1*r2;
    if (in_hi<r.lo) in_hi=r.lo;
    if (in_hi>r.hi) in_hi=r.hi;
    if (!r.isin(in_hi)) return false;
    *resulthi=in_hi;
    *resultdhi=dhi;
    return true;
  } else if (dhimode=="change") {
    float in_hi=(a.hi+b.hi)/2,out_hi=(c.hi+d.hi)/2;//prvni navrh
    if (in_hi<r1.lo) in_hi=r1.lo;if (in_hi>r1.hi) in_hi=r1.hi;
    if (out_hi<r2.lo) out_hi=r2.lo;if (out_hi>r2.hi) out_hi=r2.hi;
    if (fabs(out_hi-in_hi)>fabs(dhi)) {
      float new_out_hi=out_hi,new_in_hi=in_hi;
      if (out_hi>in_hi) new_out_hi=in_hi+fabs(dhi);
      else new_out_hi=in_hi-fabs(dhi);
      if (in_hi>out_hi) new_in_hi=out_hi+fabs(dhi);
      else new_in_hi=out_hi-fabs(dhi);
      if (r1.isin(new_in_hi)) in_hi=new_in_hi;
      else out_hi=new_out_hi;
    }
    if (out_hi<r2.lo || out_hi>r2.hi) throw SqBlocked(c.x,c.y);
    if (in_hi<r1.lo || in_hi>r1.hi) throw SqBlocked(a.x,a.y);
    *resulthi=in_hi;
    *resultdhi=out_hi-in_hi;
    return true;
  } else {
    Interval res=ia*ib*ic*id;
    if (res.lo>res.hi) return false;
    float h=(a.hi+b.hi)/2;
    if (h<res.lo) h=res.lo;
    if (h>res.hi) h=res.hi;
    *resulthi=h;
    *resultdhi=0;
    return true;
  }
}

bool Statics::buildbridge(int x,int y,int len,bool ver,float hi,float dhisum,string type) {
  HItem &a=game->himap->vertexat(x,y),&b=game->himap->vertexat(x+(ver?1:0),y+(ver?0:1)),
        &c=game->himap->vertexat(x+(ver?0:len),y+(ver?len:0)),&d=game->himap->vertexat(x+(ver?1:len),y+(ver?len:1));
  a.sethi(hi);b.sethi(hi);c.sethi(hi+dhisum);d.sethi(hi+dhisum);
  TrackBridge *res=NULL;
  if (type=="rail") res=new RBridge(ver,x,y,len,hi,dhisum);
  if (type=="road") res=new RoadBridge(ver,x,y,len,hi,dhisum);
  if (res) items.add(res);
  return res;
}
bool Statics::buildbridge(int x,int y,int len,bool ver,float dhielem,string dhimode,string type) {
//  int x1=x,y1=y,w=1,h=1;
//  if (ver) h=len;else w=len;
  float h=0,dh=0;
  if (!maketubushi(x,y,len,ver,dhielem*len,&h,&dh,dhimode)) return false;
  return buildbridge(x,y,len,ver,h,dh,type);
}

bool Statics::buildtunnel(int x,int y,int len,bool ver,float hi,float dhisum,string type) {
  HItem &a=game->himap->vertexat(x,y),&b=game->himap->vertexat(x+(ver?1:0),y+(ver?0:1)),
        &c=game->himap->vertexat(x+(ver?0:len),y+(ver?len:0)),&d=game->himap->vertexat(x+(ver?1:len),y+(ver?len:1));
  a.sethi(hi);b.sethi(hi);c.sethi(hi+dhisum);d.sethi(hi+dhisum);
  int dx=ver?0:1,dy=ver?1:0;
  int ndx=1-dx,ndy=1-dy;
  int ix=x+dx,iy=y+dy;
  float edh=dhisum/len;


  for(int i=1;i<len;i++,ix+=dx,iy+=dy) {
    if (i==1 || i==len-1) {
      game->himap->sethi(ix,iy,hi+1+i*edh);
      game->himap->sethi(ix+ndx,iy+ndy,hi+1+i*edh);
    } else {
      if (game->himap->hiat(ix,iy)<hi+1+i*edh) game->himap->sethi(ix,iy,hi+1+i*edh);
      if (game->himap->hiat(ix+ndx,iy+ndy)<hi+1+i*edh) game->himap->sethi(ix+ndx,iy+ndy,hi+1+i*edh);
    }
  }
  TrackTunnel *res=NULL;
  if (type=="rail") res=new RTunnel(ver,x,y,len,hi,dhisum);
  if (type=="road") res=new RoadTunnel(ver,x,y,len,hi,dhisum);
  if (res) items.add(res);
  return res;
}

bool Statics::buildtunnel(int x,int y,int len,bool ver,float dhielem,string dhimode,string type) {
  float h=0,dh=0;
  if (!maketubushi(x,y,len,ver,dhielem*len,&h,&dh,dhimode)) return false;
  return buildtunnel(x,y,len,ver,h,dh,type);
}

void Statics::buildbuilding(int x,int y,int dir,float hi,jtBuilding *bld) {
  int w=bld->w,h=bld->h;
  if (dir==1 || dir==3) exchange(w,h);
  game->surmap->freeplane(x,y,w,h,true);
  for(int i=0;i<=w;i++) for(int j=0;j<=h;j++) game->himap->sethi(x+i,y+j,hi);
  Building *bn=new Building(x,y,w,h,dir,hi,bld);
  items.add(bn);
}

void Statics::buildbuilding(int x,int y,int dir,jtBuilding *bld) {
  int w=bld->w,h=bld->h;
  if (dir==1 || dir==3) exchange(w,h);
  float hi=game->himap->getfreehi(x,y,w,h);
  if (hi<0) {
    throw HiBlocked(x,y);
  }
  buildbuilding(x,y,dir,hi,bld);
}

void Statics::buildrunway(int x,int y,bool ver,int len) {
  int w=ver?1:len,h=ver?len:1;
  float hi=game->himap->getfreehi(x,y,w,h);
  bool can=true;
  if (hi<0) can=false;
  if (can) {
    game->himap->sethi(x,y,w+1,h+1,hi);
    Airport *a=new Airport(x,y,w,h,ver,len,1,hi);
    items.add(a);
  } else {
    game->seterror("$Cannot-build-runway");
  }
}

void Statics::buildbusstop(int x,int y) {
  game->roadmap->wantok();
  RoadItem *ri=game->roadmap->at(x,y,RoadItem::surface);
  if (!ri || fabs(ri->dzx)+fabs(ri->dzy)>epsilon || !ri->isstreight()) {
    game->seterror("$Cannot-build-bus-stop-here");
    return;
  }
  game->roadmap->modify();
  BusStop *res=new BusStop(x,y,ri,ri->ns!=NULL,ri->za);
  items.add(res);
}

void Static::destroy() {
  delize();
  GObject::destroy();
}

void Building::clearvisibility() {
  visible=false;
}
void Building::dodelize() {
  gocttree->removegroup(this);
}
void Building::dooctalize() {
  if (!type) return;
  gloBoundBox bnd=type->bound;
  bnd=bldrotatebound(bnd,0,w,h);
  float a[3]={x+bnd.x1,z+bnd.y1,y+bnd.z1};
  float b[3]={x+bnd.x2,z+bnd.y2,y+bnd.z2};
  gocttree->add(otItem(this,0),a,b);
}
void Building::ot_render(int id) {
  visible=true;
}

void Building::destroy() {
  game->surmap->unblock(x,y,w,h);
  game->himap->unblock(x,y,w+1,h+1);
  Static::destroy();
}

void Building::createblocks() {
  game->surmap->block(x,y,w,h);
  game->himap->block(x,y,w+1,h+1);
}

Building::Building(int x,int y,int w,int h,int adir,float hi,jtBuilding *atype):Static(x,y,w,h) {
  dir=adir;type=atype;z=hi;
  createblocks();
}


/*
void Statics::buldozere(int x,int y) {
  for(int i=0;i<items.count;i++) {
    Static &it=*items[i];
    if (it.occupy(x,y)) it.buldozere(x-it.x,y-it.y);
  }
}


void Static::destroy() {
  exist=false;
  game->statics->items.remove(this);
}

void Static::staticchanged() {
  GObject::changed();
}
*/


}
//#pragma package(smart_init)



