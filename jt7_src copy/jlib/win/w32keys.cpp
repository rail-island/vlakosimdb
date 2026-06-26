#include <winlib.h>
#pragma hdrstop

#include "w32keys.h"

namespace wlib {

#define enum_keys \
ek(enter,RETURN)\
ek(esc,ESCAPE)\
ek(space,SPACE)\
ek(f1,F1)\
ek(f2,F2)\
ek(f3,F3)\
ek(f4,F4)\
ek(f5,F5)\
ek(f6,F6)\
ek(f7,F7)\
ek(f8,F8)\
ek(f9,F9)\
ek(f10,F10)\
ek(f11,F11)\
ek(f12,F12)\
ek(up,UP)\
ek(down,DOWN)\
ek(left,LEFT)\
ek(right,RIGHT)\
ek(pgup,PRIOR)\
ek(pgdn,NEXT)\
ek(home,HOME)\
ek(end,END)\
ek(bs,BACK)\
ek(delete,DELETE)\
ek(insert,INSERT)\
ek(tab,TAB)\


/*
static int getcode(const char *s) {
  if (s[0]==0) return 0;
  if (s[1]==0) {
    if (isalnum(*s)) return toupper(*s);
    if (*s==' ') return kc_space;
    return 0;
  }
  #define ek(_s,_j,_w) if (upeqstr(s,#_s)) return kc_##_j;
  enum_keys
  #undef ek
  return 0;
}

void shortcuttostr(int key,char buf[0x10]) {
  *buf=0;
  int code=key&kc_key_mask;
  if (key&ks_shift) strcat(buf,"Shift+");
  if (key&ks_ctrl) strcat(buf,"Ctrl+");
  if (key&ks_alt) strcat(buf,"Alt+");
  if (isalnum(code)) {char *e=strend(buf);*e++=code;*e=0;}
  else {
    #define ek(_s,_j,_w) if (code==kc_##_j) strcat(buf,#_s);
    enum_keys
    #undef ek
  }
}


int strtoshortcut(const char *s) {
  int res=getcode(s);
  if (res) return res;
  bool loop=true;
  while (loop) {
    loop=false;
    if (insstrbegcmp("Ctrl+",s)) res|=ks_ctrl,s+=5,loop=true;
    if (insstrbegcmp("Alt+",s)) res|=ks_alt,s+=4,loop=true;
    if (insstrbegcmp("Shift+",s)) res|=ks_shift,s+=6,loop=true;
  }
  int key=getcode(s);
  if (key) return res|key;
  return 0;
}
*/

struct KeyData {
  unsigned int repeat:16;
  unsigned int scan:8;
  unsigned int extened:1;
  unsigned int reserved:4;
  unsigned int context:1;
  unsigned int pre:1;
  unsigned int trans:1;
};

static int keytoascii(int virtkey,int lparam) {
  union {
    KeyData data;
    int par;
  };
  par=lparam;

  char state[0x100];
  GetKeyboardState(state);
  WORD chs[0x10];
  int res=ToAscii(virtkey,data.scan,state,chs,0);

  if (res==1) return *chs;
  return 0;
}

inline bool isvkasciikey(int c) {return (c>='A' && c<='Z') || (c>='0' && c<='9');}

awsKey virtkeytokey(int virtkey,int lparam) {

  const int ALTMASK=0x20000000;

  awsKey res;

  int ascii=LOWORD(virtkey);

  #define ek(_j,_w) if (virtkey==VK_##_w) res=kt_##_j;
  enum_keys
  #undef ek

  if (!res.valid()) res.setchar(ascii);

/*  if (!res.valid()) {
    int ch=0;
    if (isvkasciikey(ascii)) ch=ascii;
    else ch=keytoascii(virtkey,lparam);
    if (ch) res.setchar(ch);
  }*/

//  if (! isvkasciikey(ascii)) ascii=keytoascii(virtkey,lparam);//  LOWORD(virtkey);

//  int res=0;
//  if (isalnum(ascii)) res=toupper(ascii);
//  #define ek(_s,_j,_w) if (virtkey==VK_##_w) res=kc_##_j;
//  enum_keys
//  #undef ek
  if (GetKeyState(VK_SHIFT)<0) res|=ks_shift;
  if (GetKeyState(VK_CONTROL)<0) res|=ks_ctrl;
  if (lparam&ALTMASK) res|=ks_alt;

  if (GetKeyState(VK_LSHIFT)<0) res.shift.teststate|=kst_lshift;
  if (GetKeyState(VK_RSHIFT)<0) res.shift.teststate|=kst_rshift;
  if (GetKeyState(VK_LCONTROL)<0) res.shift.teststate|=kst_lctrl;
  if (GetKeyState(VK_RCONTROL)<0) res.shift.teststate|=kst_rctrl;

//  if (ascii>=' ') res|=ascii;
  return res;
}

}


