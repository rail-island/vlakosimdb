#ifndef gmsexprH
#define gmsexprH

#include "gmanal.h"

namespace gram {

struct RegResult {
  bstring value;
  int count;
  RegResult() {count=0;}
  void operator <<(string v) {value<<v;count+=v.length();}
  void operator <<(RegResult &r) {operator <<(r.value);}
  void operator <<(char ch) {value<<ch;count++;}
};

struct RegItem {
  static RegItem *load(Analyser &a);
  string doparse(Analyser &a) {RegResult res;parse(a,res);return res.value;}
  bool doparse(Analyser &a,string &res) {RegResult rr;bool ok=parse(a,rr);res=rr.value;return ok;}

  virtual bool parse(Analyser &a,RegResult &fw)=0;
};

struct RegOr:RegItem {
  IObjArray<RegItem> items;
  bool parse(Analyser &a,RegResult &fw);
};

struct RegCat:RegItem {
  IObjArray<RegItem> items;
  bool parse(Analyser &a,RegResult &fw);
};

struct RegIter:RegItem {
  RegItem *item;
  int min,max;
  RegIter(RegItem *item):item(item) {min=max=0;}
  ~RegIter() {delete item;}
  bool parse(Analyser &a,RegResult &fw);
  void load(Analyser &a);
};

struct RegNeg:RegItem {
  RegItem *item;
  RegNeg(RegItem *item):item(item) {}
  ~RegNeg() {delete item;}
  bool parse(Analyser &a,RegResult &fw);
};

struct RegChar:RegItem {
  char ch;char out;
  bool parse(Analyser &a,RegResult &fw);
  RegChar() {ch=out=0;}
};

struct RegSpecChar:RegItem {
  char type;
  char out;
  bool outasin;
  bool parse(Analyser &a,RegResult &fw);
  RegSpecChar() {type=out=0;outasin=false;}
};

struct RegCharSet:RegItem {
  bool parse(Analyser &a,RegResult &fw);
  void load(Analyser &a);
  RegCharSet() {for(int i=0;i<0x100;i++) isin[i]=0,chout[i]=0;}
private:
  bool isin[0x100];
  char chout[0x100];
  void setin(int beg,int end,bool ok) {for(int i=beg;i<=end;i++) isin[i]=ok;}
  void setout(int beg,int end,char out) {for(int i=beg;i<=end;i++) chout[i]=out;}
  void setdout(int beg,int end,char out) {for(int i=beg;i<=end;i++) chout[i]=(uchar)out+i-beg;}
  void setoutasin(int beg,int end) {for(int i=beg;i<=end;i++) chout[i]=(char)i;}
};

}

#endif
