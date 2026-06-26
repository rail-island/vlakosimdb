#include <jlib.h>
#pragma hdrstop

#include "sdwfont.h"

namespace sdw {

sdwFontEngine::sdwFontEngine() {
  bmp=NULL;
}
sdwFontEngine::~sdwFontEngine() {
  if (bmp) SDL_FreeSurface(bmp);
}

bool sdwFontEngine::load(XMLNode xml,string bmpfile) {
  SDL_Surface *loaded=SDL_LoadBMP(bmpfile.c_str());
  if (!loaded) return false;
  if (bmp) SDL_FreeSurface(bmp);
  bmp=SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_HWSURFACE|SDL_SRCALPHA,loaded->w,loaded->h,32,0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
  SDL_BlitSurface(loaded,NULL,bmp,NULL);
  SDL_FreeSurface(loaded);
  SDL_SetAlpha(bmp,SDL_SRCALPHA,255);

  int *buf=(int*)bmp->pixels;
  for(int i=0;i<bmp->w*bmp->h;i++) {
    char *c=(char*)&buf[i];
    if (c[1] && c[2] && c[3]) c[0]=255;else c[0]=0;
  }

  for(int i=0;i<xml.count();i++) {
    unsigned char chi=*xml[i].cvar("ch");
//    int chi=xml[i].ivar("ch");
//    if (chi<0 || chi>=0x100) continue;
    sdwFontChar &ch=chars[chi];
    ch.x=xml[i].ivar("x");
    ch.y=xml[i].ivar("y");
    ch.w=xml[i].ivar("w");
    ch.h=xml[i].ivar("h");
    if (ch.x<0 || ch.x+ch.w>bmp->w || ch.y<0 || ch.y+ch.h>bmp->h) ch.x=ch.y=ch.h=ch.w=0; 
  }
  return true;
}
void sdwFontEngine::draw(SDL_Surface *screen,string text,int x,int y,int color) {
  for(const char *c=text.c_str();*c;c++) {
    sdwFontChar &ch=chars[(unsigned char)*c];
    _drawchar(screen,ch,x,y,color);
    x+=ch.w;
  }
}
void sdwFontEngine::_drawchar(SDL_Surface *screen,sdwFontChar &ch,int x,int y,int color) {
  SDL_Rect src,dst;
  src.x=ch.x;src.y=ch.y;src.w=ch.w;src.h=ch.h;
  dst.x=x;dst.y=y;dst.w=src.w;dst.h=src.h;
  color|=0x010000FF;

  if (ch.color!=color) {
    int *buf=(int*)bmp->pixels;
    int *line=buf+bmp->w*ch.y+ch.x;
    for(int y=0;y<ch.h;y++,line+=bmp->w) {
      int *act=line;
      for(int x=0;x<ch.w;x++,act++) {
        if (*act) *act=color;
      }
    }
  }
  ch.color=color;

  SDL_BlitSurface(bmp,&src,screen,&dst);
}
int sdwFontEngine::textwi(string text) {
  float res=0;
  for(const char *c=text.c_str();*c;c++) {
    sdwFontChar &ch=chars[(unsigned char)*c];
    res+=ch.w;
  }
  return res;
}
int sdwFontEngine::texthi(string text) {
  return chars['0'].h;
}

}

