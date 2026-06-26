#ifndef xerrorH
#define xerrorH

#ifndef _DEBUG
#define NOLOGICERRORCHECK
#endif

namespace jlib {

struct XError {
  string msg;
  void init(const char *amsg) {msg=amsg;}
  void init(string amsg) {msg=amsg;}
  void initfmt(const char *format,...);
  XError(const char *amsg) {init(amsg);}
  XError(string amsg) {init(amsg);}
  XError() {}
  virtual void handle();//zpracovani vyjimky-vypsani zpravy uzivateli...
  //neslouzi k osetreni vyjimky, jen k informaci o ni
  //je volano jeste pred vuhozenim
  virtual string exceptname();//vrati jmeno vyjimky
  virtual string formatmessage() {return msg;}
};

extern void (*showXErrorMessage)(const char *msg);

#define SimpleExceptType(EType,EParent) \
struct EType:EParent {\
  EType(const char *amsg):EParent(amsg) {}\
  EType(string amsg):EParent(amsg) {}\
  EType() {}\
};

SimpleExceptType(XAssertionFailed,XError);
SimpleExceptType(XMemoryOverwrited,XError);
SimpleExceptType(XNoMemory,XError);
SimpleExceptType(XCallingOfAbstractMethod,XError);

struct XIndexError:XError {
  int count,index;
  XIndexError(int aindex,int acount,string msg=""):XError(msg) {count=acount;index=aindex;}
  string formatmessage();
};

inline void XErrorMessage(string msg) {
  if (showXErrorMessage) showXErrorMessage(msg.c_str());
}

#ifdef __FUNC__
#define __FUNCTION__ __FUNC__
#else
#define __FUNCTION__ "???"
#endif

#define __POSINFO__ fmtstr("Soubor:%s;\nFunkce:%s;\nRadka:%d\n",__FILE__,__FUNCTION__,__LINE__)
#define __OBJPOSINFO__ __POSINFO__
//#define __OBJPOSINFO__ fmtstr("Soubor:%s;\nFunkce:%s;\nObjekt:%s;\nRadka:%d\n",__FILE__,__FUNCTION__,typeid(*this).name(),__LINE__)

#define xthrow(E) \
{E e(__POSINFO__);e.handle();throw e;}

#define xobjthrow(E) \
{E e(__OBJPOSINFO__);e.handle();throw e;}

#define xthrowmsg(E,msg) \
{E e(fmtstr("%s;\nZprava:%s",__POSINFO__.c_str(),fmtstr msg.c_str() ));e.handle();throw e;}

#define xxassert(cond,E) \
{if (not (cond)) {E e(fmtstr("Podminka:%s;%s",#cond,__POSINFO__.c_str() ));e.handle();throw e;}}

#define xassert(cond) xxassert(cond,XAssertionFailed)

//lze pouzit unvitr objektu,vypise jeho dynamicke jmeno
#define xxobjassert(cond,E) \
{if (not (cond)) {E e(fmtstr("Podminka:%s;\n%s",#cond,__OBJPOSINFO__.c_str() ));e.handle();throw e;}}

#define xobjassert(cond) xxobjassert(cond,XAssertionFailed)

#define xxassertmsg(cond,msg,E) \
{if (not (cond)) {E e(fmtstr("Podminka:%s;\n%s%s",#cond,__POSINFO__.c_str(),fmtstr msg.c_str() ));e.handle();throw e;}}

//msg jsou argumenty funkce fmtstr v zavorce(volani napr: xassertmsg(i>=0,("i mimo=%d",i)); )
#define xassertmsg(cond,msg) xxassertmsg(cond,msg,XAssertionFailed)

#define xxobjassertmsg(cond,E,msg) \
{if (not (cond)) {E e(fmtstr("Podminka:%s;\n%s%s",#cond,__OBJPOSINFO__.c_str(),fmtstr msg.c_str() ));e.handle();throw e;}}

#define xobjassertmsg(cond,msg) xxobjassertmsg(cond,XAssertionFailed,msg);

//hlida,zda STATICKY retezec ma delku, ktera se vejde do pameti
#define xcheckstrlen(str) \
{if (strlen(str)>=sizeof(str)) xthrow(XMemoryOverwrited);}

#define xcheckindex(index,count) {if ((index)<0 or (index)>=(count)) {XIndexError e((index),(count),__POSINFO__);e.handle();throw e;}}

#define xobjcheckindex(index,count) {if ((index)<0 or (index)>=(count)) {XIndexError e((index),(count),__OBJPOSINFO__);e.handle();throw e;}}

#define xabstract xobjthrow(XCallingOfAbstractMethod)


#ifdef NOLOGICERRORCHECK
#  define lassert(cond)
#  define lobjassert(cond)
#  define lassertmsg(cond,msg)
#  define lobjassertmsg(cond,msg)
#  define lcheckstrlen(str)
#  define lobjcheckindex(index,count)
#  define lcheckindex(index,count)
#else
#  define lassert(cond) xassert(cond)
#  define lobjassert(cond) xobjassert(cond)
#  define lassertmsg(cond,msg) xassertmsg(cond,msg)
#  define lobjassertmsg(cond,msg) xobjassertmsg(cond,msg)
#  define lcheckstrlen(str) xcheckstrlen(str)
#  define lobjcheckindex(index,count) xobjcheckindex(index,count)
#  define lcheckindex(index,count) xcheckindex(index,count)
#endif

}

#endif
