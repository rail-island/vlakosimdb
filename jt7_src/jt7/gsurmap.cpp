#include <jlib.h>
#pragma hdrstop

#include "gsurmap.h"
#include "gameu.h"
#include "gvolatile.h"
#include "gimages.h"
#include "ghimap.h"

namespace jtyc {

SItem::SItem() {
  blocked=0;denydraw=0;tree=NULL;x=y=-1;
  terrain=gimages->terrain0;
//  ea=eb=ec=ed=NULL;
  nextdirty=NULL;
  dirty=false;
}

SqBlock::SqBlock(int ax,int ay) {
  x=ax;y=ay;next=NULL;
  game->surmap->createdblock(this);
}
SqBlock::~SqBlock() {
  game->surmap->erasedblock(this);
}
Volatile *SqBlocked::createvolatile() {
  return new BadSq(x,y);
}

void SurMap::erasedblock(SqBlock *block) {
  mtx.at(block->x,block->y)=unlink_from_unidir_list(mtx(block->x,block->y),block);
}

void SurMap::save(Archive &ar) {
  MainXMLNode xml;
  xmlsave(xml);
  ar.savexml("surmap.xml",xml);
}
void SurMap::load(Archive &ar) {
  MainXMLNode xml;
  if (ar.findfile("surmap.xml")>=0) {
    ar.loadxml("surmap.xml",xml);
    xmlload(xml);
  }
}

void SItem::plant(string name) {
  tree=gimages->searchtree(name);
}

void SurMap::plant(int x,int y,string tree) {
  map(x,y).plant(tree);
  delize();
}

/*
void SurMap::countedges() {
  countededges=true;
  for(int y=0;y<map.h;y++) for(int x=0;x<map.w;x++) {
    map(x,y).ea=map(x,y).eb=map(x,y).ec=map(x,y).ed=NULL;
  }
  for(int y=0;y<map.h;y++) for(int x=0;x<map.w;x++) {
    SItem &it=map(x,y);
    if (SItem *d=map.tryat(x+1,y+1)) {
      if (d->terrain==it.terrain) {
        SItem &n1=map(x,y+1),&n2=map(x+1,y);
        if (n1.terrain!=it.terrain && n2.terrain==it.terrain) n1.ec=it.terrain;
        if (n2.terrain!=it.terrain && n1.terrain==it.terrain) n2.ea=it.terrain;
      }
    }
    if (SItem *d=map.tryat(x+1,y-1)) {
      if (d->terrain==it.terrain) {
        SItem &n1=map(x,y-1),&n2=map(x+1,y);
        if (n1.terrain!=it.terrain && n2.terrain==it.terrain) n1.eb=it.terrain;
        if (n2.terrain!=it.terrain && n1.terrain==it.terrain) n2.ed=it.terrain;
      }
    }
  }

  for(int y=0;y<map.h;y++) for(int x=0;x<map.w-1;x++) {
    SItem &a1=map(x,y),&a2=map(x+1,y);
    if (a1.terrain==a2.terrain) {
      if (y>0) {
        SItem &b1=map(x,y-1),&b2=map(x+1,y-1);
        if (b1.terrain!=b2.terrain) {
          if (!b1.eb) b1.eb=a1.terrain;
          if (!b2.ea) b2.ea=a1.terrain;
        }
      }
      if (y<map.h-1) {
        SItem &b1=map(x,y+1),&b2=map(x+1,y+1);
        if (b1.terrain!=b2.terrain) {
          if (!b1.ec) b1.ec=a1.terrain;
          if (!b2.ed) b2.ed=a1.terrain;
        }
      }
    }
  }

  for(int y=0;y<map.h-1;y++) for(int x=0;x<map.w;x++) {
    SItem &a1=map(x,y),&a2=map(x,y+1);
    if (a1.terrain==a2.terrain) {
      if (x>0) {
        SItem &b1=map(x-1,y),&b2=map(x-1,y+1);
        if (b1.terrain!=b2.terrain) {
          if (!b1.eb) b1.eb=a1.terrain;
          if (!b2.ec) b2.ec=a1.terrain;
        }
      }
      if (x<map.w-1) {
        SItem &b1=map(x+1,y),&b2=map(x+1,y+1);
        if (b1.terrain!=b2.terrain) {
          if (!b1.ea) b1.ea=a1.terrain;
          if (!b2.ed) b2.ed=a1.terrain;
        }
      }
    }
  }
}
*/

void SurMap::clearvisibility() {
//  for(SItem *act=map.buf,*end=map.buf+map.size;act<end;act++) act->visibletree=false;
//  for(int i=0;i<map.size;i++) map[i].visibletree=false;
  for(SItem *act=dirtys;act;act=act->nextdirty) act->visibletree=false;
  cleardirty(&dirtys);
  boundtreex1=map.w;
  boundtreey1=map.h;
  boundtreex2=-1;
  boundtreey2=-1;
}

void SurMap::octalize() {
  if (octalized) return;
  for(int y=0;y<map.h;y++) for(int x=0;x<map.w;x++) {
    jtTree *tree=map(x,y).tree;
    if (tree && allowtree(x,y) && game->himap->drysq(x,y)) {
      float a[3],b[3];
      float w=tree->getw(),h=tree->geth();
      a[0]=x+0.5-w/2;a[2]=y+0.5-w/2;
      b[0]=x+0.5+w/2;b[2]=y+0.5+w/2;
      float z=(game->himap->hiat(x,y)+game->himap->hiat(x+1,y)+game->himap->hiat(x,y+1)+game->himap->hiat(x+1,y+1))/4;
      a[1]=z;
      b[1]=z+h;
      gocttree->add(otItem(this,x+y*map.w),a,b);
    }
  }
  octalized=true;
}
void SurMap::ot_render(int id) {
  map[id].visibletree=true;
  insertdirty(&dirtys,&map[id]);
  int x=id%map.w,y=id/map.w;
  if (x<boundtreex1) boundtreex1=x;
  if (y<boundtreey1) boundtreey1=y;
  if (x>boundtreex2) boundtreex2=x;
  if (y>boundtreey2) boundtreey2=y;
}
void SurMap::delize() {
  if (!octalized) return;
  gocttree->removegroup(this);
  octalized=false;
}

bool SItem::wantsave() {
  return tree || terrain!=gimages->terrain0;
}

void SItem::save(XMLNode xml) {
  if (tree) xml.setvar("tree",tree->getname());
  if (terrain!=gimages->terrain0) xml.setvar("terrain",terrain->name);
  xml.setvar("x",x);
  xml.setvar("y",y);
}
void SItem::load(XMLNode xml) {
  tree=gimages->searchtree(xml.svar("tree"));
  terrain=gimages->findterrain(xml.svar("terrain"));
}

void SurMap::xmlsave(XMLNode xml) {
  xml.setname("surmap");
  for(int i=0;i<map.size;i++) if (map[i].wantsave()) map[i].save(xml.add("item"));
}
void SurMap::xmlload(XMLNode xml) {
  for(int i=0;i<xml.count();i++) map(xml[i].ivar("x"),xml[i].ivar("y")).load(xml[i]);
}

void SurMap::setterrain(int x,int y,string terrain) {
  if (SItem *sq=at(x,y)) {
    sq->terrain=gimages->findterrain(terrain);
//    countededges=false;
  }
}

void SurMap::render(RenderContext &rc) {
  rc.setculling(false);
  for(int y=boundtreey1;y<=boundtreey2;y++) {
    for(int x=boundtreex1;x<=boundtreex2;x++) {
//      if (!game->himap->faceat(x,y).octvisible) continue;
      jtTree *tree=map(x,y).tree;
      if (oglo.drawtrees && map(x,y).visibletree && tree && allowtree(x,y) && game->himap->drysq(x,y)) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        float z=(game->himap->hiat(x,y)+game->himap->hiat(x+1,y)+game->himap->hiat(x,y+1)+game->himap->hiat(x+1,y+1))/4;
        glTranslatef(x+0.5,z,y+0.5);
        tree->render(rc);
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
      }
    }
  }
}

int SurMap::blockcount(int x,int y) {
  xassert(valid(x,y));
  int res=at(x,y)->blocked;
  SqBlock *act=mtx.at(x,y);
  while (act) act=act->next,res++;
  return res;
}

void SurMap::erase(SqBlock *bl) {
  xassert(bl);
  SqBlock *act=mtx.at(bl->x,bl->y);
  SqBlock *last=NULL;
  while (act && act!=bl) last=act,act=act->next;
  xassert(act);
  if (last) last->next=act->next;else mtx.set(bl->x,bl->y,act->next);
}

void SurMap::createdblock(SqBlock *bl) {
  blocks.addlast(bl);
  SqBlock *next=mtx.at(bl->x,bl->y);
  mtx.set(bl->x,bl->y,bl);
  bl->next=next;
}

bool SurMap::allowrr(int x,int y,bool vertical,float z,float dzx,float dzy) {
  if (blocked(x,y)) return false;
  SqBlock *bl=mtx.at(x,y);
  while (bl) {
    if (! bl->allowrr(vertical,z,dzx,dzy)) return false;
    bl=bl->next;
  }
  return true;
}
bool SurMap::allowbridge(int x,int y,bool vertical,float z) {
  if (blocked(x,y)) return false;
  SqBlock *bl=mtx.at(x,y);
  while (bl) {
    if (! bl->allowbridge(vertical,z)) return false;
    bl=bl->next;
  }
  return true;
}

bool SurMap::allowtree(int x,int y) {
  if (blocked(x,y)) return false;
  SqBlock *bl=mtx.at(x,y);
  while (bl) {
    if (! bl->allowtree()) return false;
    bl=bl->next;
  }
  return true;
}

bool SurMap::freeplane(int x,int y,int w,int h,bool throwblock) {
  for(int cy=0;cy<h;cy++) for(int cx=0;cx<w;cx++) if (! freesq(x+cx,y+cy)) {
    if (throwblock) throw SqBlocked(x+cx,y+cy);
    return false;
  }
  return true;
}

void SurMap::init() {
  doinit(game->W,game->H);
}

void SurMap::doinit(int aw,int ah) {
  w=aw;h=ah;count=w*h;
  map.init(w,h);
  mtx.init(w,h);
  mtx.fill(NULL);
  for(int y=0;y<map.h;y++) for(int x=0;x<map.w;x++) map(x,y).x=x,map(x,y).y=y;
}

SurMap::SurMap() {
  w=h=count=0;
  dirtys=NULL;
//  countededges=false;
  octalized=false;
}

}

