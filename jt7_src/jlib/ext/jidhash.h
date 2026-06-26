#ifndef jidhashH
#define jidhashH

namespace jlib {

struct IdHashItem {
  unsigned int id;
  void *data;
  int next;//memory index of next field
};

struct IdHashTable {
  IdHashTable() {buf=NULL;itemcnt=0;buflen=0;}
  ~IdHashTable();
  void *get(unsigned int id);//hash code
  void add(void *data,unsigned int id);
  void remove(unsigned int id);
  void rehash(int bufsize);
private:
  TCntVec<IdHashItem> memory;
  int *buf;//indexes to memory
  int buflen;
  int itemcnt;

  int _get(unsigned int id);//returns index to memory
  void _add(int memindex);
};

}

#endif
