#ifndef XMLdocH
#define XMLdocH

namespace jlib {

struct XMLDocument;

struct XMLError {
  string msg;
  string errline;
  int line,col;
  int pos;
  XMLError(string amsg,char *act,XMLDocument *xml);
  void show() {if (jsystem) jsystem->showerror(msg+"\n"+errline);else fprintf(stderr,"%s\n%s\n",msg.c_str(),errline.c_str());} 
};

struct XMLItem {
  XMLDocument *doc;
  struct IVARS {
    XMLItem *parent;
    int operator [](const char *name) {return atoi(parent->vars.findvalue(name).c_str());}
    int operator [](string name) {return atoi(parent->vars.findvalue(name).c_str());}
    IVARS(XMLItem *aparent) {parent=aparent;}
  } ivars;
  struct FVARS {
    XMLItem *parent;
    double operator [](const char *name) {return atof(parent->vars.findvalue(name).c_str());}
    double operator [](string name) {return atof(parent->vars.findvalue(name).c_str());}
    FVARS(XMLItem *aparent) {parent=aparent;}
  } fvars;
  struct CVARS {
    XMLItem *parent;
    const char * operator [](const char *name) {return parent->vars.findvalue(name).c_str();}
    const char * operator [](string name) {return parent->vars.findvalue(name).c_str();}
    CVARS(XMLItem *aparent) {parent=aparent;}
  } cvars;
  struct SVARS {
    XMLItem *parent;
    string operator [](const char *name) {return parent->vars.findvalue(name);}
    string operator [](string name) {return parent->vars.findvalue(name);}
    SVARS(XMLItem *aparent) {parent=aparent;}
  } svars;

  IObjArray<XMLItem> subitems;
  string name;
  string text;
  VarList vars;
  bool match(const char *aname) {return name==aname;}
  void analyse(char *&c);
  void canalyse(char *&c);
  bool analyseheader(char *&c);//vrati,zda me prazdne telo(<tag/>)
  bool analysecheader(char *&c);//vrati,zda me prazdne telo(konci strednikem)
  void save(STROUT &fw);
  void csave(STROUT &fw,string prefix);
  bool ispuretext() {return name=="";}
  bool hastext() {return text!="";}
  bool hasitem(const char *name) {return subitems.searchtype(name)>=0;}
  string gettext() {return text;}
  void settext(string value) {text=value;}

  bool isvar(const char *name) {
    return vars.searchvalue(name);
  }
  void setvar(const char *name,string value) {vars.setvar(name,value);}
  void setvar(const char *name,double value) {vars.setvar(name,value);}
  void setvar(const char *name,int value) {vars.setvar(name,value);}
  void setvar(const char *name,float value) {vars.setvar(name,value);}
  void setvar(const char *name,bool value) {setvar(name,(int)value);}
  int count() {return subitems.count;}
  XMLItem &operator [](int index) {return *subitems[index];}
  XMLItem &operator [](const char *name);
  XMLItem &operator [](string name) {return operator [](name.c_str());}
  XMLItem &add() {XMLItem *n=new XMLItem(doc);subitems.add(n);return *n;}
  XMLItem(XMLDocument *adoc=NULL):ivars(this),cvars(this),svars(this),fvars(this) {doc=adoc;}
  void savetofile();//ulozi cely XML dokument, jehoz je seoucasti
  void clear(string newname) {name=newname;clear();}
  void clear() {subitems.clear();vars.clear();}
  bool ismain();
  void assign(XMLItem &xml);
  XMLItem &forcesub(const char *name);//pokud existuj,vrati ji, jinak ji vyrabi a vrati
  XMLItem &forcesub(string name) {return forcesub(name.c_str());}
  void setsub(string name,string value) {forcesub(name).settext(value);}
  string getsub(string name) {return operator[](name).gettext();}
  void loadfromtext(string text);
  void loadfromfile(string file) {string text=loadtext(file.c_str());loadfromtext(text.c_str());}
};


struct XMLDocument {
  XMLItem main;string file;bool catcherrors;
  XMLItem empty;

  XMLDocument():main(this),empty(this) {analysing=NULL;catcherrors=true;};
  bool analyseheader(char *&c);//vrati true,pokud je to cheader
  void analyse(const char *s);
  bool _load(string file);
  bool load(string file);
  void save(STROUT &fw) {fw<<"<?xml version=\"1.0\" encoding=\"Windows-1250\"?>\n";main.save(fw);}
  void save(const char *file) {FILESTROUT fso(file);save(fso);}
  void save(string file) {save(file.c_str());}

  void savecheader(STROUT &fw);
  void csave(STROUT &fw) {savecheader(fw);main.csave(fw,"");}
  void csave(const char *file) {FILESTROUT fso(file);csave(fso);}
  //csave:trochu jiny format ukladani
  VarList defines;//definice standartnich argumentu-uklada a nacita se jen pri csave

  void savetofile() {if (file!="") save(file);}
private:
  char *analysing;
  friend class XMLItem;
  friend class XMLError;
};

}

//---------------------------------------------------------------------------
#endif
