#ifndef awsformH
#define awsformH

#include "awscontrol.h"

namespace aws {
class Window;

struct Creator {
  virtual Control *createcontrol(string name,Component *owner,System *system) {return NULL;}
  virtual Layout *createlayout(string name,ParentControl *owner) {return NULL;}
};

struct StdCreator:Creator {
  Control *createcontrol(string name,Component *owner,System *system);
  Layout *createlayout(string name,ParentControl *owner);
};

struct FormModel {
  string name;
  AbstractModel *model;
  FormModel(STRING name,AbstractModel *model):name(name),model(model) {}
  bool match(STRING aname) {return name==aname;}
};

struct InlinePacker {
  string name;
  Control *ctrl;
  InlinePacker(string name,Control *ctrl):name(name),ctrl(ctrl) {}
  bool match(STRING aname) {return name==aname;}
};

struct Form:Component,EventHandler {
  Control *load(XMLNode xml);
  void loadchilds(XMLNode xml,ParentControl *parent);
  Form(Component *owner,System *system);
//  Creator *getcreator() {return creator;}
  Control *search(STRING name);
  Window *loadwindow(XMLNode xml);
  void on_defaulthandler(BaseEvent &event);
  void loadcontrol(Control *loading,XMLNode xml,ParentControl *parent);
  Control *loadcontrol(XMLNode xml,ParentControl *parent);
  void loaddata(XMLNode xml);
  void savedata(XMLNode xml);
  AbstractModel *searchmodel(STRING name) {if (FormModel *m=models.searchvalue(name)) return m->model;return NULL;}
  void addmodel(STRING name,AbstractModel *value) {models.add(new FormModel(name,value));}
  void addinline(STRING name,Control *control);
  Control *searchinline(STRING name) {if (InlinePacker *p=inlines.searchvalue(name)) return p->ctrl;return NULL;}

//  bool can_call_direct_es;
private:
//  Creator *creator;
  IObjArray<FormModel> models;
  IObjArray<InlinePacker> inlines;
};

}

#endif
