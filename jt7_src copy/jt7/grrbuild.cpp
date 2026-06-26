#include <jlib.h>
#pragma hdrstop

#include "grrbuild.h"
#include "gview.h"
#include "gameu.h"
#include "ghimap.h"

namespace jtyc {

string RRBuilder::getmode() {
  switch (mode) {
    case bt_change:return "change";
    case bt_fixed:return "fixed";
    case bt_direct:return "direct";
  }
  return "";
}

void RRBuilder::setmode(string mode) {
  this->mode=bt_change;
  if (mode=="change") this->mode=bt_change;
  if (mode=="fixed") this->mode=bt_fixed;
  if (mode=="direct") this->mode=bt_direct;
}

void RRBuilder::buildrr(int x,int y,Spojnice rr) {
  max_tt_leap=(mode==bt_change)?fabs(dhi):0.4;
  rr_build_mhd=1;
  map->build(lastx,lasty,rr,dhi,mode);
}

void ComRBuilder::buildstep(int x,int y) {
  int dx=x-lastx,dy=y-lasty;
  Vrchol v(dx,dy);
  Spojnice sp=Spojnice(lastin,v);
  if (sp.valid()) buildrr(lastx,lasty,sp);
  else if (autofirstandlast) {
    sp=Spojnice(v,v.direct());
    buildrr(lastx,lasty,sp);
  }
  lastin=v.direct();
  lastx=x;lasty=y;
}
void ComRBuilder::erased() {
  Spojnice sp=Spojnice(lastin,lastin.direct());
  if (autofirstandlast && sp.valid()) buildrr(lastx,lasty,sp);
}
ComRBuilder::~ComRBuilder() {
}


void ComRBuilder::buildto(int x,int y) {
  int actx=lastx,acty=lasty;
  while (actx!=x || acty!=y) {
    if (actx<x) actx++;else if (actx>x) actx--;
    if (acty<y) acty++;else if (acty>y) acty--;
    buildstep(actx,acty);
  }
}


}


