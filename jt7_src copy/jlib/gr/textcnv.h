#ifndef textcnvH
#define textcnvH

#include <gr/comgr.h>
#include <gr/comcnv.h>
#include <gr/metacnv.h>

namespace gr {
using namespace jlib;

struct REGION:TCntVec<ABREC> {
  bool match(const XYREC &p) {for(int i=0;i<count;i++) if (item(i).has(p)) return true;return false;}
};

struct TEXTCANVAS {
  TEXTCANVAS(const CANVAS *acnv,int awi,FONTRESOURCE *afontresource) {init(acnv,awi,afontresource);}
  void write(const char *s,REGION *aregion=NULL,bool forcespaces=false);//vypise s akutalnim fontem, uziva wordwrap
  //forcespaces:musi byt vypsany i mezery(napr. pokud je to potrzene pismo)
  int varwrite(int index,const char *refstr,bool rightalign);//vrati x-ovou sour.kam psalo
  void font(const FONT &font);
  void fontindex(int index);
  void newline() {_newline(linehi,linehi);lineroll++;}
  virtual void _newline(int alinehi,int dy=0);//dy znamena,o kolik se posune acty dolu
  ~TEXTCANVAS() {if (not destroyed) done();}//POZOR!!!,vola vzdy TEXTCANVAS::done,ne u potomka
  void done() {newline();isactfont=false;destroyed=true;}
  FONT getactfont() {return actfont;}
  CANVAS &getcnv() {return actline;}
  int height() {return acty;}
  int curx() {return actx;}
  int linerollindex() {return lineroll;}
protected:
  bool destroyed;//indikaxce,zda uz byla volena fce done
  void initnewline();
  void simplewrite(const char *s,bool forcespaces);
  void addcurregion();//prida aktualni region od begx do pozice kurzoru
  METACANVAS actline;
  int linehi;
  int actx;
  int acty;
  int lineroll;
  int begx;//zacatek regionu
  const CANVAS *cnv;
  const CANVAS *target;
  int wi;
  int actfonthi;
  FONT actfont;
  bool isactfont;
  int actfontindex;//-1,pokud je plati actfont
  REGION *region;

  void setactfont() {if (actfontindex>=0) actline.font(actfontindex);actline.font(actfont);}
protected:
  void init(const CANVAS *acnv,int awi,FONTRESOURCE *afontresource) {cnv=acnv;wi=awi;actfonthi=0;actx=0;acty=0;linehi=0;region=NULL;target=acnv;actfontindex=-1;destroyed=false;lineroll=0;isactfont=false;actline.fontresource=afontresource;}
  TEXTCANVAS(int awi) {init(NULL,awi,NULL);}
};

}
#endif
