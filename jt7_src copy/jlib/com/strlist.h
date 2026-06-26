#ifndef HSTRLIST
#define HSTRLIST

#include <com/com0.h>
#include <com/strcom.h>
#include <com/arrays.h>
#include <com/strms.h>
//#delbeg [ABAZ]
//#include <com/linet.h>
//#delend [ABAZ]

namespace jlib {


class NATTBL;

struct STRLISTBASE {
  static const bool wantlocalcopy=true;
  static const bool wantdelete=true;
  static const bool wantinitialize=false;
  void makelocalcopy(char *&dest,const char *src) {dest=strnew(src);};
  void deletelocalcopy(char *item) {free(item);};
  void initialize(char *&dest) {dest=NULL;}
};

struct STRLIST:MemoryArray<char*,STRLISTBASE >
//#delbeg [ABAZ]
//               ,public LINET
//#delend [ABAZ]
{
public:
  int actline;
//#delbeg [ABAZ]
//  virtual void readlinetobuf();
//  void resetlinet() {actline=0;eof=false;readlinetobuf();}
//  void closelinet() {actline=-1;eof=true;}
//#delend [ABAZ]

  void set(int index,const char *c)
  {
    xcheckindex(index,count);
    free(buf[index]);buf[index]=strnew(c);
  }
  void setallocatedbuf(int index,char *c) {
    xcheckindex(index,count);
    free(buf[index]);buf[index]=c;
  }
  #define _mac_cat(arg1,arg2)\
  {\
    xcheckindex(index,count);\
    char *w=(char*)malloc(strlen(buf[index])+strlen(c)+1);\
    strcpy(w,arg1);strcat(w,arg2);\
    free(buf[index]);\
    buf[index]=w;\
  }
  void catr(int index,char *c) _mac_cat(buf[index],c);
  //items[index]=items[index]+c
  void catl(int index,char *c) _mac_cat(c,buf[index]);
  //items[index]=c+items[index]
  int load(const char *file);
  int save(const char *file);
  void asort();
//  int bsrch(const char *what);
  int search(const char *what);
  int findfirstsubstr(char *substr);//nalezne prvni retezec,ktery zacina substr
  void addfrombuf(const char *buf);
  void assignfrombuf(const char *buf);
  char *savetobuf();
  char *savetobuf(int beg,int end);
  //vytvori buffer typu char * a ulozi tam obsah
  chrs autotext(int beg,int end);
  chrs autotext();
  //ulozi radku beg..end(resp. vsechny) do chrs

  string gettext(int beg,int end,const char *sep);
  string gettext(const char *sep) {return gettext(0,count-1,sep);}
  //ulozi radky od beg do end vcetne, mezi radky je oddelovac sep
  void settext(const char *text,char sep);
  void erasetrimablelines();

  int bufsize();
  int bufsize(int beg,int end);
  void print(FILE *fw);
  void trimprint(FILE *fw);//aplikuje na radky trimr,vytiskne do fw
  void changewithtbl(char map[0x100]);
  STRLIST(const char *filename) {load(filename);actline=-1;}
  STRLIST() {actline=-1;};
  char **getbuf() {return buf;}
  char *getvalue(const char *var);
  int findvar(const char *var);
  void trimaddanddividelines(STRLIST &s);
  //prida trimred radky z s,pokud na nejake radce je \\n,rozdeli ji
  void natsort(NATTBL *tbl);//je definovano v type.cpp
  #undef _mac_cat

  char *add(const char *what) {return _add()=strnew(what);}
  char *add(string what) {return _add()=strnew(what.c_str());}
  char *insert(int index,const char *what) {return _insert(index)=strnew(what);}
  char *insert(int index,string what) {return _insert(index)=strnew(what.c_str());}
  void operator +=(const char *what) {add(what);}
  void operator +=(string what) {add(what);}
  char *add(char *what) {return _add()=strnew(what);}
  char *insert(int index,char *what) {return _insert(index)=strnew(what);}
  void add(STRLIST &src,int begin,int afterend) {_add(src,begin,afterend);}
  void add(STRLIST &src) {_add(src,0,src.count);}
  void operator +=(STRLIST &src) {add(src);}
};

//#delbeg [ABAZ]
//STRLIST &readallfiles(char *mask);//FA_HIDDEN | FA_SYSTEM);
//void readallfiles(STRLIST &res,char *mask,bool addpath=false);//FA_HIDDEN | FA_SYSTEM);
//#delend [ABAZ]

bool islinevar(const char *line,const char *var);

}

#endif
