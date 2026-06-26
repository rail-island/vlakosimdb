#ifndef glofontH
#define glofontH

#include "gloobject.h"
#include <com/rcobj.h>
#include <ext/xmlnode.h>

namespace glo {

struct gloChar {
  float x,y,w,h;
  gloChar() {x=y=w=h=0;}
};

struct gloFont {
  RefBase<Texture> tex;
  gloChar chars[0x100];
  
  void load(XMLNode xml,string bmpfile);
  void render(float x,float y,string text,RenderContext &rc);
  float textwi(string text);
  float texthi(string text);
};

}

#endif
