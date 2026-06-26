#include <jlib.h>
#pragma hdrstop
#include "autodbs.h"

namespace jlib {

bool AUTODBS::isdefined(const char *s) {
  if (eqstr(s,"last")) {
    if (arrayw<=arraystack) {fatalerror("Testovani last mimo cyklus");return false;}
    return arrayw[-1].index==arrayw[-1].array->items.count-1;
  }
  if (eqstr(s,"first")) {
    if (arrayw<=arraystack) {fatalerror("Testovani first mimo cyklus");return false;}
    return arrayw[-1].index==0;
  }
  for(ADBSELEM **e=elemstack;e<elemw;e++) {
    if ((*e)->vars.findvari(s)>=0) return true;
  }
  return false;
}

void AUTODBS::structurejump(const char *endcom,bool behind) {
  int level=0;
  while (*act) {
    string coms=jumpnextcom(behind);
    COM com;com.init(coms,' ');
    if (com=="for" or com=="ifdef" or com=="ifndef") level++;else
    if (com=="endfor" or com=="endif") {
      if (level) level--;else {
        if (not (com==endcom)) fatalerrorfmt("Spatne ukonceni bloku(ma byt %s,je %s)",endcom,com[0]);
        return;
      }
    }
  }
  fatalerror("Neukonceny blok");
}

string AUTODBS::getvalue(const string &name) {
  int up_level=0;
  char *c=(char*)name.c_str();
  while (*c=='.') c++,up_level++;
  for(ADBSELEM **e=elemw-1-up_level;e>=elemstack;e--) if (const char *value=(*e)->vars.findvar(c)) return string(value);
  return unknownvar(name);
}

void AUTODBS::docommand(const string &name) {
  COM com;com.init(name,' ');
  if (com=="for") {
    ADBSARRAY *array=elemw[-1]->findarray(com[1]);
    if (array) {
      *arrayw++=AREC(array,this);
      if (array->items.count==0) {
        structurejump("endfor",true);
        arrayw--;
      } else {
        *elemw++=array->items[0];
      }
    } else fatalerrorfmt("Pole nenalezeno:%s",com[1]);
  } else if (com=="endfor") {
    if (arrayw>arraystack) {
      arrayw[-1].index++;
      if (arrayw[-1].index<arrayw[-1].array->items.count) {
        jump(arrayw[-1].beg);
        elemw[-1]=arrayw[-1].array->items[arrayw[-1].index];
      } else arrayw--,elemw--;
    } else fatalerror("ukonceni neexistujiciho cyklu");
  } else if (com=="ifdef") {
    if (not isdefined(com[1])) structurejump("endif",true);
  } else if (com=="ifndef") {
    if (isdefined(com[1])) structurejump("endif",true);
  }
}



}

#pragma package(smart_init)
