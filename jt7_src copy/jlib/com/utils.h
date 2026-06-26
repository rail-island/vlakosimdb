#ifndef utilsH
#define utilsH
//---------------------------------------------------------------------------

namespace jlib {

struct JException {
  virtual string getmsg()=0;
  void show();
};

struct JError:JException {
  string msg;
  JError(string msg):msg(msg) {}
  string getmsg() {return msg;}
};

int fsystem(char *format,...);
char *loadbinfile(const char *file,int *filesize=NULL,const char *openmode="rb");
string loadtext(const char *file,const char *openmode="r");
char *loadbinfile(FILE *fr,int *filesize=NULL);
bool savebinfile(const char *file,const char *str,const char *openmode="wb");
inline bool savetextfile(const char *file,const char *str) {
  return savebinfile(file,str,"w");
}

void gzsavetext(const char *file,const char *text,int comprlevel=6);//text ulozi primo(neupravcuje #10,#13)
string gzloadtext(const char *file);

template <int BufSize=0x100> struct ShortStr
{
  char s[BufSize];
  operator char* () {return s;}
  ShortStr(const char *src) {strcpy(s,src);}
  template<int BufSize2> ShortStr(const ShortStr<BufSize2> &src) {strcpy(s,src.s);}
  ShortStr() {*s=0;}
  const char *c_str() const {return s;};

  template<int BufSize2> void operator =(const ShortStr<BufSize2> &src) {strcpy(s,src.s);}

  template<class T> operator T() {return T(s);}
};


template class ShortStr<0x10>;
typedef ShortStr<0x10> STR16;

void eclear();//vycisti soubor bspth+stderr
void eputs(const char *s);//zapise retezec s do bspth+stderr
void eprintf(const char *format,...);//zapise naformatovany retezec do bspth+stderr
inline void eassert(bool condition,char *msg="")
{if (not condition) { eputs("eassert\n");eputs(msg);exit(EASSERT);}}

inline void pse(const char *s) { fprintf(stderr,"%s\n",s); }

void dosnormalfile(char *file);//prevede soubor pro aktualni OS
void nicefile(char *file);//prevede soubor na mala pismena,dopredna lomitka
void backslash(char *file);
void foreslash(char *file);

FILE *fileopen(char *file,char *mode);//pouzivat pro prenositelnost
int fileremove(char *file);//pouzivat pro prenositelnost
//(nazev souboru upravuje dla existence BACKSHASH v <com/comdefs.h>)

int makedir(char *dir);
void filepath(char *file,char *path);

int getfilesize(FILE *f);
int getfilesize(const char *file);

inline int fwriteln(char *s,FILE *fw) {
  int i=fputs(s,fw);fputs("\n",fw);
  return i;
}


inline int cmpint(int i1,int i2)
{
  if (i1<i2) return -1;
  if (i1>i2) return 1;
  return 0;
}

//polni funkce
inline void aradd(void *buf,void *elem,int &count,int size=sizeof(void*))
{
  memmove((char*)buf+size*count,elem,size);
  count++;
}
void arinsert(int index,void *buf,void *elem,int &count,int size=sizeof(void*));
void arerase(int index,void *buf,int &count,int size=sizeof(void*));

inline int freadint(FILE *fr)
{
  char buf[0x40];
  fgets(buf,0x40,fr);
  return atoi(buf);
}
inline string itos(int i)
{
  char s[0x20];
  sprintf(s,"%d",i);
  return string(s);
}
string ftos(double f);
string fmtstr(const char *format,...);
char *allocfmt(const char *format,...);

bool testfileext(const char *file,const char *ext);

char *writefixstr(char *dest,char *src,char len);
//oreze retezec nebo prida mezery tak, aby mel delku len
//vrati ukazatel na konec dest(na dest+len)

void fixfmt(char *dest,...);
//zformatuje do dest, nasleduje vzdy retezec(char*), int len,
//ukonceni NULL
//napr fixfmt(dest,"abcd",2,"ef",4,NULL),v dest bude "ab  ef";

void removedir(char *dir);
string makectext(const char *text);//vypusti vsechny znaky chr(13)

template<class T> inline T suma(int from,int to,T array[],T zero=0) {
  T res=zero;
  for(int i=from;i<=to;i++) res+=array[i];
  return res;
}

int xorhash(const void *data,int size);

void fmterror(const char *src,const char *errorpos,int &line,int &col,string &errorline,bool propertional=false);
//z informace o znakove pozici chyby(src je zacatek stringu v kterem je chyba)
//spocita radek,sloupec, error line je string typu:
//  void showModal(char *p1;char *p2);
//-------------------------^

inline string fmterrorline(const char *src,const char *errorpos,bool propertional=false) {
  string res;int line,col;fmterror(src,errorpos,line,col,res,propertional);return res;
}

struct CommonParseError:JException {
  int line,col;
  int pos;
  string text;
  string file;
  string msg;
  string context,monocontext;
  void init(string atext,string amsg,int apos,string afile="");
  string preparetext(bool proportional=true);
  string getmsg() {return preparetext(true);} 
};

}

#endif
