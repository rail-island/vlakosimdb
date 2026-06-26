#ifndef xmlrepH
#define xmlrepH

//#include "stdxmlnode.h"
#include "xmlnode.h"

namespace jlib {

struct XMLRepositary {
  struct Item {
    TString<0x20> name;
    XMLDataNode *node;
  };
  XMLRepositary(XMLDataNode *amain=NULL);
  void recload(STRING dir,STRING mask,STRING repname="rep.name");//loads all files from dir and subdirs,ext is eg. *
  void loadfile(STRING file,STRING repname="rep.name");
//  void searchincludes();
  void loadfiles(STRING dir,STRING mask,STRING repname="rep.name");
  XMLDataNode *search(STRING name) {if (Item *it=searchitem(name)) return it->node;return NULL;}
  XMLNode find(STRING name) {XMLDataNode *res=search(name);xassert(res);return XMLNode(res);}
private:
  void added(XMLNode xml,STRING repname);
  TCntVec<Item> items;
  Item *searchitem(STRING name);
  XMLDataNode *main;
//  CommonXMLNode *replaceincludes(XMLNode xml);
};

}

#endif
