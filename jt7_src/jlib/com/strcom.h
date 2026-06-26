#ifndef HCOMSTR
#define HCOMSTR

namespace jlib {

inline bool chisspace(char ch) {return uchar(ch)<=uchar(' ') && (ch);}
//#define chisspace(ch_arg) ((uchar)(ch_arg)<=(uchar)' ' && (ch_arg))
//#define isascii(ch_arg) ((ch_arg)>=' ' and (ch_arg)<128)
inline char *strend(const char *c)
{
  while (*c) c++;return (char*)c;
}

inline char *strnew(const char *src)
{
  char *res=(char*)malloc(strlen(src)+1);
  strcpy(res,src);
  return res;
}

inline void trimr(char *str)
{
  char *end=strend(str)-1;
  while (chisspace(*end) && end>=str) *end--=0;
}

inline void triml(char *str)
{
  char *c=str;
  while (chisspace(*c)) *c++;
  memmove(str,c,strlen(c)+1);
}

inline void trim(char *str)
{
  trimr(str);triml(str);
}

inline const char *gettriml(const char *str) {
  const char *c=str;
  while (isspace(*c)) c++;
  return c;
}

inline char *gettriml(char *str) {
  char *c=str;
  while (isspace(*c)) c++;
  return c;
}

inline char *gettrim(char *str)
{
  trimr(str);return gettriml(str);
}

bool eqstr(register const char *s1,register const char *s2);
inline bool trimeqstr(char *s1,char *s2) { return eqstr(gettrim(s1),gettrim(s2));}
inline bool upeqstr(const char *s1,const char *s2)
{
  for(;*s1 && *s2;s1++,s2++) if (toupper(*s1)!=toupper(*s2)) return 0;
  return !*s1 && !*s2;
}
inline bool uptrimeqstr(char *s1,char *s2) { return upeqstr(gettrim(s1),gettrim(s2));}

void strdelete(char *s,int index,int count);
void strinsert(char *s,const char *substr,int index);

inline bool seek0(const char *c) {//vrati,zda za mezerami je uz konec retezce
  while ((uchar)*c<=' ' && *c) c++;
  return *c==0;
}

inline bool seekch(const char *c,char ch) {
//vrati,zda za mezerami je znak ch(znak ch muze byt i mezera)
  while ((uchar)*c<=' ' && *c && *c!=ch) c++;
  return *c==ch;
}

inline int length(const char *s) {if (s==NULL) return 0;else return strlen(s);}
inline char *copystr(char *dest,const char *src)
{if (src==NULL) {if (dest!=NULL) *dest=0;return dest;} else return strcpy(dest,src);}

//zkopiruje,vrati ukazatel na konec dest
inline char *estrcpy(register char *dest,register char *src)
{while (*src) *dest++=*src++;*dest=0;return dest;}

inline char *strnchr(char *s,int c,int n)
{
  if (n<0) return NULL;
  while (n && *s!=c) s++,n--;
  if (n) return s;else return NULL;
}

inline char *beginofthisline(register char *c)//posune na zac.tohoto radku,
//!!!predchozi radek MUSI koncit \n
{
  while (*c!=10) c--;
  return c+1;
}

inline void addchtostr(char *s,int c)
{
  if (s==NULL) return;
  register char *e=strend(s);
  *e=c;e[1]=0;
}

inline int teststr(const char *c,const char *cmpstr)
{
  while (*cmpstr  && *c) if (*c++!=*cmpstr++) return 0;
  return !*cmpstr;
}
inline int upteststr(const char *c,const char *cmpstr)
{
  while (*cmpstr  && *c) if (toupper(*c++)!=toupper(*cmpstr++)) return 0;
  return !*cmpstr;
}

inline int freestreq(const char *s1,const char *s2)
{
  while (*s1 && *s2)
  {
    while ((uchar)*s1<=' ' && *s1) s1++;
    while ((uchar)*s2<=' ' && *s2) s2++;
    if (!*s1 || !*s2 ) break;
    if (toupper(*s1++)!=toupper(*s2++)) return 0;
  }
  if (!*s1 && !*s2) return 1;
  return 0;
}

int freestrcomp(const char *s1,const char *s2);
int insstrcomp(const char *s1,const char *s2);


inline char *challoc(int size)
{
  char *mem=(char*)malloc(size);
  if (mem==NULL) xthrow(XNoMemory);
  return mem;
}

inline char *chrealloc(char *ptr0,int size)
{
  char *mem=(char*)realloc(ptr0,size);
  if (mem==NULL) xthrow(XNoMemory);
  return mem;
}
inline void strtoascii(char *s,char noasciichar='_')
{
  while (*s)
  {
    if (!isascii(*s)) *s=noasciichar;
    s++;
  }
}

inline char *emptystr() {char *res=challoc(1);*res=0;return res;}

void strins(char *dest,const char *substr);
void strdel(char *delbeg,char *delend);

//#define jmpspaces(ch) {while((uchar)*ch<=' ' && *ch) ch++;}
inline void jmpspaces(const char *&c) {while (*c && (unsigned char)*c<=' ') c++;}
inline void jmpspacesnolineends(const char *&c) {while (*c && (unsigned char)*c<=' ' and *c!='\n') c++;}


inline bool strbegcmp(const char *begstr,const char *longstr)//vrati,zda longstr zacina retezcem begstr
{
  while(*begstr) if (*longstr++!=*begstr++) return false;
  return true;
}

bool strendcmp(const char *endstr,const char *longstr);

inline bool insstrbegcmp(const char *begstr,const char *longstr)//vrati,zda longstr zacina retezcem begstr
//bez ohledu na velka a mala
{
  while(*begstr) if (toupper(*longstr++)!=toupper(*begstr++)) return false;
  return true;
}

inline bool mapstrbegcmp(const char *begstr,const char *longstr,CharMap map)
{
  while(*begstr) if (map[*longstr++]!=map[*begstr++]) return false;
  return true;
}


inline void mapstrcpy(char *dest,const char *src,const char *map)
{
  register char *d=dest;
  for(register const char *c=src;*c;c++,d++) *d=map[(uchar)*c];*d=0;
}

//struct Sub/StrType {char operator *();vrati tkuualni znak;
//void operator ++(int);preskoci na dalsi platny znak nebo na konec
//zpocatku musi byt nastaveno na prvni platny znak
//retezec je ukonceny 0 (*StrType vrati na konci 0)

template<class SubType,class StrType> static
bool freemapsubstr_cl(SubType &sub,StrType &s,CharMap map) {
  for(;*s;s++) {
    StrType x=s;
    SubType y=sub;
    for(;*x and *y;x++,y++) if (map[*x]!=map[*y]) break;
    if (not *y) return true;
  }
  return false;
}

template<void (*jmp)(const char *&)> struct JmpString {
  const char *act;
  JmpString(const char *s) {act=s;jmp(act);}
  void operator ++(int) {jmp(act);act++;jmp(act);}
  char operator *() {return *act;}
};

template struct JmpString<jmpspaces>;
typedef JmpString<jmpspaces> StdJmpString;

template<void (*jmpsub)(const char *&),void (*jmp)(const char *&)>
static char *freemapsubstr_fn(const char *sub,const char *s,CharMap map) {
  JmpString<jmpsub> qsub(sub);
  JmpString<jmp> qs(s);
  if (freemapsubstr_cl (qsub,qs,map) ) return (char*)qs.act;
  return NULL;
}

inline char *freemapsubstr(const char *sub,const char *s,CharMap map) {
//hledani podretezce, ignoruji se mezery v s i sub
  return freemapsubstr_fn<jmpspaces,jmpspaces> (sub,s,map);
}

char *mapsubstr(const char *sub,const char *s,CharMap map);
bool mapeqstr(const char *s1,const char *s2,CharMap map);

bool freemapeqstr(const char *s1,const char *s2,CharMap map);
//porovnavani s tim, ze se ignoruji mezery v s1 i s2

string readidf(pchar &c);
string readnum(pchar &c);

inline char *readline(const char *buf) {//precte radku az do konce radku,vrati uk.na dalsi radku
//pripadne uk. na konec retezce
  while (*buf && *buf!='\n') buf++;
  if (*buf) return (char*)(buf+1);else return (char*)buf;
}

inline int strecpy(char *dest,const char *src,const char *srcafterend) {
  if (srcafterend==NULL) { copystr(dest,src);return length(src); }
  if (src==NULL) {if (dest!=NULL) *dest=0; return 0;}
  register int len=srcafterend-src;
  assert(len>=0);
  memmove(dest,src,len);
  dest[len]=0;
  return len;
}

char *readtoch(register char *&s,register char *out,char end);

inline void readline(char *&c,char *buf) {
  char *w=buf;
  char *act=c;
  for(;*act and *act!='\n';act++,w++) *w=*act;
  *w=0;
  if (*act) act++;trimr(buf);
  c=act;
}

inline string lowercase(const char *s) {
  string res=s;
  for(char *c=(char*)res.c_str();*c;c++) *c=tolower(*c);
  return res;
}
inline string lowercase(string s) {return lowercase(s.c_str());}

inline string uppercase(const char *s) {
  string res=s;
  for(char *c=(char*)res.c_str();*c;c++) *c=toupper(*c);
  return res;
}
inline string uppercase(string s) {return uppercase(s.c_str());}

}
#endif
