#include <cblib.h>
#pragma hdrstop

#include "divtextcnv.h"
#include <com/sorting.cpp>
#include "pager.cpp"
#include <gr/comgr.h>
#include <gr/subcnv.h>

const int safeconst=0;

namespace gr {
using namespace jlib;

//interfaceCPP(LogPages) {
//}
bool STDCALL LOGPAGE::isleft() {
  return parent->isleftpage(parent->pages.search(this));
}

inline bool LOGPAGES::isleftpage(int index) {
  testcl;
  return ((index+(firstleft?0:1))&1)==0;
}
inline bool LOGPAGE::canaddpane(PANE &pane) {
  testcl;
  return hi+pane.hi<=maxhi;
}
inline bool PANE::first() {
  testcl;testptr(parent);
  return this==parent->panes[0];
}
inline bool PANE::last() {
  testcl;testptr(parent);
  return this==parent->panes.last();
}
inline bool LOGPAGE::freebegin() {
//  return true;
  testcl;
  return panes.count==0 or panes[0]->first();
}
inline bool LOGPAGE::freeend() {
//  return true;
  testcl;
  return panes.count==0 or panes.last()->last();
}
inline bool LOGPAGE::canplaceonbegin(PANEGRP &grp,int grphi) {
//  return false;
//  bool p1=freebegin();bool p2=grphi+grp.lastopthi()+hi0<=maxhi;
  testcl;
  return freebegin() and grphi+grp.lastopthi()+hi0<=maxhi;
}
inline bool LOGPAGE::canplaceonend(PANEGRP &grp,int grphi) {
//  return false;
  testcl;
  return freeend() and grphi+hi<=maxhi;
}

int A4DISTRIB::pgnumofpane(PANE &pane) {
  for(int i=0;i<virtlogcnt;i++) if (LOGPAGE *pg=buf[i]) {
    int idx=pg->panes.search(&pane);
    if (idx>=0) return pg->pagenum;
  }
  return 0;
}

int LOGPAGES::pgindexofpane(PANE &pane) {
  for(int i=0;i<pages.count;i++) if (pages[i]->panes.search(&pane)>=0) return i;
  return -1;
}

int PANE::askdraw(CANVAS &acnv,bool drawos) {
  cnv.draw(acnv);
  int h=hi;
  if (drawos and os) {
    h+=os->hi;
    os->cnv.draw(SUBCANVAS(acnv,0,hi));
  }
  return h;
}

void A4DISTRIB::drawa4page(CANVAS &cnv,int sheet,int side,int *ofsx,int *ofsy,int hsize,int vsize,bool drawpomlines,int pgnumfont) {
  xcheckindex(sheet,shcnt);
  xcheckindex(side,2);
  if (drawpomlines) {
    cnv.fg(lclred);
    for(int x=0;x<hcnt;x++) for(int y=0;y<vcnt;y++) cnv.bg(lcwhite).Rect(ofsx[x],ofsy[y],hsize,vsize);
    cnv.fg(lcblack);
  }
  for(int x=0;x<hcnt;x++) for(int y=0;y<vcnt;y++) {
    LOGPAGE *pg=getpage(sheet,x,y,side);
    if (pg) {
      pg->draw(SUBCANVAS(cnv,ofsx[x],ofsy[y]));
      if (pgnumfont>=0) {
        int pgnum=pg->pagenum;
        if (pgnum) cnv.font(pgnumfont).text(ofsx[x]+hsize/2,ofsy[y]+vsize,itos(pgnum).c_str());
      }
    }
  }
}

void LOGPAGE::draw(CANVAS &cnv) {
  int y=0;
  for(int i=0;i<panes.count;i++) y+=panes[i]->askdraw(SUBCANVAS(cnv,0,y),i<panes.count-1);
}

bool LOGPAGE::addpane(PANE &pane) {
  testcl;
  if (canaddpane(pane)) {
    panes+=&pane;
    hi0=hi+pane.hi;
    hi+=pane.gethisum();
    return true;
  }
  return false;
}
void LOGPAGE::erasepane(int index,int count) {
  for(int i=index;i<index+count;i++) {
    if (i<panes.count) hi-=panes[index]->gethisum();else hi-=panes[index]->hi;
  }
  panes.erase(index,count);
}
bool LOGPAGE::insertpane(int index,PANE *pane) {
  if (hi+pane->gethisum()<=maxhi) {
    hi+=pane->gethisum();hi0+=pane->gethisum();
    panes.insert(index,pane);
  }
  return false;
}
bool LOGPAGE::blocked() {
  testcl;
  if (freebegin() or freeend()) return false;
  return true;//zatim ignoruje pridavani doprostred
}

int PANEGRP::sheetcnt(int pagehi) {
  testcl;
  int res=0;
  int actpghi=pagehi;
  for(int i=0;i<panes.count;i++) {
    if (actpghi+panes[i]->hi<=pagehi) actpghi+=panes[i]->gethisum();
    else {actpghi=panes[i]->gethisum();res++;}
  }
  return res;
}

void LOGPAGES::addpane(PANE &pane) {
  testcl;
  if (pages.count-startpage>0) {//divne,ale snad dobre
    if (pages.last()->addpane(pane)) return;
  }
  addpage()->addpane(pane);
}

int LOGPAGE::getextrapagecount(PANEGRP &panes) {
  LOGPAGE *pg=this;
  int h=pg->hi,h0=pg->hi0,res=0;
  for(int i=0;i<panes.panes.count;i++) {
    PANE *p=panes.panes[i];
    if (h+p->hi>maxhi) {
      res++;h=0;h0=0;
    }
    h0=h+pg->hi;
    h+=p->gethisum();
  }
  return res;
}

void LOGPAGE::addpanegrp(PANEGRP &panes) {
  LOGPAGE *pg=this;
  for(int i=0;i<panes.panes.count;i++) {
    if (pg->canaddpane(*panes.panes[i])) pg->addpane(*panes.panes[i]);
    else {
      pg=parent->addpage();
      pg->addpane(*panes.panes[i]);
    }
  }
}

void LOGPAGES::addpanegrp(PANEGRP &pg) {
  testcl;
  for(int i=0;i<pg.panes.count;i++) {
//    MEMSTROUT so;so.separator=" ";
//    TEXTCANVAS tc(&so);
//    pg.panes[i]->cnv.draw(tc);
//    jlib::savetextfile(basedir/itos(i)%"e.zk",so.str().c_str());
    addpane(*pg.panes[i]);
  }
}
void LOGPAGES::simpledistrib(const Array<PANEGRP*> &grps,int beg,int end) {
  for(int i=beg;i<=end;i++) addpanegrp(*grps[i]);
}

void LOGPAGES::distrib(const Array<PANEGRP*> &grps,int astartpage) {
  startpage=astartpage;
  if (startpage<0) startpage=pages.count;
  while (pages.count<startpage) addpage();
  if (nodivide) {
    if (dblsides) dbldistrib(grps);else monodistrib(grps);
  } else simpledistrib(grps,0,grps.count-1);
}

bool LOGPAGES::canaddatlasttwosides(PANEGRP &pan) {
  if (pages.count==0) return true;
  int cnt0=pan.addablepanes(pages.last()->freeendhi());
  int h2=pan.counthi(cnt0+1,false);
  return h2<=pghi;
}

void LOGPAGES::dbladd(PANEGRP &grp) {
  if (pages.count-startpage==0) addpage();
  if (not isleftpage(pages.count-1)) addpage();
  if (canaddatlasttwosides(grp)) {addpanegrp(grp);return;}
  addpage();
  if (not isleftpage(pages.count-1)) addpage();
  addpanegrp(grp);
}

void LOGPAGES::dbldistrib(const Array<PANEGRP*> &grps) {
  TCntVec<PANEGRP*> g1,g2;
  for(int i=0;i<grps.count;i++) {
    int sh=grps[i]->sheetcnt(pghi-safeconst);
    if (sh>2) addpanegrp(*grps[i]);
    else if (sh>1) g2+=grps[i];
    else g1+=grps[i];
  }
  for(int i=0;i<g2.count;i++) {xassert(g2[i]);dbladd(*g2[i]);}
  monodistribshortpages(g1);
}

struct HICOMPARE {
  int operator () (PANEGRP *a,PANEGRP *b) const {return b->gethi(false)-a->gethi(false);}
};

static bool placedtoend;

void LOGPAGES::monodistribshortpages(const Array<PANEGRP*> &g) {
  heapsort(g.buf,g.count,HICOMPARE());
  for(int gi=0;gi<g.count;gi++) {//rozmistovani dalsich
    int optindex=-1,optrest;
    for(int i=startpage;i<pages.count;i++) {
      xassert(g[gi]);
      int rest=pages[i]->canplaceonthispage(*g[gi]);
      if (rest>=0) {
        if (optindex<0 or rest<optrest) optindex=i,optrest=rest;
      }
    }
    placedtoend=false;
    if (optindex<0) {addpage();optindex=pages.count-1;placedtoend=true;}
    xassert(g[gi]);
    pages[optindex]->placeonthispage(*g[gi]);
  }
}


void LOGPAGES::monodistrib(const Array<PANEGRP*> &grps) {
//  for(int i=0;i<grps.count;i++) grps[i]->repairparents();
  for(int i=0;i<grps.count;i++)
    for(int j=0;j<grps[i]->panes.count;j++)
      testptr(grps[i]->panes[j]->parent);
  TCntVec<PANEGRP*> g;
  for(int i=0;i<grps.count;i++) {
    testptr(grps[i]);
    if (grps[i]->sheetcnt(pghi-safeconst)>1) addpanegrp(*grps[i]);else g+=grps[i];
  }
  monodistribshortpages(g);
}

int PANEGRP::counthi(int from,bool ended)  {
  testcl;
  int res=0;
  for(int i=from;i<panes.count-1;i++) res+=panes[i]->gethisum();
  if (panes.count) res+=panes.last()->hi;
  if (ended) res+=lastopthi();
  return res;
}

int PANEGRP::addablepanes(int hi) {
  testcl;
  int cnt=0;
  while (cnt<=panes.count) {
    hi-=panes[cnt]->hi;
    if (hi<0) return cnt;
    hi-=panes[cnt]->oshi();
    cnt++;
  }
  return cnt;
}

int LOGPAGE::canplaceonthispage(PANEGRP &grp) {
  testcl;
  int ghi=grp.gethi(false);
  if (canplaceonbegin(grp,ghi)) return maxhi-ghi-hi0-grp.lastopthi();
  if (canplaceonend(grp,ghi)) return maxhi-ghi-hi;
  return -1;
}

void LOGPAGE::placeonthispage(PANEGRP &grp) {
  testcl;
  int ghi=grp.gethi(false);
  if (canplaceonbegin(grp,ghi)) {
    panes.insert(0,grp.panes);
    hi+=ghi+grp.lastopthi();
    hi0+=ghi+grp.lastopthi();
  } else if (canplaceonend(grp,ghi))
  {
    panes.add(grp.panes);
    hi0=hi+ghi;
    hi=hi0+grp.lastopthi();
  } else {
    xthrowmsg(XError,("placedtoend=%s,ghi=%d,maxhi=%d,sheetcnt=%d",placedtoend?"true":"false",ghi,maxhi,grp.sheetcnt(maxhi) ));
  }
}

void PANEGRP::draw(CANVAS &cnv) {
  int y0=0;
  for(int i=0;i<panes.count;i++) {
    PANE &pane=*panes[i];
    pane.cnv.draw(SUBCANVAS(cnv,0,y0));
    y0+=pane.hi;
    if (pane.os) {
      pane.os->cnv.draw(SUBCANVAS(cnv,0,y0));
      y0+=pane.os->hi;
    }
  }
}

void PANEGRP::nextpane(int hi) {
//  xassert(hi<=1000);
  panes[panes.count-1]->hi=hi;
  panes+=new PANE(this,target);
}

CANVAS *PANEGRP::setlastos(int hi,int depth) {
  if (panes.count<depth+1) return NULL;
  PANE &p=*panes[panes.count-depth-1];
  if (p.os) delete p.os;
  p.os=new OPTSPACE(hi);
  return &p.os->cnv;
}

}

#pragma package(smart_init)
