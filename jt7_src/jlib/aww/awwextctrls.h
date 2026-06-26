#ifndef awwextctrlsH
#define awwextctrlsH

#include "awwcontrol.h"
#include "commctrl.h"
#include "awwstdctrls.h"
#include "awwpaint.h"

namespace aww {

struct ScrollBoxBase {
  virtual void hscroll(int dx)=0;
  virtual void vscroll(int dy)=0;
};

class awwRadioGroup;
struct awwRadioButton:awwTextControl {
  awwRadioButton(awwRadioGroup *group,RadioGroup *rg):awwTextControl(rg),group(group) {ismousedown=false;}
  awwRadioGroup *group;
private:
  void _repairsize(int neww,int newh) {}
  bool ismousedown;
protected:
  void createparams(CreateParams &pars);
  void onmousedown(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift);
  void onmouseup(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift);
};

struct awwRadioGroup:awwGroupBox {
  awwRadioGroup(RadioGroup *group):awwGroupBox(group),group(group) {
    pushedindex=-1;
  }
  string getitems() {return items;}
  void setitems(string aitems) {items=aitems;updateitems();}
  string items;
  TCntVec<awwRadioButton*> buts;
//  IObjArray<awwRadioButton> buts;
  int getcur() {return pushedindex;}
  void setcur(int index);
  void ondestroywnd();
  void updateitems();
  void onsize(awwMessage &msg,int w,int h) {resized();}
  void resized();
  RadioGroup *group;
  void createwindow();
  void pressed(awwRadioButton *but);
  Size getbestsize();
  int pushedindex;//default=-1
};

class awwScrollBox;
struct _ScrollBar {
//  awwScrollBox *box;
  ScrollBoxBase *box;
  awwControl *boxc;
  int pos;bool hor;
  int page;
  int sum;
  int koef;
  int barcode;
  int dpos;
  HWND hwnd;
  _ScrollBar(ScrollBoxBase *box,bool hor):box(box),hor(hor) {hwnd=NULL;barcode=hor?SB_HORZ:SB_VERT;pos=0;page=0;boxc=dynamic_cast<awwControl*>(box);dpos=20;koef=1;}
//  _ScrollBar(bool hor):hor(hor) {hwnd=NULL;barcode=hor?SB_HORZ:SB_VERT;pos=0;page=0;}
  bool scroll(int pos);
  void onscroll(awwMessage &msg,int code,int apos,HWND hscroll);
  void setrange(int page,int sum);
};

struct awwScrollBox:awwControl,ScrollBoxBase {
  awwScrollBox(ScrollBox *scrollbox):awwControl(scrollbox) {hor=new _ScrollBar(this,true);ver=new _ScrollBar(this,false);inw=inh=0;denyerasebkgnd=false;isflat=false;sb=scrollbox;}
  ~awwScrollBox() {delete hor;delete ver;}
protected:
  friend class _ScrollBar;
  ScrollBox *sb;
  int inw,inh;
  bool denyerasebkgnd,isflat;
  _ScrollBar *hor,*ver;
  void setscrollpos(Point pt);
  Point getscrollpos();
  void createparams(CreateParams &pars);
  void destroywnd() {awwControl::destroywnd();hor->hwnd=ver->hwnd=NULL;}
  void createwindow();
  void onhscroll(awwMessage &msg,int code,int pos,HWND scroll) {hor->onscroll(msg,code,pos,scroll);}
  void onvscroll(awwMessage &msg,int code,int pos,HWND scroll) {ver->onscroll(msg,code,pos,scroll);}
  void onsize(awwMessage &msg,int w,int h);
  void countbounds();
  void setbounds();
  bool scrollinview(const Rect &r);
  void onerasebkgnd(awwMessage &msg);
  void resizedchild() {countbounds();setbounds();}
  void setsdp(string name,string value);

  void hscroll(int dx);
  void vscroll(int dy);
};

struct awwScrolledPainter:awwPaintControl,ScrollBoxBase {
  awwScrolledPainter(ScrolledPainter *scrollbox);
  ~awwScrolledPainter() {delete hor;delete ver;}
protected:
  friend class _ScrollBar;
  ScrolledPainter *sb;
  bool isflat;
  _ScrollBar *hor,*ver;
  int inw,inh;
  int caretx,carety,caretsize;
  int wincarethi;

  void setscrollpos(Point pt);
  Point getscrollpos();
  Size getinsize() {return Size(inw,inh);}
  void setinsize(Size insize) {inw=insize.w;inh=insize.h;setbounds();}
  void createparams(CreateParams &pars);
  void destroywnd() {awwControl::destroywnd();hor->hwnd=ver->hwnd=NULL;}
  void createwindow();
  void onhscroll(awwMessage &msg,int code,int pos,HWND scroll) {hor->onscroll(msg,code,pos,scroll);}
  void onvscroll(awwMessage &msg,int code,int pos,HWND scroll) {ver->onscroll(msg,code,pos,scroll);}
  override void onsetfocus(awwMessage &msg);
  override void onkillfocus(awwMessage &msg);
  void onsize(awwMessage &msg,int w,int h);
//  void countbounds();
  void setbounds();
  bool scrollinview(const Rect &r);
  void onerasebkgnd(awwMessage &msg);
  void resizedchild() {setbounds();}
  Size getclientsize();
  void updatedblbuf();
  void onmousewheel(awwMessage &msg,int x,int y,int delta,ShiftState shift);
//  void setsdp(string name,string value);
//  void repaint(const Rect &r);
//  void repaint();
  void setdoublebuffer(int value) {awwPaintControl::setdoublebuffer(value>0?1:0);}
  override int getcnvdx() {return -hor->pos;}
  override int getcnvdy() {return -ver->pos;}
  override int getmousedx() {return hor->pos;}
  override int getmousedy() {return ver->pos;}
  override Point getcaretpos() {return Point(caretx,carety);}
  override int getcaretsize() {return caretsize;}
  override void setcaretpos(Point value) {caretx=value.x;carety=value.y;updatecaretpos();}
  override void setcaretsize(int value) {caretsize=value;updatecaret();}
  void updatecaret();
  void updatecaretpos();
  void hscroll(int dx);
  void vscroll(int dy);
  void onpaint(awwMessage &msg);
};

}

#endif
