#include <jlib.h>
#pragma hdrstop

#include "jidhash.h"

namespace jlib {

static bool bad_size(int size) {
  return size%2==0 || size%3==0 || size%5==0 || size%7==0 || size%11==0;
}
static int countopthashsize(int minsize) {
  while (bad_size(minsize)) minsize++;
  return minsize;
}

IdHashTable::~IdHashTable() {
  if (buf) delete []buf;
}

void IdHashTable::rehash(int bufsize) {
  if (bufsize==0) return;
  if (buf) delete []buf;
  buflen=bufsize;
  buf=new int[buflen];
  for(int i=0;i<buflen;i++) buf[i]=-1;
  for(int i=0;i<memory.count;i++) _add(i);
}

int IdHashTable::_get(unsigned int id) {
  if (buflen==0) return -1;
  int index=id % buflen;
  int act=buf[index];
  while (act>=0 && memory[act].id!=id) act=memory[act].next;
  return act;
}

void *IdHashTable::get(unsigned int id) {
  int res=_get(id);
  if (res<0) return NULL;
  return memory[res].data;
}

void IdHashTable::_add(int memindex) {
  int index=memory[memindex].id % buflen;
  int next=buf[index];
  buf[index]=memindex;
  memory[memindex].next=next;
}

void IdHashTable::add(void *data,unsigned int id) {
  if (buflen<memory.count+1) rehash(countopthashsize(2*(memory.count+13)));

  IdHashItem &item=memory.add();
  item.id=id;
  item.data=data;
  item.next=-1;

  _add(memory.count-1);
}

void IdHashTable::remove(unsigned int id) {
  if (buflen==0) return;

  int pos=_get(id);
  if (pos<0) return;//doesn't exist

  int index=id%buflen;

  //disjoining from list
  if (buf[index]==pos) buf[index]=memory[pos].next;
  else {
    int act=buf[index];
    while (act>=0) {
      if (pos==memory[act].next) {
        memory[act].next=memory[pos].next;
        break;
      }
      act=memory[act].next;
    }
  }

  //deleting from memory
  int last=memory.count-1;
  if (last==pos) {//OK
    memory.count--;
    return;
  }

  memory[pos]=memory[last];
  memory.count--;

  //repair lists:last
  int lindex=memory[pos].id%buflen;
  if (buf[lindex]==last) buf[lindex]=pos;
  else {
    int lact=buf[lindex];
    while (lact>=0) {
      if (last==memory[lact].next) {
        memory[lact].next=pos;
        break;
      }
      lact=memory[lact].next;
    }
  }
}


}

