#include <jlib.h>
#pragma hdrstop

#include "dbs.h"

namespace jlib {

void DBSCRIPT::fatalerror(const char *msg) {
  *out<<"[FATAL ERROR:"<<msg<<"]";
  act=strend(act);
}
void DBSCRIPT::fatalerrorfmt(const char *msg,...) {
  va_list args;
  va_start(args,msg);
  char s[0x1000];
  vsprintf(s,msg,args);
  va_end(args);
  *out<<"[FATAL ERROR:"<<s<<"]";
  act=strend(act);
}

string DBSCRIPT::jumpnextcom(bool behind) {
  while (*act) {
    if (*act=='\\') {
      if (act[1]) act++;
    } else if (*act=='#' and act[1]=='[') {
      char *before=act;
      act++;
      string res=readpar('[',']');
      if (not behind) act=before;
      return res;
    } else act++;
  }
  return "";
}

void DBSCRIPT::COM::init(const char *s,char divider) {
  strcpy(buf,s);
  char *c=buf;
  cnt=0;
  while (*c) {
    pars[cnt++]=c;
    while (*c and *c!=divider) c++;
    if (*c==divider) *c=0,c++;
  }
  for(int i=cnt;i<MAXPARS;i++) pars[i]="";
}

bool rundbsonpart(const char *input,const char *begin,const char *end,DBSCRIPT &s,STROUT &output) {
  char *b=(char*)strstr(input,begin);
  if (b) {
    b+=strlen(begin);
    char *e=strstr(b,end);
    if (e) {
      char pam=*e;*e=0;
      s.parse(b,output);
      *e=pam;
      return true;
    }
  }
  return false;
}

string DBSCRIPT::readto(const char *end) {
  char res[0x100],*rw=res;
  int size=0xFF;
  while ( strchr(end,*act)==NULL and *act and size) *rw++=*act++,size--;
  if (strchr(end,*act)==NULL) return fmtstr("[CHYBA:zde ma byt jeden ze znaku '%s']",end);
  *rw=0;
  return string(res);
}

void DBSCRIPT::runfunc(const char *name) {
  char *ret=act;
  jumpbehind(fmtstr("#%s:",name).c_str());
  _parse(name);
  act=ret;
}

string DBSCRIPT::readpar(char beg,char end) {
  if (*act!=beg) return fmtstr("[CHYBA:zde ma byt znak '%c']",beg);
  act++;
  char res[0x100],*rw=res;
  int size=0xFF;int level=0;
  while ( (level>0 or *act!=end) and *act and size) {
    if (*act==beg) level++;
    *rw++=*act++,size--;
  }
  if (*act==end) act++;else fmtstr("[CHYBA:zde ma byt znak '%c']",end);
  *rw=0;
  return string(res);
}

void DBSCRIPT::jmpnestedremark() {
  int level=1;
  while (level and *act) {
    if (*act=='#') {
      act++;
      if (*act==':') level++,act++;else if (*act=='.') level--,act++;
    } else act++;
  }
  if (level) *out<<"[Chyba:neukoncena poznamka]";
}

void DBSCRIPT::jumpbehind(const char *what) {
  char *c=strstr(act,what);
  if (c) act=c+strlen(what);else act=strend(act);
}
void DBSCRIPT::jumpbefore(const char *what) {
  char *c=strstr(act,what);
  if (c) act=c;else act=strend(act);
}

void DBSCRIPT::_parse(const char *procname) {
  while (*act) {
    if (*act=='\\') {act++;if (*act) *out<<STROUT::CHAR(*act),act++;} //prejet znak
    else if (*act=='$') {act++;*out<<getvalue(readpar('[',']'));}
    else if (*act=='_') {act++;jmpspaces(act);}
    else if (*act=='#') {
      act++;
      if (*act=='[') {string com=readpar('[',']');docommand(com);}
      else if (*act==':') jmpnestedremark();
      else {
        string proc=readto(":.");
        if (*act==':') {
          jumpbehind(fmtstr("#%s.",proc.c_str()).c_str());
        } else {
          if (procname and proc==procname) {act++;return;}
          *out<<fmtstr("[Chyba:Ukonceni nezacate procedury %s",proc.c_str());
        }
      }
    } else *out<<STROUT::CHAR(*act++);
  }
}

}

#pragma package(smart_init)
