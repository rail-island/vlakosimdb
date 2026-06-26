#include <jlib.h>
#pragma hdrstop

#include "langman.h"
#include <gram/gmanal.h>

namespace jlib {

static int dummy_int=0;
LanguageManager *langman=NULL;

void init_lang() {
  langman=new LanguageManager();
}
void done_lang() {
  delete langman;
  langman=NULL;
}

string lang_text(string msg) {
  return langman->get_upper(msg);
}
string lang_text_ch(string msg,char spec) {
  gram::Analyser a(msg.c_str());
  bstring res;
  while (*a) {
    if (*a==spec) {
      a++;
      bstring name;
      while (a.is_idf() || *a=='-') name<<*a,a++;
      if (langman) res<<langman->get_upper(name);else res<<name;
    } else res<<*a,a++;
  }
  return res;
//  if (msg[0]==spec) return langman.get_upper(msg.copy(1,msg.length()-1));
//  return msg;
}

string LanguageManager::get_upper(string name) {
  char *name2=strnew(name.c_str());
  bool up=false;
  if (name2[0]>='A' && name2[0]<='Z') {
    name2[0]+=('a'-'A');
    up=true;
  }
  string res=mapmodify(items.svar(name2));
  free(name2);
  char *res2=strnew(res.c_str());
  if (up) {
    char p[2];
    p[0]=res2[0];p[1]=0;
    jsystem->uppercase(p);
    res2[0]=p[0];
  }
  res=res2;
  free(res2);
  if (res=="") return getdef(name);
  return res;
}
string LanguageManager::mapmodify(string str) {
  bstring res;
  const char *map=charmap.c_str();
  for(const char *c=str.c_str();*c;c++) {
    char ch=*c;
    const char *m=map;
    while (*m) {
      if (*m==ch) {
        if (m[1]) ch=m[1];
        break;
      }
      if (m[1]) m+=2;else break;
    }
    res<<ch;
  }
  return res;
}
string LanguageManager::get(string name) {
  string res=mapmodify(items.svar(name));
  if (res=="") return getdef(name);
  return res;
}
void LanguageManager::loadfromfile(string file) {
  charmap="";
  items.clear();
  loadfromtext(sFile(file).getdir(),loadtext(file.c_str()));
}
void LanguageManager::loadfromtext(sFile dir,STRING text) {
  gram::Analyser a(text.c_str());
  while (*a) {
    string name=a.readto("=",true);
    bstring value;
    while (*a) {
      if (*a=='\n') break;
      if (*a=='\r') a++;
      else if (*a=='|') value<<'\n',a++;
      else if (*a=='\\') {
        a++;
        if (*a) {
          if (a.seekch('\n')) {
            while (*a && *a!='\n') a++;
            if (*a) a++;
          } else if (*a=='n') {
            value<<'\n',a++;
          } else {
            value<<*a,a++;
          }
        }
      } else value<<*a,a++;
    }
    while (*a && *a!='\n') a++;
    if (*a) a++;
    string sname=name;
    if (sname=="#charmap") {
      charmap=value;
    } else if (sname=="#id") {
      langid=value;
    } else if (sname=="#include") {
      loadfromtext(dir,loadtext(dir/string(value)));
    } else items.setvar(sname,value);
  }
}
string LanguageManager::getdef(string name) {
  bstring res;
  for(const char *c=name.c_str();*c;c++) {
    if (*c=='-') res<<' ';else res<<*c;
  }
  return res;
}
void load_lang_file(string file) {
  if (langman) langman->loadfromfile(file);
}

}

