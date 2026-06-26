#include <jlib.h>
#pragma hdrstop

#include "xmlnode.h"
#include "xmlfile.h"
#include <gram/gmanal.h>
//#include "xmlload.h"
//#include "stdxmlnode.h"

namespace jlib {

XMLDataNode *XMLDataNode::insertchild(int index) {
  XMLDataNode *res=new XMLDataNode();
  res->parent=this;
  if (index<0 || index>=childs.count) childs.add(res);else childs.insert(index,res);
  return res;
}

XMLDataNode::~XMLDataNode() {
  for(int i=0;i<childs.count;i++) childs[i]->parent=NULL;
}

void XMLNode::savevars(VarList &vars) {
  vars.assign(node->vars);
}
void XMLNode::loadvars(VarList &vars) {
  node->vars.assign(vars);
}

/*
string XMLNode::gettext() {
  if (istext()) return node->xml_gettext();
//  MEMSTROUT res;
  string res;
  for(int i=0;i<count();i++) {
    XMLNode child=at(i);
    if (child.istext()) res<<child.gettext();
  }
  return res;
//  return res.str();
}
*/
/*
void XMLNode::settext(string text) {
  if (istext()) {
    node->xml_settext(text);
    return;
  }
  node->xml_clearnodes();
  node->xml_addtextnode(text);
}
*/



XMLNode XMLNode::create() {return XMLNode(new XMLDataNode());}

/*
XMLNode XMLNode::copy1() {
  XMLNode res=std();
  res.setname(getname());
  res.node->xml_settext(node->xml_gettext());
  for(int i=0;i<varcount();i++) res.setvar(getvarname(i),getvar(i));
  for(int i=0;i<count();i++) res.add_existing(at(i));
  return res;
}
*/

void XMLDataNode::assign(XMLDataNode *src) {
  text=src->text;
  name=src->name;
  vars.assign(src->vars);
  childs.clear();
  for(int i=0;i<src->childs.count;i++) {
    XMLDataNode *child=insertchild(-1);
    child->assign(src->childs[i]);
  }
}

void XMLNode::assign(XMLNode src) {
  node->assign(src.getnode());
//  clear();
//  setname(src.getname());
//  settext(src.gettext());
//  node->xml_settext(src.node->xml_gettext());

//  for(int i=0;i<src.varcount();i++) setvar(src.getvarname(i),src.getvar(i));
//  for(int i=0;i<src.count();i++) {
//    if (src[i].istext()) addtextnode(src[i].gettext());
//    else add().assign(src[i]);
//  }
}

void XMLNode::loadfromfile(STRING file) {
  FILE *fr=fopen(file,"rb");
  if (!fr) throw JError(string("XML load error:file not found:")+string(file));
  fclose(fr);
  string text=loadtext(file);
  sFile f(file);
  if (f.testext("htf")) {
    loadfromhtf(text,0);
  } else {
    loadfromtext(text);
  }
}

void XMLNode::loadfromtext(string text) {
  loadXMLnode(*this,text);
}

static XMLDataNode *emptynode=NULL;

struct EmptyXMLNode:XMLDataNode {
  ~EmptyXMLNode() {emptynode=NULL;}
};
//typedef XMLDataNode EmptyXMLNode;
/*
struct EmptyXMLNode:CommonXMLNode {
  void addref() {}
  void release() {}
  CommonXMLNode *xml_createchild(string name,int index) {
    throw JError("Empty XML node cannot have child");
  }
};
*/

XMLDataNode *getemptyXMLnode() {
  if (!emptynode) emptynode=new EmptyXMLNode();
  return emptynode;
}

int XMLNode::searchsub(string name) {
  int cnt=count();
  for(int i=0;i<cnt;i++) if (at(i).hasname(name)) return i;
  return -1;
}
int XMLNode::searchvar(string name) {
  int cnt=varcount();
  for(int i=0;i<cnt;i++) if (getvarname(i)==name) return i;
  return -1;
}

XMLNode XMLNode::at(string name) {
  int res=searchsub(name);
  if (res<0) return XMLNode(getemptyXMLnode());
  return at(res);
}
XMLNode XMLNode::forcesub(string name) {
  int res=searchsub(name);
  if (res<0) return add(name);
  return at(res);
}

/*
void XMLNode::getvars(VarList &vars) {
  int vc=varcount();
  vars.clear();
  for(int i=0;i<vc;i++) vars._addvar(getvarname(i),getvar(i));
}
void XMLNode::setvars(VarList &vars) {

  node->xml_varsclear();
  for(int i=0;i<vars.count;i++) setvar(vars[i]->name,vars[i]->value);
}
*/

void XMLNode::savetohtf(STROUT &fw,int flags) {
  if (! (flags&HTF_NOROOT) ) {
    fw<<"["<<getname()<<"]\n";
  }
  if (! (flags&HTF_NOVARS) ) {
    for(int i=0;i<varcount();i++) {
      fw<<getvarname(i);
      string value=getvar(i);
      if (value.search('\n')<0) {
        fw<<"="<<value<<"\n";
      } else {
        fw<<":\n"<<value<<"#END#\n";
      }
    }
  }
  for(int i=0;i<count();i++) {
    at(i).savetohtf(fw,flags&(~HTF_NOROOT)&(~HTF_NOVARS));
  }
  if (! (flags&HTF_NOROOT) ) {
    fw<<"[/"<<getname()<<"]\n";
  }
}

void loadhtfxmlnode(XMLNode xml,gram::Analyser &a,int flags) {
  a.jw();
  if (! (flags&HTF_NOROOT) ) {
    a.f_char('[');
    xml.setname(a.readto("]",true));
    a.roll();
  }
  while (a.jw(),*a) {
    if (*a=='[') {
      if (a[1]=='/') {//end
        a+=2;
        string endname=a.readto("]",true);
        if (endname!=xml.getname()) {
          a.error(endname+" expected");
        }
        a.roll();
        return;
      } else {//child node
        loadhtfxmlnode(xml.add(),a,flags&(~HTF_NOROOT)&(~HTF_NOVARS));
      }
    } else {//variable
      string name=a.readto(":=",false);
      string value;
      if (*a=='=') {
        a++;
        value=a.readto("\r\n",false);
        a.roll();
      } else {
        a.roll();
        bstring v;
        while (*a && !a.begins("#END#")) v<<*a,a++;
        a.begjmp("#END#");
        a.roll();
        value=v;
      }
      xml.setvar(name,value);
    }
  }
}

void XMLNode::loadfromhtf(string text,int flags) {
  gram::Analyser a(text.c_str());
  try {
    loadhtfxmlnode(*this,a,flags);
  } catch (gram::AnalError &e) {
    XMLLoadError err;
    err.init(text,e.msg,e.pos-text.c_str());
    throw err;
  }

}

void XMLNode::savetofile(STRING file) {
  sFile f=file.c_str();
  if (f.testext("htf")) {
    FILESTROUT fso(file);
    savetohtf(fso,0);
  } else {
    FILESTROUT fso(file);
    savetotext(fso);
  }
}

void XMLNode::save(STROUT &fw) {saveXMLnode(*this,fw);}
//void XMLNode::csave(STROUT &fw,string prefix) {csaveXMLnode(*this,fw,prefix);}

bool XMLPath::isattribute() {
  gram::Analyser a0(path.c_str()),a=a0;
  while (*a) a++;if (a>a0) a--;
  while (a>a0 && a.is_xmlidf()) a--;
  return *a=='@';
}
string XMLPath::attributename() {
  gram::Analyser a0(path.c_str()),a=a0;
  while (*a) a++;if (a>a0) a--;
  while (a>a0 && a.is_xmlidf()) a--;
  if (*a=='@') return XMLPath(a.act+1);
  return XMLPath("");
}
bool XMLPath::isdeep() {
  if (path=="/" || path=="") return false;
  gram::Analyser a(path.c_str());
  if (*a=='/') a++;
  if (a.is_xmlidf()) return true;
  return false;
}
string XMLPath::childname() {
  gram::Analyser a(path.c_str());
  if (*a=='/') a++;
  return a.readto("/",false);
}
XMLPath XMLPath::childpath() {
  gram::Analyser a(path.c_str());
  if (*a=='/') a++;
  a.jumpafter("/");
  return XMLPath(a.act);
}
XMLPath XMLPath::child(string name) {
  if (path=="/") return XMLPath(path+name);
  return XMLPath(path+"/"+name);
}
XMLPath XMLPath::parent() {
  const char *c=strrchr(path.c_str(),'/');
  return XMLPath(path.copy(0,c-path.c_str()));
}

bool XMLNode::exists(XMLPath path) {
  if (path.isdeep()) {
    string name=path.childname();
    int index=searchsub(name);
    if (index<0) return false;
    return at(index).exists(path.childpath());
  } else {
    if (path.istext()) return hastext();
    if (path.isattribute()) return isvar(path.attributename());
    return true;
  }
}

string XMLNode::getvalue(XMLPath path) {
  if (path.isdeep()) {
    return getchild(path.childname()).getvalue(path);
  } else {
    if (path.istext()) return gettext();
    if (path.isattribute()) return svar(path.attributename());
    return "";
  }
}
void XMLNode::setvalue(XMLPath path,string value) {
  if (path.isdeep()) {
    string child=path.childname();
    forcechild(child).setvalue(path,value);
  } else {
    if (path.istext()) settext(value);
    if (path.isattribute()) setvar(path.attributename(),value);
  }
}
XMLNode XMLNode::getat(XMLPath path) {
  if (path.isdeep()) return forcechild(path.childname()).getat(path.childpath());
  return *this;
}
bool XMLNode::equalto(XMLNode xml) {
  if (getname()!=xml.getname()) return false;
  if (varcount()!=xml.varcount()) return false;
  for(int i=0;i<varcount();i++) if (getvarname(i)!=xml.getvarname(i) || getvar(i)!=xml.getvar(i)) return false;
  if (count()!=xml.count()) return false;
  for(int i=0;i<count();i++) if (!at(i).equalto(xml.at(i))) return false;
  return true;
}

bool XMLNode::weekequalto(XMLNode xml) {
  if (getname()!=xml.getname()) return false;
  if (varcount()!=xml.varcount()) return false;
  if (count()!=xml.count()) return false;
  int vc=varcount();
  for(int i=0;i<vc;i++) {
    string name=getvarname(i);
    if (!xml.isvar(name)) return false;
    if (getvar(i)!=xml.svar(name)) return false;
  }

  int cc=count();
  for(int i=0;i<cc;i++) {
    string name=at(i).getname();
    int idx2=xml.searchsub(name);
    if (idx2<0) return false;
    if (!at(i).weekequalto(xml.at(idx2))) return false;
  }

  return true;
}


}


