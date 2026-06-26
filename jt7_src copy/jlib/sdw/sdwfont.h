#ifndef sdwfontH
#define sdwfontH

#include <sdl/sdl.h>
#include <ext/xmlnode.h>

namespace sdw {

struct sdwFontChar {
  int x,y,w,h;
  int color;//kterou barvou je zrovna nabarveno pismeno
  sdwFontChar() {x=y=w=h=0;color=-1;}
};

struct sdwFontEngine:RCObject {
  sdwFontChar chars[0x100];
  SDL_Surface *bmp;

  sdwFontEngine();
  ~sdwFontEngine();
  bool load(XMLNode xml,string bmpfile);
  int textwi(string text);
  int texthi(string text);
  void draw(SDL_Surface *screen,string text,int x,int y,int color);
private:
  void _drawchar(SDL_Surface *screen,sdwFontChar &ch,int x,int y,int color);
};

}

#endif
