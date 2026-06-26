#ifndef setopsH
#define setopsH


namespace jlib {

template<class T,class Compare> void intersection(Array<T> &a,Array<T> &b,TCntVec<T> &res,Compare cmp);
//prunik
template<class T,class Compare> void difference(Array<T> &a,Array<T> &b,TCntVec<T> &res,Compare cmp);
//rozdil

}


#endif
