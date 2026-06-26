#include <jlib.h>
#pragma hdrstop

#include "zbstd.h"
#include "zbobj.h"
#include "zbint.h"
#include "zbintf.h"
#include "zbrt.h"
#include <com/sorting.cpp>
#include <ext/xmldb.h>
#include "zbss.h"

#pragma option push -w-

namespace zaba {

zbInterruptFunction::zbInterruptFunction(Runtime *rt,STRING intf):zbObject(rt,intf) {
  interp=new Interpreter();
  interp->rt=rt;
}

zbObject *zbSystem::createInterruptFunction(zbObject *obj,string name,zbObject **params,int parcnt) {
  RefBase<zbInterruptFunction> res;
  res.ref=new zbInterruptFunction(rt);

  Interpreter &ir=*res->interp;
  for(int i=0;i<parcnt;i++) ir.push(params[i]);
  ir.push(obj);
  if (obj->trycallmethod(&ir,name,parcnt)) {
    ir.stopnow=true;
    ir.interpret();
    if (zbObject *top=ir.gettop()) ir.dopop();
  } else throw JError(Interpreter::nomtdmsg(obj,name,parcnt));

  return res->addref();
}

zbObject *zbInterruptFunction::opcall() {
  Interpreter &ir=*interp;
  ir.runinterrupted();
  if (zbObject *res=ir.gettop()) {
    res->addref();
    ir.dopop();
    return res;
  }
  return createVoid(rt);
}

bool zbInterruptFunction::getactive() {
  return interp->savedip;
}

void zbXMLNode::remove() {
  XMLNode p=xml.parent();
  if (p.getnode()) {
    for(int i=0;i<p.count();i++) if (p[i].getnode()==xml.getnode()) {
      p.erasesub(i);
      return;
    }
  }
}

inline zbObject *newrefstore(Runtime *rt) {return new zbRefStore(rt);}

zbObject *zbRefStore::getitem(zbObject *key) {
  if (RefStoreItem *res=items.searchvalue(key)) return res->value->addref();
  return createVoid(rt);
}

zbObject *zbRefStore::setitem(zbObject *key,zbObject *value) {
  int idx=items.searchtype(key);
  if (idx<0) {
    if (value==rt->getvoid()) return value->addref();
    items.add(new RefStoreItem(key,value));
  } else {
    if (value==rt->getvoid()) {
      items.erase(idx);
      return value->addref();
    } else {
      items[idx]->value->release();
      items[idx]->value=value->addref();
    }
  }
  return value->addref();
}

zbObject *zbRefStore::getkeys() {
  zbArray *res=new zbArray(rt);
  for(int i=0;i<items.count;i++) res->items.add(items[i]->key->addref());
  return res;
}

zbObject *zbXMLNode::getchilds() {
  zbArray *res=new zbArray(rt);
  for(int i=0;i<xml.count();i++) res->items.add(createXMLNode(xml[i],rt));
  return res;
}

bool zbXMLNode::exchange(int i1,int i2) {
  int cnt=xml.count();
  if (i1<0 || i1>=cnt || i2<0 || i2>=cnt) return false;
  xml.getnode()->childs.exchange(i1,i2);
  return true;
}

zbObject *zbXMLNode::getvarNames() {
  zbArray *res=new zbArray(rt);
  for(int i=0;i<xml.varcount();i++) res->items.add(createString(xml.getvarname(i),rt));
  return res;
}

zbObject *zbXMLNode::getparent() {
  XMLNode par=xml.parent();
  if (par.valid()) return createXMLNode(par,rt);
  return createVoid(rt);
}

zbObject *zbSystem::getinterpretState(Interpreter *ir) {
  XMLNode res=XMLNode::create();
  ir->getstate(res);
  return createXMLNode(res,rt);
}

zbObject *zbArray::segment(int from,int len) {
  zbArray *res=new zbArray(rt);
  for(int i=0;i<len && i+from<items.count;i++) {
    if (i=from<0) continue;
    res->items.add(items[i+from]->addref());
  }
  return res;
}
int zbArray::searchRef(zbObject *ref) {
  for(int i=0;i<items.count;i++) if (items[i]==ref) return i;
  return -1;
}
int zbArray::searchInt(int value) {
  for(int i=0;i<items.count;i++) if (items[i]->toint()==value) return i;
  return -1;
}
int zbArray::searchString(string value) {
  for(int i=0;i<items.count;i++) if (items[i]->tostring()==value) return i;
  return -1;
}
int zbArray::searchFloat(number_t value) {
  for(int i=0;i<items.count;i++) if (items[i]->tofloat()==value) return i;
  return -1;
}

char zbCharMap::convert(char ch) {
  const char *pos=strchr(src.c_str(),ch);
  if (pos) return tar[pos-src.c_str()];
  return ch;
}

string zbString::mapReplace(zbCharMap *map) {
  string res;
  char *dst=res.reservebuffer(strlen(value.c_str()));
  for(const char *src=value.c_str();*src;src++,dst++) *dst=map->convert(*src);
  *dst=0;
  res.updatelen();
  return res;
}

int zbString::mapIndexOf(string str,zbCharMap *map) {
  for(const char *c=value.c_str();*c;c++) {
    const char *x=c,*y=str.c_str();
    for(;*x && *y;x++,y++) if (map->convert(*x)!=map->convert(*y)) break;
    if (!*y) return c-value.c_str();
  }
  return -1;
}

bool zbString::mapBeginsWith(string x,zbCharMap *map) {
  if (x.length()>value.length()) return false;
  for(int i=0;i<x.length();i++) {
    int si=map->src.search(x[i]),li=map->src.search(value[i]);
    if (li>=0 && si>=0) {
      if (map->tar[li]!=map->tar[si]) return false;
    } else {
      if (li>=0 || si>=0) return false;
    }
  }
  return true;
}

zbObject *zbArray::searchIf(zbObject *condition) {
  RefBase<zbStore> store;store.ref=new zbStore(rt);
  zbObject *pars[1];pars[0]=store;
  for(int i=0;i<items.count;i++) {
    zbNumber *num=new zbNumber(i,rt);
    store->setprop("index",num);
    num->release();
    store->setprop("value",items[i]);
    ZabaObject res=callMethod(condition,"op:call",pars,1);
    if (res->tobool()) return items[i]->addref();
  }
  return createVoid(rt);
}
zbObject *zbArray::searchIfValue(zbObject *condition) {
  RefBase<zbStore> store;store.ref=new zbStore(rt);
  zbObject *pars[1];pars[0]=store;
  for(int i=0;i<items.count;i++) {
    zbNumber *num=new zbNumber(i,rt);
    store->setprop("index",num);
    num->release();
    store->setprop("value",items[i]);
    ZabaObject res=callMethod(condition,"op:call",pars,1);
    if (res->tobool()) return res->addref();
  }
  return createVoid(rt);
}
zbObject *zbArray::searchIfIndex(zbObject *condition) {
  RefBase<zbStore> store;store.ref=new zbStore(rt);
  zbObject *pars[1];pars[0]=store;
  for(int i=0;i<items.count;i++) {
    zbNumber *num=new zbNumber(i,rt);
    store->setprop("index",num);
    num->release();
    store->setprop("value",items[i]);
    ZabaObject res=callMethod(condition,"op:call",pars,1);
    if (res->tobool()) return createNumber(i,rt);
  }
  return createNumber(-1,rt);
}
zbObject *zbArray::convert(zbObject *conversion) {
  if (items.count==0) return createVoid(rt);
  RefBase<zbStore> store;store.ref=new zbStore(rt);
  zbObject *pars[1];pars[0]=store;
  zbArray *res=new zbArray(rt);
  for(int i=0;i<items.count;i++) {
    store->setprop("value",items[i]);
    ZabaObject newvalue=callMethod(conversion,"op:call",pars,1);
    res->items.add(newvalue->addref());
  }
  return res->addref();
}
zbObject *zbArray::searchExtrem(zbObject *getextrem,zbObject *forall) {
  if (items.count==0) return createVoid(rt);
  ZabaObject extrem;
  RefBase<zbStore> store;store.ref=new zbStore(rt);
  zbObject *pars[1];pars[0]=store;
  for(int i=0;i<items.count;i++) {
    store->setprop("value",items[i]);
    ZabaObject value=callMethod(getextrem,"op:call",pars,1);
    if (!extrem) extrem=value;
    else {
      store->setprop("extrem",value);
      store->setprop("value",extrem);
      ZabaObject isextrem=callMethod(forall,"op:call",pars,1);
      if (isextrem->tobool()) extrem=value;
      store->deleteprops(2);
    }
  }
  return extrem->addref();
}
zbObject *zbArray::collect(zbObject *condition) {
  if (items.count==0) return new zbArray(rt);
  RefBase<zbStore> store;store.ref=new zbStore(rt);
  zbObject *pars[1];pars[0]=store;
  zbArray *res=new zbArray(rt);
  for(int i=0;i<items.count;i++) {
    store->setprop("value",items[i]);
    ZabaObject copy=callMethod(condition,"op:call",pars,1);
    if (copy->tobool()) res->items.add(items[i]->addref());
  }
  return res->addref();
}

zbObject *zbArray::join(zbObject *func,STRING separator,Runtime *rt) {
  RefBase<zbStore> store;store.ref=new zbStore(rt);
  zbObject *pars[1];pars[0]=store;
  MEMSTROUT res;
  for(int i=0;i<items.count;i++) {
    store->setprop("value",items[i]);
    ZabaObject zres=callMethod(func,"op:call",pars,1);
    res<<zres->tostring();
    if (i<items.count-1) res<<separator;
  }
  return createString(res.str(),rt);
}


number_t zbNumber::op_div(number_t right,Interpreter *ir) {
  if (right==0) {
    ir->nativeerror(fmtstr("Division by zero\nStack:\n%s",ir->getstacktrace().c_str()));
    return 0;
  }
  return value/right;
}
number_t zbNumber::op_mod(number_t right,Interpreter *ir) {
  if (right==0) {
    ir->nativeerror(fmtstr("Division by zero\nStack:\n%s",ir->getstacktrace().c_str()));
    return 0;
  }
  return jlib::round(value)%jlib::round(right);
}

//zbObject *zbArray::getelem(zbNumber *index,Interpreter *ir) {
//  return items.getindex(index,ir);
//}
zbObject *zbArray::getelem(int index) {
  if (index>=0 && index<items.count) return items[index]->addref();
  return createVoid(rt);
}
zbObject *zbArray::setelem(int index,zbObject *right,Interpreter *ir) {
  if (index>=0 && index<items.count) {
    items[index]->release();
    items[index]=right->addref();
  } else {
    ir->nativeerror(fmtstr("Array index out of bounds:count=%s,index=%s",items.count,index));
  }
  return createVoid(rt);
}
//zbObject *zbArray::setelem(zbNumber *index,zbObject *right,Interpreter *ir) {
//  if (items.setindex(index,right,ir)) return right->addref();
//  return createVoid(rt);
//}
void zbArray::exchange(int i1,int i2,Interpreter *ir) {
  if (i1>=0 && i2>=0 && i1<items.count && i2!=items.count) items.exchange(i1,i2);
  else ir->nativeerror("Array.exchange:index out of bounds");
}


bool isdirectory(string file) {
  TCntVec<FileRec> r;
  jsystem->listfiles(file,r);
  return r.count>0 && r[0].isdir;
}

void zbFile::mkdir() {
  jsystem->makedir(file);
}
void zbFile::mkdirs() {
  TObjVec<sFile> dirs;
  sFile act=file;
  while (*act.c_str()) {
    if (!isdirectory(act)) dirs.add(act);
    act=act.getdir();
  }
  for(int i=dirs.count-1;i>=0;i--) jsystem->makedir(dirs[i]);
}
void zbFile::copyTo(string newfile) {
  jsystem->copyfile(file,newfile);
}
void zbFile::renameTo(string newfile) {
  rename(file,newfile.c_str());
}

zbObject *zbDelegate::opcall(Interpreter *ir,zbObject **pars,int parcnt) {
  ZabaObject res=callMethod(obj,name,pars,parcnt);
  if (res) return res->addref();
  return createVoid(rt);
}

zbObject *zbInlineMethod::opcall(Interpreter *ir,zbObject **pars,int parcnt) {
  if (parcnt!=1) {
    ir->nativeerror(fmtstr("InternalMethod::call:bad parameter count, may be 1, is %d",parcnt)+ir->posinfo());
    return createVoid(rt);
  }
  zbObject *p[2];
  p[0]=locals;p[1]=pars[0];
  ZabaObject res=callMethod(thiz,name,p,2);
  if (res) return res->addref();
  return createVoid(rt);
}

void zbMatrix::initbounds(zbObject **pars,int parcnt) {
  bounds.wantcapa(parcnt);
  for(int i=0;i<parcnt;i++) bounds.add(pars[i]->toint());
  int size=1;
  for(int i=0;i<bounds.count;i++) size*=bounds[i];
//  if (bounds.count==0) size=0;
  items.wantcapa(size);
  for(int i=0;i<size;i++) items.buf[i]=createVoid(rt);
  items.count=size;
}

int zbMatrix::countindex(Interpreter *ir,zbObject **pars,int parcnt) {
  TCntVec<int> indexes;
  indexes.wantcapa(parcnt);
  for(int i=0;i<parcnt;i++) indexes.add(pars[i]->toint());
  if (indexes.count!=bounds.count) {
    ir->nativeerror(fmtstr("Matrix error:matrix is %d-d, access is %d-d",bounds.count,indexes.count));
    return -1;
  }
  int index=0;
  for(int i=0;i<indexes.count;i++) {
    int bnd=bounds[i],idx=indexes[i];
    if (idx<0 || idx>=bnd) {
      ir->nativeerror(fmtstr("%d-th index out of range,valid 0..%d, got %d",i+1,bnd-1,idx));
      return -1;
    }
    index*=bnd;
    index+=idx;
  }
  return index;
}

zbObject *zbMatrix::opgetindex(Interpreter *ir,zbObject **pars,int parcnt) {
  int i=countindex(ir,pars,parcnt);
  if (i<0) return createVoid(rt);
  return items[i]->addref();
}
zbObject *zbMatrix::opsetindex(zbObject *right,Interpreter *ir,zbObject **pars,int parcnt) {
  int i=countindex(ir,pars,parcnt);
  if (i<0) return createVoid(rt);
  items[i]->release();
  items[i]=right->addref();
  return right->addref();
}

string zbSystem::getStackTrace(Interpreter *ir) {
  return ir->getstacktrace();
}

zbObject *zbXMLNode::nodeAt(TCntVec<int> &index,Interpreter *ir) {
  XMLNode res=xml;
  for(int i=0;i<index.count;i++) {
    if (index[i]<0 || index[i]>=res.count()) {
      ir->nativeerror("Bad multiindex in XMLNode");
      return createVoid(rt);
    }
    res=res[index[i]].getnode();
  }
  return createXMLNode(res,rt);
}

string zbXMLNode::tostring() {
  MEMSTROUT res;
  res<<xml.getname();
  int varcnt=xml.varcount();
  for(int i=0;i<varcnt;i++) res<<"["<<xml.getvarname(i)<<"="<<xml.getvar(i)<<"]";
  int subcnt=xml.count();
  for(int i=0;i<subcnt;i++) res<<"{"<<xml.at(i).getname()<<"}";
  return res.str();
}

void zbMatrix::countmultiindex(int index,TCntVec<int> &multiindex) {
  multiindex.clear();
  multiindex.wantcapa(bounds.count);
  for(int i=bounds.count-1;i>=0;i--) {
    multiindex.add(index % bounds[i]);
    index/=bounds[i];
  }
  multiindex.reverse();
}

zbObject *zbMatrix::search(zbObject *what) {
  int idx=searchi(what);
  if (idx<0) return createVoid(rt);
  TCntVec<int> m;
  countmultiindex(idx,m);
  return createMultiIndex(m,rt);
}

int zbHasItems::searchi(zbObject *what) {
  for(int i=0;i<items.count;i++) {
    ZabaObject res=callMethod(what,"op:eq",&items[i],1);
    if (res && res->tobool()) return i;
  }
  return -1;
}

string zbArray::tostring() {
  MEMSTROUT mso;
  for(int i=0;i<items.count;i++) mso<<items[i]->tostring()<<"\n";
  return mso.str();
}

zbObject *fulllistfiles(string dir,Runtime *rt) {
  TCntVec<FileRec> files;
  jsystem->listfiles(dir,files);
  sFile DIR=dir;
  zbArray *res=new zbArray(rt);
  for(int i=0;i<files.count;i++) res->items.add(createString(DIR.getdir()/files[i].name,rt));
  return res;
}

zbObject *zbFile::listFiles(string dir) {
  TCntVec<FileRec> files;
  jsystem->listfiles(file/dir,files);
  zbArray *res=new zbArray(rt);
  for(int i=0;i<files.count;i++) res->items.add(createString(files[i].name,rt));
  return res;
}
bool zbFile::isFile() {
  TCntVec<FileRec> r;
  jsystem->listfiles(file,r);
  return r.count>0 && ! r[0].isdir;
}
bool zbFile::isDirectory() {
  return isdirectory(file);
/*  TCntVec<FileRec> r;
  jsystem->listfiles(file,r);
  return r.count>0 && r[0].isdir;*/
}
zbFile *getrootdir(Runtime *rt) {
  return new zbFile(rt->getrootdir(),rt);
}

inline XMLNode loadxml(string file) {
  XMLNode res=XMLNode::create();
  try {
    res.loadfromfile(file);
  } catch (XMLLoadError &e) {
    e.show();
  }
  return res;
}

zbObject *newstring(zbObject *obj,Runtime *rt) {return createString(obj->tostring(),rt);}
zbObject *newnumber(zbObject *obj,Runtime *rt) {return createNumber(obj->tostring().fvalue(),rt);}

XMLNode zbXMLNode::sort(STRING indexes) {
  XMLSort sorting;
  Analyser a(indexes.c_str());
  while (a.jmpwhite(),*a) sorting.addcol(a.readto("\n",true).gettrimed(),SortCol::text);
  sorting.xml=xml;
  return sorting.work();
}

zbObject * zbArray::pop(Interpreter *ir) {
  if (items.count>0) {
    items.count--;
    return items.buf[items.count];
  }
  ir->nativeerror("Calling pop for clear Array");
  return createVoid(ir->rt);
}
zbObject *zbArray::shift(Interpreter *ir) {
  if (items.count>0) {
    items.count--;
    zbObject *res=items.buf[0];
    memmove(items.buf,items.buf+1,sizeof(zbObject*)*items.count);
    return res;
  }
  ir->nativeerror("Calling shift for clear Array");
  return createVoid(ir->rt);
}
zbObject *zbArray::join(STRING separator,Runtime *rt) {
  MEMSTROUT res;
  for(int i=0;i<items.count;i++) {
    res<<items.buf[i]->tostring().c_str();
    if (i<items.count-1) res<<separator;
  }
  return createString(res.str(),rt);
}

void zbArray::erase(int index,int count,Interpreter *ir) {
  if (index>=0 && index+count<=items.count) items.erase(index,count);
  else ir->nativeerror("Array.erase:index out of bounds");
}
void zbArray::erase(int index,Interpreter *ir) {
  if (index>=0 && index<items.count) items.erase(index);
  else ir->nativeerror("Array.erase:index out of bounds");
}

struct ObjCmp {
  RefBase<zbObject> cmp;
  Interpreter *ir;
  ObjCmp(zbObject *acmp,Interpreter *air) {cmp=acmp;ir=air;}
  int operator () (zbObject *left,zbObject *right) const {
    zbObject *pars[2]={left,right};
    ZabaObject res=callMethod(cmp,"op:call",pars,2);
    return res->toint();
  }
};
struct StrCmp {
  int operator () (zbObject *left,zbObject *right) const {
    return jsystem->comparetext(left->tostring().c_str(),right->tostring().c_str());
  }
};

void zbArray::sort(zbObject *obj,Interpreter *ir) {
  heapsort(reinterpret_cast<zbObject**>(items.buf),items.count,ObjCmp(obj,ir));
}

struct InlCmp {
  RefBase<zbObject> cmp;
  Interpreter *ir;
  InlCmp(zbObject *acmp,Interpreter *air) {cmp=acmp;ir=air;}
  int operator () (zbObject *left,zbObject *right) const {
    RefBase<zbStore> store;store.ref=new zbStore(ir->rt);
    zbObject *pars[1];pars[0]=store;
    store->setprop("left",left);
    store->setprop("right",right);
    ZabaObject res=callMethod(cmp,"op:call",pars,1);
    return res->toint();
  }
};

void zbArray::inlsort(zbObject *comparator,Interpreter *ir) {
  heapsort(reinterpret_cast<zbObject**>(items.buf),items.count,InlCmp(comparator,ir));
}

struct XMLInlCmp {
  RefBase<zbObject> cmp;
  Interpreter *ir;
  XMLInlCmp(zbObject *acmp,Interpreter *air) {cmp=acmp;ir=air;}
  int operator () (XMLDataNode *left,XMLDataNode *right) const {
    RefBase<zbStore> store;store.ref=new zbStore(ir->rt);
    zbObject *pars[1];pars[0]=store;
    zbObject *zleft=createXMLNode(XMLNode(left),ir->rt);store->setprop("left",zleft);zleft->release();
    zbObject *zright=createXMLNode(XMLNode(right),ir->rt);store->setprop("right",zright);zright->release();
    ZabaObject res=callMethod(cmp,"op:call",pars,1);
    return res->toint();
  }
};

void zbArray::sort() {
  heapsort(reinterpret_cast<zbObject**>(items.buf),items.count,StrCmp());
}

void zbXMLNode::inlsort(zbObject *comparator,Interpreter *ir) {
  heapsort(xml.getnode()->childs.buf,xml.count(),XMLInlCmp(comparator,ir));
}

zbObject * zbString::split(STRING separator) {
  zbArray *res=new zbArray(rt);
  const char *c=value.c_str();
  const char *sep=separator;
  while (*c) {
    const char *d=c;
    while (*d and strchr(sep,*d)==NULL) d++;
    char *s=challoc(d-c+1);strecpy(s,c,d);
    res->items.add(createString(s,rt));
    c=d;if (*c) c++;
  }
  return res;
}

zbObject *zbMultiIndex::toArray() {
  zbArray *res=new zbArray(rt);
  for(int i=0;i<items.count;i++) res->items.add(createNumber(items[i],rt));
  return res;
}

struct zbDate:zbObject {
  tm value;
  zbDate(Runtime *rt):zbObject(rt,"Date.instance") {
    time_t now=time(NULL);
    value=*localtime(&now);
  }
  zbDate(string v,Runtime *rt):zbObject(rt,"Date.instance") {
    fromString(v);
  }
  void fromString(string v) {
    zerorec(value);
    gram::Analyser a(v.c_str());
    value.tm_mday=a.readint();
    a.jsp();
    if (*a=='.') {
      a++;
      a.jsp();
      value.tm_mon=a.readint()-1;
      a.jsp();
      if (*a=='.') {
        a++;
        a.jsp();
        value.tm_year=a.readint()-1900;
        a.jsp();
        value.tm_hour=a.readint();
        a.jsp();
        if (*a==':') {
          a++;
          a.jsp();
          value.tm_min=a.readint();
          a.jsp();
          if (*a==':') {
            a++;
            a.jsp();
            value.tm_sec=a.readint();
          }
        }
      }
    }
    mktime(&value);
    if (value.tm_isdst) value.tm_hour--;
    mktime(&value);
  }
  int getday() {
    return value.tm_mday;
  }
  void setday(int newvalue) {
    value.tm_mday=newvalue;
    mktime(&value);
  }
  int getmonth() {
    return value.tm_mon+1;
  }
  void setmonth(int newvalue) {
    value.tm_mon=newvalue-1;
    mktime(&value);
  }
  int getyear() {
    return value.tm_year+1900;
  }
  void setyear(int newvalue) {
    value.tm_year=newvalue-1900;
    mktime(&value);
  }
  int gethours() {
    return value.tm_hour;
  }
  void sethours(int newvalue) {
    value.tm_hour=newvalue;
    mktime(&value);
  }
  int getminutes() {
    return value.tm_min;
  }
  void setminutes(int newvalue) {
    value.tm_min=newvalue;
    mktime(&value);
  }
  int getseconds() {
    return value.tm_sec;
  }
  void setseconds(int newvalue) {
    value.tm_sec=newvalue;
    mktime(&value);
  }
  int getticks() {
    return mktime(&value);
  }
  void setticks(int value) {
    time_t now=value;
    this->value=*localtime(&now);
  }
  string getsortString() {
    return fmtstr("%04d/%02d/%02d %02d:%02d:%02d",getyear(),getmonth(),getday(),value.tm_hour,value.tm_min,value.tm_sec);
  }
  string tostring() {
    return fmtstr("%d.%d.%d %d:%02d:%02d",getday(),getmonth(),getyear(),value.tm_hour,value.tm_min,value.tm_sec);
  }
  string format(string fmt) {
    if (fmt=="@sort") return getsortString();
    if (fmt=="@file") return fmtstr("%04d_%02d_%02d_%02d_%02d_%02d",getyear(),getmonth(),getday(),value.tm_hour,value.tm_min,value.tm_sec);
    if (fmt=="@filed") return fmtstr("%04d_%02d_%02d",getyear(),getmonth(),getday());
    return tostring();
  }
  string getasString() {return tostring();}
  void setasString(string v) {
    fromString(v);
  }
};

zbObject *newdate(Runtime *rt) {return new zbDate(rt);}
zbObject *newdate(string value,Runtime *rt) {return new zbDate(value,rt);}
zbObject *newarray(Runtime *rt,zbObject **pars,int count) {zbArray *res=new zbArray(rt);res->additems(pars,count);return res;}
zbObject *newXMLNode(Runtime *rt) {XMLNode xml=XMLNode::create();zbObject *res=createXMLNode(xml,rt);return res;}
zbObject *newstringbuf(Runtime *rt) {return new zbStringBuffer(rt);}
zbObject *newexception(string msg,Runtime *rt) {zbException *e=new zbException(rt);e->setmsg(msg);return e;}
zbObject *newstore(Runtime *rt) {return new zbStore(rt);}
zbObject *newmatrix(Runtime *rt,zbObject **pars,int parcnt) {
  zbMatrix *res=new zbMatrix(rt);
  res->initbounds(pars,parcnt);
  return res;
}
zbObject *newvarlist(Runtime *rt) {return new zbVarList(rt);}
zbObject *newmultiindex(Runtime *rt) {return new zbMultiIndex(NULL,rt);}
zbObject *newmultiindex(zbArray *array,Runtime *rt) {return new zbMultiIndex(array,rt);}
zbObject *createFile(string file,Runtime *rt) {return new zbFile(file,rt);}
zbObject *newfile(string file,Runtime *rt) {return createFile(file,rt);}

number_t frand01() {return (number_t)rand()/(number_t)RAND_MAX;}
int rangerand(int range) {return rand()%range;}

number_t gete() {return M_E;}
number_t getpi() {return M_PI;}
number_t minimum(zbObject **array,int parcnt) {
  if (parcnt==0) return 0;
  number_t res=array[0]->tofloat();
  for(int i=1;i<parcnt;i++) {
    number_t act=array[i]->tofloat();
    if (act<res) res=act;
  }
  return res;
}
number_t maximum(zbObject **array,int parcnt) {
  if (parcnt==0) return 0;
  number_t res=array[0]->tofloat();
  for(int i=1;i<parcnt;i++) {
    number_t act=array[i]->tofloat();
    if (act>res) res=act;
  }
  return res;
}

struct QSData {

};

inline int qs_compare(const void *p_a,const void *p_b) {
  QSItem *a=*(QSItem**)p_a,*b=*(QSItem**)p_b;
  int cnt=min(a->items.count,b->items.count);
  for(int i=0;i<cnt;i++) {
    string s1=a->items[i],s2=b->items[i];
    int res=jsystem->comparetext(s1.c_str(),s2.c_str());
    if (res) return res;
  }
  return 0;
}

zbMultiIndex *zbQuickSorter::sort() {
  QSData qsdata;
  for(int i=0;i<items.count;i++) {
    items[i]->data=&qsdata;
    items[i]->index=i;
  }
  TCntVec<QSItem*> its;
  its.assign(items);
  qsort(its.buf,its.count,sizeof(QSItem*),qs_compare);
  zbMultiIndex *res=new zbMultiIndex(NULL,rt);
  for(int i=0;i<its.count;i++) res->items.add(its[i]->index);
  return res;
}

void zbArray::reOrder(zbMultiIndex *order,Interpreter *ir) {
  if (order->items.count!=items.count) {
    ir->nativeerror(fmtstr("Array.reOrder, bad length of permutation"));
    return;
  }
  if (!order->isPermutation()) {
    ir->nativeerror(fmtstr("Array.reOrder, MultiIndex is not permutation"));
    return;
  }

  TCntVec<zbObject*> res;
  res.assign(items);
//  for(int i=0;i<items.count;i++) items[order->items[i]]=res[i];
  for(int i=0;i<items.count;i++) items[i]=res[order->items[i]];
}

bool zbMultiIndex::isPermutation() {
  if (items.count==0) return true;
  bool *test=new bool[items.count];
  for(int i=0;i<items.count;i++) test[i]=false;
  for(int i=0;i<items.count;i++) {
    int idx=items[i];
    if (idx<0 || idx>=items.count || test[idx]) {
      delete []test;
      return false;
    }
    test[idx]=true;
  }
  delete []test;
  return true;
}

inline zbObject *newquicksorter(Runtime *rt) {return new zbQuickSorter(rt);}
inline zbCharMap *newcharmap(Runtime *rt) {return new zbCharMap(rt);}
zbObject *newsequence(number_t start,number_t stop,number_t step,Runtime *rt) {
  zbArray *res=new zbArray(rt);
  for(number_t i=start;i<=stop;i+=step) res->items.add(new zbNumber(i,rt));
  return res;
}

void zbCharMixer::compileFile(string file,Interpreter *ir) {
  prg->compilecm(file);
}

string zbCharMixer::run(Interpreter *ir) {
  return prg->run();
}

zbCharMixer::zbCharMixer(Runtime *rt,STRING intf):zbObject(rt,intf) {
//  lib=new zaba::ssRunner(rt);
  lib=new ss::Library();
  vars=new zbVarList(rt);
  module=new ss::Module();
  module->lib=lib;
  prg=new ss::Program(module);
  vars->vars=module->globals;
}
zbCharMixer::~zbCharMixer() {
  delete prg;
  delete module;
  delete lib;
  vars->release();
}

zbObject *zbCharMixer::getvar(string name) {
  return new zbString(module->globals->svar(name),rt);
}
void zbCharMixer::setvar(string name,zbObject *value) {
  if (zbXMLNode *xml=dynamic_cast<zbXMLNode*>(value)) {
    module->globals->setvar(name,module->objtostr(ss::Library::xmltoobj(xml->xml)));
  } else {
    module->globals->setvar(name,value->tostring());
  }
}

zbCharMixer *newcharmixer(Runtime *rt) {return new zbCharMixer(rt);}
int cmpnums(number_t left,number_t right) {
  if (left<right) return -1;
  if (left>right) return 1;
  return 0;
}
int cmpstrs(string left,string right) {return jsystem->comparetext(left.c_str(),right.c_str());}

zbRegExp::zbRegExp(string regex,Runtime *rt,STRING intf):zbObject(rt,intf) {
  re.compile(regex);
}
zbObject *zbRegExp::match(string str) {
  if (re.run(str)) return new zbRegResult(str,re,rt);
  return createVoid(rt);
}
zbRegResult::zbRegResult(string atext,RegularExpr &re,Runtime *rt,STRING intf):zbObject(rt,intf) {
  text=atext;
  int cnt=re.subcount();
  for(int i=0;i<=cnt;i++) ofs.add(re.offset(i));
}
string zbRegResult::replace(string replace_str) {
  MEMSTROUT mso;
  for(const char *c=replace_str.c_str();*c;) {
    if (*c=='\\' && c[1]>='1' && c[1]<='9') {
      c++;
      mso<<getstr(*c-'0');
      c++;
    } else mso<<*c++;
  }
  return mso.str();
}
string zbString::replace(zbRegExp *re,string with) {
  return re->re.replace(value,with);
}
string zbString::replaceAll(zbRegExp *re,string with) {
  return re->re.replaceall(value,with);
}
string zbString::replace(string origin,string with) {
  string res=value;
  res.unique();
  res.replace(origin,with);
  return res;
}
string zbString::replaceAll(string origin,string with) {
  string res=value;
  res.unique();
  res.replaceall(origin,with);
  return res;
}


/*
zbObject *zbRegExp::offset(int index) {
  StringOffset o=re.offset(index);
  if (o.valid()) {
    zbMultiIndex *res=new zbMultiIndex(NULL,rt);
    res->items.add(o.begin);
    res->items.add(o.end);
    return res;
  }
  return createVoid(rt);
}
*/

zbObject *newregexp(string regex,Runtime *rt) {return new zbRegExp(regex,rt);}
zbObject *newxmlpath(string path,Runtime *rt) {return new zbXMLPath(XMLPath(path),rt);}

zbXMLNode::zbXMLNode(XMLNode xml,Runtime *rt):zbObject(rt,"XMLNode.instance"),xml(xml) {
  xassert(xml.getnode()->data==NULL);
  xml.getnode()->data=this;
}
zbXMLNode::~zbXMLNode() {
  xassert(xml.getnode()->data==this);
  xml.getnode()->data=NULL;
}
zbObject *createXMLNode(XMLNode xml,Runtime *rt) {
  if (xml.getnode()==NULL) return createVoid(rt);
  if (void *data=xml.getnode()->data) return ((zbXMLNode*)data)->addref();
  return new zbXMLNode(xml,rt);
}


#include "lib/stdobjs.h"
void initstdrt(Runtime &rt) {init_stdobjs_rt(rt);}

#include "lib/stdobjs.cc"

}

#pragma option pop


