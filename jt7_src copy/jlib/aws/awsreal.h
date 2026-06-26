#ifndef awsrealH
#define awsrealH

#include "awscontrol.h"
#include "awswindow.h"

namespace aws {

struct RealControl {
  virtual ~RealControl() {}
  virtual void notifyremove(Component *obj) {}
  virtual void show() {}
  virtual void hide() {}
  virtual bool isvisible() {return false;}
//  virtual void setpos(Point pos)=0;
//  virtual void setsize(Size size)=0;
  virtual void setpos(Point pos,Size size) {}
  virtual Point getpos() {return Point();}
  virtual Point getscreenpos() {return Point();}
  virtual Size getsize() {return Size();}
  virtual void setparent(ParentControl *parent) {}
  virtual ParentControl *getparent() {return NULL;}
  virtual Font *getfont() {return NULL;}
  virtual void setfont(Font *font) {}
  virtual void beginupdate() {}
  virtual bool wantanswer() {return true;}
  virtual void endupdate() {}
  virtual Rect getclient() {return Rect();}
  virtual void getmargins(Margins &m) {m.left=m.right=m.top=m.bottom=0;}
  virtual Size getbestsize() {return getsize();}
  property_a(hint,string)
  property_a(cursor,Cursor)

  virtual void setsdp(string name,string value) {}
  virtual string getsdp(string name) {return "";}
  virtual bool supportsdp(string name) {return false;}
  virtual void setfocus() {}

  property_a(visible,bool)
  property_a(enabled,bool)
};

struct RealChildControl:RealControl {
};

struct RealParentControl:RealControl {
  virtual void getchilds(TCntVec<Control*> &childs) {}
};

struct RealWindow:RealParentControl {
  property_a(state,WindowState)
  property_a(position,WindowPosition)
  property_a(text,string)
  property_a(clientsize,Size)
  property_a(style,string)
//  virtual void setstate(WindowState state)=0;
//  virtual WindowState getstate()=0;
//  virtual void setposition(WindowPosition position)=0;
//  virtual WindowPosition getposition()=0;

//  virtual string gettext()=0;
//  virtual void settext(string text)=0;
  virtual void showmodal() {xabstract;}
  virtual void close() {xabstract;}
  virtual void denyclose() {xabstract;}
  virtual void setmenu(MenuItem *menu) {xabstract;}
};

//struct RealTextControl:RealChildControl {
//  virtual void settext(string text)=0;
//  virtual string gettext()=0;
//};

struct RealEdit:RealChildControl {
  property_a(text,string)
  property_a(modified,bool)
  virtual void selectall() {}
};

struct RealListControl:RealChildControl {
  property_a(items,string)
  property_a(cur,int)
/*  virtual void setitems(string items)=0;
  virtual string getitems()=0;
  virtual void setcur(int index)=0;
  virtual int getcur()=0;*/
};

struct RealListBox:RealListControl {
  property_a(multiselect,bool)
  property_a(selected,string)
};
struct RealPackedList:RealListControl {
  property_a(dropdowncount,int)
//  virtual int getdropdowncount()=0;
//  virtual void setdropdowncount(int n)=0;
};

struct RealButton:RealChildControl {
  property_a(text,string)
};

struct RealPlace:RealParentControl {
};

struct RealPageSheet:RealParentControl {
  property_a(text,string)
};

struct RealPageControl:RealChildControl {
//  virtual PageSheet *add()=0;
  property_a(cur,int)
  virtual void getpages(TCntVec<PageSheet*> &pages) {xabstract;}
};

struct RealRadioGroup:RealChildControl {
  property_a(items,string)
  property_a(text,string)
  property_a(cur,int)
//  virtual void setitems(string items)=0;
//  virtual string getitems()=0;
};

struct RealLabel:RealChildControl {
  property_a(text,string)
};

struct RealMemo:RealChildControl {
  property_a(text,string)
  property_a(cur,int)
  property_a(modified,bool)
};

struct RealWrapLabel:RealChildControl {
  property_a(text,string)
};

struct RealCheckBox:RealChildControl {
  property_a(text,string)
  property_a(checked,bool)
//  virtual bool getchecked()=0;
//  virtual void setchecked(bool checked)=0;
};

struct RealGroupBox:RealParentControl {
  property_a(text,string)
//  virtual void settext(string text)=0;
//  virtual string gettext()=0;
};

struct RealScrollBox:RealParentControl {
  property_a(scrollpos,Point)
  virtual bool scrollinview(const Rect &r) {xabstract;}
};

struct RealPaintControl:RealParentControl {
  virtual void repaint(const Rect &r) {}
  virtual void repaint() {}
  property_a(doublebuffer,int)
  property_a(erasebkgnd,bool)
};

struct RealTrackBar:RealChildControl {
  property_a(min,int)
  property_a(max,int)
  property_a(cur,int)
  property_a(frequency,int)
  property_a(vertical,bool)
};

struct RealScrolledPainter:RealChildControl {
  virtual void repaint(const Rect &r) {}
  virtual void repaint() {}
  virtual bool scrollinview(const Rect &r) {xabstract;}
  property_a(scrollpos,Point)
  property_a(insize,Size)
  property_a(doublebuffer,int)
  property_a(caretpos,Point)
  property_a(caretsize,int)
};

struct RealMenuItem {
  property_a(text,string)
  virtual void addto(MenuItem *parent) {}
};

struct RealComplex:RealListBox,RealPackedList,RealEdit,RealLabel,RealGroupBox,RealRadioGroup,
RealCheckBox,RealPageSheet,RealPageControl,RealPlace,RealButton,RealWindow,RealScrollBox,
RealPaintControl,RealMemo,RealWrapLabel,RealScrolledPainter,RealTrackBar {
};


}

#endif
