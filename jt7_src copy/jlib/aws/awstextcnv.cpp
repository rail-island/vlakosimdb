#include <jlib.h>
#pragma hdrstop

#include "awstextcnv.h"
#include <gram/gmanal.h>

namespace aws {

bool TextCanvas::forcetext(int space,string text) {
  int hi=cnv.texthi(text);
  if (pagehi>=0 && hi+bufy0+inbufy>pagehi) return false;
  line.drawtext(actx+space,-hi,text);
  actx+=space+cnv.textwi(text);
  if (hi>linehi) linehi=hi;
}

void TextCanvas::flush() {
  wrap();
  buffer.draw(Canvas(cnv,0,bufy0));
  bufy0+=inbufy;
  buffer.clear();
  inbufy=0;
}

void TextCanvas::wrap() {
  if (pagehi<0) {//primo
    acty+=linehi;
    line.draw(Canvas(cnv,0,acty));
    line.clear();
    linehi=0;actx=0;
  } else {//pres buffer
    inbufy+=linehi;
    line.draw(Canvas(&buffer,0,inbufy));
    line.clear();
    linehi=0;actx=0;
  }
}

bool TextCanvas::textelem(int space,string s) {
  int w=cnv.textwi(s);
  if (actx+space+w<=wi) return forcetext(space,s);
  wrap();
  return forcetext(0,s);
}

bool TextCanvas::textelemx(int space,int x,string s) {
  int w=cnv.textwi(s);
  if (actx+space>x) wrap();
  return forcetext(x-actx,s);
}

bool TextCanvas::text(string s) {
  gram::Analyser a(s.c_str());
  int space=cnv.textwi(" ");
  while (*a) {
    bstring wrd;
    while (*a==' ') a++;
    if (!*a) return true;
    while (*a && !a.is_white()) wrd<<*a,a++;
    if (!textelem(space,wrd)) return false;
  }
  return true;
}

void TextCanvas::font(Font *font) {
  line.setfont(font);
  cnv.font(font);
}
}
