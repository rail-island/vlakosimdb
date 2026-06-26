#include <jlib.h>
#pragma hdrstop

#include "strcom.h"
#include <com/mzip.h>

namespace jlib {

bool eqstr(register const char *s1,register const char *s2)
{
  for(;*s1==*s2;s1++,s2++) if (!*s1) return true;
  return false;
}

bool strendcmp(const char *endstr,const char *longstr) {
  char *e=strend(longstr);
  int lshort=strlen(endstr),llong=strlen(longstr);
  if (lshort>llong) return false;
  return eqstr(e-lshort,endstr);
}

string readnum(pchar &c) {
  string res;
  if (*c=='-') res<<*c++;
  while (*c>='0' and *c<='9') res<<*c++;
  if (*c=='.') res<<*c++;
  while (*c>='0' and *c<='9') res<<*c++;
  return res;
}

string readidf(pchar &c) {
  string res;
  while ((*c>='a' and *c<='z') or (*c>='A' and *c<='Z') or (*c>='0' and *c<='9') or *c=='_') res<<*c++;
  return res;
}

void strdelete(char *s,int index,int count)
{
  int len=strlen(s);
  if (index>len) return;
  if (index+count>=len) { s[index]=0;return; }
  memmove(s+index,s+index+count,len-index);//-count+1);
}

void strinsert(char *s,const char *substr,int index)
{
  int len=strlen(s),sublen=strlen(substr);
  if (index==len) {strcat(s,substr);return;}
  if (index>len)
  {
    char *e=strend(s);
    while(e-s<index) *e++=' ';
    strcpy(e,substr);
    return;
  }
  memmove(s+index+sublen,s+index,len-index);
  memmove(s+index,substr,sublen);
  s[len+sublen]=0;
}


bool freemapeqstr(const char *s1,const char *s2,CharMap map)
{
  while(*s1 and *s2)
  {
    jmpspaces(s1);
    jmpspaces(s2);
    if (*s1) continue;
    if (*s2) continue;
    if (map[*s1]!=map[*s2]) return false;
    s1++,s2++;
  }
  return !(*s1 || *s2);
}

/*char *freemapsubstr(const char *sub,const char *s,CharMap map) {
  StdJmpString qs(s),qsub(sub);
  if (freemapsubstr(qs,qsub)) return qs.act;
  return NULL;
  return freemapsubstr<jmpspaces,jmpspaces> (sub,s,map);
}*/


/*char *freemapsubstr(const char *sub,const char *s,CharMap map)
{
  jmpspaces(sub);
  for(const char *c=s;*c;c++)
  {
    if (chisspace(*c)) continue;
    const char *x=c,*y=sub;
    for(;*x && *y;x++,y++)
    {
      jmpspaces(x);
      jmpspaces(y);
      if (!*x || !*y) break;
      if (map[*x]!=map[*y]) break;
    }
    if (!*y) return (char*)c;
  }
  return NULL;
}*/

bool mapeqstr(const char *s1,const char *s2,CharMap map)
{
  for(;*s1&&*s2;s1++,s2++)
    if (map[*s1]!=map[*s2]) return false;
  return !(*s1 || *s2);
}

char *mapsubstr(const char *sub,const char *s,CharMap map)
{
  for(const char *c=s;*c;c++)
  {
    const char *x=c,*y=sub;
    for(;*x && *y;x++,y++) if (map[*x]!=map[*y]) break;
    if (!*y) return (char*)c;
  }
  return NULL;
}

}
