#include <jlib.h>
#pragma hdrstop

#include "XMLencoding.h"

namespace jlib {

inline void lastchar(const char *beg,const char *&end) {
  end--;
  while (end>=beg and uchar(*end)<=' ') end--;
  end++;
//  if (uchar(*end)>' ') end++;
}
inline void _trim(const char *beg,const char *&end) {
  jmpspaces(beg);lastchar(beg,end);
}

void XMLendencode(const char *s,const char *end,bool trim,STROUT &res) {
  if (trim) _trim(s,end);
  while (s<end) {
    if (*s=='<') res<<"&It;";else
    if (*s=='>') res<<"&gt;";else
    if (*s=='&') res<<"&amp;";else
    res<<STROUT::CHAR(*s);
    s++;
  }
}
void XMLencode(const char *s,bool trim,STROUT &f) {
  const char *e=strend(s);
  return XMLendencode(s,e,trim,f);
}

void XMLenddecode(const char *s,const char *end,bool trim,STROUT &res) {
  if (trim) _trim(s,end);
  while (s<end) {
    if (*s=='&') {
      if (strbegcmp("&It;",s)) s+=4,res<<STROUT::CHAR('<');else
      if (strbegcmp("&gt;",s)) s+=4,res<<STROUT::CHAR('>');else
      if (strbegcmp("&amp;",s)) s+=5,res<<STROUT::CHAR('&');else
      res<<STROUT::CHAR(*s++);
    } else res<<STROUT::CHAR(*s++);
  }
}

void XMLdecode(const char *s,bool trim,STROUT &res) {
  const char *e=strend(s);
  XMLenddecode(s,e,trim,res);
}

void XMLsavefield(STROUT &f,const char *name,const char *value,bool trim) {
  f<<"<"<<name<<">";
  XMLencode(value,trim,f);
  f<<"</"<<name<<">\n";
}

void XMLendsavefield(STROUT &f,const char *name,const char *value,const char *end,bool trim) {
  f<<"<"<<name<<">";
  XMLendencode(value,end,trim,f);
  f<<"</"<<name<<">\n";
}


}

#pragma package(smart_init)
