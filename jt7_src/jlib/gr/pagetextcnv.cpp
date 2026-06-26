#include <jlib.h>
#pragma hdrstop

#include "pagetextcnv.h"
#include "subcnv.h"

namespace gr {
using namespace jlib;

PAGETEXTCNV::PAGETEXTCNV(const CANVAS *atarget,int apgwi,int apghi):TEXTCANVAS(apgwi) {
  target=atarget;pghi=apghi;
  buftop=0;
}

void PAGETEXTCNV::flushbuf() {
  newline();
  if (buftop+acty<=pghi or buftop==0) {//buftop==0:neda se nic delat, bude precuhovat ze stranky
    buflines.draw(SUBCANVAS(page,0,buftop));
    buflines.clear();
    buftop+=acty;acty=0;
  } else {
    newpage();flushbuf();
  }
}

void PAGETEXTCNV::_newline(int alinehi,int dy) {
  addcurregion();
  if (acty+alinehi<=pghi) {
    actline.draw(SUBCANVAS(buflines,0,acty+dy));
    acty+=alinehi;
  } else {
    flushbuf();
    actline.draw(SUBCANVAS(buflines,0,acty+dy));
    acty+=alinehi;
  }
  initnewline();
}


}

#pragma package(smart_init)
