#ifndef gmexprH
#define gmexprH

namespace gram {

struct IVariables {
  virtual int getivalue(const char *name)=0;
};

int evalexpr(const char *expr,IVariables &vars);

}

#endif
