#ifndef ssintH
#define ssintH

#include "gmanal.h"
#include "ssdefs.h"

namespace ss {
using namespace gram;

struct FnResult {
  string extractres() {string res=r;delete this;return res;}
  FnResult(string r):r(r) {}
  string r;
};

struct ParseOpts {
  char escape,jumper;
  ParseOpts() {escape='\\';jumper='_';}
};

class Interpreter;
struct Instruction {
  virtual FnResult *work(Interpreter *ir)=0;
  virtual ~Instruction() {}
};

struct Interpreter {
  void push(string what) {stack.add(what);}
  string pop() {return stack.pop();}
  string top(int dist=1) {return stack[stack.count-dist];}
  void poppars(int parcnt,Strings &pars) {
    for(int i=0;i<parcnt;i++) pars.add(stack[stack.count-parcnt+i]);
    stack.eraselast(parcnt);
  }
  void put(string str) {out<<str;}
  void clearout() {out.clear();}
  string getout() {return out.str();}
private:
  MEMSTROUT out;
  Strings stack;
};

class Program;
struct Block {
  IObjArray<Instruction> code;
  FnResult *work(Interpreter *ir);
//  void parse(Analyser &a);
  Module *module;
  Block(Module *module):module(module) {}

  void parsecm(Analyser &a,const char *endcom,Program *program,ParseOpts &po);
  //if endcom==NULL, pak musi koncit *a==0
  //if string(endcom)=="", pak muze koncit cimkoli
  //else musi koncit endcom
  //posledni prikaz neni preskocen
  void parsecmcom(Analyser &a,Program *program,ParseOpts &po);

  void parsessincm_coms(Analyser &a,ParseOpts &po);

  void parseexpr(Analyser &a,ParseOpts &po) {parseor(a,po);}
  void parsecom(Analyser &a,ParseOpts &po);
  void parsecoms(Analyser &a,ParseOpts &po);
  void parseblock(Analyser &a,ParseOpts &po);
  void parsecond(Analyser &a,ParseOpts &po);

  void parsehi(Analyser &a,ParseOpts &po);
  void parsedefer(Analyser &a,ParseOpts &po);
  void parsemul(Analyser &a,ParseOpts &po);
  void parseadd(Analyser &a,ParseOpts &po);
  void parserel(Analyser &a,ParseOpts &po);
  void parsenot(Analyser &a,ParseOpts &po);
  void parseand(Analyser &a,ParseOpts &po);
  void parseor(Analyser &a,ParseOpts &po);
};

struct Function:SubDef {
  string call(Strings &pars);
  void parse(Analyser &a,ParseOpts &po);
  void parsecm(Analyser &a,Program *program,ParseOpts &po);
  Function(Module *module):module(module),block(module) {}
private:
  Block block;
  Strings parnames;
  Module *module;
};

struct Program {
  void compile(string file);
  void compilecm(string file);
  bool caninclude(string file);
  //vraci, zda jeste nebylo includovano
  string loadinclude(string file);
  //nacte text ze souboru
  string run();
  Program(Module *module);
  string setactfile(string value) {string res=actfile;actfile=filename(value);return res;}
  void wasincluded(string file) {included.add(filename(file).getupper());}
  void restoreactfile(string value) {actfile=value;}

  void throweanal(string text,AnalError &e);
private:
  string actfile;
  string filename(string incname);
  Module *module;
  Block main;
  Strings included;
};

}

#endif
