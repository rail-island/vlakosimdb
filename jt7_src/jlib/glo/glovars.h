#ifndef glovarsH
#define glovarsH

#include <ext/xmlnode.h>
#include <com/ptrto.h>

namespace glo {

class gloVariableList;

struct gloFloatExpr {
  virtual float eval(gloVariableList *vars)=0;
  virtual bool defined(gloVariableList *vars)=0;
};
struct gloConstFloat:gloFloatExpr {
  float x;
  gloConstFloat(float x):x(x) {}
  float eval(gloVariableList *vars) {return x;}
  bool defined(gloVariableList *vars) {return true;}
};
struct gloFloatCondItem {
  string onwhat;
  float value;
  gloFloatCondItem(string onwhat,float value):onwhat(onwhat),value(value) {}
};
struct gloCondFloatExpr:gloFloatExpr {
  IObjArray<gloFloatCondItem> items;
  string var;
  float def;

  float eval(gloVariableList *vars);
  gloCondFloatExpr(string expr);
  bool defined(gloVariableList *vars);
};
struct gloVariableFloatExpr:gloFloatExpr {
  string name;
  float def;

  float eval(gloVariableList *vars);
  gloVariableFloatExpr(string s);
  bool defined(gloVariableList *vars);
};

class gloStringGradientValue;
struct gloValue {
  virtual float getfloat(float def) {return def;}
  virtual string getstring(string def) {return def;}
  virtual gloStringGradientValue *gradient() {return NULL;}
};

struct gloFloatValue:gloValue {
  float value;
  float getfloat(float def) {return value;}
  gloFloatValue(float value):value(value) {}
};
struct gloStringValue:gloValue {
  string value;
  string getstring(string def) {return value;}
  float getfloat(float def) {return atof(value.c_str());}
  gloStringValue(string value):value(value) {}
};
struct gloStringGradientValue:gloValue {
  string from,to;float t;
  string getstring(string def) {if (t<0.5) return from;return to;}
  virtual gloStringGradientValue *gradient() {return this;}
  gloStringGradientValue(string from,string to,float t):from(from),to(to),t(t) {}
};

struct gloVariable {
  string name;
  PtrTo<gloValue> value;
  bool match(STRING aname) {return name==aname;}
  gloVariable(string aname,gloValue *avalue) {name=aname;value=avalue;}
};

struct gloVariableList:RCObject {
  gloVariableList *parent;
  IObjArray<gloVariable> vars;

  gloVariableList() {parent=NULL;}

  gloVariable *searchvar(STRING name);
  void setvar(string name,float value) {setvar(name,new gloFloatValue(value));}
  void setvar(string name,string value) {setvar(name,new gloStringValue(value));}
  void setgradvar(string name,string from,string to,float t) {setvar(name,new gloStringGradientValue(from,to,t));}
  void setvar(string name,gloValue *value);
  float fvar(string name,float def=0) {gloVariable *res=searchvar(name);return res?res->value->getfloat(def):def;}
  string svar(string name,string def="") {gloVariable *res=searchvar(name);return res?res->value->getstring(def):def;}
};

bool glo_isvariable(string s);
gloFloatExpr *glo_parsefloat(string s);

}

#endif
