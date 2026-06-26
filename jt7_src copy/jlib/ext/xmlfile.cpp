#include <jlib.h>
#pragma hdrstop

#include "xmlfile.h"
#include <gram/gmanal.h>

namespace jlib {
using namespace gram;

void writeentity(STROUT &fw,char c) {
  switch (c) {
    case '<':fw<<"&lt;";break;
    case '>':fw<<"&gt;";break;
    case '\"':fw<<"&quot;";break;
    case '\'':fw<<"&apos;";break;
    case '&':fw<<"&amp;";break;
  }
}

void saveXMLvalue(STROUT &fw,string value) {
  Analyser a(value.c_str());
  while (*a) {
    if (a.is_xmlspecchar()) writeentity(fw,*a);else fw<<*a;
    a++;
  }
}
void saveHTMLvalue(STROUT &fw,string value) {
  Analyser a(value.c_str());
  while (*a) {
    if (a.is_xmlspecchar() && *a!='\'') writeentity(fw,*a);else fw<<*a;
    a++;
  }
}
string xmlencode(string plaintext) {
  MEMSTROUT mso;
  saveXMLvalue(mso,plaintext);
  return mso.str();
}
string htmlencode(string plaintext) {
  MEMSTROUT mso;
  saveHTMLvalue(mso,plaintext);
  return mso.str();
}
void decodeentity(Analyser &act,STROUT &value) {
  if (act.begjmp("&gt;")) value<<'>';
  else if (act.begjmp("&lt;")) value<<'<';
  else if (act.begjmp("&quot;")) value<<'"';
  else if (act.begjmp("&apos;")) value<<'\'';
  else if (act.begjmp("&amp;")) value<<'&';
  else act.error("Unknown entity");
}
string xmldecode(string xmltext) {
  Analyser a(xmltext.c_str());
  MEMSTROUT res;
  while (*a) {
    if (*a=='&') decodeentity(a,res);else res<<*a,a++;
  }
  return res.str();
}
string htmldecode(string htmltext) {
  return xmldecode(htmltext);
}


void saveXMLnode(XMLNode xml,STROUT &fw) {
  string name=xml.getname();
  bool isname=name!="";
  if (isname) {
    fw<<"<"<<xml.getname();
    for(int i=0;i<xml.varcount();i++) {
      fw<<" "<<xml.getvarname(i)<<"=\"";
      saveXMLvalue(fw,xml.getvar(i));
      fw<<"\"";
    }
    if (xml.count()>0 || xml.hastext()) {
      fw<<">";
      fw<<"\n";
      for(int i=0;i<xml.count();i++) {
        saveXMLnode(xml[i],fw);
      }
      if (xml.gettext()!="") {
        saveXMLvalue(fw,xml.gettext());
        fw<<"\n";
      }
      fw<<"</"<<xml.getname()<<">\n";
    } else {
      fw<<"/>\n";
    }
  } else {
    saveXMLvalue(fw,xml.gettext());
    fw<<"\n";
  }
/*
  if (xml.istext()) {
    saveXMLvalue(fw,xml.gettext());
    fw<<"\n";
  } else {
    fw<<"<"<<xml.getname();
    for(int i=0;i<xml.varcount();i++) {
      fw<<" "<<xml.getvarname(i)<<"=\"";
      saveXMLvalue(fw,xml.getvar(i));
      fw<<"\"";
    }
    if (xml.count()>0) {
      fw<<">";
      fw<<"\n";
      for(int i=0;i<xml.count();i++) {
        saveXMLnode(xml[i],fw);
      }
      fw<<"</"<<xml.getname()<<">\n";
    } else {
      fw<<"/>\n";
    }
  }
*/
}

struct XMLLoader {
  string xml_text;
  Analyser act;

  string readxmlidf() {
    bstring idf;
    if (!act.is_xmlidf()) act.error("Identifier expected");
    while (act.is_xmlidf()) idf<<*act,act++;
    return idf;
  }
  void readentity(bstring &value) {
    if (act.begjmp("&gt;")) value<<'>';
    else if (act.begjmp("&lt;")) value<<'<';
    else if (act.begjmp("&quot;")) value<<'"';
    else if (act.begjmp("&apos;")) value<<'\'';
    else if (act.begjmp("&amp;")) value<<'&';
    else act.error("Unknown entity");
  }
  string readxmlvalue() {
    bstring value;
    char beg=0;
    if (*act=='\'') beg='\'';
    if (*act=='\"') beg='\"';
    act.f_char(beg);
    while (*act && *act!=beg) {
      if (*act=='&') readentity(value);
      else {
        value<<*act;act++;
      }
    }
    act.f_char(beg);
    return value;
  }
  void _jmpxmlremark() {
    while (*act) {
      if (*act=='-') {
        if (act.begjmp("-->")) return;
      }
      act++;
    }
    //neukoncena poznamka
  }
  void jmpxmlwhite() {
    act.jw();
    while (act.begjmp("<!--")) _jmpxmlremark(),act.jw();
  }
  void addtext(XMLNode node,string text,bool endtag,bool wasflushed) {
    Analyser a(text.c_str());
    while (a.is_white()) a++;
    if (*a) {//je tam nejaky text
      if (!endtag || wasflushed) {
        XMLNode t=node.add();
        t.settext(text);
      } else {
        node.settext(text);
      }
    }
  }
  void flushtext(XMLNode node,bstring &text,bool endtag,bool wasflushed) {
    string s=text;
    addtext(node,s.gettrimed(),endtag,wasflushed);
    text.clear();
  }
  void loadhdr() {
    act.jsp();
    act.f_char('<');
    act.f_char('?');
    act.jumpafter("?>");
  }
  void loaddoc(XMLNode node) {
    try {
      loadhdr();
      jmpxmlwhite();
      load(node);
    } catch (AnalError &e) {
      XMLLoadError err;
      err.init(xml_text,e.msg,e.pos-xml_text.c_str());
      throw err;
    }
  }
  void load(XMLNode node) {
    act.f_char('<');
    node.setname(readxmlidf());
    while (act.jw(),*act && *act!='>' && *act!='/') {
      string name=readxmlidf();
      act.jw();act.f_char('=');act.jw();
      string value=readxmlvalue();
      node.setvar(name,value);
    }
    if (*act=='/') {//element bez vnitrku
      act++;act.f_char('>');return;
    }
    act.f_char('>');
    bstring text;
    bool wasflushed=false;
    for(;;) {
      if (act.is_xmlloadspec()) {
        switch (*act) {
          case '<': {
           if (act.begjmp("<!--")) {_jmpxmlremark();break;}
           if (act[1]=='/') {//koncovy tag elementu
              flushtext(node,text,true,wasflushed);
              act+=2;
              if (readxmlidf()!=node.getname()) act.error(fmtstr("</%s> expected",node.getname()));

              act.jw();
              act.f_char('>');
              return;
            } else {//podelement
              flushtext(node,text,false,wasflushed);
              XMLNode child=node.add();
              load(child);
              wasflushed=true;
            }
          } break;
          case '\0':act.error(fmtstr("</%s> expected",node.getname()));break;
          case '>':act.error("Unexpected '>'");break;
          case '&':readentity(text);break;
        }
      } else text<<*act,act++;
    }
  }
};

void loadXMLnode(XMLNode node,string text) {
  XMLLoader loader;
  loader.xml_text=text;
  Analyser a(text.c_str());
  loader.act=a;
  loader.loaddoc(node);
}


}


