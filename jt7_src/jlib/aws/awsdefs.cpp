#include <jlib.h>
#pragma hdrstop

#include "awsdefs.h"

namespace aws {

//string tostring();
//void tostring(char buffer[0x20]);

#define enum_keys \
ek(Enter,enter)\
ek(Esc,esc)\
ek(Space,space)\
ek(F1,f1)\
ek(F2,f2)\
ek(F3,f3)\
ek(F4,f4)\
ek(F5,f5)\
ek(F6,f6)\
ek(F7,f7)\
ek(F8,f8)\
ek(F9,f9)\
ek(F10,f10)\
ek(F11,f11)\
ek(F12,f12)\
ek(Up,up)\
ek(Down,down)\
ek(Left,left)\
ek(Right,right)\
ek(PgUp,pgup)\
ek(PgDn,pgdn)\
ek(Home,home)\
ek(End,end)\
ek(BackSpace,bs)\
ek(Delete,delete)\
ek(Insert,insert)\
ek(Tab,tab)\

static void getcode(const char *s,awsKey &res) {
  if (s[0]==0) {res.setnone();return;}
  if (s[1]==0) {
    if (isalnum(*s)) {
      res.setchar(*s);
      return;
    }
    if (*s==' ') {res.settype(kt_space);return;}
    res.setnone();return;
  }
  #define ek(_s,_j) if (upeqstr(s,#_s)) {res.settype(kt_##_j);return;}
  enum_keys
  #undef ek
  res.setnone();return;
}

void awsKey::tostring(char buf[0x20]) {
  *buf=0;
  if (shift&ks_shift) strcat(buf,"Shift+");
  if (shift&ks_ctrl) strcat(buf,"Ctrl+");
  if (shift&ks_alt) strcat(buf,"Alt+");
  if (type==kt_char) {char *e=strend(buf);*e++=key;*e=0;}
  else {
    #define ek(_s,_j) if (type==kt_##_j) strcat(buf,#_s);
    enum_keys
    #undef ek
  }
}

static awsKey strtoshortcut(const char *s) {
  awsKey res;
  getcode(s,res);
  if (res.valid()) return res;
  bool loop=true;
  while (loop) {
    loop=false;
    if (insstrbegcmp("Ctrl+",s)) res|=ks_ctrl,s+=5,loop=true;
    if (insstrbegcmp("Alt+",s)) res|=ks_alt,s+=4,loop=true;
    if (insstrbegcmp("Shift+",s)) res|=ks_shift,s+=6,loop=true;
  }
  getcode(s,res);
  return res;
}

void awsKey::fromstring(STRING s) {
  *this=strtoshortcut(s);
}

void Rect::operator *=(const Rect &r) {
  if (r.a.x>a.x) a.x=r.a.x;
  if (r.a.y>a.y) a.y=r.a.y;
  if (r.b.x<b.x) b.x=r.b.x;
  if (r.b.y<b.y) b.y=r.b.y;
}

void Rect::operator +=(const Rect &r) {
  if (r.a.x<a.x) a.x=r.a.x;
  if (r.a.y<a.y) a.y=r.a.y;
  if (r.b.x>b.x) b.x=r.b.x;
  if (r.b.y>b.y) b.y=r.b.y;
}

}

