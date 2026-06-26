#include <jlib.h>
#pragma hdrstop

#include "charadd.h"

namespace jlib {


void CHARADD::add(const char *str) {
  int len=strlen(str);
  testspace(len);
  memmove(act,str,len);
  act+=len;
  *act=0;
}
void CHARADD::add(STRLIST &s) {
  for(int i=0;i<s.count;i++) addln(s[i]);
}

void CHARADD::addendedstr(const char *str,char end) {
  int len=strlen(str);
  testspace(len+1);
  memmove(act,str,len);
  act+=len;
  *act++=end;
  *act=0;
}

void CHARADD::trimaddendedstr(const char *str,char end) {
  int len=strlen(str);
  testspace(len+1);
  memmove(act,str,len);
  char *a2=act+len;
  while (a2>act and *a2<=' ') a2--;
  act=a2;
  if (end) *act++=end;
  *act=0;
}

void CHARADD::addfix(const char *s,int len) {
  testspace(len+1);
  for(;len and *s;len--,s++,act++) *act=*s;
  if (len) addchar(' ',len);
  *act=0;
}

void CHARADD::addchar(char c,int num) {
  testspace(num+1);
  for(;num;num--,act++) *act=c;
  *act=0;
}


}
//#pragma package(smart_init)

