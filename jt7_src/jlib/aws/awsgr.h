#ifndef awsgrH
#define awsgrH

#include "awsdefs.h" 
#include <gr/comgr.h>
//#delbeg [ABAZ]
//#include <gr/comcnv.h>
//#include <zbuf/bmpwrap.h>
//#delend [ABAZ]
#include <ext/xmlnode.h>
#include <com/ptrto.h>

namespace aws {

class RealCanvas;class Picture;

struct Font {
  virtual int getheight()=0;//v pixelech
  virtual int getwidth(string text)=0;

  virtual int getheight(RealCanvas *cnv)=0;//v pixelech
  virtual int getwidth(RealCanvas *cnv,string text)=0;

  //properties
  virtual int getsize()=0;
  virtual void setsize(int size)=0;
  virtual int getcolor()=0;
  virtual void setcolor(int color)=0;
  virtual string getname()=0;
  virtual void setname(string name)=0;
  virtual bool getunderline()=0;
  virtual void setunderline(bool underline)=0;
  virtual bool getbold()=0;
  virtual void setbold(bool bold)=0;
  virtual bool getitalic()=0;
  virtual void setitalic(bool italics)=0;
  virtual bool getstrikeout()=0;
  virtual void setstrikeout(bool strikeout)=0;

  virtual void assign(Font *src)=0;
  virtual bool edit()=0;
  virtual Font *clone()=0;
  void save(XMLNode xml);
  void load(XMLNode xml);

  virtual ~Font() {}
};

class Canvas;
class RealCanvas;
typedef SmartPtr<RealCanvas> SPCanvas;

struct StretchDrawOpts {
  Rect src;
  Rect dest;
  char rop[0x10];
  int rop_arg;
  StretchDrawOpts(const Rect &asrc,const Rect &adest,const char *arop,int arop_arg) {
    src=asrc;dest=adest;strcpy(rop,arop);rop_arg=arop_arg;
  }
};
struct BltDrawOpts {
  Rect src;
  Point dest;
  char rop[0x10];
  int rop_arg;
  BltDrawOpts(const Rect &asrc,const Point &adest,const char *arop,int arop_arg) {
    src=asrc;dest=adest;strcpy(rop,arop);rop_arg=arop_arg;
  }
};

struct RealCanvas {
  SPCanvas ptr;
  SPCanvas operator &() {return ptr;}
  RealCanvas():ptr(SmartPtrInit<RealCanvas>(this)) {}
  ~RealCanvas() {ptr.erased();}

  virtual Rect getcliprect() {xabstract;}
  virtual void setfgcolor(int color) {xabstract;}
  virtual void setbgcolor(int color) {xabstract;}
  virtual void setpenwi(int wi) {xabstract;}
  virtual void drawpixel(int x,int y,int color) {xabstract;}
  virtual int getpixel(int x,int y) {xabstract;}
  virtual void drawline(int x1,int y1,int x2,int y2) {xabstract;}
  virtual void drawrect(int x1,int y1,int x2,int y2) {xabstract;}
  virtual void drawellipse(int x1,int y1,int x2,int y2) {xabstract;}
  virtual void drawpolygon(const Point *pts,int count) {xabstract;}
  virtual int gettextwi(string text) {xabstract;}
  virtual int gettexthi(string text) {xabstract;}
  virtual void setfont(Font *afont) {xabstract;}
  virtual void drawtext(int x,int y,string str) {xabstract;}
  virtual bool dodrawpicture(BltDrawOpts &o,Picture *picture) {return false;}
  virtual bool dostretchpicture(StretchDrawOpts &o,Picture *picture) {return false;}
  virtual void drawchar(int x,int y,char ch,int pos) {char s[2]={ch,0};drawtext(x,y,s);}
  virtual void cliptext(int x,int y,int x1,int y1,int x2,int y2,string text) {xabstract;}
  virtual void drawframe(const Rect &r,bool sunken) {xabstract;}
  virtual void drawradio(int x,int y,bool checked) {xabstract;}
  virtual void drawcheck(int x,int y,bool checked) {xabstract;}
  virtual void floodfill(int x,int y,string mode,int color) {xabstract;}
//#delbeg [ABAZ]
//  virtual void blitbmp(zbuf::Bitmap *bmp,int destx,int desty,int srcx,int srcy,int w,int h) {xabstract;}
//#delend [ABAZ]
  virtual void nextband(Canvas &cnv,int hi,string type);
  virtual int getcharwi(char ch) {xabstract;}
  virtual RealCanvas *getasker() {return this;}

  void drawpicture(int x,int y,const Rect &src,Picture *picture,STRING rop,int rop_arg);
  void stretchpicture(const Rect &dest,const Rect &src,Picture *picture,STRING rop,int rop_arg);
  bool trydrawpicture(BltDrawOpts &o,Picture *picture);
  bool trystretchpicture(StretchDrawOpts &o,Picture *picture);
};

struct Canvas {
  int dx,dy;

  SPCanvas cnv;
//  RealCanvas *cnv;
  Canvas(SPCanvas cnv,int dx,int dy):cnv(cnv),dx(dx),dy(dy) {}
  Canvas(const Canvas &cnv,int dx,int dy):cnv(cnv.cnv),dx(cnv.dx+dx),dy(cnv.dy+dy) {}

  Canvas nextband(int hi,string type="") {Canvas res=*this;cnv->nextband(res,hi,type);return res;}
  Canvas &fg(int color) {cnv->setfgcolor(color);return *this;}
  Canvas &bg(int color) {cnv->setbgcolor(color);return *this;}
  Canvas &penwi(int wi) {cnv->setpenwi(wi);return *this;}
  Canvas &line(int x1,int y1,int x2,int y2) {cnv->drawline(x1+dx,y1+dy,x2+dx,y2+dy);return *this;}
  Canvas &rect(int x1,int y1,int x2,int y2) {cnv->drawrect(x1+dx,y1+dy,x2+dx,y2+dy);return *this;}
  Canvas &rect(const Rect &r) {return rect(r.a.x,r.a.y,r.b.x,r.b.y);}
  Canvas &ellipse(int x1,int y1,int x2,int y2) {cnv->drawellipse(x1+dx,y1+dy,x2+dx,y2+dy);return *this;}
  Canvas &font(Font *afont) {cnv->setfont(afont);return *this;}
  Canvas &text(int x,int y,string str) {cnv->drawtext(x+dx,y+dy,str);return *this;}
  Canvas &drawch(int x,int y,char ch,int pos) {cnv->drawchar(x+dx,y+dy,ch,pos);return *this;}
  Canvas &picture(int x,int y,Picture *picture);
  Canvas &picture(const Rect &r,Picture *picture);
  Canvas &blt(int x,int y,const Rect &src,Picture *picture,STRING rop,int rop_arg);
  Canvas &stretch(const Rect &dest,const Rect &src,Picture *picture,STRING rop,int rop_arg);
  Canvas &drawradio(int x,int y,bool checked) {cnv->drawradio(x+dx,y+dy,checked);return *this;}
  Canvas &drawcheck(int x,int y,bool checked) {cnv->drawcheck(x+dx,y+dy,checked);return *this;}
  Canvas &pixel(int x,int y,int color) {cnv->drawpixel(x+dx,y+dy,color);return *this;}
  Canvas &polygon(const Point *pts,int count);
  Canvas &floodfill(int x,int y,string mode,int color) {cnv->floodfill(x,y,mode,color);return *this;}
  int getpixel(int x,int y) {return cnv->getpixel(x+dx,y+dy);}
  int textwi(string text) {return cnv->gettextwi(text);}
  int texthi(string text) {return cnv->gettexthi(text);}
  int charwi(char ch) {return cnv->getcharwi(ch);}
  Rect clip() {return cnv->getcliprect()-Point(dx,dy);}
//#delbeg [ABAZ]
//  void blitbmp(zbuf::Bitmap *bmp,int destx,int desty,int srcx,int srcy,int w,int h) {
//    cnv->blitbmp(bmp,destx+dx,desty+dy,srcx,srcy,w,h);
//  }
//#delend [ABAZ]
  void drawframe(const Rect &r,bool sunken) {cnv->drawframe(r+Point(dx,dy),sunken);}
  void centertext(const Rect &r,string text,bool centerx=true,bool centery=true,bool clip=false);
  void drawselection(Rect r);
  void randomline(int x1,int y1,int x2,int y2);//nakresli caru nahodnou barvou

  SPCanvas getreal() const {return cnv;}
private:
  Canvas() {dx=dy=0;}
};

///Rozdeluje operace podle typu na ruzna platna
struct RedirectCanvas:RealCanvas {
//  RealCanvas *draw,*ask,*band;
//  RedirectCanvas(RealCanvas *draw,RealCanvas *ask,RealCanvas *band):draw(draw),ask(ask),band(band) {}
//  RedirectCanvas(SPCanvas draw,SPCanvas ask,SPCanvas band):draw(draw),ask(ask),band(band) {}
  Rect getcliprect() {return draw->getcliprect();}
  void setfgcolor(int color) {draw->setfgcolor(color);}
  void setbgcolor(int color) {draw->setbgcolor(color);}
  void drawline(int x1,int y1,int x2,int y2) {draw->drawline(x1,y1,x2,y2);}
  void drawpixel(int x,int y,int color) {draw->drawpixel(x,y,color);}
  void drawpolygon(const Point *pts,int count) {draw->drawpolygon(pts,count);}
  int getpixel(int x,int y) {return ask->getpixel(x,y);}
  void drawrect(int x1,int y1,int x2,int y2) {draw->drawrect(x1,y1,x2,y2);}
  void drawellipse(int x1,int y1,int x2,int y2) {draw->drawellipse(x1,y1,x2,y2);}
  int gettextwi(string text) {return ask->gettextwi(text);}
  int gettexthi(string text) {return ask->gettexthi(text);}
  void setfont(Font *afont);// {ask->setfont(afont);draw->setfont(afont);}
  void drawtext(int x,int y,string str) {draw->drawtext(x,y,str);}
  void drawchar(int x,int y,char ch,int pos) {draw->drawchar(x,y,ch,pos);}
  void cliptext(int x,int y,int x1,int y1,int x2,int y2,string text) {draw->cliptext(x,y,x1,y1,x2,y2,text);}
  void drawframe(const Rect &r,bool sunken) {draw->drawframe(r,sunken);}
  SPCanvas operator &() {return ptr;}
  RealCanvas *getasker() {return ask.ptr();}
  void setdraw(SPCanvas value) {draw=value;}
  void setask(SPCanvas value) {ask=value;}
  void setband(SPCanvas value) {band=value;}
  void setpenwi(int wi) {draw->setpenwi(wi);}
  void drawradio(int x,int y,bool checked) {draw->drawradio(x,y,checked);}
  void drawcheck(int x,int y,bool checked) {draw->drawcheck(x,y,checked);}
  bool dodrawpicture(BltDrawOpts &o,Picture *picture) {return draw->trydrawpicture(o,picture);}
  bool dostretchpicture(StretchDrawOpts &o,Picture *picture) {return draw->trystretchpicture(o,picture);}
private:
  SPCanvas draw,ask,band;
};

struct ResizeCanvas:RealCanvas {
  Rect getcliprect() {Rect r=cnv->getcliprect();r.a.x/=kx;r.b.x/=kx;r.a.y/=ky;r.b.y/=ky;return r;}
  virtual void setfgcolor(int color) {cnv->setfgcolor(color);}
  virtual void setbgcolor(int color) {cnv->setbgcolor(color);}
  virtual void setpenwi(int wi) {cnv->setpenwi(wi*kx);}
  virtual void drawpixel(int x,int y,int color) {cnv->drawpixel(x*kx,y*ky,color);}
  virtual int getpixel(int x,int y) {return cnv->getpixel(x*kx,y*ky);}
  virtual void drawline(int x1,int y1,int x2,int y2) {cnv->drawline(x1*kx,y1*ky,x2*kx,y2*ky);}
  virtual void drawpolygon(const Point *pts,int count);
  virtual void drawrect(int x1,int y1,int x2,int y2) {cnv->drawrect(x1*kx,y1*ky,x2*kx,y2*ky);}
  virtual void drawellipse(int x1,int y1,int x2,int y2) {cnv->drawellipse(x1*kx,y1*ky,x2*kx,y2*ky);}
  virtual int gettextwi(string text) {return cnv->gettextwi(text)/kx;}
  virtual int gettexthi(string text) {return cnv->gettexthi(text)/ky;}
  virtual void setfont(Font *afont) {font=afont->clone();font->setsize(font->getsize()*ky);cnv->setfont(font);}
  virtual void drawtext(int x,int y,string str) {cnv->drawtext(x*kx,y*ky,str);}
  virtual void drawchar(int x,int y,char ch,int pos) {cnv->drawchar(x*kx,y*ky,ch,pos);}
  virtual void cliptext(int x,int y,int x1,int y1,int x2,int y2,string text) {cnv->cliptext(x*kx,y*ky,x1*kx,y1*ky,x2*kx,y2*ky,text);}
  virtual void drawframe(const Rect &r,bool sunken) {cnv->drawframe(convrect(r),sunken);}
  virtual void nextband(Canvas &acnv,int hi,string type) {cnv->nextband(acnv,hi/ky,type);}
  virtual int getcharwi(char ch) {return cnv->getcharwi(ch)/kx;}

  void drawradio(int x,int y,bool checked) {cnv->drawradio(x*kx,y*ky,checked);}
  void drawcheck(int x,int y,bool checked) {cnv->drawcheck(x*kx,y*ky,checked);}
  bool dodrawpicture(BltDrawOpts &o,Picture *picture);
  bool dostretchpicture(StretchDrawOpts &o,Picture *picture);

  ResizeCanvas(SPCanvas cnv,float kx,float ky):cnv(cnv),kx(kx),ky(ky) {}
  SPCanvas operator &() {return ptr;}
private:
  Rect convrect(Rect r) {r.a.x*=kx;r.b.x*=kx;r.a.y*=ky;r.b.y*=ky;return r;}
  PtrTo<Font> font;
  float kx,ky;
  SPCanvas cnv;
};

///MetaCanvas-pamatuje si, co na nej bylo kresleno (uziti spolu s RecirectCanvas)
struct MetaCanvas:RealCanvas {
private:
  struct Item {
    virtual void draw(Canvas &cnv)=0;
  };
  struct FgItem:Item {
    int color;
    FgItem(int color):color(color) {}
    void draw(Canvas &cnv) {cnv.fg(color);}
  };
  struct BgItem:Item {
    int color;
    BgItem(int color):color(color) {}
    void draw(Canvas &cnv) {cnv.bg(color);}
  };
  struct LineItem:Item {
    int x1,y1,x2,y2;
    LineItem(int x1,int y1,int x2,int y2):x1(x1),y1(y1),x2(x2),y2(y2) {}
    void draw(Canvas &cnv) {cnv.line(x1,y1,x2,y2);}
  };
  struct RectItem:Item {
    int x1,y1,x2,y2;
    RectItem(int x1,int y1,int x2,int y2):x1(x1),y1(y1),x2(x2),y2(y2) {}
    void draw(Canvas &cnv) {cnv.rect(x1,y1,x2,y2);}
  };
  struct EllItem:Item {
    int x1,y1,x2,y2;
    EllItem(int x1,int y1,int x2,int y2):x1(x1),y1(y1),x2(x2),y2(y2) {}
    void draw(Canvas &cnv) {cnv.ellipse(x1,y1,x2,y2);}
  };
  struct TextItem:Item {
    int x,y;string text;
    TextItem(int x,int y,string text):x(x),y(y),text(text) {}
    void draw(Canvas &cnv) {cnv.text(x,y,text);}
  };
  struct CharItem:Item {
    int x,y;char ch;int pos;
    CharItem(int x,int y,char ch,int pos):x(x),y(y),ch(ch),pos(pos) {}
    void draw(Canvas &cnv) {cnv.drawch(x,y,ch,pos);}
  };
  struct FontItem:Item {
    Font *font;
    FontItem(Font *afont) {font=afont->clone();}
    ~FontItem() {delete font;}
    void draw(Canvas &cnv) {cnv.font(font);}
  };
  struct PictureItem:Item {
    BltDrawOpts o;
    RefBase<Picture> picture;
    PictureItem(BltDrawOpts &o,Picture *apicture):o(o) {picture=apicture;}
    void draw(Canvas &cnv) {cnv.blt(o.dest.x,o.dest.y,o.src,picture,o.rop,o.rop_arg);}
  };
  struct StretchPictureItem:Item {
    StretchDrawOpts o;
    RefBase<Picture> picture;
    StretchPictureItem(StretchDrawOpts &o,Picture *apicture):o(o) {picture=apicture;}
    void draw(Canvas &cnv) {cnv.stretch(o.dest,o.src,picture,o.rop,o.rop_arg);}
  };
  struct SetPenWiItem:Item {
    int wi;
    SetPenWiItem(int wi):wi(wi) {}
    void draw(Canvas &cnv) {cnv.penwi(wi);}
  };
  struct PixelItem:Item {
    int x,y,color;
    PixelItem(int x,int y,int color):x(x),y(y),color(color) {}
    void draw(Canvas &cnv) {cnv.pixel(x,y,color);}
  };
  struct PolygonItem:Item {
    Point *pts;int count;
    PolygonItem(const Point *apts,int acount) {count=acount;pts=new Point[count];for(int i=0;i<count;i++) pts[i]=apts[i];}
    ~PolygonItem() {delete[] pts;}
    void draw(Canvas &cnv) {cnv.polygon(pts,count);}
  };
  IObjArray<Item> items;
public:
//  Rect getcliprect() {xabstract;}
  void setfgcolor(int color) {items.add(new FgItem(color));}
  void setbgcolor(int color) {items.add(new BgItem(color));}
  void drawline(int x1,int y1,int x2,int y2) {items.add(new LineItem(x1,y1,x2,y2));}
  void drawrect(int x1,int y1,int x2,int y2) {items.add(new RectItem(x1,y1,x2,y2));}
  void drawellipse(int x1,int y1,int x2,int y2) {items.add(new EllItem(x1,y1,x2,y2));}
  void drawpixel(int x,int y,int color) {items.add(new PixelItem(x,y,color));}
  void drawpolygon(const Point *pts,int count) {items.add(new PolygonItem(pts,count));}
//  int gettextwi(string text) {return target->gettextwi(text);}
//  int gettexthi(string text) {return target->gettexthi(text);}
//  void setfont(Font *afont) {target->setfont(afont);items.add(new FontItem(afont));}
  void setfont(Font *afont) {items.add(new FontItem(afont));}
  void drawtext(int x,int y,string str) {items.add(new TextItem(x,y,str));}
  void drawchar(int x,int y,char ch,int pos) {items.add(new CharItem(x,y,ch,pos));}
  void cliptext(int x,int y,int x1,int y1,int x2,int y2,string text) {xabstract;}
  void drawframe(const Rect &r,bool sunken) {xabstract;}
  bool dodrawpicture(BltDrawOpts &o,Picture *picture) {items.add(new PictureItem(o,picture));return true;}
  bool dostretchpicture(StretchDrawOpts &o,Picture *picture) {items.add(new StretchPictureItem(o,picture));return true;}
  void setpenwi(int wi) {items.add(new SetPenWiItem(wi));}
//#delbeg [ABAZ]
//  void blitbmp(zbuf::Bitmap *bmp,int destx,int desty,int srcx,int srcy,int w,int h) {xabstract;}
//#delend [ABAZ]
//  RealCanvas *target;
public:
  void replacetext(string oldtext,string newtext);
  void draw(Canvas &cnv) {for(int i=0;i<items.count;i++) items[i]->draw(cnv);}
  void clear() {items.clear();}
//  MetaCanvas(RealCanvas *target=NULL):target(target) {}
//  MetaCanvas(const Canvas &target):target(target.getreal()) {}
  SPCanvas operator &() {return ptr;}
//  RealCanvas *getthis() {return this;}
};


struct StdMetaCanvas:RedirectCanvas {
  MetaCanvas meta;

  SPCanvas operator &() {return ptr;}

  StdMetaCanvas(SPCanvas target) {setdraw(&meta);setask(target);setband(&meta);}
  StdMetaCanvas(const Canvas &target) {setdraw(&meta);setask(target.getreal());setband(&meta);}

//  StdMetaCanvas(RealCanvas *target):RedirectCanvas(&meta,target,&meta) {}
//  StdMetaCanvas(SPCanvas target):RedirectCanvas(&meta,target,&meta) {}
//  StdMetaCanvas(const Canvas &target):RedirectCanvas(&meta,target.getreal(),&meta) {}
//  StdMetaCanvas(RealCanvas *target):RedirectCanvas(meta.getthis(),target,meta.getthis()) {}
//  StdMetaCanvas(const Canvas &target):RedirectCanvas(meta.getthis(),target.getreal(),meta.getthis()) {}
//  StdMetaCanvas(RealCanvas *target):RedirectCanvas(ptr,target,ptr) {}
//  StdMetaCanvas(const Canvas &target):RedirectCanvas(ptr,target.getreal(),ptr) {}
  void draw(Canvas &cnv) {meta.draw(cnv);}
  void clear() {meta.clear();}
};


struct NullCanvas:RealCanvas {
  Rect getcliprect() {return Rect(0,0,0,0);}
  virtual void setfgcolor(int color) {}
  virtual void setbgcolor(int color) {}
  virtual void setpenwi(int wi) {}
  virtual void drawpixel(int x,int y,int color) {}
  virtual int getpixel(int x,int y) {return 0;}
  virtual void drawline(int x1,int y1,int x2,int y2) {}
  virtual void drawrect(int x1,int y1,int x2,int y2) {}
  virtual void drawellipse(int x1,int y1,int x2,int y2) {}
  virtual int gettextwi(string text) {return 0;}
  virtual int gettexthi(string text) {return 0;}
  virtual void setfont(Font *afont) {}
  virtual void drawtext(int x,int y,string str) {}
  virtual void drawchar(int x,int y,char ch,int pos) {}
  virtual void cliptext(int x,int y,int x1,int y1,int x2,int y2,string text) {}
  virtual void drawframe(const Rect &r,bool sunken) {}
  virtual void nextband(Canvas &acnv,int hi,string type) {}
  virtual int getcharwi(char ch) {return 0;}
  SPCanvas operator &() {return ptr;}
};


}

#endif
