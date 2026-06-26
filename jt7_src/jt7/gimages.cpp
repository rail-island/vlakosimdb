#include <jlib.h>
#pragma hdrstop

#include "gimages.h"
#include "gameu.h"
#include "gview.h"
#include "grrmap.h"
#include "grrcom.h"
#include "gmtcom.h"
#include "imotocar.h"
#include "imachine.h"
#include "istatic.h"
#include "icab.h"
#include "ismoke.h"

namespace jtyc {

Images *gimages=NULL;

void load_images() {
  if (gimages) delete gimages;
  gimages=new Images();
  PersistentObject *dir=new PODirectory(basedir/"objects");

  gimages->load(dir);
  gimages->als.load();
}

#define find_alias(type,array,obj0) {\
  type *res=array.psearchv(name);\
  if (res) return res;\
  for(int i=0;i<als.array.count;i++) {\
    if (als.array[i]->src==name) res=array.psearchv(als.array[i]->dest);\
    if (res) {\
      game->addloadmessage("warning:used alias:"+name+"->"+als.array[i]->dest);\
      return res;\
    }\
  }\
  game->addloadmessage("warning:object not found:"+name);\
  return obj0;\
}

jtMachine *Images::findmachine(string name) find_alias(jtMachine,machines,machine0)
jtMotocar *Images::findmotocar(string name) find_alias(jtMotocar,motocars,motocar0)
jtBuilding *Images::findbuilding(string name) find_alias(jtBuilding,blds,NULL)

#define load_func(objname,jtClass)\
jtClass *load##objname(XMLNode xml,PersistentObject *obj) {\
  if (xml.getname()==#objname) {\
    jtClass *res=new jtClass();\
    res->load(obj,xml);\
    return res;\
  }\
  return NULL;\
}
load_func(loco,jtMachine)
load_func(busstop,jtBusStop)
load_func(motocar,jtMotocar)
load_func(bridge,jtBridge)
load_func(tunnel,jtTunnel)
load_func(semaphore,jtSemaphore)
//load_func(depo,jtStation)
load_func(rstation,jtStation)
load_func(building,jtBuilding)
load_func(railroad,jtRail)
load_func(road,jtRoad)
load_func(cab,jtCab)
load_func(aircraft,jtAircraft)
#undef load_func


void Images::load(PersistentObject *obj) {
  Strings childs;
  obj->getchilds(childs);
  for(int i=0;i<childs.count;i++) {
    try {
      PersistentObject *child=obj->openchild(childs[i]);
      MainXMLNode xml;
      child->loadxml("index.xml",xml);
#define load_func(objname,jtClass,array)\
if (jtClass *object=load##objname(xml,child)) array.add(object);
load_func(loco,jtMachine,machines)
load_func(busstop,jtBusStop,busstops)
load_func(motocar,jtMotocar,motocars)
load_func(bridge,jtBridge,bridges)
load_func(tunnel,jtTunnel,tunnels)
load_func(semaphore,jtSemaphore,sems)
//load_func(depo,jtStation,depos)
load_func(rstation,jtStation,stations)
load_func(building,jtBuilding,blds)
load_func(railroad,jtRail,rails)
load_func(road,jtRoad,roads)
load_func(cab,jtCab,cabs)
load_func(aircraft,jtAircraft,aircrafts)
#undef load_func
      if (xml.getname()=="trees") {
        for(int i=0;i<xml.count();i++) {
          jtSimpleTree *tree=new jtSimpleTree();
          trees.add(tree);
          tree->load(child,xml[i]);
        }
      }
      if (xml.getname()=="terrains") {
        for(int i=0;i<xml.count();i++) {
          jtTerrain *t=new jtTerrain();
          terrains.add(t);
          t->load(child,xml[i]);
        }
      }
    } catch (JException &e) {
      throw JError("Error by loading file:"+childs[i]+"\n"+e.getmsg());
    }
  }

#define find_def(mp_objs,mp_result,mp_name) \
  if (mp_objs.count==0) throw JError("No "+string(mp_name)+" found");\
  for(int i=0;i<mp_objs.count;i++) {\
    if (mp_objs[i]->isdefault) {\
      mp_result=mp_objs[i];\
      break;\
    }\
  }\
  if (!mp_result) mp_result=mp_objs[0];

  for(int i=0;i<stations.count;i++) if (stations[i]->isdepo) {
    depos.add(stations[i]);
    stations[i]=NULL;
  }
  stations.pack(NULL);// null();

  find_def(machines,machine0,"mechine")
  find_def(busstops,busstop0,"busstop")
  find_def(motocars,motocar0,"motocar")
  find_def(bridges,bridge0,"bridge")
  find_def(tunnels,tunnel0,"tunnel")
  find_def(sems,sem0,"semaphore")
  find_def(depos,depo0,"depo")
  find_def(stations,station0,"station")
  find_def(trees,tree0,"tree")
  find_def(terrains,terrain0,"terrain")
  find_def(rails,rail0,"rail")
  find_def(roads,road0,"road")
  find_def(cabs,cab0,"cab")
  find_def(aircrafts,aircraft0,"aircraft")

  water=findterrain("water");

#undef find_def

  for(int i=0;i<machines.count;i++) if (!machines[i]->cab) machines[i]->cab=cab0;
}


void jtSemaphore::load(PersistentObject *data,XMLNode xml) {
  model=jt_loadmodel(data,"model.zip");
  gradtime=xml.fvar("gradtime");
  isdefault=xml.ivar("default");
  name=xml.svar("name");
  title=lang_text_ch(xml.svar("title"));
}

void jtSimpleTree::load(PersistentObject *data,XMLNode xml) {
  name=xml.svar("name");
  title=lang_text_ch(xml.svar("title"));
  if (xml.svar("file")!="") texture=texman->findtexture("pack:"+xml.svar("file"),data);
  isdefault=xml.ivar("default");
  w=xml.fvar("w");
  h=xml.fvar("h");
  if (w==0) w=h*texture->ratiowh();
  if (h==0) h=w*texture->ratiohw();
}

void jtSimpleTree::render(RenderContext &rc) {
  rc.push();
  texture->bind(rc.ctxid);
  rc.settexture2d(true);
  rc.setalphatest(true);
//  glEnable(GL_TEXTURE_2D);
//  glEnable(GL_ALPHA_TEST);
//  glAlphaFunc(GL_GEQUAL,0.5);
  glColor3f(1,1,1);
  glBegin(GL_QUADS);

  glTexCoord2f(0,0);glVertex3f(-w/2,0,0);
  glTexCoord2f(1,0);glVertex3f(w/2,0,0);
  glTexCoord2f(1,1);glVertex3f(w/2,h,0);
  glTexCoord2f(0,1);glVertex3f(-w/2,h,0);

  glTexCoord2f(0,0);glVertex3f(0,0,-w/2);
  glTexCoord2f(1,0);glVertex3f(0,0,w/2);
  glTexCoord2f(1,1);glVertex3f(0,h,w/2);
  glTexCoord2f(0,1);glVertex3f(0,h,-w/2);

  glEnd();

//  glDisable(GL_TEXTURE_2D);
//  glDisable(GL_ALPHA_TEST);
  rc.pop();
}

void jtTerrain::load(PersistentObject *data,XMLNode xml) {
  if (xml.svar("file")!="") texture=texman->findtexture("pack:"+xml.svar("file"),data);
  name=xml.svar("name");
  title=lang_text_ch(xml.svar("title"));
  isdefault=xml.ivar("default");
}

void jtWay::load(PersistentObject *data,XMLNode xml) {
  name=xml.svar("name");
  title=lang_text_ch(xml.svar("title"));
  isdefault=xml.ivar("default");
  XMLNode sur=xml["surface"];
  for(int i=0;i<sur.count();i++) {
    if (TrackTT *tt=loadtrack(data,sur[i])) items.add(tt);
  }
}

bool jtRoad::allow(jtMotocar *motocar) {
  if (motocar->roadstyle=="") return true;
  return allowstyles.search(motocar->roadstyle)>=0;
}

void jtRail::load(PersistentObject *data,XMLNode xml) {
  wire_top=xml.fvar("wire_top");
  wire_bot=xml.fvar("wire_bot");
  train_y=xml.fvar("train_y");
  jtWay::load(data,xml);
}

void jtRoad::load(PersistentObject *data,XMLNode xml) {
  motodist=xml.fvar("motodist");
  moto_y=xml.fvar("moto_y");
  allowstyles.fromtext(xml.svar("allowstyles"),';');
  jtWay::load(data,xml);
}

GravelRoadTT::GravelRoadTT(PersistentObject *data,XMLNode xml):StdTrackTT(data,xml) {
  if (tex.texture) tex.texture->setrepeat_s(true),tex.texture->setrepeat_t(true);
  d1=xml.fvar("d1");
  d2=xml.fvar("d2");
  d3=xml.fvar("d3");
}
void GravelRoadTT::dorender(TrackItem *ari,int att,RenderContext &rc) {
  RoadItem *ri=(RoadItem*)ari;
  draw_gravel(ri,hi,rc,d1,d2,d3);
}

MidlineRoadTT::MidlineRoadTT(PersistentObject *data,XMLNode xml):StdTrackTT(data,xml) {
  w=xml.fvar("w");
}
void MidlineRoadTT::dorender(TrackItem *ari,int att,RenderContext &rc) {
  RoadItem *ri=(RoadItem*)ari;
  draw_midline(ri,hi,rc,w);
}

void TramRailRoadTT::dorender(TrackItem *ari,int att,RenderContext &rc) {
  RoadItem *ri=(RoadItem*)ari;
  draw_tramrail(ri,hi,rc,gauge2,w,dist);
}
TramRailRoadTT::TramRailRoadTT(jtRoad *road,PersistentObject *data,XMLNode xml):StdTrackTT(data,xml) {
  if (tex.texture) tex.texture->setrepeat_t(true);
  gauge2=xml.fvar("gauge2");
  w=xml.fvar("w");
  dist=road->motodist;
}

void WireRoadTT::dorender(TrackItem *ari,int att,RenderContext &rc) {
  RoadItem *ri=(RoadItem*)ari;
  draw_roadwire(ri,hi,rc,gauge2,w,dist);
}
WireRoadTT::WireRoadTT(jtRoad *road,PersistentObject *data,XMLNode xml):StdTrackTT(data,xml) {
  if (tex.texture) tex.texture->setrepeat_t(true);
  gauge2=xml.fvar("gauge2");
  w=xml.fvar("w");
  dist=road->motodist;
}


TrackTT *jtRoad::loadtrack(PersistentObject *data,XMLNode xml) {
  if (xml.hasname("gravel")) return new GravelRoadTT(data,xml);
  if (xml.hasname("midline")) return new MidlineRoadTT(data,xml);
  if (xml.hasname("tramrail")) {
    hastramrail=true;
    return new TramRailRoadTT(this,data,xml);
  }
  if (xml.hasname("wire")) {
    haswire=true;
    return new WireRoadTT(this,data,xml);
  }
  return NULL;
}

TrackTT *jtRail::loadtrack(PersistentObject *data,XMLNode xml) {
  if (xml.hasname("rail")) return new RailRailTT(data,xml);
  if (xml.hasname("sleeper")) return new SleeperRailTT(data,xml);
  if (xml.hasname("gravel")) return new GravelRailTT(data,xml);
  if (xml.hasname("wire")) return new WireRailTT(data,xml);
  if (xml.hasname("vwire")) return new VWireRailTT(data,xml);
  if (xml.hasname("cols")) return new ColsRailTT(data,xml,this);
  return NULL;
}

ModelRailTT::ModelRailTT(PersistentObject *data,XMLNode xml) {
  if (xml.svar("model")!="") model=jt_loadmodel(data,xml.svar("model"));
  gauge2=xml.fvar("gauge2");
}

void ModelRailTT::render(TrackItem *ari,int att,RenderContext &rc) {
  RItem *ri=(RItem*)ari;
  Spojnice tt(att);
  fpoint pos=tt.pos(tt.len()/2);
  fvector diff=tt.diff(tt.len()/2);
  fvector dpos=diff.getnormal().multiply(gauge2,gauge2);
  fpoint pt=pos+dpos;
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  lastpos.x=ri->x+pos.x;lastpos.z=ri->y+pos.y;lastpos.y=ri->za+pos.x*ri->dzx+pos.y*ri->dzy;
  glTranslatef(pt.x+ri->x,ri->za+pt.x*ri->dzx+pt.y*ri->dzy,pt.y+ri->y);
  int rot=0;
  if (tt.data==tt_24) rot=!rot;
  if (tt.data==tt_34) rot=!rot;
  if (tt.data==tt_bd) rot=!rot;
  if (tt.data==tt_14) rot=!rot;
  if (tt.data==tt_bc) rot=!rot;
  if (tt.data==tt_2d) rot=!rot;
  if (tt.data==tt_2a) rot=!rot;
  if (tt.data==tt_3a) rot=!rot;
  if (tt.data==tt_1d) rot=!rot;
  glRotatef(90-diff.fangle()/M_PI*180+rot*180,0,1,0);
  model->render(rc);
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

ColsRailTT::ColsRailTT(PersistentObject *data,XMLNode xml,jtRail *parent):ModelRailTT(data,xml),parent(parent) {
  frequency=xml.ivar("frequency");
  wire_y=xml.fvar("wire_y");
  wire_linewi=xml.fvar("wire_linewi");
  wiretex.load(data,xml,"wire_");
  if (model) model->scale(0.1,0.1,0.1);
}
void ColsRailTT::render(TrackItem *ari,int att,RenderContext &rc) {
  RItem *ri=(RItem*)ari;
  Spojnice tt(att);
  if (ri->canhavecol()) {
    rc.push();
    rc.setlighting(true);
    ModelRailTT::render(ari,att,rc);
    rc.pop();
    if (wire_y>0 && parent->wire_bot>0) {
      rc.push();
      rc.setculling(false);
      wiretex.bind(rc);
      fpoint3d p1=lastpos,p2=lastpos;
      p1.y+=wire_y;p2.y+=parent->wire_top;
      glTexLine(rc,0,1,wire_linewi,p1,p2);
      rc.pop();
    }
  }
}

void WireRailTT::dorender(TrackItem *ari,int att,RenderContext &rc) {
  RItem *ri=(RItem*)ari;
  Spojnice tt(att);
  draw_wire(tt,ri,hi,rc,quality,gauge2,linewi,tex.texture);
}

WireRailTT::WireRailTT(PersistentObject *data,XMLNode xml):StdTrackTT(data,xml) {
  if (tex.texture) tex.texture->setrepeat_s(true);
  gauge2=xml.fvar("gauge2");
  quality=xml.fvar("quality");
  if (xml.svar("linewi")!="") linewi=xml.fvar("linewi");else linewi=1;
}

VWireRailTT::VWireRailTT(PersistentObject *data,XMLNode xml):StdTrackTT(data,xml) {
  if (tex.texture) tex.texture->setrepeat_s(true);
  y1=xml.fvar("y1");
  y2=xml.fvar("y2");
  if (xml.svar("linewi")!="") linewi=xml.fvar("linewi");else linewi=1;
}

void VWireRailTT::dorender(TrackItem *ari,int att,RenderContext &rc) {
  RItem *ri=(RItem*)ari;
  Spojnice tt(att);
  fpoint pos=tt.pos(0);
  fpoint3d pt;
  pt.x=ri->x+pos.x;pt.z=ri->y+pos.y;pt.y=ri->za+pos.x*ri->dzx+pos.y*ri->dzy;
  fpoint3d a=pt,b=pt;
  a.y+=y1,b.y+=y2;
  glTexLine(rc,0,1,linewi,a,b);
}

void TT_Texture::load(PersistentObject *data,XMLNode xml,string prefix) {
  if (xml.svar(prefix+"file")!="") texture=texman->findtexture("pack:"+xml.svar(prefix+"file"),data);
  r=xml.fvar(prefix+"r");
  g=xml.fvar(prefix+"g");
  b=xml.fvar(prefix+"b");
  transparent=xml.ivar(prefix+"transparent");
  if (texture) texture->settransparent(transparent);
}

StdTrackTT::StdTrackTT(PersistentObject *data,XMLNode xml) {
  hi=xml.fvar("hi");
  tex.load(data,xml,"");
}

void TT_Texture::bind(RenderContext &rc) {
  if (texture) {
    rc.setalphatest(transparent);
    rc.settexture2d(true);
    glColor3f(1,1,1);
    texture->bind(rc.ctxid);
  } else {
    rc.settexture2d(false);
    glColor3f(r,g,b);
  }
}

void StdTrackTT::render(TrackItem *ri,int tt,RenderContext &rc) {
  rc.push();
  tex.bind(rc);
  rc.setculling(false);
  dorender(ri,tt,rc);
  rc.pop();
/*
  bool t2d=rc.gettexture2d();
  if (texture) {
    rc.setalphatest(transparent);
    rc.settexture2d(true);
    glColor3f(1,1,1);
    texture->bind(rc.ctxid);
  } else {
    rc.settexture2d(false);
    glColor3f(r,g,b);
  }
  rc.setculling(false);
  dorender(ri,tt,rc);
  rc.settexture2d(t2d);
*/
}

RailRailTT::RailRailTT(PersistentObject *data,XMLNode xml):StdTrackTT(data,xml) {
  if (tex.texture) tex.texture->setrepeat_s(true);
  gauge2=xml.fvar("gauge2");
  quality=xml.fvar("quality");
  w=xml.fvar("w");
}

void RailRailTT::dorender(TrackItem *ari,int att,RenderContext &rc) {
  RItem *ri=(RItem*)ari;
  Spojnice tt(att);
  draw_rail(tt,ri,hi,rc,quality,w,gauge2);
}

SleeperRailTT::SleeperRailTT(PersistentObject *data,XMLNode xml):StdTrackTT(data,xml) {
  cover=xml.fvar("cover");
  len=xml.fvar("len");
  wi=xml.fvar("wi");
}
void SleeperRailTT::dorender(TrackItem *ari,int att,RenderContext &rc) {
  RItem *ri=(RItem*)ari;
  Spojnice tt(att);
  draw_sleepers(tt,ri,hi,rc,len,wi,cover);
}

GravelRailTT::GravelRailTT(PersistentObject *data,XMLNode xml):StdTrackTT(data,xml) {
  if (tex.texture) tex.texture->setrepeat_s(true),tex.texture->setrepeat_t(true);
  quality=xml.fvar("quality");
  w=xml.fvar("w");
}
void GravelRailTT::dorender(TrackItem *ari,int att,RenderContext &rc) {
  RItem *ri=(RItem*)ari;
  Spojnice tt(att);
  draw_gravel(tt,ri,hi,rc,w,quality);
}

void jtWay::render(TrackItem *ri,int tt,RenderContext &rc) {
  for(int i=0;i<items.count;i++) items[i]->render(ri,tt,rc);
}

void jtTerrain::bind(RenderContext &rc) {
  if (texture) texture->bind(rc.ctxid);
}

Images::~Images() {
}

Images::Images() {
}


}


