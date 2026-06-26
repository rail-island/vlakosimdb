#include <jlib.h>
#pragma hdrstop

#include "glofont.h"

namespace glo {

void gloFont::load(XMLNode xml,string bmpfile) {
  tex=texman->findtexture("file:"+bmpfile,NULL);
  int w=xml.ivar("w"),h=xml.ivar("h");
  if (!w || !h) return;
  for(int i=0;i<xml.count();i++) {
    unsigned char chi=*xml[i].cvar("ch");
//    int chi=xml[i].ivar("ch");
//    if (chi<0 || chi>=0x100) continue;
    gloChar &ch=chars[chi];
    ch.x=xml[i].fvar("x");
    ch.y=xml[i].fvar("y");
    ch.w=xml[i].fvar("w");
    ch.h=xml[i].fvar("h");
    ch.x/=w;ch.w/=w;
    ch.y/=h;ch.h/=h;
  }
}
void gloFont::render(float x,float y,string text,RenderContext &rc) {
  tex->bind(rc.ctxid);
  for(const char *c=text.c_str();*c;c++) {
    gloChar &ch=chars[(unsigned char)*c];
    glBegin(GL_QUADS);
    glTexCoord2f(ch.x,1-ch.y);glVertex2f(x,y);
    glTexCoord2f(ch.x+ch.w,1-ch.y);glVertex2f(x+ch.w,y);
    glTexCoord2f(ch.x+ch.w,1-ch.y-ch.h);glVertex2f(x+ch.w,y-ch.h);
    glTexCoord2f(ch.x,1-ch.y-ch.h);glVertex2f(x,y-ch.h);
    glEnd();
    x+=ch.w;
  }
}
float gloFont::textwi(string text) {
  float res=0;
  for(const char *c=text.c_str();*c;c++) {
    gloChar &ch=chars[(unsigned char)*c];
    res+=ch.w;
  }
  return res;
}
float gloFont::texthi(string text) {
  return chars['0'].h;
}

}

