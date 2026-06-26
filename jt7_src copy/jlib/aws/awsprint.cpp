#include <jlib.h>
#pragma hdrstop

#include "awsprint.h"
#include "awspaint.h"

namespace aws {

int LogPages::searchpane(Pane *pane) {
  for(int i=0;i<pages.count;i++) if (pages[i]->panes.search(pane)>=0) return i;
  return -1;
}

Pane *PaneGrp::nextpane(int hi,string type) {
  if (type=="optional" && panes.count>=2) {
    panes.last()->sethi(hi);
    panes[panes.count-2]->setopt(panes[panes.count-1]);
    panes.eraselast();
//    return panes.last()->getopt();
  } else {
    if (panes.count) {
      panes.last()->sethi(hi);
    }
  }
  Pane *res=new Pane(this);
  panes.add(res);
  return res;
/*  if (panes.count) {
    if (type=="optional") panes.last()->setopt(true);
    panes.last()->sethi(hi);
  }
  Pane *res=new Pane(this);
  panes.add(res);
  return res;*/
}

void PaneGrp::begindoc(SPCanvas dest) {
  panes.clear();
  cnv=new PaneGrpCanvas(this,nextpane(0,""),dest);
}

void PaneGrp::enddoc() {
  if (panes.count>0 && panes.last()->gethisum()==0) panes.eraselast(1);
  cnv=NULL;
}

void PaneGrpCanvas::nextband(Canvas &cnv,int hi,string type) {
  Pane *pane=panegrp->nextpane(hi,type);
  setdraw(&pane->meta);
//  cnv.cnv=&pane->meta;
}

void LogPages::assign(LogPages &src) {
  pghi=src.pghi;
  pgwi=src.pgwi;
  pages.clear();
  for(int i=0;i<src.pages.count;i++) {
    LogPage *page=new LogPage(this);
    page->panes.assign(src.pages[i]->panes);
    pages.add(page);
  }
}

void LogPages::print(Pageable *prn) {
  for(int i=0;i<pages.count;i++) {
    Canvas cnv=prn->beginpage();
    pages[i]->draw(cnv);
    prn->endpage();
  }
}

void LogPage::draw(Canvas cnv) {
  int dy=0;
  for(int i=0;i<panes.count;i++) {
    panes[i]->meta.draw(Canvas(cnv,0,dy));
    dy+=panes[i]->gethisum();
  }
}
void LogPage::addpane(Pane *pane) {
//  hi+=pane->gethi();
  panes.add(pane);
  counthis();
}
bool LogPage::canaddpane(Pane *pane) {
  return hi+pane->getminhi()<=logpages->getpghi();
}


void LogPages::setpropsfrom(Pageable *prn) {
  pgwi=prn->getpgwi();pghi=prn->getpghi();
}

void LogPages::addpanegrp(PaneGrp *grp) {
  if (pages.count==0) addpage();
  int index=0;
  while (index<grp->panes.count) {
    while (index<grp->panes.count && pages.last()->canaddpane(grp->panes[index])) {
      pages.last()->addpane(grp->panes[index++]);
    }
    if (index<grp->panes.count) {
      if (pages.last()->gethi()==0) pages.last()->addpane(grp->panes[index++]);
      addpage();
    }
  }
}

int PaneGrp::gethi(bool lastopthi) {
  int res=0;
  for(int i=0;i<panes.count-1;i++) res+=panes[i]->gethisum();
  if (panes.count>0) {
    if (lastopthi) res+=panes.last()->gethisum();
    else res+=panes.last()->getminhi();
  }
//  for(int i=0;i<panes.count-1;i++) res+=panes[i]->gethi();
//  if (panes.count>0 && !panes.last()->isopt()) res+=panes.last()->gethi();
  return res;
}
int PaneGrp::getlastopthi() {
  if (panes.count>0) return panes.last()->getopthi();
//    if (panes.last()->isopt()) return panes.last()->gethi();
  return 0;
}

void LogPage::counthis() {
  hi=0;hi0=0;
  for(int i=0;i<panes.count-1;i++) hi+=panes[i]->gethisum();
  if (panes.count>0) {
    hi0+=panes.last()->getminhi();
    hi+=panes.last()->gethisum();
  }
/*
  hi=0;hi0=0;
  if (panes.count==0) return;
  for(int i=0;i<panes.count-1;i++) hi0+=panes[i]->gethi();
  hi=hi0;
  if (panes.last()->isopt()) hi+=panes.last()->gethi();
  else hi0+=panes.last()->gethi(),hi=hi0;
*/
}

int LogPage::getmaxhi() {
  return logpages->getpghi();
}

//*****************************************************
//Copied
//*****************************************************


bool LogPage::isleft() {
  return logpages->isleftpage(getindex());
}

int LogPage::getindex() {
  return logpages->pages.search(this);
}

bool LogPages::isleftpage(int index) {
  return ((index+(firstleft?0:1))&1)==0;
}
bool Pane::first() {
  return this==panegrp->panes[0];
}
bool Pane::last() {
  return this==panegrp->panes.last();
}
bool LogPage::freebegin() {
  return panes.count==0 || panes[0]->first();
}
bool LogPage::freeend() {
  return panes.count==0 || panes.last()->last();
}
bool LogPage::canplaceonbegin(PaneGrp *grp) {
  return freebegin() && grp->gethi(true)+hi0<=getmaxhi();
}
bool LogPage::canplaceonend(PaneGrp *grp) {
  return freeend() && hi+grp->gethi(false)<=getmaxhi();
}

int LogPages::pgindexofpane(Pane *pane) {
  for(int i=0;i<pages.count;i++) if (pages[i]->panes.search(pane)>=0) return i;
  return -1;
}

void LogPage::erasepane(int index,int count) {
  panes.erase(index,count);
  counthis();
}
void LogPage::insertpane(int index,Pane *pane) {
  if (hi+pane->gethisum()<=getmaxhi()) {
    panes.insert(index,pane);
    counthis();
  }
  throw JError("Cannot insert pane");
}

int PaneGrp::sheetcnt(int pagehi) {
  int res=0;
  int actpghi=pagehi;
  for(int i=0;i<panes.count;i++) {
    if (actpghi+panes[i]->getminhi()<=pagehi) actpghi+=panes[i]->gethisum();
    else {actpghi=panes[i]->gethisum();res++;}
  }
  return res;
}

void LogPages::addpane(Pane *pane) {
  if (pages.count-startpage>0) {//divne,ale snad dobre
    if (pages.last()->canaddpane(pane)) {
      pages.last()->addpane(pane);
      return;
    }
  }
  addpage()->addpane(pane);
}

int LogPage::getextrapagecount(PaneGrp *panegrp) {
  LogPage *pg=this;
  int h=pg->hi,h0=pg->hi0,res=0;
  for(int i=0;i<panegrp->panes.count;i++) {
    Pane *p=panegrp->panes[i];
    if (h+p->getminhi()>getmaxhi()) {
      res++;h=0;h0=0;
    }
    h0=h+pg->hi;
    h+=p->gethisum();
  }
  return res;
}

void LogPage::addpanegrp(PaneGrp *panegrp) {
  xassert(logpages);
  LogPage *pg=this;
  for(int i=0;i<panegrp->panes.count;i++) {
    if (pg->canaddpane(panegrp->panes[i])) pg->addpane(panegrp->panes[i]);
    else {
      pg=logpages->addpage();
      pg->addpane(panegrp->panes[i]);
    }
  }
}

int LogPage::canplaceonthispage(PaneGrp *grp) {
  int ghi=grp->gethi(false);
  if (canplaceonbegin(grp)) return getmaxhi()-ghi-hi0-grp->getlastopthi();
  if (canplaceonend(grp)) return getmaxhi()-ghi-hi;
  return -1;
}
/*
void LogPage::placeonthispage(PANEGRP &grp) {
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
*/

//**********************************************************************
//PageDistrib
//**********************************************************************

int PageDistrib::getpage(int sheet,int x,int y,int side) {
  xcheckindex(sheet,shcnt);
  xcheckindex(x,hcnt);
  xcheckindex(y,vcnt);
  xcheckindex(side,2);
  int v;
  if (side) v=lpersh*sheet+y*hcnt*2+x*2+side;else v=lpersh*sheet+y*hcnt*2+(hcnt-x-1)*2+side;
  xcheckindex(v,virtlogcnt);
  return buf[v];
}

void PageDistrib::_setpage(int sheet,int x,int y,int side,int value) {
  int v;
  if (side) v=lpersh*sheet+y*hcnt*2+x*2+side;else v=lpersh*sheet+y*hcnt*2+(hcnt-x-1)*2+side;
  buf[v]=value;
}

inline int _page(int index,int pagecnt,int begcnt) {
  if (index<begcnt) return -1;
  if (index<pagecnt+begcnt) return index-begcnt;
  return -1;
}

PageDistrib::PageDistrib(int ahcnt,int avcnt,int pagecnt,int nopagesatbegin,bool linear) {
#define page(_index_) (_page(_index_,pagecnt,begcnt))
  begcnt=nopagesatbegin;
  pgcnt=pagecnt;
  hcnt=ahcnt;vcnt=avcnt;
  xassert(hcnt);xassert(vcnt);
  lpersh=2*vcnt*hcnt;
  virtlogcnt=pagecnt+begcnt;
  while (virtlogcnt%lpersh) virtlogcnt++;//virtualni pocet logickych stranek
  shcnt=virtlogcnt/lpersh;
  buf=new int[virtlogcnt];
  if (linear) {//linearne-po radcich
    int actpage=0;
    for(int i=0;i<shcnt;i++)
      for(int j=0;j<2;j++)
        for(int l=0;l<vcnt;l++)
          for(int k=0;k<hcnt;k++)
            _setpage(i,k,l,j,page(actpage++));
//    for(int i=0;i<virtlogcnt;i++) buf[i]=page(i);
  } else if (hcnt%2==1) {//liche horizontalne
    for(int i=0;i<virtlogcnt;i++) buf[i]=page(i);
  } else {//sude horizontalne
    int *act=buf;
    for(int i=0;i<virtlogcnt/2;i+=2) {
      *act++=page(i);
      *act++=page(i+1);
      *act++=page(virtlogcnt-i-1);
      *act++=page(virtlogcnt-i-2);
    }
  }
#undef page
}


void RealPages::distrib(Array<int> &ofsx,Array<int> &ofsy,VirtualPages &src,bool linear) {
  PageDistrib pd(ofsx.count,ofsy.count,src.vp_count(),0,linear);
  pages.clear();
  for(int i=0;i<pd.sheetcnt()*2;i++) {
    RealPage *page=new RealPage();
    pages.add(page);
    for(int y=0;y<ofsy.count;y++) for(int x=0;x<ofsx.count;x++) {
      int index=pd.getpage(i/2,x,y,i%2);
      if (index>=0) {
        VirtualPage *vpage=src.vp_getpage(index);
        vpage->draw(Canvas(&page->meta,ofsx[x],ofsy[y]));
      }
    }
  }
}


}


