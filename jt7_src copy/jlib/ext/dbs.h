#ifndef dbsH
#define dbsH

namespace jlib {

/**
DBScript je jazyk, ktery produkuje textovy vystup(nejcasteji HTML soubor)
\c da na vystup c(jakykoli znak),_ preskoci bile znaky,
$[v] zapise hodnotu promenne v(zepta se potomka getvalue("v")),
#[p} udela prikaz p,#: je zacatek komentare,#. konec komentare
#f[a][b][c]: je zacatek funkce f s parametry a,b,c(bez parametru:#f:,parametry zatim neimplementovany),
#f. je konec funkce f
*/

struct DBSCRIPT {
  struct POS;
  void jump(POS pos) {if (pos.p) act=pos.p;}
  POS getpos() {return POS(act);}

  void parse(const char *input,STROUT &output) {init(input,output);_parse();}
protected:
  struct COM {
    static const int MAXPARS=0x10;
    char buf[0x100];
    int cnt;//pocet slov
    char *pars[MAXPARS];
    void init(const char *s,char divider);
    void init(const string &s,char divider) {init(s.c_str(),divider);}
    bool operator ==(const char *s) {return eqstr(pars[0],s);}
    char * operator [] (int index) {return pars[index];}
  };

  virtual string getvalue(const string &name)=0;
  virtual void docommand(const string &name)=0;

  void runfunc(const char *name);

  string jumpnextcom(bool behind);

  string readpar(char beg,char end);
  string readto(const char *end);
  void jumpbehind(const char *what);
  void jumpbefore(const char *what);
  STROUT *out;
  void _parse(const char *procname=NULL);
  void init(const char *input,STROUT &output) {buf=act=(char*)input;out=&output;}
  string unknownvar(const string &name) {return fmtstr("[Chyba:neznama promenna:%s]",name.c_str());}
  void fatalerror(const char *msg);
  void fatalerrorfmt(const char *msg,...);

  char *act;
private:
  char *buf;
  class POS {char *p;explicit POS(char *a) {p=a;}friend class DBSCRIPT;public:POS() {p=NULL;}};
  void jmpnestedremark();
};

bool rundbsonpart(const char *input,const char *begin,const char *end,DBSCRIPT &s,STROUT &output);
//pusti s na useku za begin koncicim end

}

#endif
