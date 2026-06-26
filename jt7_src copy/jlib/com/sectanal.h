#ifndef sectanalH
#define sectanalH

#include <com/varlist.h>

namespace jlib {

class SECTANAL;

class SECTION {
private:
  friend class SECTANAL;
  TObjVec<SECTION> items;
  char *inbeg,*inafterend;
  char *outbeg,*outafterend;
  SECTANAL *parent;

  void read(STRANAL &q,const char *specbegin,const char *specend,bool main=false);//main==true:inbeg==outbeg&&inafterand==outafterend
  void readsectnameandprops(STRANAL &q,const char *specbegin);
public:
  VARLIST vars;
  char name[0x40];
  struct _EMPTYSECTIONINITIALIZER {};

  bool getpropvalue(const char *name,char *buf);
  void fillinstrlist(STRLIST &s);//vyplni obsah do s,jen pred prvni subsekce
  void init() {parent=NULL;inbeg=NULL;inafterend=NULL;outbeg=NULL;outafterend=NULL;*name=0;}
  SECTION(_EMPTYSECTIONINITIALIZER) {init();}
  SECTION() {init();}
  SECTION &operator[] (int index) {return items[index];}
  SECTION &operator[] (const char *subsect);

  int count() {return items.count;}
  int findsection(const char *subsect);//najde podsekci pomenovanou subsect
  int subseccnt();//vrati,kolik podsekci obsahuje(rekurzeivne)
  SECTION &finditem(int index);//index<subseccnt(),vrati tolikatou sekci

  char *alloctext();
  string text();
  void text(char *buf);

  void clear();
};

class SECTANAL {
public:
  SECTION main;
  SECTANAL() {};
  void read(char *buf,const char *specbegin=":::",const char *specend=NULL);
  //buf musi byt platny ukazatel po celou dobu existence SECTANAL
  //specbegin je retezec,ktery musi byt na zacatku radku pred jmenem sekce
  /*priklad:
    :::sekce1
    neco
    ...
    :::/sekce1
  */
  //pokud specend nezmenen, je to specbegin/
  SECTION &item(int index);//vrati sekci indexatou od zacatku
};


extern SECTION emptysection;

}

#endif
