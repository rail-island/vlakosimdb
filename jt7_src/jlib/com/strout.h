#ifndef stroutH
#define stroutH

#include "bms.h"

namespace jlib {


struct STROUT {
  struct CHAR {
    char ch;
    explicit CHAR(char c) {ch=c;}
  };
  struct QSTR {
    char *buf;
    explicit QSTR(const char *abuf) {buf=(char*)abuf;}
  };
  STROUT &operator <<(const char *text) {
    flushbuf();
    strm->writedata(text,strlen(text));
    return *this;
  }
  STROUT &operator <<(STRING text) {return operator <<(text.c_str());}
  STROUT &operator <<(int i) {
    if (bufcapa()<0x20) flushbuf();
    sprintf(bufw,"%d",i);
    bufw=strend(bufw);
//    char s[0x100];sprintf(s,"%d",i);strm->writedata(s,strlen(s));
    return *this;
  }
  STROUT &operator <<(char c) {
    if (bufw>=buf+BUFSIZE) flushbuf();
    *bufw++=c;
    return *this;
  }
  STROUT &operator <<(double f) {
    if (bufcapa()<0x20) flushbuf();
    sprintf(bufw,"%f",f);
    bufw=strend(bufw);
//    char s[0x100];sprintf(s,"%f",f);strm->writedata(s,strlen(s));
    return *this;
  }
  STROUT &operator <<(CHAR c) {
//    strm->writedata(&c,1);
    if (bufw>=buf+BUFSIZE) flushbuf();
    *bufw++=c.ch;
    return *this;
  }
  STROUT &operator <<(string text) {
    flushbuf();
    strm->writedata(text.c_str(),text.length());
    return *this;
  }
  STROUT &operator <<(QSTR str);//zapise retezec v uvozovkach(+zdoji pripadne uvozovku unvitr)
  STROUT(BINWRITE *f) {strm=f;bufw=buf;}
  ~STROUT() {flushbuf();}
  void writecstring(const char *s,char edge='"');
protected:
  BINWRITE *strm;
  static const int BUFSIZE=0x100;
  char buf[BUFSIZE];
  char *bufw;
  inline void flushbuf() {if (bufw>buf) {if (bufw>buf) strm->writedata(buf,bufw-buf);bufw=buf;}}
  inline int bufcapa() {return BUFSIZE-(bufw-buf);}
};

struct MEMSTROUT:STROUT {
  BLOCKMEMORYSTREAM bms;
  MEMSTROUT():STROUT(&bms) {}
  string str();
  void save(const char *file) {savetextfile(file,str().c_str());}
  void clear() {bms.rewrite();bufw=buf;}
};

struct FILESTROUT:STROUT {
  BINWFILE fw;
  FILESTROUT(const char *file,const char *openmode="w"):fw(file,openmode),STROUT(&fw) {}
  ~FILESTROUT() {flushbuf();}
};


}
#endif
