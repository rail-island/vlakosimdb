#ifndef awwpaintH
#define awwpaintH

#include <aws/awspaint.h>
#include "awwcontrol.h"
#include <win/w32gr.h>
#include "awwfont.h"

namespace aww {

struct awwCanvas:RealCanvas {
  wlib::WinDC wdc;
  Rect clip;
  Rect getcliprect() {Rect res=clip;res.inflate(1,1);return res;}
  void setfgcolor(int color) {wdc.setpen(color);}
  void setbgcolor(int color) {wdc.setbrush(color);}
  virtual void setpenwi(int wi) {wdc.setpenwi(wi);}
  void drawline(int x1,int y1,int x2,int y2) {wdc.line(x1,y1,x2,y2);}
  void drawrect(int x1,int y1,int x2,int y2) {repairrect(x1,y1,x2,y2);wdc.rect(x1,y1,x2,y2);}
  void drawellipse(int x1,int y1,int x2,int y2) {repairrect(x1,y1,x2,y2);wdc.ellipse(x1,y1,x2,y2);}
  void drawpixel(int x,int y,int color) {wdc.drawpixel(x,y,color);}
  void drawpolygon(const Point *pts,int count) {wdc.polygon((POINT*)pts,count);}
  int getpixel(int x,int y) {return wdc.getpixel(x,y);}
  int gettextwi(string text) {return wdc.gettextwi(text);}
  int gettexthi(string text) {return wdc.gettexthi(text);}
//  void drawpicture(int x,int y,Picture *picture) {picture->blit(Canvas(ptr,x,y));}
//  void stretchpicture(const Rect &dest,Picture *picture) {picture->st xabstract;}
  void setfont(Font *afont);
  void drawtext(int x,int y,string str) {wdc.textout(x,y,str);}
  void cliptext(int x,int y,int x1,int y1,int x2,int y2,string text) {wdc.cliptext(x,y,x1,y1,x2,y2,text);}
  void drawframe(const Rect &r,bool sunken);
  void drawradio(int x,int y,bool checked);
  void drawcheck(int x,int y,bool checked);
  void floodfill(int x,int y,string mode,int color) {wdc.floodfill(x,y,mode,color);}
//#delbeg [ABAZ]
//  void blitbmp(zbuf::Bitmap *bmp,int destx,int desty,int srcx,int srcy,int w,int h);
//#delend [ABAZ]
  awwCanvas(HDC dc):wdc(dc) {}
private:
  void repairrect(int &x1,int &y1,int &x2,int &y2);
};

/*
struct awwCanvas:Canvas {
  wlib::WinDC wdc;
  Rect clip;
  int dx,dy;
  Rect getcliprect() {return clip-Point(dx,dy);}
  void setfgcolor(int color) {wdc.setpen(color);}
  void setbgcolor(int color) {wdc.setbrush(color);}
  void drawline(int x1,int y1,int x2,int y2) {wdc.line(x1+dx,y1+dy,x2+dx,y2+dy);}
  void drawrect(int x1,int y1,int x2,int y2) {wdc.rect(x1+dx,y1+dy,x2+dx,y2+dy);}
  void drawellipse(int x1,int y1,int x2,int y2) {wdc.ellipse(x1+dx,y1+dy,x2+dx,y2+dy);}
  int gettextwi(string text) {return wdc.gettextwi(text);}
  int gettexthi(string text) {return wdc.gettexthi(text);}
  void setfont(Font *afont) {if (awwFont *f=dynamic_cast<awwFont*>(afont)) wdc.setfont(f->gethandle());}
  void drawtext(int x,int y,string str) {wdc.textout(x+dx,y+dy,str);}
  void cliptext(int x,int y,int x1,int y1,int x2,int y2,string text) {wdc.cliptext(x+dx,y+dy,x1+dx,y1+dy,x2+dx,y2+dy,text);}
  void drawframe(const Rect &r,bool sunken);
  void blitbmp(zbuf::Bitmap *bmp,int destx,int desty,int srcx,int srcy,int w,int h);
  awwCanvas(HDC dc):wdc(dc) {dx=dy=0;}
};
*/

struct awwBmp {
  HBITMAP hbmp;
  int w,h;
  int bits;//-1:neznema
  awwBmp() {oldbmp=NULL;hbmp=NULL;w=h=0;bits=-1;}
  ~awwBmp() {done();}
  void resize(int aw,int ah) {w=aw;h=ah;resize();}
  void resize();
  void loadbmp(string file);
  void wantsize(int aw,int ah) {if (w<aw or h<ah) resize(aw,ah);}
  void done() {
    if (hbmp) DeleteObject(hbmp);w=h=0;
    hbmp=NULL;
  }
  void drawto(HDC dest,int x,int y) {
    HDC mdc=getdc();
    BitBlt(dest,x,y,w,h,mdc,0,0,SRCCOPY);
    releasedc(mdc);
  }
  HDC getdc() {
    HDC mdc;
    HDC sdc=GetDC(0);
    mdc=CreateCompatibleDC(sdc);
    ReleaseDC(0,sdc);
    oldbmp=SelectObject(mdc,hbmp);
    return mdc;
  }
  void releasedc(HDC mdc) {
    SelectObject(mdc,oldbmp);
    oldbmp=NULL;
    DeleteDC(mdc);
  }
  void doop(void (*trop)(int *buf,int &w,int &h));
  void hflip();
  void vflip();
  void transp();
  void invert();
private:
  HBITMAP oldbmp;
};

struct awwPicture:Picture {
  awwBmp bmp;
  awwCanvas *canvas;
  awwPicture() {canvas=NULL;}
//  Canvas getcanvas() {xassert(canvas==NULL);canvas=new awwCanvas(bmp.getdc());return Canvas(&*canvas,0,0);}
//  void releasecanvas(Canvas &cnv) {xassert(cnv.cnv.ptr()==canvas);bmp.releasedc(canvas->wdc.getdc());canvas=NULL;}
  Canvas getcanvas();
  void releasecanvas();
//  void blit(Canvas cnv);
//  void transparentblit(Canvas cnv,int transparentcolor);
  bool dodraw(BltDrawOpts &o,RealCanvas *cnv);
  bool dostretch(StretchDrawOpts &o,RealCanvas *cnv);
//  void bitblt(Canvas cnv,Rect src,STRING rop,int rop_arg);
//  void stretchblt(Canvas cnv,Rect dest,Rect src,STRING rop,int rop_arg);
  void setw(int w) {releasecanvas();bmp.resize(w,bmp.h);}
  void seth(int h) {releasecanvas();bmp.resize(bmp.w,h);}
  int getw() {return bmp.w;}
  int geth() {return bmp.h;}
  void load(string file);
  void save(string file);
  void setbits(int bits) {releasecanvas();bmp.bits=bits;bmp.resize();}
  int getbits() {return bmp.bits;}
  void hflip() {releasecanvas();bmp.hflip();}
  void vflip() {releasecanvas();bmp.vflip();}
  void invert() {releasecanvas();bmp.invert();}
  void transp() {releasecanvas();bmp.transp();}
  ~awwPicture() {releasecanvas();}
//#delbeg [ABAZ]
//  static zbuf::Bitmap *loadbitmap(string file,int bits);
//#delend [ABAZ]
private:
//  bool _bitblt(Canvas cnv,const Rect &src,int rop);
//  bool _stretchblt(Canvas cnv,const Rect &dest,const Rect &src,int rop);
  bool _bitblt(RealCanvas *cnv,const Point &dest,const Rect &src,int rop);
  bool _stretchblt(RealCanvas *cnv,const Rect &dest,const Rect &src,int rop);
};


/*
dblbuf==0:zadny doublebuffer
dblbuf==1:doublebuffer velikosti max clip-rectu, ctrl->onpaint je volano pro kazde WM_PAINT
dblbuf==2:onpaint je volano jen pri repaint, na WM_PAINT reaguje bez pomoci aws::PaintControl
*/

struct awwPaintControl:awwControl {
  void onpaint(awwMessage &msg);
  void onsize(awwMessage &msg,int w,int h);
  awwPaintControl(Paintable *pc):awwControl(dynamic_cast<Control*>(pc)),pc(pc) {dblbuf=0;erasebkgnd=true;}
  void createparams(CreateParams &pars);
  void repaint(const Rect &r);
  void repaint();
  void onmousedown(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift) {
    awwControl::onmousedown(msg,x,y,button,shift);
    SetCapture(win);
  }
  void onmouseup(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift) {
    ReleaseCapture();
    awwControl::onmouseup(msg,x,y,button,shift);
  }
  void setdoublebuffer(int value) {
    if (dblbuf!=value) dblbuf=value,updatedblbuf();
  }
  int getdoublebuffer() {return dblbuf;}
  void seterasebkgnd(bool value) {erasebkgnd=value;}
  bool geterasebkgnd() {return erasebkgnd;}
private:
  void updatedblbuf();
  void repaintto(HDC dc,RECT *clip);
  void bufpaint(HDC dc,RECT &clip);
  void repainttomemdc();

  bool erasebkgnd;
  awwBmp bmp;
  Paintable *pc;
  int dblbuf;
protected:
  virtual int getcnvdx() {return 0;}
  virtual int getcnvdy() {return 0;}
};


}

#endif
