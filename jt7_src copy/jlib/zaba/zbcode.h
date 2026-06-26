#ifndef zbcodeH
#define zbcodeH

#include <gram/gmanal.h>
#include "zbstd.h"
#include "zbdefs.h"

namespace zaba {
using namespace gram;

struct Error {
  string msg;
  Error(string msg):msg(msg) {}
};

// com(setlocal,1) com(getlocal,1)
#define enumcoms \
  com(END,0) com(cm,2) com(xcm,2) com(label,1)\
  com(jmp,1) com(condjmp,1) com(condnotjmp,1)\
  com(pop,0) com(pushobj,1) com(doret,1) com(co,1)\
  com(getprop,1) com(setprop,1) com(initprop,1) com(assignprop,1) com(deleteprops,1) \
  com(xgetprop,1)\
  com(jx,1) com(jnx,1)\
  com(pushthis,0) com(pushnull,0) com(pushsys,0) com(pushlocals,0)\
  com(raise,0) com(xraise,0) com(xchg,2) com(polypop,1) com(pushparam,1)\
  com(pushtry,1) com(poptry,0)\
  com(dup,1) com(move,2)\
  com(find,2) com(join,0) com(callnear,1) com(retnear,0)\
  com(fadd,0) com(fmul,0) com(fdiv,0) com(fsub,0) com(idiv,0) com(imod,0) com(cat,1)\
  com(testeq,0)\
  com(feq,0) com(fneq,0) com(fgt,0) com(flt,0) com(fget,0) com(flet,0)\
  com(faddc,1) com(fmulc,1)\
  com(yield,0)\
  com(comcnt,0)

/*Popis instrukci:
*END:ukoncuje kod
*cm jmeno_metody pocet_parametru : zavola metodu
*label cislo : navesti, pomocna instrukce
*setlocal jmeno_promenne : nastavi lokalni promennou
*doret pocet : ukonci funkci, pricemz zachova pocet objektu z vracholu zasobniku
 (typicky 1=navratovaa hodnota)
*/

namespace Coms {
#define com(_name,_iparcnt) _name,
  enum {enumcoms COMCNT};
#undef com
}

#define com(_name,_iparcnt) #_name,
static const char *comnames[]={enumcoms "NOCOMMAND"};
#undef com
#define com(_name,_iparcnt) _iparcnt,
static const int compars[]={enumcoms 0};
#undef com
#define com(_name,_iparcnt) _iparcnt+1,
static const int comlens[]={enumcoms 1};
#undef com

struct zbObject;

struct Instruction {
  union {
    int i;
    int com;
    zbObject *obj;
    const char *s;
    number_t *pnum;
  };
  Instruction(int ipar=0) {i=ipar;}
  Instruction(number_t *fpar) {pnum=fpar;}
  Instruction(zbObject *aobj) {obj=aobj;}
  Instruction(const char *str) {s=str;}
};


struct Code {
  Instruction *get1() {return code.buf;}
  void load(Analyser &a);
  Code(Runtime *rt):rt(rt) {}
private:
  TCntVec<Instruction> code;
  STRLIST strs;
  IObjArray<number_t> numbers;

  ObjectArray objs;
  Runtime *rt;

  void linklabels();
  void addins(int instruction,Instruction p1=0,Instruction p2=0,Instruction p3=0);
private:
  void _add(Instruction i) {code.add()=i;}
  void _idfp(Analyser &a);
  void _intp(Analyser &a);
  void _flgp(Analyser &a);
  void _nump(Analyser &a);
  void _strp(Analyser &a);
  void _floatp(Analyser &a);
};


}

#endif
