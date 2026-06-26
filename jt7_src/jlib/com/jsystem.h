#ifndef jsystemH
#define jsystemH

namespace jlib {

struct JMemBlock {//zapouzdruje blok pameti
  virtual void *lock()=0;
  virtual void unlock()=0;
  virtual int getsize()=0;
  virtual void resize(int size)=0;//musi byt volano na odemknuty blok
  virtual ~JMemBlock() {};
  virtual JMemBlock *clone()=0;
};

struct StdMemBlock:JMemBlock {
  void *ptr;int size;int lockcnt;
  StdMemBlock(int size);
  void *lock();
  void unlock();
  int getsize();
  void resize(int newsize);
  ~StdMemBlock();
  JMemBlock *clone();
};

struct FileRec {
  char name[0x100];
  bool isdir;
  int time;
};

struct JSystem {
  virtual void showinfo(string msg) {puts(msg.c_str());}
  virtual void showerror(string msg) {puts(msg.c_str());}
  virtual void showwarning(string msg) {puts(msg.c_str());}
  virtual string ask(string question,string def) {return def;}
  virtual bool getopenfn(string filter,string initialdir,string &res) {return false;}
  virtual bool getsavefn(string filter,string initialdir,string &res) {return false;}
  virtual bool edittext(string &text,int *position=NULL) {return false;}
  virtual bool dialogyn(string text) {return false;}
  virtual int dialogync(string text) {return 2;}
  virtual bool dialogar(string text) {return 0;}
  virtual bool dialogoc(string text) {return 1;}
  virtual void showwait(string msg) {}
  virtual void hidewait() {}
  virtual string listfiles(STRING directory) {return "";}//vrati soubory oddelene strednikem
  //directory je ve tvaru c:\\*.txt
  virtual string listdirs(STRING directory) {return "";}
  virtual void listfiles(STRING directory,TCntVec<FileRec> &files) {}
  virtual void copyfile(STRING oldname,STRING newname) {}
  virtual bool makedir(STRING path);
  virtual void uppercase(char *c) {while (*c) *c=toupper(*c),c++;}
  virtual void lowercase(char *c) {while (*c) *c=toupper(*c),c++;}
  virtual int comparestr(const char *s1,const char *s2) {return strcmp(s1,s2);}
  virtual int comparetext(const char *s1,const char *s2) {return strcmp(s1,s2);}
  //nezavisle na velikosti pismen
  virtual JMemBlock *tempblock(int size=0) {return new StdMemBlock(size);}
  //alokuje blok takovy, ze by nemel zabirat porad misto v pameti
  struct EditItem {
    int type;void *ptr;
    string cpt;string opt;
    EditItem(int atype,void *aptr,string acpt,string aopt="") {type=atype;ptr=aptr;cpt=acpt;opt=aopt;}
  };
  static const int etEND=0,etSTRING='s',etFLOAT='f',etDOUBLE='d',etINT='i',etCOMBO='c',etOPEN='o',etSAVE='a';
  /*typy:
  STRING *string caption
  FLOAT *float caption
  DOUBLE *double caption
  INT *int caption
  COMBO *int caption values//je to combobox,values jsou oddeleny strednikem
  OPEN *string caption//opendialog
  SAVE *string caption//savedialog
  */
  virtual bool editvalues(Array<EditItem*> &items,string msg) {return false;}
  bool editvalues(string msg,...);
  //argumenty:int ptr char* [char*], ukonceno END,[char*] je pritomen v zavislosti na typu
  string getopenfn(string filter,string initialdir) {
    string res;
    if (getopenfn(filter,initialdir,res)) return res;
    return "";
  }
  string getsavefn(string filter,string initialdir) {
    string res;
    if (getsavefn(filter,initialdir,res)) return res;
    return "";
  }
  JSystem();
};

extern JSystem *jsystem;

}

#endif
