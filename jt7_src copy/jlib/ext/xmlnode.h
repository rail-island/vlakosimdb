#ifndef xmlnodeH
#define xmlnodeH

#include <com/rcobj.h>

#define HTF_NOENDS 1
#define HTF_NOROOT 2
#define HTF_NOVARS 2

namespace jlib {

struct XMLDataNode {
  void *data;
  XMLDataNode *parent;
  VarList vars;
  string text;
  string name;
  TRCArray<XMLDataNode> childs;
  int refcnt;

  XMLDataNode() {data=NULL;parent=NULL;refcnt=0;}
  virtual ~XMLDataNode();

  void addref() {refcnt++;}
  void release() {if (!--refcnt) delete this;}

  void assign(XMLDataNode *src);
  XMLDataNode *insertchild(int index);
};

/**
XMLPath:absolutni:zacina /
@promenna:promenna
#:text
/:oddelovac uzlu
*/
struct XMLPath {
  string path;
  const char *c_str() {return path.c_str();}
  operator string () {return path;}
  operator STRING () {return path;}
  XMLPath(STRING apath) {path=apath;}
  XMLPath() {}
  bool isabsolute() {return path[0]=='/';}
  bool isrelative() {return !isabsolute();}
  bool istext() {return path.length()>0 && path[path.length()-1]=='@';}
  bool isattribute();
  bool isvalue() {return isattribute() || istext();}
  string attributename();
  bool issub() {return !istext() && !isvalue();}
  ///zda se tyka poduzle
  bool isdeep();
  ///if (isdeep()), pak vrati jmeno ditete
  string childname();
  ///cesta od ditete
  XMLPath childpath();
  XMLPath parent();
  XMLPath child(string name);
  XMLPath attribute(string name) {return XMLPath(path+"@"+name);}
  XMLPath text() {return XMLPath(path+"#");}
};

XMLDataNode *getemptyXMLnode();

struct XMLLoadError:CommonParseError {
};

/*
struct XMLLoadError {
  string text;
  int pos;
  string msg;
  XMLLoadError(string text,int pos,string msg):text(text),pos(pos),msg(msg) {}
};
*/

struct XMLNode {
  XMLNode(XMLDataNode *anode) {node=anode;if (node) node->addref();}
  ~XMLNode() {if (node) node->release();node=NULL;}
  XMLNode() {node=NULL;}
  XMLNode(const XMLNode &src) {if (src.node) src.node->addref();node=src.node;}
  void operator =(const XMLNode &src) {if (src.node) src.node->addref();if (node) node->release();node=src.node;}

  void loadfromtext(string text);
  void loadfromfile(STRING file);
  void loadfromhtf(string text,int flags);
  XMLDataNode *getnode() {return node;}
  void setnode(XMLDataNode *anode) {node=anode;}
  void save(STROUT &fw);
//  void csave(STROUT &fw,string prefix);
  void savetofile(STRING file);
  void savetotext(STROUT &fw) {fw<<"<?xml version=\"1.0\" encoding=\"Windows-1250\"?>\n";save(fw);}
  void savetohtf(STROUT &fw,int flags);
//  void csavetofile(STRING file) {FILESTROUT fso(file);csavetotext(fso);}
//  void csavetotext(STROUT &fw) {fw<<"#cdata\n";csave(fw,"");}

  XMLNode at(int index) {return XMLNode(node->childs[index]);}
  XMLNode operator [](int index) {return at(index);}
  XMLNode operator [](string name) {return at(name);}
  XMLNode at(string name);

  void setvar(string name,string value) {node->vars.setvar(name,value);}
  void setvar(string name,double value) {node->vars.setvar(name,value);}
  void setvar(string name,int value) {node->vars.setvar(name,value);}
  void setvar(string name,float value) {node->vars.setvar(name,value);}
  void setvar(string name,bool value) {node->vars.setvar(name,value);}
  void savevars(VarList &vars);
  void loadvars(VarList &vars);
  bool valid() {return node;} 

//  bool ispuretext() {return *node->xml_getname()==0;}
  string gettext() {return node->text;}
  string getname() {return node->name;}
  XMLNode insert(int index) {return node->insertchild(index);}
  void setname(string name) {node->name=name;}
  void clear() {node->childs.clear();node->vars.clear();}
  void clearvars() {node->vars.clear();}
  void clearsubs() {node->childs.clear();}
  bool hasname(string name) {return node->name==name;}
  int count() {return node->childs.count;}
  string getvar(string name) {return node->vars.findvalue(name);}
  string getvar(int index) {return node->vars[index]->value;}
  string getvarname(int index) {return node->vars[index]->name;}
  void erasevar(int index) {node->vars.erase(index);}
  void erasevar(string name) {node->vars.erasevar(name);}
  int varcount() {return node->vars.count;}
  bool isvar(string name) {for(int i=0;i<varcount();i++) if (getvarname(i)==name) return true;return false;}
  bool validvar(string name) {return getvar(name)!="";}
  XMLNode forcesub(string name);
  XMLNode add(string name) {XMLNode res=insert(-1);res.setname(name);return res;}
  XMLNode add() {return insert(-1);}
//  const char *gettext() {return node->xml_gettext();}
  void settext(string text) {node->text=text;}

  bool remove(XMLNode child) {return node->childs.remove(child.getnode());}
  void erasesub(int index) {node->childs.erase(index);}
//  void replacesub(int index,XMLNode xml) {node->xml_replacenode(index,xml.getnode());}
  bool erasesub(string name) {int idx=searchsub(name);if (idx>=0) erasesub(idx);return idx>=0;}
  void assign(XMLNode src);
  int searchsub(string name);
  int searchvar(string name);
  bool hastext() {return node->text!="";}

  int ivar(string name) {return atoi(cvar(name));}
  double fvar(string name) {return atof(cvar(name));}
  const char *cvar(string name) {return getvar(name).c_str();}
  string svar(string name) {return getvar(name);}

//  void getvars(VarList &vars);
//  void setvars(VarList &vars);

  void setsub(string name,string value) {forcesub(name).settext(value);}
  string getsub(string name) {return at(name).gettext();}

//  bool isproxy() {return node->xml_isproxy();}
//  void addproxy(XMLNode xml) {node->xml_addproxynode(xml.node);}

//  void addtextnode(string text) {node->xml_addtextnode(text);}
//  void add_existing(XMLNode xml) {node->xml_addexisting(xml.node);}

  static XMLNode create();

//  XMLNode copy1();

  operator XMLDataNode *() {return node;}
  string getvalue(XMLPath path);
  bool exists(XMLPath path);
  void setvalue(XMLPath path,string value);
  XMLNode getat(XMLPath path);
  XMLNode parent() {return XMLNode(node->parent);}
  XMLNode getchild(string name) {if (name=="..") return parent();return at(name);}
  XMLNode forcechild(string name) {if (name=="..") return parent();return forcesub(name);}
  bool equalto(XMLNode xml);
  bool weekequalto(XMLNode xml);
protected:
  XMLDataNode *node;
//  CommonXMLNode *get_ref() {return node;}
//  void set_ref(CommonXMLNode *ref) {node=ref;}
};


//typedef TRefBase<CommonXMLNode,XMLNode> XMLRef;
/*
struct XMLRef:TRefBase<CommonXMLNode,XMLNode> {
  XMLRef() {}
  XMLRef(XMLNode node) {*this=node.getnode();}
  XMLRef(CommonXMLNode *node) {*this=node;}
};
*/


struct MainXMLNode:XMLNode {
  MainXMLNode() {XMLNode &x=*this;x=XMLNode::create();}
};

//#if (sizeof(XMLNode)!=sizeof(void*))
//#if (sizeof(XMLNode)!=8)
//#error sizeof(XMLNode)!=sizeof(void*)
//#endif

}

#endif
