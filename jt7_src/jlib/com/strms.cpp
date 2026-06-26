#include <jlib.h>
#pragma hdrstop

#include "strms.h"

namespace jlib {

void BINWRITE::writew1(short w) {
  writec((w & 0xFF00)>>8);
  writec(w & 0x00FF);
}
void BINWRITE::writel1(long l) {
  writec((l&0xFF000000)>>24);
  writec((l&0x00FF0000)>>16);
  writec((l&0x0000FF00)>>8);
  writec(l&0x000000FF);
}

int BINREAD::readtostream(int bytes,BINWRITE &fw) {
  char buf[0x10000];
  int res=0;
  while (bytes>0 and not eof) {
    int c=min(bytes,0x10000);
    bytes-=c;
    int readed=readdata(buf,c);
    fw.writedata(buf,readed);
    res+=readed;
  }
  return res;
}


string BINREAD::readstreamtostring() {
  char buf[0x10000];
  MEMSTROUT f;
  while (not eof) {
    int len=readdata(buf,0xFFFF);
    buf[len]=0;
    f<<buf;
  }
  return f.str();
}

char *BINREAD::readandallocs() {
  long len=readi();
  char *s=challoc(len+1);
  readdata(s,len);
  s[len]=0;
  return s;
}


int MEMORYSTREAM::load(BINREAD &fr) {
  int size;
  fr>>size;
  wantcapa(size);
  fr.readdata(buffer,size);
  writepos=size;readpos=0;
  _changeddata();
  return size;
}

int MEMORYWRITE::writedata(const void *buf,int size) {
  wantcapa(writepos+size);
  memcpy(buffer+writepos,buf,size);
  writepos+=size;
  return size;
}

int MEMORYSTREAM::writedata(const void *buf,int size) {
  wantcapa(writepos+size);
  memcpy(buffer+writepos,buf,size);
  writepos+=size;
  _changeddata();
  return size;
}

int MEMORYSTREAM::readdata(void *buf,int size) {
  int res=min(size,writepos-readpos);
  if (res) memcpy(buf,buffer+readpos,res);
  readpos+=res;
  _changeddata();
  return res;
}

int MEMORYREAD::readdata(void *buf,int size) {
  int res=min(size,afterend-readpos);
  memcpy(buf,readpos,res);
  readpos+=res;
  eof=readpos<afterend;
  return res;
}

MEMORYREAD::MEMORYREAD(char *buf,int len,bool copyintoprivatebuf) {
  if (copyintoprivatebuf) {
    buffer=challoc(len);memcpy(buffer,buf,len);
    readpos=buffer;afterend=buffer+len;
  } else {
    buffer=NULL;readpos=buf;afterend=buf+len;
  }
}

MEMORYREAD::MEMORYREAD(BINREAD &fr) {
  int size;
  fr>>size;
  buffer=challoc(size);
  fr.readdata(buffer,size);
  afterend=buffer+size;
  readpos=buffer;
}


void BINREAD::jump(int n) {
  char s[0x100];
  while (n) {
    int readed=readdata(s,min(0x100,n));
    if (readed<0x100) return;
    n-=readed;
  }
}

void BINRFILE::jump(int n) {
  fseek(fr,n,SEEK_CUR);
  eof=feof(fr);
}

void *BINRFILE::readtoeof(int *size) {
  int pos=ftell(fr);
  fseek(fr,0,SEEK_END);
  int endpos=ftell(fr);
  fseek(fr,pos,SEEK_SET);
  int s=endpos-pos+1;
  void *res=malloc(s);
  s=readdata(res,s);
  if (size) *size=s;
  return res;
}

void BINWRITE::writefromstream(BINREAD &stream,int count) {
  char *buf=new char[count];
  stream.readdata(buf,count);
  writedata(buf,count);
  delete []buf;
}

string BINREAD::readstring() {
  string res;
  int len=readi();
  readdata(res.reservebuffer(len),len);
  res.setlen(len);
  return res;
}

void BINWRITE::writestring(string s) {
  writei(s.length());
  writedata(s.c_str(),s.length());
}

int BINRFILE::readdata(void *buf,int size) {
  int s=fread(buf,1,size,fr);
  eof=(s!=size);
  return s;
}

BINRFILE::BINRFILE(const char *file,const char *openmode) {
  fr=fopen(file,openmode);
  eof=! fr;
  if (eof) throw JError(string("Cannot open file for read:")+file);
}

int BINWFILE::writedata(const void *buf,int size) {
  xassert(fw);
  return fwrite(buf,1,size,fw);
}

bool BINWFILE::open(const char *file,const char *openmode) {
  close();
  fw=fopen(file,openmode);
  return fw;
}

BINWFILE::BINWFILE(const char *file,const char *openmode) {
  fw=NULL;
  if (!open(file)) throw JError(string("Cannot open file for write:")+file);
}


}
