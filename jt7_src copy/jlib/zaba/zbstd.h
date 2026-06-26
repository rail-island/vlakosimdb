#ifndef zbstdH
#define zbstdH
#include "zbobj.h"
#include <ext/xmlnode.h>
#include <gram/ssint.h>
#include <com/ptrto.h>
#include <ext/regexintf.h>

namespace zaba {

zbObject *createXMLNode(XMLNode xml,Runtime *rt);

struct zbNumber:zbObject {
  number_t value;
  zbNumber(number_t value,Runtime *rt):zbObject(rt,"Number.instance"),value(value) {}
  string tostring() {return ftos(value);}
  string typestr() {return "Number";}
  number_t op_add(number_t right) {return value+right;}
  number_t op_sub(number_t right) {return value-right;}
  number_t op_mul(number_t right) {return value*right;}
  number_t op_div(number_t right,Interpreter *ir);
  number_t op_mod(number_t right,Interpreter *ir);
  bool op_eq(number_t right) {return value==right;}
  bool op_neq(number_t right) {return value!=right;}
  bool op_let(number_t right) {return value<=right;}
  bool op_get(number_t right) {return value>=right;}
  bool op_lt(number_t right) {return value<right;}
  bool op_gt(number_t right) {return value>right;}
  number_t op_inc() {return value+1;}
  number_t op_dec() {return value-1;}
  number_t cvalue() {return value;}
  number_t op_minus() {return -value;}
  int toint() {return jlib::round(value);}
  number_t tofloat() {return value;}

  number_t sin() {return std::sin(value);}
  number_t cos() {return std::cos(value);}
  number_t tan() {return std::tan(value);}
  number_t asin() {return std::asin(value);}
  number_t acos() {return std::acos(value);}
  number_t atan() {return std::atan(value);}
  number_t log() {return std::log(value);}
  number_t exp() {return std::exp(value);}
  number_t power(number_t exponent) {return std::pow(value,exponent);}
  number_t abs() {return std::fabs(value);}
  number_t sgn() {if (value<0) return 1;if (value<0) return -1;return 0;}
  number_t round() {return jlib::round(value);}
};

struct zbCharMap:zbObject {
  string src,tar;
  zbCharMap(Runtime *rt,STRING intf="CharMap.instance"):zbObject(rt,intf) {}
  string getsource() {return src;}
  string gettarget() {return tar;}
  void setsource(string value) {src=value;}
  void settarget(string value) {tar=value;}
  char convert(char ch);
};

struct zbCodeAddress:zbObject {
  Instruction *ip;
  zbCodeAddress(Runtime *rt,Instruction *ip):ip(ip),zbObject(rt,"") {}
  Instruction *getcodeaddress() {return ip;}
};

class zbRegExp;

struct zbString:zbObject {
  string value;
//  void callmethod(Interpreter *in,const char *name,int parcnt);
  zbString(string value,Runtime *rt):zbObject(rt,"String.instance"),value(value) {}
  string tostring() {return value;}
  string typestr() {return "String";}
  int length() {return value.length();}
  string op_add(string right) {return value+right;}
  string cvalue() {return value;}
  int toint() {return value.ivalue();}
  number_t tofloat() {return value.fvalue();}
  bool beginsWith(string x) {return value.begins(x);}
  bool endsWith(string x) {return value.ends(x);}
  bool mapBeginsWith(string x,zbCharMap *map);
  int mapIndexOf(string str,zbCharMap *map);
  string substr(int index,int count,Interpreter *ir) {return value.copy(index,count);}
  string substring(int index,int end,Interpreter *ir) {return value.copy(index,end-index);}
  string toLowerCase() {return value.getlower();}
  string toUpperCase() {return value.getupper();}
  string trim() {return value.gettrimed();}
  string charAt(int index,Interpreter *ir) {string res;res<<value.c_str()[index];return res;}
  zbObject *split(STRING sep);
  void save(string file) {savebinfile(file.c_str(),value.c_str());}
  int indexOf(STRING str) {return value.searchstr(str);}
  bool op_eq(string right) {return value==right;}
  bool op_neq(string right) {return value!=right;}
  bool op_lt(string right) {return jsystem->comparetext(value.c_str(),right.c_str())<0;}
  bool op_gt(string right) {return jsystem->comparetext(value.c_str(),right.c_str())>0;}
  bool op_let(string right) {return jsystem->comparetext(value.c_str(),right.c_str())<=0;}
  bool op_get(string right) {return jsystem->comparetext(value.c_str(),right.c_str())>=0;}
  string replace(zbRegExp *re,string with);
  string replaceAll(zbRegExp *re,string with);
  string replace(string origin,string with);
  string replaceAll(string origin,string with);
  string translate() {return lang_text_ch(value,'$');}
  string mapReplace(zbCharMap *map);
};

struct zbStringBuffer:zbObject {
  string buffer;
  string cvalue() {return buffer;}
  string tostring() {return buffer;}
  void add(string what) {buffer+=what;}
  void erase(int index,int end) {buffer.erase(index,end-index);}
  zbStringBuffer(Runtime *rt,STRING intf="StringBuffer.instance"):zbObject(rt,intf) {}
  zbObject *clone() {zbStringBuffer *res=new zbStringBuffer(rt);res->buffer=buffer;return res;}
};

template<class T> struct ObjectArrayBase {
  static const bool wantlocalcopy=false;
  static const bool wantdelete=true;
  static const bool wantinitialize=false;
  void deletelocalcopy(T *item) {item->release();};
  void makelocalcopy(T *&dest,T *src) {dest=src;};
  void initialize(T *&dest) {}
};

template<class T> struct TObjectArray:MemoryArray<T*,ObjectArrayBase<T> > {
  T *getindex(zbNumber *index,Interpreter *ir) {
    int idx=countindex(index,ir);
    if (idx<0 || idx>=count) {
      error(ir,idx);
      return createVoid(ir->rt);
    }
    return buf[idx]->addref();
  }
  bool setindex(zbNumber *index,zbObject *right,Interpreter *ir) {
    int idx=countindex(index,ir);
    if (idx<0 || idx>=count) {
      error(ir,idx);
      return false;
    }
    buf[idx]->release();
    buf[idx]=right->addref();
    return true;
  }
private:
  int countindex(zbNumber *index,Interpreter *ir) {return round(index->cvalue());}
  void error(Interpreter *ir,int idx) {
    ir->nativeerror(fmtstr("Index out of range, valid %d..%d,got %d",0,count-1,idx));
  }
};

typedef TObjectArray<zbObject> ObjectArray;

struct zbHasItems:zbObject {
  ObjectArray items;
  zbHasItems(Runtime *rt,STRING intf):zbObject(rt,intf) {}
  void enum_refs_and_add_inrefs() {for(int i=0;i<items.count;i++) items[i]->gc_add_in_ref();}
  void enum_refs_and_validate() {for(int i=0;i<items.count;i++) items[i]->gc_validate();}
  void unlink_refs() {for(int i=0;i<items.count;i++) items[i]->gc_release();items.count=0;}
  int searchi(zbObject *what);
  void assign(zbHasItems *src) {items.clear();for(int i=0;i<src->items.count;i++) items.add(src->items[i]->addref());}
};

class zbMultiIndex;
struct zbArray:zbHasItems {
//  zbObject *getelem(zbNumber *index,Interpreter *ir);
  zbObject *getelem(int index);
//  zbObject *setelem(zbNumber *index,zbObject *right,Interpreter *ir);
  zbObject *setelem(int index,zbObject *right,Interpreter *ir);
//  zbObject *push(zbObject **pars,int count) {additems(pars,count);return createVoid(rt);}
  void additems(zbObject **pars,int count) {for(int i=0;i<count;i++) items.add(pars[i]->addref());}
  zbArray(Runtime *rt,STRING intf="Array.instance"):zbHasItems(rt,intf) {}
  string tostring();

  void reverse() {items.reverse();}
  int getcount() {return items.count;}
  zbObject *pop(Interpreter *ir);
  zbObject *shift(Interpreter *ir);
  zbObject *join(STRING separator,Runtime *rt);
  zbObject *join(zbObject *func,STRING separator,Runtime *rt);
  void inlsort(zbObject *comparator,Interpreter *ir);
  void sort(zbObject *obj,Interpreter *ir);
  void sort();
  void erase(int index,int count,Interpreter *ir);
  void erase(int index,Interpreter *ir);
  void insert(int index,zbObject *obj) {items.insert(index,obj->addref());}
  int search(zbObject *what) {return searchi(what);}
  void remove(zbObject *what) {int idx=searchi(what);if (idx>=0) items.erase(idx);}
  void removeRef(zbObject *what) {for(int i=0;i<items.count;i++) if (items[i]==what) {items.erase(i);return;}}

  int toint() {return items.count;}
  number_t tofloat() {return items.count;}
  zbObject *getlast() {if (items.count>0) return items.last()->addref();return createVoid(rt);}
  zbObject *getIterator();
  zbObject *clone() {zbArray *res=new zbArray(rt);res->assign(this);return res;}
  void reOrder(zbMultiIndex *order,Interpreter *ir);
  void clear() {items.clear();}
  void makeInts() {for(int i=0;i<items.count;i++) {zbObject *obj=new zbNumber(items[i]->toint(),rt);items[i]->release();items[i]=obj;}}
  void makeFloats() {for(int i=0;i<items.count;i++) {zbObject *obj=new zbNumber(items[i]->tofloat(),rt);items[i]->release();items[i]=obj;}}
  void makeStrings() {for(int i=0;i<items.count;i++) {zbObject *obj=new zbString(items[i]->tostring(),rt);items[i]->release();items[i]=obj;}}
  void makeBools() {for(int i=0;i<items.count;i++) {zbObject *obj=createLogical(items[i]->tobool(),rt);items[i]->release();items[i]=obj;}}
  zbObject *searchIf(zbObject *condition);
  zbObject *searchIfIndex(zbObject *condition);
  zbObject *searchIfValue(zbObject *condition);
  int searchRef(zbObject *ref);
  int searchInt(int value);
  int searchString(string value);
  int searchFloat(number_t value);
  zbObject *searchExtrem(zbObject *extrem,zbObject *forall);
  zbObject *collect(zbObject *condition);
  zbObject *convert(zbObject *conversion);
  zbObject *segment(int from,int len);
  void addArray(zbArray *array) {for(int i=0;i<array->items.count;i++) items.add(array->items[i]->addref());}
  void exchange(int i1,int i2,Interpreter *ir);
  bool moveUp(int index) {if (index>=1 && index<items.count) {items.exchange(index,index-1);return true;}return false;}
  bool moveDown(int index) {if (index>=0 && index<items.count-1) {items.exchange(index,index+1);return true;}return false;}
};

struct zbArrayIterator:zbObject {
  zbArray *array;int index;
  zbArrayIterator(zbArray *array,Runtime *rt,STRING intf="ArrayIterator.instance"):zbObject(rt,intf) {
    this->array=array;
    array->addref();
    index=0;
  }
  ~zbArrayIterator() {array->release();}
  zbObject *moveNext() {index++;return createVoid(rt);}
  zbObject *getValue() {if (index<array->items.count) return array->items[index]->addref();return createVoid(rt);}
};

struct zbMatrix:zbHasItems {
  TCntVec<int> bounds;
  zbMatrix(Runtime *rt,STRING intf="Matrix.instance"):zbHasItems(rt,intf) {}
  void initbounds(zbObject **pars,int parcnt);
  zbObject *opgetindex(Interpreter *ir,zbObject **pars,int parcnt);
  zbObject *opsetindex(zbObject *right,Interpreter *ir,zbObject **pars,int parcnt);
  zbObject *opsetindex(zbObject **pars,int parcnt,Interpreter *ir) {return opsetindex(pars[parcnt-1],ir,pars,parcnt-1);}
  void fill(zbObject *value) {for(int i=0;i<items.count;i++) {items[i]->release();items[i]=value->addref();}}
  zbObject *search(zbObject *what);
  zbObject *clone() {zbMatrix *res=new zbMatrix(rt);res->assign(this);res->bounds.assign(bounds);return res;}
private:
  int countindex(Interpreter *ir,zbObject **pars,int parcnt);
  void countmultiindex(int index,TCntVec<int> &multiindex);
};

inline zbObject *zbArray::getIterator() {return new zbArrayIterator(this,rt);}

struct zbDelegate:zbObject {
  zbObject *obj;
  string name;
  zbDelegate(Runtime *rt,zbObject *aobj,string name,STRING intf="Delegate.instance"):zbObject(rt,intf),name(name) {obj=aobj->addref();}
  void enum_refs_and_add_inrefs() {obj->gc_add_in_ref();}
  void enum_refs_and_validate() {obj->gc_validate();}
  void unlink_refs() {obj->gc_release();obj=NULL;}
  zbObject *opcall(Interpreter *ir,zbObject **pars,int parcnt);
};

struct zbStore:zbInstance {
  void opsetprop(string name,zbObject *prop) {vars.setvar(name,prop);}
  zbObject *idxsetprop(string name,zbObject *prop) {opsetprop(name,prop);return prop->addref();}
  zbObject *opgetprop(string name) {zbObject *res=getprop(name);if (res) return res->addref();return createVoid(rt);}
  zbObject *idxgetprop(string name) {return opgetprop(name);}
  zbStore(Runtime *rt,STRING intf="Store.instance"):zbInstance(rt,intf) {}
  zbObject *getIterator();
};

struct RefStoreItem {
  zbObject *key,*value;
  ~RefStoreItem() {key->release();value->release();}
  RefStoreItem(zbObject *akey,zbObject *avalue) {key=akey->addref();value=avalue->addref();}
  bool match(zbObject *akey) {return key==akey;}
};

struct zbRefStore:zbObject {
  zbObject *getitem(zbObject *key);
  zbObject *setitem(zbObject *key,zbObject *value);
  zbRefStore(Runtime *rt,STRING intf="RefStore.instance"):zbObject(rt,intf) {}
  zbObject *getkeys();
private:
  IObjArray<RefStoreItem> items;
};

struct zbStoreIterator:zbObject {
  int index;
  zbStore *store;
  zbStoreIterator(zbStore *astore,Runtime *rt,STRING intf="StoreIterator.instance"):zbObject(rt,intf) {
    index=0;store=astore;store->addref();
  }
  void moveNext() {index++;}
  zbObject *getValue() {
    if (index<store->vars.vars.count) return new zbString(store->vars.vars[index]->name,rt);
    return createVoid(rt);
  }
  ~zbStoreIterator() {store->release();}
};

inline zbObject *zbStore::getIterator() {return new zbStoreIterator(this,rt);}

struct zbSystem:zbInstance {
  zbObject *createarray(zbObject **pars,int count) {zbArray *res=new zbArray(rt);res->additems(pars,count);return res;}
  zbSystem(Runtime *rt,STRING intf="System.instance"):zbInstance(rt,intf) {}
  string getStackTrace(Interpreter *ir);
  zbObject *createDelegate(string name,zbObject *obj) {return new zbDelegate(rt,obj,name);}
  void opsetprop(string name,zbObject *prop) {vars.setvar(name,prop);}
  zbObject *opgetprop(string name) {zbObject *res=getprop(name);if (res) return res->addref();return createVoid(rt);}
  zbObject *getinterpretState(Interpreter *ir);
  zbObject *createInlineMethod(zbObject *locals,zbObject *obj,string name);
  number_t getclock() {return (number_t)std::clock()/(number_t)CLK_TCK;}
  zbObject *createInterruptFunction(zbObject *obj,string name,zbObject **params,int parcnt);
  int command(string com) {return system(com.c_str());}
};

struct zbFile:zbObject {
  sFile file;
  zbObject *listFiles(string dir);
  bool isFile();
  bool isDirectory();
  sFile opdiv(string path) {return file/path;}
  sFile opmod(string ext) {return file%ext.c_str();}
  zbFile(string afile,Runtime *rt,STRING intf="File.instance"):zbObject(rt,intf) {file=afile;}
  string tostring() {return file;}
  string load() {return loadtext(file);}
  void mkdir();
  void mkdirs();
  string relTo(string dir) {return file.relto(dir);}
  string getDirectory() {return file.getdir();}
  string getFile() {return file.getfile();}
  void copyTo(string newfile);
  void renameTo(string newfile);
  bool hasExt(string ext) {return file.testext(ext.c_str());}
  void remove() {std::remove(file.c_str());}
};

struct zbMultiIndex:zbObject {
  TCntVec<int> items;
  TCntVec<int> &cvalue() {return items;}
  zbMultiIndex(zbArray *array,Runtime *rt,STRING intf="MultiIndex.instance"):zbObject(rt,intf) {
    if (array) {
      items.wantcapa(array->items.count);
      for(int i=0;i<array->items.count;i++) items.add(array->items[i]->toint());
    }
  }
  int opgetindex(int index) {return items[index];}
  int opsetindex(int index,int value) {items[index]=value;return value;}
  void reverse() {items.reverse();}
  void add(int what) {items.add(what);}
  zbObject *toArray();
  string tostring() {MEMSTROUT res;res<<"(";for(int i=0;i<items.count;i++) {res<<items[i];if (i<items.count-1) res<<",";}res<<")";return res.str();}
  zbObject *clone() {zbMultiIndex *res=new zbMultiIndex(NULL,rt);res->items.assign(items);return res;}
  bool isPermutation();
};


struct zbVarList:zbObject {
  RefBase<VarList> vars;
  zbVarList(Runtime *rt,STRING intf="VarList.instance"):zbObject(rt,intf) {vars=new VarList();}
  zbObject *getIterator();
  void clear() {vars->clear();}
  void eraseVar(string name) {vars->erasevar(name);}
  string opgetprop(string name) {return vars->findvalue(name);}
  void opsetprop(string name,string value) {vars->setvar(name,value);}
  VarList &cvalue() {return *vars;}
};
struct zbVarListIterator:zbObject {
  RefBase<zbVarList> list;
  int index;
  zbVarListIterator(zbVarList *alist,Runtime *rt,STRING intf="VarListIterator.instance"):zbObject(rt,intf) {
    list=alist;index=0;
  }
  void moveNext() {index++;}
  zbObject *getValue() {
    if (index<list->vars->count) return createString((*list->vars)[index]->name,rt);
    return createVoid(rt);
  }
};
inline zbObject *zbVarList::getIterator() {
  return new zbVarListIterator(this,rt);
}

struct zbXMLIterator:zbObject {
  XMLNode xml;
  int index;
  zbXMLIterator(XMLNode xml,Runtime *rt,STRING intf="XMLIterator.instance"):zbObject(rt,intf),xml(xml) {
    index=0;
  }
  void moveNext() {index++;}
  zbObject *getValue() {
    if (index<xml.count()) return createXMLNode(xml[index],rt);
    return createVoid(rt);
  }
};

struct zbXMLPath:zbObject {
  XMLPath path;
  zbXMLPath(XMLPath path,Runtime *rt):zbObject(rt,"XMLPath.instance"),path(path) {}
};

struct zbXMLNode:zbObject {
  XMLNode xml;
  zbXMLNode(XMLNode xml,Runtime *rt);
  ~zbXMLNode();
  string getname() {return xml.getname();}
  string gettext() {return xml.gettext();}
  void setname(string text) {xml.setname(text);}
  void settext(string text) {xml.settext(text);}
  XMLNode getsubpath(zbXMLPath *path) {return xml.getat(path->path);}
  XMLNode getSub(STRING name) {return xml[name];}
  XMLNode at(int index) {return xml[index];}
  int getcount() {return xml.count();}
  int getvarCount() {return xml.varcount();}
  string varAt(int index) {return xml.getvar(index);}
  string varNameAt(int index) {return xml.getvarname(index);}
  string getVar(STRING name) {return xml.svar(name);}
  XMLNode sort(STRING indexes);
  void inlsort(zbObject *comparator,Interpreter *ir);
//  XMLNode copy1() {return xml.copy1().getnode();}
  XMLNode cvalue() {return xml;}
  XMLNode add() {return xml.add();}
  XMLNode add(string name) {return xml.add(name);}
  XMLNode forceSub(string name) {return xml.forcesub(name).getnode();}
  void setVar(string name,string value) {xml.setvar(name,value);}
  void eraseSub(string name) {xml.erasesub(name);}
  void eraseAt(int index) {xml.erasesub(index);}
  void assign(XMLNode src) {xml.assign(src);}
  void save(string file) {xml.savetofile(file);}
  void clear() {xml.clear();}
  void load(string file) {xml.loadfromfile(file);}
  void saveVars(zbVarList *varlist) {xml.savevars(*varlist->vars);}
  void loadVars(zbVarList *varlist) {xml.loadvars(*varlist->vars);}
  void clearVars() {xml.clearvars();}
  void eraseVar(string name) {xml.erasevar(name);}
  bool equalTo(XMLNode right) {return xml.equalto(right);}
  bool weekEqualTo(XMLNode right) {return xml.weekequalto(right);}
  bool isVar(string name) {return xml.isvar(name);}
  bool isSub(string name) {return xml.searchsub(name)>=0;}
  void clearSubs() {xml.clearsubs();}
  XMLNode insert(int index) {return xml.insert(index).getnode();}
  zbObject *nodeAt(TCntVec<int> &index,Interpreter *ir);
  bool opeq(zbXMLNode *right) {return xml.getnode()==right->xml.getnode();}
  bool opneq(zbXMLNode *right) {return xml.getnode()!=right->xml.getnode();}
  string tostring();
  zbObject *getIterator() {return new zbXMLIterator(xml,rt);}
  zbObject *clone() {XMLNode r=XMLNode::create();zbObject *res=createXMLNode(r,rt);r.assign(xml);return res;}
  zbObject *getparent();
  zbObject *getchilds();
  zbObject *getvarNames();
  void remove();
  bool exists(zbXMLPath *path) {return xml.exists(path->path);}
  bool exchange(int i1,int i2);
};

struct QSData;
struct QSItem {
  Strings items;
  QSData *data;
  int index;
  QSItem(zbObject **pars,int count) {
    for(int i=0;i<count;i++) items.add(pars[i]->tostring());
  }
};

struct zbQuickSorter:zbObject {
  IObjArray<QSItem> items;
  zbQuickSorter(Runtime *rt,STRING intf="QuickSorter.instance"):zbObject(rt,intf) {}
  void push(zbObject **pars,int count) {items.add(new QSItem(pars,count));}
  zbMultiIndex *sort();
};

struct zbCharMixer:zbObject {
  zbVarList *vars;
  ss::Program *prg;
  ss::Module *module;
  ss::Library *lib;
  zbCharMixer(Runtime *rt,STRING intf="CharMixer.instance");
  ~zbCharMixer();
  zbObject *getglobals() {return vars->addref();}
  void compileFile(string file,Interpreter *ir);
  string run(Interpreter *ir);
  zbObject *getvar(string name);
  void setvar(string name,zbObject *value);
  bool hasSub(string name) {return module->hassub(name);}
  string callSub(string name) {Strings pars;return module->callsub(name,pars);}
};

struct zbRegExp:zbObject {
  RegularExpr re;
//  RegularExpr *getlink() {return &re;}
//  zbObject *offset(int index=0);
  zbRegExp(string regex,Runtime *rt,STRING intf="RegExp.instance");
  zbObject *match(string str);
};

struct zbRegResult:zbObject {
  zbRegResult(string atext,RegularExpr &re,Runtime *rt,STRING intf="RegResult.instance");
  int getcount() {return ofs.count;}
  string gettext() {return text;}
  string getstr(int index) {return ofs[index](text);}
  int begin(int index) {return ofs[index].begin;}
  int end(int index) {return ofs[index].end;}
  string replace(string replace_str);
  string getmatch() {return getstr(0);}
private:
  TCntVec<StringOffset> ofs;
  string text;
};

struct zbInlineMethod:zbObject {
  RefBase<zbObject> locals;
  RefBase<zbObject> thiz;
  string name;
  zbInlineMethod(Runtime *rt,zbObject *locals,zbObject *thiz,string name,STRING intf="InlineMethod.instance"):zbObject(rt,intf),locals(locals),name(name),thiz(thiz) {}
  zbObject *opcall(Interpreter *ir,zbObject **pars,int parcnt);
};

struct zbInterruptFunction:zbObject {
  PtrTo<Interpreter> interp;
  zbInterruptFunction(Runtime *rt,STRING intf="InterruptFunction.instance");
  zbObject *opcall();
  bool getactive();
};


void initstdrt(Runtime &rt);
zbObject *createXMLNode(XMLNode xml,Runtime *rt);
inline zbObject *zbSystem::createInlineMethod(zbObject *locals,zbObject *obj,string name) {
  return new zbInlineMethod(rt,locals,obj,name);
}


}

#endif
