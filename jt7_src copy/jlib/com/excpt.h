#ifndef excptH
#define excptH

namespace jlib {


class Ecommon {
  char msg[0x100];
public:
  Ecommon(const char *amsg);
  Ecommon() {*msg=0;}
  char *what() {return msg;}
};

}

#endif
