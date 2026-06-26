#ifndef handlemanH
#define handlemanH

namespace jlib {

struct HandleItem {
  int refcnt;
  void *data;
};

struct HandleManager {
  int create(void *data);//returns handle
  void destroy(int handle);
  void addref(int handle);
  void release(int handle);
  void *get(int handle);
private:
  TCntVec<HandleItem> items;
};

}

#endif
