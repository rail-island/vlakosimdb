#ifndef awstableH
#define awstableH

#include "awsmodels.h"
#include "awscontrol.h"
#include "awspaint.h"

namespace aws {

enum TableColType {tct_text,tct_check};

struct TableCol {
  char text[0x20];
  char id[0x20];
  TableColType type;
  bool match(const char *aid) {return eqstr(id,aid);}
  TableCol(STRING atext,const char *aid,TableColType atype=tct_text) {strcpy(id,aid);strcpy(text,atext);type=atype;}
  TableCol() {}
};

struct TableModel:AbstractModel {
  virtual int getrowcount()=0;
  virtual string getcelltext(const char *fldid,int y)=0;
  virtual void getcols(TCntVec<TableCol> &res)=0;
};

class Table;

struct TableContent:ScrolledPainter {
  void onpaint(Canvas &cnv,awsEvent &event);
  TableContent(Component *owner,System *system,Table *table):ScrolledPainter(owner,system),table(table) {
    cellhi=20;cellspacing=3;cbx=cby=1;
  }
  void repaintline(int line);
  void onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);

  int cbx,cby;//sirka hranice mezi bunkami
  int cellhi;//,cellwi;
  int cellspacing;
  Table *table;
  int getcellwi(int x);// {return cellwi;}
  int getcellleft(int x);// {return x*cellwi;}
  Rect getcellrect(int x,int y) {return Rect(getcellleft(x),cellhi*y,getcellwi(x),cellhi);}
  int rowcnt,colcnt;
  void setbounds(int rows,int cols);
  Point mousetocell(Point m);
};

struct TableColItem {
  char text[0x20];
  char id[0x20];
  int left,wi;
  TableColType type;
  bool visible;
  TableColItem(const TableCol &col,int wi0) {
    strcpy(id,col.id);strcpy(text,col.text);wi=wi0;left=0;visible=true;
    type=col.type;
  }
  int right() {return left+wi;}
};

rtti_intf(ColClickEvent)
rtti_intf(CellClickEvent)
rtti_intf(GetLineColorEvent)

struct ColClickEvent:NotifyEvent {
  char id[0x20];
  ColClickEvent(STRING aid) {strcpy(id,aid);classtype=ColClickEventClass;}
};

struct CellClickEvent:NotifyEvent {
  char id[0x20];
  int line;
  ShiftState shift;
  CellClickEvent(STRING aid,int aline,ShiftState ashift) {strcpy(id,aid);line=aline;shift=ashift;classtype=CellClickEventClass;}
};

struct GetLineColorEvent:NotifyEvent {
  int line;int color;
  GetLineColorEvent(int aline) {line=aline;classtype=GetLineColorEventClass;color=clnnone;}
};


struct Table:PaintControl,AbstractImplementation,EventHandler {
  TableModel *getmodel() {return dynamic_cast<TableModel*>(AbstractImplementation::getmodel());}
  Table(Component *owner,System *system);
  void onpaint(Canvas &cnv,awsEvent &event);
  void onsize(int w,int h);
  void changed_model();
  void onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  void onmouseup(int x,int y,MouseButton button,ShiftState shift,awsEvent &event);
  void onmousemove(int x,int y,ShiftState shift,awsEvent &event);
  void gotocell(Point cell);
  void colclicked(STRING id);
  void cellclicked(STRING id,int line,ShiftState shift);
  void cellchecked(STRING id,int line,ShiftState shift);
  int getlinecolor(int line);
  int getselrow() {return actcelly;}
  string getselcol() {if (actcellx>=0) return cols[vismap[actcellx]].id;return "";}
  void setselcol(string id);
  void setselrow(int row);
  void onmousewheel(int x,int y,int delta,ShiftState shift,awsEvent &event);
//  void setlinecolor(int color) {linecoloranswer=color;}
//  int getdrawingline() {return drawingline;}
  void repaint();
  void repaintline(int line);
  void update();
  void savedata(XMLNode xml);
  void loaddata(XMLNode xml);
  int findcol(STRING id);
private:
  TCntVec<TableColItem> cols;

  TableContent *content;
//  ScrollBox *scr;

//  int linecoloranswer;
//  int drawingline;

  int actcellx,actcelly;
  int bgcolor,selbg;
  int tophi,leftwi;
  int borderx,bordery;
  int mincolsize;
  int resizingcol;//=-1
  int movingcol;//=-1
  int mdownx,mdowny;
  bool smalldist;//indikuje, zda nebylo mysi tazeno moc daleko pri stisku leveho tlacitka
  int maxsmalldist;
  friend class TableContent;
  XMLNode loadeddata;

  void countcols();//nastavi viscols,colswi,vismap,cols[i].left
  int viscols,colswi;
  int resizehdr;//==2
  int vismap[0x100];
  EventValue ev_colclick,ev_cellclick,ev_getlinecolor,ev_cellcheck;

  void showloadeddata(XMLPath root);
  void repainthdr() {repaint(Rect(0,bordery,getsize().w,tophi));}
  void repaint(Rect r) {PaintControl::repaint(r);}
protected:
  void onkeydown(awsKey key,awsEvent &event);
  void on_notify(NotifyEvent &ev);
  void loadprops(XMLNode xml,Form *form);
};

void init_aws_table_rtti();

}

#endif
