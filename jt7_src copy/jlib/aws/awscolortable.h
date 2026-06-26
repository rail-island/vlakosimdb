#ifndef awscolortableH
#define awscolortableH

#include <aws/awspaint.h>

namespace aws {


struct ctColorDef {
  int row,col,color;
};

struct ColorTable:PaintControl {
  ColorTable(Component *owner,System *system);

  int getcols() {return hcnt;}
  void setcols(int value) {hcnt=value;if (hcnt<1) hcnt=1;recount();}
  int getrows() {return vcnt;}
  void setrows(int value) {vcnt=value;if (vcnt<1) vcnt=1;recount();}

  void setcolor(int row,int col,int color);
  int getcolor(int row,int col);

  void savecolors(XMLNode xml);
  void loadcolors(XMLNode xml);
protected:
  void onsize(int w,int h);
  void onpaint(Canvas &cnv,awsEvent &event);
  void onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  void loadprops(XMLNode xml,Form *form);
  void savedata(XMLNode xml);
  void loaddata(XMLNode xml);
private:
  TCntVec<ctColorDef> colors;
  int hcnt,vcnt;
  float xsqsize,ysqsize;
  void recount();
  EventValue ev_select;

  void drawcrect(Canvas cnv,int x,int y,int w,int h,bool pushed,int color);
};



}

#endif
