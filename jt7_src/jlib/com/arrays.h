#ifndef arraysH
#define arraysH

/*
struktura Base by mela definovat nektere z techto metod+mit nastavene tyto staticke atribtuy
template<class T> struct Base {
  void makelocalcopy(T &dest,const T &src);
  void deletelocalcopy(T &item);
  static const bool wantlocalcopy;//zda se musi volat makelocalcopy,nebo zda staci prekopirovat pamet
  static const bool wantdelete;//zda je potreba volat deletelocalcopy
  bool equals(const T &item1,const T &item2);//volano z search,zjistuje rovnost
);*/

/*
MemoryArray:sablona,odvozena od parametru,ktery musi mit definovane vyse uvedene metody a atributy
konvence:metody zacinajici _:nemusi probihat zadna kontrola,jsou protected
*/

#pragma option push -w-
#pragma option push -vi

#define CHECKINGINDEX

namespace jlib {

template<class T> struct Array {//jednoduche pole pro cteni,vsechna pole jsou jeho potomkem
  T *buf;
  int count;
  T &operator [] (int index) const {xcheckindex(index,count);return buf[index];}
  explicit Array(T &src) {buf=&src;count=1;}
  Array(T *abuf,int acount) {buf=abuf;count=acount;}
  Array() {buf=NULL;count=0;}
  int search(const T &item,int start=0) {
    for(int i=start;i<count;i++) if (buf[i]==item) return i;
    return -1;
  }
  template<class S> int psearchi(S what) {for(int i=0;i<count;i++) if (buf[i]->match(what)) return i;return -1;}
  template<class S> T psearchv(S what) {for(int i=0;i<count;i++) if (buf[i]->match(what)) return buf[i];return NULL;}
  template<class S> int vsearchi(S what) {for(int i=0;i<count;i++) if (buf[i].match(what)) return i;return -1;}
  template<class S> T *vsearchv(S what) {for(int i=0;i<count;i++) if (buf[i].match(what)) return buf+i;return NULL;}
};

template<class T> struct ArrayIntf {//virtualni pole
  virtual T at(int index)=0;
  virtual int Count()=0;
};

template<class T,class Base> struct MemoryArray:Array<T>,Base,ArrayIntf<T> {
private:
  void __deleteitems(int index,int delcnt) {
    for(int i=index,stop=index+delcnt;i<stop;i++) deletelocalcopy(buf[i]);
  }
protected:
  struct TMemory {
    char buf[sizeof(T)];
  };
  T at(int index) {return item(index);}
  int Count() {return count;}

  void __initialize(int index,int count) {
    for(int i=index;i<index+count;i++) initialize(buf[i]);
  }
  void _initialize(int index,int count) {
    if (wantinitialize) __initialize(index,count);
  }
  void __initialize(int index) {
    initialize(buf[index]);
  }
  void _initialize(int index) {
    if (wantinitialize) __initialize(index);
  }

  void _deleteitems(int index,int delcnt) {
    if (wantdelete) __deleteitems(index,delcnt);
  }
  void _erasememory(int index,int delcnt) {
    memmove(buf+index,buf+index+delcnt,sizeof(T)*(count-index-delcnt));
    count-=delcnt;
  }
  void _erasecheck(int index,int delcnt) {
    lobjcheckindex(index,count);
    lobjcheckindex(index+delcnt-1,count);
  }
  void _copyitems(T dest[],const T src[],int cnt) {
    if (wantlocalcopy) {
      for(int i=0;i<cnt;i++) makelocalcopy(dest[i],src[i]);
    } else memmove(dest,src,cnt*sizeof(T));
  }
  T& _insert(int index,int inscnt=1) {//pouze zvetsi pamet a odsune zbytek,vrati odkaz na prvni prvek
    xobjassert(index>=0);
    wantcapa(count+inscnt);
    if (index<count) memmove(buf+index+inscnt,buf+index,sizeof(T)*(count-index));
    _initialize(index,inscnt);
    count+=inscnt;
    return buf[index];
  }
  T& _add() {//pouze vratri odkaz na novy prvek,neinicializuje
    wantcapa(count+1);
    _initialize(count);
    return buf[count++];
  }
  void _add(const Array<T> &a,int begin,int afterend) {
    if (afterend<=begin) return;
    lobjcheckindex(begin,a.count);
    lobjcheckindex(afterend-1,a.count);
    wantcapa(count+afterend-begin);
    _initialize(count,afterend-begin);
    _copyitems(buf+count,a.buf+begin,afterend-begin);
    count+=afterend-begin;
  }
  void _insert(int index,Array<T> &a,int begin,int afterend) {
    if (afterend<=begin) return;
    lassert(index<=count);
    lobjcheckindex(begin,a.count);
    lobjcheckindex(afterend-1,a.count);
    _insert(index,afterend-begin);
    _copyitems(buf+index,a.buf+begin,afterend-begin);
  }
  void _moveitems(int to,int from,int count) {
    memmove(buf+to,buf+from,sizeof(T)*count);
  }
  TMemory &mem_at(int index) {return reinterpret_cast<TMemory&>(buf[index]);}
public:
  static const int MINJMPCAPA=8;
  int capa;
  static int newcapa(int wanted) {return max((wanted*3)/2,wanted+MINJMPCAPA);}

  T& item(int index) {
    lobjcheckindex(index,count);
    return buf[index];
  };
  T &last() {
    xobjassert(count>0);return buf[count-1];
  }
  void setcapa(int newcapa) {
    xobjassert(newcapa>=count);
    buf=(T*)realloc(buf,newcapa*sizeof(T));
    if (buf==NULL and newcapa>0) xthrow(XNoMemory);
    capa=newcapa;
  };
  void wantcapa(int acapa) { if (acapa>capa) setcapa(newcapa(acapa));};
  void exchange(int index1,int index2) {
    lobjcheckindex(index1,count);
    lobjcheckindex(index2,count);
    T pom=buf[index1];
    buf[index1]=buf[index2];
    buf[index2]=pom;
  }
  T &add() {return _add();}
  void add(const Array<T> &a) {_add(a,0,a.count);}
  void add(const Array<T> &a,int begin,int afterend) {_add(a,begin,afterend);}
  void insert(int index,Array<T> &a) {insert(index,a,0,a.count);}
  void insert(int index,Array<T> &a,int begin,int afterend) {_insert(index,a,begin,afterend);}
  T &insert(int index) {return _insert(index);}
  T* insert(int index,int inscnt) {//pouze zvetsi pamet a odsune zbytek,vrati odkaz na prvni prvek
    return &_insert(index,inscnt);
  }
  T pop() {
    xassert(count>0);
    T res=buf[count-1];
    if (wantdelete) _deleteitems(count-1,1);
    count--;
    return res;
  }
  void erase(int index,int delcnt=1) {
    if (delcnt>0) {
      _erasecheck(index,delcnt);
      _deleteitems(index,delcnt);
      _erasememory(index,delcnt);
    }
  }
  void quickerase(int index) {//prehazi poradi,ale je rychlejsi
    xobjcheckindex(index,count);
    _deleteitems(index,1);
    count--;
    if (count>0) {
      initialize(buf[index]);
      buf[index]=buf[count];
    }
  }
  void clear() {_deleteitems(0,count);count=0;}

  MemoryArray() {capa=0;}
  virtual ~MemoryArray() {clear();free(buf);}

  void eraselast(int delcnt=1) {
    xobjassert(delcnt<=count);
    _deleteitems(count-delcnt,delcnt);
    count-=delcnt;
  }
  int search(const T &item,int start=0) {
    for(int i=start;i<count;i++) if (equals(buf[i],item)) return i;
    return -1;
  }
  bool remove(const T &item) {
    int idx=search(item);
    if (idx<0) return false;
    erase(idx);
    return true;
  }
  bool quickremove(const T &item) {
    int idx=search(item);
    if (idx<0) return false;
    quickerase(idx);
    return true;
  }
  bool removeall(const T &item) {
    int idx=search(item);
    int res=0;
    while (idx>=0) {
      erase(idx);
      idx=search(item);
      res++;
    }
    return res;
  }

  void reverse() {
    if (count<=1) return;
    T *left=buf,*right=buf+count-1;
    while (left<right) {
      T pom=*left;
      *left=*right;
      *right=pom;
      left++;
      right--;
    }
  }

  void pack(T val) {//spakuje vsechny prvky s hodnotou val,nevola zadne erasedeleted
    T *w=buf;
    for(int i=0;i<count;i++) if (buf[i]!=val) *w++=buf[i];
    count=w-buf;
  }

  void replace(int index,const T &item) {
    lobjcheckindex(index,count);
    if (wantdelete) deletelocalcopy(buf[index]);
    if (wantlocalcopy) makelocalcopy(buf[index],item);
    else buf[index]=item;
  }
  T& operator[] (int index) {return item(index);}
  void add(const T &what) {if (wantlocalcopy) makelocalcopy(add(),what);else add()=what;}
  void push(const T &what) {add(what);}
  void insert(int index,const T &what) {if (wantlocalcopy) makelocalcopy(insert(index),what);else insert(index)=what;}
  void operator +=(const Array<T> &src) { add(src);}
  void operator +=(const T &what) { add(what);}
  void assign(Array<T> &src) {clear();add(src);}
  MemoryArray &operator <<(const T &what) {add(what);return *this;}
  bool operator ==(const Array<T> &src) {
    if (count!=src.count) return false;
    for(int i=0;i<count;i++) if (buf[i]!=src.buf[i]) return false;
    return true;
  }
  bool operator !=(Array<T> &src) {return not operator ==(src);}
  void moveto(int source,int target) {
    if (source==target) return;
    xobjcheckindex(source,count);
    xobjcheckindex(target,count);
    if (target<source) {
      TMemory pom=mem_at(source);
      _moveitems(target+1,target,source-target);
      mem_at(target)=pom;
    } else {
      TMemory pom=mem_at(source);
      _moveitems(source,source+1,target-source);
      mem_at(target)=pom;
    }
  }
private:
  MemoryArray(const Array<T> &src);
  MemoryArray(const MemoryArray &src);
  void operator =(const Array<T> &src);
  void operator =(const MemoryArray &src);
};

template<class T> struct TCntVecBase {
  static const bool wantlocalcopy=false;
  static const bool wantdelete=false;
  static const bool wantinitialize=false;
  void makelocalcopy(T &dest,const T &src) {};
  void deletelocalcopy(T &item) {};
  bool equals(const T &item1,const T &item2) {return item1==item2;}
  void initialize(T &item) {}
};

template<class T> struct TCntVec:MemoryArray<T,TCntVecBase<T> > {
};

template<class T> struct IObjArrayBase {
  static const bool wantlocalcopy=false;
  static const bool wantdelete=true;
  static const bool wantinitialize=false;
  void makelocalcopy(T *&dest,const T *src) {};
  void deletelocalcopy(T *item) {delete item;};
  bool equals(const T *item1,const T *item2) {return item1==item2;}
  void initialize(T *item) {}
};

template<class T> struct IObjArray:MemoryArray<T*,IObjArrayBase<T> > {
  template<class S> int searchtype(S what) {//musi byt definovano T::match(S)
    for(int i=0;i<count;i++) if (buf[i]->match(what)) return i;
    return -1;
  }
  template<class S> T * searchvalue(S what) {//musi byt definovano T::match(S)
    for(int i=0;i<count;i++) if (buf[i]->match(what)) return buf[i];
    return NULL;
  }
  template<class S> void setfld(S T::*fld,const S &value,int beg,int end) {
    lcheckindex(beg,count);
    lcheckindex(end,count);
    for(int i=beg;i<=end;i++) buf[i]->*fld=value;
  }
  template<class S> void setfld(S T::*fld,const S &value) {
    for(int i=0;i<count;i++) buf[i]->*fld=value;
  }
  void packnull() {pack(NULL);}//vsechny ukazatele,ktere jsou NULL,vyhaze
  void assign(Array<T> &src) {xabstract;}
};

template <class T> struct Stack:TCntVec<T>
{
  int push(const T &what) { add(what);return count-1;}
  T pop() {xobjassert(count>0);return buf[--count];}
  T &top() {return item(count-1);}
  T &top(int distfromtop) {return item(count-distfromtop-1);}
};

template<class T> struct TObjVecBase {
  static const bool wantlocalcopy=true;
  static const bool wantdelete=true;
  static const bool wantinitialize=true;
  void makelocalcopy(T &dest,const T &src) {new(JP_VOID(&dest)) T(src);};
  void deletelocalcopy(T &item) { item.~T(); };
  void initialize(T &item) {new (JP_VOID(&item)) T();}
  bool equals(const T &a,const T &b) {return a==b;}
};

template<class T> struct TObjVec:MemoryArray<T,TObjVecBase<T> > {
  struct PTRARRAY:ArrayIntf<T*> {
    TObjVec *parent;
    PTRARRAY(TObjVec<T> *aparent) {parent=aparent;}
    int Count() {return parent->count;}
    T *at(int index) {return &parent->item(index);}
  };

  T &addnew() {T &res=_add();new(JP_VOID(&res)) T();return res;}
  //vrati novy prvek inicializovany default konstruktorem
  template<class S> T &addnew(S init) {T &res=_add();new(JP_VOID(&res)) T(init);return res;}
  //vrati novy prvek inicializovany konstruktorem s jednim parametrem typu S

  T &insertnew(int index) {T &res=_insert(index);new(&res) T();return res;}
  template<class S> T &insertnew(int index,S init) {T &res=_insert(index);new(&res) T(init);return res;}

  PTRARRAY ptrarray() {return PTRARRAY(this);}

  template<class S> int searchtype(S what) {//musi byt definovano T::math(S)
    for(int i=0;i<count;i++) if (buf[i].match(what)) return i;
    return -1;
  }

  template<class S> T *searchvalue(S what) {//musi byt definovano T::math(S)
    for(int i=0;i<count;i++) if (buf[i].match(what)) return &buf[i];
    return NULL;
  }

  template<class S> void setfld(S T::*fld,const S &value,int beg,int end) {
    lcheckindex(beg,count);
    lcheckindex(end,count);
    for(int i=beg;i<=end;i++) buf[i].*fld=value;
  }
  template<class S> void setfld(S T::*fld,const S &value) {
    for(int i=0;i<count;i++) buf[i].*fld=value;
  }
};

template<class T> struct VirtualArrayBase {
  static const bool wantlocalcopy=true;
  static const bool wantdelete=true;
  static const bool wantinitialize=true;
  virtual void makelocalcopy(T &dest,const T &src) {dest=src;};
  virtual void deletelocalcopy(T &item) {};
  virtual bool equals(const T &item1,const T &item2) {return false;}
  virtual void initialize(T &item) {}
};

template<class T> struct VirtualArray:MemoryArray<T,VirtualArrayBase<T> > {
};



}

#pragma option pop
#pragma option pop

#endif
