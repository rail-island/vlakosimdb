#ifndef awslayoutH
#define awslayoutH

#include "awscontrol.h"

namespace aws {

struct Layout {
  ParentControl *owner;
  Layout(ParentControl *owner):owner(owner) {}
  virtual void work()=0;
  virtual void loadprops(XMLNode xml) {}
  virtual void loadchild(XMLNode xml,Control *ctrl) {}
  ///pokud je auto, musi si samo nacist deti
  virtual bool isauto() {return false;}
};

struct Layout1:Layout {
  Layout1(ParentControl *owner):Layout(owner) {}
  void work();
};

struct GridLayout:Layout {
  GridLayout(ParentControl *owner,int cols,int rows):Layout(owner),cols(cols),rows(rows) {}
  GridLayout(ParentControl *owner):Layout(owner) {cols=rows=-1;}
  void work();
  void loadprops(XMLNode xml) {Layout::loadprops(xml);cols=xml.ivar("ly.cols");rows=xml.ivar("ly.rows");}
protected:
  int cols,rows;
};

struct VListLayout:Layout {
  VListLayout(ParentControl *owner):Layout(owner) {sizename="ll.vsize";}
  void loadchild(XMLNode xml,Control *ctrl);
protected:
  void _work(int size);//spocita items.realsize na zaklade items.all,.rel,.realsize
  string sizename;
  struct Item {
    Control *ctrl;
    int size;//velikost(rel:v %)
    bool all;//zda vsechno ostatni(*)
    bool rel;//zda je to relativni(v %)
    int realsize;//pocet vyzadovanych pixelu
  };
  TCntVec<Item> items;
  void work();
protected:
  virtual int getw(Size s) {return s.w;}
  virtual int geth(Size s) {return s.h;}
  virtual int getx(Point p) {return p.x;}
  virtual int gety(Point p) {return p.y;}
  virtual string gethletter() {return "h";}
  virtual void setpos(Control *ctrl,int x,int y,int w,int h) {ctrl->setpos(x,y,w,h);}
};

struct HListLayout:VListLayout {
  HListLayout(ParentControl *owner):VListLayout(owner) {sizename="ll.hsize";}
  virtual int getw(Size s) {return s.h;}
  virtual int geth(Size s) {return s.w;}
  virtual int getx(Point p) {return p.y;}
  virtual int gety(Point p) {return p.x;}
  virtual string gethletter() {return "w";}
  void setpos(Control *ctrl,int x,int y,int w,int h) {ctrl->setpos(y,x,h,w);}
};


/*
class HFlowLayout;

struct FlowElem {
  virtual void work(H)=0;
};

struct FlowAddControl {

};
*/

struct HFlowLayout:Layout {
  HFlowLayout(ParentControl *owner):Layout(owner) {hcenter=vcenter=false;hjump=vjump=0;}
protected:
  virtual STRING gethcenter() {return "ly.hcenter";}
  virtual STRING getvcenter() {return "ly.vcenter";}
  virtual STRING getlefts() {return "ly.left";}
  virtual STRING gettops() {return "ly.top";}
  virtual STRING getrights() {return "ly.right";}
  virtual STRING getbottoms() {return "ly.bottom";}
  void loadprops(XMLNode xml);
  void loadchild(XMLNode xml,Control *ctrl);
  void wrap();
  void work();
  virtual int getw(Size s) {return s.w;}
  virtual int geth(Size s) {return s.h;}
  virtual int getx(Point p) {return p.x;}
  virtual int gety(Point p) {return p.y;}
  virtual void setpos(Control *ctrl,int x,int y) {ctrl->setpos(x,y);}
  bool hcenter,vcenter;
  int hjump,vjump;
private:
  //promenne vyuzivane jen ve work()
//  int actx,basey,maxy;
  int maxw,acty;
  Rect layrec;
  int left,right,top,bottom;
  void setposes();
  struct Line {
    TCntVec<Control*> childs;
    int lineh,linew;
    Line() {lineh=linew=0;}
    bool addctrl(Control *ctrl,HFlowLayout *layout);
    void work(HFlowLayout *layout,int x0);
  };
  friend class Line;
  IObjArray<Line> lines;
  Line *actline;
};

struct AutoLayout:Layout {
  AutoLayout(ParentControl *owner):Layout(owner) {}
  void work();
};

/*
struct VListLayout:ListLayout {
  VListLayout(ParentControl *owner):ListLayout(owner) {sizename="vsize";}
protected:
  void work();
};
*/

/*
struct HListLayout:ListLayout {
  HListLayout(ParentControl *owner):ListLayout(owner) {sizename="hsize";}
protected:
  void work();
};
*/

}

#endif
