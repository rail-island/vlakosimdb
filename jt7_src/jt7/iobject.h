#ifndef iobjectH
#define iobjectH

#include <glo/gloobject.h>
#include <ext/persistentobject.h>

namespace jtyc {
using namespace glo;

struct jtDataObject:RCObject {
  string name;
  string title;
  string path;
  string author;
  RefBase<PersistentObject> zipobj;

  string getname() {return name;}
  string gettitle() {return title;}
  bool match(STRING s) {return name==s;}
  virtual ~jtDataObject() {}
  virtual void load(PersistentObject *data,XMLNode xml) {}
};

struct jtModelObject:jtDataObject {
  PtrTo<glo::gloModel> model;
};

glo::gloModel *jt_loadmodel(PersistentObject *parent,string file);
glo::gloModel *jt_loadmodel(PersistentObject *obj);

extern bool ignore_visibility;

}

#endif
