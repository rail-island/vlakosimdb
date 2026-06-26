#ifndef linetH
#define linetH

namespace jlib {

//pozor,LINET nacita radky bez koncovych bilych znaku!!!
class LINET//trida pro radkove CTENI textu
{
public:
  static const int staticbufsize=0x1000;
  bool lf() {readlinetobuf();return !eof;};//odradkuje a nacte dalsi radku
  //vrati,zda neco je v buf

  bool eof;//konec souboru?(=uz neni v buf radka)
  char linetbuf[staticbufsize];
  LINET() {eof=true;*linetbuf=0;}

  bool lineis(char *s) {return eqstr(s,linetbuf);}
  bool lineupis(char *s) {return upeqstr(s,linetbuf);}
  bool linetrimis(char *s) {return trimeqstr(s,linetbuf);};
  bool lineuptrimis(char *s) {return uptrimeqstr(s,linetbuf);};

  bool operator == (char *s) {return lineis(s);}
  bool operator ^= (char *s) {return lineupis(s);}
  bool operator -= (char *s) {return linetrimis(s);}
  bool operator *= (char *s) {return lineuptrimis(s);}

  bool operator != (char *s) {return !linetrimis(s);}

  bool controltype(char *filetype);
protected:

  virtual void readlinetobuf() {};//precte radku do buf,odradkuje v vstupnim souboru
};

class TEXTFILE:public LINET
{
public:
  FILE *f;
  TEXTFILE(char *filename,char *filetype=NULL);
  virtual void readlinetobuf();
  ~TEXTFILE() {if (f) fclose(f);}
};


}

//---------------------------------------------------------------------------
#endif
