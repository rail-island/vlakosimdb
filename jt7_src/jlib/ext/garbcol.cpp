#include <jlib.h>
#pragma hdrstop

#include "garbcol.h"
#include <typeinfo>

namespace jlib {

template<class T> GarbageCollector<T>::GarbageCollector() {
  T *pom=NULL;
  distance_from_object_begin=((char*)&pom->gc_data)-(char*)0;
}

template<class T> void GarbageCollector<T>::run() {
  typedef TDVSIter<GC_Data> Iter;
  for(Iter iter=list.first();*iter;iter++) iter->inrefcnt=0,iter->valid_object=false;
  for(Iter iter=list.first();*iter;iter++) objfromdata(*iter)->enum_refs_and_add_inrefs();
  for(Iter iter=list.first();*iter;iter++) if (objfromdata(*iter)->_refcnt()>iter->inrefcnt) {
    iter->validate();
    objfromdata(*iter)->enum_refs_and_validate();
  }
//  TDVSList<GC_Data> to_delete_list;
//  for(Iter iter=list.first();*iter;) if (not iter->valid_object) to_delete_list.addlast(*iter);
/*  for(Iter iter=list.first();*iter;iter++) {
    if (iter->valid_object) {
      int a=0;
      T *obj=objfromdata(*iter);
      eputs(typeid(*obj).name());
    }
    if (not iter->valid_object) objfromdata(*iter)->unlink_refs();
  }*/
  for(Iter iter=list.first();*iter;iter++) if (not iter->valid_object) objfromdata(*iter)->unlink_refs();
  for(Iter iter=list.first();*iter;) {
    GC_Data *gd=*iter;
    iter++;
    if (not gd->valid_object) {
      T *obj=objfromdata(gd);
      if (obj->_refcnt()>0) {
        eprintf("GC Error:%s[refcnt=%d,inrefcnt=%d]\n",typeid(*obj).name(),obj->_refcnt(),gd->inrefcnt);
        xabstract;
      }
      obj->destroy();
    }
  }
//  while (not to_delete_list.empty()) objfromdata(to_delete_list.firstelem())->destroy();
}


}


