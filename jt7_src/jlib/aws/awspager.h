#ifndef awspagerH
#define awspagerH

#include <aws/awspaint.h>

namespace aws {

struct awsPager;


struct awsPageSheet:Place {
  awsPager *pager;
  string text;
  int textwi,wi;
  awsPageSheet(awsPager *apager,System *system);
  void loadprops(XMLNode xml,Form *form);
  void notifyremove(Component *obj);
};

struct awsHeader:PaintControl {
  awsPager *pager;

  awsHeader(awsPager *apager,System *system);
  void onmouseup(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  void onpaint(Canvas &cnv,awsEvent &event);
  void notifyremove(Component *obj);
};

struct awsPager:Place {
  int getcur() {return cur;}
  void setcur(int value) {selectpage(value);}
  awsPager(Component *owner,System *system);
protected:
  void drawhdr(Canvas cnv,Size s);
  void loadprops(XMLNode xml,Form *form);
  void selectpage(int page);
  void notifyremove(Component *obj);
  void onsize(int w,int h);

  int alignleft,alignright;
  int hdrsize;
  awsHeader *hdr;
  TCntVec<awsPageSheet*> sheets;
  int cur;

  friend class awsHeader;
  friend class awsPageSheet;
};

}

#endif
