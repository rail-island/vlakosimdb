#ifndef memobjsH
#define memobjsH

namespace jlib {

template<class T> struct PtrVector {
  int size;
  T **buf;
  PtrVector(int asize) {
    buf=new T*[size=asize];
    for(int i=0;i<size;i++) buf[i]=NULL;
  }
  ~PtrVector() {
    for(int i=0;i<size;i++) delete buf[i];
    delete []buf;
  }
  T *&operator [] (int index) {return buf[index];}
};
template<class T> struct PtrMatrix:PtrVector<T> {
  int W,H;
  PtrMatrix(int w,int h):PtrVector<T>(w*h) {
    W=w;H=h;
  }
  T *&operator () (int x,int y) {return buf[y*W+x];}
  T *at(int x,int y) {if (x<0 or x>=W or y<0 or y>=H) return NULL;return buf[y*W+x];}
};


template<class T> struct MemBlock {
  T *buf;int capa;
  void wantcapa(int newcapa) {
    if (newcapa>capa) buf=(T*)realloc(buf,(capa=newcapa)*sizeof(T));
  }
  MemBlock() {
    buf=NULL;capa=0;
  }
  ~MemBlock() {
    if (buf) free(buf);
  }
};

template<class T> struct OWNER {
  T * operator ->() const {return obj;}
  void operator =(T *src) {if (obj) delete obj;obj=src;}
  operator T *() const {return obj;}
  OWNER() {obj=NULL;}
  explicit OWNER(T *ptr) {obj=ptr;}
  ~OWNER() {if (obj) delete obj;}

  T *obj;
};

struct LQItem {
  LQItem *next;
  virtual ~LQItem() {}
  LQItem() {next=NULL;}
};

template<class T> struct ListQeue {
  T *first,*last;
  ListQeue() {first=NULL;last=NULL;}
  ~ListQeue() {clear();}
  void clear() {
    T *act=first;
    while (act) {
      T *act2=act;
      act=(T*)act->next;
      delete act2;
    }
    first=NULL;last=NULL;
  }
  void insert(T *item) {
    if (first) {
      item->next=first;
      first=item;
    } else init(item);
  }
  void add(T *item) {
    if (first) {
      last->next=item;
      item->next=NULL;
      last=item;
    } else init(item);
  }
  T *popfirst() {
    if (first) {
      T *res=first;
      first=(T*)res->next;
      if (first==NULL) last=NULL;
      return res;
    }
    return NULL;
  }
  void erasefirst() {delete popfirst();}
private:
  void init(T *item) {
    first=item;last=item;item->next=NULL;
  }
};

template<class T> struct ValueMatrix {
  int w,h;
  T *buf;
  ValueMatrix(int w,int h):w(w),h(h) {buf=new T[w*h];}
  bool valid(int x,int y) {return x>=0 and x<w and y>=0 and y<h;}
  T &_at(int x,int y) {return buf[y*w+x];}
  T &at(int x,int y) {xassert(valid(x,y));return buf[y*w+x];}
  T &operator () (int x,int y) {return at(x,y);}
  ~ValueMatrix() {if (buf) delete []buf;}
  ValueMatrix() {w=h=0;buf=NULL;}
  void setsize(int aw,int ah) {if (buf) delete []buf;w=aw;h=ah;buf=new T[w*h];}
  void foreach(void (T::*func)() ) {
    int size=w*h;
    for(int i=0;i<size;i++) ((buf[i]).*func)();
  }
  void assign(const ValueMatrix<T> &src) {setsize(src.w,src.h);if (src.buf) memcpy(buf,src.buf,w*h*sizeof(T));}
  bool equals(const ValueMatrix<T> &src) {
    if (w!=src.w or h!=src.h) return false;
    int size=w*h;
    for(int i=0;i<size;i++) if (buf[i]!=src.buf[i]) return false;
    return true;
  }
  void settoall(T value) {
    int size=w*h;
    for(int i=0;i<size;i++) buf[i]=value;
  }
};

template<class T> struct MatrixTranspositer {
  ValueMatrix<T> *matrix;
  int w,h;
  MatrixTranspositer(ValueMatrix<T> *matrix):matrix(matrix) {w=matrix->h;h=matrix->w;}
  T &operator () (int x,int y) {return matrix->at(y,x);}
};

template<class T> struct MatrixTranspositer2 {
  ValueMatrix<T> *matrix;
  int w,h;bool transp;
  MatrixTranspositer2(ValueMatrix<T> *matrix,bool transp):matrix(matrix),transp(transp) {
    w=matrix->w;h=matrix->h;if (transp) exchange(w,h);
  }
  T &operator () (int x,int y) {return transp?matrix->at(y,x):matrix->at(x,y);}
};


}

#endif
