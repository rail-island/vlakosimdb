#include <jlib.h>
#pragma hdrstop

#include "zbdefs.h"
#include "zbrt.h"
#include "zbobj.h"

namespace zaba {

zbObject *createVoid(Runtime *rt) {
  return rt->getvoid()->addref();
}
zbObject *createNumber(number_t num,Runtime *rt) {return new zbNumber(num,rt);}
zbObject *createString(string s,Runtime *rt) {return new zbString(s,rt);}
zbObject *createLogical(bool value,Runtime *rt) {if (value) return rt->getsys()->addref();return rt->getvoid()->addref();}
zbObject *createMultiIndex(TCntVec<int> &items,Runtime *rt) {zbMultiIndex *res=new zbMultiIndex(NULL,rt);res->items.assign(items);return res;}

}


