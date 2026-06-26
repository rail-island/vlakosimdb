#ifndef wsystemH
#define wsystemH

#include <com/jsystem.h>

namespace wlib {

void initbasedir();

struct WSystem:jlib::JSystem {
  void showinfo(string msg);
  void showerror(string msg);
  void showwarning(string msg);
  string ask(string question,string def);
  bool getopenfn(string filter,string initialdir,string &res);
  bool getsavefn(string filter,string initialdir,string &res);
  bool edittext(string &text,int *position=NULL);
  bool dialogyn(string text);
  int dialogync(string text);
  bool dialogar(string text);
  bool dialogoc(string text);
  bool makedir(jlib::STRING path);
  WSystem() {jsystem=this;initbasedir();}
  bool editvalues(Array<EditItem*> &items,string msg);
  void showwait(string msg);
  void hidewait();
  JMemBlock *tempblock(int size);
  string listfiles(jlib::STRING directory);
  void listfiles(jlib::STRING directory,TCntVec<FileRec> &files);
  string listdirs(jlib::STRING directory);
  void uppercase(char *c) {CharUpper(c);}
  void lowercase(char *c) {CharLower(c);}
  int comparestr(const char *s1,const char *s2) {return CompareString(LOCALE_USER_DEFAULT,0,s1,strlen(s1),s2,strlen(s2))-2;}
  int comparetext(const char *s1,const char *s2) {return CompareString(LOCALE_USER_DEFAULT,NORM_IGNORECASE,s1,strlen(s1),s2,strlen(s2))-2;}
  void copyfile(jlib::STRING oldname,jlib::STRING newname);
protected:
  virtual HANDLE getactparent() {return 0;}
  bool getfilename(string initialdir,string filter,bool save,char buf[0x200]);
};

WSystem *createwsystem();

}

#endif
