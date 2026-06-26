#include <stdio.h>

#ifndef HPARAMS
#define HPARAMS

namespace jlib {

extern char **params;
extern int paramcount;
extern char basepath[0x100];

void filepath(char *file,char *path);
int paramidx(char *paramname);

void initbasepath();

#define initpars\
  {params=argv;paramcount=argc;filepath(params[0],basepath);initbasepath();}

inline char *parafter(char *parname,int plus)
{
  int i=paramidx(parname);
  if (i>=0 && i+plus<paramcount) return params[i+plus];else return NULL;
}

inline int isparam(char *parname)
{ return paramidx(parname)>0; }

}

#endif
