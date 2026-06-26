#include <jlib.h>
#pragma hdrstop

#include "stdxmlnode.h"

namespace jlib {

void StdXMLNode::xml_setvar(const char *name,const char *value) {
  if (value) vars.setvar(name,value);else vars.erasevar(name);
}


}


