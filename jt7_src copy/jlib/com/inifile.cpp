#include <jlib.h>
#pragma hdrstop

#include "inifile.h"
#include <ext/hexstrm.h>

namespace jlib {

INIFILE *prgini=NULL;

string INIFILE::readsection(const char *sect) {
  STRLIST s;readsection(sect,s);return s.autotext();
}
void INIFILE::writesection(const char *sect,const char *str) {
  STRLIST s;s.assignfrombuf(str);writesection(sect,s);
}

void INIFILE::writehexsect(const char *sect,const void *buf,int size) {
  MEMHEXWRITE mhw;
  mhw.writedata(buf,size);
  writesection(sect,mhw.str().c_str());
}

void INIFILE::writenumssect(const char *sect,int *nums,int count) {
  STRLIST s;
  for(int c=count,*n=nums;c>0;c--,n++) s+=itos(*n).c_str();
  writesection(sect,s);
}

int INIFILE::readnumssect(const char *sect,int *nums,int maxcount) {
  STRLIST s;
  readsection(sect,s);
  int cnt=max(maxcount,s.count);
  for(int i=0,*n=nums;i<cnt;i++,n++) *n=atoi(s[i]);
  return cnt;
}

void INIFILE::readintsect(const char *sect,TCntVec<int> &res) {
  STRLIST s;
  readsection(sect,s);res.clear();
  for(int i=0;i<s.count;i++) res+=atoi(s[i]);
}


bool INIFILE::readsection(const char *sect,STRLIST &lines)
{
  lines.clear();
  char sbeg[0x80],send[0x80];
  sprintf(sbeg,"[%s]",sect);
  sprintf(send,"[/%s]",sect);
  bool writing=false;
  for(int i=0;i<count;i++)
  {
    if (writing)
    {
      if (eqstr(send,item(i))) return true;
      lines+=item(i);
    } else
    if (eqstr(sbeg,item(i))) writing=true;
  }
  return false;
}

bool INIFILE::erasesection(const char *sect)
{
  char sbeg[0x80],send[0x80];
  sprintf(sbeg,"[%s]",sect);
  sprintf(send,"[/%s]",sect);
  int beg=search(sbeg),end=search(send);
  if (beg<end and beg>=0)
  {
    erase(beg,end-beg+1);
    return true;
  }
  return false;
}

void INIFILE::writesection(const char *sect,STRLIST &lines)
{
  char sbeg[0x80],send[0x80];
  sprintf(sbeg,"[%s]",sect);
  sprintf(send,"[/%s]",sect);
  erasesection(sect);
  add(sbeg);
  add(lines);
  add(send);
}

int INIFILE::setvalue(const char *sect,const char *var,int value) {
  char s[0x100];sprintf(s,"%d",value);return setvalue(sect,var,s);
}
int INIFILE::setvalue(const char *sect,const char *var,double value) {
  char s[0x100];sprintf(s,"%f",value);return setvalue(sect,var,s);
}


int INIFILE::findsect(const char *sect)
{
  char s[0x100];
  sprintf(s,"[%s]",sect);
  for(int i=0;i<count;i++) if (eqstr(s,item(i))) return i;
  return -1;
}
int INIFILE::findinivar(const char *sect,const char *var)
{
  char sbeg[0x80],send[0x80];
  sprintf(sbeg,"[%s]",sect);
  sprintf(send,"[/%s]",sect);

  int i0=search(sbeg);
  if (i0<0) return -1;
  for (int i=i0+1;i<count;i++)
  {
    if (eqstr(item(i),send)) return -1;
    if (islinevar(item(i),var)) return i;
  }
  return -1;
}
char *INIFILE::getvalue(const char *sect,const char *var)
{
  int i=findinivar(sect,var);
//  eclear();
//  eputs(autotext().c_str());
  if (i>=0) return strchr(item(i),'=')+1;
  return NULL;
}
int INIFILE::setvalue(const char *sect,const char *var,const char *value)
{
  int i=findinivar(sect,var);
  char s[0x200];
  sprintf(s,"%s=%s",var,value);
  if (i>=0)
  {
    set(i,s);
    return i;
  } else
  {//promenna jeste neexistuje(v dane sekci)
    int si=findsect(sect);
    if (si>=0) {insert(si+1,s);return si+1;}
    else//neexistuje ani sekce
    {
      char sec[0x100];
      sprintf(sec,"[%s]",sect);add(sec);
      add(s);
      sprintf(sec,"[/%s]",sect);add(sec);
      return count-2;
    }
  }
}
int INIFILE::getintdef(const char *sect,const char *var,int def)
{
  if (!this) return def;
  char *v=getvalue(sect,var);
  if (v) return atoi(v);return def;
}
double INIFILE::getrealdef(const char *sect,const char *var,double def)
{
  if (!this) return def;
  char *v=getvalue(sect,var);
  if (v) return atof(v);return def;
}


chrs INIFILE::getstrdef(const char *sect,const char *var,chrs def)
{
  if (!this) return def;
  char *v=getvalue(sect,var);
  if (v) return v;return def;
}

INIFILE::INIFILE(const char *file)
{
  init();
//  filename[0]=0;
  if (*file) {
    char s[0x200];
    strcpy(s,basepath);
    strcat(s,file);
    strcpy(filename,s);
    load(s);
  }
}

}

//#pragma package(smart_init)
