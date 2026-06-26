#include "callfn.h"

void __stdcall neco(void* name){}

void *callfunction(void **params,void *function) {
  while (*params) {
    neco(*params);
    params++;
  }
}


 