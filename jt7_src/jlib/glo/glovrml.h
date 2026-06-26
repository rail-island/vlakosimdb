#ifndef glovrmlH
#define glovrmlH

#include "glotextures.h"

namespace glo {

struct VRMLTexture {
  string file;
  RefBase<Texture> texture;
  bool match(Texture *atexture) {return texture==atexture;}
  void save(string file);
};
struct VRMLTextures {
  sFile textdir;
  string textdirname;
  IObjArray<VRMLTexture> items;
  //vrati plne kvalifikovanou cestu
  sFile add(Texture *texture);
  VRMLTextures() {}
  void save();
};


}

#endif
