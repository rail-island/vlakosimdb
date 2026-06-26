#include <jlib.h>
#pragma hdrstop

#include "icab.h"

namespace jtyc {

CabCircle::CabCircle(XMLNode xml,PersistentObject *data) {
  startangle=xml.fvar("startangle");
  endangle=xml.fvar("endangle");
  root=XMLPath(xml.svar("root"));
  startvalue=xml.fvar("startvalue");
  endvalue=xml.fvar("endvalue");
  x=xml.fvar("x");
  y=xml.fvar("y");
  radius=xml.fvar("radius");
  wi=xml.fvar("wi");
  if (data->hasfile(xml.svar("file"))) tex=texman->tryfindtexture("pack:"+xml.svar("file"),data);
  if (tex) tex->settransparent(true);
}

void CabCircle::render(RenderContext &rc,XMLNode xml) {
  float value=xml.getvalue(root).fvalue();
  float vs=endvalue-startvalue;
  if (endvalue>startvalue) {
    if (value<startvalue) value=startvalue;
    if (value>endvalue) value=endvalue;
  }
  if (endvalue<startvalue) {
    if (value>startvalue) value=startvalue;
    if (value<endvalue) value=endvalue;
  }
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(x,y,0);
  if (fabs(vs)>0.001) glRotatef(startangle+(endangle-startangle)/vs*(value-startvalue),0,0,1);
  if (tex) tex->bind(rc.ctxid);
  glColor4f(1,1,1,1);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);glVertex2f(-radius,-wi/2);
  glTexCoord2f(1,0);glVertex2f(+radius,-wi/2);
  glTexCoord2f(1,1);glVertex2f(+radius,+wi/2);
  glTexCoord2f(0,1);glVertex2f(-radius,+wi/2);
  glEnd();
  glPopMatrix();
}

bool jtCab::bind(RenderContext &rc,Texture *tex) {
  if (tex) tex->bind(rc.ctxid);
  return tex;
}

void jtCab::load(PersistentObject *data,XMLNode xml) {
  name=xml.svar("name");
  title=lang_text_ch(xml.svar("title"));
  isdefault=xml.ivar("default");
  if (data->hasfile(xml.svar("file"))) bg=texman->tryfindtexture("pack:"+xml.svar("file"),data);
  if (bg) bg->settransparent(true);
  if (xml.svar("hi")!="") hi=xml.fvar("hi");
  fovy=xml.fvar("fovy");
  if (fovy<1) fovy=1;
  if (bg) {
    x=xml.fvar("x")/bg->getw();
    y=xml.fvar("y")/bg->geth();
    w=xml.fvar("w")/bg->getw();
    h=xml.fvar("h")/bg->geth();
  }
  XMLNode its=xml["items"];
  for(int i=0;i<its.count();i++) {
    XMLNode it=its[i];
    if (it.getname()=="circle") items.add(new CabCircle(it,data));
  }
}

void jtCab::render(RenderContext &rc,XMLNode xml) {
  glAlphaFunc(GL_GEQUAL,0.5);
  rc.push();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  rc.settexture2d(true);
  rc.setdepthtest(false);
  rc.setalphatest(true);
  if (bind(rc,bg)) {
    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);glVertex2f(-1,-1);
    glTexCoord2f(1,0);glVertex2f(1,-1);
    glTexCoord2f(1,1);glVertex2f(1,1);
    glTexCoord2f(0,1);glVertex2f(-1,1);
    glEnd();
  }
  for(int i=0;i<items.count;i++) items[i]->render(rc,xml);
  glColor4f(1,1,1,1);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  rc.pop();
}


}

