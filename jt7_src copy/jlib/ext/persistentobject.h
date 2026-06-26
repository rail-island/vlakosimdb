#ifndef persistentobjectH
#define persistentobjectH

#include <com/rcobj.h>
#include <ext/jarchive.h>

namespace jlib {

struct PODataFile {
  string filename;
  string name;//jmeno v rodici
  string text;//muze obsahovat text, ten je pak jen v pameti (vubec se nedava na disk)
  bool hastext;//zda obsahuje text (je platna polozka text, neni platna filename)
  bool changed;
  bool curusing;//zda je aktualne potreba
  bool removed;
  PODataFile() {changed=false;curusing=true;hastext=false;removed=false;}
};

struct PersistentObject:RCObject {
  ///vycisti,vyplni detmi
  virtual void getchilds(Strings &items)=0;
  virtual PersistentObject *openchild(string name,bool mustexist=false);
  virtual ~PersistentObject() {}
  virtual void getfiles(Strings &items)=0;

  virtual string getfile(string name)=0;
  virtual void closedgetfile(string path,string name) {}

  virtual string setfile(string name)=0;
  virtual void closedsetfile(string path,string name) {}

  virtual string getfilename() {return "";}
  virtual string loadtext(string name)=0;
  virtual void savetext(string name,string text)=0;

  virtual void updatechild(PersistentObject *child) {}
  virtual void updatedatafile(PODataFile *file) {}

  virtual bool hasfile(string file)=0;
  bool removedfile(string file);

  void remove() {removed=true;}
  ///okamzite fyzicke odebrani souboru
  void removefile(string file);
//  virtual void

  virtual void flush();

  void loadxml(string name,XMLNode xml);
  void savexml(string name,XMLNode xml);

  PersistentObject() {parent=NULL;changed=false;isdirectory=false;removed=false;}
//  void closechilds();

//  void setparent(PersistentObject *value) {openparent=value;}
//  void addchild(PersistentObject *value) {openchilds.add(value);}
//  virtual void closedchild(PersistentObject *value) {openchilds.quickremove(value);}

  PersistentObject *getparent() {return parent;}
  ///vrati prarodice, ktery uz nema rodice
  PersistentObject *getroot();

  virtual void closehandle() {}

  string getname() {return name;}

  static void wantinit();
  static string tempname(string ext);

  PersistentObject *openchildpath(string path,bool mustexist=false);
  //pokud filepath=="child.zip/file.bmp", vrati openchild("child.zip"), do filename da "file.bmp"
  PersistentObject *findchildwithfile(string filepath,string *filename);
private:
  friend class PODirectory;
  friend class POArchive;

  PODataFile *findfile(string name);
  PersistentObject *findchild(string name);
  void repairchildslist(Strings &items);
  void repairfileslist(Strings &items);

  bool isdirectory;
  TRCArray<PersistentObject> childs;
  IObjArray<PODataFile> files;
  PersistentObject *parent;
  string name;
  bool removed;
  bool changed;//zda je jiny obsah nez je ulozeny v rodici

  void closed();
};


struct PODirectory:PersistentObject {
  string dirname;
  PODirectory(string dirname):dirname(dirname) {isdirectory=true;}
  PersistentObject *openchild(string name,bool mustexist=false);
  void getchilds(Strings &items);
  ~PODirectory() {flush();}
  string getfilename() {return dirname;}
  void updatechild(PersistentObject *child);

  string getfile(string name) {return dirname+"/"+name;}
  string setfile(string name) {return dirname+"/"+name;}

  void getfiles(Strings &items);
  bool hasfile(string file);

  void updatedatafile(PODataFile *file);

  string loadtext(string name) {return jlib::loadtext((dirname+"/"+name).c_str());}
  void savetext(string name,string text) {jlib::savetextfile((dirname+"/"+name).c_str(),text.c_str());}
//  void close() {closechilds();closed();}
};

struct POArchive:PersistentObject {
  Archive ar;
  string filename;
  ///zda je to docasny soubor, tj, po close se musi aktualizovat v rodici a smazat
//  bool temporary;
  string getfilename() {return filename;}
  POArchive(string filename):filename(filename) {openmode=0;}
  PersistentObject *openchild(string name,bool mustexist=false);
  void closedchild(PersistentObject *value);
  void close();
  void getchilds(Strings &items);
  ~POArchive() {flush();}
  string getfile(string name);
  void closedgetfile(string path,string name);
  string setfile(string name);
  void closedsetfile(string path,string name);
  string loadtext(string name);
  void savetext(string name,string text);

  void updatechild(PersistentObject *child);
  void updatedatafile(PODataFile *file);

  void getfiles(Strings &items);
  bool hasfile(string file);

  void wantwrite();
  void wantread();
  void closehandle();
protected:
  int openmode;//o:closed,1:readonly,2:readwrite
};

}

#endif
