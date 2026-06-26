#include <jlib.h>
#pragma hdrstop

#include "linet.h"

namespace jlib {

bool LINET::controltype(char *filetype)
{
  if (filetype && *linetbuf)
  {
    //kontrola,zda prvni radka je ve tvaru .TEXT.<filetype>
    bool sedi=strbegcmp(".TEXT.",linetbuf);
    if (sedi) sedi=eqstr(linetbuf+strlen(".TEXT."),filetype);
    return sedi;
  }
  return false;
}

TEXTFILE::TEXTFILE(char *filename,char *filetype)
{
  f=fopen(filename,"r");
  if (!f) return;
  eof=false;
  readlinetobuf();
  if (controltype(filetype)) readlinetobuf();
}

void TEXTFILE::readlinetobuf()
{
  if (eof) return;
  eof=(fgets(linetbuf,staticbufsize,f)==NULL);
  if (eof) {fclose(f);f=NULL;*linetbuf=0;} else trimr(linetbuf);
}


}

#pragma package(smart_init)
