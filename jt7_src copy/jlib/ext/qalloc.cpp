#include <jlib.h>
#pragma hdrstop

#include "qalloc.h"

namespace jlib {

typedef struct QAllocator::BIGBLOCK {
  BIGBLOCK *next;
} QBigBlock;

struct QAllocator::FREEBLOCK {
  long reserved;
  FREEBLOCK *next;
};
#define _BIGSIZE_ ((sizeof(QBigBlock)+7)&~7)

inline char *bigdata(QBigBlock *bl) {return (char*)bl+_BIGSIZE_;}


void *QAllocator::alloc() {
  if (frees) {
    void *res=frees;
    frees=frees->next;
    return res;
  }
  int cnt=items_to_alloc();
  BIGBLOCK *nb=(BIGBLOCK*)malloc(_BIGSIZE_+cnt*itemsize);
  if (!nb) return NULL;
  nb->next=bigs;
  bigs=nb;
  for(char *actfree=bigdata(nb)+itemsize;cnt>1;cnt--,actfree+=itemsize) {
    FREEBLOCK *fb=(FREEBLOCK*)actfree;
    fb->next=frees;frees=fb;
  }
  return bigdata(nb);
}
void QAllocator::free(void *p) {
  FREEBLOCK *f=(FREEBLOCK*)p;
  f->next=frees;
  frees=f;
}
QAllocator::QAllocator(int itemsize) {
  if (itemsize<sizeof(FREEBLOCK)) itemsize=sizeof(FREEBLOCK);
  this->itemsize=itemsize;
  itemstoalloc=0x1000/itemsize;
  if (itemstoalloc<=0) itemstoalloc=1;
  bigs=NULL;
  frees=NULL;
}
QAllocator::~QAllocator() {
//  xassert(frees==NULL);
  while (bigs) {
    BIGBLOCK *next=bigs->next;
    free(bigs);
    bigs=next;
  }
}


/*
****************************************************************
*                        StackAllocator
****************************************************************
*/

struct StackAllocator::BLOCK {
  BLOCK *next;
  int size;
  int start;//kolikaty ve prvni byte bloku v celem zasobniku
  void *offset(int index) {return (char*)(this+1)+index;}
};

StackAllocator::BLOCK *StackAllocator::allocblock(int reqsize) {
  int datasize=max(2*reqsize,minblocksize);
  BLOCK *res=(BLOCK*)malloc(datasize+sizeof(BLOCK));
  res->size=datasize;
  res->start=top;
  res->next=NULL;
  return res;
}

void *StackAllocator::alloc(int size) {
  if (last) {
    if (size<=last->size+last->start-top) {
      void *res=last->offset(top-last->start);
      top+=size;
      return res;
    } else {//muze byt i pripad, ze dalsi block uz je alokovany
      while (last->next) {
        last=last->next;//top=last->start;
        if (last->size>=size) {
          top=last->start+size;
          return last->offset(0);
        }
      }
      top=last->start+last->size;

      last->next=allocblock(size);
      last=last->next;
      top+=size;
      return last->offset(0);
    }
  } else {
    first=last=allocblock(size);
    top+=size;
    return first->offset(0);
  }
}
StackAllocator::Pos StackAllocator::mark() {
  Pos res;
  res.block=last;
  res.top=top;
  return res;
}
void StackAllocator::release(Pos pos) {
  last=pos.block;
  top=pos.top;
}
void StackAllocator::clear() {
  BLOCK *act=first;
  while (act) {
    BLOCK *p=act;
    act=act->next;
    free(p);
  }
  top=0;first=last=NULL;
}
StackAllocator::~StackAllocator() {
  clear();
}
StackAllocator::StackAllocator() {
  top=0;first=last=NULL;
//  minblocksize=0x10;
  minblocksize=0x1000;
}


}


