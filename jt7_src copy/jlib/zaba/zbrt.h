#ifndef zbrtH
#define zbrtH

#include "zbintf.h"
#include <ext/garbcol.h>

namespace zaba {

struct SubImpl {
  Native_Sub sub;
  char name[0x100];
};

struct StdInstanceCreator:InstanceCreator {
  zbObject *createInstance(Runtime *rt,STRING intf);
};

struct MyCreator {
  RefBase<InstanceCreator> creator;
  char name[0x20];
  MyCreator(STRING aname,InstanceCreator *acreator) {creator=acreator;strcpy(name,aname);xcheckstrlen(name);}
  bool match(STRING aname) {return aname==name;}
};

struct InitProcRequest:DVSElem {
  string name;
  int priority;
  RefBase<zbObject> obj;
  InitProcRequest(string name,int priority,zbObject *obj):priority(priority),obj(obj),name(name) {}
};

struct Runtime {
  void addsub(STRING name,Native_Sub sub);
  void addcreator(STRING name,InstanceCreator *creator);

  Native_Sub searchsub(STRING name);
  ///vrati vzdy platne Interface, prip. vyhodi vyjimku
  Interface *searchintf(STRING name);
  InstanceCreator *searchcreator(STRING name);
  zbObject *getvoid() {return void_obj;}
  zbObject *getsys() {return sys_obj;}

  void uselib(string dir);
  void usemod(string file);

  void addfile(string file);
  void adddir(string dir);

  //vola se, aby se inicializovaly nactene interface, ktere jeste nebyly inicializavany
  void loadedlib();
//  void linkloaded();
  void addinitproc(zbObject *obj,STRING name,int priority);
  Runtime();
  ~Runtime();
  GarbageCollector<zbObject> *getgc() {return gc;}
  zbObject *createinstance(STRING intf);
  InstanceCreator *getstdcreator() {return stdcreator;}
  sFile getrootdir() {return rootdir;}
  void setargs(int argc,char **argv);
  void addintf(STRING name);

  sFile rootdir;
private:
  TDVSList<InitProcRequest> initrequests;
  RefBase<InstanceCreator> stdcreator;
  GarbageCollector<zbObject> *gc;

  Interface *emptyintf,*sysintf,*stringintf,*numberintf,*arrayintf,*voidintf;

  zbObject *void_obj,*sys_obj;
  IObjArray<Interface> intfs;
  IObjArray<MyCreator> mycreators;
  TCntVec<SubImpl> subs;

  //nacte vsechny soubory z adresare
//  void loaddir(const sFile &dir);
  void load(Analyser &a);
  void loadfile(string file);
  void loadzbfile(string file);
  void loadzbcfile(string file);
  string getfilefromstack();

  Strings filestoload;
  Strings loadedfiles;

  string loadingfile;
  //spusti init proc a vyjme ji ze spojaku, vrati, zda jeste neco udelalo
  bool runinitproc();
};

void zbc_jmpemptylines(Analyser &a);

}

#endif
