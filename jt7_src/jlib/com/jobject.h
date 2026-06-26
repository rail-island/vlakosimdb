#ifndef jobjectH
#define jobjectH

#include "arrays.h"
#include "strms.h"

//#pragma option push -w-
//#pragma option push -vi

namespace jlib {

struct JObject {
  void addref() {refcnt++;}
  void release() {if (!--refcnt) delete this;}
  virtual void save(BINWRITE &fw) {fw<<saveindex<<refcnt;}
protected:
  JObject() {saveindex=-1;refcnt=0;}
  JObject(BINREAD &fr) {fr>>saveindex>>refcnt;}
  virtual ~JObject() {}
private:
  int refcnt;
  int saveindex;
  friend class JSaver;
  friend class JLoader;
};

typedef int (*Ftypeof)(JObject *obj);
typedef JObject * (*Floadobj)(int type,BINREAD &fr);
//ciselne vyjadreni typu musi byt >= 0 

struct JSaver {
  int saveobject(JObject *obj);
  JSaver(BINWRITE &fw,Ftypeof atypeof);
  ~JSaver() {close();}
  void close();
private:
  TCntVec<JObject*> linked,saved;
  Ftypeof typeof_f;
  int actobj;
  BINWRITE *fw;
};

struct JLoader {
private:
  struct LINK {
    int gid;
    void *ptr;
    LINK(int agid,void *aptr) {gid=agid;ptr=aptr;}
  };
public:
  void *data;
  int loadobject();
  JLoader(BINREAD &fr,Floadobj aloadobj);
  ~JLoader() {close();}
  void close();
  void addlink(void *addr,int gid) {links.add(LINK(gid,addr));}
private:
  void connect();
  TCntVec<LINK> links;
  TCntVec<JObject*> objs;
  BINREAD *fr;
  int toload;
  Floadobj loadobj;
};

JSaver *fw_saver(BINWRITE &fw) {return reinterpret_cast<JSaver*>(fw.data);}
JLoader *fr_loader(BINREAD &fr) {return reinterpret_cast<JLoader*>(fr.data);}

template<class T> struct RefTo {
  T *obj;
  RefTo() {obj=NULL;}
  T * operator ->() const {return obj;}
  operator T * () const {return obj;}
//  template<class S> RefTo(const RefTo<S> &src) {obj=src.obj;if (obj) obj->addref();}
  RefTo(const RefTo<T> &src) {obj=src.obj;if (obj) obj->addref();}
  ~RefTo() {
    if (obj) obj->release();
  }
  void operator =(const RefTo<T> &src) {
    if (obj) obj->release();
    obj=src.obj;if (obj) obj->addref();
  }
  void operator =(T *src) {
    if (obj) obj->release();
    obj=src;if (obj) obj->addref();
  }
  RefTo(T *src) {obj=src;if (obj) obj->addref();}
  void set_int(int saveindex) {obj=reinterpret_cast<T*>(saveindex);}
  bool match(T *aobj) {return obj==aobj;}
};

template<class T> struct BackRef {//to same jako RefTo, jen nejsou citany odkazy
  //platnost odkazu musi byt zajistena logikou programu, ne citanim odkazu
  T *obj;
  BackRef() {obj=NULL;}
  T * operator ->() const {return obj;}
  operator T * () const {return obj;}
  void operator =(T *src) {obj=src;}
  BackRef(T *src) {obj=src;}
  void set_int(int saveindex) {obj=reinterpret_cast<T*>(saveindex);}
};

template<class T,class S> inline bool operator ==(const RefTo<T> &a,const RefTo<S> &b) {return a.obj==b.obj;}
template<class T,class S> inline bool operator ==(const RefTo<T> &a,const BackRef<S> &b) {return a.obj==b.obj;}
template<class T,class S> inline bool operator ==(const BackRef<T> &a,const RefTo<S> &b) {return a.obj==b.obj;}
template<class T,class S> inline bool operator ==(const BackRef<T> &a,const BackRef<S> &b) {return a.obj==b.obj;}

template<class T> inline BINWRITE & operator <<(BINWRITE &fw,RefTo<T> &link) {
  fw<<fw_saver(fw)->saveobject(link.obj);
  return fw;
}
template<class T> inline BINREAD & operator >>(BINREAD &fr,RefTo<T> &link) {
  int si=fr.readi();
  if (si>=0) {
    link.set_int(si);
    fr_loader(fr)->addlink(&link.obj,si);
  }
  return fr;
}
template<class T> inline BINWRITE & operator <<(BINWRITE &fw,BackRef<T> &link) {
  fw<<fw_saver(fw)->saveobject(link.obj);
  return fw;
}
template<class T> inline BINREAD & operator >>(BINREAD &fr,BackRef<T> &link) {
  int si=fr.readi();
  if (si>=0) {
    link.set_int(si);
    fr_loader(fr)->addlink(&link.obj,si);
  }
  return fr;
}

template<class T> class RefArray:public TObjVec<RefTo<T> > {
  typedef TObjVec<RefTo<T> > ANCESTOR;
public:
  RefTo<T> &add() {
    RefTo<T> &p=ANCESTOR::add();
    p.obj=NULL;
    return p;
  }
  void save(BINWRITE &fw) {
    fw<<count;
    for(int i=0;i<count;i++) fw<<buf[i];
  }
  void load(BINREAD &fr) {
    int cnt;fr>>cnt;wantcapa(cnt);
    for(int i=0;i<cnt;i++) fr>>add();
  }
  void add(T *ref) {
    RefTo<T> &p=ANCESTOR::add();
    p.obj=ref;
    ref->addref();
  }
  void insert(int index,T *ref) {
    RefTo<T> &p=ANCESTOR::insert(index);
    p.obj=ref;
    ref->addref();
  }
  void erase(int index) {
    xcheckindex(index,count);
    buf[index]=NULL;
    TObjVec<RefTo<T> >::erase(index);
  }
  void erase(T *ref) {
    int idx=searchtype(ref);
    erase(idx);
  }
  RefTo<T> & operator [] (int index) {return item(index);}
};

template<class T> inline BINWRITE & operator <<(BINWRITE &fw,RefArray<T> &links) {
  links.save(fw);return fw;
}
template<class T> inline BINREAD & operator >>(BINREAD &fr,RefArray<T> &links) {
  links.load(fr);return fr;
}

template<class T> struct RefVector {
  int size;
  RefTo<T> *buf;
  RefVector(int asize) {
    buf=new RefTo<T>[size=asize];
  }
  ~RefVector() {delete []buf;}
  RefTo<T> &operator [] (int index) {return buf[index];}
  T *at(int index) {if (index<0 or index>=size) return NULL;return buf[index].obj;}
  void save(BINWRITE &fw) {
    fw<<size;
    for(int i=0;i<size;i++) fw<<buf[i];
  }
  void load(BINREAD &fr) {
    if (buf) delete []buf;
    fr>>size;buf=new RefTo<T>[size];
    for(int i=0;i<size;i++) fr>>buf[i];
  }
  RefVector(BINREAD &fr) {
    fr>>size;buf=new RefTo<T>[size];
    for(int i=0;i<size;i++) fr>>buf[i];
  }
  RefVector<T> *shallow_copy() {
    RefVector<T> *res=new RefVector<T>(size);
    for(int i=0;i<size;i++) (*res)[i]=operator[](i);
    return res;
  }
};
template<class T> struct RefMatrix:RefVector<T> {
  int W,H;
  RefMatrix(int w,int h):RefVector<T>(w*h) {
    W=w;H=h;
  }
  RefTo<T> &operator () (int x,int y) {return buf[y*W+x];}
  T *at(int x,int y) {if (x<0 or x>=W or y<0 or y>=H) return NULL;return buf[y*W+x].obj;}
  void save(BINWRITE &fw) {
    RefVector<T>::save(fw);
    fw<<W<<H;
  }
  void load(BINREAD &fr) {
    RefVector<T>::load(fr);
    fr>>W>>H;
  }
  RefMatrix(BINREAD &fr):RefVector<T>(fr) {
    fr>>W>>H;
  }
  RefMatrix<T> *shallow_copy() {
    RefMatrix<T> *res=new RefMatrix<T>(W,H);
    for(int x=0;x<W;x++) for(int y=0;y<H;y++) (*res)(x,y)=operator()(x,y);
    return res;
  }
};

template<class T> inline BINWRITE & operator <<(BINWRITE &fw,RefVector<T> &vec) {
  vec.save(fw);return fw;
}
template<class T> inline BINREAD & operator >>(BINREAD &fr,RefVector<T> &vec) {
  vec.load(fr);return fr;
}

template<class T> inline BINWRITE & operator <<(BINWRITE &fw,RefMatrix<T> &mat) {
  mat.save(fw);return fw;
}
template<class T> inline BINREAD & operator >>(BINREAD &fr,RefMatrix<T> &mat) {
  mat.load(fr);return fr;
}


}

//#pragma option pop
//#pragma option pop

#endif
