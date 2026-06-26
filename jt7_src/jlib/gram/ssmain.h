#ifndef ssmainH
#define ssmainH

#include <gram/gmanal.h>
#include "ssdefs.h"

namespace ss {
using namespace gram;

class Runtime;

struct Runtime {
  Analyser a;
  Module *module;

  string evalexpr();
  void run(char endch=0);
  Runtime(Module *module):module(module) {}

  string ssexprhi();
  string ssexprdefer();
  string ssexprmul();
  string ssexpradd();
  string ssexprrel();
  string ssexprlog();
  string readidf();
  void runcom();
  void assignvar(string name,string value);
  void jumpblock();
  void runcoms();
  void runblock();
  void runfor();
  bool evalcond(bool brackets=true);
  void doblock(bool run) {if (run) runblock();else jumpblock();}
  friend class DefLocals;
  friend class FnDef;
};

struct FnDef:SubDef {
  Strings args;
  Analyser body;
//  Runtime *rt;
  Module *module;

  void load(Analyser &a);
  string call(Strings &pars);
//  FnDef(Runtime *rt):rt(rt),module(rt->module) {}
  FnDef(Module *module):module(module) {}
};

}

#endif
