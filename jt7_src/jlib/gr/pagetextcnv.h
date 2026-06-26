#ifndef pagetextcnvH
#define pagetextcnvH

#include "textcnv.h"
#include "metacnv.h"

namespace gr {
using namespace jlib;

struct PAGETEXTCNV:TEXTCANVAS {
  PAGETEXTCNV(const CANVAS *atarget,int apgwi,int apghi);
  METACANVAS buflines;//nekolik radku za sebou,ktere musi byt na stejne strance
  METACANVAS page;
  int buftop;

  int pghi;

  void _newline(int alinehi,int dy);
  void done() {newline();flushbuf();if (buftop>0) newpage();destroyed=true;}
  virtual void newpage() {page.clear();buftop=0;}//vetsinou se stranka jeste nekam ulozi

  void flushbuf();
};

}
#endif
