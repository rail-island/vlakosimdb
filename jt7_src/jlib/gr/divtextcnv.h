#ifndef divtextcnvH
#define divtextcnvH

#include <gr/comgr.h>
#include <gr/subcnv.h>
#include <gr/metacnv.h>
#include <gr/pager.h>

#define NOCLASSCTRL
#include "debug/classctrl.h"

namespace gr {
using namespace jlib;

#include "divtextcnv.i"

class PANEGRP;class LOGPAGES;

struct OPTSPACE {
  METACANVAS cnv;
  int hi;
  explicit OPTSPACE(int ahi=0,const CANVAS *atarget=NULL):cnv(atarget) {hi=ahi;}
};

struct PANE:IPANE {//jeden oddil
  METACANVAS cnv;
  OPTSPACE *os;//pokud na te same strance je jeste neco pod timto,je tisknuto os
  int hi;
  PANEGRP *parent;
  explicit PANE(PANEGRP *aparent,const CANVAS *atarget):cnv(atarget) {os=NULL;hi=0;parent=aparent;initcl;}

  int draw(CANVAS &acnv) {return askdraw(cnv,not last());}
  int askdraw(CANVAS &acnv,bool drawos);//vrati vysku

  int gethisum() {return hi+oshi();}
  int oshi() {if (os) return os->hi;return 0;}
  bool first();
  bool last();

  ~PANE() {if (os) delete os;donecl;}
  clstr;

  bool STDCALL isfirst() {return first();}
  bool STDCALL islast() {return last();}
  int STDCALL gethi() {return gethisum();}
  IPANEGRP * STDCALL getparent();
};

struct PANEGRP:IPANEGRP {
  IObjArray<PANE> panes;
  const CANVAS *target;
  explicit PANEGRP(const CANVAS *atarget=NULL) {target=atarget;panes+=new PANE(this,target);initcl;}
  ~PANEGRP() {donecl;}
  CANVAS &cnv() {return panes[panes.count-1]->cnv;}
  void clear(const CANVAS *atarget=NULL) {target=atarget;panes.clear();panes+=new PANE(this,target);}
  void nextpane(int hi);
  void draw(CANVAS &cnv);

  int sheetcnt(int pagehi);//spocita nejmensi pocet stranek,na ktere se vejde v zavislosti
  //na jejich vysce

  int lastopthi() {if (panes.count and panes[panes.count-1]->os) return panes[panes.count-1]->os->hi;return 0;};

  int gethi(bool ended) {return counthi(0,ended);}
  //spocita celkovou vysku,ended:zda se ma zapocitavat posledni os
  int counthi(int from,bool ended);//to same,zacina od pane s indexem from

  int addablepanes(int hi);//spocita,kolik panes se jeste vejse na tuto do hi

  CANVAS *setlastos(int hi,int depth=0);//nastavi OPTSPACE u pane o x depth zpatky
  //(napr. depth==1:=>panes[count-2]=new OPTSPACE(hi)...
  void repairparents() {for(int i=0;i<panes.count;i++) panes[i]->parent=this;}


  IPANE * STDCALL getpane(int index) {return panes[index];}
  int STDCALL panecount() {return panes.count;}
  int STDCALL sheetCount(int pghi) {return sheetcnt(pghi);}

  clstr;
};
IPANEGRP * STDCALL PANE::getparent() {return parent;}

struct LOGPAGE:ILOGPAGE {
  bool addpane(PANE &pane);//zkusi pridat pane
  void addpanegrp(PANEGRP &panes);//prida panes,pokud se nevejde na tuto stranku,prida na dalis
  void erasepane(int index,int count);
  bool insertpane(int index,PANE *pane);
  int getextrapagecount(PANEGRP &panes);//vrati, kolik dalsich stranek by jeste bylo potreba,
  //aby se tam panes veslo,nic nepridava
  bool canaddpane(PANE &pane);//zjisti,zda se da pridat pane
  int gethi() {return hi;}
  int freeendhi() {return maxhi-hi;}
  explicit LOGPAGE(LOGPAGES *aparent);

  bool blocked();//nic nelze pridavat,i kdyz se vejde
  bool freebegin();//zda lze vkladat na zacatek
  bool freeend();//zda lze pridavat na konec(<=>nepokracuje na dalsi strance)

  int canplaceonthispage(PANEGRP &grp);//uziva se pri monodistrib
  //vrati -1,pokud nelze umistit nebo >=0,coz je misto,ktere pri tomto umisteni zbyde volne

  bool canplaceonbegin(PANEGRP &grp,int grphi);
  bool canplaceonend(PANEGRP &grp,int grphi);

  void placeonthispage(PANEGRP &grp);

  void draw(CANVAS &cnv);

  ~LOGPAGE() {donecl;}
  clstr;
  TCntVec<PANE*> panes;

  int pagenum;//cislo strany(od 1),pokud je 0,netiskne se
private:
  int hi;//aktualni vyska stranky,zapocitava se i vyska OPTSPACE posledni pane=>muze byt >maxhi
  int hi0;//vyska stranky bez posledniho OPTSPACE,hi0<=maxhi VZDY
  int maxhi;
  LOGPAGES *parent;

protected:
  virtual bool STDCALL addPane(IPANE *pane) {return addpane(dynamic_cast<PANE&>(*pane));}
  virtual bool STDCALL canaddPane(IPANE *pane) {return canaddpane(dynamic_cast<PANE&>(*pane));}
  virtual int STDCALL getHi() {return gethi();}

  virtual bool STDCALL isblocked() {return blocked();}
  virtual bool STDCALL isfreebegin() {return freebegin();}
  virtual bool STDCALL isfreeend() {return freeend();}
  virtual bool STDCALL isleft();

  virtual void STDCALL addPaneGrp(IPANEGRP *panes) {return addpanegrp(dynamic_cast<PANEGRP&>(*panes));}
  virtual int STDCALL getExtraPageCount(IPANEGRP *panes) {return getextrapagecount(dynamic_cast<PANEGRP&>(*panes));}

  virtual int STDCALL getPaneCount() {return panes.count;}
  virtual IPANE * STDCALL getPane(int index) {return panes[index];}

  virtual void STDCALL erasePane(int index,int count) {erasepane(index,count);}
  virtual bool STDCALL insertPane(int index,IPANE *pane) {return insertpane(index,(PANE*)pane);}
};

//jsinterfaceH(LogPages,LOGPAGES)

struct LOGPAGES:ILOGPAGES {
  IObjArray<LOGPAGE> pages;

  bool nodivide;//snazi se nerozsdelovat PANEGRP=>jsou pak na strankach zprehazene
  bool dblsides;//pocita s tim,ze jsou vzdy videt dve stranky najednou
  void distrib(const Array<PANEGRP*> &grps,int astartpage=-1);
  //startpage-cislo stranky,od ktere se zacina distribuovat,v pripade,
  //za stranek je mene,jsou pridane prazdne,-1:distribuuje se od dalsi stranky
  //(astartpage:=pages.count)
  bool firstleft;//zda prvni strana(s indexem 0) je na leve strane
  //vyuzive se jen,pokud dblsides==true,default:false

  LOGPAGES(int apghi,bool anodivide=false,bool adblsides=false,bool afirstleft=false) {pghi=apghi;nodivide=anodivide;dblsides=adblsides;firstleft=afirstleft;initcl;startpage=0;}

  void dbladd(PANEGRP &grp);//prida na konec dvojstranu zacinajici na leve strance(muze vlozit prazdnou stranu)

  ~LOGPAGES() {donecl;}
  clstr;
  void simpledistrib(const Array<PANEGRP*> &grps) {simpledistrib(grps,0,grps.count-1);}
  void addpanegrp(PANEGRP &pg);

  int pgindexofpane(PANE &pane);//vrati cislo stranky cislovane od 0,-1,kdyz nenajde

  void makepgnums(int first=1) {for(int i=0;i<pages.count;i++) pages[i]->pagenum=first+i;}
  void makepgnumsfromto(int from,int to,int first=1) {for(int i=from;i<to;i++) pages[i]->pagenum=first+i-from;}

private:
  int startpage;

  void simpledistrib(const Array<PANEGRP*> &grps,int beg,int end);
  //jednoducha distribuce od beg,end je na konci,oba indexy v grps
  void addpane(PANE &pane);

  void dbldistrib(const Array<PANEGRP*> &grps);
  void monodistrib(const Array<PANEGRP*> &grps);

  void monodistribshortpages(const Array<PANEGRP*> &g);

  LOGPAGE *addpage() {LOGPAGE *pg=new LOGPAGE(this);pages+=pg;return pg;}
  int pghi;

  bool isleftpage(int index);

  bool canaddatlasttwosides(PANEGRP &pan);//vrati,zda se pan vejde na POSLEDNI A jednu stranlu
  //(aby se vesla,je potreba pri return true pridate jeste jednu stranku

  friend class LOGPAGE;

  virtual ILOGPAGE * STDCALL addPage() {return addpage();}
  virtual int STDCALL getpagehi() {return pghi;}
  virtual ILOGPAGE * STDCALL getPage(int index) {return pages[index];}
  virtual ILOGPAGE *last() {return pages.count?pages.last():NULL;}
  virtual int pageCount() {return pages.count;}
};

template struct A4DISTRIBUTION<LOGPAGE*>;

struct A4DISTRIB:A4DISTRIBUTION<LOGPAGE*> {
  A4DISTRIB(int hcnt,int vcnt,Array<LOGPAGE*> &pages,int begcnt):A4DISTRIBUTION<LOGPAGE*>(hcnt,vcnt,pages,NULL,begcnt) {}

  void drawa4page(CANVAS &cnv,int sheet,int side,int *ofsx,int *ofsy,int hsize,int vsize,
            bool drawpomlines,int pgnumfont=-1);
  //nakresli jednu stranku,vsize,hsize jseou velikosti stranek,
  //na ktere se tiskne,drawpomlines==true:nakresli cervene okraje
  //pokud pgnumfont>=1,tisknou se cisla stran(u tech stran,ktere maji cisla !=0)

  int pgnumofpane(PANE &pane);//vrati cislo stranky nebo 0,pokud nenajde
};

inline void drawlogpage(LOGPAGE *logpage,CANVAS &cnv) {logpage->draw(cnv);}

inline LOGPAGE::LOGPAGE(LOGPAGES *aparent) {hi=0;hi0=0;parent=aparent;maxhi=parent->pghi;initcl;pagenum=0;}
}


//---------------------------------------------------------------------------
#endif
