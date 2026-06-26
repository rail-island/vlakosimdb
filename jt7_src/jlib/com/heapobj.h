#ifndef HHEAPOBJ
#define HHEAPOBJ

namespace jlibheap {


//tridici algoritmy vyuzivajici metody tridy
template<class T,class S> void heapsift(T a[],int l,int r,int (S::*cmp)(T,T),S &self)
{
#define ar(i) a[i-1]
  l++;r++;
  int i=l,j=2*i;
  T x=ar(i);
  while (j<=r)
  {
    if (j<r &&  (self.*cmp)(ar(j),ar(j+1))<0 ) j++;
    if ( (self.*cmp)(x,ar(j))>=0 ) break;
    ar(i)=ar(j);
    i=j;j=2*i;
  }
  ar(i)=x;
}
template<class T,class S> void heapsort(T a[],int num,int (S::*cmp)(T,T),S &self)
{
  if (num<=1) return;
  for(int l=num/2;l>=0;l--) heapsift(a,l,num-1,cmp,self);
  for(int r=num-1;r>0;r--)
  {
    T x=a[0];a[0]=a[r];a[r]=x;heapsift(a,0,r-1,cmp,self);
  }
}

}

#endif
