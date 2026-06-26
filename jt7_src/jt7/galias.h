#ifndef galiasH
#define galiasH

namespace jtyc {

struct ImageAlias {
  string src,dest;
};

struct Aliases {
  IObjArray<ImageAlias> motocars,blds,machines;
  void load();
private:
  void loadfile(string s,char mode);
};

}
#endif
