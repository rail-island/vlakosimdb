#include <jlib.h>
#pragma hdrstop

#include "gsemaphore.h"
#include "gameu.h"
#include "grrmap.h"
#include "gview.h"
#include "gameu.h"
#include "gimages.h"

namespace jtyc {

inline void unvisit(void*,RItem *ri) {ri->visited=false;}

void Semaphore::notifyremove(GInstance *obj) {
  if (obj==ri) ri=NULL;
  GObject::notifyremove(obj);
}

void Semaphore::buildnext() {
  switch (dir) {
    case 0:dir=1;break;
    case 1:dir=-1;break;
    case -1:dir=0;break;
  }
//  ri->dir=dir;
}

void Semaphore::settype(jtSemaphore *atype) {
  type=atype;
  gradsum=type->gradtime;
}

void Semaphore::run(double deltatime) {
  pgrad-=deltatime;
  ngrad-=deltatime;
  if (pgrad<0) pgrad=0;
  if (ngrad<0) ngrad=0;
}

Semaphore::Semaphore(RItem *ari,Spojnice att,int adir,jtSemaphore *atype) {
  ri=ari;tt=att;dir=adir;pfree=nfree=true;
//  ri->dir=adir;
  blocked=false;
  gradt=0;lastgradtime=0;
  type=atype;
  gradsum=type->gradtime;
  pgrad=ngrad=0;
}
/*void Semaphore::save(BINWRITE &fw) {
  GObject::save(fw);fw<<dir<<pfree<<nfree<<ri<<tt.data;
}
Semaphore::Semaphore(BINREAD &fr):GObject(fr) {
  fr>>dir>>pfree>>nfree>>ri>>tt.data;blocked=false;
}*/
Semaphore::Semaphore(RItem *ari,XMLNode xml) {
  ri=ari;dir=xml.ivar("dir");
  tt=ri->spojnice();
//  pfree=nfree=true;
//  blocked=false;
  pfree=xml.ivar("pfree");
  nfree=xml.ivar("nfree");
  blocked=xml.ivar("blocked");
  type=gimages->findsemaphore(xml.svar("type"));
  pgrad=ngrad=0;
  gradsum=type->gradtime;
}

rdist_t Semaphore::dijskr_penalty(Spojnice tt) {
  rdist_t res=0;
  if (dir==0 && !isfree(tt.beg()))
    res+=beop.semaphore_dir_0_penalty;
  return res;
}

void Semaphore::xmlsave(XMLNode xml) {
  xml.setname("semaphore");
  xml.setvar("dir",dir);
  xml.setvar("pfree",pfree);
  xml.setvar("nfree",nfree);
  xml.setvar("blocked",blocked);
  xml.setvar("type",type->name);
}

/*
void Semaphore::_redraw(View *v,bool isfree,fpoint pos,fvector diff,ZBitmap *bmp) {
  Voxels *sem=isfree?game->images->greensem:game->images->redsem;
  ZBMPPT pt=v->ltop(point3d<float>(pos,ri->za+ri->dzx/2.0+ri->dzy/2.0));
  matrix3d<float> rot;
  rot.dorotatez(-v->viewangle+diff.fangle());
  sem->blittozbmp(bmp,pt.x,pt.y,pt.z,rot,v->prj.ky,v->prj.kz);
}

void Semaphore::redraw(View *v,ZBitmap *bmp) {
  const double semdist=0.4;
  fpoint pos=tt.pos(tt.len()/2)+fpoint(ri->x,ri->y);
  fvector diff=tt.diff(tt.len()/2);
  fvector dpos=diff.getnormal().multiply(semdist,semdist);
  if (dir>=0) _redraw(v,pfree,pos+dpos,diff,bmp);
  if (dir<=0) _redraw(v,nfree,pos-dpos,diff,bmp);
}

void Semaphore::redrawstatic(View *v,ZBitmap *bmp) {
}
*/

bool sem_isinverse(Spojnice tt) {
  if (tt.data==tt_24) return true;
  if (tt.data==tt_34) return true;
  if (tt.data==tt_bd) return true;
  if (tt.data==tt_14) return true;
  if (tt.data==tt_bc) return true;
  if (tt.data==tt_2d) return true;
  if (tt.data==tt_2a) return true;
  if (tt.data==tt_3a) return true;
  if (tt.data==tt_1d) return true;
  return false;
}

void Semaphore::_render(bool isfree,fpoint pos,fvector diff,bool positivedir,float gradrest,RenderContext &rc) {
  gloVariableList vars;
  if (gradrest>0) vars.setgradvar("state",isfree?"stop":"free",isfree?"free":"stop",(gradsum-gradrest)/gradsum);
  else vars.setvar("state",isfree?"free":"stop");
  gloVariableList *lastvars=rc.vars;
  rc.vars=&vars;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(pos.x,ri->za+ri->dzx/2.0+ri->dzy/2.0,pos.y);
  int rot=positivedir?1:0;

  if (sem_isinverse(tt)) rot=!rot;

  glRotatef(90-diff.fangle()/M_PI*180+rot*180,0,1,0);
  rc.push();
  rc.settexture2d(false);
  rc.setblending(false);
  rc.setlighting(true);
  type->model->render(rc);
  rc.pop();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  rc.vars=lastvars;
}

void Semaphore::render(RenderContext &rc) {
  const double semdist=0.4;
  fpoint pos=tt.pos(tt.len()/2)+fpoint(ri->x,ri->y);
  fvector diff=tt.diff(tt.len()/2);
  fvector dpos=diff.getnormal().multiply(semdist,semdist);
  if (beop.right_sided_semaphores) dpos=-dpos;
  if (dir>=0) _render(pfree,pos+dpos,diff,true,pgrad,rc);
  if (dir<=0) _render(nfree,pos-dpos,diff,false,ngrad,rc);
}

static bool has_begin(int flags,Vrchol v) {
  for(int i=1;i<=tt_cnt;i++) {
    Spojnice tt(i);
    if (tt.flag()&flags) {
      if (tt.beg()==v) return true;
      if (tt.end()==v) return true;
    }
  }
  return false;
}

static RItem *valid_soused(RItem *ri,Vrchol v) {//zjisti, zda ri ma souseda, na ktereho se
//dostane z vrcholu v a vrati ho
  RItem *res=game->rrmap->first_at(ri->x+v.dx(),ri->y+v.dy());
  if (!res) return NULL;
  Vrchol direct=v.direct();float zout=ri->getz(v);
  while (res) {
    if (!res->visited && fabs(res->getz(direct)-zout)<epsilon && has_begin(res->flags,direct)) return res;
    res=(RItem*)res->next;
  }
  return NULL;
}

inline void add_soused(RItem *soused,TCntVec<RItem*> &res,TCntVec<Sem_Rec> &sems,Vrchol in,bool &isfree) {
  if (soused->hassem()) {
    Sem_Rec &sr=sems.add();
    sr.ri=soused;sr.in=in;
    sr.blocked=false;
    if (soused->isblocked()) isfree=false,sr.blocked=true;
  } else res.add(soused);
}

void sem_neightboors(RItem *ri,TCntVec<RItem*> &res,TCntVec<Sem_Rec> &sems,bool &isfree) {
  bool vr_tested[vr_cnt+1];
  for(int i=0;i<=vr_cnt;i++) vr_tested[i]=false;
  for(int i=1;i<=tt_cnt;i++) {
    Spojnice tt(i);
    if (tt.flag()&ri->flags) {
      Vrchol beg=tt.beg();
      if (! vr_tested[beg.data]) {
        vr_tested[beg.data]=true;
        if (RItem *soused=valid_soused(ri,beg)) add_soused(soused,res,sems,beg.direct(),isfree);
      }
      Vrchol end=tt.end();
      if (! vr_tested[end.data]) {
        vr_tested[end.data]=true;
        if (RItem *soused=valid_soused(ri,end)) add_soused(soused,res,sems,end.direct(),isfree);
      }
    }
  }
}

void Semaphore::setfree(bool isfree,Vrchol v) {
  if (v==tt.beg()) {
    if (pfree!=isfree) pgrad=gradsum;
    pfree=isfree;
  }
  if (v==tt.end()) {
    if (nfree!=isfree) ngrad=gradsum;
    nfree=isfree;
  }
}
bool Semaphore::isfree(Vrchol v) {
  if (v==tt.beg()) return nfree;//opacne nez setfree
  if (v==tt.end()) return pfree;
  return false;
//  throw JError("Semaphore::Bad vr");
//  return false;
}
static void doupdate(TCntVec<RItem*> &active,TCntVec<Sem_Rec> &sems,bool &isfree) {
  while (active.count>0) {
    RItem *act=active.last();active.eraselast();
    act->visited=true;if (act->istrain()) isfree=false;
    sem_neightboors(act,active,sems,isfree);
  }
  for(int i=0;i<sems.count;i++) {
    Sem_Rec &sr=sems[i];
    if (!sr.blocked) sr.ri->setfree(isfree,sr.in);
    else sr.ri->setfree(false,sr.in);
  }
}
static void updateside(RRVRCH start) {
  bool isfree=true;
  game->rrmap->foreach(unvisit,NULL);
  if (RItem *active0=valid_soused(start.ri,start.v)) {
    TCntVec<RItem*> active;TCntVec<Sem_Rec> sems;
    add_soused(active0,active,sems,start.v.direct(),isfree);
    doupdate(active,sems,isfree);
  }
}

static void get_neightboors_side_cycle(TCntVec<RItem*> &active,TCntVec<Sem_Rec> &sems,bool &isfree) {
  while (active.count>0) {
    RItem *act=active.last();active.eraselast();
    act->visited=true;
    sem_neightboors(act,active,sems,isfree);
  }
}
static void get_neightboors_side(TCntVec<Sem_Rec> &res,RRVRCH start) {
  bool isfree=true;
  game->rrmap->foreach(unvisit,NULL);
  if (RItem *active0=valid_soused(start.ri,start.v)) {
    TCntVec<RItem*> active;
    add_soused(active0,active,res,start.v.direct(),isfree);
    get_neightboors_side_cycle(active,res,isfree);
  }
}
void Semaphore::updatesems(RItem *ri) {
  game->rrmap->foreach(unvisit,NULL);
  bool isfree=true;
  TCntVec<RItem*> active;TCntVec<Sem_Rec> sems;
  active.add(ri);
  doupdate(active,sems,isfree);
}

void Semaphore::get_neightboors(TCntVec<Sem_Rec> &res,Vrchol v) {
  RRSPOJ rr(ri,tt);
  RRVRCH beg=rr.beg(),end=rr.end();
  if (end.v==v) get_neightboors_side(res,end);
  if (beg.v==v) get_neightboors_side(res,beg);
}

void Semaphore::update() {
  RRSPOJ rr(ri,tt);
  RRVRCH beg=rr.beg(),end=rr.end();
  pfree=true;nfree=true;
  if (beg.valid()) updateside(beg);
  if (end.valid()) updateside(end);
}

bool free_sem_square(RItem *testri) {
  game->rrmap->foreach(unvisit,NULL);
  TCntVec<RItem*> active;
  active.add(testri);
  int act=0;
  while (act<active.count) {
    RItem *ri=active[act++];
    if (ri->istrain()) return false;
    ri->visited=true;
    if (!ri->hassem()) {
      for(int i=1;i<=tt_cnt;i++) {
        Spojnice tt(i);
        if (ri->flags & tt.flag()) {
          RRVRCH v1=RRVRCH(ri,tt.beg()).beside(),v2=RRVRCH(ri,tt.end()).beside();
          if (v1.ri && !v1.ri->visited) active.add(v1.ri);
          if (v2.ri && !v2.ri->visited) active.add(v2.ri);
        }
      }
    }
  }
  return true;
}


}

