#include <jlib.h>
#pragma hdrstop

#include "sdwcanvas.h"
#include "sdwopts.h"
#include "sdwfont.h"
#include <sdl/sdl_gfxprimitives.h>
//#include <sdl/bfont/bfont.h>
#include "sdwpaint.h"

namespace sdw {

bool iscollision(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2) {
	if (x1<x2) {
		if (x1+w1>x2) {
			if (y1<y2) {
				if (y1+h1>y2) return true;
			} else {
				if (y2+h2>y1) return true;
			}
		}
	} else {
		if (x2+w2>x1) {
			if (y2<y1) {
				if (y2+h2>y1) return true;
			}	else {
				if (y1+h1>y2) return true;
			}
		}
	}
	return false;
}

SDL_Rect SDLRect(int x,int y,int w,int h) {
  SDL_Rect res;
  res.x=x;res.y=y;res.w=w;res.h=h;
  return res;
}
SDL_Rect SDLBounds(int x1,int y1,int x2,int y2) {
  SDL_Rect res;
  res.x=x1;res.y=y1;res.w=x2-x1+1;res.h=y2-y1+1;
  return res;
}

void sdwCanvas::circle(int x,int y,int rad,unsigned int color,int alpha) {
  circleColor(surface,x+dx,y+dy,rad,nativetosdl(color,alpha));
}
void sdwCanvas::fcircle(int x,int y,int rad,unsigned int color,int alpha) {
  filledCircleColor(surface,x+dx,y+dy,rad,nativetosdl(color,alpha));
}
void sdwCanvas::ellipse(int x,int y,int rx,int ry,unsigned int color,int alpha) {
  ellipseColor(surface,x+dx,y+dy,rx,ry,nativetosdl(color,alpha));
}
void sdwCanvas::fellipse(int x,int y,int rx,int ry,unsigned int color,int alpha) {
  filledEllipseColor(surface,x+dx,y+dy,rx,ry,nativetosdl(color,alpha));
}
void sdwCanvas::rect(int x1,int y1,int x2,int y2,unsigned int color,int alpha) {
  rectangleColor(surface,x1+dx,y1+dy,x2+dx,y2+dy,nativetosdl(color,alpha));
}
void sdwCanvas::frect(int x1,int y1,int x2,int y2,unsigned int color,int alpha) {
  for(int y=y1;y<=y2;y++) hlineColor(surface,x1+dx,x2+dx,y+dy,nativetosdl(color,alpha));
//  boxColor(surface,x1+dx,y1+dy,x2+dx,y2+dy,nativetosdl(color,alpha));
}
void sdwCanvas::line(int x1,int y1,int x2,int y2,unsigned int color,int alpha) {
  lineColor(surface,x1+dx,y1+dy,x2+dx,y2+dy,nativetosdl(color,alpha));
}
void sdwCanvas::text(int x,int y,string text,unsigned int color,sdwCommonFont *font,int alpha) {
  if (font) font->draw(surface,x+dx,y+dy,text,nativetosdl(color,alpha));
  else stringColor(surface,x+dx,y+dy,(char*)text.c_str(),nativetosdl(color,alpha));
/*
  if (font->issystem()) {
    stringColor(surface,x+dx,y+dy,(char*)text.c_str(),nativetosdl(color,alpha));
  } else if {

    font->draw(surface,text,x+dx,y+dy,nativetosdl(color,alpha));
  }
  static sdwFontEngine *font=NULL;
  if (!font) {
    MainXMLNode xml;
    try {
      xml.loadfromfile(basedir/"data"/"font_sizes.xml");
    } catch (...) {
      return;
    }
    font=new sdwFontEngine();
    font->load(xml,basedir/"data"/"font_images.bmp");
  }
  font->draw(surface,text,x+dx,y+dy,nativetosdl(color,alpha));
*/
//  static BFont_Info *info=NULL;
//  if (!info) info=LoadFont(basedir/"data"/"font"/"font01.bmp");
//  PutStringFont(surface,info,x+dx,y+dy,text.c_str());
}
void sdwCanvas::drawradio(int x,int y,bool checked,ViewOpts *vo) {
  circle(x,y,vo->radioradius,vo->clradio);
  if (checked) fcircle(x,y,vo->smallradioradius,vo->clradiosmall);
}
void sdwCanvas::bound(SDL_Rect value) {
  value.x+=dx;value.y+=dy;
  SDL_Rect act;
  SDL_GetClipRect(surface,&act);
  int x1=act.x,y1=act.y,x2=act.x+act.w,y2=act.y+act.h;
  int newx1=value.x,newy1=value.y,newx2=value.x+value.w,newy2=value.y+value.h;
  if (newx1>x1) x1=newx1;
  if (newy1>y1) y1=newy1;
  if (newx2<x2) x2=newx2;
  if (newy2<y2) y2=newy2;
  SDL_Rect newclip=SDLRect(x1,y1,x2-x1,y2-y1);
  SDL_SetClipRect(surface,&newclip);
}


SDL_Surface *createSDLSurface(int width,int height) {
  unsigned int rmask,gmask,bmask,amask;
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
  #else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
  #endif

  SDL_Surface *res=SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,rmask,gmask,bmask,amask);
  return res;
}

unsigned int nativetosdl(unsigned int color,unsigned int alpha) {
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    return (nativervalue(color)<<0)|(nativegvalue(color)<<8)|(nativebvalue(color)<<16)|(alpha<<24);
  #else
    return (nativervalue(color)<<24)|(nativegvalue(color)<<16)|(nativebvalue(color)<<8)|(alpha<<0);
  #endif
}


}


