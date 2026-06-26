#include <jlib.h>
#pragma hdrstop

#include "strlist.h"

namespace jlib {

void STRLIST::erasetrimablelines() {
  for(int i=0;i<count;i++) {
    char *&line=buf[i];
    if (seek0(line)) {free(line);line=NULL;}
  }
  pack(NULL);
}

string STRLIST::gettext(int beg,int end,const char *sep) {
  MEMSTROUT mso;
  for(int i=beg;i<end;i++) mso<<item(i)<<sep;
  if (beg<=end) mso<<item(end);//bez sep na konci
  return mso.str();
}
void STRLIST::settext(const char *text,char sep) {
  clear();
  char *c=(char*)text;
  while (c and *c) {
    char *d=strchr(c,sep);
    if (d) {
      *d=0;add(c);*d=sep;c=d+1;
    } else add(c),c=NULL;
  }
}

void STRLIST::trimaddanddividelines(STRLIST &s)
{
  for(int i=0;i<s.count;i++)
  {
    char line[0x10000];
    char *a=line,*c=s[i];
    while (*c)
    {
      if (*c=='\n')
      {
        *a=0;
        xcheckstrlen(line);
        trimr(line);add(line);a=line;c++;
      } else *a++=*c++;
    }
    if (a>line) {*a=0;xcheckstrlen(line);trimr(line);add(line);}
  }
}

#if 0
//#delbeg [ABAZ]

//#include <dir.h>
void readallfiles(STRLIST &res,char *mask,bool addpath)
{
  char tf[0x200];
  gentmpfile(tf);
  char m2[0x200];strcpy(m2,mask);
  dosnormalfile(m2);
  dosnormalfile(tf);
  fsystem("dir /B %s >%s",m2,tf);
//  fputs(fmtstr("dir /B %s >%s",m2,t),stderr);
  res.load(tf);
//  res.print(stderr);
  fileremove(tf);
  char path[0x200];
  res.clear();
  foreslash(m2);
  strecpy(path,m2,strrchr(m2,'/'));
  addchtostr(path,'/');
  dosnormalfile(path);
  if (addpath)
    for(int i=0;i<res.count;i++)
      res.catl(i,path);
/*  struct ffblk f;
  char path[0x200];
  res.clear();
  strecpy(path,mask,strrchr(mask,'/'));
  addchtostr(path,'/');
  int done = findfirst(mask,&f,0);
  while (!done)
  {
    char fn[0x200];
    if (addpath) strcpy(fn,path);else *fn=0;
    strcat(fn,f.ff_name);
    res.add(fn);
    done = findnext(&f);
  }
  res.asort();*/
}


STRLIST &readallfiles(char *mask)
{
  STRLIST *res=new STRLIST;
  readallfiles(*res,mask,false);
  return *res;
}

void STRLIST::readlinetobuf()
{
  if (actline<count and actline>=0) strcpy(linetbuf,item(actline));else *linetbuf=0;
  if (actline>=0) actline++;
  eof=actline>count or eof;if (eof) *linetbuf=0;
}
//#delend [ABAZ]
#endif

void STRLIST::changewithtbl(char map[0x100])
{
  for(int i=0;i<count;i++)
    for(char *c=item(i);*c;c++) *c=map[*c];
}

int STRLIST::bufsize() {return bufsize(0,count-1);}
char *STRLIST::savetobuf() {return savetobuf(0,count-1);}

int STRLIST::bufsize(int beg,int end)
{
  int r=0;
  for(int i=beg;i<=end;i++) r+=strlen(item(i));
  return r+count+1;
}

char *STRLIST::savetobuf(int beg,int end)
{
  char *res=challoc(bufsize(beg,end)),*r=res;
  for(int i=beg;i<=end;i++) {strcpy(r,item(i));r=strend(r);*r++=10;}
  *r=0;
  return res;
}

chrs STRLIST::autotext(int beg,int end) {
  xcheckindex(beg,count);
  xcheckindex(end,count);
  char *pombuf=new char[bufsize(beg,end)],*r=pombuf;
//  chrs res;res.wantcapa(bufsize(beg,end));
  for(int i=beg;i<=end;i++) {r=estrcpy(r,buf[i]);if (i<end) *r++='\n';}
  *r=0;//res._setlen(r-res._getbuf());
  string res=pombuf;
  delete []pombuf;
  return res;
}
chrs STRLIST::autotext() {if (count) return autotext(0,count-1);return "";}

void STRLIST::addfrombuf(const char *buf)
{
  const char *c=buf;
  while (*c)
  {
    char *cn=readline(c);
    char line[0x200];
    strecpy(line,c,cn);
    trimr(line);
    add(line);
    c=cn;
  }
}

void STRLIST::assignfrombuf(const char *buf)
{
  clear();
  addfrombuf(buf);
}

int STRLIST::findfirstsubstr(char *substr)
{
  for(int i=0;i<count;i++)
  {
    if (strbegcmp(substr,item(i))) return i;
  }
  return -1;
}

int _compstr(const void *a,const void *b)
{
  return strcmp( *(char**)(a), *(char**)(b) );
}
void STRLIST::asort()
{
  qsort(buf,count,sizeof(char*),_compstr);
}

void STRLIST::trimprint(FILE *fw)
{
  for(int i=0;i<count;i++) trimr(item(i));
  print(fw);
}

/*int STRLIST::bsrch(char *what)
{
//  char **c=(char**)bsearch(what,buf,count,sizeof(char*),_compstr);
//  if (c) return c-buf;
//  return -1;
  for(int i=0;i<count;i++) if (eqstr(item(i),what)) return i;
  return -1;
}*/

int STRLIST::search(const char *what)
{
  for(int i=0;i<count;i++) if (eqstr(item(i),what)) return i;
  return -1;
}

int STRLIST::findvar(const char *var)
{
  for (int i=0;i<count;i++)
  {
    if (islinevar(item(i),var)) return i;
  }
  return -1;
}
char *STRLIST::getvalue(const char *var)
{
  int i=findvar(var);
  if (i>=0) return strchr(item(i),'=')+1;
  return NULL;
}


/*STRLIST &readallfiles(char *mask)
{
  char com[0x200];
  sprintf(com,"dir %s /b",mask);
  FILE *p = popen(com, "r");
  char s[0x100];
  STRLIST *res=new STRLIST;
  while (fgets(s,0x100,p)!=NULL) res->add(s);
  pclose(p);
  return *res;
}*/


void STRLIST::print(FILE *fw)
{
  for(int i=0;i<count;i++) fprintf(fw,"%s\n",item(i));
}

int STRLIST::save(const char *file)
{
  FILE *fw=fopen(file,"w");
  if (fw==NULL) return 0;
  for(int i=0;i<count;i++) fprintf(fw,"%s\n",item(i));
  fclose(fw);
  return 1;
}

int STRLIST::load(const char *file)
{
  clear();
  FILE *fr=fopen(file,"r");
  if (fr==NULL) return 0;
  while (!feof(fr))
  {
    char s[0x10000];
    char *ch=fgets(s,0x10000,fr);
    trimr(s);
    if (ch!=NULL) add(s);
  }
  fclose(fr);
  return 1;
}

bool islinevar(const char *line,const char *var)
{
  while (*line && *var)
  {
    if (*line!=*var) return false;
    line++;var++;
    if ( !*var && *line=='=' ) return true;
  }
  return false;
}

}
