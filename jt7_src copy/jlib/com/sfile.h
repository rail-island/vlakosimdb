#ifndef sfileH
#define sfileH
#include <com/com0.h>

namespace jlib {

struct sFile {
//class sFile {//trida obalujici retezec souboru nebo adresare
//  char s[0x100];
  string s;
  const char *fileextbeg() const;//vrati ukazatel na tecku nebo na konec retezce
//public:
  static const char dirsep='\\';
  static inline void normalize(char *c) {for(;*c;c++) if (*c=='\\'||*c=='/') *c=dirsep;}

  operator const char *() const {return s.c_str();}
  operator string() const {return s;}
  operator STRING() const {return s;}
//  template<typename T> operator T() const {return T(s);}
  const char *c_str() const {return s.c_str();}

  void init(const char *src);

  sFile(const char *src) {init(src);}
//  template<typename T> sFile(const T &src) {init(src.c_str());}
  sFile(const string &src) {init(src.c_str());}
  sFile() {}
  void operator =(const char *src) {init(src);}
//  template<typename T> void operator =(const T &src) {init(src.c_str());}

  string getext() const;
  sFile makeext(const char *ext) const;
  void setext(const char *ext);
  bool hasext() const {return *fileextbeg();}
  bool testext(const char *ext) const;

  sFile getdir() const;
  void operator --(int);//urizne posledni adresar

  sFile getfile() const;

  void add(const char *file);

  void docommand(const char *com);//pusti prikaz nad touto instanci
  //prikazy:
  //e.pri : nastavi priponu pri
  //d : vrati rodicovsky adresar
  //f : vrati jmeno souboru
  void docommands(const char *coms);//pusti prikazy nad touto instanci
  //prikazy jsou oddelene strednikem(napr coms=="f;e.exe")

  string loadline(int index);
  string loadtext();
  bool hasfile(string file);

  bool hasdevice() const {return s[1]==':';}
  bool isabsolute() const {return _isabsolute(s.c_str());}
  bool isrelative() const {return !isabsolute();}
  static bool _isabsolute(const char *s);
  sFile relto(sFile dir) const;//vrati relativni cestu k dir
  struct NO_INIT {};
  sFile(NO_INIT) {}

  bool isfile();
  bool isdir();
  void mkdirs();
  void rmdir();
  bool mkdir();
};

inline sFile operator /(const sFile &dir,const char *file) {
  sFile res=dir;
  res.add(file);
  return res;
}

inline sFile operator /(const sFile &dir,const string &file) {
  sFile res=dir;
  res.add(file.c_str());
  return res;
}

/*
template<class T> inline sFile operator /(const sFile &dir,const T &file) {
  sFile res=dir;
  res.add(file.c_str());
  return res;
}
*/

inline sFile operator %(const sFile &dir,const char *ext) {
  sFile res=dir;
  res.setext(ext);
  return res;
}

inline sFile operator *(const sFile &dir,const char *commands) {
  sFile res=dir;
  res.docommands(commands);
  return res;
}

extern sFile basedir;



}

#endif
