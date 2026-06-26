#ifndef awshtmlviewH
#define awshtmlviewH

#include "awspaint.h"
#include "awssystem.h"
#include <gram/gmanal.h>

namespace aws {

struct HTMLStyle {
  int bold,italic,underline,strikeout,fontsize;
  //-1:unspecified
  bool iscolor;
  int color;
  string name;
  string fontname;
  string link;
  bool wrap;
  HTMLStyle() {color=bold=italic=underline=strikeout=fontsize=-1;wrap=false;iscolor=false;}
  void load(XMLNode xml);
  void updatefont(Font *font);
  bool match(STRING aname) {return name==aname;}
  bool wrapbegin() {return wrap;}
  bool wrapend() {return wrap;}
};

struct HTMLData {
  virtual HTMLStyle *findstyle(STRING name)=0;
  virtual Picture *findpic(string name)=0;
  virtual HTMLStyle *getbasestyle()=0;
};


struct HTMLCanvasState {
  int x,y,maxhi;
  bool beginline;
};

struct HTMLCanvas {
  HTMLCanvas(SPCanvas dest,Canvas draw,int wi,System *system,HTMLData *main);
  int gethi() {wrap();return y;}
  Rect drawword(string word);
  void wrap();
  void pushfont() {fonts.add(font->clone());}
  void popfont() {setfont(fonts.last());fonts.count--;}
  Rect drawspace();
  Point getpt() {return Point(x,y);}
  Font *clonefont() {return font->clone();}
  void setfont(Font *fnt) {font=fnt;dest->setfont(font);if (!nullcnt) line.setfont(font);}
  void nullbegin();
  void nullend();
  HTMLCanvasState getstate() {HTMLCanvasState res;res.x=x;res.y=y;res.maxhi=maxhi;res.beginline=beginline;return res;}
  void setstate(HTMLCanvasState &state) {x=state.x;y=state.y;maxhi=state.maxhi;beginline=state.beginline;}
  bool isnull() {return nullcnt;}
  void hline();
  int getx0() {return x0;}
  void setx0(int value) {x0=value;}
  void listmark();
  void centeron() {wrap();center++;}
  void centeroff() {wrap();if (center) center--;}
  bool iscenter() {return center;}
  Rect drawimg(Picture *img);
//  void heading(int type);
private:
  IObjArray<Font> fonts;
  PtrTo<Font> font;
  MetaCanvas line;
  Canvas draw;
//  MetaCanvas all;
  SPCanvas dest;
//  Canvas cnv;
  System *system;
  HTMLData *main;
  int nullcnt;
  int x,y;
  int x0;
  int wi;
  int maxhi;
  int center;
  bool beginline;
};

struct HTMLNode {
  HTMLData *main;
  virtual ~HTMLNode() {}
  virtual void repaint(HTMLCanvas &cnv) {}
//  string parseto(gram::Analyser &a,string end);
  void jumptag(gram::Analyser &a) {while (*a && *a!='>') a++;if (*a) a++;}
  void parsevars(gram::Analyser &a,VarList &vars);
  HTMLNode(HTMLData *main):main(main) {}
  Rect viewrect;//aktualizovanbo vzdy pri draw
  void draw(HTMLCanvas &cnv);
  virtual string gethref(int x,int y) {return "";}
};

struct HTMLWord:HTMLNode {
  string word;
  HTMLWord(string word,HTMLData *main):word(word),HTMLNode(main) {}
  void repaint(HTMLCanvas &cnv) {viewrect=cnv.drawword(word);}
};

struct HTMLImage:HTMLNode {
  RefBase<Picture> image;
  HTMLImage(HTMLData *main):HTMLNode(main) {}
  void repaint(HTMLCanvas &cnv) {if (image) viewrect=cnv.drawimg(image);else viewrect=Rect(cnv.getpt(),cnv.getpt());}
  void parse(gram::Analyser &a);
};


struct HTMLWrap:HTMLNode {
  void repaint(HTMLCanvas &cnv) {cnv.wrap();viewrect=Rect(cnv.getpt(),cnv.getpt());}
  HTMLWrap(HTMLData *main):HTMLNode(main) {}
};

struct HTMLCenterOn:HTMLNode {
  void repaint(HTMLCanvas &cnv) {cnv.centeron();}
  HTMLCenterOn(HTMLData *main):HTMLNode(main) {}
};

struct HTMLCenterOff:HTMLNode {
  void repaint(HTMLCanvas &cnv) {cnv.centeroff();}
  HTMLCenterOff(HTMLData *main):HTMLNode(main) {}
};

struct HTMLLine:HTMLNode {
  void repaint(HTMLCanvas &cnv) {cnv.wrap();cnv.hline();}
  HTMLLine(HTMLData *main):HTMLNode(main) {}
};

struct HTMLSpace:HTMLNode {
  void repaint(HTMLCanvas &cnv) {viewrect=cnv.drawspace();}
  HTMLSpace(HTMLData *main):HTMLNode(main) {}
};

struct HTMLPoly:HTMLNode {
  void repaint(HTMLCanvas &cnv);
  string parse(gram::Analyser &a);
  void clear() {nodes.clear();wasspace=false;}
  HTMLPoly(HTMLData *main):HTMLNode(main) {}
  string gethref(int x,int y);
private:
  IObjArray<HTMLNode> nodes;
  bool wasspace;
  void addnode(HTMLNode *node) {wasspace=false;nodes.add(node);}
  void addspace() {if (wasspace) return;nodes.add(new HTMLSpace(main));}
};

struct HTMLList:HTMLNode {
  void parse(gram::Analyser &a);
  void repaint(HTMLCanvas &cnv);
  HTMLList(HTMLData *main):HTMLNode(main) {}
  IObjArray<HTMLPoly> items;
  string gethref(int x,int y);
};

struct HTMLFmt:HTMLNode {
  VarList vars;
  string type;
  HTMLStyle *style;
  HTMLPoly poly;
  HTMLFmt(string type,HTMLData *main):type(type),HTMLNode(main),poly(main) {style=main->findstyle(type);}
  void parse(gram::Analyser &a);
  void repaint(HTMLCanvas &cnv);
  string gethref(int x,int y);
};

rtti_intf(FindPictureEvent)
rtti_intf(LinkEvent)

struct FindPictureEvent:NotifyEvent {
  char src[0x100];
  FindPictureEvent(STRING asrc) {strcpy(src,asrc);classtype=FindPictureEventClass;}
};
struct LinkEvent:NotifyEvent {
  char href[0x100];
  LinkEvent(STRING ahref) {strcpy(href,ahref);classtype=LinkEventClass;}
};


struct PictureItem {
  string src;
  RefBase<Picture> picture;
  PictureItem(string src,Picture *picture):src(src),picture(picture) {}
  bool match(STRING asrc) {return src==asrc;}
};

/*
struct HTMLMain {
  IObjArray<HTMLStyle> styles;
  HTMLStyle basestyle;
  void loadstyles(XMLNode xml);
  HTMLStyle *findstyle(STRING name) {return styles.searchvalue(name);}
//  virtual void onlink(string link) {}
//  virtual Picture *findpic(string name) {return NULL;}
};
*/

class HTMLView;
struct HTMLViewMain:HTMLData {
  IObjArray<HTMLStyle> styles;
  HTMLStyle basestyle;
  HTMLView *view;
  
  void loadstyles(XMLNode xml);
  Picture *findpic(string src);
  HTMLStyle *findstyle(STRING name) {return styles.searchvalue(name);}
  HTMLStyle *getbasestyle() {return &basestyle;}
  HTMLViewMain(HTMLView *view):view(view) {}
};

struct HTMLView:ScrolledPainter {
  HTMLView(Component *owner,System *system);
  void onpaint(Canvas &cnv,awsEvent &event);
  void settext(string value) {text=value;updated();}
  string gettext() {return text;}
  void updated() {painted=false;gram::Analyser a(text.c_str());html.parse(a);repaint();}
  void onsize(int w,int h);
  void onmousemove(int x,int y,ShiftState shift,awsEvent &event);
  void onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  void changecursor(Cursor cursor) {if (getcursor()!=cursor) setcursor(cursor);}
  Picture *searchpicture(string src) {PictureItem *pi=pics.searchvalue(src);if (pi) return pi->picture;return NULL;}
  void searchedpicture(string src,Picture *picture) {if (!searchpicture(src)) pics.add(new PictureItem(src,picture));}
  void loadprops(XMLNode xml,Form *form);
  void onlink(string href);
  Picture *findpic(string src);
private:
  int format(Canvas &cnv);
  EventValue ev_findpicture,ev_link;

  IObjArray<PictureItem> pics;
  HTMLPoly html;
  MetaCanvas meta;
  HTMLViewMain main;
  string text;
  bool painted;
  int fmtw;
};

void init_aws_html_rtti();

}

#endif
