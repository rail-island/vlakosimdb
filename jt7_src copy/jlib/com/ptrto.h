#ifndef ptrtoH
#define ptrtoH
namespace jlib {

template<class T> struct PtrToMem {
  T *buf;
  T* operator ->() {return buf;}
  PtrToMem(int size=sizeof(T)) {
    buf=(T*)malloc(size);
    xxassert(buf,XNoMemory);
    fillarray<char>(reinterpret_cast<char*>(buf),size,0);}
  ~PtrToMem() {free(buf);obj=NULL;}
};

template<class T> struct PtrToObj {
  T *obj;
  T* operator ->() {return obj;}
  PtrToObj() {obj=new T;}
  ~PtrToObj() {delete obj;obj=NULL;}
};

template<class T> struct PtrTo {
  T *obj;
  T* operator ->() {return obj;}
  operator T *() {return obj;}
  void operator =(T *ref) {if (obj) delete obj;obj=ref;}
  PtrTo() {obj=NULL;}
  ~PtrTo() {if (obj) delete obj;obj=NULL;}
};


}
#endif