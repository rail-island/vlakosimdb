#include <jlib.h>
#pragma hdrstop

#include "jhash.h"

namespace jlib {

template<class T> T *IHashTable<T>::search(int hash) {
  int index=(unsigned long)hash % size;
  T *act=buf[index];
  while (act && act->h_hashcode()!=hash) act=act->h_getnext();
  return act;
}

template<class T> template<class S> T *IHashTable<T>::search(int hash,const S &key) {
  int index=(unsigned long)hash % size;
  T *act=buf[index];
  while (act && (act->h_hashcode()!=hash || !act->h_match(key)) ) act=act->h_getnext();
  return act;
}

template<class T> void IHashTable<T>::add(T *item) {
  int index=(unsigned long)item->h_hashcode() % size;
  if (buf[index]) buf[index]->h_setnext(item);
  buf[index]=item;
}

template<class T> void IHashTable<T>::rehash(int newsize) {
  int oldsize=size;
  T **oldbuf=buf;
  initbuf(newsize);
  for(int i=0;i<oldsize;i++) {
    T *act=oldbuf[i];
    while (act) {
      add(act);
      act=act->h_getnext();
    }
  }
  delete []oldbuf;
}

}


