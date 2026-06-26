#ifndef sdwcanvasH
#define sdwcanvasH

#include <sdl/sdl.h>
#include <gr/comgr.h>
#include <aws/awsdefs.h>

namespace sdw {
class ViewOpts;
class sdwCommonFont;

using namespace gr;
using namespace aws;

bool iscollision(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
inline bool iscollision(const Rect &r1,const Rect &r2) {
  return iscollision(r1.a.x,r1.a.y,r1.size().w,r1.size().h,r2.a.x,r2.a.y,r2.size().w,r2.size().h);
}

SDL_Rect SDLRect(int x,int y,int w,int h);
SDL_Rect SDLBounds(int x1,int y1,int x2,int y2);
//inline unsigned int nativetosdl(unsigned int nativecolor,unsigned int alpha=0xFF) {return (nativecolor>>8)|alpha;}
unsigned int nativetosdl(unsigned int color,unsigned int alpha=0xFF);

struct sdwCanvas {
  int dx,dy;
  SDL_Surface *surface;
  SDL_Rect getclip() {SDL_Rect res;SDL_GetClipRect(surface,&res);res.x-=dx;res.y-=dy;return res;}
  void setclip(SDL_Rect value) {value.x+=dx;value.y+=dy;SDL_SetClipRect(surface,&value);}
  void bound(SDL_Rect value);//zmensi cliprect
  sdwCanvas() {dx=dy=0;surface=NULL;}
  sdwCanvas(sdwCanvas cnv,int x,int y) {surface=cnv.surface;dx=x+cnv.dx;dy=y+cnv.dy;}
  sdwCanvas(SDL_Surface *asurface,int x=0,int y=0) {surface=asurface;dx=x;dy=y;}
  void circle(int x,int y,int rad,unsigned int color,int alpha=0xFF);
  void fcircle(int x,int y,int rad,unsigned int color,int alpha=0xFF);
  void ellipse(int x,int y,int rx,int ry,unsigned int color,int alpha=0xFF);
  void fellipse(int x,int y,int rx,int ry,unsigned int color,int alpha=0xFF);
  void rect(int x1,int y1,int x2,int y2,unsigned int color,int alpha=0xFF);
  void frect(int x1,int y1,int x2,int y2,unsigned int color,int alpha=0xFF);
  void line(int x1,int y1,int x2,int y2,unsigned int color,int alpha=0xFF);
  void text(int x,int y,string text,unsigned int color,sdwCommonFont *font,int alpha=0xFF);
  void drawradio(int x,int y,bool checked,ViewOpts *vo);
};

SDL_Surface *createSDLSurface(int width,int height);

}

#endif
