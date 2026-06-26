#include <jlib.h>
#pragma hdrstop

#include "sorting.h"

namespace jlib {


template<class T,class Compare> void bubleelem(T a[],int num,const Compare &cmp,int elemindex) {
  int i=elemindex;
  while(i>0 and cmp(a[i-1],a[i])>0) {T pom=a[i];a[i]=a[i-1];a[i-1]=pom;i--;}
  while(i<num-1 and cmp(a[i],a[i+1])>0) {T pom=a[i];a[i]=a[i+1];a[i+1]=pom;i++;}
}

static const void *qsortcomparator=NULL;

template<class Compare,class T> int stdqsortcompare(const void *a,const void *b) {
  const Compare &cmp=*(reinterpret_cast<const Compare*>(qsortcomparator));
  return cmp(*reinterpret_cast<const T*>(a),*reinterpret_cast<const T*>(b));
}

template<class T,class Compare> void stdquicksort(T a[],int num,const Compare &cmp) {
  xassert(qsortcomparator==NULL);
  qsortcomparator=reinterpret_cast<const void*>(&cmp);
  qsort(a,num,sizeof(T),stdqsortcompare<Compare,T>);
  qsortcomparator=NULL;
}

template<class T,class Compare> void heapshift(T a[],int l,int r,const Compare &cmp) {
#define ar(i) a[i-1]
  l++;r++;
  int i=l,j=2*i;
  T x=ar(i);
  while (j<=r)
  {
    if (j<r && cmp(ar(j),ar(j+1))<0 ) j++;
    if ( cmp(x,ar(j))>=0 ) break;
    ar(i)=ar(j);
    i=j;j=2*i;
  }
  ar(i)=x;
}

template<class T,class Compare> void heapsort(T a[],int num,const Compare &cmp) {
  if (num<=1) return;
  for(int l=num/2;l>=0;l--) heapshift(a,l,num-1,cmp);
  for(int r=num-1;r>0;r--) {
    T x=a[0];a[0]=a[r];a[r]=x;heapshift(a,0,r-1,cmp);
  }
}

}

#pragma package(smart_init)

