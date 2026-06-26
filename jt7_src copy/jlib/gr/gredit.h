#ifndef greditH
#define greditH

#include "comcnv.h"

namespace gr {

const int ss_shift=1,ss_ctrl=2,ss_left=4,ss_right=8,ss_alt=0x10;

class GREditState;
struct GREditor {
  enum CURSOR {normal,text,cross,hand};
  virtual void repaint()=0;
  virtual void resize(int w,int h)=0;
  virtual void setstate(GREditState *state)=0;
  virtual void setcursor(CURSOR cursor)=0;
};

struct GREditState {
  enum BUTTON {left,middle,right};
  virtual void mdown(BUTTON but,GREditor *sender,int x,int y,int shift) {}
  virtual void mmove(GREditor *sender,int x,int y,int shift) {}
  virtual void mup(BUTTON but,GREditor *sender,int x,int y,int shift) {}

  virtual void repaint(GREditor *sender,CANVAS &cnv)=0;
  virtual void init(GREditor *sender) {}
  virtual void done(GREditor *sender) {}
};

}

#endif
