#ifndef awstreeH
#define awstreeH

#include "awspaint.h"

namespace aws {

class TreeSource;
typedef RefBase<TreeSource> TreeSrc;

struct TreeSource {
  virtual string gettext()=0;
  virtual int getsubs()=0;
  virtual TreeSrc nodeat(int index)=0;
  virtual bool equalto(TreeSource *right) {return right==this;}
  virtual void addref()=0;
  virtual void release()=0;
};

struct TreeNode {
  virtual TreeSource *getsource()=0;
};

struct NodeClickEvent:NotifyEvent {
  TreeNode *node;
  NodeClickEvent(TreeNode *node):node(node) {}
};

class Tree;

struct NodeImage:TreeNode {
  enum WHAT {leftdown,rightdown};

  int x,y;
  int hi;int wi;
  bool packed;
  Tree *tree;
  NodeImage *parent;
  RefBase<TreeSource> src;
  IObjArray<NodeImage> subs;

  TreeSource *getsource() {return src;}
  void changed();
  Rect getsmallrect();
  int gethi(Canvas &cnv) {if (hi==0) counthi(cnv);return hi;}
  int getwi(Canvas &cnv) {if (wi==0) countwi(cnv);return wi;}
  void counthi(Canvas &cnv);
  void countwi(Canvas &cnv);
  bool mevent(int x,int y,WHAT what);
  void draw(Canvas cnv,int x,int y);
  NodeImage(TreeSource *asrc,Tree *atree,NodeImage *aparent);
  void updatesubs();
  void drawsign(Canvas &cnv);
  void expand();
  void collapse();
  void update();
  NodeImage *getprev();
  NodeImage *getnext(bool canchild=true);
  NodeImage *getlastopened();
};

struct Tree:ScrolledPainter {
  NodeImage *main;
  Tree(Component *owner,System *system);
  ~Tree() {delete main;}
  NodeImage *selected;

  void setsource(TreeSource *src);
  void update();
  void nodetoindex(NodeImage *act,TCntVec<int> &multiindex);
  NodeImage *indextonode(TCntVec<int> &multiindex,bool expand=false);
  bool getactive(TCntVec<int> &multiindex);
  void setactive(TCntVec<int> &multiindex);
protected:
  void onpaint(Canvas &cnv,awsEvent &event);
  void onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  void onmouseup(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  void onmousemove(int x,int y,ShiftState shift,awsEvent &event);
  void onkeydown(awsKey key,awsEvent &event);
  void onchar(char ch,awsEvent &event);
  void donodeclick(NodeImage *node) {NodeClickEvent ev(node);donotify(ev,"onnodeclick",ev_nodeclick.get());}
  void loadprops(XMLNode xml,Form *form);
private:
  EventValue ev_nodeclick;
  bool wantresize;
  int itemhi;
  int signw,signh,signspacing;
  friend class NodeImage;
};

}

#endif
