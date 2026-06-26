#ifndef jreflectH
#define jreflectH

namespace rtti {

class Class;

struct VarType {
  int count;//pokud je to pole>0,jinak 0
  Class *type;
  bool istype(Class *cl) {return count==0 && type==cl;}
  bool isarray(Class *cl) {return count>0 && type==cl;}
  VarType(Class *type,int count=0):type(type),count(count) {}
};

struct FldDef {
  VarType type;
  int offset;
  int attr;
  char name[0x20];
  bool istype(Class *cl) {return type.istype(cl);}
  bool isarray(Class *cl) {return type.isarray(cl);}
  bool match(STRING aname) {return aname==name;}
  FldDef(STRING aname,int offset,VarType type):offset(offset),type(type) {strcpy(name,aname);attr=0;}
};

struct Class {
  char name[0x20];
  int size;
  TCntVec<FldDef> flds;
  template <class T> void init_prim(STRING aname) {size=sizeof(T);strcpy(name,aname);}
  void init_ptr(STRING aname) {size=sizeof(void*);strcpy(name,aname);}
  template <class T> void init_rec(STRING aname) {size=sizeof(T);strcpy(name,aname);}
  void addfld(STRING name,int offset,VarType type) {flds.add(FldDef(name,offset,type));}
  void setattr(STRING fld,int attr) {if (FldDef *f=flds.vsearchv(fld)) f->attr=attr;}
  int getattr(STRING fld) {if (FldDef *f=flds.vsearchv(fld)) return f->attr;return 0;}
};

template<class T> inline T get_any(void *record,int offset) {return *(T*)(((char*)record)+offset);}
//inline void *get_ptr(void *record,int offset) {return *(void**)(((char*)record)+offset);}
//inline int get_int(void *record,int offset) {return *(int*)(((char*)record)+offset);}
inline void *get_ptr(void *record,int offset) {return get_any<void*>(record,offset);}
inline int get_int(void *record,int offset) {return get_any<int>(record,offset);}
inline void set_ptr(void *record,int offset,void *value) {*(void**)(((char*)record)+offset)=value;}
inline void set_int(void *record,int offset,int value) {*(int*)(((char*)record)+offset)=value;}
template<class T> inline void get_array(T *buf,void *record,int offset,int count) {
  T *src=(T*)(((char*)record)+offset);
  for(int i=0;i<count;i++) buf[i]=src[i];
}


#define rtti_intf(name) extern rtti::Class *name##Class;
#define rtti_define(name) rtti::Class *name##Class=NULL;
#define rtti_impl(name) static rtti::Class C##name;name##Class=&C##name;

#define rtti_def_fld(record,fldname,fldtype) {\
  record *pom=NULL;\
  record##Class->addfld(#fldname,reinterpret_cast<int>(&pom->fldname),rtti::VarType(fldtype##Class));\
}
#define rtti_array_fld(record,fldname,fldtype) {\
  record *pom=NULL;\
  record##Class->addfld(#fldname,reinterpret_cast<int>(&pom->fldname),rtti::VarType(fldtype##Class,sizeof(pom->fldname)/sizeof(*pom->fldname)));\
}

rtti_intf(Int)
rtti_intf(Char)
rtti_intf(Float)
rtti_intf(Double)
rtti_intf(String)
rtti_intf(ConstPtrChar)

void init_rtti();

}

#endif
