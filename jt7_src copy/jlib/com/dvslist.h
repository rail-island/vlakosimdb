#ifndef dvslistH
#define dvslistH

namespace jlib {

struct DVSElem {
  DVSElem *next;
  DVSElem *prev;
  virtual ~DVSElem() {unlink();}
  DVSElem() {prev=NULL;next=NULL;}
  bool inlist() {return next && prev;}
  void unlink() {
    if (inlist()) {
      prev->next=next;
      next->prev=prev;
      prev=NULL;
      next=NULL;
    }
  }
  void _unlink() {prev=next=NULL;}
  void insmebefore(DVSElem *what) {
    unlink();
    next=what;
    prev=what->prev;
    prev->next=this;
    next->prev=this;
  }
  void insmeafter(DVSElem *what) {
    unlink();
    prev=what;
    next=what->next;
    prev->next=this;
    next->prev=this;
  }
};

struct DVSHead:DVSElem {
  DVSHead() {next=prev=this;}
  ~DVSHead() {next=prev=NULL;}
};

template<class T> struct TDVSIter {
  DVSElem *head,*act;
  TDVSIter(DVSElem *head,DVSElem *act):head(head),act(act) {};
  operator T* () {return act==head?NULL:(T*)act;}
  T* operator *() {return act==head?NULL:(T*)act;}
  T* operator ->() {xassert(act!=head);return (T*)act;}
  void operator ++(int) {act=act->next;}
  void operator --(int) {act=act->prev;}
  T &val() {xassert(act!=head);return*(T*)act;}
};
typedef TDVSIter<DVSElem> DVSIter;

template<class T> struct TDVSList {
  DVSHead head;
  TDVSIter<T> first() {return TDVSIter<T>(&head,head.next);}
  TDVSIter<T> last() {return TDVSIter<T>(&head,head.prev);}
  void addfirst(DVSElem *elem) {elem->insmeafter(&head);}
  void addlast(DVSElem *elem) {elem->insmebefore(&head);}
  T *firstelem() {return head.next==&head?NULL:(T*)head.next;}
  T *lastelem() {return head.prev==&head?NULL:(T*)head.prev;}
  void clear() {
    while (T *fe=firstelem()) fe->unlink();
  }
  bool empty() {return !firstelem();}
};
typedef TDVSList<DVSElem> DVSList;


}

#endif
