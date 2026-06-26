#include "setops.h"

namespace jlib {

template<class T,class Compare> void intersection(Array<T> &a,Array<T> &b,TCntVec<T> &res,Compare cmp) {
  T *pa=a.buf,*pb=b.buf,*aend=pa+a.count,*bend=pb+b.count;
  while (pa<aend and pb<bend) {
    while (pa<aend and cmp(*pa,*pb)<0) pa++;
    while (pb<bend and cmp(*pb,*pa)<0) pb++;
    if (pa<aend and pb<bend and cmp(*pa,*pb)==0) {res+=*pa;pa++;pb++;}
  }
}

template<class T,class Compare> void difference(Array<T> &a,Array<T> &b,TCntVec<T> &res,Compare cmp) {
  T *pa=a.buf,*pb=b.buf,*aend=pa+a.count,*bend=pb+b.count;
  while (pa<aend and pb<bend) {
    while (pb<bend and cmp(*pb,*pa)<0) pb++;
    if (pa<aend and pb<bend) {
      if (cmp(*pa,*pb)==0) {pa++;pb++;} else {res+=*pa;pa++;}
    }
  }
  while (pa<aend) res+=*pa++;
}

}


#pragma package(smart_init)
