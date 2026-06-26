#ifndef awstextcnvH
#define awstextcnvH

#include "awspaint.h"

namespace aws {

struct TextCanvas {
  bool text(string s);
  bool textelemx(int space,int x,string s);
  ///vypise text, ale nemuze uz dal delit;space plati, pokud neni za zacatku radku
  bool textelem(int space,string s);
  void font(Font *font);
  void wrap();
  void flush();
  TextCanvas(Canvas cnv,int wi,int pagehi=-1):cnv(cnv),wi(wi),pagehi(pagehi) {
    linehi=0;actx=0;acty=0;bufhi=0;bufy0=0;bufhi=0;inbufy=0;
  }
private:
  ///nikdy neodradkovava, natvrdo napise, vrati, zda se vejde na stranku
  bool forcetext(int space,string text);

  int actx,acty;
  int linehi;

  int pagehi;//<0:neomezena
  int wi;
  Canvas cnv;
  MetaCanvas line;

  MetaCanvas buffer;
  int bufhi;
  int bufy0;//horni pozice bufferu
  int inbufy;//pozice vuci bufferu
};

}

#endif
