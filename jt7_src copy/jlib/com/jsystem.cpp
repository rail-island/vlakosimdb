#include <jlib.h>
#pragma hdrstop

#include "jsystem.h"
#include <gram/gmanal.h>
#include "jreflect.h"
#include <dir.h>

namespace jlib {

JSystem *jsystem=NULL;

static void jsys_showXErrorMessage(const char *msg) {
  jsystem->showerror(msg);
}

JSystem::JSystem() {
  showXErrorMessage=jsys_showXErrorMessage;
  gram::Analyser::static_init();
  rtti::init_rtti();
}

StdMemBlock::StdMemBlock(int asize) {
  size=asize;
  ptr=malloc(size);lockcnt=0;
  if (size>0) xassert(ptr);
}
void *StdMemBlock::lock() {
  lockcnt++;return ptr;
}
void StdMemBlock::unlock() {
  lockcnt--;xassert(lockcnt>=0);
}
int StdMemBlock::getsize() {
  return size;
}
void StdMemBlock::resize(int newsize) {
  xassert(lockcnt==0);ptr=realloc(ptr,newsize);size=newsize;
  if (size>0) xassert(ptr);
}
StdMemBlock::~StdMemBlock() {
  free(ptr);
}
JMemBlock *StdMemBlock::clone() {
  StdMemBlock *res=new StdMemBlock(size);
  memcpy(res->ptr,ptr,size);
  return res;
}


bool JSystem::editvalues(string msg,...) {
  IObjArray<EditItem> items;
  va_list(args);
  va_start(args,msg);
  while (int type=va_arg(args,int)) {
    void *ptr=va_arg(args,void*);
    char *cpt=va_arg(args,char*);
    char *opt="";
    if (type==etCOMBO) opt=va_arg(args,char*);
    items.add(new EditItem(type,ptr,cpt,opt));
  }
  va_end(args);
  return editvalues(items,msg);
}

bool JSystem::makedir(STRING path) {
  return mkdir(path.c_str());
}

}

//#pragma package(smart_init)
