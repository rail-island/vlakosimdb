#ifndef stdxmlnodeH
#define stdxmlnodeH

#include "xmlnode.h"
#include <com/rcobj.h>

namespace jlib {

struct StdXMLNode:AbstractXMLNode {
//  IObjArray<AbstractXMLNode> childs;
  TRCArray<AbstractXMLNode> childs;
  VarList vars;
  string text;
  string name;

  int xml_getsubcount() {return childs.count;}
  AbstractXMLNode *xml_nodeat(int index) {return childs[index];}
  void xml_clearnodes() {childs.clear();}
  bool xml_erasenode(AbstractXMLNode *child) {return childs.remove(child);}
  void xml_setvar(const char *name,const char *value);
  const char *xml_getvar(const char *name) {return vars.findvar(name);}
  void xml_varsclear() {vars.clear();}

  void xml_settext(const char *atext) {text=atext;}
  const char *xml_gettext() {return text.c_str();}

  AbstractXMLNode *xml_createchild(const char *name) {
    StdXMLNode *res=new StdXMLNode();
    if (name) res->name=name;
    childs.add(res);
    return res;
  }
  const char *xml_getname() {return name.c_str();}
  virtual void xml_setname(const char *aname) {name=aname;}

  int xml_varcount() {return vars.count;}
  const char *xml_getvarat(int index) {return vars[index]->value.c_str();}
  const char *xml_getvarnameat(int index) {return vars[index]->name.c_str();}

  void xml_addproxynode(AbstractXMLNode *link) {childs.add(new ProxyXMLNode(link));}

  void xml_addexisting(AbstractXMLNode *node) {childs.add(node);}
private:
  int refcnt;
protected:
  void addref() {refcnt++;}
  void release() {if (!--refcnt) delete this;}
};

struct MainXMLNode:XMLNode {
  MainXMLNode():XMLNode(new StdXMLNode()) {}
  ~MainXMLNode() {delete node;}
};


}

#endif
