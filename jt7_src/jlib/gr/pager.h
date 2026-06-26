#ifndef pagerH
#define pagerH

namespace gr {
using namespace jlib;

template<class PAGE> struct A4DISTRIBUTION {
  A4DISTRIBUTION(int hcnt,int vcnt,Array<PAGE> &pages,PAGE nopage,int nopagesatbegin);
  //nopage je hodnota nestranky:zadna stranka nemuze mit hodnotu nopage
  //nopagesatbegin je pocet stran na zacatku, ktere se preskoci
  PAGE getpage(int sheet,int x,int y,int side);//vraci stranku
  //na listu cislo sheet,x,y:souradnice od 0,side==0:predni strana listu
  //side==1:zadni strana
  ~A4DISTRIBUTION() {delete []buf;}
  int sheetcnt() {return shcnt;}//pocet listu
  int freepgcnt() {return shcnt*hcnt*vcnt*2-pgcnt-begcnt;}
protected:
  PAGE *buf;
  //v buf ulozeno v poradi vyznamnosti:po listech,radcich,slopcich,zepredu/zezadu
  int shcnt;
  int hcnt;
  int vcnt;

  int begcnt;
  int pgcnt;
  int lpersh;//logickych stranek na list
  int virtlogcnt;//shcnt*lpersh
};


}
#endif
