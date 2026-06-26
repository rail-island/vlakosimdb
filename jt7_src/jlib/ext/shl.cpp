#include <jlib.h>
#pragma hdrstop

#include "shl.h"

namespace jlib {

SHLWRITER::SHLWRITER(EDITOR *aedit) {
  edit=aedit;
  actcol=0;
  actline=0;
  left=edit->getleft();
  top=edit->gettop();
  wi=edit->getwi();
  hi=edit->gethi();
}

void SHLWRITER::writes(const char *c,EDITOR::ATTR a) {
  if (visline()) while (*c) lineokwritech(*c,a),c++; 
}

}

#pragma package(smart_init)
