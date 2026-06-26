#ifndef garbcolH
#define garbcolH

#include <com/dvslist.h>

namespace jlib {

struct GC_Data:DVSElem {
  int inrefcnt;
  bool valid_object;
  void validate() {valid_object=true;}
  void addinref() {inrefcnt++;}
};

/*
struct T {
  GC_Data gc_data;

  int refcnt;//pocet referenci, neni udrzovano GC, ale potrebuje to pro spravnou fci
  void enum_refs_and_add_inrefs();//pro vsechny reference z tohoto objektu vola inrefcnt++
  void enum_refs_and_validate();//pro vsechny reference vola validate
  void unlink_refs();//odpoji vsechny reference, nesmi je znicit, ani kdyz maji refcnt==0 !!!
  //(pokud jsou udrzovany GC)
  void destroy();//normalne {delete this;}
};
*/



template<class T> struct GarbageCollector {
  TDVSList<GC_Data> list;
  void add(T *obj) {list.addlast(&obj->gc_data);}
  GarbageCollector();
  void run();
private:
  int distance_from_object_begin;
  T *objfromdata(GC_Data *data) {return reinterpret_cast<T*>((char*)data-distance_from_object_begin);}
};

}

#endif
