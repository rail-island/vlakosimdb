#include <jlib.h>
#pragma hdrstop

#include "handleman.h"

namespace jlib {

int HandleManager::create(void *data) {
  int res=-1;
  for(int i=0;i<items.count;i++) if (items[i].refcnt==0) res=i;
  if (res<0) res=items.count,items.add();
  items[res].data=data;
  items[res].refcnt=0;
  return res;
}
void HandleManager::destroy(int handle) {
  items[handle].data=NULL;
}
void HandleManager::addref(int handle) {
  items[handle].refcnt++;
}
void HandleManager::release(int handle) {
  items[handle].refcnt--;
  xassert(items[handle].refcnt>=0);
}
void *HandleManager::get(int handle) {
  return items[handle].data;
}


}


