#include <jlib.h>
#pragma hdrstop

#include "vectors.h"

namespace jmath {

template<class REAL> VECTOR<REAL>::VECTOR(const VECTOR<REAL> &src) {
  cnt=src.cnt;
  buf=new REAL[cnt];
  memmove(buf,src.buf,sizeof(REAL)*cnt);
}
template<class REAL> void VECTOR<REAL>::operator =(const VECTOR<REAL> &src) {
  delete []buf;
  cnt=src.cnt;
  buf=new REAL[cnt];
  memmove(buf,src.buf,sizeof(REAL)*cnt);
}

template<class REAL> string VECTOR<REAL>::str() {
  MEMSTROUT os;
  for(int i=1;i<=cnt;i++) write(item(i),os),os<<" ";
  return os.str();
}

template<class REAL> VECTOR<REAL>::VECTOR(const char *src) {
  STDSTRANAL q(src);
  q>>cnt;
  src=q.act;
  buf=new REAL[cnt];
  for(int i=1;i<=cnt;i++) src=read(item(i),src);
}


}

#pragma package(smart_init)
