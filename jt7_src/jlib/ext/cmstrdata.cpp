#include <jlib.h>
#pragma hdrstop

#include "cmstrdata.h"
#include <gram/gmanal.h>

namespace jlib {

//Tvar Recordu:
//#[Record]name1:offset1(=0);len1.name2:offset2;len2.#text...

string SRgetprop(string obj,string prop) {
  if (! obj.begins("#[Record]")) return "";
  gram::Analyser a(obj.c_str());
  a.begjmp("#[Record]");
  while (*a && *a!='#') {
    string name=a.readto(":",true);
    if (name==prop) {
      int pos=a.readint();
      a.f_char(';');
      int len=a.readint();
      return obj.copy(pos+obj.search('#',1),len);
    } else a.jumpafter(".");
  }
  return "";
}
string SRcreate(VarList &vars) {
  string values;
  string header;
  header<<"#[Record]";
  for(int i=0;i<vars.count;i++) {
    header<<vars[i]->name<<':'<<values.length()<<';'<<itos(vars[i]->value.length())<<'.';
    values<<vars[i]->value;
  }
  return header+"#"+values;
}
void SRdecode(string obj,VarList &vars) {
  if (! obj.begins("#[Record]")) return;
  gram::Analyser a(obj.c_str());
  a.begjmp("#[Record]");
  while (*a && *a!='#') {
    string name=a.readto(":",true);
    int pos=a.readint();
    a.f_char(';');
    int len=a.readint();
    vars.setvar(name,obj.copy(pos+obj.search('#',1),len));
    a.f_char('.');
  }
}
string SRsetprop(string obj,string prop,string value) {
  VarList vars;
  SRdecode(obj,vars);
  vars.setvar(prop,value);
  return SRcreate(vars);
}
//void SRecord::add(SRecord right);

void checkSA(string array) throw(SAERROR) {
  if (! array.begins("#[Array]")) throw SAERROR(fmtstr("%s neni SA pole",array));
}

bool isSA(string array) {
  return array.begins("#[Array]");
}

string createSA(Strings &items) {
  MEMSTROUT mso;
  char *prolog=challoc(items.count*8+32);
  sprintf(prolog,"#[Array]%d        ",items.count);
  char *w=prolog+16;
  int charpos=items.count*8+16;
  for(int i=0;i<items.count;i++) {
    int len=items[i].length();
    sprintf(w,"%d        ",charpos);
    charpos+=len;
    w+=8;
  }
  prolog[items.count*8+16]=0;
  mso<<prolog;
  free(prolog);
  for(int i=0;i<items.count;i++) mso<<items[i];
  return mso.str();
}
void decodeSA(string array,Strings &out) {
  int cnt=SAcount(array);
  out.clear();
  for(int i=0;i<cnt;i++) out.add(SAitem(array,i));
}
int SAcount(string array) {
  checkSA(array);
  return atoi(array.c_str()+8);
}
string SAitem(string array,int index) {
  int cnt=SAcount(array);
  if (index<0 or index>=cnt) throw SAERROR(fmtstr("Index SA %d mimo hranice %d..%d",index,0,cnt-1));
  int charindex=atoi(array.c_str()+16+index*8);
  if (index==cnt-1) return array.c_str()+charindex;//je to posledni
  int endindex=atoi(array.c_str()+24+index*8);
  char *res=challoc(endindex-charindex+0x10);
  strecpy(res,array.c_str()+charindex,array.c_str()+endindex);
  string r=res;
  free(res);
  return r;
}

}

#pragma package(smart_init)


