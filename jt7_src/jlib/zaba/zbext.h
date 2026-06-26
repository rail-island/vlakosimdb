#ifndef zbextH
#define zbextH

#include "zbobj.h"
#include <ext/xmlnode.h>
#include <gram/ssint.h>
#include <com/ptrto.h>
#include <ext/jarchive.h>
#include <ext/persistentobject.h>

namespace zaba {

void initextrt(Runtime &rt);

struct zbArchive:zbObject {
  Archive ar;
  Archive *getlink() {return &ar;}
  Archive &cvalue() {return ar;}
  zbArchive(Runtime *rt,STRING intf="Archive.instance"):zbObject(rt,intf) {}
  zbObject *list();
};
inline zbObject *newarchive(Runtime *rt) {return new zbArchive(rt);}

struct zbPersistentObject:zbObject {
  RefBase<PersistentObject> obj;
  PersistentObject *getlink() {return obj;}
  PersistentObject *cvalue() {return obj;}
  string listChilds();
  string listFiles();
  zbPersistentObject(PersistentObject *aobj,Runtime *rt,STRING intf="PersistentObject.instance"):zbObject(rt,intf) {obj=aobj;}
  void close() {obj=NULL;}
  zbObject *openChild(string name);
  zbObject *openChildPath(string path);
  zbObject *getparent();
  zbObject *getroot();
  string tostring() {return obj->getname();} 
};



}

#endif
