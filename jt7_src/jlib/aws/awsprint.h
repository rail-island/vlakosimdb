#ifndef awsprintH
#define awsprintH

#include "awscomponent.h"
#include "awsgr.h"
#include <com/ptrto.h>

namespace aws {

/*
struct PrintData {
  RefArray<Font> fonts;
};
*/

struct Pageable {
  virtual Canvas beginpage()=0;
  virtual void endpage()=0;
  virtual int getpghi()=0;
  virtual int getpgwi()=0;
};

struct Printer:Component,Pageable {
  virtual void loadprops(XMLNode xml)=0;
  virtual void saveprops(XMLNode xml)=0;
  ///vraci informacni platno, to umi vse krome vlastniho kresleni, platnost je do zruseni Printer
  virtual Canvas getinfo()=0;
  ///vracene platno je platne do volani endpage
  virtual Canvas beginpage()=0;
  virtual void endpage()=0;
  virtual int getpgwi()=0;
  virtual int getpghi()=0;
  ///vraci font se zmenenou velikosti(dostane obrazovkovy font)
  virtual Font *createprnfont(Font *src)=0;
  ///nastaveni a vyber tiskarny
  virtual void setup()=0;
  ///nastaveni vybrane tiskarny
  virtual void detailsetup()=0;
  virtual void begindoc(STRING title,STRING file)=0;
  virtual void enddoc()=0;
  virtual int getppmmx()=0;
  virtual int getppmmy()=0;
  Printer(Component *owner,System *system):Component(owner,system) {}
};

class PaneGrp;
struct Pane:RCObject {
  MetaCanvas meta;
  PaneGrp *panegrp;

  void sethi(int ahi) {hi=ahi;}
  int gethisum() {return getminhi()+getopthi();}
  int getminhi() {return hi;}
  int getopthi() {return opt?opt->hi:0;}
//  bool isopt() {return optional;}//pokud je prvni nebo posledni na strance, muze se vynechat
//  void setopt(bool value) {optional=value;}
  bool first();
  bool last();

  Pane *getopt() {return opt;}
  void setopt(Pane *pane) {opt=pane;}

  Pane(PaneGrp *panegrp):panegrp(panegrp) {hi=0;}//optional=false;}
  void deletedgrp() {panegrp=NULL;}
private:
  int hi;
  RefBase<Pane> opt;
//  bool optional;
};


struct PaneGrpCanvas:RedirectCanvas {
  PaneGrp *panegrp;Pane *pane;
  PaneGrpCanvas(PaneGrp *panegrp,Pane *pane,SPCanvas dest):panegrp(panegrp),pane(pane) {
     setdraw(&pane->meta);setask(dest);setband(&*this);
  }
  virtual void nextband(Canvas &cnv,int hi,string type);
};

/*
struct PaneGrpCanvas:RealCanvas {
  PaneGrp *panegrp;Pane *pane;RealCanvas *dest;
  //PaneGrpCanvas:dotazy posila do destcanvas, operace uklada do aktualni Pane,
  //nextpane vytvori dalsi pane
  virtual void setfgcolor(int color) {pane->meta.setfgcolor(color);}
  virtual void setbgcolor(int color) {pane->meta.setbgcolor(color);}
  virtual void setpenwi(int wi) {pane->meta.setpenwi(wi);}
  virtual void drawline(int x1,int y1,int x2,int y2) {pane->meta.drawline(x1,y1,x2,y2);}
  virtual void drawrect(int x1,int y1,int x2,int y2) {pane->meta.drawrect(x1,y1,x2,y2);}
  virtual void drawellipse(int x1,int y1,int x2,int y2) {pane->meta.drawellipse(x1,y1,x2,y2);}
  virtual int gettextwi(string text) {return dest->gettextwi(text);}
  virtual int gettexthi(string text) {return dest->gettexthi(text);}
  virtual void setfont(Font *afont) {pane->meta.setfont(afont);dest->setfont(afont);}
  virtual void drawtext(int x,int y,string str) {pane->meta.drawtext(x,y,str);}
  virtual void nextband(Canvas &cnv,int hi);

  PaneGrpCanvas(PaneGrp *panegrp,Pane *pane,RealCanvas *dest):panegrp(panegrp),pane(pane),dest(dest) {}
};
*/

struct PaneGrp:RCObject {
  TRCArray<Pane> panes;
  PtrTo<PaneGrpCanvas> cnv;
  void begindoc(SPCanvas dest);
  void enddoc();
  int gethi(bool lastopthi);//spocita vyska
  int getlastopthi();//vrati vysku posledni pane, pokud je optional
  int gethisum() {int res=0;for(int i=0;i<panes.count;i++) res+=panes[i]->gethisum();return res;}
  int sheetcnt(int pagehi);

  Canvas getcnv() {if (!cnv) throw JError("callink PaneGrp::getcnv() without PaneGrp::begindoc()");return Canvas(&*cnv,0,0);}
  Pane *nextpane(int hi,string type);
  ~PaneGrp() {for(int i=0;i<panes.count;i++)  panes[i]->deletedgrp();}
};

class LogPages;

struct VirtualPage {
  virtual void draw(Canvas cnv)=0;
};
struct VirtualPages {
  virtual int vp_count()=0;
  virtual VirtualPage *vp_getpage(int index)=0;
};

struct LogPage:VirtualPage,RCObject {
  TRCArray<Pane> panes;
  LogPages *logpages;

  int gethi() {return hi;}

  void draw(Canvas cnv);
  void addpane(Pane *pane);
  bool canaddpane(Pane *pane);
  void deletedpages() {logpages=NULL;}
  bool isleft();
  int getindex();
  bool freebegin();
  bool freeend();
  bool isblocked() {return !freebegin() && !freeend();}
  bool canplaceonbegin(PaneGrp *grp);
  bool canplaceonend(PaneGrp *grp);
  int canplaceonthispage(PaneGrp *grp);
  int getmaxhi();
  void insertpane(int index,Pane *pane);
  void erasepane(int index,int count);
  void addpanegrp(PaneGrp *panegrp);
  int getextrapagecount(PaneGrp *panegrp);

  LogPage(LogPages *logpages):logpages(logpages) {hi=0;hi0=0;}
private:
  void counthis();
  int hi;//aktualni vyska stranky,zapocitava se i vyska OPTSPACE posledni pane=>muze byt >maxhi
  int hi0;//vyska stranky bez posledniho OPTSPACE,hi0<=maxhi VZDY
};

struct LogPages:VirtualPages,RCObject {
  TRCArray<LogPage> pages;
  int pghi,pgwi;

/*  virtual Canvas beginpage();
  virtual void endpage();
  virtual int getpghi() {return pghi;}
  virtual int getpgwi() {return pgwi;}*/
  int getpghi() {return pghi;}
  int getpgwi() {return pgwi;}

  virtual int vp_count() {return pages.count;}
  virtual VirtualPage *vp_getpage(int index) {return pages[index];}

  void setsize(int w,int h) {pgwi=w;pghi=h;}
  void print(Pageable *prn);
  void setpropsfrom(Pageable *prn);
  void addpanegrp(PaneGrp *grp);
  LogPage *addpage() {LogPage *res=new LogPage(this);pages.add(res);return res;}
  void addpane(Pane *pane);
  int pgindexofpane(Pane *pane);
  int searchpane(Pane *pane);

  LogPages() {pghi=pgwi=0;firstleft=false;}
  ~LogPages() {for(int i=0;i<pages.count;i++) pages[i]->deletedpages();}

  bool isleftpage(int index);
  void assign(LogPages &src);
//  void complete(Array<int> &ofsx,Array<int> &ofsy);
private:
  bool firstleft;
  static const int startpage=0;
//  LogPage *actpage;
};

struct RealPage:VirtualPage {
  MetaCanvas meta;
  void draw(Canvas cnv) {meta.draw(cnv);}
};

struct RealPages:VirtualPages {
  IObjArray<RealPage> pages;
  virtual int vp_count() {return pages.count;}
  virtual VirtualPage *vp_getpage(int index) {return pages[index];}
  void distrib(Array<int> &ofsx,Array<int> &ofsy,VirtualPages &src,bool linear);
};

struct PageDistrib {
  PageDistrib(int hcnt,int vcnt,int pagecnt,int nopagesatbegin,bool linear);
  //nopagesatbegin je pocet stran na zacatku, ktere se preskoci
  //linear:nedela sesitek, ale cary
  int getpage(int sheet,int x,int y,int side);//vraci stranku cislovanou od 0
  //na listu cislo sheet,x,y:souradnice od 0,side==0:predni strana listu
  //side==1:zadni strana
  ~PageDistrib() {delete []buf;}
  int sheetcnt() {return shcnt;}//pocet listu
  int freepgcnt() {return shcnt*hcnt*vcnt*2-pgcnt-begcnt;}
protected:
  int *buf;
  //v buf ulozeno v poradi vyznamnosti:po listech,radcich,slopcich,zepredu/zezadu
  int shcnt;
  int hcnt;
  int vcnt;

  int begcnt;
  int pgcnt;
  int lpersh;//logickych stranek na list
  int virtlogcnt;//shcnt*lpersh

  void _setpage(int sheet,int x,int y,int side,int value);
};


}

#endif
