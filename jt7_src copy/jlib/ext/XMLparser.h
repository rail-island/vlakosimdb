#ifndef XMLparserH
#define XMLparserH

#include <com/strutils.h>

namespace jlib {

struct XMLERROR:XError {
//  void handle() {};
  XMLERROR(const char *amsg):XError(amsg) {}
  XMLERROR(string amsg):XError(amsg) {}
  XMLERROR() {}
};

struct XMLPARSER;

struct XMLPARSERINTF {
  virtual XMLPARSERINTF *begintag(XMLPARSER *sender,const char *name) {return NULL;}//pokud vrati NULL,nejsou analyzovany deti
  virtual void endtag(XMLPARSER *sender,const char *name) {}
  virtual void tagvalue(XMLPARSER *sender,const char *name,const char *value) {}

  virtual void beginthis(XMLPARSER *sender) {};
  virtual void endthis(XMLPARSER *sender) {};

};

struct XMLPARSER {
  bool wantvalue;

  char *xml;
  CHARANAL act;

  void parse(const char *axml,XMLPARSERINTF *intf);

  char *getproperty(const char *name);//muze vratiti NULL,vraci hodnotu promenne name
protected:
  char internalbuf[0x1000];//sem se ukladaji promenne pro getproperty
  void parselevel(XMLPARSERINTF *intf,const char *endtag);
  void parsetag(XMLPARSERINTF *intf);
  int level;
};

}

#endif
