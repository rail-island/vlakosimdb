#include <jlib.h>
#pragma hdrstop

#include "makecode.h"

namespace jlib {


static void makecodepart(char *s,char *code,int len,char *codemap)
{
  char *c=s,*a=code;
  while (*c && a-code<len)
  {
    register char chsug=codemap[(uchar)*c];
    if (chsug>='A' && chsug<='Z') *a++=chsug;
    if (*c>='0' && *c<='9') *a++=*c;
    c++;
  }
  while (a-code<len) *a++='_';
  *a=0;
}

void makecode(char *codemap,char *suggest,int partnum,...)
{
  *suggest=0;
  va_list args;
  va_start(args,partnum);
  for(int i=0;i<partnum;i++)
  {
    char *c=va_arg(args,char*);
    int len=va_arg(args,int);
    char s[0x100];
    makecodepart(c,s,len,codemap);
    strcat(suggest,s);
  }
  va_end(args);
}


}
#pragma package(smart_init)
