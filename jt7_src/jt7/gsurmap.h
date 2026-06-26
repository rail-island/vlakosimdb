#ifndef gsurmapH
#define gsurmapH

#include "game0.h"
#include <com/dvslist.h>

namespace jtyc {
class jtTree;

struct SqBlocked:EBlocked {
  int x,y;
  SqBlocked(int ax,int ay) {x=ax;y=ay;}
  Volatile *createvolatile();
};

struct SqBlock:DVSElem {
  int x,y;
  SqBlock *next;
  SqBlock(int ax,int ay);
  ~SqBlock();
//  SqBlock(BINREAD &fr):GObject(fr) {fr>>x>>y>>next;}
//  void save(BINWRITE &fw) {GObject::save(fw);fw<<x<<y<<next;}
  virtual bool allowrr(bool vertical,float za,float dzx,float dzy) {return false;}
  virtual bool allowbridge(bool vertical,float z) {return false;}//zda se povoluje postavit most ve vysce z
  virtual bool allowtree() {return false;}
};

struct BridgeBlock:SqBlock {
  bool vertical;float z;
  bool allowrr(bool avertical,float za,float dzx,float dzy) {
    int v=jmax(za,za+dzx,za+dzy);
    return vertical!=avertical && v+beop.train_hi<=z;
  }
//  BridgeBlock(BINREAD &fr):SqBlock(fr) {fr>>vertical>>z;}
//  void save(BINWRITE &fw) {SqBlock::save(fw);fw<<vertical<<z;}
  BridgeBlock(int x,int y,bool avertical,float az):SqBlock(x,y) {
    vertical=avertical;z=az;
  }
};
struct RailBlock:SqBlock {
  bool vertical;float v;
  bool allowbridge(bool avertical,float z) {
    return vertical!=avertical && v+beop.train_hi<=z;
  }
//  RailBlock(BINREAD &fr):SqBlock(fr) {fr>>vertical>>v;}
  RailBlock(int x,int y,bool avertical,float za,float dzx,float dzy):SqBlock(x,y) {
    v=jmax(za,za+dzx,za+dzy);vertical=avertical;
  }
//  void save(BINWRITE &fw) {SqBlock::save(fw);fw<<vertical<<v;}
};


struct SItem {
  SItem *nextdirty;
  short blocked;
  short denydraw;
  bool water;
  bool visibletree;
  bool dirty;
  short x,y;
  
  jtTree *tree;
  jtTerrain *terrain;

//  jtTerrain
//  *ed,  *ec,
//  *ea,  *eb;//edges, counted by countedges()

//  SurMap *map;

  bool wantsave();
  void save(XMLNode xml);
  void load(XMLNode xml);
  void unplant() {tree=NULL;}
  void plant(string name);
  SItem();
//  SItem() {blocked=0;map=NULL;denydraw=0;}
//  void load(BINREAD &fr) {fr>>water>>blocked>>denydraw;}
//  void save(BINWRITE &fw) {fw<<water<<blocked<<denydraw;}
};

struct SurMap:GOTObject {
  TMatrix<SqBlock*> mtx;
  DVSList blocks;
//  OWNER<RefMatrix<SqBlock> > mtx;
//  SItem *buf;
  TMatrix<SItem> map;
  int w,h,count;
  SItem *dirtys;
  SurMap();

  void init();
//  SurMap(XMLNode xml);
  void doinit(int aw,int ah);
//  ~SurMap() {delete []buf;}
  bool freesq(int x,int y) {return mtx.at(x,y)==NULL && !blocked(x,y);}
  bool valid(int x,int y) {return x>=0 and x<w and y>=0 and y<h;}
  bool allowrr(int x,int y,bool vertical,float z,float dzx,float dzy);
  bool allowbridge(int x,int y,bool vertical,float z);
  SItem *at(int x,int y) {return map.tryat(x,y);}//valid(x,y)?buf+y*w+x:NULL;}
  void block(int x,int y) {_block(x,y,1);}
  void unblock(int x,int y) {_block(x,y,-1);}
  bool freeplane(int x,int y,int w,int h,bool throwblock=false);
  bool allowbridge(int x,int y,bool vertical,int w,int h,int z) {
    for(int cx=0;cx<w;cx++) for(int cy=0;cy<h;cy++) if (not allowbridge(x+cx,y+cy,vertical,z)) return false;
    return true;
  }
  bool allowtree(int x,int y);
  bool blocked(int x,int y) {if (SItem *s=at(x,y)) return s->blocked;return true;}
  void erase(SqBlock *sq);
  int blockcount(int x,int y);//vrati pocet zablokovani(SItem::blocked+pocet SqItem v spojaku)
  void block(int x,int y,int w,int h) {_block(x,y,w,h,1);}
  void unblock(int x,int y,int w,int h) {_block(x,y,w,h,-1);}
  void xmlsave(XMLNode xml);
  void xmlload(XMLNode xml);
  void save(Archive &ar);
  void load(Archive &ar);
  bool candraw(int x,int y) {if (SItem *s=at(x,y)) return s->denydraw==0;return false;}
  void denydraw(int x,int y) {if (SItem *s=at(x,y)) s->denydraw++;}
  void allowdraw(int x,int y) {if (SItem *s=at(x,y)) s->denydraw--;}
  void unplant(int x,int y,int w,int h) {for(int cx=0;cx<w;cx++) for(int cy=0;cy<h;cy++) map(x+cx,y+cy).unplant();}
  void plant(int x,int y,string tree);
  void render(RenderContext &rc);
  void setterrain(int x,int y,string terrain);
//  void countedges();
//  void wantedges() { if (!countededges) countedges();}
  void octalize();
  void delize();
  void clearvisibility();

  void createdblock(SqBlock *bl);
  void erasedblock(SqBlock *block);

  void ot_render(int id);
private:
  int boundtreex1,boundtreey1,boundtreex2,boundtreey2;
  bool octalized;
//  bool countededges;
  void _block(int x,int y,int d) {if (SItem *s=at(x,y)) s->blocked+=d;}
  void _block(int x,int y,int w,int h,int d) {
    for(int cx=0;cx<w;cx++) for(int cy=0;cy<h;cy++) _block(x+cx,y+cy,d);
  }
};

}

#endif
