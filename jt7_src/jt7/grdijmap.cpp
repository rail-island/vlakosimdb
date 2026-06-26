#include <jlib.h>
#pragma hdrstop

#include "grdijmap.h"
#include "rdijskr.h"

namespace jtyc {

const int min_steps=4;

static void pocitej_vrcholy(void *sum,RItem *ri) {
  ri->info.begin=*((int*)sum);
  *((int*)sum)+=ri->info.vrcnt;
}

static void spojuj_vrcholy(void *m,RItem *ri) {
  DijRMap *map=(DijRMap*)m;
  for(int i=0;i<ri->info.vrcnt;i++) {
    RRVRCH sousedi[8];rdist_t dist[8];Spojnice tts[8];
    RRVRCH thisvr=RRVRCH(ri,ri->info[i]);
    RRVRCH beside=thisvr.beside();
    int cnt=beside.neightboors(sousedi,dist,tts);
    DijRVr &v=map->items[map->actvr++];
    v.vr=thisvr;v.souscnt=0;
    v.beside=beside;
    for(int j=0;j<cnt;j++) {
      RRVRCH s=sousedi[j];
      {
        v.sousedi[v.souscnt].len=dist[j];
        v.sousedi[v.souscnt].vr=s.dijvr();
        v.sousedi[v.souscnt].tt=tts[j];
        v.souscnt++;
      }
    }
  }
}

void DijRMap::update() {
  int vrcnt=0;
  map->foreach(pocitej_vrcholy,&vrcnt);
  items.clear();
  items.wantcapa(vrcnt);
  items.count=vrcnt;
  for(int i=0;i<items.count;i++) items[i].construct();
  actvr=0;
  map->foreach(spojuj_vrcholy,this);
  modified=false;
}

void DijRMap::preparedijskr() {
  if (modified) update();
  for(int i=0;i<items.count;i++) items[i].initdij();
  active.clear();active.wantcapa(items.count);
  final.clear();final.wantcapa(items.count);
}

void DijRMap::addactive(DijRVr *v,double value0) {
  active.add(v);v->active=true;v->value=value0;
}

rdist_t DijRVr::countdist(int idx) {
  rdist_t res=sousedi[idx].len;
  RItem *ri=beside.ri;
  res+=ri->dijskr_penalty(sousedi[idx].tt);
  return res;
}

void DijRMap::dijskr(DijCallBack &dcb) {
//  eclear();
//  int maxact=0;
//  int cnt=0;
//  int cl0=clock();

  while (active.count>0 && ! dcb.isend()) {
    int minidx=-1;double minvalue;
    for(int i=0;i<active.count;i++) if (minidx<0 || active[i]->value<minvalue) minvalue=active[i]->value,minidx=i;
    DijRVr *pactv=active[minidx],&actv=*pactv;
//    actv.vr.eprint();
    dcb.reached(pactv);final.add(pactv);
    actv.final=true;
    active.erase(minidx);

//    maxact=max(active.count,maxact);
//    cnt++;

    for(int i=0;i<actv.souscnt;i++) {
      double newvalue=actv.value+actv.countdist(i);
      DijRVr::Soused &soused=actv.sousedi[i];
      if (!soused.vr->active) active.add(soused.vr),soused.vr->active=true;
      if (newvalue<soused.vr->value) {
        soused.vr->value=newvalue;
        soused.vr->steps=actv.steps+1;
        soused.vr->src=pactv;
      }
    }
  }

//  int cl=clock()-cl0;
//  maxact=0;
}

int DijRMap::freecango(DijRVr *end,rdist_t trainlen) {
//  DijRVr *end=dest->getend();
  if (!end) return 0;
  TCntVec<DijRVr*> vrs;
  DijRVr *act=end;
  while (act) {
    vrs.add(act);
    act=act->src;
  }
  vrs.reverse();
  int fsi=-1;//index prvniho semaforu
  for(int i=1;i<vrs.count;i++) {
    DijRVr *prev=vrs[i-1],*act=vrs[i];

    if (act->vr.ri->hassem()) {
      if (!act->vr.ri->isfree(prev->vr.v.direct())) {
        return 0;//prvni semafor cerveny
      } else {
        fsi=i;
        break;
      }
    }
  }
  if (fsi<0) return 2;//cesta volna az do konce
  rdist_t actlen=0;
  for(int i=fsi+1;i<vrs.count && actlen<trainlen;i++) {
    DijRVr *prev=vrs[i-1],*act=vrs[i];
    Spojnice tt=Spojnice(prev->vr.v.direct(),act->vr.v);

    if (act->vr.ri->hassem()) {
      if (!act->vr.ri->isfree(prev->vr.v.direct())) {
        return 1;
      }
    }
    actlen+=tt.len();
  }

  return 2;
}


void DijRMap::_findway(DijRVr *end,IObjArray<WayItem> &way,TempPenaltyItems &tmpitems,WayInfo *info) {
//  int cl0=clock();

  int cnt=way.count;

//  DijRVr *end=dest->getend();
  if (!end) return;

  TCntVec<DijRVr*> vrs;

  DijRVr *act=end;
  while (act) {
    vrs.add(act);
    act=act->src;
  }

  vrs.reverse();

//  for(WayItem **beg=way.buf+cnt,**end=way.buf+way.count-1;beg<end;beg++,end--) exchange(*beg,*end);
  //ted jeste musime trasu urezat, aby nezustala nejdednoznacna cast
  //delka nove trasy je aspon 1 (abychom neco nasli)
  //pokud nejblizsi semafor je obousmerny, cnt muzeme zvysit za nej

  int addcnt=1;
  if (cnt>0) addcnt=0;
  //addcnt-kolik items se ma pridat do way, muze se jedine zvetsovat

  //pokud cesta obsahuje prave jeden obousmerny semafor, najde se az do konce,
  //za obousmernym semaforem se penalizuje

  int sem2idx=-1;
  for(int i=0;i<vrs.count;i++) {
//    if (!vrs[i]->beside.valid()) break;
//    if (vrs[i]->beside.ri->hassem()) {//ma semafor
    if (!vrs[i]->vr.valid()) break;
    if (vrs[i]->vr.ri->hassem()) {//ma semafor
      if (!vrs[i]->vr.ri->dijskr_hassem2()) {//semafor je jednosmerny
        sem2idx=-1;
        break;
      }
      if (sem2idx>=0) {//vice obousmernych semaforu
        sem2idx=-1;
        break;
      }
      sem2idx=i;
    }
  }

  if (sem2idx<0) {//typicka varianta, neni pravda, ze trasa obsahuje prave 1 semafor, ktery je obousmerny
   //oreze se do prvni krizovatky
    while(addcnt<vrs.count-1) {
      if (vrs[addcnt]->souscnt>1) break;
      addcnt++;
    }
  } else {
    addcnt=vrs.count;
  }

  for(int i=1;i<=addcnt && i<vrs.count;i++) {
    DijRVr *prev=vrs[i-1],*act=vrs[i];
    if (prev) {
      RRSPOJ tt=RRSPOJ(act->vr.ri,Spojnice(prev->vr.v.direct(),act->vr.v));
      xassert(tt.valid());
      way.add(new WayItem(tt,act==end));
    }
  }

  tmpitems.clear();
  if (sem2idx>=0) {//penalizace koleje, na kterou se jede
    for(int i=sem2idx+1;i<vrs.count && i<=addcnt;i++) tmpitems.add(vrs[i-1]->vr.ri);
  }


/*
  bool isfirstsem2=false;//zda prvni semafor je a je obousmerny
  int firstsem2idx=-1;

  for(int i=addcnt+1;i<vrs.count;i++) {
    if (!vrs[i]->beside.valid()) break;
    if (vrs[i]->beside.ri->hassem()) {//ma semafor
      if (vrs[i]->beside.ri->dijskr_hassem2()) {//prvni sefafor je obousmerny
        addcnt=i;//ne -1, protoze se necha za semafor
        firstsem2idx=i;
        isfirstsem2=true;
      }
      break;
    }
  }

  while(addcnt<vrs.count-1) {
    if (vrs[addcnt]->souscnt>1) {
      int xxx=0;
      break;
    }
    addcnt++;
  }

  for(int i=1;i<=addcnt && i<vrs.count;i++) {
    DijRVr *prev=vrs[i-1],*act=vrs[i];
    if (prev) {
      RRSPOJ tt=RRSPOJ(act->vr.ri,Spojnice(prev->vr.v.direct(),act->vr.v));
      xassert(tt.valid());
      way.add(new WayItem(tt,act==end));
    }
  }

  //ted se penalizuje pro ostatni vlaky vsechny RItems od prvniho obouhsmerneho semaforu
  //pokud je obousmerny do dalsi krizovatky nebo semaforu
  tmpitems.clear();
  if (isfirstsem2) {
    for(int i=firstsem2idx+1;i<vrs.count && i<=addcnt;i++) {
//      if (!vrs[i]->beside.valid()) break;
//      if (vrs[i]->beside.ri->hassem() || vrs[i]->souscnt>1) break;
//      tmpitems.add(vrs[i]->beside.ri);
      if (vrs[i-1]->vr.ri->hassem() || vrs[i]->souscnt>1) break;
      tmpitems.add(vrs[i-1]->vr.ri);
    }
  }
//  tmpitems.clear();

*/

  //zapiseme informaci o najite ceste
  if (info) {
    rdist_t len=0;
    for(int i=0;i<cnt;i++) len+=way[i]->tt.tt.len();
    for(int i=1;i<vrs.count;i++) {
      len+=Spojnice(vrs[i-1]->vr.v.direct(),vrs[i]->vr.v).len();
    }
    info->len=len;
  }

//  int cl=clock()-cl0;

}


/*
struct Vetev {
  DijRVr *vr,*altnext;
  int index;
  Set32 up;//na ktere vrcholy se da dostat
  void init(DijRVr *avr,int aindex,DijRVr *alternativenext) {
    vr=avr;index=aindex;up.clear();altnext=alternativenext;
  }
};

//int v_compare(Vetev &a,Vetev &b) {
//  return a.vr->steps-b.vr->steps;
//}

static bool getvr(RDestination *dest,DijRVr *next,bool mustaccept,double &dist) {
  bool res=false;
  for(int i=0;i<32;i++) if (next->targets[i] && ((! mustaccept) || dest->accept(i))) {
    if (res==false || next->dist<dist) dist=next->dist;
    res=true;
  }
  return res;
}

static DijRVr *optimal_next(RDestination *dest,DijRVr *src,bool mustaccept) {
  int minidx=-1;double mindist;
  for(int i=0;i<src->nextcnt;i++) {
    double actdist=0;
    if (getvr(dest,src->next[i],mustaccept,actdist)) {
      if (minidx<0 || actdist<mindist) mindist=actdist,minidx=i;
    }
  }
  return minidx>=0?src->next[minidx]:NULL;
}

static DijRVr *optimal_next(RDestination *dest,DijRVr *src) {
  if (DijRVr *res=optimal_next(dest,src,true)) return res;
  return optimal_next(dest,src,false);
};

void DijRMap::_findway(RDestination *dest,IObjArray<WayItem> &way) {
//  TCntVec<DijRVr*> ends;
//  TCntVec<Vetev> vetve;
//  for(int i=0;i<dest->items.count;i++) if (dest->items[i].reached) ends.add(dest->items[i].reached);
  DijRVr *begin=NULL;//zacatek cesty
  for(int i=0;i<dest->items.count;i++) if (DijRVr *actend=dest->items[i].reached) {
//    int idx=vetve.count;
    double dist=0;
//    vetve.add().init(ends[i],idx,NULL);
    int lastwaycnt=0;Set32 lasttargets;
    DijRVr *v=actend,*next=NULL;
    while (v) {
      v->dist=dist;
      v->targets+=lasttargets;
      if (!begin || v->steps<begin->steps) begin=v;
      if (v->waycnt==0 && next) v->addnext(next);
      if (v->waycnt>lastwaycnt) {
        v->addnext(next);
//        int idx=vetve.count;
//        vetve.add().init(v,idx,next);
      }
      lastwaycnt=v->waycnt;
      v->waycnt++;
      next=v;lasttargets=v->targets;
      if (v->src) dist+=v->value-v->src->value;
      v=v->src;
    }
  }
//  heapsort(vetve.buf,vetve.count,v_compare);
  //vlastni vyrobeni cesty
  //ted mame stromecek (zadany mnozinou rozvetveni) vetve
  xassert(begin);
  DijRVr *v=begin;
  while (v) {
    DijRVr *next;
    if (v->nextcnt>1) {//je potreba se rozhodnout
      if (v->steps>min_steps) {//cesta dostatecne dlouha, nemusime se rozhodovat
        next=NULL;
      } else next=optimal_next(dest,v);
    } else if (v->nextcnt==1) next=v->next[0];
    else next=NULL;
    if (next) {
      RRSPOJ tt=RRSPOJ(next->vr.ri,Spojnice(v->vr.v.direct(),next->vr.v));
      xassert(tt.valid());
      way.add(new WayItem(tt,next->nextcnt==0));
    }
    v=next;
  }
}
*/

/*
bool RDestCallBack::isend() {
  return dest->isend();
}
void RDestCallBack::reached(DijRVr *vr) {
  dest->reach(vr->vr,vr);
}
*/


}

//#pragma package(smart_init)
