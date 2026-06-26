#ifndef shlH
#define shlH

#include "editor.h"

namespace jlib {

struct SHLWRITER {
  EDITOR *edit;
  int actcol,actline;
  int left,top,wi,hi;
  SHLWRITER(EDITOR *aedit);
  void lf() {actline++;actcol=0;}
  bool visline() {return actline>=top and actline<top+hi;}
  bool viscol() {return actcol>=left and actcol<left+wi;}
  bool vis() {return visline() and viscol();}

  void writech(char ch,EDITOR::ATTR a) {if (vis()) _writech(ch,a);actcol++;}
  void lineokwritech(char ch,EDITOR::ATTR a) {if (viscol()) _writech(ch,a);actcol++;}
  void writes(const char *s,EDITOR::ATTR a);
  char *getline() {return actline<edit->lines.count?edit->lines[actline]:NULL;}
protected:
  void _writech(char ch,EDITOR::ATTR a) {edit->scr(actcol-left,actline-top)=EDITOR::ELEM(ch,a);}
};

}

#endif
