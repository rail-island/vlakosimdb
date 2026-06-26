#include <jlib.h>
#pragma hdrstop

#include "sdwpaint.h"
#include "sdwsystem.h"
#include <sdl/sdl_gfxprimitives.h>

namespace sdw {

sdwPaintControl::sdwPaintControl(PaintControl *ctrl,sdwSystem *sys):sdwControl(ctrl,sys) {
  paintctrl=ctrl;
}

void sdwPaintControl::dopaint(Canvas cnv) {
  awsEvent ev;
  paintctrl->onpaint(cnv,ev);
}

void sdwPaintControl::draw(sdwCanvas cnv) {
  sdwRealCanvas sdwc(cnv,sys);
  aws::Canvas awscnv(&sdwc,0,0);
  dopaint(awscnv);
}

void sdwRealCanvas::drawrect(int x1,int y1,int x2,int y2) {
  if (brushcolor!=clnnone) cnv.frect(x1,y1,x2,y2,brushcolor);
  if (pencolor!=clnnone) cnv.rect(x1,y1,x2,y2,pencolor);
}
void sdwRealCanvas::drawellipse(int x1,int y1,int x2,int y2) {
  int x=(x1+x2)/2,y=(y1+y2)/2,rx=abs(x2-x1)/2,ry=abs(y2-y1)/2;
  if (brushcolor!=clnnone) cnv.fellipse(x,y,rx,ry,brushcolor);
  if (pencolor!=clnnone) cnv.ellipse(x,y,rx,ry,pencolor);
}
void sdwRealCanvas::drawframe(const Rect &r,bool sunken) {
  int cout=sys->vo->clrisedframeout,cin=sys->vo->clrisedframein,cbg=sys->vo->clrisedframebg;
  if (sunken) cout=sys->vo->clsunkenframeout,cin=sys->vo->clsunkenframein,cbg=sys->vo->clsunkenframebg;
  cnv.frect(r.a.x,r.a.y,r.b.x,r.b.y,cbg);
  cnv.rect(r.a.x,r.a.y,r.b.x,r.b.y,cout);
  cnv.rect(r.a.x+1,r.a.y+1,r.b.x-1,r.b.y-1,cin);
}
void sdwRealCanvas::drawradio(int x,int y,bool checked) {
  cnv.drawradio(x,y,checked,sys->vo);
}
void sdwRealCanvas::drawcheck(int x,int y,bool checked) {
}
void sdwRealCanvas::cliptext(int x,int y,int x1,int y1,int x2,int y2,string text) {
  SDL_Rect clip=cnv.getclip();
  cnv.bound(SDLBounds(x1,y1,x2,y2));
  cnv.text(x,y,text,font->getcolor(),font);
  cnv.setclip(clip);
}

void sdwSystemFont::assign(Font *src) {
  if (sdwSystemFont *f=dynamic_cast<sdwSystemFont*>(src)) {
    color=f->color;
  }
}

void sdwBitmapFont::assign(Font *src) {
  if (sdwBitmapFont *f=dynamic_cast<sdwBitmapFont*>(src)) {
    color=f->color;
    eng=f->eng;
  }
}

sdwRealCanvas::sdwRealCanvas(sdwCanvas cnv,sdwSystem *sys):cnv(cnv),sys(sys) {
  pencolor=clnblack;brushcolor=clnwhite;
  font=dynamic_cast<sdwCommonFont*>(sys->createfont());
}

void sdwBitmapFont::draw(SDL_Surface *surface,int x,int y,string text,int sdlcolor) {
  eng->draw(surface,text,x,y,sdlcolor);
}
void sdwSystemFont::draw(SDL_Surface *surface,int x,int y,string text,int sdlcolor) {
  stringColor(surface,x,y,(char*)text.c_str(),sdlcolor);
}

int sdwRealCanvas::gettextwi(string text) {
  return font->getwidth(text);
}
int sdwRealCanvas::gettexthi(string text) {
  return font->getheight();
}

void sdwPicture::wantbmp() {
  if (w>0 && h>0 && !bmp) {
    bmp=createSDLSurface(w,h);
  }
}

void sdwPicture::seth(int value) {
  clearbmp();
  h=value;
}
void sdwPicture::setw(int value) {
  clearbmp();
  w=value;
}

bool sdwPicture::dodraw(BltDrawOpts &o,RealCanvas *cnv) {
  if (sdwRealCanvas *rc=dynamic_cast<sdwRealCanvas*>(cnv)) {
    if (bmp) {
      SDL_Surface *dstsur=rc->cnv.surface;
      SDL_Rect src;src.x=o.src.a.x;src.y=o.src.a.y;src.w=o.src.size().w;src.h=o.src.size().h;
      SDL_Rect dst;dst.x=rc->cnv.dx+o.dest.x;dst.y=rc->cnv.dy+o.dest.y;dst.w=src.w;dst.h=src.h;
      SDL_BlitSurface(bmp,&src,dstsur,&dst);
    }
    return true;
  }
  return false;
}
bool sdwPicture::dostretch(StretchDrawOpts &o,RealCanvas *cnv) {
  return false;
}
void sdwPicture::load(string file) {
  clearbmp();
  bmp=SDL_LoadBMP(file.c_str());
  if (bmp) w=bmp->w,h=bmp->h;
}
void sdwPicture::save(string file) {
  if (bmp) SDL_SaveBMP(bmp,file.c_str());
}


}

