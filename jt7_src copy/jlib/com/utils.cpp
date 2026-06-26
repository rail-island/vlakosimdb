#include <jlib.h>
#pragma hdrstop

#include "utils.h"
#include "mzip.h"

namespace jlib {

void JException::show() {
  eputs(getmsg().c_str());
  jsystem->showerror(getmsg());
}

void CommonParseError::init(string atext,string amsg,int apos,string afile) {
  text=atext;msg=amsg;pos=apos;file=afile;
  fmterror(text.c_str(),text.c_str()+apos,line,col,context,true);
  fmterror(text.c_str(),text.c_str()+apos,line,col,monocontext,false);
}
string CommonParseError::preparetext(bool proportional) {
  MEMSTROUT res;
  res<<file<<":"<<line+1<<"("<<pos+1<<","<<col+1<<")\n";
  res<<msg<<"\n";
  res<<(proportional?context:monocontext)<<"\n";
  return res.str();
}

int xorhash(const void *data,int size) {
  unsigned long res=0;
  unsigned long *c=(unsigned long *)data;
  int s4=size/sizeof(unsigned long);
  for(int i=0;i<s4;i++) res^=*c++;
  return res;
}

string ftos(double value) {
  char s[0x100];sprintf(s,"%f",double(value));
  char *e=strend(s)-1;
  while (e>s) {
    if (*e=='0') *e=0;
    else {
      if (*e=='.') *e=0;
      break;
    }
    e--;
  }
  return string(s);
}
void fmterror(const char *src,const char *errorpos,int &linenum,int &colnum,string &errorline,bool propertional) {
  char *c=(char*)src,*act=(char*)errorpos;
  char line[0x200];
  linenum=0;*line=0;colnum=0;
  while (*c) {
    char *nc=readline(c);
    if (act>=c and act<nc) {
      colnum=act-c;
      strecpy(line,c,nc);
      break;
    }
    linenum++;
    c=nc;
  }
  trimr(line);
  char *e=strend(line);
  if (propertional) {
    strinsert(line,"*****",colnum);
  } else {
    *e++='\n';
    for(int i=0;i<colnum;i++) *e++='-';
    *e++='^';
    *e=0;
  }
  errorline=line;
}

int getfilesize(FILE *f) {
  if (f==NULL) return 0;
  int p0=ftell(f);
  fseek(f,0,SEEK_END);
  int i=ftell(f);
  fseek(f,p0,SEEK_SET);
  return i;
}

int getfilesize(const char *file) {
  FILE *fr=fopen(file,"rb");
  if (fr) {
    fseek(fr,0,SEEK_END);
    int res=ftell(fr);
    fclose(fr);
    return res;
  }
  return -1;
}

char *loadbinfile(const char *file,int *filesize,const char *openmode)
{
  FILE *fr=fopen(file,openmode);
  return loadbinfile(fr,filesize);
}

char *loadbinfile(FILE *fr,int *filesize)
{
  char *res=NULL;
  int size=getfilesize(fr);
  if (filesize!=NULL) *filesize=size;

  if (fr==NULL) return NULL;

  res=challoc(size+0x10);

  size=fread(res,1,size,fr);
  if (filesize) *filesize=size;

  res[size]=0;//ukonceni pripadneho retezce
  fclose(fr);
  return res;
}

bool savebinfile(const char *file,const char *str,const char *openmode)
{
  FILE *fw=fopen(file,openmode);
  if (fw)
  {
    fwrite(str,1,strlen(str),fw);
  }
  fclose(fw);
  return fw;
}

string loadtext(const char *file,const char *openmode) {
  char *s=loadbinfile(file,NULL,openmode);
  string res=s?s:"";
  if (s) free(s);
  return res;
}

string fmtstr(const char *format,...) {
  va_list args;
  char s[0x1000];
  va_start(args,format);
  vsprintf(s,format,args);
  va_end(args);
  xcheckstrlen(s);
  return string(s);
}
char *allocfmt(const char *format,...) {
  va_list args;
  char s[0x1000];
  va_start(args,format);
  vsprintf(s,format,args);
  va_end(args);
  xcheckstrlen(s);
  return strnew(s);
}


void gzsavetext(const char *file,const char *text,int comprlevel) {
  GZWRITE fw(file);
  fw.writedata(text,strlen(text));
}

string gzloadtext(const char *file) {
  GZREAD fr(file);
  return fr.readstreamtostring();
}

string makectext(const char *text) {
  string res;
  char *buf=res.reservebuffer(strlen(text)),*w=buf,*c=(char*)text;
  for(;*c;c++) if (*c!=13) *w++=*c;
  *w=0;
  res.updatelen();
  return res;
}

int fsystem(char *format,...)
{
  va_list args;
  va_start(args,format);
  char com[0x10000];
  vsprintf(com,format,args);
  int res=system(com);
  va_end(args);
  return res;
}

}

//#pragma package(smart_init)
