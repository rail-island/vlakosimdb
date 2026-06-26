#include <jlib.h>
#pragma hdrstop

#include "mzip.h"
#include <zlib/zlib.h>

namespace jlib {

int mzip(void *src,int srclen,void *dest,int destlen) {
  compress((Bytef*)dest,(uLongf*)&destlen,(Bytef*)src,srclen);
  return destlen;
}

int munzip(void *src,int srclen,void *dest,int destlen) {
  uncompress((Bytef*)dest,(uLongf*)&destlen,(Bytef*)src,srclen);
  return destlen;
}

GZREAD::GZREAD(const char *file) {
  fr=gzopen(file,"rb");eof=not fr;
}
int GZREAD::readdata(void *buf,int size) {
  xassert(fr);
  int s=gzread(fr,buf,size);
  eof=(s!=size);
  return s;
}
GZREAD::~GZREAD() {
  if (fr) gzclose(fr);
}

GZWRITE::GZWRITE(const char *file,int level) {
  if (level<0) level=0;if (level>9) level=9;
  char s[5];s[0]='w';s[1]='b';s[2]=level+'0';s[3]=0;
  fw=gzopen(file,s);
}
GZWRITE::~GZWRITE() {
  if (fw) gzclose(fw);
}
int GZWRITE::writedata(const void *buf,int size) {
  xassert(fw);
  return gzwrite(fw,(void*)buf,size);
}


}

//#pragma package(smart_init)
