#ifndef rotatesolidH
#define rotatesolidH

#include <jmath/polyedr.h>
#include "coloralg.h"

namespace glo {
using namespace jmath;

struct CylindricColorizeItem {
  string texture;
  ColorCond cond;
  bool material;
  float xscale,yscale;
  bool reflectx,reflecty;
  bool transp;
  void load(XMLNode xml);
};

struct CylindricColorize {
  IObjArray<CylindricColorizeItem> items;
  void load(XMLNode xml);
  CylindricColorizeItem *find(string faceid);
};


///normaly (culling) se spocitaji dobre jedine jde-li o konvexni telese
void generaterotatesolid(prPolyedr &pedr,TCntVec<fpoint3d> &pts,int cnt,CylindricColorize &cize,Strings &faceids);


}

#endif
