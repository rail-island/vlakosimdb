//---------------------------------------------------------------------------

#ifndef inifileHeader
#define inifileHeader

#include <com/strlist.h>
#include <ext/backinfo.h>

namespace jlib {

#include "inifile.i"

struct INIFILE:STRLIST,INIFILEI {
public:
  char filename[0x100];
  void init() {*filename=0;}
  INIFILE(const char *file="");//cesta je vztazena k bspth
  ~INIFILE() { if (*filename) save(filename); }
  int findsect(const char *sect);//vrati index prvniho radku sekce,nebo -1,kdyz neex.
  //sect je retezec bez []
  bool issect(const char *sect) {return findsect(sect)>=0;}
  int findinivar(const char *sect,const char *var);//vrati index radku,na kterem je promenna var
  //v sekci sect nebo -1,kdyz v teto sekci neni
  char *getvalue(const char *sect,const char *var);//vrati ukazatel na hodnotu promenne nebo NULL,kdyz neex.
  int setvalue(const char *sect,const char *var,const char *value);//nastavi promennou var v sekci sect
  //vrati cislo radku,kam zapsalo
  int setvalue(const char *sect,const char *var,int value);
  int setvalue(const char *sect,const char *var,double value);
  int getintdef(const char *sect,const char *var,int def);
  double getrealdef(const char *sect,const char *var,double def);
  chrs getstrdef(const char *sect,const char *var,chrs def);
  bool readsection(const char *sect,STRLIST &lines);//precte sekce sect,vrati,zda existuje
  bool erasesection(const char *sect);//vymaze sekci,vrati,zda uspesne
  void writesection(const char *sect,STRLIST &lines);//zapise lines do sekce sect,kterou nejdrive vymaze

  string readsection(const char *sect);
  void writesection(const char *sect,const char *str);

  void writehexsect(const char *sect,const void *buf,int size);

  void writenumssect(const char *sect,int *nums,int count);
  int readnumssect(const char *sect,int *nums,int maxcount);
  void readintsect(const char *sect,TCntVec<int> &res);//vycisti res,vrati tam cisla

protected:
  virtual char * STDCALL getValue(const char *sect,const char *var) {return getvalue(sect,var);}
  virtual void STDCALL setValue(const char *sect,const char *var,const char *value) {setvalue(sect,var,value);}
};

extern INIFILE *prgini;

}

//---------------------------------------------------------------------------
#endif
