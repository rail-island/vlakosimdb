#ifndef jstrobjsH
#define jstrobjsH

#include <com/rcobj.h>

namespace jlib {

struct Strings:TObjVec<string> {
  int datasize(int beg,int end) {
    int sum=0;
    for(int i=beg;i<end;i++) sum+=buf[i].length();
    return sum;
  }
  int datasize() {return datasize(0,count);}
  string gettext(int beg,int end,char sep);
  string gettext(char sep='\n') {return gettext(0,count,sep);}
  void fromtext(string text,char sep);
};

struct VarRecord {
  string name,value;
  VarRecord(string aname,string avalue) {name=aname;value=avalue;}
  bool match(string aname) {return name==aname;}
};

struct VarList:RCObject,IObjArray<VarRecord> {
  const char *findvar(string name) {
    VarRecord *var=searchvalue(name);
    return var?var->value.c_str():NULL;
  }
  string findvalue(string name) {
    VarRecord *var=searchvalue(name);
    if (var) return var->value;
    string res;
    return res;
  }
  void _addvar(string name,string value) {add(new VarRecord(name,value));}
  void setvar(string name,string value) {
    if (VarRecord *vr=searchvalue(name)) vr->value=value;else _addvar(name,value);
  }
  void setvar(string name,int value) {setvar(name,itos(value));}
  void setvar(string name,bool value) {setvar(name,itos(value));}
  void setvar(string name,double value) {setvar(name,ftos(value));}
  void setvar(string name,float value) {setvar(name,ftos(value));}
  void assign(VarList &src);
  void erasevar(string name) {
    int idx=searchtype(name);
    if (idx>=0) erase(idx);
  }
  int ivar(STRING name) {return atoi(cvar(name));}
  double fvar(STRING name) {return atof(cvar(name));}
  const char *cvar(STRING name) {return findvalue(name).c_str();}
  string svar(STRING name) {return findvalue(name);}
  bool isvar(STRING name) {return searchvalue(name);}
};

}
#endif
