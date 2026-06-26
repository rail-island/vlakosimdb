#include <jlib.h>
#pragma hdrstop

#include "glovars.h"
#include <gram/gmanal.h>

namespace glo {

void gloVariableList::setvar(string name,gloValue *value) {
  if (gloVariable *var=vars.searchvalue(name)) {
    var->value=value;
  } else {
    gloVariable *newvar=new gloVariable(name,value);
    vars.add(newvar);
  }
}

gloVariable *gloVariableList::searchvar(STRING name) {
  gloVariableList *list=this;
  while (list) {
    if (gloVariable *res=list->vars.searchvalue(name)) return res;
    list=list->parent;
  }
  return NULL;
}

bool gloVariableFloatExpr::defined(gloVariableList *vars) {
  if (!vars) return false;
  return vars->searchvar(name);
}
float gloVariableFloatExpr::eval(gloVariableList *vars) {
  if (vars) return vars->fvar(name,def);
  return def;
}
bool gloCondFloatExpr::defined(gloVariableList *vars) {
  if (!vars) return false;
  return vars->searchvar(var);
}
float gloCondFloatExpr::eval(gloVariableList *vars) {
  if (!vars) return def;
  gloVariable *v=vars->searchvar(var);
  if (!v) return def;
  if (gloStringGradientValue *grad=v->value->gradient()) {
    int fromi=-1,toi=-1;
    for(int i=0;i<items.count;i++) {
      if (items[i]->onwhat==grad->from) fromi=i;
      if (items[i]->onwhat==grad->to) toi=i;
    }
    if (fromi>=0 && toi>=0) {
      float v1=items[fromi]->value,v2=items[toi]->value;
      return v1+grad->t*(v2-v1);
    }
  }
  string value=v->value->getstring("");
  for(int i=0;i<items.count;i++) {
    if (items[i]->onwhat==value) return items[i]->value;
  }
  return def;
}
gloVariableFloatExpr::gloVariableFloatExpr(string s) {
  def=0;
  gram::Analyser a(s.c_str());
  a.f_char('$');
  name=a.readidf();
  if (*a=='!') {
    a++;
    def=a.readdouble();
  }
}

gloCondFloatExpr::gloCondFloatExpr(string expr) {
  def=0;
  gram::Analyser a(expr.c_str());
  a.f_char('?');
  var=a.readidf();
  if (*a!='>') return;
  a++;
  while (*a!=')') {
    string name=a.readidf();
    if (*a!=':') return;
    a++;
    float f=a.readdouble();
    items.add(new gloFloatCondItem(name,f));
    if (*a=='!') {
      a++;
      def=a.readdouble();
    }
    if (*a!=';') return;
    a++;
  }
}
bool glo_isvariable(string s) {
  return s.begins("?") || s.begins("$");
}
gloFloatExpr *glo_parsefloat(string s) {
  if (s.begins("?")) return new gloCondFloatExpr(s);
  if (s.begins("$")) return new gloVariableFloatExpr(s);
  return new gloConstFloat(s.fvalue());
}


}

