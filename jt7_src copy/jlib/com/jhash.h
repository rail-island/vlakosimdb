#ifndef jhashH
#define jhashH

namespace jlib {

/*
struct IHashItem {
  HashItem *h_getnext();
  void h_setnexth(HashItem *);
  int h_hashcode();
  bool match(const S &key);
}
*/

inline bool bad_size(int size) {
  return size%2==0 || size%3==0 || size%5==0 || size%7==0 || size%11==0;
}
inline int countopthashsize(int minsize) {
  while (bad_size(minsize)) minsize++;
  return minsize;
}

template<class T> struct IHashTable {//T:IHashItem
  T **buf;
  int size;

  T *search(int hash);
  void add(T *item);
  void rehash(int newsize);
  template<class S> T *search(int hash,const S &key);//requires match

  IHashTable(int initsize=1) {initbuf(initsize);}
  ~IHashTable() {delete []buf;}
  void setminhashsize(int minsize) {rehash(countopthashsize(minsize));}
private:
  void initbuf(int asize) {size=asize;if (size<1) size=1;buf=new T*[size];for(int i=0;i<size;i++) buf[i]=NULL;}
};

}

#endif
