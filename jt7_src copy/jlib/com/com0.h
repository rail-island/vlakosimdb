#ifndef HCOM0
#define HCOM0

#define ENOMEMERROR 5
#define EINDEXERROR 6
#define EASSERT 7

//#include <com/comdef.h>
#define STDCALL __stdcall

#define override
#define final_class

#define direct_property(type,name) type name;type get##name() {return name;} void set##name(type value) {name=value;}

struct JP_VOID {
  void *p;
  explicit JP_VOID(void *p):p(p) {}
};
inline void * operator new(size_t size,JP_VOID ptr) {return ptr.p;}

namespace jlib {

typedef string chrs;

//inline void * operator new(size_t size) {return malloc(size);}
//inline void operator delete(void *p,size_t size) {free(p);}

template<class T> inline T& jmin(T &a,T &b) {return a<b?a:b;}
template<class T> inline T& jmax(T &a,T &b) {return a>b?a:b;}

#ifndef min
template<class T> inline const T& min(const T &a,const T &b) {return a<b?a:b;}
#endif
#ifndef max
template<class T> inline const T& max(const T &a,const T &b) {return a>b?a:b;}
#endif

template<class T> inline const T& jmin(const T &a,const T &b,const T &c) {return min(min(a,b),c);}
template<class T> inline const T& jmax(const T &a,const T &b,const T &c) {return max(max(a,b),c);}

template<class T> inline const T& jmin(const T &a,const T &b,const T &c,const T &d) {return min(min(a,b),min(c,d));}
template<class T> inline const T& jmax(const T &a,const T &b,const T &c,const T &d) {return max(max(a,b),max(c,d));}


template<class T> inline void exchange(T &a,T &b) {T pom=a;a=b;b=pom;}

template <class T> inline void exchangemem(T &a,T &b) {
  char buf[sizeof(T)];
  memcpy(buf,&a,sizeof(T));
  memcpy(&a,&b,sizeof(T));
  memcpy(&b,buf,sizeof(T));
}

#define and &&
#define or ||
#define not !
#define jesude(arg_a) (not ((arg_a)&1))
#define jeliche(arg_a) ((arg_a)&1)

template<class T> inline void fillarray(T *buf,int count,T init)
  {for(T *c=buf,*end=buf+count;c<end;c++) *c=init;}

inline int round(double arg) {return floor(arg+0.5);}
inline double frand() {return rand()/(RAND_MAX+1.0);}


#define RTB(c,cn,line)\
  char *cn=readline(c);\
  char line[0x100];\
  strecpy(line,c,cn);\
  trimr(line);

typedef char* pchar;
typedef unsigned char uchar;


//struktury
struct XYREC {
  int x,y;
  XYREC() {}
  XYREC(int ax,int ay) {x=ax;y=ay;}
};
inline bool operator ==(const XYREC &a,const XYREC &b) {return a.x==b.x and a.y==b.y;}
inline bool operator !=(const XYREC &a,const XYREC &b) {return a.x!=b.x or a.y!=b.y;}
struct XYZREC {
  int x,y,z;
  XYZREC() {};
  XYZREC(int ax,int ay,int az) {x=ax;y=ay;z=az;}
};

template<class T> inline void makeless(T &a,T &b) {if (a>b) {T pom=a;a=b;b=pom;}}


struct CharMap {
  const char *map;
  CharMap(const char *amap) {map=amap;}
  char operator [](char c) {return map[uchar(c)];}
};

extern const string estr;

inline void zeromem(void *p,int size) {
  fillarray(reinterpret_cast<char*>(p),size,'\0');
}
template<class T> inline void zerorec(T &rec) {zeromem(&rec,sizeof(T));}

inline void setbit1(void *data,int bitindex) {
  int *dt=(int*)data;
  dt+=(bitindex>>5);
  *dt|=(1<<(bitindex&0x1F));
}
inline void setbit0(void *data,int bitindex) {
  int *dt=(int*)data;
  dt+=(bitindex>>5);
  *dt&=~(1<<(bitindex&0x1F));
}
inline bool getbit(void *data,int bitindex) {
  int *dt=(int*)data;
  dt+=(bitindex>>5);
  return *dt&(1<<(bitindex&0x1F));
}

template<class T,class S> inline void repairindex(T &index,S count) {
  if (index<0) index=0;
  if (index>=count) index=count-1;
}

}

#endif
