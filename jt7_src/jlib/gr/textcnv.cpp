#include <jlib.h>
#pragma hdrstop

#include "textcnv.h"
#include <gr/subcnv.h>

namespace gr {
using namespace jlib;

inline void jmpsp(const char *&c,char *&w) {
  while (*c and (uchar)*c<=' ') *w++=*c++;
}
inline void jmpwrd(const char *&c,char *&w) {
  while (*c and (uchar)*c>' ') *w++=*c++;
}

void TEXTCANVAS::addcurregion() {
  if (region) {
    region->add(ABREC(begx,acty+linehi-actfonthi,actx-begx,actfonthi));
  }
}

void TEXTCANVAS::initnewline() {
  linehi=0;begx=0;
  actline.clear();actx=0;
  setactfont();
//  actline.font(actfont);
}

void TEXTCANVAS::_newline(int alinehi,int dy) {
  xassert(cnv);
  addcurregion();
  actline.draw(SUBCANVAS(*cnv,0,acty+dy));
  acty+=alinehi;
  initnewline();
}

void TEXTCANVAS::simplewrite(const char *s,bool forcespaces) {
  if (forcespaces or not seek0((char*)s)) actline.text(actx,-actfonthi,s);
  actx+=target->textwi(s);
  if (linehi<actfonthi) linehi=actfonthi;
}

void TEXTCANVAS::write(const char *c,REGION *aregion,bool forcespaces) {
  xassert(target);

  region=aregion;
  char s[0x100],*ws=s;
  jmpsp(c,ws);*ws=0;
  begx=actx;
  if (target->textwi(s)+actx>wi) {newline();ws=s;} else {simplewrite(s,forcespaces);ws=s;}
  while (*c) {
    jmpwrd(c,ws);*ws=0;
    if (target->textwi(s)+actx>wi) {newline();ws=s;simplewrite(s,forcespaces);} else {simplewrite(s,forcespaces);ws=s;}
    jmpsp(c,ws);*ws=0;
    if (target->textwi(s)+actx>wi) {newline();ws=s;} else {simplewrite(s,forcespaces);ws=s;}
  }
  addcurregion();
  region=NULL;
}

int TEXTCANVAS::varwrite(int index,const char *refstr,bool rightalign) {
  int twi=target->textwi(refstr);
  if (twi+actx>wi) newline();
  if (rightalign) actx=wi-twi;
  int res=actx;
  actline.vartext(actx,-actfonthi,index);
  actx+=twi;
  if (linehi<actfonthi) linehi=actfonthi;
  return res;
}


void TEXTCANVAS::font(const FONT &font) {
  xassert(target);

  actfontindex=-1;
  target->font(font);
  actfonthi=target->texthi("EM");
  actline.font(font);
  actfont=font;
}

void TEXTCANVAS::fontindex(int index)  {
  xassert(target);

  isactfont=false;
  target->font(index);
  actfonthi=target->texthi("EM");
  actline.font(index);
  actfontindex=index;
}


}
#pragma package(smart_init)

