#ifndef dateH
#define dateH

namespace jlib {


struct JDATE {
  int secs;//pocet sekund od 1.1.1970
  char s[0x40];
  JDATE(int day,int month,int year);
  char *c_str();
};

}

#endif
