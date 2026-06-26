#include <jlib.h>
#pragma hdrstop
//---------------------------------------------------------------------------

#include "stranal.h"

//#define _TEMPLATE export template<void (*jmpwhite_par)(char *&)>
//#define _STRANAL STRANAL<jmpwhite_par>

#define _TEMPLATE
#define _STRANAL STRANAL


namespace jlib {


void cjmpwhite(char *&c)
{
  while(*c) {
    if (uchar(*c)<=' ') {c++;continue;}
    if (*c=='/') {//mozna komantar
      if (c[1]=='/') {while(*c and *c!='\n') c++;c++;continue;}
      if (c[1]=='*') {
        while (*c) if (c[0]=='*' and c[1]=='/') {c+=2;break;} else c++;
        continue;
      }
    }
    break;
  }
}

EAnal::EAnal(int apos,char *format,...) {
  va_list args;
  va_start(args,format);
  vsprintf(msg,format,args);
  va_end(args);
  pos=apos;
}

_TEMPLATE bool _STRANAL::findtext(const char *text) {
  char *c=strstr(act,text);
  if (c) {
    act=c+strlen(text);return true;
  } else return false;
}


_TEMPLATE _STRANAL& _STRANAL::operator >>(IDFSTR idf) {
  char *r=seeidf(idf.buf);
  if (!r) throw EAnal("Zde ma byt identifikator",pos());
  act=r;
  return *this;
}

_TEMPLATE _STRANAL& _STRANAL::operator >>(QSTR s) {
  char *r=seestr(s.buf);
  if (!r) throw EAnal("Zde ma byt retezec",pos());
  act=r;
  return *this;
}

_TEMPLATE _STRANAL& _STRANAL::operator >>(int &val) {
  char *r=seeint(val);
  if (!r) throw EAnal("Zde ma byt cislo",pos());
  act=r;
  return *this;
}

_TEMPLATE _STRANAL& _STRANAL::operator >>(float &val) {
  char *r=seefloat(val);
  if (!r) throw EAnal("Zde ma byt realne cislo",pos());
  act=r;
  return *this;
}

_TEMPLATE _STRANAL& _STRANAL::operator >>(double &val) {
  char *r=seedouble(val);
  if (!r) throw EAnal("Zde ma byt realne cislo",pos());
  act=r;
  return *this;
}

_TEMPLATE _STRANAL& _STRANAL::operator <<(const IDFSTR idf) {
  jmpwhite(act);if (!checkidf(idf.buf)) throw EAnal(pos(),"Zde musi byt znak slovo '%s'",idf);
  return *this;
}

_TEMPLATE _STRANAL& _STRANAL::operator <<(const char *s) {
  jmpwhite(act);if (not strbegcmp(s,act)) throw EAnal(pos(),"Zde musi byt retezec '%s'",s);
  act+=strlen(s);
  return *this;
}


_TEMPLATE _STRANAL& _STRANAL::operator <<(char ch) {
  jmpwhite(act);if (*act!=ch) throw EAnal(pos(),"Zde musi byt znak '%c'",ch);
  act++;return *this;
}

_TEMPLATE bool _STRANAL::operator ==(const IDFSTR idf) {
  char *r=testidf(idf.buf);if (r) act=r;
  return r;
}

_TEMPLATE bool _STRANAL::operator ==(char ch) {
  char *c=act;jmpwhite(c);if (*c==ch) {act=c+1;return true;}
  return false;
}

_TEMPLATE bool _STRANAL::operator !=(char ch) {
  char *c=act;jmpwhite(c);return *c!=ch;
}

_TEMPLATE bool _STRANAL::operator !=(const char *s) {
  char *c=act;jmpwhite(c);return not strbegcmp(s,c);
}

_TEMPLATE bool _STRANAL::checkidf(const char *idf)
{
  jmpwhite(act);
  char *idfbeg=act;
  while (*idf==*act and *idf) idf++,act++;
  if (*idf) {act=idfbeg;return false;}
  if (isidfchar(*act)) {act=idfbeg;return false;}
  return true;
}

_TEMPLATE char *_STRANAL::seestr(char *s)
{
  char *c=act;
  jmpwhite(c);
  if (*c==QUOTE)
  {
    c++;
    UvozovkyUprestredRetezce:
    while (*c && *c!=QUOTE) *s++=*c++;
    if (*c==QUOTE && c[1]==QUOTE) {*s++=QUOTE;c+=2;goto UvozovkyUprestredRetezce;}
    *s=0;
    if (*c==QUOTE) {*c++;return c;}
    throw EAnal("Neukonceny retezec",pos());
  }
  return NULL;
}

_TEMPLATE char *_STRANAL::seeidf(char *s)
{
  char *c=act;
  jmpwhite(c);
  if (isbegidfchar(*c))
  {
    *s++=*c++;
    while (isidfchar(*c)) *s++=*c++;
  }
  *s=0;
  if (c>act) return c;else return NULL;
}

_TEMPLATE char *_STRANAL::seeint(int &val)
{
  char *c=act;jmpwhite(c);
  val=0;
  bool minus=*c=='-';if (minus) c++;
  while (*c>='0' and *c<='9') val=val*10+*c-'0',c++;
  if (minus) val=-val;
  if (c>act) return c;else return NULL;
}
_TEMPLATE char *_STRANAL::seefloat(float &val) {
  double v;
  char *res=seedouble(v);
  val=v;
  return res;
}
_TEMPLATE char *_STRANAL::seedouble(double &val) {
  char *c=act;
  jmpwhite(c);
  char *end;
  val=strtod(c,&end);
  if (end>c) return end;
  return NULL;
}

_TEMPLATE bool _STRANAL::aredata()
{ char *c=act;jmpwhite(c);return *c;}

_TEMPLATE bool _STRANAL::begwith(const char *s) {
  char *c=act;
  while(*s) if (*c++!=*s++) return false;
  act=c;return true;
}
_TEMPLATE bool _STRANAL::ubegwith(const char *s) {
  char *c=act;
  while(*s) if (toupper(*c++)!=toupper(*s++)) return false;
  act=c;return true;
}


_TEMPLATE char *_STRANAL::testidf(const char *idf)
{
  char *c=act;
  jmpwhite(c);
  while (*idf==*c and *idf) idf++,c++;
  if (*idf) return NULL;
  if (isidfchar(*c)) return NULL;
  return c;
}

_TEMPLATE _STRANAL &_STRANAL::skipto(char ch) {
  while (*act and *act!=ch) act++;
  if (*act!=ch) throw EAnal(pos(),"Nenalezen znak %c",ch);
  act++;
  return *this;
}

_TEMPLATE _STRANAL &_STRANAL::readto(char ch,char *out) {
  while (*act and *act!=ch) *out++=*act++;*out=0;
  if (*act!=ch) throw EAnal(pos(),"Nenalezen znak %c",ch);
  act++;
  return *this;
}

_TEMPLATE _STRANAL &_STRANAL::readto(const char *end,char *out) {
  while (*act and not strbegcmp(end,act)) *out++=*act++;
  *out=0;
  if (not strbegcmp(end,act)) throw EAnal(pos(),"Nenalezen retezec '%s'",end);
  act+=strlen(end);
  return *this;
}

_TEMPLATE _STRANAL &_STRANAL::readtochar(const char *endchars,char *out) {
  while (*act and strchr(endchars,*act)==NULL) *out++=*act++;*out=0;
  return *this;
}

_TEMPLATE _STRANAL &_STRANAL::cppreadto(char ch,char *out) {
  while (*act) {
    if (*act=='\\') {
      act++;
      if (*act==0) throw EAnal(pos(),"Zde nesmi byt konec souboru");
      *out++=*act++;continue;
    }
    if (*act==ch) break;
    *out++=*act++;
  }
  *out=0;
  if (*act!=ch) throw EAnal(pos(),"Nenalezen znak %c",ch);
  act++;
  return *this;
}


_TEMPLATE _STRANAL &_STRANAL::readto(char ch,STROUT &f) throw() {
  while (*act and *act!=ch) f<<*act++;
  if (*act) act++;
  return *this;
}


_TEMPLATE bool _STRANAL::operator ==(const char *s) {
  char *last=act;
  jmpwhite();
  if (begwith(s)) return true;
  act=last;return false;
}

_TEMPLATE void _STRANAL::readline(char *buf) {
  char *w=buf;
  for(;*act and *act!='\n';act++,w++) *w=*act;
  *w=0;
  if (*act) act++;trimr(buf);
}

_TEMPLATE string _STRANAL::readline() {
  char line[0x1000];
  readline(line);
  return string(line);
}

_TEMPLATE void _STRANAL::skipline() {
  for(;*act and *act!='\n';act++);
  if (*act) act++;
}

_TEMPLATE void _STRANAL::readline(STROUT &f) {
  char line[0x1000];readline(line);trimr(line);f<<line;
}

}

//#pragma package(smart_init)
