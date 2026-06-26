#ifndef langH
#define langH

namespace jlib {

struct LANG {
  struct VAR {
    char *name,*value;
    VAR(char *aname,char *avalue) {name=aname;value=avalue;}
    ~VAR() {free(name);free(value);}
  };
  struct SECT {
    char *name;
    IObjArray<VAR> vars;
    SECT(char *&c);
    const char *getval(const char *name);//vrati vzdy platny retezec(ne NULL)
    const char *operator [](const char *name) {return getval(name);}
    SECT() {name="";}
  };
  IObjArray<SECT> sects;

  SECT emptysect;

  SECT &getsect(const char *name);
  SECT &operator [](const char *name) {return getsect(name);}//vrati prip.odkaz na empty

  LANG(const char *buf);
};

}

#endif
