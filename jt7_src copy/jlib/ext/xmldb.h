#ifndef xmldbH
#define xmldbH

#include <aws/awstable.h>

namespace jlib {

struct XMLTableFld {
  char name[0x20];
  char text[0x20];
  bool sub;
  XMLTableFld(STRING aname,STRING atext,bool asub) {strcpy(name,aname);strcpy(text,atext);sub=asub;}
  bool match(const char *aname) {return eqstr(name,aname);}
};

struct XMLTableModel:aws::TableModel {
  XMLNode xml;
  IObjArray<XMLTableFld> flds;
  void clear() {flds.clear();model_update();}
  void add(STRING name,STRING text,bool sub) {flds.add(new XMLTableFld(name,text,sub));model_update();}
  int getrowcount() {return xml.count();}
  string getcelltext(const char *fldid,int y);
  void getcols(TCntVec<aws::TableCol> &res);
  XMLTableModel() {refcnt=0;}
protected:
  int refcnt;
  void addref() {refcnt++;}
  void release() {if (!--refcnt) delete this;}
};

struct SortCol {
  char name[0x20];
  enum {number,text,str} type;
};

struct XMLSort {
  XMLNode xml;
  TCntVec<SortCol> index;
  void addcol(STRING name,int type) {SortCol &col=index.add();strcpy(col.name,name);col.type=type;}
  XMLNode work();
};

}

#endif
