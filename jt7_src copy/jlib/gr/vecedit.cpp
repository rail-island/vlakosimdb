#include <jlib.h>
#pragma hdrstop

#include "vecedit.h"
#include <jmath/analgeom.cpp>

namespace gr {
using namespace jlib;


bool VECEDIT::setbackground(VECOBJ *abg,const ABREC &r) {
  background=NULL;
  if (abg==NULL) return false;
  ABREC src;
  if (not abg->size(src)) return false;
  sc->eqfromrect(src,r);
  background=abg;
  return true;
}

void VECEDIT::selectobj(int index,STATE newstate) {
  cancel();
  if (index>=0 and index<obj->elems.count) actelem=obj->elems[index];else actelem=NULL;
  state=actelem?newstate:none;
  if (state==editaddpoint) {
    actelem->addpoint(XYREC(logmousex,logmousey));
  }
}

void VECEDIT::cancel() {
  if (state==addpoint) {
    obj->elems+=actelem;
    actelem->deletelastpoint();state=none;actelem=NULL;
  } else if (state==point1 or state==point2 or state==point0) {
    delete actelem;actelem=NULL;state=none;
  } else if (state==editaddpoint) {
    actelem->deletelastpoint();state=none;actelem=NULL;
  }
  actelem=NULL,state=none;
}

bool VECEDIT::mousedown(int mx,int my) {
  mousemoved(mx,my);
  if (state==point1) {
    actelem->setpoint(0,XYREC(logmousex,logmousey));
    state=point2;return false;
  } else
  if (state==point0) {
    actelem->addpoint(XYREC(logmousex,logmousey));
    actelem->addpoint(XYREC(logmousex,logmousey));
    state=addpoint;return false;
  } else
  if (state==addpoint) {
    actelem->addpoint(XYREC(logmousex,logmousey));
    return false;
  } else
  if (state==editaddpoint) {
    actelem->addpoint(XYREC(logmousex,logmousey));
    return false;
  } else
  if (state==edit) {
    editptidx=nearpoint(mousex,mousey,actelem);
    if (editptidx>=0) {
      state=movepoint;
    }
    return false;
  } else
  if (state==erasepoint) {
    editptidx=nearpoint(mousex,mousey,actelem);
    if (editptidx>=0) actelem->deletepoint(editptidx);
  } else
  if (state==insertpoint) {
    int lineidx=nearline(mousex,mousey,actelem);
    if (lineidx>=0) {
      actelem->insertpoint(lineidx,XYREC(logmousex,logmousey));
    }
  }
  return false;
}

void VECEDIT::mousemoved(int mx,int my) {
  mousex=mx;mousey=my;
  logmousex=sc->xptol(mousex);logmousey=sc->yptol(mousey);
  logmousex-=jmpx/2;
  logmousey-=jmpy/2;
  while (logmousex%jmpx) logmousex++;
  while (logmousey%jmpy) logmousey++;
  
  mousex=sc->ltop(XYREC(logmousex,logmousey)).x;
  mousey=sc->ltop(XYREC(logmousex,logmousey)).y;
}

bool VECEDIT::mouseup(int mx,int my) {
  mousemoved(mx,my);
  if (state==point2) {
    actelem->setpoint(1,XYREC(logmousex,logmousey));
    state=none;
    obj->elems+=actelem;actelem=NULL;return true;
  } else
  if (state==movepoint) {
    actelem->setpoint(editptidx,XYREC(logmousex,logmousey));
    state=edit;return false;
  }
  return false;
}

void VECEDIT::draw() {
  if (obj==NULL) return;

  if (background) background->draw(*sc);
  obj->draw(*sc,state==movepoint or state==editaddpoint?actelem:NULL);

  switch (state) {
    case none:break;
    case point0:case point1:drawcross(logmousex,logmousey);break;
    case point2:actelem->setpoint(1,XYREC(logmousex,logmousey));actelem->edraw(*sc);break;
    case addpoint:case editaddpoint:actelem->setlastpoint(XYREC(logmousex,logmousey));actelem->edraw(*sc);break;
    case view:drawpts(actelem);break;
    case movepoint:actelem->setpoint(editptidx,XYREC(logmousex,logmousey));actelem->edraw(*sc);drawpts(actelem);break;
    case edit:case erasepoint:drawcross(logmousex,logmousey);drawpts(actelem);break;
    case insertpoint:drawcross(logmousex,logmousey);drawpts2(actelem);break;
  }
}

void VECEDIT::newobj(VECELEM *elem,STATE newstate) {
  actelem=elem;
  state=newstate;
}

inline float sqr(float arg) {return arg*arg;}

int VECEDIT::nearpoint(int x,int y,VECELEM *elem) {
  float mindist;
  int minidx=-1;
  for(int i=0;i<elem->pointcount();i++) {
    XYREC p=sc->ltop(elem->point(i));
    float actd=sqrt(sqr(p.x-x)+sqr(p.y-y));
    if (minidx<0 or actd<mindist) minidx=i,mindist=actd;
  }
  return mindist<=ptmaxdist?minidx:-1;
}

void VECEDIT::drawpt(const XYREC &p,LOGICCOLOR color) {
  XYREC phys=sc->ltop(p);
  sc->getparent().fg(color).penxor().dEll(phys.x-ptsize/2,phys.y-ptsize/2,ptsize,ptsize).pencopy();
}

void VECEDIT::drawpts(VECELEM *elem) {
  int n=nearpoint(mousex,mousey,elem);
  for(int i=0;i<elem->pointcount();i++) {
    drawpt(elem->point(i),i==n?lcgreen:lcred);
  }
}

int VECEDIT::nearline(int x,int y,VECELEM *elem) {
  int minidx=-1;float mind=0;
  int cnt=elem->pointcount();
  for(int i=0;i<cnt;i++) {
    jmath::agline<int> line(sc->ltop(elem->point(i)),sc->ltop(elem->point((i+1)%cnt)));
    float actd=line.pdistfromline(jmath::agpoint<int>(x,y));
    if (minidx<0 or actd<mind) mind=actd,minidx=i;
  }
  return minidx;
}

void VECEDIT::drawpts2(VECELEM *elem) {
  int minidx=nearline(mousex,mousey,elem);
  int cnt=elem->pointcount();
  for(int i=0;i<cnt;i++) {
    LOGICCOLOR color=lclred;
    if (minidx>=0 and (i==minidx or i==(minidx+1)%cnt)) color=lcgreen;
    drawpt(elem->point(i),color);
  }
}


}
#pragma package(smart_init)
