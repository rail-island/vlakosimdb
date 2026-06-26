#include <jlib.h>
#pragma hdrstop

#include "textanal.h"

namespace jlib {


char *readelementinbrackets(char *buf,char begin,char separator,char end,char *outbuf)
{
  char *c=buf;
  if (*c==begin) c++;
  char *a=outbuf;
  while (*c && *c!=separator && *c!=end) *a++=*c++;
  *a=0;
  if (*c==separator && c[1] && c[1]!=end) return c+1;else return NULL;
}

int findvarinvars(char *varlist,char *var)
{
  if (!*var) return -1;
  char *v=varlist;
  for(int i=0;v;i++)
  {
    char s[32],*ve=strchr(v,';');
    if (ve) strecpy(s,v,ve);else strcpy(s,v);
    if (ve) v=ve+1;else v=NULL;
    if (eqstr(s,var)) return i;
  }
  return -1;
}

int strtoenum(char *enumstrs,char *unknownstr)
{
  return findvarinvars(enumstrs,unknownstr)+1;
}

chrs enumtostr(char *enumstrs,int enumtypevalue,char *nonestr)
{
  if (enumtypevalue<=0) return nonestr;
  char *c=enumstrs;
  for(int i=1;i<enumtypevalue;i++)
  {
    c=strchr(c,';');
    if (!c) return nonestr;
    c++;
  }
  char *cn=strchr(c,';');
  if (!cn) return c;
  char var[0x100];
  strecpy(var,c,cn);
  eassert(strlen(var)<0x100,"enumtostr-moc dlouhe");
  return (chrs)var;
}

extern "C" int analvarlist(char *s,char *varnames,...)
{
  char *outstrs[32],**outtop=outstrs;
  int vpocet=0;
  {
    char *c=varnames;
    for(;*c;c++) if (*c==';') vpocet++;
    if (c[-1]!=';') vpocet++;
  }
  va_list args;
  va_start(args,varnames);
  for(int i=0;i<vpocet;i++) *outtop++=va_arg(args,char*);
  va_end(args);
  assert(vpocet==outtop-outstrs);

  for(int i=0;i<vpocet;i++) *outstrs[i]=0;//vyprazdnime hodnoty

  int cnt=0;
  while(*s)
  {
    jmpspaces(s);
    char idf[0x100],*idftop=idf;
    while(*s && *s!='=') *idftop++=*s++;*idftop=0;
    if (*s=='=')
    {
      int v=findvarinvars(varnames,idf);
//      printf("%d\n",v);
      if (v>=0)
      {
        s++;assert(v<vpocet);
        char *out=outstrs[v];
        while (*s>' ') *out++=*s++;*out=0;
        cnt++;
      } else {if (*s) s++;while (*s>' ') s++;}
    }
  }
  return cnt;
}

}
