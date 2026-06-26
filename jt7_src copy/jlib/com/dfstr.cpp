#include <jlib.h>
#pragma hdrstop

namespace jlib {


STR16 FILESTR::getext() const
{
  STR16 res;
  char *e=fileextbeg();
  if (*e=='.') e++;
  strcpy(res.s,e);
  return res;
}

void normalize_path(char *s)
{
  for(char *c=s;*c;c++) if (*c=='\\' or *c=='/') *c=dirsep;
}

void DIRSTR::assign(const FILESTR &file)
{
  char *fs=(char*)file.s;
  char *e=strrchr(fs,dirsep);
  if (e) strecpy(s,fs,e);else *s=0;
}

char *FILESTR::fileextbeg()
{
  char *sep=strrchr(s,dirsep),*tec=strrchr(s,'.');
  if (tec>sep) return tec;//pokud je to tecka u souboru,ne u adresare
  return strend(s);//nema priponu
}

void FILESTR::setext(const char *ext)
{
  char *e=fileextbeg();
  if (ext)
  {
    if (*ext=='.') ext++;
    if (*ext) *e++='.';
    strcpy(e,ext);
  } else *e=0;
}

void FILESTR::addext(const char *ext)
{
  char *e=fileextbeg();
  if (ext and *e==0)
  {
    if (*ext and *ext!='.') *e++='.';
    strcpy(e,ext);
  }
}

DIRSTR FILESTR::getdir() const
{
  DIRSTR res;
  const char *e=strrchr(s,dirsep);
  if (e) strecpy(res.s,s,e);
  return res;
}

void FILESTR::getfname(char *buf) const
{
  const char *e=strrchr(s,dirsep);
  if (e) strcpy(buf,e+1);else strcpy(buf,s);
}

chrs FILESTR::getfname() const {
  char name[0x100];getfname(name);return chrs(name);
}

void FILESTR::getnamewithoutext(char *buf) const {
  getfname(buf);if (char *e=strchr(buf,'.')) *e=0;
}
chrs FILESTR::getnamewithoutext() const {
  char name[0x100];getnamewithoutext(name);return chrs(name);
}

void FILESTR::setdir(const DIRSTR &dir)
{
  char f[0x100];
  getfname(f);

  strcpy(s,dir.s);
  char *e=strend(s);
  *e++=dirsep;
  strcpy(e,f);
}

FILESTR DIRSTR::operator()(const char *file) const
{
  FILESTR res;
  strcpy(res.s,s);
  char *e=strend(res.s);
  if (e>res.s) *e++=dirsep;
  strcpy(e,file);
  return res;
}

bool FILESTR::testext(const char *ext) const
{
  if (*ext=='.') ext++;
  char *e=fileextbeg();
  if (*e=='.') e++;
  while (toupper(*e)==toupper(*ext)) e++,ext++;
  return *e==0 and *ext==0;//obe skoncily,tedy jsou stejne
}

bool FILESTR::hasext() const
{
  return *fileextbeg();
}

DIRSTR getworkdir()
{
  char s[0x100];
  getcurdir(0,s+3);
  *s=getdisk()+'A';s[1]=':';s[2]=dirsep;
  return DIRSTR(s);
}

ShortStr<64> DIRSTR::getlastdir() const
{
  char *e=(char*)strrchr(s,dirsep);
  if (e) return e+1;else return s;
}

void DIRSTR::trimlastdir()
{
  char *e=(char*)strrchr(s,dirsep);
  if (e) *e=0;else {if (isrel()) *s=0;}
}
void DIRSTR::adddirtopath(const DIRSTR &dir)
{
  if (dir.isrel())
  {
    char *e=strend(s);if (e>s) *e++=dirsep;strcpy(e,dir.s);
  } else strcpy(s,dir.s);
}

chrs FILESTR::loadline(int index)
{
  FILE *fr=fopen(s,"r");
  if (!fr) return (chrs)"";
  char buf[0x1000];
  int i=0;bool suc;
  while(i<=index and (suc=fgets(buf,0x1000,fr))) i++;
  fclose(fr);
  trimr(buf);
  if (suc) return (chrs)buf;
  return (chrs)"";
}

ShortStr<> FILESTR::todirsep(char sep) const
{
  ShortStr<> res;
  strcpy(res.s,s);
  for(char *c=res.s;*c;c++) if (*c==dirsep) *c=sep;
  return res;
}

FILESTR FILESTR::filename() {
  FILESTR res;getfname(res.s);return res;
}
FILESTR FILESTR::purefilename() {
  FILESTR res;getnamewithoutext(res.s);return res;
}

chrs FILESTR::loadtext() {
  char *buf=loadbinfile(s,NULL,"r");
  chrs res=buf;
  free(buf);
  return res;
}


}

#pragma package(smart_init)
