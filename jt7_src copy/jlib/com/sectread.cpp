#include <jlib.h>
#pragma hdrstop

#include "sectread.h"

namespace jlib {


void SECTREAD::readlinetobuf()
{
  if (eof) return;
  char *cn=readline(linetact);
  strecpy(linetbuf,linetact,cn);
  trimr(linetbuf);
  if (eqstr(endsect,linetbuf))
  {
    eof=true;
    *linetbuf=0;
    linetact=NULL;
  } else linetact=cn;
}
void SECTREAD::resetlinet()
{
  linetact=linetbeg;
}
void SECTREAD::closelinet()
{
  *linetbuf=0;eof=true;linetact=NULL;linetbeg=NULL;*endsect=0;
}
SECTREAD::SECTREAD(char *file)
{
  filebuf=NULL;
  FILE *fr=fopen(file,"rb");
  if (fr==NULL) {failed=true;return;}
  failed=false;
  fseek(fr,0,SEEK_END);
  int size=ftell(fr)+1;
  fseek(fr,0,SEEK_SET);
  filebuf=challoc(size+2);
  size=fread(filebuf,1,size,fr);
  fclose(fr);
  filebuf[size]=0;

  closelinet();
}
SECTREAD::SECTREAD(SECTREADBUFFER buf) {
  filebuf=strnew(buf.buf);
  closelinet();
}

bool SECTREAD::initlinet(const char *sect)
{
  linetbeg=findline(fmtstr("[%s]",sect).c_str());
  if (linetbeg==NULL) return false;
  sprintf(endsect,"[/%s]",sect);
  linetbeg=readline(linetbeg);
  eof=false;
//  readlinetobuf();
  return true;
}
char *SECTREAD::findline(const char *trimredline)
{
  char *c=filebuf;
  while (*c)
  {
    char *cn=readline(c);
    char line[0x100];
    strecpy(line,c,cn);
    trimr(line);
    if (eqstr(line,trimredline)) return c;
    c=cn;
  }
  return NULL;
}

char *SECTREAD::findvar(const char *sect,const char *var)
{
  char sbeg[0x80],send[0x80];
  sprintf(sbeg,"[%s]",sect);
  sprintf(send,"[/%s]",sect);

  char *c=findline(sbeg);
  if (c==NULL) return NULL;
  while (*c)
  {
    RTB(c,cn,line);
    if (eqstr(line,send)) return NULL;
    if (islinevar(line,var)) return c;
    c=cn;
  }
  return NULL;
}
bool SECTREAD::getvalue(const char *sect,const char *var,char *value)
{
  *value=0;
  char *c=findvar(sect,var);
  if (!c) return false;
  RTB(c,cn,line);
  strcpy(value,strchr(line,'=')+1);
  return true;
}
int SECTREAD::getintdef(const char *sect,const char *var,int def)
{
  char val[0x10];
  if (getvalue(sect,var,val)) return atoi(val);else return def;
}
chrs SECTREAD::getstrdef(const char *sect,const char *var,chrs def)
{
  char val[0x100];
  if (getvalue(sect,var,val)) return (chrs)val;else return def;
}
SECTREAD *SECTREAD::makesubsect(const char *sect)
{
  char sbeg[0x80],send[0x80];
  sprintf(sbeg,"[%s]",sect);
  sprintf(send,"[/%s]",sect);

  char *beg=findline(sbeg);
  if (!beg) return NULL;
  beg=readline(beg);
  char *c=beg;
  while (*c)
  {
    RTB(c,cn,line);
    if (eqstr(line,send))
    {
      SECTREAD *res=new SECTREAD;
      res->failed=false;
      res->filebuf=challoc(c-beg+1);
      strecpy(res->filebuf,beg,c);
      return res;
    }
    c=cn;
  }
  return NULL;
}

}
