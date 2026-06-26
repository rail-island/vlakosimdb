#include <jlib.h>
#pragma hdrstop

#include "jobject.h"

namespace jlib {

struct SavedObjectNotKnown {};
struct LoadedObjectNotKnown {};

const int MAGIC=0x1B45FA76;

int JSaver::saveobject(JObject *obj) {
  if (obj==NULL) return -1;
  if (obj->saveindex>=0) return obj->saveindex;
  obj->saveindex=actobj++;
  linked.add(obj);
  return obj->saveindex;
}
JSaver::JSaver(BINWRITE &afw,Ftypeof atypeof) {
  actobj=0;fw=&afw;fw->data=this;typeof_f=atypeof;
}
void JSaver::close() {
  while (linked.count>0) {
    JObject *last=linked.last();
    linked.eraselast();
    saved.add(last);
    int type=typeof_f(last);
    if (type<0) {
//      jsystem->showerror(fmtstr("Saved object not known:%s",typeid(*last).name()));
      jsystem->showerror("Saved object not known");
      throw SavedObjectNotKnown();
    }
    *fw<<type;
    last->save(*fw);
  }
  for(int i=0;i<saved.count;i++) saved[i]->saveindex=-1;
  *fw<<-1;//zakazana identifikace objektu
  *fw<<MAGIC;
}
JLoader::JLoader(BINREAD &afr,Floadobj aloadobj) {
  fr=&afr;toload=0;fr->data=this;loadobj=aloadobj;data=NULL;
}
void JLoader::connect() {
  for(int i=0;i<links.count;i++) {
    LINK &l=links[i];
    int index=*reinterpret_cast<int*>(l.ptr);
    xassert(index==l.gid);
    xassert(index>=0);
    for(int j=0;j<objs.count;j++) if (index==objs[j]->saveindex) {
      *reinterpret_cast<JObject**>(l.ptr)=objs[j];
    }
  }
}

void JLoader::close() {
  int type=fr->readi();
  while (type>=0) {
    JObject *loaded=loadobj(type,*fr);
    if (loaded==NULL) {
      jsystem->showerror(fmtstr("Loaded object not known:%i",type));
      throw LoadedObjectNotKnown();
    }
    type=fr->readi();
    objs.add(loaded);
  }
  fr->magic(MAGIC);
  connect();
  for(int i=0;i<objs.count;i++) objs[i]->saveindex=-1;
}

int JLoader::loadobject() {
  return fr->readi();
}

}

//#pragma package(smart_init)
