#ifndef fontsH
#define fontsH

#include "comgr.h"
#include <ext/resman.h>

namespace gr {

const int fsbold=0x1,fsitalic=0x2,fsunderline=0x4,fsstrikeout=0x8;

struct FONTSIZES {
  int wis[0x100];
  int hi;
  int textwi(const char *text);
};

struct FONT {
  void init() {zerorec(*this);*name=0;height=10;width=0;style=0;color=lcblack;hashcode=0;}
  FONT() {init();}
  FONT(const char *aname,int astyle,LOGICCOLOR acolor,int aheight,int awidth);
  void counthash();

  char name[0x20];
  int height,width,style;
  LOGICCOLOR color;
  int hashcode;
};

inline bool operator ==(const FONT &r1,const FONT &r2) {
  return eqstr(r1.name,r2.name) and r1.height==r2.height and r1.width==r2.width and r1.style==r2.style and r1.color==r2.color;
}

struct FONTRESADAPTER:VECTORRESOURCEBASE<void*> {
  void allochandle(const FONT &data,void *&handle) {handle=createfont(data);}
  void freehandle(void *handle) {freefont(handle);}
  bool equalresources(const FONT &r1,const FONT &r2) {return r1==r2;}
  int gethashcode(const FONT &data) {return data.hashcode;}
  virtual void *createfont(const FONT &data)=0;
  virtual void freefont(void *handle)=0;
};

struct FONTRESOURCE:RESOURCEMANAGER<FONT,void *,FONTRESADAPTER> {
};

inline BINWRITE &operator <<(BINWRITE &fw,const FONT &fnt) {
  fw.wr(fnt);
//  fw<<fnt.name<<fnt.style<<fnt.color<<fnt.height<<fnt.width<<fnt.hashcode;
  return fw;
}
inline BINREAD &operator >>(BINREAD &fr,FONT &fnt) {
  fr.rr(fnt);
//  fr>>fnt.name>>fnt.style>>fnt.color>>fnt.height>>fnt.width>>fnt.hashcode;
  return fr;
}


}

#endif
