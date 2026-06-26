#include <jlib.h>
#pragma hdrstop

#include "sectanal.h"

namespace jlib {


SECTION emptysection=SECTION::_EMPTYSECTIONINITIALIZER();

int SECTION::findsection(const char *subsect) {
  for(int i=0;i<count();i++) if (eqstr(subsect,items[i].name)) return i;
  return -1;
}

char *SECTION::alloctext() {
  char *buf=new char[inafterend-inbeg+1];
  strecpy(buf,inbeg,inafterend);
  trimr(buf);
  return buf;
}
string SECTION::text() {
  string res;
  strecpy(res.reservebuffer(inafterend-inbeg+1),inbeg,inafterend);
  res.updatelen();
  res.trimr();
  return res;
}
void SECTION::text(char *buf) {strecpy(buf,inbeg,inafterend);}


SECTION &SECTANAL::item(int index) {
  int cnt=main.subseccnt();
  xcheckindex(index,cnt);
  return main.finditem(index);
}

int SECTION::subseccnt() {
  int res=0;
  for(int i=0;i<count();i++) res+=items[i].subseccnt();
  return res+items.count;
}
SECTION &SECTION::finditem(int index) {
  for(int i=0;i<count();i++) {
    if (index==0) return items[i];
    int cnt=items[i].subseccnt();
    index--;
    if (index<cnt) return items[i].finditem(index);
    index-=cnt;
  }
}

void SECTION::clear() {
  items.clear();
  vars.clear();
}

void SECTANAL::read(char *buf,const char *specbegin,const char *specend) {
  char pomend[0x100];sprintf(pomend,"%s/",specbegin);
  xcheckstrlen(pomend);
  NOJMPSTRANAL q(buf);
  main.clear();
  main.read(q,specbegin,specend?specend:pomend,true);
}

void SECTION::readsectnameandprops(STRANAL &q,const char *specbegin) {
  q<<specbegin;
  q.readtochar("\n ",name);
  char props[0x400];
  q.readline(props);
  xcheckstrlen(props);
  vars.initfromqstr(props);
}

void SECTION::read(STRANAL &q,const char *specbegin,const char *specend,bool main) {
  outbeg=inbeg=q.act;
  if (not main) {
    readsectnameandprops(q,specbegin);
    inbeg=q.act;
  }
  for(;;) {
    while (q.aredata() and q!=specbegin and q!=specend) q.readline();
    if (not q.aredata()) {
      if (main) {inafterend=q.act;outafterend=q.act;return;}//OK
      throw EAnal("Neukoncena sekce",q.pos());
    }
    if (inafterend==NULL) inafterend=q.act;
    if (q==specend) {//konec sekce
      char n2[0x100];
      q.readline(n2);
      xcheckstrlen(n2);
      if (strcmp(n2,name)) throw EAnal("Ukoncena jina sekce, nez byla zacata",q.pos());
      outafterend=q.act;
      return;
    }
    //tady uz je to jiste vnitrni sekce
    SECTION &sub=items.addnew();
    sub.read(q,specbegin,specend);
  }
}

SECTION &SECTION::operator[] (const char *subsect) {
  int idx=findsection(subsect);
  if (idx<0) return emptysection;
  return items[idx];
}

void SECTION::fillinstrlist(STRLIST &s) {
  NOJMPSTRANAL q(inbeg);
  while (q.act<inafterend) {
    char sp[0x200];
    char *last=q.act;q.readline();
    strecpy(sp,last,q.act);
    xcheckstrlen(sp);
    trimr(sp);
    s+=sp;
  }
}

}

#pragma package(smart_init)
