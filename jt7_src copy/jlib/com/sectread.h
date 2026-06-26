#ifndef HSECTREAD
#define HSECTREAD

#include <com/com0.h>
#include <com/strms.h>

namespace jlib {

struct SECTREADBUFFER {
  char *buf;
  explicit SECTREADBUFFER(char *p) {buf=p;}
};

class SECTREAD:public LINET//umi radkove cist z jednotlivych sekci,
//zaroven slouzi jako INIFILE readonly(rychlejsi),vhodne pro velka data
//(ktera se vejdou do pameti)-soubor je cely v jednom bloku
{
public:
  char *filebuf,*linetact,*linetbeg;//pamet,kde je ulozen text souboru,
  //aktualni pozice pro LINET,pocatecni pozice pro LINET(nastavuje se initlinet
  char endsect[0x100];//nazev konce sekce pro cteni v LINET (napr. [/linet]);
  bool failed;//zda se nepovedlo otevrit soubor
protected:
  virtual void readlinetobuf();
public:
  void resetlinet();
  void closelinet();
  SECTREAD(char *file);
  SECTREAD(SECTREADBUFFER buf);
  SECTREAD() {closelinet();filebuf=NULL;failed=true;}
  ~SECTREAD() {free(filebuf);}
  bool initlinet(const char *sect);
  char *findline(const char *trimredline);

  char *findvar(const char *sect,const char *var);
  bool getvalue(const char *sect,const char *var,char *value);
  int getintdef(const char *sect,const char *var,int def);
  string getstrdef(const char *sect,const char *var,string def);
  SECTREAD *makesubsect(const char *sect);
};

}

#endif
