#include <jlib.h>
#pragma hdrstop

#include "gmsexpr.h"

namespace gram {

RegItem *loaditem(Analyser &a);

inline bool isiterator(Analyser &a) {return *a=='*' || *a=='+';}

static RegItem *loadspeccharitem(Analyser &a) {
  RegSpecChar *res=new RegSpecChar();
  res->type=*a;
  a++;
  if (*a==':') {
    a++;
    if (*a=='?') res->out=0,res->outasin=false,a++;
    else res->out=CharSet::readchar(a),res->outasin=false;
  } else res->outasin=true;
  return res;
}

static RegItem *loadcharitem(Analyser &a) {
  RegChar *res=new RegChar();
  res->ch=CharSet::readchar(a);
  if (*a==':') {
    a++;
    if (*a=='?') res->out=0,a++;
    else res->out=CharSet::readchar(a);
  } else res->out=res->ch;
  return res;
}

static RegItem *loadcharsetitem(Analyser &a) {
  RegCharSet *res=new RegCharSet();
  res->load(a);
  if (isiterator(a)) {
    RegIter *r=new RegIter(res);
    r->load(a);
    return r;
  }
  return res;
}

void RegCharSet::load(Analyser &a) {
  a.f_char('[');
  setin(0,0xff,false);
  setoutasin(0,0xff);
  bool add=true;
  if (*a=='^') {add=false;a++;setin(1,0xff,true);}
  while (*a && *a!=']') {
    if (*a=='^') add=!add;
    char ch=CharSet::readchar(a);
    if (*a=='-') {
      a++;
      char end=CharSet::readchar(a);
      setin((uchar)ch,(uchar)end,add);
      if (*a==':') {
        a++;
        if (*a=='?') setout((uchar)ch,(uchar)end,0),a++;
        else if (*a=='.') setoutasin((uchar)ch,(uchar)end);
        else {
          char out=CharSet::readchar(a);
          setdout((uchar)ch,(uchar)end,out);
        }
      } else setoutasin((uchar)ch,(uchar)end);
    } else {
      setin((uchar)ch,(uchar)ch,add);
      if (*a==':') {
        a++;
        if (*a=='?') setout((uchar)ch,(uchar)ch,0),a++;
        else {
          char out=CharSet::readchar(a);
          setdout((uchar)ch,(uchar)ch,out);
        }
      }
    }
  }
  a.f_char(']');
}

static RegItem *loadcompouditem(Analyser &a) {
  a.f_char('(');
  RegItem *res=loaditem(a);
  a.f_char(')');
/*  if (isiterator(a)) {
    RegIter *r=new RegIter(res);
    r->load(a);
    return r;
  }*/
  return res;
}

static RegItem *loadelemwithoutiter(Analyser &a) {
  if (*a=='!') {a++;return new RegNeg(loadelemwithoutiter(a));}
  if (*a=='(') return loadcompouditem(a);
  if (*a=='[') return loadcharsetitem(a);
  if (*a=='.') return loadspeccharitem(a);
  return loadcharitem(a);
}

static RegItem *loadelemitem(Analyser &a) {
  RegItem *res=loadelemwithoutiter(a);
  if (isiterator(a)) {
    RegIter *r=new RegIter(res);r->load(a);return r;
  }
  return res;
}

static RegItem *loadcatitem(Analyser &a) {
  RegCat *res=new RegCat();
  while (*a && *a!=';' && *a!=')' && *a!='|') {
    RegItem *act=loadelemitem(a);
    res->items.add(act);
  }
  return res;
}

static RegItem *loadoritem(Analyser &a) {
  RegOr *res=new RegOr();
  while (*a && *a!=';' && *a!=')') {
    RegItem *act=loadcatitem(a);
    res->items.add(act);
    if (*a=='|') a++;else break;
  }
  return res;
}

static RegItem *loaditem(Analyser &a) {
  return loadoritem(a);
}

RegItem *RegItem::load(Analyser &a) {
  return loaditem(a);
}

void RegIter::load(Analyser &a) {
  min=0;max=0xFFFFFFF;
  if (*a=='+') min=1,a++;
  if (*a=='*') a++;
}

//**********************************Parse****************************************

bool RegCharSet::parse(Analyser &a,RegResult &fw) {
  uchar ch=(uchar)*a;
  if (isin[ch]) {
    if (chout[ch]) fw<<chout[ch];
    a++;
    return true;
  }
  return false;
}

bool RegOr::parse(Analyser &a,RegResult &fw) {
  Analyser beg=a;
  for(int i=0;i<items.count;i++) {
    a=beg;
    RegResult res;
    if (items[i]->parse(a,res)) {
      fw<<res;
      return true;
    }
  }
  a=beg;
  return items.count==0;
}

bool RegSpecChar::parse(Analyser &a,RegResult &fw) {
  if (type=='.') {
    if (outasin) fw<<*a;
    else if (out) fw<<out;
    a++;
    return true;
  }
  xabstract;
  return false;
}

bool RegCat::parse(Analyser &a,RegResult &fw) {
  for(int i=0;i<items.count;i++)
    if (!items[i]->parse(a,fw)) return false;
  return true;
}

bool RegChar::parse(Analyser &a,RegResult &fw) {
  if (*a==ch) {
    if (out) fw<<out;
    a++;return true;
  }
  return false;
}

bool RegNeg::parse(Analyser &a,RegResult &fw) {
  RegResult res;
  Analyser was=a;
  bool ok=item->parse(a,res);
  a=was;
  return ! ok;
}

bool RegIter::parse(Analyser &a,RegResult &fw) {
  int count=0;
  RegResult res;
  while (count<=max) {
    Analyser was=a;
    int lastsize=res.count;
    bool ok=item->parse(a,res);
    if (res.count==lastsize) ok=false;//nic se neprecetlo:uz by se nikdy nic neprecetlo
    if (!ok) {
      a=was;
      fw<<res;
      return count>=min;
    }
    count++;
  }
  return true;
}


}


