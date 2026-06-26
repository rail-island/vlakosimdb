#ifndef HTEXTANAL
#define HTEXTANAL

#include <com/com0.h>

namespace jlib {

extern "C" int analvarlist(char *s,char *varnames,...);
//s je ret. ve tvaru var1=hodnota var2=hodnota ...
//varnames jsou jmena promennych oddelena stredniky(napr.var1;var2)
//... jsou char *,ulozi se do nich hodnoty v poradi,v jakem jsou promenne ve vars

char *readelementinbrackets(char *buf,char begin,char separator,char end,char *outbuf);

int strtoenum(char *enumstrs,char *unknownstr);
//enumstrs jsou stredniky oddelene nazvy hodnot vyctoveho typu,unknownstr
//je hodnota vyctoveho typu (retezec), vrati jeji hodnotu(pocitano od 1!!!)

chrs enumtostr(char *enumstrs,int enumtypevalue,char *nonestr="(NONE)");
int findvarinvars(char *varlist,char *var);

}

#endif
