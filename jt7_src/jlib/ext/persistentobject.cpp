#include <jlib.h>
#pragma hdrstop

#include "persistentobject.h"
#include <dir.h>
#include <gram/gmanal.h>

namespace jlib {

static bool po_initialized=false;
int po_maxopenarchives=8;
static int po_lastname=0;

TRCArray<POArchive> openarchives;

static void po_openedarchive(POArchive *archive) {
  openarchives.add(archive);
  while (openarchives.count>po_maxopenarchives) openarchives[0]->closehandle();
}
static void po_closedarchive(POArchive *archive) {
  openarchives.remove(archive);
}


static void po_clearfiles() {
  TCntVec<FileRec> files;
  jsystem->listfiles(basedir/"temp_PO"/"*.*",files);
  for(int i=0;i<files.count;i++) remove((basedir/"temp_PO"/files[i].name).c_str());
}

void po_finalize() {
  po_clearfiles();
  _rmdir(basedir/"temp_PO");
}

string PersistentObject::tempname(string ext) {
  wantinit();
  return sFile(basedir/"temp_PO"/"tmp"+itos(po_lastname++))%ext.c_str();
}

static void po_initalize() {
  po_clearfiles();
  atexit(po_finalize);
  mkdir(basedir/"temp_PO");
  po_initialized=true;
}

void PersistentObject::wantinit() {
  if (!po_initialized) po_initalize();
}

PersistentObject *PersistentObject::openchildpath(string path,bool mustexist) {
  PersistentObject *act=this;
  gram::Analyser a(path.c_str());
  if (*a=='/') act=act->getroot(),a++;
  while (*a) {
    string name=a.readto("/",true);
    if (*a=='/') a++;
    if (name=="..") {
      if (parent) act=parent;
    } else act=act->openchild(name,mustexist);
    if (!act) return NULL;
  }
  return act;
}

PersistentObject *PersistentObject::findchildwithfile(string filepath,string *filename) {
  int pos=filepath.search('/');
  if (pos>=0) {
    string childpath=filepath.copy(0,pos);
    if (filename) *filename=filepath.copy(pos+1,0x1000);
    return openchildpath(childpath);
  }
  if (filename) *filename=filepath;
  return this;
}

PersistentObject *PersistentObject::getroot() {
  PersistentObject *res=this;
  while (res->parent) res=res->parent;
  return res;
}

PersistentObject *PersistentObject::openchild(string name,bool mustexist) {
  PersistentObject *res=findchild(name);
  if (res) {
    res->removed=false;
    return res;
  }
  return NULL;
}

void PersistentObject::removefile(string file) {
  if (!hasfile(file)) return;
  if (PODataFile *data=findfile(file)) {
    data->removed=true;
  } else {
    PODataFile *ren=new PODataFile();//virtualni datafile
    files.add(ren);
    ren->removed=true;
    ren->name=file;
  }
}

void POArchive::updatechild(PersistentObject *child) {
  wantwrite();
  if (ar.findfile(child->name)>=0) ar.erasefile(child->name);
  if (!child->removed) ar.packfile(child->name,child->getfilename());
}
void POArchive::updatedatafile(PODataFile *file) {
  wantwrite();
  if (ar.findfile(file->name)>=0) ar.erasefile(file->name);
  if (!file->removed) {
    if (file->hastext) ar.savetext(file->name,file->text);
    else ar.packfile(file->name,file->filename);
  }
}

void POArchive::wantwrite() {
  if (openmode==2) return;
  if (openmode==1) ar.close();
  openmode=0;
  ar.open(filename);
  po_openedarchive(this);
  openmode=2;
}

void POArchive::wantread() {
  if (openmode>=1) return;
  openmode=0;
  ar.openreadonly(filename);
  po_openedarchive(this);
  openmode=1;
}

void POArchive::closehandle() {
  ar.close();
  po_closedarchive(this);
  openmode=0;
}

void PODirectory::updatedatafile(PODataFile *file) {
  if (file->removed) std::remove(sFile(dirname)/file->name);
  else {
    if (file->hastext) savetextfile(sFile(dirname)/file->name,file->text.c_str());
  }
}

void PODirectory::updatechild(PersistentObject *child) {
  if (child->removed) std::remove(sFile(dirname)/child->name);
}

void PersistentObject::flush() {
  for(int i=0;i<childs.count;i++) childs[i]->flush();
/*
  if (isdirectory) {
    return;
  }
*/
  for(int i=0;i<childs.count;i++) if (childs[i]->changed || childs[i]->removed) {
    changed=true;
    updatechild(childs[i]);
    childs[i]->changed=false;
  }
  for(int i=0;i<files.count;i++) if (files[i]->changed || files[i]->removed) {
    changed=true;
    updatedatafile(files[i]);
    files[i]->changed=false;
  }
  for(int i=0;i<childs.count;i++) if (childs[i]->removed) childs[i]=NULL;
  childs.pack(NULL);
  for(int i=0;i<files.count;i++) if (files[i]->removed) {
    delete files[i];
    files[i]=NULL;
  }
  files.packnull();
  closehandle();
}

//void PersistentObject::closechilds() {
//  while (openchilds.count>0) delete openchilds[0];
//}

/*
void PersistentObject::closed() {
  if (openparent) {
    openparent->closedchild(this);
    openparent=NULL;
  }
}
*/


void PersistentObject::loadxml(string name,XMLNode xml) {
  xml.loadfromtext(loadtext(name));
}

void PersistentObject::savexml(string name,XMLNode xml) {
  MEMSTROUT mso;
  xml.savetotext(mso);
  savetext(name,mso.str());

//  string path=setfile(name);
//  xml.savetofile(path);
//  closedsetfile(path,name);
}

bool PersistentObject::removedfile(string file) {
  if (PODataFile *data=findfile(file)) {
    if (data->removed) return true;
  }
  if (PersistentObject *obj=findchild(file)) {
    if (obj->removed) return true;
  }
  return false;
}

PersistentObject *PODirectory::openchild(string name,bool mustexist) {
  if (PersistentObject *res=PersistentObject::openchild(name,mustexist)) return res;

  if (mustexist) {
    FILE *fr=fopen((dirname+"/"+name).c_str(),"rb");
    if (!fr) return NULL;
    fclose(fr);
  }

  POArchive *res=new POArchive(dirname+"/"+name);
  res->parent=this;
//  res->setparent(this);
  childs.add(res);
//  addchild(res);
  res->name=name;
  res->wantwrite();
  return res;
}

void PODirectory::getfiles(Strings &items) {
  TCntVec<FileRec> files;
  jsystem->listfiles(dirname+"/*.*",files);
  for(int i=0;i<files.count;i++) if (!sFile(files[i].name).testext("zip")) items.add(files[i].name);
  repairfileslist(items);
}
bool PODirectory::hasfile(string file) {
  if (removedfile(file)) return false;
  TCntVec<FileRec> files;
  jsystem->listfiles(sFile(dirname)/file,files);
  return files.count>0;
}
void PODirectory::getchilds(Strings &items) {
  TCntVec<FileRec> files;
  jsystem->listfiles(dirname+"/*.*",files);
  for(int i=0;i<files.count;i++) if (sFile(files[i].name).testext("zip")) items.add(files[i].name);
  repairchildslist(items);
//  for(int i=0;i<childs.count;i++) {
//    if (items.search(childs[i]->name)<0) items.add(childs[i]->name);
//  }
}

PersistentObject *PersistentObject::findchild(string name) {
  for(int i=0;i<childs.count;i++) if (childs[i]->name==name) return childs[i];
  return NULL;
}

void PersistentObject::repairfileslist(Strings &items) {
  for(int i=0;i<files.count;i++) {
    if (files[i]->removed) continue;
    if (items.search(files[i]->name)<0) items.add(files[i]->name);
  }
  for(int i=0;i<items.count;) {
    PODataFile *data=findfile(items[i]);
    if (data && data->removed) items.erase(i);else i++;
  }
}
void PersistentObject::repairchildslist(Strings &items) {
  for(int i=0;i<childs.count;i++) {
    if (childs[i]->removed) continue;
    if (items.search(childs[i]->name)<0) items.add(childs[i]->name);
  }
  for(int i=0;i<items.count;) {
    PersistentObject *child=findchild(items[i]);
    if (child && child->removed) items.erase(i);else i++;
  }
}


void POArchive::getchilds(Strings &items) {
  ArchiveRec rec;
  wantread();
  int cnt=ar.count();
  for(int i=0;i<cnt;i++) {
    ar.get(i,rec);
    sFile file(rec.name);
    if (file.testext("zip")) items.add(rec.name);
  }
  repairchildslist(items);
//  for(int i=0;i<childs.count;i++) if (items.search(childs[i]->name)<0) items.add(childs[i]->name);
}
void POArchive::getfiles(Strings &items) {
  ArchiveRec rec;
  wantread();
  int cnt=ar.count();
  for(int i=0;i<cnt;i++) {
    ar.get(i,rec);
    sFile file(rec.name);
    if (!file.testext("zip")) items.add(rec.name);
  }
  repairfileslist(items);
}
bool POArchive::hasfile(string file) {
  if (removedfile(file)) return false;
  if (PODataFile *res=findfile(file)) {
    if (!res->removed) return true;
  }
  wantread();
  return ar.findfile(file)>=0;
}



PersistentObject *POArchive::openchild(string name,bool mustexist) {
  if (PersistentObject *res=PersistentObject::openchild(name)) return res;

  string newfile=tempname(sFile(name).getext());
  wantread();
  bool exists=ar.findfile(name)>=0;
  if (exists) ar.unpackfile(name,newfile);
  if (mustexist && !exists) return NULL;
  POArchive *res=new POArchive(newfile);
  res->parent=this;
//  res->setparent(this);
//  res->temporary=true;
//  addchild(res);
  childs.add(res);
  res->name=name;
  if (!exists) res->wantwrite();
  return res;
}

PODataFile *PersistentObject::findfile(string name) {
  for(int i=0;i<files.count;i++) if (files[i]->name==name) return files[i];
  return NULL;
}

string POArchive::getfile(string name) {
  if (PODataFile *res=PersistentObject::findfile(name)) {
    res->curusing=true;
    if (res->hastext) {
      res->filename=tempname(sFile(name).getext());
      res->name=name;
    }
    return res->filename;
  }
  string res=tempname(sFile(name).getext());
  wantread();
  ar.unpackfile(name,res);
  PODataFile *f=new PODataFile();
  files.add(f);
  f->curusing=true;
  f->filename=res;
  f->name=name;
  return res;
}
void POArchive::closedgetfile(string path,string name) {
  if (PODataFile *dt=findfile(name)) dt->curusing=false;
}
string POArchive::setfile(string name) {
  if (PODataFile *res=findfile(name)) {
    res->curusing=true;
    res->removed=false;
    if (res->hastext) {
      res->filename=tempname(sFile(name).getext());
      res->name=name;
    }
    return res->filename;
  }
  string res=tempname(sFile(name).getext());
  PODataFile *f=new PODataFile();
  files.add(f);
  f->curusing=true;
  f->filename=res;
  f->name=name;
  return res;
}

void POArchive::closedsetfile(string path,string name) {
  if (PODataFile *res=PersistentObject::findfile(name)) {
    res->changed=true;
    res->curusing=false;
  }
}

string POArchive::loadtext(string name) {
  if (PODataFile *res=findfile(name)) {
    if (res->hastext) return res->text;
    return jlib::loadtext(res->filename.c_str());
  }
  wantread();
  PODataFile *f=new PODataFile();
  files.add(f);
  f->curusing=true;
  f->name=name;
  f->text=ar.loadtext(name);
  f->hastext=true;
  return f->text;
}
void POArchive::savetext(string name,string text) {
  PODataFile *res=findfile(name);
  if (!res) {
    res=new PODataFile();
//    res->filename=tempname(sFile(name).getext());
    res->name=name;
    files.add(res);
  }
  res->removed=false;
  res->changed=true;
  res->curusing=false;
  if (res->filename!="") std::remove(res->filename.c_str());
  res->filename="";
  res->hastext=true;
  res->text=text;
//  savetextfile(res->filename.c_str(),text.c_str());
}


/*
void POArchive::close() {
  closechilds();
  ar.close();

  closed();
  if (temporary) {
    remove(filename.c_str());
    filename="";
  }
}


void POArchive::closedchild(PersistentObject *value) {
  if (ar.findfile(value->childname)>=0) ar.erasefile(value->childname);
  ar.packfile(value->childname,value->getfilename());
  PersistentObject::closedchild(value);
}
*/


}


