#include <jlib.h>
#pragma hdrstop

#include "jreflect.h"

namespace rtti {

rtti_define(Int)
rtti_define(Char)
rtti_define(Float)
rtti_define(Double)
rtti_define(String)
rtti_define(ConstPtrChar)


void init_rtti() {
  rtti_impl(Int)
  rtti_impl(Char)
  rtti_impl(Float)
  rtti_impl(Double)
  rtti_impl(String)
  rtti_impl(ConstPtrChar)

  IntClass->init_prim<int>("int");
  FloatClass->init_prim<float>("float");
  DoubleClass->init_prim<double>("double");
  StringClass->init_prim<string>("string");
  ConstPtrCharClass->init_prim<const char*>("constptrchar");
}

}

