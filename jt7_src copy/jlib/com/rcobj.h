#ifndef rcobjH
#define rcobjH

//#define REFCHECK

#ifdef REFCHECK
#define ref_check xassert(ref==NULL || ref->refcnt>0)
#define ref_check_other(other) xassert(other.ref==NULL || other.ref->refcnt>0)
#else
#define ref_check
#define ref_check_other(other)
#endif

//#define PROXYCHECKING

namespace jlib {

struct RCObject {
  int refcnt;
  RCObject() {refcnt=0;}
  virtual ~RCObject() {}
  inline RCObject *addref() {refcnt++;return this;}
  inline void release() {refcnt--;if (refcnt==0) delete this;}
};


template<class T> struct RCArrayBase {
  static const bool wantlocalcopy=true;
  static const bool wantdelete=true;
  static const bool wantinitialize=true;
  void deletelocalcopy(T *item) {if (item) item->release();};
  void makelocalcopy(T *&dest,T *src) {dest=src;dest->addref();};
  void initialize(T *&dest) {dest=NULL;}
  bool equals(const T *item1,const T *item2) {return item1==item2;}
};

template<class T> struct TRCArray:MemoryArray<T*,RCArrayBase<T> > {
};

typedef TRCArray<RCObject> RCArray;

/*
class Base<T> {
  T *get_ref();
  void set_ref(T *ref);
};
*/

template<class T,class Base> struct TRefBase:Base {
  TRefBase() {set_ref(NULL);}
  ~TRefBase() {if (get_ref()) get_ref()->release();}
  TRefBase(T *ref) {set_ref(ref);if (ref) ref->addref();}
  TRefBase(const TRefBase<T,Base> &r) {set_ref(r.get_ref());if (get_ref()) get_ref()->addref();}
  void operator =(const TRefBase<T,Base> &r) {if (r.get_ref()) r.get_ref()->addref();if (get_ref()) get_ref()->release();set_ref(r.get_ref());}
  T *operator ->() {return get_ref();}
};

template<class T> struct RefBase {
  T *ref;
  RefBase() {ref=NULL;}
  ~RefBase() {ref_check;if (ref) ref->release();}
  RefBase(T *ref):ref(ref) {if (ref) ref->addref();ref_check;}
  RefBase(const RefBase<T> &r) {ref_check_other(r);ref=r.ref;if (ref) ref->addref();}
  T *operator ->() {ref_check;return ref;}
  void operator =(const RefBase<T> &r) {ref_check;ref_check_other(r);if (r.ref) r.ref->addref();if (ref) ref->release();ref=r.ref;}
  operator T * () {ref_check;return ref;}
};

template<class T> struct RefCounter:T {
  RefCounter() {refcnt=0;}
  virtual ~RefCounter() {}
  void addref() {refcnt++;}
  void release() {if (!--refcnt) delete this;}
protected:
  int refcnt;
};

template<class T> struct ProxyObject:RCObject {
  #ifdef PROXYCHECKING
  int check;
  #endif
  T *obj;
  void erased() {obj=NULL;}
  T *get() {
    #ifdef PROXYCHECKING
    xassert(check!=531);xassert(check==135);
    #endif
    return obj;
  }
  ProxyObject(T *object):obj(object) {
    #ifdef PROXYCHECKING
    check=135;
    #endif
  }
  ~ProxyObject() {
    #ifdef PROXYCHECKING
    xassert(obj==NULL);check=531;
    #endif
  }//obj musi nejdrive zavolat erased
};

template<class T> struct SmartPtrInit {
  T *obj;
  explicit SmartPtrInit(T *obj):obj(obj) {}
};

template<class T> struct SmartPtr {
  ProxyObject<T> *ref;
  SmartPtr(SmartPtrInit<T> init) {ref=new ProxyObject<T>(init.obj);ref->addref();}
  SmartPtr() {ref=NULL;}
  ~SmartPtr() {if (ref) ref->release();}
//  SmartPtr(ProxyObject<T> *ref):ref(ref) {if (ref) ref->addref();}
  SmartPtr(const SmartPtr<T> &r) {ref=r.ref;if (ref) ref->addref();}
  T *operator ->() {return ref->get();}
  void operator =(const SmartPtr<T> &r) {if (r.ref) r.ref->addref();if (ref) ref->release();ref=r.ref;}
//  operator T * () {return ref->obj;}
  void erased() {if (ref) ref->erased();}
  bool isnull() {return ref==NULL || ref->get()==NULL;}
  T *ptr() {return ref->get();}
};

}

#endif
