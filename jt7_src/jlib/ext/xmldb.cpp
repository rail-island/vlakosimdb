#include <jlib.h>
#pragma hdrstop

#include "xmldb.h"

namespace jlib {

void XMLTableModel::getcols(TCntVec<aws::TableCol> &res) {
  for(int i=0;i<flds.count;i++) {
    aws::TableCol &n=res.add();
    strcpy(n.text,flds[i]->text);
    strcpy(n.id,flds[i]->name);
//    n.id=(int)flds[i];
  }
}

/*
static void checkxml(XMLNode xml) {
  xml.gettext().do_string_control();
  xml.getsub("title").do_string_control();
  for(int i=0;i<xml.count();i++) checkxml(xml[i]);
}
*/

string XMLTableModel::getcelltext(const char *fldid,int y) {
  XMLTableFld *fld=flds.searchvalue(fldid);
  if (!fld) return string("Error");
  // (XMLTableFld*)fldid;
//  if (flds.search(fld)<0) return "Error";
//  checkxml(xml);
  string res;
  if (fld->sub) res=xml[y].getsub(fld->name);
  else res=xml[y].svar(fld->name);
  return res;
}

struct CompareDef {
  XMLSort *sorter;
};

struct SortItem {
  XMLDataNode *xml;
  CompareDef *cmp;
};

static int xml_compare(const void *p_a,const void *p_b) {
  SortItem &a=*(SortItem*)p_a,&b=*(SortItem*)p_b;
  XMLNode xa=a.xml,xb=b.xml;
  TCntVec<SortCol> &index=a.cmp->sorter->index;
  for(int i=0;i<index.count;i++) {
    string idxname=index[i].name;
    string sa=xa.getsub(idxname),sb=xb.getsub(idxname);
    int res=jsystem->comparetext(sa.c_str(),sb.c_str());
    if (res) return res;
  }
  return 0;
//  return jsystem->comparetext(xa.getsub("title"),xb.getsub("title"));
}

XMLNode XMLSort::work() {
  TCntVec<SortItem> sitems;
  CompareDef cmpdef;
  cmpdef.sorter=this;
  for(int i=0;i<xml.count();i++) {
    SortItem &si=sitems.add();
    si.xml=xml[i].getnode();
    si.cmp=&cmpdef;
  }
  qsort(sitems.buf,sitems.count,sizeof(SortItem),xml_compare);
  XMLNode res=XMLNode::create();
  for(int i=0;i<sitems.count;i++) res.add_existing(sitems[i].xml);
  return res;
}

}



