//---------------------------------------------------------------------------

#ifndef dfstrH
#define dfstrH

#include <com/com0.h>
#include <dir.h>


namespace jlib {


const char dirsep='\\';


void normalize_path(char *s);

struct FILESTR;
struct DIRSTR;

//predpoklad:v DIRSTR i FILESTR uz jsou spravna lomitka(dirsep)

//DIRSTR:adresar bez posledniho lomitka(pr:c:/jena,NE c:/jena/)
struct DIRSTR
{
  char s[0x100];
  operator char* () {return s;}
  void normal() {normalize_path(s);char *e=strend(s)-1;if (*e==dirsep) *e=0;}
  void assign(const FILESTR &file);
  explicit DIRSTR(const FILESTR &file) {assign(file);}
  explicit DIRSTR(const char *src) {strcpy(s,src);normal();}
  DIRSTR(const DIRSTR &dir) {strcpy(s,dir.s);}
  DIRSTR() {*s=0;}
  FILESTR operator()(const char *file) const;

  const char *c_str() {return s;}

  void operator =(const FILESTR &file) {assign(file);}
  void operator =(const char *src) {strcpy(s,src);normal();}

  ShortStr<64> getlastdir() const;//vrati posledni(nejvyssi) adresar z hiearchie
  void trimlastdir();//oreze dalsi adresar
  void adddirtopath(const DIRSTR &dir);//pokud dir je relativni cesta,spoji sebe a dir,
  //jinak se nastavi na dir
  bool hasdevice() const {return s[2]==':';}
  bool isabsolute() const {return s[0]==dirsep or (s[2]==':' and s[3]==dirsep);}
  bool isrel() const {return not isabsolute();}
};

inline DIRSTR operator -(const DIRSTR &src,int dircount) {
  DIRSTR res=src;while(dircount--) res.trimlastdir();return res;
}

inline DIRSTR operator +(const DIRSTR &dir1,const DIRSTR &dir2)
{DIRSTR res=dir1;res.adddirtopath(dir2);return res;}

struct FILESTR
{
  char s[0x100];
  operator char* () {return s;}
  void normal() {normalize_path(s);}
  DIRSTR getdir() const;//vrati adresar
  void setdir(const DIRSTR &dir);//nastavi adresar
  STR16 getext() const;//vrati priponu bez tecky
  void setext(const char *ext);
  void addext(const char *ext);

  explicit FILESTR(const char *src) {strcpy(s,src);normal();}
  FILESTR(const FILESTR &file) {strcpy(s,file.s);}
  FILESTR() {*s=0;}

  void getfname(char *buf) const;
  chrs getfname() const;

  void getnamewithoutext(char *buf) const;
  chrs getnamewithoutext() const;

  char *fileextbeg();//vrati ukazatel na tecku nebo na konec retezce
  bool hasext() const;
  bool testext(const char *ext) const;

  const char *c_str() {return s;}
  void operator =(const char *src) {strcpy(s,src);normal();}

  chrs loadline(int index);//precte indexatou radku z souboru(mez by byt textovy),oreze zprava

  ShortStr<> todirsep(char sep) const;

  DIRSTR directory() const {return getdir();}//vrati adresar
  FILESTR filename();//vrati soubor bez adresera
  FILESTR purefilename();//vrati soubor bez adresare a bez pripony

  FILESTR Setext(char *ext) {FILESTR res;strcpy(res.s,s);res.setext(ext);return res;}

  chrs loadtext();//nacte jako textovy soubor
};

DIRSTR getworkdir();//vrati cestu i s diskem

}

//---------------------------------------------------------------------------
#endif
