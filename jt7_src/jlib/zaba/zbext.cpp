#include <jlib.h>
#pragma hdrstop

#include "zbext.h"
#include "zbstd.h"
#include "zbobj.h"
#include "zbint.h"
#include "zbintf.h"
#include "zbrt.h"
#include <com/sorting.cpp>
#include <ext/xmldb.h>
#include "zbss.h"


namespace zaba {

zbObject *zbArchive::list() {
  zbArray *res=new zbArray(rt);
  int cnt=ar.count();
  for(int i=0;i<cnt;i++) {
    ArchiveRec rec;
    ar.get(i,rec);
    res->items.add(createString(rec.name,rt));
  }
  return res;
}

string zbPersistentObject::listChilds() {
  if (!obj) throw JError("PersistentObject.list():object is closed");
  Strings items;
  obj->getchilds(items);
  return items.gettext('\n');
}
string zbPersistentObject::listFiles() {
  if (!obj) throw JError("PersistentObject.list():object is closed");
  Strings items;
  obj->getfiles(items);
  return items.gettext('\n');
}


zbObject *zbPersistentObject::openChild(string name) {
  PersistentObject *res=obj->openchild(name);
  if (res) return new zbPersistentObject(res,rt);
  return createVoid(rt);
}
zbObject *zbPersistentObject::openChildPath(string path) {
  PersistentObject *res=obj->openchildpath(path);
  if (res) return new zbPersistentObject(res,rt);
  return createVoid(rt);
}

zbObject *newpersistentobject(string file,Runtime *rt) {
  TCntVec<FileRec> files;
  jsystem->listfiles(file,files);
  if (files.count<1) return createVoid(rt);
  PersistentObject *res=NULL;
  if (files[0].isdir) res=new PODirectory(file);else res=new POArchive(file);
  return new zbPersistentObject(res,rt);
}

zbObject *zbPersistentObject::getparent() {
  PersistentObject *res=obj->getparent();
  if (res) return new zbPersistentObject(res,rt);
  return createVoid(rt);
}

zbObject *zbPersistentObject::getroot() {
  PersistentObject *res=obj->getroot();
  return new zbPersistentObject(res,rt);
}

#include "extlib/extobjs.h"
void initextrt(Runtime &rt) {init_extobjs_rt(rt);}

#include "extlib/extobjs.cc"

}


