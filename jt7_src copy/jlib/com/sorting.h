#ifndef sortingH
#define sortingH

namespace jlib {

template<class T,class Compare> void heapsort(T a[],int num,const Compare &cmp);
//Compare je trida s definovanym operatorem int ()(T a,T b),vrati porovnani a,b
//(T se de cmp muze predavat i (konstantni) referenci

template<class T,class Compare> void stdquicksort(T a[],int num,const Compare &cmp);
//vola standartni fci qsort

template<class T,class Compare> void bubleelem(T a[],int num,const Compare &cmp,int elemindex);
//zatridi prvek a[elemindex],pocita s tim,ze tento prvek neni zatrideny,ostatni jsou setridene

}

#endif
