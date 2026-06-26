#include <winlib.h>
#pragma hdrstop

#include "_winalloc.h"

const int ALLOC_MAGIC=0x68F1EA0B;
const int FREE_MAGIC =0xA5BB0B1A;

const int FILL_ALLOC=0xAB,FILL_FREE=0xCD;

struct HDR {
  int magic;
};

static void error(const char *error) {
  MessageBox(0,error,"Chyba",MB_OK);
}

struct FreeBlock {
  void *data;
  int size;
  bool check() {
    int rnd=26351;
    uchar *ar=(uchar*)data;
    for(int i=0;i<size;i++) {
      if (ar[i]!=(uchar)rnd) return false;
      rnd=(rnd*3743) % 37821;
    }
    return true;
  }
  void fill() {
    int rnd=26351;
    uchar *ar=(uchar*)data;
    for(int i=0;i<size;i++) {
      ar[i]=(uchar)rnd;
      rnd=(rnd*3743) % 37821;
    }
  }
};
const int FREESIZE=0x100000;
struct FreeArray {
  FreeBlock freed[FREESIZE];
  int count;
  void add(FreeBlock fb) {
    if (count>=FREESIZE) {error("to many free blocks");throw 0;}
    freed[count]=fb;count++;
  }
  FreeBlock &operator [](int index) {return freed[index];}
} freed;


void checkfreed() {
  for(int i=0;i<freed.count;i++) {
    FreeBlock &b=freed[i];
    if (!b.check()) {
      char s[0x100];
      sprintf(s,"Chyba - prepsana pamet na %8X",b.data);
      error(s);throw 0;
    }
  }
}

static bool nahoda() {
  static int x=0;
  x++;x%=100;
  return x==0;
}
static bool nahoda2() {
  return false;
  static int x=0;
  x++;x%=2;
  return x==0;
}

extern "C" void *std::malloc(size_t size) {
  if (nahoda()) checkfreed();
  if (nahoda2()) {
    void *data=malloc(20);
    FreeBlock fd;fd.data=data;fd.size=20;fd.fill();freed.add(fd);
  }
  void *res=LocalAlloc(LMEM_FIXED,size+sizeof(int));
  ((HDR*)res)->magic=ALLOC_MAGIC;
  FillMemory((HDR*)res+1,size,FILL_ALLOC);
  return (HDR*)res+1;
}

extern "C" void std::free(void *p) {
  if (nahoda()) checkfreed();
  if (!p) return;
  HDR *base=(HDR*)p-1;
//  xassert(base->magic!=FREE_MAGIC);
//  xassert(base->magic==ALLOC_MAGIC);
  if (base->magic!=ALLOC_MAGIC) {
    char s[0x100];
    sprintf(s,"Chyba:spatne magic:magic==%08X",base->magic);
    if (base->magic==FREE_MAGIC) strcat(s,"\nmagic==freemagic");
    error(s);
    throw 1;
  }
//  xassertmsg(base->magic==ALLOC_MAGIC,("magic==%08X",base->magic));
//  base->magic=FREE_MAGIC;
//  FillMemory(p,LocalSize(base)-sizeof(HDR),FILL_FREE);
  FreeBlock fr;
  fr.data=base;fr.size=LocalSize(base);
  fr.fill();
  freed.add(fr);
//  if (LocalFree(base)) throw 0;
}

extern "C" void *std::realloc(void *block, size_t size) {
  if (!block) return std::malloc(size);
  if (size==0) {std::free(block);return NULL;}
  void *res=std::malloc(size);
  int blsize=LocalSize((HDR*)block-1)-sizeof(HDR);
  if (size>0 && blsize>0) memcpy(res,block,min(size,(size_t)blsize));
  std::free(block);
//  if (res) ((HDR*)res)[-1].magic=ALLOC_MAGIC;
  return res;
}

#pragma package(smart_init)
