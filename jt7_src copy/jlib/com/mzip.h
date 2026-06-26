#ifndef mzipH
#define mzipH

#include <com/strms.h>

namespace jlib {

int mzip(void *src,int srclen,void *dest,int destlen);
int munzip(void *src,int srclen,void *dest,int destlen);

class GZREAD:public BINREAD
{
public:
  void *fr;
  GZREAD(const char *file);
  int readdata(void *buf,int size);
  ~GZREAD();
};

class GZWRITE:public BINWRITE
{
public:
  void *fw;
  GZWRITE(const char *file,int level=6);
  ~GZWRITE();
  int writedata(const void *buf,int size);
};

}

#endif
