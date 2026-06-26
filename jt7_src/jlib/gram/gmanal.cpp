#include <jlib.h>
#pragma hdrstop

#include "gmanal.h"

namespace gram {

CharSet Analyser::whites,Analyser::alphas,Analyser::digits,
        Analyser::alnums,Analyser::idfbegs,Analyser::idfs,
        Analyser::xmlspecs,Analyser::xmlidfs,Analyser::xmlloadspecs;

void Analyser::static_init() {
  static bool inited=false;
  if (inited) return;
  inited=true;
  whites.regload("[1]-[32]");
//  whites.setinterval(0,0xFF,false);whites.setinterval(1,' ',true);
  alphas.regload("a-zA-Z");
//  alphas.setinterval(0,0xFF,false);alphas.setinterval('a','z',true);alphas.setinterval('A','Z',true);
  digits.regload("0-9");
//  digits.setinterval(0,0xFF,false);digits.setinterval('0','9',true);
  alnums.regload("a-zA-Z0-9");
//  alnums.setinterval(0,0xFF,false);alnums.setinterval('a','z',true);alnums.setinterval('A','Z',true);alnums.setinterval('0','9',true);
  idfs.regload("a-zA-Z0-9_");
//  idfs.setinterval(0,0xFF,false);idfs.setinterval('_','_',true);idfs.setinterval('a','z',true);idfs.setinterval('A','Z',true);idfs.setinterval('0','9',true);
  idfbegs.regload("a-zA-Z_");
//  idfbegs.setinterval(0,0xFF,false);idfbegs.setinterval('_','_',true);idfbegs.setinterval('a','z',true);idfbegs.setinterval('A','Z',true);
  xmlspecs.regload("\"'<>&");
//  xmlspecs.setinterval(0,0xFF,false);xmlspecs.setchs("\"'<>&",true);
  xmlidfs.regload("-.:_a-zA-Z0-9");
//  xmlidfs.setinterval(0,0xFF,false);xmlidfs.setchs(".-:_",true);xmlidfs.setinterval('a','z',true);xmlidfs.setinterval('A','Z',true);xmlidfs.setinterval('0','9',true);
  xmlloadspecs.regload("<>&[0]");
//  xmlloadspecs.setinterval(0,0xFF,false);xmlloadspecs.setchs("<>&",true);xmlloadspecs.setch(0,true);
}

void Analyser::error(string msg) {
  throw AnalError(msg,act);
}

bool Analyser::seekch(char ch) {
  const char *c=act;
  while (is_white(*c) && *c!=ch) c++;
  if (*c==ch) return true;
  return false;
}

void Analyser::jcw() {
  retry:
  jmpwhite();
  if (*act=='/') {
    if (act[1]=='*') {
      jumpafter("*/");
      goto retry;
    }
    if (act[1]=='/') {
      jumpafter("\n");
      goto retry;
    }
  }
}

bool Analyser::readidf(char *buf,int bufsize) {
  if (bufsize==0) return false;
  bool res=is_idfbeg();
  if (res) {
    while (is_idf(*act) && bufsize!=1) *buf++=*act++,bufsize--;
  }
  *buf=0;
  return res;
}
string Analyser::readidf() {
  if (is_idfbeg()) {
    bstring res;
    while (is_idf()) res<<*act++;
    return res;
  } else error("Identifier expected");
  return string("");
}

double Analyser::readdouble(char *buf) {
  char *end;
  double res=strtod(act,&end);
  if (buf) strecpy(buf,act,end);
  act=end;
  return res;
}

int Analyser::readint() {
  int res=0;
  while (*act>='0' && *act<='9') {
    res*=10;
    res+=*act++-'0';
  }
  return res;
/*  int res=atoi(act);
  while (isdigit(*act)) act++;
  return res;*/
}

void Analyser::readline(char *buf,char end) {
  char *w=buf;
  while (*act && *act!=end) *w++=*act++;
  if (*act) act++;
  *w=0;w--;
  while (is_white(*w) && w>=buf) *w--=0;
}

void Analyser::readcstring(char *buf,char end) {
  while (*act && *act!=end) {
    if (*act=='\\') {
      act++;
      if (*act=='n') *buf++='\n';
      else *buf++=*act;
      if (*act) act++;
    } else *buf++=*act++;
  }
  if (*act) act++;
  *buf=0;
}
string Analyser::readcstring(char end) {
  bstring res;
  while (*act && *act!=end) {
    if (*act=='\\') {
      act++;
      if (*act=='n') res<<'\n';
      else res<<*act;
      if (*act) act++;
    } else res<<*act++;
  }
  if (*act) act++;
  return res;
}


void Analyser::jumpcstring(char end) {
  while (*act && *act!=end) {
    if (*act=='\\') {
      act++;
      if (*act) act++;
    } else act++;
  }
  if (*act) act++;
}


int Analyser::readto(char *buf,int size,const char *ends,bool jumplast) {
  size--;
  while (size>0 && *act) {
    const char *e=strchr(ends,*act);
    if (e) {
      *buf=0;
      if (jumplast) act++;
      return e-ends;
    }
    *buf++=*act++;size--;
  }
  *buf=0;
  return size>0?-1:-2;
}

string Analyser::readto(const char *ends,bool jumplast) {
  bstring res;
  while (*act) {
    const char *e=strchr(ends,*act);
    if (e) {
      if (jumplast) act++;
      break;
    }
    res<<*act++;
  }
  return res;
}

void Analyser::readto(const char *ends,STROUT &fw,bool jumplast) {
  while (*act) {
    const char *e=strchr(ends,*act);
    if (e) {
      if (jumplast) act++;
      break;
    }
    fw<<*act++;
  }
}


bool Analyser::jumpafter(const char *end) {
  const char *eor=strstr(act,end);
  if (eor) act=eor+strlen(end);else act=strend(act);
  return eor;
}

void Analyser::readexpr(char *buf,int bufsize,char beg,char end) {
  int level=0;
  bufsize--;
  while (*act && bufsize>0) {
    if (*act==beg) level++;
    if (*act==end) {
      if (level==0) {*buf=0;act++;return;}
      level--;
    }
    *buf++=*act++,bufsize--;
  }
  *buf=0;
}

/*
***********************************************
               CharSet
***********************************************
*/

static int readcharacter(Analyser &a) {
  if (*a=='#') {
    a++;
    int res=-1;
    if (*a=='\\') {
      a++;
      if (*a=='n') res='\n';
      if (*a) a++;
    } else res=*a,a++;
    return res;
  } else if (Analyser::is_digit(*a)) {
    return a.readint();
  }
  return -1;
}

void CharSet::load(const char *s) {
  Analyser a(s);
  while (*a) {
    bool add=true;
    if (*a=='-') a++,add=false;
    int beg=readcharacter(a);
    if (beg>=0) {
      if (*a=='.' && a[1]=='.') {
        a+=2;
        int end=readcharacter(a);
        setinterval(beg,end,add);
      } else {
        setinterval(beg,beg,add);
      }
    } else return;
    if (*a==',') a++;
  }
}

void CharSet::stdload(const char *c) {
  Analyser a(c);
  bool add=true;
  while (*a) {
    switch (*a) {
      case '\\':a++;if (*a) setch(*a,add),add=true,a++;break;
      case '^':a++;add=false;break;
      default:setch(*a,add),add=true,a++;break;
    }
  }
}

char CharSet::readchar(Analyser &a) {
  if (*a=='[') {
    a++;
    int ch=a.readint();
    a.f_char(']');
    return ch;
  } else if (*a=='\\') {
    a++;
    char ch=*a;
    if (ch=='n') ch='\n';
    else if (ch=='t') ch='\t';
    a++;
    return ch;
  }
  char ch=*a;
  a++;
  return ch;
}

void CharSet::regload(STRING s) {
  Analyser a(s.c_str());
  regload(a);
}

void CharSet::regload(Analyser &a) {
  setinterval(0,0xff,false);
  bool add=true;
  if (*a=='^') {add=false;a++;setinterval(1,0xff,true);}
  while (*a && *a!=']') {
    if (*a=='^') add=!add;
    char ch=readchar(a);
    if (*a=='-') {
      a++;
      char end=readchar(a);
      setinterval((uchar)ch,(uchar)end,add);
    } else setinterval((uchar)ch,(uchar)ch,add);
  }
}


}


