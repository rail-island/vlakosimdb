#ifndef gstaticH
#define gstaticH

#include "game0.h"

namespace jtyc {

struct Static:GObject {
  int x,y,w,h;
  bool octalized;
  Static(int ax,int ay,int aw,int ah) {
    x=ax;y=ay;w=aw;h=ah;
    octalized=false;
  }
  bool occupy(int ax,int ay) {return ax>=x && ax<x+w && ay>=y && ay<y+h;}
  virtual bool clickoccupy(int ax,int ay) {return occupy(ax,ay);}
  virtual void render(RenderContext &rc) {}
  virtual void rendercaptions(RenderContext &rc) {}
  virtual void afterloadlink() {}
//  virtual void redraw(View *v)=0;
//  virtual void redrawstatic(View *v)=0;
//  virtual void clickon(int dx,int dy) {};//co se state pri kliknuti na static
  //dx,dy:souradnice vuci objektu(ctvereckove)
//  Static(BINREAD &fr):GObject(fr) {
//    fr>>x>>y>>w>>h>>exist;
//  }
  Static(XMLNode xml) {
    x=xml.ivar("x");y=xml.ivar("y");w=xml.ivar("w");h=xml.ivar("h");
    octalized=false;
  }
//  void save(BINWRITE &fw) {GObject::save(fw);fw<<x<<y<<w<<h<<exist;}
//  virtual void viewchanged() {}
//  virtual void buldozere(int dx,int dy) {}//prekryta fce be mela volat fci destroy
  //(pokud k buldozerovani dojde)
  virtual void run(double deltatime) {}
  virtual void clearmobiles() {}
  virtual void xmlsave(XMLNode xml) {
    xml.setvar("x",x);xml.setvar("y",y);xml.setvar("w",w);xml.setvar("h",h);
    xml.setname("static");
  }
  virtual void dooctalize() {}
  virtual void dodelize() {}
  virtual void clearvisibility() {}
  void octalize() {
    if (octalized) return;
    dooctalize();
    octalized=true;
  }
  void delize() {
    if (!octalized) return;
    dodelize();
    octalized=false;
  }
  void destroy();
//  void staticchanged();
//protected:
//  void destroy();
};

struct Statics:GInstance {
  void render(RenderContext &rc) {for(int i=0;i<items.count;i++) items[i]->render(rc);}
  void rendercaptions(RenderContext &rc) {for(int i=0;i<items.count;i++) items[i]->rendercaptions(rc);}
//  void redraw(View *v) {for(int i=0;i<items.count;i++) items[i]->redraw(v);}
//  void redrawstatic(View *v) {for(int i=0;i<items.count;i++) items[i]->redrawstatic(v);}
//  void click(int x,int y);
//  void buldozere(int x,int y);
/*  template<class T> T *findat(int x,int y) {//najde Static na pozici [x,y],ale pouze typu T
    for(int i=0;i<items.count;i++) if (items[i]->occupy(x,y)) {
      if (T *res=dynamic_cast<T*>(items[i])) return res;
    }
    return NULL;
  }*/
  Static *search(int x,int y);
  Statics();
//  Statics(BINREAD &fr) {game=frgame(fr);fr>>items;}
//  Statics(Game *game,XMLNode xml);
//  void save(BINWRITE &fw) {fw<<items;}
//  RefArray<Static> items;
  TCntVec<Static*> items;
//  TRCArray<Static> items;
//  IObjArray<Static> items;
  void run(double deltatime) {for(int i=0;i<items.count;i++) items[i]->run(deltatime);};
//  void viewchanged() {for(int i=0;i<items.count;i++) items[i]->viewchanged();}
//  void xmlsave(XMLNode xml) {xml.setname("statics");for(int i=0;i<items.count;i++) items[i]->xmlsave(xml.add());}
/*  template<class T> T *findname(string name) {
    for(int i=0;i<items.count;i++) if (T *item=dynamic_cast<T*>((Static*)items[i])) if (item->name==name) return item;
    return NULL;
  }*/
  void octalize() {for(int i=0;i<items.count;i++) items[i]->octalize();}
  void delize() {for(int i=0;i<items.count;i++) items[i]->delize();}
  void clearvisibility() {for(int i=0;i<items.count;i++) items[i]->clearvisibility();}
  void init();
  void save(Archive &ar);
  void load(Archive &ar);
  void afterloadlink();
  void notifyremove(GInstance *obj);
  void builddepo(int x1,int y1,int x2,int y2,int dir) {buildrstation(x1,y1,x2,y2,dir,true);}
  void buildrstation(int x1,int y1,int x2,int y2,int dir,bool isdepo=false);
  void buildsemaphore(int x,int y,string type);
  bool buildbridge(int x,int y,int len,bool ver,float hi,float dhisum,string type);
  bool buildbridge(int x,int y,int len,bool ver,float dhielem,string dhimode,string type);
  bool buildtunnel(int x,int y,int len,bool ver,float hi,float dhisum,string type);
  bool buildtunnel(int x,int y,int len,bool ver,float dhielem,string dhimode,string type);
  void clearmobiles() {for(int i=0;i<items.count;i++) items[i]->clearmobiles();}
  void buildbuilding(int x,int y,int dir,float hi,jtBuilding *bld);
  void buildbuilding(int x,int y,int dir,jtBuilding *bld);
  void buildrunway(int x,int y,bool ver,int len);
  void buildbusstop(int x,int y);
};


struct Station:Static {
  Station(int x,int y,int w,int h):Static(x,y,w,h) {}
  Station(XMLNode xml):Static(xml) {setname(xml.svar("name"));}
  void xmlsave(XMLNode xml) {Static::xmlsave(xml);xml.setvar("name",getname());}
};

struct Building:Static {
  jtBuilding *type;
  int dir;
  float z;

  jtBuilding *gettype() {return type;}
  Building(int x,int y,int w,int h,int adir,float hi,jtBuilding *atype);
  Building(XMLNode xml);
  void render(RenderContext &rc);
  void xmlsave(XMLNode xml);
  void destroy();
  void createblocks();
  void clearvisibility();
  void dodelize();
  void dooctalize();
  void ot_render(int id);
private:
  bool visible;
};


}
#endif
