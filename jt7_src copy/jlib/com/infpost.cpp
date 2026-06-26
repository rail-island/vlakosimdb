#include <jlib.h>
#pragma hdrstop

#include "infpost.h"

#define chisop(ch) (CHISOP & chardefs[ch])
#define chisnum(ch) (CHISNUM & chardefs[ch])
#define chisalpha(ch) (CHISALPHA & chardefs[ch])
#define chismain(ch) (CHISMAIN & chardefs[ch])
#define chisend(ch) (CHISEND & chardefs[ch])
#define chisalnum(ch) (chisalpha(ch)||chisnum(ch))

namespace jlib {


static int readelem(char *&c,char *elem,char chardefs[0x100]);
static int getelem(char *&c,char *elem,char chardefs[0x100],STRLIST &sops);

void OPS::load(STRLIST &s)
{
//  s.print(stderr);exit(0);
  for(int i=0;i<s.count;i++)
  {
    OPREC &r=add();
    char type[0x100];
    sscanf(s[i],"%s %d %s",r.name,&r.pri,type);
    char *t=gettrim(type);
    if (upeqstr(type,"bin")) r.type=OTBIN;else
    if (upeqstr(type,"pre")) r.type=OTPREFIX;else
    if (upeqstr(type,"post")) r.type=OTPOSTFIX;else
    eraselast();
  }
}

bool OPS::load(char *file)
{
  STRLIST s;
  bool r=s.load(file);if (r) load(s);
  return r;
}

struct STREC{int op,t;char *s;int pri;};
static STREC makeop(int idx,int pri) { STREC r;r.op=idx;r.t='o';r.s=NULL;r.pri=pri;return r;}
static STREC makefn(char *name) { STREC r;r.op=-1;r.t='f';r.s=strnew(name);r.pri=0;return r;}
static STREC srmakech(int ch) { STREC r;r.op=-1;r.t=ch;r.s=NULL;r.pri=0;return r;}

int inftopost(char *in,STRLIST &s,char chardefs[0x100],OPS &ops,char *error)
#define err(s) {if(error) strcpy(error,s);return c-in;}
#define serr(s) {if(error) strcpy(error,s.c_str());return c-in;}
{
  STRLIST sops;
  if (error) *error=0;
  for(int i=0;i<ops.count;i++) if (chisalpha(ops[i].name[0])) sops+=ops[i].name;
//  sops.print(stderr);
  int mayop=0;//zda je na rade oparator
  //kdyz mayop && e==ITOP,pak operator je prefizovy unarni
  Stack<STREC> stack;
  char *c=in;
  while(*c)
  {
    char es[0x100];
    int e=getelem(c,es,chardefs,sops);
    if (e<0) serr(fmtstr("Chybny retezec:%s",es));
    if (e=='(')
    {
      if (mayop) err("Ma byt operator,nalezeno '('");
      stack.push(srmakech('('));
    } else
    if (e==',')
    {
      if (!mayop) err("Ma byt identifikator,nalezeno ','");
      stack.push(srmakech(','));
      mayop=0;
    } else
    if (e==')')
    {
      int argc=mayop;
      while(stack.count && stack.top().t!='f' && stack.top().t!='(')
      {
        if (stack.top().t=='o') s+=(char*)ops[stack.pop().op].getpriname().c_str();else
        if (stack.top().t==',') stack.pop(),argc++;else
        err("Chyba uvnitr zavorek");
      }
      if (stack.count==0) err("')' bez '('");
      if (stack.top().t=='(')
      {
        stack.pop();
        if (argc!=1) err("Identita musi mit vzy prave jeden argument");
      } else
      if (stack.top().t=='f')
      {
        assert(argc<1000);
        s+=(char*)fmtstr("f%03d %s",argc,stack.top().s).c_str();
        free(stack.pop().s);
        mayop=1;
      }
/*      if (!mayop) err("Na rade je operator");
      while( stack.count && strcmp(stack.top().name,"(")
       && strcmp(stack.top().name,"f"))
        s+=stack.pop().getpriname();
      if (stack.count==0) err("Chybi '('");

      stack.pop();*/
    } else
    if (e==ITFUNC)
    {
      assert(*c=='(');c++;
      stack.push(makefn(es));
    } else
    if (e==ITOP)
    {
      //mayop==je to binarni nebo postfixovy operator
      int i=ops.findop(es,!mayop);
      if (i<0) serr(fmtstr("Neznamy operator:%s;typ:%s",es,mayop?"bin/post":"pre"));
      OPREC &r=ops[i];
      //nejdrive vypustime operatory s vyssi a stejnou prioritou
      while( stack.count && stack.top().pri>=r.pri )
        s+=(char*)ops[stack.pop().op].getpriname().c_str();
      mayop=0;
      stack.push(makeop(i,r.pri));
    } else
    if (e)
    {
      if (mayop) serr(fmtstr("Zde ma byt operator misto %s",es));
      if (e==ITSTR) s+=(char*)fmtstr("s%s",es).c_str(); else
      if (e==ITCHAR) s+=(char*)fmtstr("c%s",es).c_str(); else
      if (e==ITREAL) s+=(char*)fmtstr("r%s",es).c_str(); else
      if (e==ITINT) s+=(char*)fmtstr("n%s",es).c_str(); else
      if (e==ITIDF) s+=(char*)fmtstr("i%s",es).c_str();
      mayop=1;
    }
  }
  if (!mayop) err("Spatne ukonceni vyrazu");
  while(stack.count)
  {
    if (stack.top().t=='o') s+=(char*)ops[stack.pop().op].getpriname().c_str();else
    err("Spatne ukonceni vyrazu");
  }
  return -1;
}


void fillchardefs(char a[0x100],char *ops,char *mains,char *ends)//operatory ukoncene 0
{
  fillarray(a,0x100,'\0');
  for(char *c=ops;*c;c++) a[uchar(*c)]|=CHISOP;
  for(char *c=mains;*c;c++) a[uchar(*c)]|=CHISMAIN;
  for(char *c=ends;*c;c++) a[uchar(*c)]|=CHISEND;
  for(int i='a';i<='z';i++) a[i]|=CHISALPHA;for(int i='A';i<='Z';i++) a[i]|=CHISALPHA;
  for(int i='0';i<='9';i++) a[i]|=CHISNUM;
}

int OPS::findop(char *name,bool isprefix)
{
  for(int i=0;i<count;i++) if (eqstr(item(i).name,name) && (item(i).type==OTPREFIX)==isprefix) return i;
  return -1;
}


static int readelem(char *&c,char *elem,char chardefs[0x100])
{
  jmpspaces(c);
  if (chisop(*c))
  {
    char *e=elem;
    while (chisop(*c)) *e++=*c++;*e=0;
    return ITOP;
  }
  if (chisalpha(*c))
  {
    char *e=elem;
    while (chisalnum(*c)) *e++=*c++;*e=0;
    jmpspaces(c);
    if (*c=='(') return ITFUNC;else return ITIDF;
  }
  if (chisnum(*c) || *c=='-')
  {
    char *e=elem;
    if (*c=='-')  *e++='-',c++;
    while (chisnum(*c)) *e++=*c++;*e=0;
    if (*c!='.' && *c!='e' && *c!='E') return ITINT;
    if (*c=='.')  *e++='.',c++;
    while (chisnum(*c)) *e++=*c++;
    if (*c=='e' || *c=='E')  *e++='E',c++;
    if (*c=='-')  *e++='-',c++;
    while (chisnum(*c)) *e++=*c++;*e=0;
    return ITREAL;
  }
  if (*c=='\'')//apostrof
  {
    char *e=elem;c++;
    for(;(*c!='\'' || c[1]=='\'') && *c;)
      {if (*c=='\'') c++;*e++=*c++;}
    *e=0;if (*c) c++;
    return ITCHAR;
  }
  if (*c=='"')
  {
    char *e=elem;c++;
    for(;(*c!='"' || c[1]=='"') && *c;)
      {if (*c=='"') c++;*e++=*c++;}
    *e=0;if (*c) c++;
    return ITSTR;
  }
  if (chismain(*c)) return *c++;
  if (chisend(*c)) return 0;
  if (!*c) return 0;
  return -1;
}

static int getelem(char *&c,char *elem,char chardefs[0x100],STRLIST &sops)
{
  int res=readelem(c,elem,chardefs);
  if (res==ITIDF && sops.search(elem)>=0) return ITOP;
  return res;
}


}
