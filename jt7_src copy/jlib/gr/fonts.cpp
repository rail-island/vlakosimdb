#include <jlib.h>
#pragma hdrstop

#include "fonts.h"

namespace gr {

FONT::FONT(const char *aname,int astyle,LOGICCOLOR acolor,int aheight,int awidth) {
  zerorec(*this);
  strcpy(name,aname);
  color=acolor;
  style=astyle;
  height=aheight;
  width=awidth;
  counthash();
}

void FONT::counthash() {
  register int res=0;
  for(char *c=name;*c;c++) res<<=3,res^=*c;
  res^=height;
  res^=width;
  res^=style;
  res^=color.l;
  hashcode=res;
}

int FONTSIZES::textwi(const char *text) {
  int res=0;
  for(uchar *c=(uchar*)text;*c;c++) res+=wis[*c];
  return res;
}

}


#pragma package(smart_init)
