#ifndef sscmH
#define sscmH

#include "ssdefs.h"
#include <gram/gmanal.h>

namespace ss {
using namespace gram;

//struct Structure {
//  const char *type;
//};

struct CharMixer {
  string runstr(const char *s);
  void runfile(const char *file);

  CharMixer(Module *module):module(module) {out=NULL;writewhites=true;iserror=false;}

  Analyser a;
  MEMSTROUT *out;
  Module *module;
  bool writewhites;
  VarList loadedfiles;
  sFile inputdir;
//  Structure sstack[0x100],*sw;
  MEMSTROUT stderrout;
  bool iserror;
private:
  string actfile;
  void writeerrtext(const char *prgtext,AnalError &e);
  bool iscommand() {return *a=='#' && a[1]=='[';}
  void runcommand();
  void jumpto(const char *endname,bool jmplastcom,bool stoponifs);
//  void runto(const char *endname,bool jmplastcom);
  void jumpcommand();
  void runrepeat();
  void runif();
  void runcoms(const char *endcom=NULL);
  void runinclude(string file);
//  bool isendcommand() {return a.begins("#[endfor") || a.begins("#[endwhile") || a.begins("#[endif") || a.begins("#[until") || a.begins("#[else");}
  void doifblock(bool reallydo) {if (reallydo) runcoms("");else jumpto("",false,true);}
  bool evalcond();
  string evalexpr();
  void saferun(const char *s,const char *end);
  const char *loadfile(string file);
  friend class MacDef;
};

struct MacDef:SubDef {
  Strings args;
  Analyser body;
  CharMixer *cm;
  Module *module;
  string file;

  void load(Analyser &a);
  string call(Strings &pars);
  MacDef(CharMixer *cm):cm(cm),module(cm->module) {}
};

}

#endif
