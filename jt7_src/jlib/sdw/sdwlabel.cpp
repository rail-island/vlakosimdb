#include <jlib.h>
#pragma hdrstop

#include "sdwlabel.h"
#include "sdwpaint.h"

namespace sdw {

sdwLabel::sdwLabel(Label *ctrl,sdwSystem *sys):sdwControl(ctrl,sys) {
  label=ctrl;
}

void sdwLabel::updatesize() {
  w=getfont()->getwidth(text);
  h=getfont()->getheight();
  changedpos();
}

void sdwLabel::draw(sdwCanvas canvas) {
  canvas.text(0,0,text,getfont()->getcolor(),font);
}

sdwWrapLabel::sdwWrapLabel(WrapLabel *ctrl,sdwSystem *sys):sdwControl(ctrl,sys) {
  label=ctrl;
}

void sdwWrapLabel::draw(sdwCanvas canvas) {
  canvas.text(0,0,text,getfont()->getcolor(),font);
/*  const char *c=text.c_str();
  bstring str;
  int hi=getfont->getheight(),y=0;
  while (*c) {
    if (*c=='\n') canvas.text(0,y,str),y+=hi,str.clear(),c++;
    else {
      bstring wrd;
      while (*c && *c!=' ' && *c!='\n')
    }
    else if (*c==' '
    str<<*c++;
  }*/
}


}

