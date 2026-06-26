#include <jlib.h>
#pragma hdrstop

#include "zbrt.h"
#include "zbobj.h"
#include <ext/garbcol.cpp>
#include "abazparse.h"

namespace zaba {

void zbc_jmpemptylines(Analyser &a) {
  while (a.seekch(';') || a.seekch('\n')) a.roll();
}

zbObject *StdInstanceCreator::createInstance(Runtime *rt,STRING intf) {
  return new zbInstance(rt,intf);
}

Interface *Runtime::searchintf(STRING name) {
  Interface *res=intfs.searchvalue(name);
  if (res) return res;
  if (*name)
    throw JError(fmtstr("Interface not found:%s",name));
  return emptyintf;
}

void Runtime::addfile(string file) {
  file=file.getupper();
  if (filestoload.search(file)<0) filestoload.add(file);
}

void Runtime::adddir(string dir) {
  string files=jsystem->listfiles(dir/"*.zbc")+jsystem->listfiles(dir/"*.zb");
  for(lineiter it=files.getlines(';');*it;it++) addfile(sFile(dir)/ *it);
}

void Runtime::addsub(STRING name,Native_Sub sub) {
  SubImpl &impl=subs.add();
  strcpy(impl.name,name);
  xcheckstrlen(impl.name);
  impl.sub=sub;
}

Native_Sub Runtime::searchsub(STRING name) {
  for(int i=0;i<subs.count;i++) if (name==subs[i].name) return subs[i].sub;
  return NULL;
}

bool Runtime::runinitproc() {
  if (initrequests.empty()) return false;
  InitProcRequest *r=initrequests.firstelem();
  callMethod(r->obj,r->name,NULL,0);
  delete r;
  return true;
}

void Runtime::addinitproc(zbObject *obj,STRING name,int priority) {
  InitProcRequest *r=new InitProcRequest(name,priority,obj);
  
  TDVSIter<InitProcRequest> first=initrequests.first();//prvni prvek se stejnou prioritou
  while (*first && first->priority<priority) first++;

  while (*first && first->priority==priority) {
    TDVSIter<InitProcRequest> runner=first;
    while (*runner && runner->priority==priority) {
      if (obj->intf->requires(runner->obj->intf->name)) goto require;
      runner++;
    }
    goto insert;
    require:
    first++;
  }
  insert:
  r->insmebefore(first.act);
}

string Runtime::getfilefromstack() {
  for(int i=0;i<filestoload.count;i++) if (loadedfiles.search(filestoload[i])<0) return filestoload[i];
  throw JError("Error loading files");
}

void Runtime::loadedlib() {
  while (loadedfiles.count<filestoload.count) loadfile(getfilefromstack());

  TCntVec<Interface*> toinit;
  for(int i=0;i<intfs.count;i++) if (!intfs[i]->initialized) toinit.add(intfs[i]);

  TCntVec<Interface*> includesort,rest;
  //seradit dle  cestecneho usporadani daneho include
  rest.assign(toinit);
  while (rest.count>0) {
    bool finded=false;
    for(int i=0;i<rest.count;i++) if (rest[i]->includesonly(includesort)) {
      includesort.add(rest[i]);rest.erase(i);finded=true;break;
    }
    if (!finded) throw JError("interface include cycle");
  }

  for(int i=0;i<includesort.count;i++) includesort[i]->makeinclude();

  for(;;) {
    bool worked=false;
    bool all=true;
    for(int i=0;i<toinit.count;i++) {
      if (toinit[i]->makecreator()) worked=true;
      if (!toinit[i]->hascreator()) all=false;
    }
    if (all) break;
    if (!worked) throw JError("interface creatoras cycle");
  }

  for(int i=0;i<toinit.count;i++) {
    if (toinit[i]->autocreate) {
      zbObject *obj=toinit[i]->create();
      obj->release();
    }
  }

  for(int i=0;i<toinit.count;i++) toinit[i]->initialized=true;

  while (runinitproc()) ;
}

zbObject *Runtime::createinstance(STRING intf) {
  return searchintf(intf)->create();
}

void Runtime::addcreator(STRING name,InstanceCreator *creator) {
  mycreators.add(new MyCreator(name,creator));
}

InstanceCreator *Runtime::searchcreator(STRING name) {
  MyCreator *res=mycreators.searchvalue(name);
  if (res) return res->creator;
  return NULL;
}

Runtime::Runtime() {
  gc=new GarbageCollector<zbObject>();
  stdcreator=new StdInstanceCreator();
  emptyintf=new Interface(this);strcpy(emptyintf->name,"Interface.empty");intfs.add(emptyintf);
  sysintf=new Interface(this);strcpy(sysintf->name,"System.instance");intfs.add(sysintf);
  stringintf=new Interface(this);strcpy(stringintf->name,"String.instance");intfs.add(stringintf);
  numberintf=new Interface(this);strcpy(numberintf->name,"Number.instance");intfs.add(numberintf);
  arrayintf=new Interface(this);strcpy(arrayintf->name,"Array.instance");intfs.add(arrayintf);
  voidintf=new Interface(this);strcpy(voidintf->name,"Object.instance");intfs.add(voidintf);
  void_obj=new zbObject(this,"Object.instance");
  sys_obj=new zbSystem(this);
}
Runtime::~Runtime() {
  sys_obj->release();
  void_obj->release();
  #ifdef INTERPRETDEBUG
  eputs("Garbage Collector:\n&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
  #endif
  stdcreator=NULL;
  intfs.clear();mycreators.clear();
  gc->run();
  delete gc;
//  free(gc);
  gc=NULL;
}

void Runtime::addintf(STRING name) {
  Interface *intf=new Interface(this);
  strcpy(intf->name,"ApplicationClass.instance");
  intfs.add(intf);
}

void Runtime::setargs(int argc,char **argv) {
  zbArray *array=new zbArray(this);
  for(int i=0;i<argc;i++) array->items.add(createString(argv[i],this));
  sys_obj->setprop("arguments",array);
  array->release();
}

void Runtime::load(Analyser &a) {
  while (*a) {
    if (a.begjmp("interface")) {
      char name[0x100];
      a.jsp();
      a.readto(name,0x100,": \n\r",false);
      a.roll();
      Interface *intf=intfs.psearchv(name);
      if (!intf) {
        intf=new Interface(this);intfs.add(intf);
        strcpy(intf->name,name);
      }
      intf->load(a);
    } else {
      a.jsp();
      if (*a==';') a.roll();
      else if (a.seekch()) a.roll();else a.error("interface expected");
    }
  }
}

void Runtime::uselib(string dir) {
  adddir(sFile(loadingfile).getdir()/dir);
}
void Runtime::usemod(string file) {
  addfile(sFile(loadingfile).getdir()/file);
}

void Runtime::loadfile(string file) {
  loadingfile=file;
  loadedfiles.add(file);
  if (sFile(file).testext("zb")) {
    try {
      loadzbfile(file);
    } catch (CommonParseError &pe) {
      pe.file=file;
      throw JError(pe.getmsg());
    }
  }
  if (sFile(file).testext("zbc")) loadzbcfile(file);
  loadingfile="";
}

void Runtime::loadzbfile(string file) {
  string text=loadtext(file.c_str());
  P_Module *res=abazparse(text);
  if (res) {
    ZBCWriter zbcw;
    res->write(zbcw,sFile(file).getfile()%"");
    string code=zbcw.getcode();
    Analyser a(code.c_str());
    load(a);
  }
}

void Runtime::loadzbcfile(string file) {
  string text=loadtext(file.c_str());
  Analyser a(text.c_str());
  try {
    load(a);
  } catch (AnalError &e) {
    CommonParseError pe;
    pe.init(text,e.msg,e.pos-text.c_str(),file);
    throw pe;
  }
}

}


