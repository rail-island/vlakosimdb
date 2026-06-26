#include <jlib.h>
#pragma hdrstop

#include "excpt.h"

namespace jlib {


Ecommon::Ecommon(const char *amsg) {
  strcpy(msg,amsg);
}


}

#pragma package(smart_init)
