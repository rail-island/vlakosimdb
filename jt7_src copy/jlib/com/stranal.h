#ifndef stranalH
#define stranalH
#include <com/com0.h>

namespace jlib {

struct BESTR{
char *beg,*aend;
};

class EAnal {
public:
  char msg[0x100];
  int pos;
  EAnal(char *amsg,int apos) {strcpy(msg,amsg);pos=apos;}
  EAnal(int apos,char *format,...);
  const char *what() {return msg;}
};

inline void nojmpwhite(char *&) {}
inline void stdjmpwhite(char *&c)//preskoci mezery a C++ komentare
{ while(*c && (unsigned char)*c<=' ') c++;}

void cjmpwhite(char *&c);//preskoci mezery a C++ komentare

//template <void (*jmpwhite_par)(char *&)>
class STRANAL {
static const char QUOTE='\"';
public:
  struct QSTR {
    char *buf;
    explicit QSTR(char *abuf) {buf=abuf;}
  };
  //to same co char*,ale aby se odlisilo,kdy se ma nacist retezec a kdy idf
  //QSTR:retezec,char*:identifikator
  struct IDFSTR {char *buf;explicit IDFSTR(char *abuf) {buf=abuf;}};


  virtual void jmpwhite_par(char *&)=0;
  char *act,*buf;
  //act je aktualni pozice

  STRANAL(const char *buf) {this->buf=(char*)buf;act=(char*)buf;}
  STRANAL(string &buf) {this->buf=(char*)buf.c_str();act=(char*)buf.c_str();}
  STRANAL() {buf=NULL;act=NULL;}
  void init(const char *c) {buf=(char*)c;act=(char*)c;}
  char operator *() {return *act;}
  void operator ++(int) {if (*act) act++;}

  //operatory:
  //>>(T &val):precte hodnotu typu T do val,posune ukazovatko za prectenou,
  //pokud neni na rade hodnota typu T,chyba
  //<<(T val):precte hodnotu typu T,musi se shodovat s val,(jinak chyba),posune za prectenou
  //bool ==(T val):otestuje, zda nasledujici hodnota je typu T a je val,pokud ano,
  //nastavi ukazovatko za prectenou a vrati true,pokud ne,vrati false a s ukazovatkem nehne
  //bool (T &val):precte hodnotu do val,pokud je typu T,vrati true a posune ukazovatko,
  //jinak nic+vrati false

  //read:
  STRANAL& operator >>(IDFSTR idf);
  STRANAL& operator >>(QSTR s);
  STRANAL& operator >>(int &val);
  STRANAL& operator >>(float &val);
  STRANAL& operator >>(double &val);

  //check:
  STRANAL& operator <<(const IDFSTR idf);
  STRANAL& operator <<(char ch);
  STRANAL& operator <<(const char *s);

  //test:
  bool operator ==(const IDFSTR idf);
  bool operator ==(char ch);
  bool operator ==(const char *s);

  //ask:

  //!=:opak ==,ale pri uspechu ani neuspechu nemeni act
  bool operator !=(char ch);
  bool operator !=(const char *s);

  //ostatni:
  void jmpwhite() {jmpwhite_par(act);}//preskoci bile znaky,akomentare atd...(dle parametru sablony)
  void jmpwhite(char *&c) {jmpwhite_par(c);}
  void jmpspaces() {stdjmpwhite(act);}//preskoci mezery

  //see fce-nehybou s act,vraci,zda byl precten dany typ,vrati NULL pri neuspechu
  //ukazatel na dalsi cteni pri uspechu
  char *seeidf(char *s);
  char *seestr(char *s);
  char *seeint(int &val);
  char *seefloat(float &val);
  char *seedouble(double &val);

  //check fce-to same co <<,jen nevolaji vyjumku, ale vraci bool
  bool checkidf(const char *idf);//netypicka
  //idf nemusi byt jen identifikator-testuje se tak,ze se preskoci mezery a pak se musi rovnat
  //idf ze zacatkem retezce,skoci se za idf+za idf v act musi byt konec identifikatoru
  //pokud vrati false,act ukazuje na zacatek spatneho identifikatoru

  //test-fce:jako ==
  char *testidf(const char *idf);//vrati ukazatel za konec idf v act pri uspechu,jinak NULL

  bool aredata();//vrati,zda je v buferu jeste neco krome bilych znaku a komentaru
  int pos() {return act-buf;}

  bool begwith(const char *s);//zjisti,zda act zacina s,pokud ano,posune act za s
  //nepreskakuje bile znaky!!!
  bool ubegwith(const char *s);//to same,ale nerozlisuje male a velka pismena

  STRANAL &readto(char ch,char *out);//precte az do ch,ulozi do buf,pokud nenarazi na ch,throw EAnal
  //nevola jmpwhite!!!
  STRANAL &readto(char ch,STROUT &f) throw();//precte az do ch,ulozi do buf,nevola jmpwhite!!!

  STRANAL &cppreadto(char ch,char *out);//precte az do ch,ulozi do buf,pokud nenarazi na ch,throw EAnal
  //znak \neco zapise do out,nezjistuje,zda neco==ch

  STRANAL &readto(const char *end,char *out);//precte az do end,ulozi do buf,pokud nenarazi na ch,throw EAnal

  STRANAL &skipto(char ch);

  STRANAL &readtochar(const char *endchars,char *out);//precte az do jednoho ze znaku v end,ulozi do out
  //koncici znak NEpreskoci


  void readline(char *buf);//precte radku do buf,skoci na zacatek dalsi,trimr(buf)
  string readline();
  void readline(STROUT &f);//precte radku,zapise trimr do f
  void skipline();//preskoci na zacatek dalsi radky

  bool findtext(const char *text);//skoci na konec textu text,vrati,zda text byl nalezen(vola strstr)
};

/*template class STRANAL<cjmpwhite>;
typedef STRANAL<cjmpwhite> CSTRANAL;

template class STRANAL<stdjmpwhite>;
typedef STRANAL<cjmpwhite> STDSTRANAL;

template class STRANAL<nojmpwhite>;
typedef STRANAL<nojmpwhite> NOJMPSTRANAL;*/
#define ANALMACRO(NAME,JMPPROC) \
struct NAME:STRANAL {\
  void jmpwhite_par(char *&c) {JMPPROC(c);}\
  NAME(const char *buf):STRANAL(buf) {}\
  NAME(string &buf):STRANAL(buf) {}\
  NAME() {}\
}\

ANALMACRO(CSTRANAL,cjmpwhite);
ANALMACRO(NOJMPSTRANAL,nojmpwhite);
ANALMACRO(STDSTRANAL,stdjmpwhite);

/*struct CSTRANAL:STRANAL {
  void jmpwhite_par(char *&c) {cjmpwhite(c);}
  CSTRANAL(const char *buf):STRANAL(buf) {}
};
struct NOJMPSTRANAL:STRANAL {
  void jmpwhite_par(char *&) {}
  NOJMPSTRANAL(const char *buf):STRANAL(buf) {}
};
struct STDSTRANAL:STRANAL {
  void jmpwhite_par(char *&c) {stdjmpwhite(c);}
  STDSTRANAL(const char *buf):STRANAL(buf) {}
};*/

inline bool isbegidfchar(unsigned char what)
{ return what=='_' || (what>='a' && what<='z') || (what>='A' && what<='Z');}

inline bool isidfchar(unsigned char what)
{ return what=='_' || (what>='0' && what<='9') || (what>='a' && what<='z') || (what>='A' && what<='Z');}


}

#endif
