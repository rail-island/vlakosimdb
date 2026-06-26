#include <jlib.h>
#pragma hdrstop

#include "sfile.h"
#include "dir.h"

namespace jlib {

static sFile::NO_INIT no_init;
sFile basedir(no_init);

bool sFile::isfile() {
  FILE *fr=fopen(s.c_str(),"rb");
  bool res=fr;
  if (fr) fclose(fr);
  return res;
}

bool sFile::hasfile(string file) {
  return (*this/file).isfile();
}

sFile sFile::relto(sFile dir) const {
  if (isrelative() or dir.isrelative()) return *this;
  const char *c=c_str(),*endcom=c,*d=dir.c_str(),*endcomd=d;
  while (*c and *d and toupper(*c)==toupper(*d)) {
    if (*c=='/' or *c=='\\') endcom=c,endcomd=d;
    c++,d++;
  }
  if (*c==0) return *this;
  if (*d==0 and (*c=='\\' or *c=='/')) endcom=c,endcomd=d;
  if (endcom==s) return *this;
  bstring res;

/*  sFile res;char *w=res.s;
  for(d=endcomd;*d;d++) if (*d=='/' or *d=='\\') *w++='.',*w++='.',*w++=dirsep;
  strcpy(w,endcom+1);*/
  for(d=endcomd;*d;d++) if (*d=='/' || *d=='\\')  res<<".."<<dirsep;
  res<<endcom+1;

  return sFile(res);
}

const char *sFile::fileextbeg() const {
  const char *sep=strrchr(c_str(),dirsep),*tec=strrchr(c_str(),'.');
  if (tec>sep) return tec;//pokud je to tecka u souboru,ne u adresare
  return strend(c_str());//nema priponu
}

void sFile::init(const char *src) {
  s=src;
//  strcpy(s,src);
  normalize((char*)c_str());
//  xcheckstrlen(s);
  char *e=(char*)strend(c_str());if (e>c_str() && e[-1]==dirsep) e[-1]=0;
  s.updatelen();
}

string sFile::getext() const {
  char *e=(char*)fileextbeg();
  if (*e=='.') e++;
  return string(e);
}
sFile sFile::makeext(const char *ext) const {
  sFile res(*this);res.setext(ext);return res;
}
void sFile::setext(const char *ext) {
  const char *e=fileextbeg();
  if (*ext=='.') ext++;
  if (ext && *ext) {
    s=s.copy(0,e-c_str())+"."+ext;
  } else {
    s=s.copy(0,e-c_str());
  }
}

sFile sFile::getdir() const {
//  string res;
//  char *w=res.reservebuffer(c_str());
  const char *e=strend(c_str());
  while (e>=c_str() && *e!='\\' && *e!='/') e--;
  if (e<c_str()) e=NULL;
  if (e) return sFile(s.copy(0,e-c_str()));
  else return sFile("");
//   strecpy(w,c_str(),e);
//  else *w=0;
//  res.updatelen();
//  return res;
}

void sFile::operator--(int) {
  char *e=(char*)strrchr(c_str(),dirsep);
  if (e) s=s.copy(0,e-c_str());
}

sFile sFile::getfile() const {
  const char *e=strrchr(c_str(),dirsep);
  if (e) {
    int idx=e-c_str();
    return sFile(s.copy(idx+1,s.length()-idx-1));
  }
  return *this;
//  if (e) strcpy(res.s,e+1);else strcpy(res.s,s);
//  return res;
}

bool sFile::_isabsolute(const char *s) {
  if (!*s) return false;
  if (s[0]=='\\' || s[0]=='/') return true;
  if (s[0] && s[1]==':' && (s[2]=='\\' || s[2]=='/')) return true;
  return false;
}

void sFile::add(const char *file) {
  if (_isabsolute(file)) init(file);
  else {
    if (*c_str()) {
      string res=s;
      if (*file) res<<dirsep<<file;
      s=res;
    } else s=file;
/*    char *e=strend(s);
    if (e>s) *e++=dirsep;
    strcpy(e,file);
    xcheckstrlen(s);*/
  }
}

bool sFile::testext(const char *ext) const {
  if (*ext=='.') ext++;
  const char *e=fileextbeg();
  if (*e=='.') e++;
  while (*e && *ext) {
    if (toupper(*e)!=toupper(*ext)) return false;
    e++;ext++;
  }
  return *e==0 && *ext==0;//obe skoncily,tedy jsou stejne
}

void sFile::docommand(const char *com) {
  switch (*com) {
    case 'e':setext(com+1);break;
    case 'd':operator --(0);break;
    case 'f':s=getfile();break;
  }
}

void sFile::docommands(const char *coms) {
  char com[0x10];
  const char *c=coms;
  const char *e=strchr(coms,';');
  while (*c) {
    strecpy(com,c,e);
    docommand(com);
    if (e) {c=e+1;continue;} else break;
  }
}

string sFile::loadline(int index) {
  FILE *fr=fopen(c_str(),"r");
  if (!fr) return (string)"";
  char buf[0x1000];
  int i=0;bool suc;
  while(i<=index and (suc=fgets(buf,0x1000,fr))) i++;
  fclose(fr);
  trimr(buf);
  if (suc) return (string)buf;
  return (string)"";
}

string sFile::loadtext() {
  char *buf=loadbinfile(c_str(),NULL,"r");
  if (buf==NULL) return string("");
  string res(buf);
  free(buf);
  return res;
}

bool sFile::isdir() {
  bool res=false;
  struct ffblk ffblk;
  int done = findfirst(s.c_str(),&ffblk,FA_DIREC);
  if (!done && (ffblk.ff_attrib & FA_DIREC)) res=true;
  findclose(&ffblk);
  return res;
}

void sFile::mkdirs() {
  IObjArray<sFile> dirs;
  sFile act=*this;
  while (*act.c_str()) {
    if (!act.isdir()) dirs.add(new sFile(act));
    act=act.getdir();
  }
  for(int i=dirs.count-1;i>=0;i--) dirs[i]->mkdir();

}

void sFile::rmdir() {
  std::rmdir(c_str());
}

bool sFile::mkdir() {
  return std::mkdir(c_str());
}

}



