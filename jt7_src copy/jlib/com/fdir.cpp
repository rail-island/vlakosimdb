#include <jlib.h>
#pragma hdrstop

#include "fdir.h"

namespace jlib {


unsigned tmpdirint=0,tmpfileint=0;
const char dosslash='\\';
char deftmpdir[0x200];

void adddosbs(char *path)
{
  char *e=strend(path);
  if (e[-1]=='/' or e[-1]=='\\') return;
  *e++=dosslash;*e=0;
}

void tmpinit()
{
  tmpdirint=time(NULL)%0x1000000;tmpfileint=tmpdirint;
  sprintf(deftmpdir,"%stmp%c",basepath,dosslash);
}

void gentmpdir(char *dir)
{
  sprintf(dir,"%sTD%X%c",deftmpdir,tmpdirint++,dosslash);
}
void gentmpfile(char *file)
{
  sprintf(file,"%sTFc%X",deftmpdir,tmpfileint++);
}

chrs extractfilename(char *file)
{
  char f[0x100];extractfilename(file,f);return (chrs)f;
}

void extractfilename(char *file,char *outbuf)
{
  strcpy(outbuf,file);
  foreslash(outbuf);
  char *p=strrchr(outbuf,'/');
  if (p) strcpy(outbuf,p+1);
}

TMPDIR::TMPDIR()
{
  tmpnam(tmpdir);
  makedir(tmpdir);
  strcpy(tmppth,tmpdir);
  char *e=strend(tmppth);
  foreslash(tmppth);
  if (e[-1]!='/') {*e++='/';*e=0;}
  dosnormalfile(tmppth);
}
TMPDIR::~TMPDIR()
{
  STRLIST s;
  readfiles(s,true);
  for(int i=0;i<s.count;i++) fileremove(s[i]);
  removedir(tmpdir);
}
void TMPDIR::readfiles(STRLIST &files,bool addpath)
{
  char s[0x200];sprintf(s,"%s*.*",tmppth);
  readallfiles(files,s,addpath);
}
void TMPDIR::add(STRLIST &buf,char *name)
{
  char s[0x200];sprintf(s,"%s%s",tmppth,name);
  buf.save(s);
}
void TMPDIR::addtozip(char *zipfile)
{
  char s[0x200];sprintf(s,"%s*.*",tmppth);
  spawnl(P_WAIT,basedir/"pkzip",basedir/"pkzip","-a",zipfile,s,NULL);
}


void makepath(const char *dir,const char *file,const char *ext,char *path)
{
  if (dir) strcpy(path,dir);else *path=0;
  char *e=strend(path);
  if (e[-1]!='\\' and e[-1]!='/' and e>path) *e++='\\';
  strcpy(e,file);
  if (ext)
  {
    char *e2=strchr(e,'.');
    if (e2==NULL) e2=e;
    if (*ext and *ext!='.') *e2++='.';
    strcpy(e2,ext);
  }
//  if (strchr(file,'.')==NULL and ext) strcat(path,ext);
}

char *lastslash(char *in)
{
  char *e=strend(in)-1;
  while (e>=in and *e!='/' and *e!='\\') e--;
  if (e>=in) return e;
  return NULL;
}
void splitpath(const char *path,char *dir,char *file,char *ext)
//vynecha '/'
{
  if (dir)//rozdelime na cestu a soubor
  {
    char *s=lastslash((char*)path);
    if (s)
    {
      char pom=*s;*s=0;
      strcpy(dir,path);*s=pom;
      if (file) strcpy(file,s+1);
    } else if (file) strcpy(file,path);
  } else if (file) strcpy(file,path);
  if (file and ext)//rozdeleni na priponu a nazev
  {
    char *s=strrchr(file,'.');
    if (s) {strcpy(ext,s);*s=0;}
  }
}

void binsavestring(char *buf,char *file)
{
  FILE *fw=fopen(file,"wb");
  fwrite(buf,1,strlen(buf),fw);
  fclose(fw);
}

void fsavestring(char *buf,char *file)
{
  FILE *fw=fopen(file,"w");
  fwrite(buf,1,strlen(buf),fw);
  fclose(fw);
}

char *binloadstring(char *file)
{
  char buf[0x10000];
  FILE *fr=fopen(file,"rb");
  if (fr==NULL) return NULL;
  int len=fread(buf,1,0xFFFF,fr);
  fclose(fr);
  buf[len]=0;
  return strnew(buf);
}

char *floadstring(char *file)
{
  char buf[0x10000];
  FILE *fr=fopen(file,"r");
  int len=fread(buf,1,0xFFFF,fr);
  fclose(fr);
  buf[len]=0;
  return strnew(buf);
}

}
