#include <jlib.h>
#pragma hdrstop

#include "regexintf.h"
#include "regex.h"

namespace jlib {

string StringOffset::get(string whole) {
  string res;
  if (end>begin && begin>=0) {
    return whole.copy(begin,end-begin);
  }
  return res;
}

void RegularExpr::compile(STRING par_regex) {
  clear();
  re=regcomp(par_regex.c_str());
}

bool RegularExpr::run(string atext) {
  if (!re) return false;
  text=atext;
  return regexec(re,text.c_str());
}

StringOffset RegularExpr::offset(int index) {
  if (index>=NSUBEXP) return StringOffset();
  if (!re) return StringOffset();
  if (!re->startp[index]) return StringOffset();
  return StringOffset(re->startp[index]-text.c_str(),re->endp[index]-text.c_str());
}
int RegularExpr::subcount() {
  if (!re) return 0;
  if (!re->startp[0]) return 0;
  for(int i=1;i<NSUBEXP;i++) {
    if (!re->startp[i]) return i-1;
  }
  return NSUBEXP-1;
}

string RegularExpr::replace_with_subs(string replacestr) {
  MEMSTROUT mso;
  for(const char *c=replacestr.c_str();*c;) {
    if (*c=='\\' && c[1]>='1' && c[1]<='9') {
      c++;
      mso<<str(*c-'0');
      c++;
    } else mso<<*c++;
  }
  return mso.str();
}

string RegularExpr::replace(string atext,string replace_with) {
  if (run(atext)) {
    atext.erase(offset(0).begin,offset(0).length());
    atext.insert(offset(0).begin,replace_with_subs(replace_with));
    return atext;
  } else return atext;
}

string RegularExpr::replaceall(string atext,string replace_with) {
  while (run(atext)) {
    atext.erase(offset(0).begin,offset(0).length());
    atext.insert(offset(0).begin,replace_with_subs(replace_with));
  }
  return atext;
}

string makeexregex(string src) {
  bstring res;
  bool casesen=true,inset=false,negset=false;
  const char *c=src.c_str();
  while (*c) {
    if (*c=='\\') {
      if (c[1]=='n') res<<'\n',c+=2;
      else if (c[1]=='t') res<<'\t',c+=2;
      else if (c[1]=='r') res<<'\r',c+=2;
      else if (c[1]=='c') {
        if (c[2]=='+') casesen=true,c+=3;
        else if (c[2]=='-') casesen=false,c+=3;
        else res<<*c++;
      } else {
        res<<*c++;
        if (*c) res<<*c++;
      }
    } else if (*c=='[') {
      inset=true;negset=false;
      res<<*c++;
    } else if (*c==']' && inset) {
      inset=false;
      res<<*c++;
    } else {
      if (!casesen && ((*c>='a' && *c<='z') || (*c>='A' && *c<='Z'))) {
        if (inset) res<<toupper(*c)<<tolower(*c);
        else res<<"["<<toupper(*c)<<tolower(*c)<<"]";
        c++;
      } else res<<*c++;
    }
  }
  return res;
}

}
