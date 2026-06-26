#include <jlib.h>
#pragma hdrstop

#include "xmlload.h"

namespace jlib {

struct XMLLoader {
string xml_text;
VarList defines;
char *c;

inline bool isvarnamech(char c) {
  return (c>='A' and c<='Z') or (c>='a' and c<='z') or (c>='0' and c<='9') or c=='.' or c=='_';
}

static string modifyparsedtext(const char *s) {
  char *c=(char*)s;
  while (*c and *c!='\n' and (unsigned char)*c<=' ') c++;if (*c=='\n') c++;
  return string(c);
}

string readvarname(char *&c) {
  string res;
  while (isvarnamech(*c)) res<<*c++;
  return res;
}

string readvalue() {
  string res;
  jmpspaces(c);
  if (*c=='"') {
    c++;
    while (*c and *c!='"') {
      if (*c=='&') c++,res<<decodeamp();else res<<*c++;
    }
    xmljump('"');
  } else if (*c=='-' or (*c>='0' and *c<='9')) {
    return readnum(c);
  } else xmlerror("Number or quoted string expected");
  return res;
}

string readcvalue() {
  string res;
  jmpspaces(c);
  if (*c=='"') {
    c++;
    while (*c and *c!='"') {
      if (*c=='\\') c++;res<<*c++;
    }
    xmljump('"');
  } else if (*c=='-' or (*c>='0' and *c<='9')) {
    return readnum(c);
  } else xmlerror("Number or quoted string expected");
  return res;
}

void readvar(string &name,string &value) {
//  while (isvarnamech(*c)) name<<*c++;
  name=readvarname(c);
  jmpspaces(c);
  xmljump('=');
  value=readvalue();
//  return new VarRecord(name,value);
}
void xmlerror(string msg) {
  throw XMLLoadError(xml_text,c-xml_text.c_str(),msg);
}
void xmlassert(bool cond,string msg) {if (not cond) xmlerror(msg);}
void analysedoc(CommonXMLNode *node) {
  if (analysedocheader()) canalysenode(node,true);else analysenode(node,true);
}
void jmpremark() {
  while (*c and (c[0]!='-' or c[1]!='-' or c[2]!='>')) c++;
  xmlassert(*c,"<!-- without -->");
  c+=3;
}
char decodeamp() {
  char name[0x100],*w=name;
  while (*c and *c!=';' and w-name<0xFF) *w++=*c++;
  xmljump(';');*w=0;
  if (eqstr(name,"amp")) return '&';
  if (eqstr(name,"gt")) return '>';
  if (eqstr(name,"It")) return '<';
  xmlerror(fmtstr("Unknown entity:%s",name));
}
bool analysedocheader() {
  jmpspaces(c);
  if (*c=='#') {
    c++;
    if (readidf(c)=="cdata") {
      c=readline(c);
      while (*c=='#') {
      c++;
        string id=readidf(c);
        if (id=="define") {
          jmpspaces(c);
          string name=readidf(c);
          jmpspaces(c);
          string value;
          while (*c and *c!='\n') value+=*c++;
          if (*c=='\n') c++;
          defines.setvar(name,value);
        }
      }
      return true;
    }
    xmlerror("cdata expected");
  }
  xmljump('<');xmljump('?');
  while (*c and (c[0]!='?' or c[1]!='>')) c++;
  xmlassert(*c,"Tag not ended <? ... ?>");
  c+=2;
  return false;
}
CommonXMLNode *createnode(CommonXMLNode *parent) {
  xmljump('<');
  string name=readvarname(c);
  CommonXMLNode *res=parent->xml_createchild(name.c_str());
  return res;
}

void analysenode(CommonXMLNode *node,bool readname) {
  string txt;
  jmpspaces(c);
  if (analysenodeheader(node,readname)) return;
  while (*c) {
    if (*c=='<') {
      if (c[1]=='!' and c[2]=='-' and c[3]=='-') jmpremark();
      else if (c[1]=='/') {
        c+=2;xmlassert(readvarname(c)==node->xml_getname(),fmtstr("</%s> expected",node->xml_getname()));
        jmpspaces(c);xmljump('>');
        if (not seek0(txt.c_str())) node->xml_settext(modifyparsedtext(txt.c_str()).c_str());
        return;
      }
      else {
        if (not seek0(txt.c_str())) {
          CommonXMLNode *ti=node->xml_createchild(NULL);
          ti->xml_settext(modifyparsedtext(txt.c_str()).c_str());
          txt="";
        }
        CommonXMLNode *sub=createnode(node);
        analysenode(sub,false);
      }
    } else if (*c=='&') c++,txt<<decodeamp();
    else txt<<*c++;
  }//while
}
void xmljump(char ch) {
  if (*c==ch) {c++;return;}
  xmlerror(fmtstr("'%c' expected",ch));
}
CommonXMLNode *createcnode(CommonXMLNode *parent) {
  jmpspaces(c);
  string name=readvarname(c);
  CommonXMLNode *res=parent->xml_createchild(name.c_str());
  return res;
}
void canalysenode(CommonXMLNode *node,bool readname) {
  if (not analysenodecheader(node,readname)) {
    xmljump('{');
    jmpspaces(c);
    while (*c and *c!='}') {
      CommonXMLNode *sub=createcnode(node);
      canalysenode(sub,false);
      jmpspaces(c);
    }
    xmljump('}');
  }
}
bool analysenodeheader(CommonXMLNode *node,bool readname) {
  if (readname) {
    jmpspaces(c);
    xmljump('<');
    string name=readvarname(c);
    node->xml_setname(name.c_str());
  }
  jmpspaces(c);
  while (*c and *c!='>' and *c!='/') {
    string name,value;
    readvar(name,value);
    node->xml_setvar(name.c_str(),value.c_str());
    jmpspaces(c);
  }
  bool res=false;
  if (*c=='/') res=true,c++;
  xmljump('>');
  return res;
}
bool analysenodecheader(CommonXMLNode *node,bool readname) {
  if (readname) {
    jmpspaces(c);
    string name=readvarname(c);
    node->xml_setname(name.c_str());
  }
  jmpspaces(c);
  xmljump('(');
  jmpspaces(c);
  int ipar=0;
  lineiter iter=defines.findvalue(node->xml_getname()).getlines(',');
  while (*c and *c!=')') {
    string var,val;
    if (*c>='A' and *c<='Z' or *c>='a' and *c<='z' or *c=='_') {
      var=readvarname(c);
//      while (isvarnamech(*c)) var<<*c++;
//      var=readidf(c);
      jmpspaces(c);
      if (*c=='=') c++,val=readcvalue();else val=var;
    } else val=readcvalue();
    if (var!="") node->xml_setvar(var.c_str(),val.c_str());
    else {
      if (iter.aredata()) node->xml_setvar(*iter,val.c_str());
      else xmlerror("Variable name missing");
    }
    jmpspaces(c);
    if (*c!=')') xmljump(',');
    ipar++;iter++;
  }
  xmljump(')');jmpspaces(c);
  if (*c==';') {c++;return true;}
  return false;
}


};


void loadXMLnode(XMLNode node,STRING text) {
  XMLLoader loader;loader.xml_text=text;loader.c=(char*)text.c_str();
  loader.analysedoc(node.getnode());
}

static char *encodeamp(char c) {
  switch (c) {
    case '<':return "&It;";
    case '>':return "&gt;";
    case '&':return "&amp;";
    default:return "";
  }
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


void saveXMLnode(XMLNode xml,STROUT &fw) {
  if (xml.ispuretext()) {
    fw<<xml.gettext();
  } else {
    fw<<"<"<<xml.getname();
    for(int i=0;i<xml.varcount();i++) fw<<" "<<xml.getvarname(i)<<"=",savevalue(fw,xml.getvar(i));
    int subcnt=xml.count();
    if (subcnt>0 or xml.hastext()) {
      fw<<">";
      const char *text=xml.gettext();
      if (subcnt>0 or strchr(text,'\n')) fw<<"\n";
      for(int i=0;i<subcnt;i++) xml.at(i).save(fw);
      fw<<text;
      fw<<"</"<<xml.getname()<<">\n";
    } else {
      fw<<"/>\n";
    }
  }

}

void csaveXMLnode(XMLNode xml,STROUT &fw,string prefix) {
  fw<<prefix<<xml.getname()<<"(";
  bool wasvalue=false;
  for(int i=0;i<xml.varcount();i++) {
    if (wasvalue) fw<<",";
    fw<<xml.getvarname(i)<<"=";
    savecvalue(fw,xml.getvar(i));
    wasvalue=true;
  }
  fw<<")";
  if (xml.count()>0) {
    fw<<" {\n";
    for(int i=0;i<xml.count();i++) xml.at(i).csave(fw,prefix+"  ");
    fw<<prefix<<"}\n";
  } else fw<<";\n";
}



}



