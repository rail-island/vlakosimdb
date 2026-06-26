#ifndef jslinkH
#define jslinkH

#include <js/jscommon.h>
#include <js/jsintf.h>

#define jsinterfaceH(type,bigtype) extern js::Interface * type##_interface;\
class bigtype;\
struct type:js::ProxyObject<bigtype> {\
  type() {intf=type##_interface;}\
};

namespace js {

template<class T> struct ProxyObject:Object {
  T *ref;
  void deleted() {ref=NULL;}
  T *get(js::ComInt *ci) {
    if (ref==NULL) ci->raisefmt("Object %s nas been deleted",typeid(T).name());
    return ref;
  }
};

}

#endif
