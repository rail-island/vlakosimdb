#include <jlib.h>
#pragma hdrstop

#include "jstring.h"

namespace jlib {

#ifdef STRINGCONTROL
int dbg_string_count=0;
#endif

string emptystring;

#ifdef STRINGCONTROL
void string::do_string_control() {
  if (buf) {
    xassert(abs((int)buf)>0x1000);
    xassert(getrec()->refcnt>0);
    xassert(getrec()->magic==STRINGMAGIC);
    if (strlen(buf)!=getrec()->len) {
      xthrowmsg(XError,("strlen(buf)!=getrec()->len,buf='%s',strlen(buf)=%d,getrec()->len=%d",
      buf,strlen(buf),getrec()->len));
    }
    xassert(strlen(buf)<=getrec()->capa);
    xassert(getrec()->len==getrec()->len2);
  }
}

string::~string() {
  do_string_control();
  release();
}

#endif

#ifdef STRINGDESTROYCONTROL
string::~string() {
  if (buf) {
    if (abs((int)buf)<0x600000) {
//    xassert(abs((int)buf)>0x600000);
      xassertmsg(getrec()->refcnt>0,("refcnt=%d,adr=%x",getrec()->refcnt,buf));
    }
  }
  release();
}
#endif

void string::clear() {
  if (buf) {
    stringrecord *rec=getrec();
    if (rec->refcnt>1) {
      rec->refcnt--;buf=NULL;
    } else {
      rec->len=0;
      #ifdef STRINGCONTROL
      rec->len2=0;
      #endif
      *buf=0;
    }
  }
}

void string::updatelen() {
  if (buf) {
    getrec()->len=strlen(buf);
    #ifdef STRINGCONTROL
    getrec()->len2=getrec()->len;
    #endif
    xassert(getrec()->len<=getrec()->capa);
  }
  _ctrl_str;
}

bool string::replace(STRING oldstr,STRING newstr) {
  const char *pos=strstr(c_str(),oldstr);
  if (pos) {
    int ip=pos-c_str();
    unique();
    erase(ip,strlen(oldstr));
    insert(ip,newstr);
    return true;
  }
  return false;
}
int string::replaceall(STRING oldstr,STRING newstr) {
  int res=0;
  while (replace(oldstr,newstr)) res++;
  return res;
}

/*
string string::copy(int index,int count) {
  string res;
  if (index+count>length()) count=length()-index;
  if (count<0) return res;
  res.initbuf(count);
  strecpy(res.buf,c_str()+index,c_str()+index+count);
  return res;
}
*/
string string::gettrimed() {
  string res(gettriml(c_str()));
  res.trimr();
  return res;
}

char *string::reservebuffer(int len) {
  xassert(buf==NULL);
  initbuf(len);
  return buf;
}
char *string::reservebuffer(const char *src) {
  xassert(buf==NULL);
  initbuf(src);
  return buf;
}

void string::makelower() {
  unique();
  if (buf) for(char *c=buf;*c;c++) *c=tolower(*c);
}
void string::makeupper() {
  unique();
  if (buf) for(char *c=buf;*c;c++) *c=toupper(*c);
}

void string::insert(int pos,string what) {
  unique();
  xassert(pos<=length());
  int newlen=length()+what.length();
  wantcapa(newlen);
  strins(buf+pos,what.c_str());
  setlen(newlen);
}
void string::erase(int pos,int count) {
  xassert(pos<length());
  if (buf) {
    unique();
    strdel(buf+pos,buf+pos+count);
    updatelen();
  }
}
string::string(const char *src,int srclen,int pos,int count) {
  if (src and pos<srclen) {
    initbuf(count,0);
    strecpy(buf,src+pos,src+pos+count);
    updatelen();
  } else buf=NULL;
}
void string::sprintf(const char *format,...) {
  unique();
  wantcapa(SPRINTF_BUF_SIZE);
  va_list args;
  va_start(args,format);
  std::vsprintf(buf,format,args);
  va_end(args);
  int len=strlen(buf);
  xassert(len<SPRINTF_BUF_SIZE);
  updatelen();
}

void string::vsprintf(const char *format,va_list args) {
  unique();
  wantcapa(SPRINTF_BUF_SIZE);
  std::vsprintf(buf,format,args);
  int len=strlen(buf);
  xassert(len<SPRINTF_BUF_SIZE);
  updatelen();
}

bool string::begins(const char *s) {
  return strbegcmp(s,c_str());
}
bool string::ends(const char *s) {
  return strendcmp(s,c_str());
}

void string::setlen(int newlen) {
  xassert(buf);
  REC *r=getrec();
  xassert(r->refcnt==1);
  xassert(newlen<=r->capa);
  buf[newlen]=0;
  r->len=newlen;
  #ifdef STRINGCONTROL
  r->len2=r->len;
  #endif
}


void string::trimr() {
  unique();
  if (buf) {
    jlib::trimr(buf);
    updatelen();
  }
}

string::REC * string::wantcapa(int size) {
  if (buf) {
    register REC *r=getrec();
    if (size>r->capa) {
      int x=newcapa(size);
      REC *nr=(REC*)malloc(x+sizeof(REC)+1);
      nr->init(r->len,x);
      strcpy(nr->gets(),buf);
      r->release();
      buf=nr->gets();
      _ctrl_str;
      return nr;
    } else {
      _ctrl_str;
      return r;
    }
  } else return initbuf(size,0);
}

void string::add(char ch) {
  unique();
  REC *r=wantcapa(length()+1);
  _ctrl_str;
  buf[r->len++]=ch;buf[r->len]=0;
  #ifdef STRINGCONTROL
  r->len2++;
  #endif
  _ctrl_str;
}
void string::add(const char *str) {
  unique();
  int len=strlen(str);
  REC *r=wantcapa(len+length());
  strcat(buf,str);
  updatelen();
  _ctrl_str;
}

void string::add(const string &str) {
  if (buf==NULL) {
    buf=str.buf;
    if (buf) getrec()->refcnt++;
    return;
  }
  unique();
  int len=str.length();
  REC *r=wantcapa(len+length());
  strcat(buf,str.c_str());
  buf[r->len+=len]=0;
  #ifdef STRINGCONTROL
  r->len2=r->len;
  #endif
  _ctrl_str;
}

string string::stringat(int index,char separator) {
  for(lineiter it=getlines(separator);it.aredata();it++,index--) if (!index) return *it;
  return "";
}
int string::itemcount(char separator) {
  int res=0;
  for(lineiter it=getlines(separator);it.aredata();it++) res++;
  return res;
}
int string::searchinlines(const char *str,char separator) {
  int index=0;
  for(lineiter it=getlines(separator);it.aredata();it++,index++) if (eqstr(str,*it)) return index;
  return -1;
}


}

//#pragma package(smart_init)
