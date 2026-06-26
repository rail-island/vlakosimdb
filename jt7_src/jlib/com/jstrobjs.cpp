#include <jlib.h>
#pragma hdrstop

#include "jstrobjs.h"
namespace jlib {

void Strings::fromtext(string text,char sep) {
  clear();
  for(lineiter it=text.getlines(sep);*it;it++) add(*it);
}

string Strings::gettext(int beg,int end,char sep) {
  int size=datasize(beg,end)+end-beg;
  string res;
  char *w=res.reservebuffer(size);
  for(int i=beg;i<end;i++) strcpy(w,item(i).c_str()),w+=item(i).length(),*w++=sep;
  *w=0;
  res.updatelen();
  return res;
}

void VarList::assign(VarList &src) {
  clear();
  for(int i=0;i<src.count;i++) add(new VarRecord(src[i]->name,src[i]->value));
}

}
//#pragma package(smart_init)
