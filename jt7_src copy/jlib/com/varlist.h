#ifndef varlistH
#define varlistH

namespace jlib {

class BINREAD;class BINWRITE;

struct PrivateVarListRec {
  char *name,*value;
  PrivateVarListRec(char *aname,char *avalue) {name=aname;value=avalue;}
};

struct VARLISTRECORD {
  char *name,*value;
  VARLISTRECORD(const PrivateVarListRec &rec) {name=rec.name;value=rec.value;}
  VARLISTRECORD(const VARLISTRECORD &rec) {name=strnew(rec.name);value=strnew(rec.value);}
  ~VARLISTRECORD() {free(name);free(value);}
  VARLISTRECORD() {name=NULL;value=NULL;}
  void setvalue(const char *avalue) {free(value);value=strnew(avalue);trimr(value);}
  bool multiline();
  void save(STROUT &f);
};

struct VARLIST:TObjVec<VARLISTRECORD> {
  void setvar(const char *name,const char *value);
  void _addvar(const char *name,const char *value);
  int findvari(const char *name);
  const char *findvar(const char *name);
  const char *surefindvar(const char *name);//pokud nenalezne,vyhodi vyjimku
  void save(BINWRITE &fw);
  void load(BINREAD &fr);
  void initfromqstr(const char *str);//inicializuje z retezce typu var1="koko" var2="bebe" ...
};

}

#endif
