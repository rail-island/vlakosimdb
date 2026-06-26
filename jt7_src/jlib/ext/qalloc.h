#ifndef qallocH
#define qallocH

namespace jlib {

struct QAllocator {
  void *alloc();
  void free(void *p);
  QAllocator(int itemsize);
  ~QAllocator();
private:
  struct BIGBLOCK;
  struct FREEBLOCK;
  BIGBLOCK *bigs;
  FREEBLOCK *frees;

  int itemsize;
  int itemstoalloc;
  int items_to_alloc() {return itemstoalloc;}
};


struct StackAllocator {//nema uvolnovani, analogicka praci s haldou jako Mark a Release v TP
  struct BLOCK;
  struct Pos {
    BLOCK *block;
    int top;
  };

  void *alloc(int size);
  Pos mark();
  void release(Pos pos);
  ~StackAllocator();
  StackAllocator();
  void clear();
private:
  BLOCK *first,*last;
  int top;
  int minblocksize;

  BLOCK *allocblock(int reqsize);
};

}

#endif
