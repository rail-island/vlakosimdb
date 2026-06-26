#ifndef jalgoH
#define jalgoH
//---------------------------------------------------------------------------

//Algoritmy pro praci s poli
//predpoklada existenci indexovaciho operatoru

namespace jlib {


template<class T> int jt_find(T *a,T what,int cnt)
{
  for(int i=0;i<cnt;i++)
    if(a[i]==what) return i;
  return -1;
}

template<class T> int jt_search(T *a,T what,T jt_NULL,int cnt)
{
  for(int i=0;i<cnt;i++)
    if(a[i]==T) return a[i];
  return jt_NULL;
}

template<class T> void jt_reverse(T *a,int cnt)
{
  for(int l=0,r=cnt-1;l<r;l++,r--)
  {
    T pom=a[l];
    a[l]=a[r];
    a[r]=pom;
  }
}

}

#endif
