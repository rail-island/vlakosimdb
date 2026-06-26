#include <jlib.h>
#pragma hdrstop

#include "bms.h"

namespace jlib {

int BLOCKMEMORYSTREAM::freebytes() {
  if (last) return last->capa-last->writepos;
  return 0;
}

int BLOCKMEMORYSTREAM::writedata(const void *abuf,int size) {
  const char *buf=reinterpret_cast<const char*>(abuf);
  int size0=size;
  if (int f=freebytes()) {
    int w=min(size,f);
    last->write(buf,w);
    buf+=w;
    size-=w;
    writepos+=w;
  }
  if (size) {//neveslo se
    allocblock(size);
    last->write(buf,size);
  }
  writepos+=size;
  return size0;
}

int BLOCKMEMORYSTREAM::load(BINREAD &fr) {
  clear();
  int size;
  fr>>size;
  allocblock(size);
  last->write(fr,size);
  return size;
}

void BLOCKMEMORYSTREAM::allocblock(int size) {
  xassert(freebytes()==0);
  newblock(jmax(size,minblocksize));
}

void BLOCKMEMORYSTREAM::newblock(int datasize) {
  BLOCK *b=(BLOCK*)malloc(sizeof(BLOCK)+datasize);
  b->init(datasize,writepos);
  if (first==NULL) first=b;
  if (last) last->next=b;
  last=b;
}

void BLOCKMEMORYSTREAM::clear() {
  BLOCK *act=first;
  while (act) {
    BLOCK *p=act;
    act=act->next;
    free(p);
  }
  first=NULL;last=NULL;writepos=0;readpos=0;readblock=NULL;
}

int BLOCKMEMORYSTREAM::readdata(void *abuf,int size) {
  char *buf=reinterpret_cast<char*>(abuf);
  int readed=0;
  while (readblock and size) {
    int afr=availforread(),r=min(afr,size);
    memcpy(buf,readblock->data()+readpos-readblock->offset,r);
    buf+=r;readpos+=r;size-=r;
    if (afr==r) readblock=readblock->next;
    readed+=r;
  }
  eof=readblock==NULL;
  return readed;
}

int BLOCKMEMORYSTREAM::availforread() {
  if (readblock) return readblock->writepos+readblock->offset-readpos;
  return 0;
}

void BLOCKMEMORYSTREAM::savedata(BINWRITE &fw) {
  BLOCK *act=first;
  while (act) {
    fw.writedata(act->data(),act->writepos);
    act=act->next;
  }
}

}

//#pragma package(smart_init)
