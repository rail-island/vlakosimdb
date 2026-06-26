#include <jlib.h>
#pragma hdrstop

#include "com0.h"

namespace jlib {


char **params;
int paramcount=0;
char basepath[0x100];

/*string getcurdate() {
}
string getcurtime() {
}
string getcurdatetime() {
}*/

const string estr="";

int makedir(char *dir)
{ return fsystem("md %s",dir);}

char *readtoch(register char *&s,register char *out,char end)
{
  for(;*s and *s!=end;*out++=*s++);
  *out=0;
  return out;
}

void eclear()
{
  FILE *fw=fopen(basedir/"stderr","w");
  if (fw) fclose(fw);
}
void eputs(const char *s)
{
  FILE *fw=fopen(basedir/"stderr","a");
  if (!fw) return;
  fputs(s,fw);fclose(fw);
}
void eprintf(const char *format,...)
{
  FILE *fw=fopen(basedir/"stderr","a");
  if (!fw) return;
  va_list args;
  va_start(args,format);
  vfprintf(fw,format,args);
  va_end(args);
  fclose(fw);
}

void removedir(char *dir)
{
  char com[0x1000];
  sprintf(com,"rd %s",dir);
  system(com);
}

char *writefixstr(char *dest,char *src,char len)
{
  register int l0=strlen(src);
  if (l0>len) { memmove(dest,src,len);dest[len]=0;return dest+len;}
  char *d=dest;
  //len>=l0
  for(int i=0;i<len-l0;i++) *d++=' ';
  strcpy(d,src);
  return dest+len;
}

void fixfmt(char *dest,...)
{
  va_list args;
  va_start(args,dest);
  for(;;)
  {
    char *s=va_arg(args,char*);
    if (!s) return;
    int len=va_arg(args,int);
    dest=writefixstr(dest,s,len);
  }
//  va_end(args);
}


bool testfileext(const char *file,const char *ext)
{
  char *fe=strend(file),*ee=strend(ext);
  fe--,ee--;
  while (fe>=file && ee>=ext)
  {
    if (toupper(*fe)!=toupper(*ee)) return false;
    fe--,ee--;
  }
  return (ee==ext-1 && *fe=='.');
}

//void initbasepath() { bspth=basepath; }

int _globalstrcnt=0;

void printstrcnt() {fprintf(stderr,"_globalstrcnt=%d\n",_globalstrcnt);}

int paramidx(char *paramname)
{
  for(int i=0;i<paramcount;i++) if (strcmp(params[i],paramname)==0) return i;
  return -1;
}

int freestrcomp(const char *s1,const char *s2)
{
  while(*s1 && *s2)
  {
    while (*s1<=' ' && *s1) s1++;
    while (*s2<=' ' && *s2) s2++;
    if (!*s1 || !*s2 ) break;
    register int u1=toupper(*s1++),u2=toupper(*s2++),
    cmp=cmpint(u1,u2);
    if (cmp) return cmp;
  }
  if (*s1 && !*s2) return 1;
  if (*s2 && !*s1) return -1;
  return 0;
}

int insstrcomp(const char *s1,const char *s2)
{
  while(*s1 && *s2)
  {
    register int u1=toupper(*s1++),u2=toupper(*s2++),
    cmp=cmpint(u1,u2);
    if (cmp) return cmp;
  }
  if (*s1 && !*s2) return 1;
  if (*s2 && !*s1) return -1;
  return 0;
}

/*void arinsert(int index,void *buf,void *elem,int &count,int size)
{
  if (index<0) throw EINDEXERROR;else
  if (index>=count) aradd(buf,elem,count,size);else
  {
    memmove((char*)buf+index*size,(char*)buf+(index+1)*size,size*(count-index));
    memmove((char*)buf+size*index,elem,size);
    count++;
  }
}

void arerase(int index,void *buf,int &count,int size)
{
  if (index<0||index>=count) throw EINDEXERROR;
  memmove((char*)buf+(index+1)*size,(char*)buf+index,size*(count-index-1));
  count--;
}*/

void strins(char *dest,const char *substr)
{
  int len=strlen(dest),sublen=strlen(substr);
  memmove(dest+sublen,dest,len+1);
  memmove(dest,substr,sublen);
}

void strdel(char *delbeg,char *delend)
{
  int movlen=strlen(delend);
  memmove(delbeg,delend,delend-delbeg+movlen);
}

FILE *fileopen(char *file,char *mode)
{
  char s[0x200];strcpy(s,file);dosnormalfile(s);return fopen(s,mode);
}
int fileremove(char *file)
{
  char s[0x200];strcpy(s,file);dosnormalfile(s);return remove(s);
}
void dosnormalfile(char *file)//prevede soubor pro aktualni OS
{
#ifdef BACKSLASH
  backslash(file);
#else
  foreslash(file);
#endif
}
void nicefile(char *file)//prevede soubor na mala pismena,dopredna lomitka
{ for(char *c=file;*c;c++) {if (*c=='\\') *c='/';*c=toupper(*c);}}
void backslash(char *file)
{ for(char *c=file;*c;c++) if (*c=='/') *c='\\'; }
void foreslash(char *file)
{ for(char *c=file;*c;c++) if (*c=='\\') *c='/'; }


}
