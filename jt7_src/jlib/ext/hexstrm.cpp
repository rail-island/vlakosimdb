#include <jlib.h>
#pragma hdrstop

#include "hexstrm.h"

char hexdigit[]="0123456789ABCDEF";

void HEXWRITE::writebyte(unsigned char x) {
  if (bytesperline>=0 && byteatactline>=bytesperline) *so<<"\n",byteatactline=0;
  char b[0x10];
  b[0]=hexdigit[ (x&(~0xF))>>4 ];
  b[1]=hexdigit[ (x&0xF) ];
  b[2]=' ';
  b[3]=0;
  *so<<b;
  byteatactline++;
}

int HEXWRITE::writedata(const void *buf,int size) {
  uchar *c=(uchar*)buf;
  for(int s=size;s>0;s--,c++) writebyte(*c);
  return size;
}

int HEXREAD::readdata(void *buf,int size) {
  char *w=(char*)buf;
  for(int s=size;s>0 and not eof;s--,w++) *w=readbyte();
  return w-(char*)buf;
}

inline int readhexdigit(char c) {
  if (c>='0' and c<='9') return c-'0';
  if (c>='a' and c<='f') return c-'a'+10;
  if (c>='A' and c<='F') return c-'A'+10;
  return 0;
}

uchar HEXREAD::readbyte() {
  jmpspaces(act);
  char res=(readhexdigit(*act)<<4);
  act++;res|=readhexdigit(*act);act++;
  jmpspaces(act);
  eof=*act==0;
  return res;
}

void HEXREAD::save(BINWRITE &fw) {
  char buf[0x100],*w=buf;
  while (not eof) {
    if (w-buf==0x100) fw.writedata(buf,w-buf),w=buf;
    *w++=readbyte();
  }
  fw.writedata(buf,w-buf);
}

#pragma package(smart_init)
