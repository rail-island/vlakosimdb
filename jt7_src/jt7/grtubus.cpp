#include <jlib.h>
#pragma hdrstop

#include "grtubus.h"
#include "gview.h"
#include "gameu.h"
#include "grrmap.h"
#include "groad.h"
#include "ghimap.h"
#include "gsurmap.h"
#include "grrmap.h"
#include "ghiview.h"

namespace jtyc {

//int gettubusw(int len,bool ver) {

//  int x1=x,y1=y,w=1,h=1;
//  if (ver) h=len;else w=len;
//}

TrackTubus::TrackTubus(bool avertical,int x,int y,int alen,float ah0,float adhisum):Static(x,y,avertical?1:alen,avertical?alen:1) {
  h0=ah0;dhisum=adhisum;
  vertical=avertical;
  len=alen;
}

bool TrackTubus::railcanremove() {
  bool res=true;
  for(int i=0;i<items.count;i++) if (((RItem*)items[i])->trains>0) res=false;
  if (!res) game->seterror("$Cannot-destroy-tubus");
  return res;
}
bool TrackTubus::roadcanremove() {
  return true;
}

void TrackBridge::render(RenderContext &rc) {
  float dhe=dhisum/len;
  for(int i=0,act=0;i<instance->items.count;act+=instance->items[i]->item->cnt,i++) {
    if (!instance->items[i]->visible) continue;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    int dx=0,dy=0;
    if (vertical) dy=act;else dx=act;
    glTranslatef(x+dx,h0+dhe*act,y+dy);
    glMapCrook(vertical,dhe);
    instance->items[i]->item->render(rc,vertical);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
}

void TrackBridge::createbridgeblocks() {
  if (vertical) {
    for(int cy=1;cy<h-1;cy++) {
      BridgeBlock *bl=new BridgeBlock(x,y+cy,vertical,h0);
      sqblocks.add(bl);
    }
  } else {
    for(int cx=1;cx<w-1;cx++) {
      BridgeBlock *bl=new BridgeBlock(x+cx,y,vertical,h0);
      sqblocks.add(bl);
    }
  }
}

void TrackBridge::created() {
  createritems();
  createblocks();
  createbridgeblocks();
  createbridgeitems();
}

TrackBridge::TrackBridge(bool vertical,int x,int y,int len,float ah0,float adhisum):TrackTubus(vertical,x,y,len,ah0,adhisum) {
  bridge=gimages->bridge0;
}

void TrackBridge::dodelize() {
  gocttree->removegroup(this);
}

void TrackBridge::clearvisibility() {
  for(int i=0;i<instance->items.count;i++) instance->items[i]->visible=false;
}

void TrackBridge::ot_render(int id) {
  instance->items[id]->visible=true;
}

void TrackBridge::dooctalize() {
  float dhe=dhisum/len;
  for(int i=0,act=0;i<instance->items.count;act+=instance->items[i]->item->cnt,i++) {
    int cnt=instance->items[i]->item->cnt;
    int dx=0,dy=0;
    if (vertical) dy=act;else dx=act;
    gloBoundBox bnd=instance->items[i]->item->bound;
    bnd.y1+=act*dhe;
    bnd.y2+=dhe*cnt+act*dhe;
    bnd=bldrotatebound(bnd,vertical,vertical?1:cnt,vertical?cnt:1);
    float a[3]={bnd.x1+x+dx,bnd.y1+h0,bnd.z1+y+dy};
    float b[3]={bnd.x2+x+dx,bnd.y2+h0,bnd.z2+y+dy};
    gocttree->add(otItem(this,i),a,b);
  }

}

void TrackBridge::createbridgeitems() {
  instance=bridge->complete(items.count);
  for(int i=0,act=0;i<instance->items.count;act+=instance->items[i]->item->cnt,i++) {
    for(int j=act;j<act+instance->items[i]->item->cnt;j++) {
      items[j]->setbridgeitem(instance->items[i]);
    }
  }

//  bridgeitems.wantcapa(items.count);
//  bridgeitems.count=bridge->makebridge(items.count,bridgeitems.buf);
}

void TrackTubus::createritems() {
  int x1=x,y1=y,x2=x+w-1,y2=y+h-1;
  float dhi=dhisum/len;
  if (vertical) {
    for(int y=y1;y<=y2;y++) {
      TrackItem *ri=createritem(x1,y,vertical,y==y1||y==y2,h0+dhi*(y-y1));
      ri->dzy=dhi;
      items.add(ri);
    }
  } else {
    for(int x=x1;x<=x2;x++) {
      TrackItem *ri=createritem(x,y1,vertical,x==x1||x==x2,h0+dhi*(x-x1));
      ri->dzx=dhi;
      items.add(ri);
    }
  }
}

void TrackTubus::createblocks() {
  int x1=x,y1=y,x2=x+w-1,y2=y+h-1;
  float h1=h0+dhisum;
  if (vertical) {
    HiBlock *bl;
    bl=new FixedHiBlock(x1,y1,h0);hblockers.add(bl);
    bl=new FixedHiBlock(x1+1,y1,h0);hblockers.add(bl);
    bl=new FixedHiBlock(x1,y2+1,h1);hblockers.add(bl);
    bl=new FixedHiBlock(x1+1,y2+1,h1);hblockers.add(bl);
    for(int y=y1+1;y<=y2;y++) {
      bl=blocker(x1,y,y-y1);hblockers.add(bl);
      bl=blocker(x1+1,y,y-y1);hblockers.add(bl);
    }
  } else {
    HiBlock *bl;
    bl=new FixedHiBlock(x1,y1,h0);hblockers.add(bl);
    bl=new FixedHiBlock(x1,y1+1,h0);hblockers.add(bl);
    bl=new FixedHiBlock(x2+1,y1,h1);hblockers.add(bl);
    bl=new FixedHiBlock(x2+1,y1+1,h1);hblockers.add(bl);
    for(int x=x1+1;x<=x2;x++) {
      bl=blocker(x,y1,x-x1);hblockers.add(bl);
      bl=blocker(x,y1+1,x-x1);hblockers.add(bl);
    }
  }
  game->surmap->block(x1,y1);
  game->surmap->block(x2,y2);
}

void TrackTubus::destroy() {
  for(int i=0;i<hblockers.count;i++) delete hblockers[i];
  hblockers.clear();
  for(int i=0;i<sqblocks.count;i++) delete sqblocks[i];
  sqblocks.clear();
  int x1=x,y1=y,x2=x+w-1,y2=y+h-1;
  game->surmap->unblock(x1,y1);
  game->surmap->unblock(x2,y2);
  for(int i=0;i<items.count;i++) delete items[i];
  items.clear();
  game->rrmap->modify();
  Static::destroy();
}



/*
TrackItem *TrackBridge::createritem(int x,int y,bool vertical,bool edge,float ha) {
  return new BridgeRItem(this,x,y,ha,vertical,edge);
}
*/
HiBlock *TrackBridge::blocker(int x,int y,int index) {
  return new MaxHiBlock(x,y,h0-1+index*dhisum/len);
}

void TrackBridge::xmlsave(XMLNode xml) {
  TrackTubus::xmlsave(xml);
  xml.setname(getxmlname());
  xml.setvar("type",bridge->name);
}

/*
void RTubus::created(bool createritems) {
  int x1=x,y1=y,x2=x+w-1,y2=y+h-1;
  if (vertical) {
    for(int y=y1;y<=y2;y++) {
      RItem *ri=createritem(x1,y,vertical,y==y1||y==y2);
      items.add(ri);
      game->rrmap->build(ri);
    }
    HiBlock *bl;
    bl=new FixedHiBlock(game,x1,y1,h0);game->himap->addblock(bl);hblockers.add(bl);
    bl=new FixedHiBlock(game,x1+1,y1,h0);game->himap->addblock(bl);hblockers.add(bl);
    bl=new FixedHiBlock(game,x1,y2+1,h0);game->himap->addblock(bl);hblockers.add(bl);
    bl=new FixedHiBlock(game,x1+1,y2+1,h0);game->himap->addblock(bl);hblockers.add(bl);
    for(int y=y1+1;y<=y2;y++) {
      bl=blocker(x1,y);hblockers.add(bl);game->himap->addblock(bl);
      bl=blocker(x1+1,y);hblockers.add(bl);game->himap->addblock(bl);
    }
  } else {
    for(int x=x1;x<=x2;x++) {
      RItem *ri=createritem(x,y1,vertical,x==x1||x==x2);
      items.add(ri);
      game->rrmap->build(ri);
    }
    HiBlock *bl;
    bl=new FixedHiBlock(game,x1,y1,h0);game->himap->addblock(bl);hblockers.add(bl);
    bl=new FixedHiBlock(game,x1,y1+1,h0);game->himap->addblock(bl);hblockers.add(bl);
    bl=new FixedHiBlock(game,x2+1,y1,h0);game->himap->addblock(bl);hblockers.add(bl);
    bl=new FixedHiBlock(game,x2+1,y1+1,h0);game->himap->addblock(bl);hblockers.add(bl);
    for(int x=x1+1;x<=x2;x++) {
      bl=blocker(x,y1);hblockers.add(bl);game->himap->addblock(bl);
      bl=blocker(x,y1+1);hblockers.add(bl);game->himap->addblock(bl);
    }
  }
  game->surmap->block(x1,y1);
  game->surmap->block(x2,y2);
}
*/

void TrackTubus::xmlsave(XMLNode xml) {
  Static::xmlsave(xml);
  xml.setvar("vertical",vertical);
  xml.setvar("h0",h0);
  xml.setvar("dhisum",dhisum);
  xml.setvar("len",len);
  bstring its;
  for(int i=0;i<items.count;i++) its<<itos(items[i]->saveid)<<";";
  xml.setvar("items",its);
}

TrackTubus::TrackTubus(XMLNode xml,TrackMap *map):Static(xml) {
  vertical=xml.ivar("vertical");
  h0=xml.fvar("h0");
  dhisum=xml.fvar("dhisum");
  len=xml.ivar("len");
  for(lineiter it=xml.svar("items").getlines(';');*it;it++) {
    TrackItem *ri=map->finditemid(atoi(*it));
    if (!ri) throw JError("Error loading bridge");
    items.add(ri);
  }
}

TrackBridge::TrackBridge(XMLNode xml,TrackMap *map):TrackTubus(xml,map) {
  if (items.count>0) items[0]->setedge(true),items.last()->setedge(true);
  for(int i=0;i<items.count;i++) items[i]->setbridge(this);
  bridge=gimages->findbridge(xml.svar("type"));
  createblocks();
  createbridgeitems();
  createbridgeblocks();
}

void TrackTunnel::settype(string value) {
  delize();
  tunnel=gimages->findtunnel(value);
}

void TrackBridge::settype(string value) {
  delize();
//  bridgeitems.clear();
  bridge=gimages->findbridge(value);
  createbridgeitems();
}

void TrackTunnel::xmlsave(XMLNode xml) {
  TrackTubus::xmlsave(xml);
  xml.setname(getxmlname());
  xml.setvar("type",tunnel->name);
}
HiBlock *TrackTunnel::blocker(int x,int y,int index) {
  return new MinHiBlock(x,y,h0+1+index*dhisum/len);
}

void TrackTunnel::changedtype() {
}

void TrackTunnel::markhiview() {
//  FaceStruct &f1=game->himap->faces(x,y),&f2=game->himap->faces(x+w-1,y+h-1);
  if (HVFace *f1=game->hiview->tryface(x,y)) {
    f1->settunnel(true,vertical,dhisum/len);
    f1->tshor=tunnel->horspace;f1->tsver=tunnel->verspace;
  }
  if (HVFace *f2=game->hiview->tryface(x+w-1,y+h-1)) {
    f2->settunnel(false,vertical,dhisum/len);
    f2->tshor=tunnel->horspace;f2->tsver=tunnel->verspace;
  }
}

void TrackTunnel::createtunnelblocks() {
  game->himap->block(x,y,2,2);
  game->himap->block(x+w-1,y+h-1,2,2);
}

static void vertex(bool vertical,float x,float y,float z) {
  if (vertical) glVertex3f(x,y,z);
  else glVertex3f(z,y,x);
}

static void rendertunneltop(bool vertical,int len,jtTunnel *tunnel,int x,int y,float h0,float dhisum) {
  if (!vertical) exchange(x,y);
  float dhi=dhisum/len;
  float yd=1-tunnel->verspace;
  float tw=1-2*tunnel->horspace;
  glBegin(GL_QUADS);
  //leva zed
  glTexCoord2f(0,0);vertex(vertical,x+tunnel->horspace,h0+dhi,y+1);//vepredu dole
  glTexCoord2f(0,yd*tunnel->topscale);vertex(vertical,x+tunnel->horspace,h0+dhi+yd,y+1);//vepredu nahore
  glTexCoord2f((len-2)*tunnel->topscale,yd*tunnel->topscale);vertex(vertical,x+tunnel->horspace,h0+dhisum-dhi+yd,y+len-1);//vzadu nahore
  glTexCoord2f((len-2)*tunnel->topscale,0);vertex(vertical,x+tunnel->horspace,h0+dhisum-dhi,y+len-1);//vzadu dole
  //prava zed
  glTexCoord2f(0,0);vertex(vertical,x+1-tunnel->horspace,h0+dhi,y+1);
  glTexCoord2f(0,yd*tunnel->topscale);vertex(vertical,x+1-tunnel->horspace,h0+dhi+yd,y+1);
  glTexCoord2f((len-2)*tunnel->topscale,yd*tunnel->topscale);vertex(vertical,x+1-tunnel->horspace,h0+dhisum-dhi+yd,y+len-1);
  glTexCoord2f((len-2)*tunnel->topscale,0);vertex(vertical,x+1-tunnel->horspace,h0+dhisum-dhi,y+len-1);
  //horni zed
  glTexCoord2f(0,0);vertex(vertical,x+tunnel->horspace,h0+dhi+yd,y+1);//vepredu vlevo
  glTexCoord2f(0,tw*tunnel->topscale);vertex(vertical,x+1-tunnel->horspace,h0+dhi+yd,y+1);//vepredu vpravo
  glTexCoord2f((len-2)*tunnel->topscale,tw*tunnel->topscale);vertex(vertical,x+1-tunnel->horspace,h0+dhisum-dhi+yd,y+len-1);//vzadu vpravo
  glTexCoord2f((len-2)*tunnel->topscale,0);vertex(vertical,x+tunnel->horspace,h0+dhisum-dhi+yd,y+len-1);//zvadu vlevo
  glEnd();
}

static void rendertunnelbot(bool vertical,int len,jtTunnel *tunnel,int x,int y,float h0,float dhisum) {
  if (!vertical) exchange(x,y);
  float dhi=dhisum/len;
  float yd=1-tunnel->verspace;
  float tw=1-2*tunnel->horspace;
  glBegin(GL_QUADS);
  //podlaha
  glTexCoord2f(0,0);vertex(vertical,x+tunnel->horspace,h0,y);//vepredu vlevo
  glTexCoord2f(0,tw*tunnel->botscale);vertex(vertical,x+1-tunnel->horspace,h0,y);//vepredu vpravo
  glTexCoord2f(len*tunnel->botscale,tw*tunnel->botscale);vertex(vertical,x+1-tunnel->horspace,h0+dhisum,y+len);//vzadu vpravo
  glTexCoord2f(len*tunnel->botscale,0);vertex(vertical,x+tunnel->horspace,h0+dhisum,y+len);//zvadu vlevo
  glEnd();
}

void TrackTunnel::ot_render(int id) {
  visible=true;
}

void TrackTunnel::render(RenderContext &rc) {
  if (!visible) return;
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(x,h0,y);
  glMapCrook(vertical,dhisum/len);
  tunnel->render(rc,vertical?3:2);

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPushMatrix();

  glTranslatef(x+w-1,h0+dhisum-dhisum/len,y+h-1);
  glMapCrook(vertical,dhisum/len);
  tunnel->render(rc,vertical?1:0);

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
//  if (game->view->type && len>2) {//vnitrek tunelu
//    rc.push();
    if (tunnel->top) tunnel->top->bind(rc.ctxid);
    rc.setculling(false);
    bool tsave=rc.settexture2d(true);
    rendertunneltop(vertical,len,tunnel,x,y,h0,dhisum);
    if (tunnel->bot) tunnel->bot->bind(rc.ctxid);
    rendertunnelbot(vertical,len,tunnel,x,y,h0,dhisum);
    rc.settexture2d(tsave);
//  }
}

void TrackTunnel::clearvisibility() {
  visible=false;
}
void TrackTunnel::dooctalize() {
  float hmin=h0,hmax=h0+dhisum;
  if (hmax<hmin) exchange(hmin,hmax);
  hmax+=1;
  float a[3]={x,hmin,y};
  float b[3]={x+w,hmax,y+h};
  gocttree->add(otItem(this,0),a,b);
}
void TrackTunnel::dodelize() {
  gocttree->removegroup(this);
}

void TrackTunnel::destroy() {
  game->himap->unblock(x,y,2,2);
  game->himap->unblock(x+w-1,y+h-1,2,2);
//  game->himap->faces(x,y).unsettunnel();
//  game->himap->faces(x+w-1,y+h-1).unsettunnel();
  TrackTubus::destroy();
}

TrackTunnel::TrackTunnel(XMLNode xml,TrackMap *map):TrackTubus(xml,map) {
  tunnel=gimages->findtunnel(xml.svar("type"));
  if (items.count>0) items[0]->setedge(true),items.last()->setedge(true);
  for(int i=0;i<items.count;i++) items[i]->settunnel(this);
  createblocks();
  createtunnelblocks();
  changedtype();
}

void TrackTunnel::created() {
  createritems();
  createblocks();
  createtunnelblocks();
}

TrackTunnel::TrackTunnel(bool avertical,int x,int y,int alen,float ah0,float adhisum):TrackTubus(avertical,x,y,alen,ah0,adhisum) {
  tunnel=gimages->tunnel0;
}

RTunnel::RTunnel(bool avertical,int x,int y,int len,float ah0,float dhisum):TrackTunnel(avertical,x,y,len,ah0,dhisum) {
  created();
}
RTunnel::RTunnel(XMLNode xml):TrackTunnel(xml,game->rrmap) {
}

RBridge::RBridge(bool avertical,int x,int y,int len,float ah0,float dhisum):TrackBridge(avertical,x,y,len,ah0,dhisum) {
  created();
}
RBridge::RBridge(XMLNode xml):TrackBridge(xml,game->rrmap) {
}
TrackItem *RTunnel::createritem(int x,int y,bool vertical,bool inedge,float ha) {
  return new TunnelRItem(this,x,y,ha,vertical,inedge);
}
TrackItem *RBridge::createritem(int x,int y,bool vertical,bool edge,float ha) {
  return new BridgeRItem(this,x,y,ha,vertical,edge);
}


RoadTunnel::RoadTunnel(bool avertical,int x,int y,int len,float ah0,float dhisum):TrackTunnel(avertical,x,y,len,ah0,dhisum) {
  created();
}
RoadTunnel::RoadTunnel(XMLNode xml):TrackTunnel(xml,game->roadmap) {
}

RoadBridge::RoadBridge(bool avertical,int x,int y,int len,float ah0,float dhisum):TrackBridge(avertical,x,y,len,ah0,dhisum) {
  created();
}
RoadBridge::RoadBridge(XMLNode xml):TrackBridge(xml,game->roadmap) {
}

TrackItem *RoadTunnel::createritem(int x,int y,bool vertical,bool inedge,float ha) {
  return new TunnelRoadItem(this,x,y,ha,vertical,inedge);
}
TrackItem *RoadBridge::createritem(int x,int y,bool vertical,bool edge,float ha) {
  return new BridgeRoadItem(this,x,y,ha,vertical,edge);
}



}

//#pragma package(smart_init)
