#ifndef autodbsH
#define autodbsH

#include "dbs.h"

namespace jlib {

class ADBSARRAY;
struct ADBSELEM {
  IObjArray<ADBSARRAY> arrays;
  VARLIST vars;
  virtual ADBSARRAY *findarray(const string &name);
};
struct ADBSARRAY {
  string name;
  IObjArray<ADBSELEM) items;
  bool match(const string &aname) {return name==aname;}
};
ADBSARRAY *ADBSELEM::findarray(const string &name) {
  int idx=arrays.searchtype(name);
  if (idx>=0) return arrays[idx];
  return NULL;
}

//prekryje metody getvalue a docommand pro zpracovani zadane struktury
struct AUTODBS:DBSCRIPT {
  struct AREC {
    ADBSARRAY *array;
    int index;
    POS beg;
    char next[0x100],nextcom[0x100];
    AREC(ADBSARRAY *array,AUTODBS *parent) {
      this->array=array;index=0;beg=parent->getpos();
      sprintf(next,"#[next%s]",array->name.c_str());
      sprintf(nextcom,"next%s",array->name.c_str());
    }
    AREC() {}
  };
  ADBSELEM *elemstack[0x100],**elemw;
  AREC arraystack[0x100],*arrayw;
  string getvalue(const string &name);
  void docommand(const string &name);
  AUTODBS(ADBSELEM *base) {elemw=elemstack;*elemw++=base;arrayw=arraystack;}
  void structurejump(const char *endcom,bool behind);

  bool isdefined(const char *s);
};

}

#endif
