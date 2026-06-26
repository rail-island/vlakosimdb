#ifndef strconvH
#define strconvH

namespace jlib {

class StrConv {
public:
  string str;
  StrConv(const char *src) {str=src;}
  StrConv(string src) {str=src;}
  StrConv& operator >>(void (*convproc)(const char *inbuf,STROUT &outbuf)) {
    MEMSTROUT res;
    convproc(str.c_str(),res);
    str=res.str();
    return *this;
  }
  StrConv& operator >>(void (*convproc)(char *changedbuf));
  void operator >>(string &out) {out=str;}
};

}

#endif
