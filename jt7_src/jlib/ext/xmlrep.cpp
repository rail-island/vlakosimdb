#include <jlib.h>
#pragma hdrstop

#include "xmlrep.h"

namespace jlib {

/*
void XMLRepositary::searchincludes() {
  replaceincludes(XMLNode(main));
}

CommonXMLNode *XMLRepositary::replaceincludes(XMLNode xml) {
  if (xml.isproxy()) return NULL;
  if (xml.getname()=="rep.include") {
    if (CommonXMLNode *node=search(xml.cvar("name"))) return node;
  }
  retry:
  for(int i=0;i<xml.count();i++) {
    CommonXMLNode *replacing=replaceincludes(xml[i]);
    if (replacing) {
      xml.replacesub(i,XMLNode(replacing));
//      xml.erasesub(i);
//      xml.addproxy(XMLNode(replacing));
      goto retry;
    }
  }
  return NULL;
}
*/

XMLRepositary::XMLRepositary(XMLDataNode *amain) {
  main=amain;
  if (!main) main=new XMLDataNode();
}

void XMLRepositary::loadfiles(STRING dir,STRING mask,STRING repname) {
  string files=jsystem->listfiles(sFile(dir.c_str())/mask.c_str());
  for(lineiter it=files.getlines(';');*it;it++) loadfile(sFile(dir.c_str())/ *it ,repname);
}

void XMLRepositary::loadfile(STRING file,STRING repname) {
  XMLDataNode *newnode=main->insertchild(-1);
  XMLNode xml(newnode);
  try {
    xml.loadfromfile(file);
    added(xml,repname);
  } catch (...) {
    xml.remove(newnode);
//    main->xml_erasenode(newnode);
  }
}

void XMLRepositary::added(XMLNode xml,STRING repname) {
  if (xml.isvar(repname) && *xml.cvar(repname)) {
    Item &it=items.add();
    it.name=xml.cvar(repname);
    it.node=xml.getnode();
  }
  for(int i=0;i<xml.count();i++) added(xml[i],repname);
}

void XMLRepositary::recload(STRING dir,STRING mask,STRING repname) {
  string dirs=jsystem->listdirs(sFile(dir.c_str())/"*.*");
  loadfiles(dir,mask,repname);
  for(lineiter it=dirs.getlines(';');*it;it++) {
    STRING act=*it;
    if (act!="." && act!="..") recload(sFile(dir.c_str())/act.c_str(),mask);
  }
//  searchincludes();
}

XMLRepositary::Item *XMLRepositary::searchitem(STRING name) {
  for(int i=0;i<items.count;i++) if (items[i].name==name) return &items[i];
  return NULL;
}


}



