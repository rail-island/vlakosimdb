#ifndef sdwpaintH
#define sdwpaintH

#include "sdwcontrol.h"
#include "sdwcanvas.h"
#include <aws/awspaint.h>
#include "sdwfont.h"

namespace sdw {

struct sdwCommonFont:aws::Font {
  //properties
  int getsize() {return 1;}
  void setsize(int size) {}
  int getcolor() {return color;}
  void setcolor(int value) {color=value;}
  string getname() {return "";}
  void setname(string name) {}
  bool getunderline() {return false;}
  void setunderline(bool underline) {}
  bool getbold() {return false;}
  void setbold(bool bold) {}
  bool getitalic() {return false;}
  void setitalic(bool italics) {}
  bool getstrikeout() {return false;}
  void setstrikeout(bool strikeout) {}

  virtual void draw(SDL_Surface *surface,int x,int y,string text,int sdlcolor)=0;

  int color;
};

struct sdwSystemFont:sdwCommonFont {
  int getheight() {return 8;}
  int getwidth(string text) {return text.length()*8;}

  int getheight(RealCanvas *cnv) {return 8;}
  int getwidth(RealCanvas *cnv,string text) {return text.length()*8;}

  void draw(SDL_Surface *surface,int x,int y,string text,int sdlcolor);

  void assign(Font *src);
  bool edit() {}
  Font *clone() {sdwSystemFont *res=new sdwSystemFont();res->assign(this);return res;}

  ~sdwSystemFont() {}
  sdwSystemFont() {color=clnblack;}
};

struct sdwBitmapFont:sdwCommonFont {
  int getheight() {return eng->texthi("AA");}
  int getwidth(string text) {return eng->textwi(text);;}

  int getheight(RealCanvas *cnv) {return getheight();}
  int getwidth(RealCanvas *cnv,string text) {return getwidth(text);}
  void draw(SDL_Surface *surface,int x,int y,string text,int sdlcolor);

  void assign(Font *src);
  bool edit() {}
  Font *clone() {sdwBitmapFont *res=new sdwBitmapFont();res->assign(this);return res;}

  ~sdwBitmapFont() {}
  sdwBitmapFont() {color=clnblack;eng=new sdwFontEngine();}

  RefBase<sdwFontEngine> eng;
};

struct sdwRealCanvas:RealCanvas {
  sdwCanvas cnv;
  Rect getcliprect() {SDL_Rect res=cnv.getclip();return Rect(res.x,res.y,res.w,res.h);}
  void setfgcolor(int color) {pencolor=color;}
  void setbgcolor(int color) {brushcolor=color;}
  void setpenwi(int wi) {}
  void drawline(int x1,int y1,int x2,int y2) {cnv.line(x1,y1,x2,y2,pencolor);}
  void drawrect(int x1,int y1,int x2,int y2);
  void drawellipse(int x1,int y1,int x2,int y2);
  int gettextwi(string text);
  int gettexthi(string text);
  void setfont(Font *afont) {}
  void drawtext(int x,int y,string str) {cnv.text(x,y,str,font->getcolor(),font);}
  void cliptext(int x,int y,int x1,int y1,int x2,int y2,string text);
  void drawframe(const Rect &r,bool sunken);
  void drawradio(int x,int y,bool checked);
  void drawcheck(int x,int y,bool checked);

  sdwRealCanvas(sdwCanvas cnv,sdwSystem *sys);
  int pencolor,brushcolor;
  sdwSystem *sys;
  PtrTo<sdwCommonFont> font;
};



struct sdwPaintControl:sdwControl {
  sdwPaintControl(PaintControl *ctrl,sdwSystem *sys);

  void draw(sdwCanvas cnv);
  virtual void dopaint(Canvas cnv);

  int getdoublebuffer() {return 1;}
  void setdoublebuffer(int value) {}
  bool geterasebkgnd() {return true;}
  void seterasebkgnd(bool value) {}

  PaintControl *paintctrl;
};

struct sdwPicture:Picture {
  SDL_Surface *bmp;
  int w,h;

  sdwPicture() {w=h=0;bmp=NULL;}
  ~sdwPicture() {clearbmp();}

  Canvas getcanvas() {throw JError("No canvas");}
  void releasecanvas() {}

  bool dodraw(BltDrawOpts &o,RealCanvas *cnv);
  bool dostretch(StretchDrawOpts &o,RealCanvas *cnv);
  int getw() {return w;}
  void setw(int value);
  int geth() {return h;}
  void seth(int value);
  int getbits() {return 32;}
  void setbits(int bits) {}
  void load(string file);
  void save(string file);
  void hflip() {}
  void vflip() {}
  void transp() {}
  void invert() {}
private:
  void clearbmp() {if (bmp) SDL_FreeSurface(bmp);bmp=NULL;}
  void wantbmp();
};


}

#endif
