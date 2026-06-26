#include <jlib.h>
#pragma hdrstop

#include "pager.h"

namespace gr {
using namespace jlib;

template<class PAGE> PAGE A4DISTRIBUTION<PAGE>::getpage(int sheet,int x,int y,int side) {
  xcheckindex(sheet,shcnt);
  xcheckindex(x,hcnt);
  xcheckindex(y,vcnt);
  xcheckindex(side,2);
  int v;
  if (side) v=lpersh*sheet+y*hcnt*2+x*2+side;else v=lpersh*sheet+y*hcnt*2+(hcnt-x-1)*2+side;
  xcheckindex(v,virtlogcnt);
  return buf[v];
}

template<class PAGE> inline PAGE _page(int index,Array<PAGE> &pages,int begcnt,PAGE nopage) {
  if (index<begcnt) return nopage;
  if (index<pages.count+begcnt) return pages[index-begcnt];
  return nopage;
}

template<class PAGE> A4DISTRIBUTION<PAGE>::A4DISTRIBUTION(int ahcnt,int avcnt,Array<PAGE> &pages,PAGE nopage,int nopagesatbegin) {
//#define page(_index_) ((_index_)<pages.count?pages[_index_]:nopage)
#define page(_index_) (_page(_index_,pages,begcnt,nopage))
  begcnt=nopagesatbegin;
  pgcnt=pages.count;
  hcnt=ahcnt;vcnt=avcnt;
  xassert(hcnt);xassert(vcnt);
  lpersh=2*vcnt*hcnt;
  virtlogcnt=pages.count+begcnt;
  while (virtlogcnt%lpersh) virtlogcnt++;//virtualni pocet logickych stranek
  shcnt=virtlogcnt/lpersh;
  buf=new PAGE[virtlogcnt];
  if (hcnt%2==1) {//liche horizontalne
    for(int i=0;i<virtlogcnt;i++) buf[i]=page(i);
  } else {//sude horizontalne
    PAGE *act=buf;
    for(int i=0;i<virtlogcnt/2;i+=2) {
      *act++=page(i);
      *act++=page(i+1);
      *act++=page(virtlogcnt-i-1);
      *act++=page(virtlogcnt-i-2);
    }
  }
#undef page
}

}
#pragma package(smart_init)
