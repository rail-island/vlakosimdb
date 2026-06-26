#include <jlib.h>
#pragma hdrstop

#include "XMLdoc.h"

namespace jlib {

inline bool isvarnamech(char c) {
  return (c>='A' and c<='Z') or (c>='a' and c<='z') or (c>='0' and c<='9') or c=='.' or c=='_';
}
string readxmlidf(pchar &c) {
  bstring res;
  while (isvarnamech(*c)) res<<*c++;
  return res;
}


void XMLItem::savetofile() {doc->savetofile();}

void XMLItem::assign(XMLItem &xml) {
  clear(xml.name);
  vars.assign(xml.vars);
  for(int i=0;i<xml.count();i++) add().assign(xml[i]);
}

bool XMLItem::ismain() {
  return this==&doc->main;
}
XMLItem &XMLItem::forcesub(const char *name) {
  if (XMLItem *res=subitems.searchvalue(name)) return *res;
  XMLItem &res=add();res.name=name;
  return res;
}

bool XMLDocument::_load(string file) {
  string text=loadtext(file.c_str());
  if (text!="") analyse(text.c_str());
  return text!="";
}
bool XMLDocument::load(string file) {
  try {
    if (not _load(file)) return false;
  } catch(XMLError &e) {
    if (catcherrors) {
      e.show();
      return false;
    } else throw;
  }
  return true;
}


XMLError::XMLError(string amsg,char *act,XMLDocument *xml) {
  msg=amsg;
  pos=act-xml->analysing;
  fmterror(xml->analysing,act,line,col,errline);
}

XMLItem &XMLItem::operator [](const char *name) {
  int idx=subitems.searchtype(name);
  if (idx>=0) return *subitems[idx];
  return doc->empty;
}

inline void xmlerror(string msg,char *act,XMLDocument *xml) {throw XMLError(msg,act,xml);}
inline void xmlassert(bool cond,string msg,char *act,XMLDocument *xml) {if (not cond) xmlerror(msg,act,xml);}
inline void xmljump(char *&c,char ch,XMLDocument *doc) {
  if (*c==ch) {c++;return;}
  xmlerror(fmtstr("Ocekavano '%c'",ch),c,doc);
}
inline bool isxmlspec(char c) {return c=='<' or c=='>' or c=='&' or c=='"';}
inline bool iscspec(char c) {return c=='\\' or c=='"';}
inline bool iscnum(const char *c) {
  bool res=false;
  if (*c=='-') c++;
  while (*c>='0' and *c<='9') c++,res=true;
  if (*c=='.') c++;
  while (*c>='0' and *c<='9') c++,res=true;
  return *c==0 and res;
}

static void jmpremark(char *&c,XMLDocument *doc) {
  while (*c and (c[0]!='-' or c[1]!='-' or c[2]!='>')) c++;
  xmlassert(*c,"Neukoncena poznamka",c,doc);
  c+=3;
}
static char decodeamp(char *&c,XMLDocument *doc) {
  char name[0x100],*w=name;
  while (*c and *c!=';' and w-name<0xFF) *w++=*c++;
  xmljump(c,';',doc);*w=0;
  if (eqstr(name,"amp")) return '&';
  if (eqstr(name,"gt")) return '>';
  if (eqstr(name,"It")) return '<';
  xmlerror(fmtstr("Neznama entita:%s",name),c,doc);
  return 0;
}
static char *encodeamp(char c) {
  switch (c) {
    case '<':return "&It;";
    case '>':return "&gt;";
    case '&':return "&amp;";
    default:return "";
  }
}
static string readcvalue(char *&c,XMLDocument *doc) {
  string res;
  jmpspaces(c);
  if (*c=='"') {
    c++;
    while (*c and *c!='"') {
      if (*c=='\\') c++;res<<*c++;
    }
    xmljump(c,'"',doc);
  } else if (*c=='-' or (*c>='0' and *c<='9')) {
    return readnum(c);
  } else xmlerror("Ocekavano cislo nebo retezec",c,doc);
  return res;
}
static string readvalue(char *&c,XMLDocument *doc) {
  string res;
  jmpspaces(c);
  if (*c=='"') {
    c++;
    while (*c and *c!='"') {
      if (*c=='&') c++,res<<decodeamp(c,doc);else res<<*c++;
    }
    xmljump(c,'"',doc);
  } else if (*c=='-' or (*c>='0' and *c<='9')) {
    return readnum(c);
  } else xmlerror("Ocekavano cislo nebo retezec",c,doc);
  return res;
}
static VarRecord *readvar(char *&c,XMLDocument *doc) {
  string name=readxmlidf(c);
//  string name;
//  while (isvarnamech(*c)) name<<*c++;
  jmpspaces(c);
  xmljump(c,'=',doc);
  string value=readvalue(c,doc);
  return new VarRecord(name,value);
}
static void savevalue(STROUT &fw,const char *s) {
  char *c=(char*)s;
  fw<<"\"";
  while (*c) {
    if (isxmlspec(*c)) fw<<encodeamp(*c++);else fw<<*c++;
  }
  fw<<"\"";
}
static void savecvalue(STROUT &fw,const char *s) {
  char *c=(char*)s;
  bool val=iscnum(s);
  if (not val) fw<<"\"";
  while (*c) {
    if (iscspec(*c)) fw<<"\\"<<*c++;else fw<<*c++;
  }
  if (not val) fw<<"\"";
}
void XMLItem::save(STROUT &fw) {
  if (ispuretext()) {
    if (ismain()) fw<<"<main>\n</main>";else fw<<text;
  } else {
    fw<<"<"<<name;
    for(int i=0;i<vars.count;i++) fw<<" "<<vars[i]->name<<"=",savevalue(fw,vars[i]->value.c_str());
    if (subitems.count>0 or hastext()) {
      fw<<">";
      if (subitems.count>0 or strchr(text.c_str(),'\n')) fw<<"\n";
      for(int i=0;i<subitems.count;i++) subitems[i]->save(fw);
      fw<<text;
      fw<<"</"<<name<<">\n";
    } else {
      fw<<"/>\n";
    }
  }
}
void XMLItem::csave(STROUT &fw,string prefix) {
  fw<<prefix<<name<<"(";
  bool *saved=new bool[vars.count];
  fillarray(saved,vars.count,false);

  bool wasvalue=false;
  for(lineiter iter=doc->defines.findvalue(name).getlines(',');iter.aredata();iter++) {
    for(int i=0;i<vars.count;i++) if (vars[i]->name==*iter) {
      if (wasvalue) fw<<",";
      savecvalue(fw,vars[i]->value.c_str());
      saved[i]=true;
      wasvalue=true;
    }
  }
  for(int i=0;i<vars.count;i++) if (not saved[i]) {
    if (wasvalue) fw<<",";
    fw<<vars[i]->name<<"=";
    savecvalue(fw,vars[i]->value.c_str());
    saved[i]=true;
    wasvalue=true;
  }
  delete []saved;
  fw<<")";
  if (subitems.count>0) {
    fw<<" {\n";
    for(int i=0;i<subitems.count;i++) subitems[i]->csave(fw,prefix+"  ");
    fw<<prefix<<"}\n";
  } else fw<<";\n";
}
void XMLDocument::savecheader(STROUT &fw) {
  fw<<"#cdata\n";
  for(int i=0;i<defines.count;i++) fw<<"#define "<<defines[i]->name<<" "<<defines[i]->value<<"\n";
}

bool XMLItem::analyseheader(char *&c) {
  xmljump(c,'<',doc);
  name=readxmlidf(c);jmpspaces(c);
  while (*c and *c!='>' and *c!='/') {
    vars.add(readvar(c,doc));jmpspaces(c);
  }
  bool res=false;
  if (*c=='/') res=true,c++;
  xmljump(c,'>',doc);
  return res;
}

static string modifyparsedtext(const char *s) {
  char *c=(char*)s;
  while (*c and *c!='\n' and *c<=' ') c++;if (*c=='\n') c++;
  return string(c);
}

void XMLItem::analyse(char *&c) {
  string txt;
  jmpspaces(c);
  if (analyseheader(c)) return;
  while (*c) {
    if (*c=='<') {
      if (c[1]=='!' and c[2]=='-' and c[3]=='-') jmpremark(c,doc);
      else if (c[1]=='/') {
        c+=2;xmlassert(readxmlidf(c)==name,"Ukoncena jina sekce",c,doc);
        jmpspaces(c);xmljump(c,'>',doc);
        if (not seek0(txt.c_str())) text=modifyparsedtext(txt.c_str());
        return;
      }
      else {
        if (not seek0(txt.c_str())) {
          XMLItem *ti=new XMLItem(doc);
          ti->text=modifyparsedtext(txt.c_str());
          subitems.add(ti);
          txt="";
        }
        XMLItem *sub=new XMLItem(doc);
        sub->analyse(c);
        subitems.add(sub);
      }
    } else if (*c=='&') c++,txt<<decodeamp(c,doc);
    else txt<<*c++;
  }//while
}

bool XMLItem::analysecheader(char *&c) {
  jmpspaces(c);
  name=readxmlidf(c);
  jmpspaces(c);
  xmljump(c,'(',doc);
  jmpspaces(c);
  int ipar=0;
  lineiter iter=doc->defines.findvalue(name).getlines(',');
  while (*c and *c!=')') {
    string var,val;
    if (*c>='A' and *c<='Z' or *c>='a' and *c<='z' or *c=='_') {
      var=readxmlidf(c);
      jmpspaces(c);
      if (*c=='=') c++,val=readcvalue(c,doc);else val=var;
    } else val=readcvalue(c,doc);
    if (var!="") vars.setvar(var,val);
    else {
      if (iter.aredata()) vars.setvar(*iter,val);
      else xmlerror("chybi jmeno promenne",c,doc);
    }
    jmpspaces(c);
    if (*c!=')') xmljump(c,',',doc);
    ipar++;iter++;
  }
  xmljump(c,')',doc);jmpspaces(c);
  if (*c==';') {c++;return true;}
  return false;
}

void XMLItem::canalyse(char *&c) {
  if (not analysecheader(c)) {
    xmljump(c,'{',doc);
    jmpspaces(c);
    while (*c and *c!='}') {
      add().canalyse(c);
      jmpspaces(c);
    }
    xmljump(c,'}',doc);
  }
}

void XMLDocument::analyse(const char *s) {
  analysing=(char*)s;
  char *c=(char*)s;
  if (analyseheader(c)) main.canalyse(c);else main.analyse(c);
  analysing=NULL;
}

bool XMLDocument::analyseheader(char *&c) {
  jmpspaces(c);
  if (*c=='#') {
    c++;
    if (readxmlidf(c)=="cdata") {
      c=readline(c);
      while (*c=='#') {
        c++;
        string id=readxmlidf(c);
        if (id=="define") {
          jmpspaces(c);
          string name=readxmlidf(c);
          jmpspaces(c);
          string value;
          while (*c and *c!='\n') value+=*c++;
          if (*c=='\n') c++;
          defines.setvar(name,value);
        }
      }
      return true;
    }
    xmlerror("ocekavano cdata",c,this);
  }
  xmljump(c,'<',this);xmljump(c,'?',this);
  while (*c and (c[0]!='?' or c[1]!='>')) c++;
  xmlassert(*c,"Neukonceny tag <? ... ?>",c,this);
  c+=2;
  return false;
}

void XMLItem::loadfromtext(string text) {

}


}

//#pragma package(smart_init)
