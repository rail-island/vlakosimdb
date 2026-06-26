#ifndef HHEAP
#define HHEAP

namespace jlibheap {


template<class T,class S> void heapsift(T a[],int l,int r,int (*cmp)(T,T,S),S param)
{
#define ar(i) a[i-1]
  l++;r++;
  int i=l,j=2*i;
  T x=ar(i);
  while (j<=r)
  {
    if (j<r && (*cmp)(ar(j),ar(j+1),param)<0 ) j++;
    if ( (*cmp)(x,ar(j),param)>=0 ) break;
    ar(i)=ar(j);
    i=j;j=2*i;
  }
  ar(i)=x;
}

template<class T,class S> void heapsort(T a[],int num,int (*cmp)(T,T,S),S param)
{
  if (num<=1) return;
  for(int l=num/2;l>=0;l--) heapsift(a,l,num-1,cmp,param);
  for(int r=num-1;r>0;r--)
  {
    T x=a[0];a[0]=a[r];a[r]=x;heapsift(a,0,r-1,cmp,param);
  }
}

template<class T> void heapsift(T a[],int l,int r,int (*cmp)(T,T))
{
#define ar(i) a[i-1]
  l++;r++;
  int i=l,j=2*i;
  T x=ar(i);
  while (j<=r)
  {
    if (j<r && (*cmp)(ar(j),ar(j+1))<0 ) j++;
    if ( (*cmp)(x,ar(j))>=0 ) break;
    ar(i)=ar(j);
    i=j;j=2*i;
  }
  ar(i)=x;
}

template<class T> void heapsort(T a[],int num,int (*cmp)(T,T))
{
  if (num<=1) return;
  for(int l=num/2;l>=0;l--) heapsift(a,l,num-1,cmp);
  for(int r=num-1;r>0;r--)
  {
    T x=a[0];a[0]=a[r];a[r]=x;heapsift(a,0,r-1,cmp);
  }
}

inline int _internal_int_cmp(int a,int b) {return a-b;}
inline void heapsort(int a[],int num)
{ heapsort(a,num,_internal_int_cmp); }

/*template<class T,class S> void quicksort(T a[],int num,int (*cmp)(T,T,S),S param)
{
  struct {int l,r;} MEZ;
  Stack<MEZ> zas;
  do{
  }
  if (num<=1) return;
  for(int l=num/2;l>=0;l--) heapsift(a,l,num-1,cmp,param);
  for(int r=num-1;r>0;r--)
  {
    T x=a[0];a[0]=a[r];a[r]=x;heapsift(a,0,r-1,cmp,param);
  }
}*/

}

#include <com/heapobj.h>

#endif
