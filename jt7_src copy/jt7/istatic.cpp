#include <jlib.h>
#pragma hdrstop

#include "istatic.h"

namespace jtyc {

float BSF=0.1;//buildingsizefactor

static bool accetable(jtCompletable &what,int made,int rest) {
  rest-=what.cnt;
  if (rest<0) return false;
  if (what.min>=0 && (made<what.min || rest<what.min)) return false;
  if (what.max>=0 && (made>what.max && rest>what.max)) return false;
  if (what.bmin>=0 && made<what.bmin) return false;
  if (what.emin>=0 && rest<what.emin) return false;
  if (what.bmax>=0 && made>what.bmax) return false;
  if (what.emax>=0 && rest>what.emax) return false;
  return true;
}

static bool try_complete(Array<jtCompletable> &avail,TCntVec<int> &res,int made,int rest) {
  if (rest==0) return true;
  for(int i=0;i<avail.count;i++) {
    if (accetable(avail[i],made,rest)) {
      res.add(avail[i].id);
      if (try_complete(avail,res,made+avail[i].cnt,rest-avail[i].cnt)) return true;
      res.eraselast();
    }
  }
  return false;
}

void jt_complete(Array<jtCompletable> &avail,TCntVec<int> &res,int cnt) {
  try_complete(avail,res,0,cnt);
}

void StationInstance::render(float x,float y,float z,RenderContext &rc,int dir) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(x,z,y);
  jt_transformate(dir,trl,trc);

  for(int tri=0,tr=0;tri<perons.count;tr+=perons[tri]->peron->trc,tri++) {
    PeronInstance *pi=perons[tri];
    for(int ii=0,ir=0;ii<pi->items.count;ir+=pi->items[ii]->item->trl,ii++) {
      if (!ignore_visibility) if (!pi->items[ii]->visible) continue;
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glTranslatef(ir,0,tr);
      pi->items[ii]->item->render(rc,0);
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
    }
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void jtBusStop::octalize(void *obj,float x,float y,float z,OctTree *ot) {
  float a[3]={x,z+bound.y1,y};
  float b[3]={x+1,z+bound.y2,y+1};
  ot->add(otItem(obj,0),a,b);
}

void StationInstance::octalize(void *obj,int x,int y,float z,int dir,OctTree *ot) {
  for(int tri=0,tr=0;tri<perons.count;tr+=perons[tri]->peron->trc,tri++) {
    PeronInstance *pi=perons[tri];
    for(int ii=0,ir=0;ii<pi->items.count;ir+=pi->items[ii]->item->trl,ii++) {
      StationItemInstance *sii=pi->items[ii];
      gloBoundBox bnd=sii->item->bound;
      bnd=bldrotatebound(bnd,dir,sii->item->w,sii->item->h);
      switch (dir) {
        case 0:bnd.translate(ir,0,tr);break;
        case 1:bnd.translate(tr,0,trl-ir-1);break;
        case 2:bnd.translate(trl-ir-1,0,trc-tr-1);break;
        case 3:bnd.translate(trc-tr-1,0,ir);break;
      }
      float a[3]={x+bnd.x1,z+bnd.y1,y+bnd.z1};
      float b[3]={x+bnd.x2,z+bnd.y2,y+bnd.z2};
      ot->add(otItem(obj,tri*256+ii),a,b);
    }
  }
}
void StationInstance::ot_render(int id) {
  perons[id/256]->items[id%256]->visible=true;
}


void BridgeInstance::render(float x,float y,float z,RenderContext &rc,bool vertical) {
  for(int i=0,act=0;i<items.count;act+=items[i]->item->cnt,i++) {
    if (!ignore_visibility) if (!items[i]->visible) continue;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    int dx=0,dy=0;
    if (vertical) dy=act;else dx=act;
    glTranslatef(x+dx,y,z+dy);
    items[i]->item->render(rc,vertical);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
}

void jtBusStop::load(PersistentObject *data,XMLNode xml) {
  model=jt_loadmodel(data,"model.zip");
  model->scale(BSF,BSF,BSF);
  bound=model->bound();
  isdefault=xml.ivar("default");
  name=xml.svar("name");
  path=xml.svar("path");
  title=lang_text_ch(xml.svar("title"));
}

void jtCommonBuilding::loadmodel(PersistentObject *data,string file) {
  model=jt_loadmodel(data,file);
  model->scale(BSF,BSF,BSF);
  bound=model->bound();
}

void jtStationItem::load(PersistentObject *data,XMLNode xml) {
  trc=xml.ivar("tracks");trl=xml.ivar("cnt");
  h=trc;w=trl;
  name=xml.svar("name");
  loaditem(w,h,data,xml);
}

void jtBuilding::load(PersistentObject *data,XMLNode xml) {
  name=xml.svar("name");
  title=lang_text_ch(xml.svar("title"));
  w=xml.ivar("w");h=xml.ivar("h");
  loaditem(w,h,data,xml);
}

void jtStation::load(PersistentObject *data,XMLNode xml) {
  name=xml.svar("name");
  isdepo=xml.ivar("depo");
  title=lang_text_ch(xml.svar("title"));
  isdefault=xml.ivar("default");
  XMLNode its=xml["items"];
  for(int i=0;i<its.count();i++) {
    jtStationItem *item=new jtStationItem();
    items.add(item);
    item->load(data,its[i]);
    items_c.add(jtCompletable(its[i]));
  }
  XMLNode pers=xml["perons"];
  for(int i=0;i<pers.count();i++) {
    jtStationPeron *item=new jtStationPeron(this);
    perons.add(item);
    perons_c.add(jtCompletable(pers[i]));
    perons_c[i].id=i;
    item->load(data,pers[i]);
  }
}

StationInstance *jtStation::complete(int trc,int trl) {
  StationInstance *res=new StationInstance();
  res->trc=trc;res->trl=trl;
  TCntVec<int> pers;
  jt_complete(perons_c,pers,trc);
  for(int i=0;i<pers.count;i++) {
    PeronInstance *pi=new PeronInstance();
    res->perons.add(pi);
    jtStationPeron *per=perons[pers[i]];
    pi->peron=per;

    TCntVec<int> its;
    jt_complete(per->items_c,its,trl);

    for(int j=0;j<its.count;j++) pi->items.add(new StationItemInstance(per->items[its[j]]));
  }
  return res;
}

void jtStationPeron::load(PersistentObject *data,XMLNode xml) {
  string itname=xml.svar("item");
  trc=xml.ivar("cnt");
  for(int i=0;i<parent->items.count;i++) {
    if (parent->items[i]->name==itname && parent->items[i]->trc==trc) {
      items.add(parent->items[i]);
      items_c.add(parent->items_c[i]);
      items_c.last().id=items.count-1;
    }
  }
}

void jtCompletableItemBuilding::loaditem(int w,int h,PersistentObject *data,XMLNode xml) {
  loadmodel(data,xml.svar("model"));
  XMLNode models=xml["models"];
  if (model) bound=model->bound();
  for(int i=0;i<models.count();i++) {
    int wsm=w,hsm=h;
    if (models[i].svar("w")!="") wsm=models[i].ivar("w");
    if (models[i].svar("h")!="") hsm=models[i].ivar("h");
    CompletableItemBuildingItem *added=new CompletableItemBuildingItem(wsm,hsm);
    added->load(data,models[i]);
    this->models.add(added);
    bound.extend(added->bound);
  }
}

void CompletableItemBuildingItem::load(PersistentObject *data,XMLNode xml) {
  model=jt_loadmodel(data,xml.svar("model"));
  model->scale(BSF,BSF,BSF);
  dx=xml.fvar("dx");
  dy=xml.fvar("dy");
  dz=xml.fvar("dz");
  dir=xml.fvar("dir");
  bound=model->bound();
  bound=bldrotatebound(bound,dir,w,h);
  bound.translate(dx,dy,dz);
}

gloBoundBox bldrotatebound(const gloBoundBox &box,int dir,int w,int h) {
  gloBoundBox res=box;
  res.x1=res.z1=0;
  if (dir%2) res.x2=h,res.z2=w;else res.x2=w,res.z2=h;
  return res;
}

void CompletableItemBuildingItem::render(RenderContext &rc,int dir,int w,int h) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  jt_transformate(dir,w,h);
  glTranslatef(dx,dy,dz);
  jt_transformate(this->dir,this->w,this->h);
  model->render(rc);
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void jtCompletableItemBuilding::render(RenderContext &rc,int dir) {
  jtCommonBuilding::render(rc,dir);
  for(int i=0;i<models.count;i++) models[i]->render(rc,dir,w,h);
}

void jtBridgeItem::load(PersistentObject *data,XMLNode xml) {
  cnt=xml.ivar("cnt");
  w=cnt;
  loaditem(cnt,1,data,xml);
}

jtCompletable::jtCompletable(XMLNode xml) {
  cnt=0;min=max=-1;id=-1;
  bmin=bmax=emin=emax=-1;
  if (xml.svar("cnt")!="") cnt=xml.ivar("cnt");
  if (xml.svar("min")!="") min=xml.ivar("min");
  if (xml.svar("max")!="") max=xml.ivar("max");
  if (xml.svar("bmin")!="") bmin=xml.ivar("bmin");
  if (xml.svar("bmax")!="") bmax=xml.ivar("bmax");
  if (xml.svar("emin")!="") emin=xml.ivar("emin");
  if (xml.svar("emax")!="") emax=xml.ivar("emax");
}

void jtBridge::load(PersistentObject *data,XMLNode xml) {
  name=xml.svar("name");
  title=lang_text_ch(xml.svar("title"));
  isdefault=xml.ivar("default");
  XMLNode its=xml["items"];
  for(int i=0;i<its.count();i++) {
    jtBridgeItem *item=new jtBridgeItem();
    items.add(item);
    item->load(data,its[i]);
    items_c.add(jtCompletable(its[i]));
    items_c[i].id=i;
  }
}

BridgeInstance *jtBridge::complete(int len) {
  BridgeInstance *res=new BridgeInstance();
  TCntVec<int> ids;
  jt_complete(items_c,ids,len);
  for(int i=0;i<ids.count;i++) res->items.add(new BridgeItemInstance(items[ids[i]]));
  return res;
}

void jtTunnel::load(PersistentObject *data,XMLNode xml) {
  name=xml.svar("name");
  title=lang_text_ch(xml.svar("title"));
  horspace=xml.fvar("hor");
  verspace=xml.fvar("ver");
  loadmodel(data,"model.zip");
  isdefault=xml.ivar("default");
  if (xml.svar("top.texture")!="") top=texman->tryfindtexture(xml.svar("top.texture"),data);
  if (xml.svar("bot.texture")!="") bot=texman->tryfindtexture(xml.svar("bot.texture"),data);
  topscale=xml.fvar("top.texscale");
  botscale=xml.fvar("bot.texscale");
  if (topscale<0.001) topscale=1;
  if (botscale<0.001) botscale=1;
}

void jt_transformate(int dir,int w,int h) {
  switch (dir) {
    case 0:glTranslatef(0,0,0);glRotatef(0,0,1,0);break;
    case 1:glTranslatef(0,0,w);glRotatef(90,0,1,0);break;
    case 2:glTranslatef(w,0,h);glRotatef(180,0,1,0);break;
    case 3:glTranslatef(h,0,0);glRotatef(270,0,1,0);break;
  }
}


void jt_renderbuilding(RenderContext &rc,float dx,float dy,float dz,int w,int h,int dir,glo::gloModel *model) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(dx,dy,dz);
  jt_transformate(dir,w,h);
  model->render(rc);
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void jtCommonBuilding::render(RenderContext &rc,int dir) {
  jt_renderbuilding(rc,0,0,0,w,h,dir,model);
}

void jtBusStop::render(RenderContext &rc,bool vertical) {
  glMatrixMode(GL_MODELVIEW);
  if (vertical) {
    glPushMatrix();
    glTranslatef(0,0,1);
    glRotatef(90,0,1,0);
    model->render(rc);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1,0,0);
    glRotatef(270,0,1,0);
    model->render(rc);
    glPopMatrix();
  } else {
    glPushMatrix();
    glTranslatef(0,0,0);
    model->render(rc);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1,0,1);
    glRotatef(180,0,1,0);
    model->render(rc);
    glPopMatrix();
  }
}




}

