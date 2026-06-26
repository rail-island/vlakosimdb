#include <jlib.h>
#pragma hdrstop

#include "ghimap.h"
#include "gview.h"
#include "gameu.h"
#include <opengl.h>
#include "gimages.h"
#include "gvolatile.h"
#include "gsurmap.h"
#include <jmath/analgeom.cpp>

namespace jtyc {

HiBlock::HiBlock(int ax,int ay) {
  x=ax;y=ay;next=NULL;
  game->himap->createdblock(this);
}

HiBlock::~HiBlock() {
  game->himap->erasedblock(this);
}

Volatile *HiBlocked::createvolatile() {
  return new BadVr(x,y);
}

void HiMap::erasedblock(HiBlock *block) {
  mtx.at(block->x,block->y)=unlink_from_unidir_list(mtx(block->x,block->y),block);
}

void HiMap::createdblock(HiBlock *bl) {
  blocks.addlast(bl);
  HiBlock *next=mtx.at(bl->x,bl->y);
  mtx.set(bl->x,bl->y,bl);
  bl->next=next;
}

void HiMapBackup::storemap() {
  for(int i=0;i<map->verteces.size;i++) buf[i]=map->verteces[i].hi;
}
void HiMapBackup::restoremap() {
  for(int i=0;i<map->verteces.size;i++) map->verteces[i].hi=buf[i];
}

void HiMap::risecircle(float x,float y,float rad,float dhi) {
  circle(x,y,rad,dhi);
  repairwater(HMBorders(x-rad-2,y-rad-2,x+rad+2,y+rad+2));
  modify(x-rad-2,y-rad-2,x+rad+2,y+rad+2);
//  modified=true;
//  delize();
}

void HiMap::circle(float mx,float my,float size,float disp) {
  int x0=mx-size-1;if (x0<0) x0=0;
  int y0=my-size-1;if (y0<0) y0=0;
  int x1=mx+size+2;if (x1>verteces.w) x1=verteces.w;
  int y1=my+size+2;if (y1>verteces.h) y1=verteces.h;
  for(int x=x0;x<x1;x++) {
    for(int y=y0;y<y1;y++) {
      float pd=distance(fpoint(x,y),fpoint(mx,my));
      if (abs(pd)<=size) {
        if (verteces(x,y).blocked) continue;
        if (mtx(x,y)) continue;
        float &h=hiat(x,y);
        h+=disp/2+cos(pd/size*M_PI)*disp/2;
        h=round(h*1024)/1024.0;
        if (h<0) h=0;
        if (h>maxmaphi) h=maxmaphi;
      }
    }
  }
}

void HiMap::repairwater(int x,int y) {
  if (denyrepairwater) return;
  
  VertexStruct &v=verteces(x,y);
  if (v.waterhi<v.hi) v.waterhi=-1;

  VertexStruct *a=verteces.tryat(x+1,y);
  VertexStruct *b=verteces.tryat(x-1,y);
  VertexStruct *c=verteces.tryat(x,y+1);
  VertexStruct *d=verteces.tryat(x,y-1);

  if (a) if (a->waterhi>v.waterhi) v.waterhi=a->waterhi;
  if (b) if (b->waterhi>v.waterhi) v.waterhi=b->waterhi;
  if (c) if (c->waterhi>v.waterhi) v.waterhi=c->waterhi;
  if (d) if (d->waterhi>v.waterhi) v.waterhi=d->waterhi;

  if (v.waterhi<v.hi) v.waterhi=-1;

  if (a) if (a->waterhi<v.waterhi) fillwater(x+1,y,v.waterhi);
  if (b) if (b->waterhi<v.waterhi) fillwater(x-1,y,v.waterhi);
  if (c) if (c->waterhi<v.waterhi) fillwater(x,y+1,v.waterhi);
  if (d) if (d->waterhi<v.waterhi) fillwater(x,y-1,v.waterhi);
}

void HiMap::repairwater(HMBorders brd) {
  brd.repair(verteces.w,verteces.h);
  for(int y=brd.y1;y<=brd.y2;y++) for(int x=brd.x1;x<=brd.x2;x++) repairwater(x,y);
}

/*
void HiMap::repairwater() {
  return;
  if (denyrepairwater) return;
  changedwater();//water vsede>=hi nebo ==-1
  for(int i=0;i<verteces.size;i++) {
    VertexStruct &vs=verteces[i];
    if (vs.waterhi<0) {
      float maxwhi=-1;//maximalni vyska hladiny v okoli
      if (VertexStruct *b=verteces.tryat(vs.x-1,vs.y)) if (b->waterhi>maxwhi) maxwhi=b->waterhi;
      if (VertexStruct *b=verteces.tryat(vs.x+1,vs.y)) if (b->waterhi>maxwhi) maxwhi=b->waterhi;
      if (VertexStruct *b=verteces.tryat(vs.x,vs.y-1)) if (b->waterhi>maxwhi) maxwhi=b->waterhi;
      if (VertexStruct *b=verteces.tryat(vs.x,vs.y+1)) if (b->waterhi>maxwhi) maxwhi=b->waterhi;
      if (maxwhi>=vs.hi) {//tento ctverecek je zatopeny
        bool mustdry=false;
        if (VertexStruct *b=verteces.tryat(vs.x-1,vs.y)) if (b->maxhi()<maxwhi) mustdry=true;
        if (VertexStruct *b=verteces.tryat(vs.x+1,vs.y)) if (b->maxhi()<maxwhi) mustdry=true;
        if (VertexStruct *b=verteces.tryat(vs.x,vs.y-1)) if (b->maxhi()<maxwhi) mustdry=true;
        if (VertexStruct *b=verteces.tryat(vs.x,vs.y+1)) if (b->maxhi()<maxwhi) mustdry=true;
        if (mustdry) {
          if (verteces.tryat(vs.x-1,vs.y)) drywater(vs.x-1,vs.y);
          if (verteces.tryat(vs.x+1,vs.y)) drywater(vs.x+1,vs.y);
          if (verteces.tryat(vs.x,vs.y-1)) drywater(vs.x,vs.y-1);
          if (verteces.tryat(vs.x,vs.y+1)) drywater(vs.x,vs.y+1);
        } else fillwater(vs.x,vs.y,maxwhi);
      }
    }

  }
}
*/

void FaceStruct::setbroken() {
  if (octalized) gocttree->quickremove(otItem(game->himap,y*game->himap->faces.w+x),octalized),octalized=NULL;
  mac_insert_dirty(FaceStruct,&game->himap->broken_list,this,next_broken,broken)
}

void HiMap::modify(int x1,int y1,int x2,int y2) {
  if (x1<0) x1=0;if (x2>=faces.w) x2=faces.w-1;
  if (y1<0) y1=0;if (y2>=faces.h) y2=faces.h-1;
  for(int y=y1;y<=y2;y++) for(int x=x1;x<=x2;x++) faces(x,y).setbroken();
}

void HiMap::doinit(int aw,int ah) {
  clear();
  verteces.init(aw+1,ah+1);
  for(int y=0;y<verteces.h;y++) for(int x=0;x<verteces.w;x++) verteces(x,y).x=x,verteces(x,y).y=y;

  faces.init(aw,ah);
  for(int y=0;y<faces.h;y++) for(int x=0;x<faces.w;x++) faces(x,y).x=x,faces(x,y).y=y;
//  for(int i=0;i<verteces.size-1;i++) verteces[i]->nextdirty=verteces[i+1];
//  dirtyv=verteces[0];
//  for(int i=0;i<verteces.size-1;i++) verteces[i]->nextdirty=verteces[i+1];
//  dirtyf=faces[0];
  for(int i=0;i<faces.size;i++) faces[i].setbroken();
  mtx.init(aw+1,ah+1);
  mtx.fill(NULL);
}

/*
void FaceStruct::settunnel(bool less,bool vertical,float dhi) {
  tunneldhi=dhi;
  if (vertical) {
    if (less) tunneldir=2;else tunneldir=0;
  } else {
    if (less) tunneldir=1;else tunneldir=3;
  }
}
*/

void HiMap::init() {
  doinit(game->W,game->H);
}

/*
void HiMap::countfacenormals() {
  for(int y=1;y<verteces.h;y++) {
    for(int x=1;x<verteces.w;x++) {
      fpoint3d a(get(x,y)),b(get(x-1,y)),c(get(x,y-1));
      fvector3d u=a-b,v=a-c;
      fvector3d normal=u%v;//normala
      faceat(x-1,y-1).n=normal.size1();
    }
  }
}

void HiMap::countvertexnormals() {
  int w=verteces.w,h=verteces.h;
  for(int y=1;y<h-1;y++) {
    for(int x=1;x<w-1;x++) {
      fvector3d v1=faceat(x-1,y-1).n,
                 v2=faceat(x,y-1).n,
                 v3=faceat(x-1,y).n,
                 v4=faceat(x,y).n;
      vertexat(x,y).n=(v1+v2+v3+v4)*0.25;
    }
  }
  vertexat(0,0).n=faceat(0,0).n;
  vertexat(w-1,0).n=faceat(w-2,0).n;
  vertexat(w-1,h-1).n=faceat(w-2,h-2).n;
  vertexat(0,h-1).n=faceat(0,h-2).n;
  for(int x=1;x<w-1;x++) {
    vertexat(x,0).n=((fvector3d)faceat(x-1,0).n+(fvector3d)faceat(x,0).n)/2;
    vertexat(x,h-1).n=((fvector3d)faceat(x-1,h-2).n+(fvector3d)faceat(x,h-2).n)/2;
  }
  for(int y=1;y<h-1;y++) {
    vertexat(0,y).n=((fvector3d)faceat(0,y-1).n+(fvector3d)faceat(0,y).n)/2;
    vertexat(w-1,y).n=((fvector3d)faceat(w-2,y-1).n+(fvector3d)faceat(w-2,y).n)/2;
  }
}
*/

/*
void VertexStruct::countintensity(const fvector3d &L) {
  fvector3d N=n;
  if (N.y<=0) N=-N;
  float k=N*L;
  k/=2;
  k+=1;
  intensity=k;
}
*/

/*
void HiMap::countintensity() {
//  float mink=1000,maxk=-10000;
//  fvector3d sun(cos(oglo.sunangle/180*M_PI),oglo.sunydir,sin(oglo.sunangle/180*M_PI));
  fvector3d sun(1,-1,0);
  fmatrix3d rot;rot.dorotatey(game->view->getviewangle()/180*M_PI);
  sun=rot*sun;
  fvector3d L=sun.size1();
  for(int y=0;y<verteces.h;y++) {
    for(int x=0;x<verteces.w;x++) {
      fvector3d N=vertexat(x,y).n;
      if (N.y<=0) N=-N;
      float k=N*L;
      k/=2;
      k+=1;
      vertexat(x,y).intensity=k;
//      if (k>maxk) maxk=k;
//      if (k<mink) mink=k;
    }
  }
//  int j=0;
}
*/

/*
void HiMap::countbounds() {
}
*/

/*
void HiMap::recount(int flags) {
  if (modified) flags=~0;
  if (flags & CF_CHANGETER) countfacenormals(),countvertexnormals();
//  if (flags & CF_ROTATE) countintensity();
//  uncountvrs();
/*  if ((flags & CF_RESIZE) || (flags & CF_SCALE) || (flags & CF_ROTATE) || (flags & CF_SEEANGLE)) countvrs();
  else if (flags & CF_MOVE) movevrs();*/
//  modified=false;
//

void HiMap::faultalg() {

}

bool HiMap::checkplane(int x0,int y0,int w,int h,float *h0) {
  float hi0=hiat(x0,y0);
  for(int y=0;y<=h;y++) for(int x=0;x<=w;x++) {
    float acth=hiat(x0+x,y0+y);
    if (fabs(acth-hi0)>jtyc::epsilon) {
      game->seterror("$Plane-required");
      game->addvolatile(new BadVr(x0+x,y0+y));
//      throw HiBlocked(x0+x,y0+y);
      return false;
    }
  }
  if (h0) *h0=hi0;
  return true;
}

void HiMap::save(Archive &ar) {
  float *h=new float[verteces.w*verteces.h];
  for(int x=0;x<verteces.w;x++) for(int y=0;y<verteces.h;y++) h[y*verteces.w+x]=verteces(x,y).hi;
  ar.savearray("heightmap.floatarray",h,verteces.w*verteces.h);
  for(int x=0;x<verteces.w;x++) for(int y=0;y<verteces.h;y++) h[y*verteces.w+x]=verteces(x,y).waterhi;
  ar.savearray("watermap.floatarray",h,verteces.w*verteces.h);
  delete []h;
}

void HiMap::load(Archive &ar) {
//  modified=true;
  int cnt=verteces.w*verteces.h;
  float *h=ar.loadarray<float>("heightmap.floatarray",&cnt);
  for(int x=0;x<verteces.w;x++) for(int y=0;y<verteces.h;y++) verteces(x,y).hi=h[y*verteces.w+x];
  delete []h;
  float *wh=ar.loadarray<float>("watermap.floatarray",&cnt);
  for(int x=0;x<verteces.w;x++) for(int y=0;y<verteces.h;y++) verteces(x,y).waterhi=wh[y*verteces.w+x];
  delete []wh;
  modifyall();
}

float HiMap::interpolatehi(float x,float y) {
  float ha=0,hb=0,hc=0,hd=0;
  int xi=(int)x,yi=(int)y;
  if (HItem *it=hat(xi,yi)) ha=it->hi;
  if (HItem *it=hat(xi+1,yi)) hb=it->hi;
  if (HItem *it=hat(xi+1,yi+1)) hc=it->hi;
  if (HItem *it=hat(xi,yi+1)) hd=it->hi;
  float dx=x-xi,dy=y-yi;
  float h1=ha+dy*(hd-ha),h2=hb+dy*(hc-hb);
  return h1+dx*(h2-h1);
}

void HiMap::clear() {
//  if (faces) delete []faces;
//  faces=NULL;
//  if (verteces) delete []verteces;
//  verteces=NULL;
  faces.clear();
  verteces.clear();
//  w=h=0;
}

bool HiMap::cangenerate() {
  bool res=true;
  for(int i=0;i<verteces.size;i++) {
    VertexStruct &vs=verteces[i];
    if (vs.blocked) res=false;
    if (mtx(vs.x,vs.y)) res=false;
  }
  if (!res) {
    game->seterror("$cannot-generate-terrain-not-empty");
    return false;
  }
  for(int i=0;i<verteces.size;i++) verteces[i].waterhi=-1;
//  modified=true;
  return true;
}

void HiMap::circles(int steps,float mindisp,float maxdisp,float minrad,float maxrad) {

  if (!cangenerate()) return;
//  int steps=5000;
//  float mindisp=0.1,maxdisp=2;
//  float minrad=1,maxrad=5;

  int itmindisp=steps/2;
  float radexp=1;
  float raddivfactor=pow(maxrad,radexp);

  for(int i=0;i<steps;i++) {
    float disp,rad;
		if (i<itmindisp) rad=maxrad+((float)i/itmindisp)*(minrad-maxrad);else rad=minrad;
		if (i<itmindisp) disp=maxdisp+((float)i/itmindisp)*(mindisp-maxdisp);else disp=mindisp;
    disp*=pow(rad,radexp)/raddivfactor;
    float x=rand()/(float)RAND_MAX*verteces.w;
    float y=rand()/(float)RAND_MAX*verteces.h;
    circle(x,y,rad,disp);
  }
  modifyall();
//  delize();
//  modified=true;
}

void HiMap::clearhis(float hi0) {
  if (!cangenerate()) return;
  for(int i=0;i<verteces.size;i++) verteces[i].hi=hi0;
  for(int i=0;i<verteces.size;i++) verteces[i].waterhi=-1;
  modifyall();
//  modified=true;
}


//void HiMap::uncountvrs() {
//  for(VertexStruct *act=verteces.buf,*end=verteces.buf+verteces.size;act<end;act++) act->counted=false;
//  for(int i=0;i<verteces.size;i++) verteces[i].uncount();
/*
  for(int y=0;y<verteces.h;y++) {
    for(int x=0;x<verteces.w;x++) {
      vertexat(x,y).uncount();
    }
  }
*/
//}


/*
void HiMap::countvisiblefaces() {
  int w=verteces.w,h=verteces.h;
  for(int y=1;y<h;y++) {
    VertexStruct *line2=verteces.buf+y*w,*line1=line2-w;
    FaceStruct *fline=faces.buf+(y-1)*(w-1);
    for(int x=1;x<w;x++) {
      fline[x-1].fullvisible=line1[x].visible && line1[x-1].visible && line2[x].visible && line2[x-1].visible;
      fline[x-1].partvisible=line1[x].visible || line1[x-1].visible || line2[x].visible || line2[x-1].visible;
      fline[x-1].visibleneightboor=false;
    }
  }
  for(int x=1;x<w-2;x++) for(int y=1;y<h-2;y++) if (faces(x,y).partvisible) {
    faces(x,y+1).visibleneightboor=true;
    faces(x,y-1).visibleneightboor=true;
    faces(x-1,y).visibleneightboor=true;
    faces(x+1,y).visibleneightboor=true;
  }
  for(int i=0;i<faces.size;i++) if (faces[i].visibleneightboor) faces[i].partvisible=true;
}
*/

ZPoint VertexStruct::countscrpt() {
  return game->view->ltop(game->himap->getmax(x,y));
}

/*
void VertexStruct::countvr() {
  insertdirty(&game->himap->dirtyv,this);
  counted=true;
  _scrpt=game->view->ltop(game->himap->getmax(x,y));
  _visible=_scrpt.x>=-1 && _scrpt.x<=1 && _scrpt.y>=-1 && _scrpt.y<=1;
//  if (game->view->type) {
//    visible=game->view->type->visible(fvector3d(x,y,0));
//    visible=true;
//  } else {
//    scrpt=game->view->ltop(game->himap->getmax(x,y));
//    visible=scrpt.x>=-1 && scrpt.x<=1 && scrpt.y>=-1 && scrpt.y<=1;
//  }
}
*/


/*
void VertexStruct::movevr(float dx,float dy) {
  counted=true;
  scrpt.x+=dx;scrpt.y+=dy;
  visible=scrpt.x>=-1 && scrpt.x<=1 && scrpt.y>=-1 && scrpt.y<=1;
}

void HiMap::movevrs() {
  if (!game->view->type) {
    float dx=game->view->dx-counteddx,dy=game->view->dy-counteddy;
    for(int y=0;y<verteces.h;y++)
      for(int x=0;x<verteces.w;x++)
        vertexat(x,y).movevr(-dx,dy);
    countvisiblefaces();
    counteddx=game->view->dx;
    counteddy=game->view->dy;
  } else {
    for(int y=0;y<verteces.h;y++)
      for(int x=0;x<verteces.w;x++)
        vertexat(x,y).countvr();
    countvisiblefaces();
  }
}
*/

/*
void HiMap::countvrs() {
//  int c=clock();
  for(int y=0;y<verteces.h;y++) {
    for(int x=0;x<verteces.w;x++) {
      vertexat(x,y).countvr();
    }
  }
//  int s=clock()-c;
//  c=0;
//  counteddx=game->view->dx,counteddy=game->view->dy;
//  countvisiblefaces();
}
*/

void HiMap::sethi(int x,int y,float hi) {
  if (HItem *i=verteces.tryat(x,y)) {
    i->sethi(hi);
    modify(x-1,y-1,x+1,y+1);
    repairwater(HMBorders(x,y,x,y));
  }
}
void VertexStruct::sethi(float newhi) {
  if (fabs(hi-newhi)<=jtyc::epsilon) return;
  if (blocked) throw HiBlocked(x,y);
  if (!game->himap->allowset(x,y,newhi)) throw HiBlocked(x,y);
  if (newhi<0 || newhi>maxmaphi) throw HiBlocked(x,y);
  hi=newhi;
}

void HiMap::block(int x,int y,int w,int h) {
  for(int j=0;j<h;j++) for(int i=0;i<w;i++) block(x+i,y+j);
}
void HiMap::unblock(int x,int y,int w,int h) {
  for(int j=0;j<h;j++) for(int i=0;i<w;i++) unblock(x+i,y+j);
}

void HiMap::sethi(int x,int y,int w,int h,float hi) {
  denyrepairwater++;
  for(int j=0;j<h;j++) for(int i=0;i<w;i++) sethi(x+i,y+j,hi);
  denyrepairwater--;
  repairwater(HMBorders(x,y,x+w-1,y+h-1));
}

void HiMap::makeinterval(int x,int y,Interval &i) {
  HiBlock *hb=mtx.at(x,y);
  while (hb) {
    hb->makeinterval(i);
    hb=hb->next;
  }
  HItem &v=vertexat(x,y);
  if (v.blocked) {
    if (i.lo<v.hi) i.lo=v.hi;
    if (i.hi>v.hi) i.hi=v.hi;
  }
}

bool HiMap::allowset(int x,int y,float h) {
  HiBlock *hb=mtx.at(x,y);
  while (hb) {
    if (not hb->allowsethi(h)) return false;
    hb=hb->next;
  }
  return true;
}

void HiMap::octalize() {
  countbroken();
//  if (octalized) return;
/*
  for(int y=0;y<faces.h;y++) for(int x=0;x<faces.w;x++) {
    float a[3],b[3];
    a[0]=x;a[2]=y;b[0]=x+1;b[2]=y+1;//ctverecek
    float hmin=10000,hmax=0;
    float h1=hiat(x,y),h2=hiat(x+1,y),h3=hiat(x+1,y+1),h4=hiat(x,y+1);
    if (verteces(x,y).waterhi>=0) hmin=hmax=verteces(x,y).waterhi;
    else {
      if (h1<hmin) hmin=h1;if (h2<hmin) hmin=h2;if (h3<hmin) hmin=h3;if (h3<hmin) hmin=h3;
    }
    if (h1>hmax) hmax=h1;if (h2>hmax) hmax=h2;if (h3>hmax) hmax=h3;if (h3>hmax) hmax=h3;
    a[1]=hmin;b[1]=hmax;
    gocttree->add(otItem(this,x+y*faces.w),a,b);
  }
*/
//  octalized=true;
}
/*
void HiMap::delize() {
  if (!octalized) return;
  gocttree->removegroup(this);
  octalized=false;
}
*/


bool HiMap::checkdxdy() {
  ZPoint za=verteces(0,0).countscrpt(),zb=verteces(verteces.w-1,0).countscrpt(),
         zc=verteces(verteces.w-1,verteces.h-1).countscrpt(),zd=verteces(0,verteces.h-1).countscrpt();
  fpoint a=fpoint(za.x,za.y),b=fpoint(zb.x,zb.y),c=fpoint(zc.x,zc.y),d=fpoint(zd.x,zd.y);
  fvector e=a-b,f=b-c,g=c-d,h=d-a;
//  e=e.getnormal();f=f.getnormal();g=g.getnormal();h=h.getnormal();
  fpline l1(e.getnormal(),a),l2(f.getnormal(),b),l3(g.getnormal(),c),l4(h.getnormal(),d);
  fpoint scr(0,0);
  fpoint mid((a+b+c+d)*0.25f);
  if (l1(scr)<0 || l2(scr)<0 || l3(scr)<0 || l4(scr)<0) {
    agline<float> k1(a,b),k2(b,c),k3(c,d),k4(d,a);
    float d1=k1.pdistfromline(scr);
    float d2=k2.pdistfromline(scr);
    float d3=k3.pdistfromline(scr);
    float d4=k4.pdistfromline(scr);
    float d=jmin(d1,d2,d3,d4);
    fvector t=mid-scr;
    t=t*(float)(d/t.fsize());
    game->view->maptype.dx+=t.x;
    game->view->maptype.dy-=t.y;
    return true;
  }
  return false;
}

/*
bool HiMap::checkdxdy() {
  float minx=0x100,maxx=-0x100,miny=0x100,maxy=-0x100;
  for(int i=0;i<verteces.size;i++) {
    if (verteces[i].scrpt.x<minx) minx=verteces[i].scrpt.x;
    if (verteces[i].scrpt.y<miny) miny=verteces[i].scrpt.y;
    if (verteces[i].scrpt.x>maxx) maxx=verteces[i].scrpt.x;
    if (verteces[i].scrpt.y>maxy) maxy=verteces[i].scrpt.y;
  }
  bool res=false;
  if (maxx<1) game->view->maptype.dx=1-maxx,res=true;
  if (maxy<1) game->view->maptype.dy+=1-maxy,res=true;
  if (minx>-1) game->view->maptype.dx+=1+minx,res=true;
  if (miny>-1) game->view->maptype.dy-=1+miny,res=true;
  return res;
}
*/

//inline void fcolor(float color) {glColor3f(0.3,color,0.3);}

void HiMap::clearvisibility() {

  for(int y=boundsqy1;y<=boundsqy2;y++) for(int x=boundsqx1;x<=boundsqx2;x++) {
    faces(x,y).octvisible=false;
  }

/*
  for(FaceStruct *act=dirtyf;act;act=act->nextdirty) {
    act->octvisible=false;
  }
  cleardirty(&dirtyf);

  for(VertexStruct *act=dirtyv;act;act=act->nextdirty) {
    act->counted=false;
  }
  cleardirty(&dirtyv);
*/

//  for(int i=0;i<faces.size;i++) faces[i].octvisible=false;
//  for(FaceStruct *act=faces.buf,*end=faces.buf+faces.size;act<end;act++) act->octvisible=false;
//  uncountvrs();
//  for(int i=0;i<verteces.size;i++) verteces[i].uncount();

  boundsqx1=faces.w;
  boundsqy1=faces.h;
  boundsqx2=-1;
  boundsqy2=-1;

  boundvx1=verteces.w;
  boundvy1=verteces.h;
  boundvx2=-1;
  boundvy2=-1;
}
void HiMap::ot_render(int id) {
  faces[id].octvisible=true;
//  insertdirty(&dirtyf,&faces[id]);
  int x=id%faces.w,y=id/faces.w;
  if (x<boundsqx1) boundsqx1=x;
  if (y<boundsqy1) boundsqy1=y;
  if (x>boundsqx2) boundsqx2=x;
  if (y>boundsqy2) boundsqy2=y;
}
void HiMap::repairbounds() {
  if (boundsqx1<0) boundsqx1=0;
  if (boundsqy1<0) boundsqy1=0;
  if (boundsqx2>=faces.w) boundsqx2=faces.w-1;
  if (boundsqy2>=faces.h) boundsqy2=faces.h-1;

  if (boundsqx1<boundvx1) boundvx1=boundsqx1;
  if (boundsqy1<boundvy1) boundvy1=boundsqy1;
  if (boundsqx2+1>boundvx2) boundvx2=boundsqx2+1;
  if (boundsqy2+1>boundvy2) boundvy2=boundsqy2+1;
}

/*
bool HiMap::valid(int x,int y,float mhd) {
  if (! hat(x,y)) return true;
  int h=hiat(x,y);
  #define zk(X,Y) if (final(X,Y) && ! validdist(h,X,Y,mhd)) return false
  zk(x+1,y);zk(x+1,y+1);zk(x,y+1);zk(x-1,y+1);zk(x-1,y);zk(x-1,y-1);zk(x,y-1);zk(x+1,y-1);
  return true;
  #undef zk
}
*/

void HiMap::clearfinal(HMBorders brd) {
  brd.repair(verteces.w,verteces.h);
  for(int y=brd.y1;y<=brd.y2;y++) for(int x=brd.x1;x<=brd.x2;x++) verteces(x,y).final=false;
}

void HiMap::repairpoint(int x,int y,float mhd) {
  HMBorders brd;
  if (HItem *i=hat(x,y)) {
    i->final=true;
    repair(x,y+1,mhd,brd);
  }
  clearfinal(brd);
}
void HiMap::repairrect(int rx,int ry,int rw,int rh,float mhd) {
  denyrepairwater++;
  HMBorders brd;
  brd.x1=rx;brd.y1=ry;brd.x2=rx+rw-1;brd.y2=ry+rh-1;
  for(int y=ry;y<ry+rh;y++) for(int x=rx;x<rx+rw;x++) if (HItem *i=hat(x,y)) i->final=true;
  repair(rx,ry+rh+1,mhd,brd);
  clearfinal(brd);
  denyrepairwater--;
  repairwater(brd);
}

bool HiMap::validate(int x,int y,float mhd) {
  if (!valid(x,y)) return false;

  HItem *it=hat(x,y);
  it->final=true;
  Interval interval;
  makeinterval(x,y,interval);

  #define zk(X,Y) if (HItem *a=hat(X,Y)) {\
    if (a->final) {\
      if (a->hi-it->hi>mhd) {\
        interval.lo=max(interval.lo,a->hi-mhd);\
      }\
      if (it->hi-a->hi>mhd) {\
        interval.hi=min(interval.hi,a->hi+mhd);\
      }\
    }\
  }
  zk(x+1,y);zk(x+1,y+1);zk(x,y+1);zk(x-1,y+1);zk(x-1,y);zk(x-1,y-1);zk(x,y-1);zk(x+1,y-1);
  #undef zk

  if (interval.isin(it->hi)) return false;
  if (it->blocked)
    throw HiBlocked(x,y);

  if (it->hi<interval.lo) it->sethi(interval.lo);
  if (it->hi>interval.hi) it->sethi(interval.hi);

  return true;
/*
  int h0=it.hi;

  for(int dhi=0;dhi<0x20;dhi++) {
    if (allowset(x,y,h0+dhi)) {
      it.hi=h0+dhi;
      if (valid(x,y,mhd)) return true;
    }
    if (allowset(x,y,h0-dhi)) {
      it.hi=h0-dhi;
      if (valid(x,y,mhd)) return true;
    }
  }
  throw HiBlocked(x,y);
*/
}

struct FloodFillItem {
  int xmax,xmin,y;
};
struct FloodFillStack:TCntVec<FloodFillItem> {
  void push(int xmin,int xmax,int y) {
    FloodFillItem &ffi=add();
    ffi.xmin=xmin;ffi.xmax=xmax;ffi.y=y;
  }
  void pop(int &xmin,int &xmax,int &y) {
    FloodFillItem &ffi=last();
    xmin=ffi.xmin;xmax=ffi.xmax;y=ffi.y;
    eraselast();
  }
};

bool FloodFiller::operator () (int x,int y) {
  if (x<0 || x>=hm->verteces.w || y<0 || y>=hm->verteces.h) return false;
  VertexStruct &v=hm->verteces(x,y);
//  if (v.floodfilled) return false;
//  if (fillborder) fill(v);
  return !isborder(v);
}

bool FloodFiller::floodfilled(int x,int y) {
  if (x<0 || x>=hm->verteces.w || y<0 || y>=hm->verteces.h) return true;
  VertexStruct &v=hm->verteces(x,y);
  return v.floodfilled;
}

/*
bool FloodFiller::ncond(int x,int y) {
  if (x<0 || x>=hm->verteces.w || y<0 || y>=hm->verteces.h) return true;
  VertexStruct &v=hm->verteces(x,y);
//  if (v.floodfilled) return true;
//  if (fillborder) fill(v);
  return isborder(v);
}
*/

void HiMap::ff_search(FloodFillStack &stack,FloodFiller &fw,int xmin,int xmax,int y) {
//  if (!fw(xmin,y)) return;
  while (!fw(xmin,y) && xmin<verteces.w) xmin++;
  while (!fw(xmax,y) && xmax>=0) xmax--;
//  if (xmax<xmin) return;
  while (fw(xmin-1,y)) xmin--;
  while (xmin<=xmax) {
//  do {
    int xm=xmin;
    while (fw(xm+1,y)) xm++;
//    if (!fw.floodfilled(xmin,xm))
    stack.push(xmin,xm,y);
    xmin=xm+2;
    while (xmin<=xmax && !fw(xmin,y)) xmin++;
  }
//  while (xmin<=xmax);
}

/*
static void recfloodfill(int x,int y,FloodFiller &ff,HMBorders &brd) {
  if (ff.hm->verteces.valid(x,y) && ff(x,y)) {
    brd.extend(x,y);
    VertexStruct &v=ff.hm->verteces(x,y);
    ff.fill(v);
    v.floodfilled=true;
    recfloodfill(x+1,y,ff,brd);
    recfloodfill(x-1,y,ff,brd);
    recfloodfill(x,y+1,ff,brd);
    recfloodfill(x,y-1,ff,brd);
  }
}
*/

void HiMap::floodfill(int vx,int vy,FloodFiller &fw,bool fillborder,HMBorders &brd) {
  FloodFillStack stack;
  fw.hm=this;
  fw.fillborder=fillborder;
//  for(int i=0;i<verteces.size;i++) verteces[i].floodfilled=false;

//  recfloodfill(vx,vy,fw,brd);

  ff_search(stack,fw,vx,vx,vy);
  while (stack.count>0) {
    int xmin,xmax,y;
    stack.pop(xmin,xmax,y);
    if (!fw.floodfilled(xmin,y)) {
      brd.extend(xmin,y);
      brd.extend(xmax,y);
      for(int xi=xmin;xi<=xmax;xi++) {
        fw.fill(verteces(xi,y));
        verteces(xi,y).floodfilled=1;
      }
      ff_search(stack,fw,xmin,xmax,y-1);
      ff_search(stack,fw,xmin,xmax,y+1);
    }
  }

  brd.repair(verteces.w,verteces.h);

  if (fillborder) {
    for(int y=brd.y1;y<=brd.y2;y++) for(int x=brd.x1;x<=brd.x2;x++) {
      if (verteces(x,y).floodfilled&1) {
        if (VertexStruct *n=verteces.tryat(x+1,y)) n->floodfilled|=2;
        if (VertexStruct *n=verteces.tryat(x-1,y)) n->floodfilled|=2;
        if (VertexStruct *n=verteces.tryat(x,y+1)) n->floodfilled|=2;
        if (VertexStruct *n=verteces.tryat(x,y-1)) n->floodfilled|=2;
      }
    }

    brd.x1--;brd.y1--;brd.x2++;brd.y2++;
    brd.repair(verteces.w,verteces.h);

    for(int y=brd.y1;y<=brd.y2;y++) for(int x=brd.x1;x<=brd.x2;x++) {
      if (verteces(x,y).floodfilled) fw.fill(verteces(x,y));
    }
  }

  for(int y=brd.y1;y<=brd.y2;y++) for(int x=brd.x1;x<=brd.x2;x++) verteces(x,y).floodfilled=0;
}

struct WaterFF:FloodFiller {
  float waterhi;
  bool isborder(VertexStruct &v) {return v.hi>waterhi;}
  void fill(VertexStruct &v) {v.waterhi=waterhi;}
};

void HiMap::fillwater(int x,int y,float waterhi) {
  HMBorders brd;
  if (verteces(x,y).waterhi>waterhi) return;
  WaterFF wf;
  wf.waterhi=waterhi;
  floodfill(x,y,wf,false,brd);
  changedwater(brd);
//  modifyall();
//  delize();
}

struct DryWaterFF:FloodFiller {
  float waterhi;
  bool isborder(VertexStruct &v) {return fabs(v.waterhi-waterhi)>jtyc::epsilon;}
  void fill(VertexStruct &v) {v.waterhi=-1;}
};

void HiMap::drywater(int x,int y) {
  HMBorders brd;
  DryWaterFF wf;
  wf.waterhi=verteces(x,y).waterhi;
  if (wf.waterhi<0) return;
  floodfill(x,y,wf,false,brd);
  changedwater(brd);
//  modifyall();
//  delize();
}

void HiMap::dryall() {
  for(int i=0;i<verteces.size;i++) verteces[i].waterhi=-1;
  modifyall();
//  changed();
//  delize();
}

void HiMap::countbroken() {
/*
  FaceStruct *actf=broken_list;
  while (actf) {
    VertexStruct &a=vertexat(actf->x,actf->y),&b=vertexat(actf->x+1,actf->y);
    VertexStruct &c=vertexat(actf->x+1,actf->y+1),&d=vertexat(actf->x,actf->y+1);
    a.tmpbool=false;b.tmpbool=false;c.tmpbool=false;d.tmpbool=false;
    a.tmpnext=NULL;b.tmpnext=NULL;c.tmpnext=NULL;d.tmpnext=NULL;
    actf=actf->next_broken;
  }
  actf=broken_list;
  VertexStruct *vl=NULL;
  while (actf) {
    VertexStruct &a=vertexat(actf->x,actf->y),&b=vertexat(actf->x+1,actf->y);
    VertexStruct &c=vertexat(actf->x+1,actf->y+1),&d=vertexat(actf->x,actf->y+1);
    if (!a.tmpbool) a.tmpnext=vl,vl=&a,a.tmpbool=true;
    if (!b.tmpbool) b.tmpnext=vl,vl=&b,b.tmpbool=true;
    if (!c.tmpbool) c.tmpnext=vl,vl=&c,c.tmpbool=true;
    if (!d.tmpbool) d.tmpnext=vl,vl=&d,d.tmpbool=true;
    actf=actf->next_broken;
  }

  actf=broken_list;
  while (actf) {
    fpoint3d a(get(actf->x+1,actf->y+1)),b(get(actf->x,actf->y+1)),c(get(actf->x+1,actf->y));
    fvector3d u=a-b,v=a-c;
    fvector3d normal=u%v;//normala
    actf->n=normal.size1();
    actf=actf->next_broken;
  }

  VertexStruct *actv=vl;
  int w=verteces.w,h=verteces.h;
  while (actv) {
    int x=actv->x,y=actv->y;
    if (x==0) {
      if (y==0) actv->n=faceat(0,0).n;
      else if (y==h-1) actv->n=faceat(0,h-2).n;
      else actv->n=((fvector3d)faceat(0,y-1).n+(fvector3d)faceat(0,y).n)/2;
    } else if (x==w-1) {
      if (y==0) actv->n=faceat(w-2,0).n;
      else if (y==h-1) actv->n=faceat(w-2,h-2).n;
      else actv->n=((fvector3d)faceat(w-2,y-1).n+(fvector3d)faceat(w-2,y).n)/2;
    } else if (y==0) {
      actv->n=((fvector3d)faceat(x-1,0).n+(fvector3d)faceat(x,0).n)/2;
    } else if (y==h-1) {
      actv->n=((fvector3d)faceat(x-1,h-2).n+(fvector3d)faceat(x,h-2).n)/2;
    } else {
      fvector3d v1=faceat(x-1,y-1).n,
                 v2=faceat(x,y-1).n,
                 v3=faceat(x-1,y).n,
                 v4=faceat(x,y).n;
      actv->n=(v1+v2+v3+v4)*0.25;
    }
    actv=actv->tmpnext;
  }
*/
//  actf=broken_list;
  FaceStruct *actf=broken_list;
  while (actf) {
    int x=actf->x,y=actf->y;
    float a[3],b[3];
    a[0]=x;a[2]=y;b[0]=x+1;b[2]=y+1;//ctverecek
    float hmin=10000,hmax=0;
    float h1=hiat(x,y),h2=hiat(x+1,y),h3=hiat(x+1,y+1),h4=hiat(x,y+1);
    if (verteces(x,y).waterhi>=0) hmin=hmax=verteces(x,y).waterhi;
    else {
      if (h1<hmin) hmin=h1;if (h2<hmin) hmin=h2;if (h3<hmin) hmin=h3;if (h3<hmin) hmin=h3;
    }
    if (h1>hmax) hmax=h1;if (h2>hmax) hmax=h2;if (h3>hmax) hmax=h3;if (h3>hmax) hmax=h3;
    a[1]=hmin;b[1]=hmax;
    actf->octalized=gocttree->add(otItem(this,x+y*faces.w),a,b);
    actf=actf->next_broken;
  }

  mac_clear_dirty(FaceStruct,&broken_list,next_broken,broken)

}

void HiMap::changedwater(HMBorders brd) {
  brd.repair(verteces.w,verteces.h);
  for(int y=brd.y1;y<=brd.y2;y++) for(int x=brd.x1;x<=brd.x2;x++) verteces(x,y).changedwater();
  modify(brd.x1,brd.y1,brd.x2-1,brd.y2-1);
//  for(int i=0;i<verteces.size;i++) verteces[i].changedwater();
//  for(int i=0;i<verteces.size;i++) verteces[i].changedwater();
//  modifyall();
//  delize();
}

float HiMap::getfreehi(int x,int y,int w,int h,bool *test) {
  Interval ibig;
  float sum=0;
  for(int j=0;j<=h;j++) for(int i=0;i<=w;i++) {
    if (test && !test[(w+1)*j+i]) continue;
    Interval i0;
    game->himap->makeinterval(i+x,j+y,i0);
    sum+=game->himap->hiat(i+x,j+y);
    ibig=ibig*i0;
  }
  float res=sum/((w+1)*(h+1));
  if (ibig.lo>ibig.hi) return -1;
  if (res<ibig.lo) res=ibig.lo;
  if (res>ibig.hi) res=ibig.hi;
  return res;
}

void HiMap::repair(int x,int y,float mhd,HMBorders &brd) {
  int minx=verteces.w,miny=verteces.h,maxx=-1,maxy=-1;
  int dxs[]={1,0,-1,0};
  int dys[]={0,-1,0,1};
  int dir=0,nextdir=1;
  int dnc=0;//kolikrat uz se zmenil smer, aniz by neco bylo zmeneno
  while (dnc<=4) {
    brd.extend(x,y);
    if (validate(x,y,mhd)) dnc=0;
    int nx=x+dxs[nextdir],ny=y+dys[nextdir];
    if (!valid(nx,ny) || !vertexat(nx,ny).final) dir=nextdir,nextdir++,nextdir%=4,dnc++;
    x+=dxs[dir];y+=dys[dir];
  }
  modify(brd.x1,brd.y1,brd.x2,brd.y2);
//  delize();
/*  MEMSTROUT sp;
  for(int y=0;y<h;y++) {
    for(int x=0;x<w;x++) {char p[0x10];sprintf(p,"%4d",gethi(x,y));sp<<p;}
    sp<<"\n";
  }
  savetextfile(basedir/"mapa.txt",sp.str().c_str());*/
}

}

