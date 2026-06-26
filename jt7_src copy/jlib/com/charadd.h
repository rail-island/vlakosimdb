#ifndef HCHARADD
#define HCHARADD
#include <com/com0.h>
#include <com/strlist.h>


namespace jlib {


struct CHARADD {
  char *act;char *buf;
  char *bufend;
  void testspace(int bytecnt) {xassert(act+bytecnt<bufend);}
  CHARADD(char *abuf,int buflen) {buf=act=abuf;bufend=buf+buflen;testspace(1);*act=0;}
  void reset() {act=buf;*act=0;}
  void add(const char *str);
  void add(STRLIST &s);
  void operator +=(const char *str) { add(str); }
  void operator +=(STRLIST &s) { add(s); }
  void operator +=(string str) { add(str.c_str()); }
  void operator /=(const char *str) { addln(str); }
  void operator /=(string str) { addln(str.c_str()); }
  void jmptonextstr() { testspace(1);act++;*act=0;}
  void operator ++(int) { jmptonextstr(); }
  void addendedstr(const char *str,char end);//prida str a za nej znak end
  void addln(const char *str) {addendedstr(str,'\n');}
  void trimaddendedstr(const char *str,char end);
  void trimaddln(const char *str) {trimaddendedstr(str,'\n');}
  void trimadd(const char *str) {trimaddendedstr(str,0);}

  void addfix(const char *s,int len);//prida s na fix znaku,bud oreze,nebo noplni zprava mezerani
  void addchar(char c,int num);//prida num znaku c

//  CHARADD &operator <<(char ch) {testspace(1);*act++=ch;*act=0;return *this;}
  CHARADD &operator <<(int i) {testspace(0x10);sprintf(act,"%d",i);act=strend(act);return *this;}
  CHARADD &operator <<(const char *s) {add(s);return *this;}
};

}

#endif
