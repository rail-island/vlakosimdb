#ifndef charmixerH
#define charmixerH
//---------------------------------------------------------------------------

namespace jlib {

class CMARRAY;
struct CMELEM {
  IObjArray<CMARRAY> arrays;
  VarList vars;
  virtual CMARRAY *findarray(string name);
};
struct CMARRAY {
  string name;
  IObjArray<CMELEM) items;
  bool match(const char *aname) {return name==aname;}
  CMARRAY(string aname,CMELEM *aelem) {
    name=aname;aelem->arrays.add(this);
  }
};
CMARRAY *CMELEM::findarray(string name) {
  int idx=arrays.searchtype(name.c_str());
  if (idx>=0) return arrays[idx];
  return NULL;
}

/*
CHARMIXER je skriptovy jazyk umoznujici na zaklade danych dat (pomoci zanoreni
objektu type CMELEM a CMARRAY) vytvorit textovy vystup
*/

typedef VarRecord CMVAR;

struct CMERROR {
  string msg;
  CMERROR(string amsg) {msg=amsg;}
};

struct CHARMIXER {
  void runscript(const char *file,CMELEM *base,bool catcherrors);
  string (*customfuncs)(void *customparam,string fn,Strings &params);
  void *customparam;
  CHARMIXER() {customfuncs=NULL;customparam=NULL;}
private:
  struct EditAborted {};
  sFile inputdir,outputdir,prgdir;
  struct AREC;struct MACRO;
  friend class CHARMIXER::AREC;

  struct STATE {
    char *act,*end;
    MEMSTROUT *out;
    VarList *locals;
    string actfile;
  };
  STATE getstate() {
    STATE res;
    res.act=act;res.end=end;res.out=out;res.locals=locals;res.actfile=actfile;
    return res;
  }
  void setstate(STATE st) {
    act=st.act;end=st.end;out=st.out;locals=st.locals;actfile=st.actfile;
  }

  VarList loadedfiles;
  char *act,*end;
  string actfile;
  MEMSTROUT *out;//aktualni vystup
  IObjArray<MACRO> macros;
  VarList globals,*locals;

  struct AREC {
    CMARRAY *array;
    int index;
    char *beg;
    AREC(CMARRAY *array,CHARMIXER *parent) {
      this->array=array;index=0;beg=parent->getpos();
    }
    AREC() {}
  };
  struct MACRO {
    string name;
    Strings params;
    string file;
    char *beg,*end;
    bool match(const char *aname) {return name==aname;}
  };
  struct FEREC {
    Strings items;
    int index;
    char *beg;
    FEREC() {index=0;beg=NULL;}
  };

  CMELEM *elemstack[0x100],**elemw;
  AREC arraystack[0x100],*arrayw;
  char *whilestack[0x100],**whilew;
  IObjArray<FEREC> festack;

  void jump(char *pos) {if (pos) act=pos;}
  char * getpos() {return act;}

  void run();
  void jumpto(char end);
  inline bool data() {return act<end;}
  inline bool data(int dist) {return act<end-dist;}
  char * loadfile(string file);
  void jmpnestedremark();
  string readbrackets(char beg,char end);
  string readto(const char *end,bool jumplastchar);
  void readlangstrs(Strings &strs);
  void fatalerror(const char *msg);
  void fatalerrorfmt(const char *msg,...);
  CMVAR *getvar(string name);
  string getvalue(string name);
  void runcom(char *commandbegin);

  void runifdef(Strings &params,bool yes);
  void runfor(Strings &params);
  void runendfor();
  void runforeach(Strings &params);
  void runendforeach();
  void runwhile(char *combegin,Strings &params);
  void runendwhile();
  void runif(Strings &params);
  void jumptocondition();

  bool isdefined(string what);
  bool structurejump(const char *endcom,bool stoponelse);//vrati, zda naslo elif
  void jumptonextcom();

  inline bool isremark() {return act[0]=='#' and act[1]==':' and data(1);}
  inline bool iscommand() {return act[0]=='#' and act[1]=='[' and data(1);}

  string readcomandparams(Strings &params);
  string unknownvar(string name) {return fmtstr("[Chyba:neznama promenna:%s]",name.c_str());}
  string readexpr(const char *end);
  string readandrunfunc(bool ismacro);
  string call(string fn,Strings &params);

  void runinclude(Strings &params);
  void rundefine(Strings &params);
  string callmacro(string name,Strings &params);

  bool stob(string s);
  string btos(bool b) {return b?"true":"false";}

  string calland(Strings &pars);
  string callor(Strings &pars);
  string callsum(Strings &pars);
  string callmul(Strings &pars);

  string ftos(double value);
  string hasitems(string var);

  void checkloadedfile(string name,string value);

  CMARRAY *findarray(string name);
  bool doedit(VarList *vars,Strings &pars);
};


}



#endif
