#ifndef spaceeditH
#define spaceeditH

#include <zbuf/bmp3d.h>
#include <ext/xmldoc.h>
#include <gr/comgr.h>

namespace zbuf {

typedef COLORANDLINE<int> Lcolor;

inline Lcolor logictofg(gr::LOGICCOLOR fg) {
  return Lcolor(gr::reversergb(fg.getnative()),true);
};
inline Lcolor logictobg(gr::LOGICCOLOR bg) {
  return Lcolor(gr::reversergb(bg.getnative()),false);
};

typedef BMP3D<Lcolor> SPBMP;
typedef VOXELARRAY<Lcolor,float> LVoxels;

struct SPAction {
  virtual void perform(SPBMP &bmp,int x0,int y0,int z0,double koef)=0;
  virtual void editprops() {}
  virtual string getname()=0;
  virtual void save(XMLItem &xml)=0;

  static SPAction *loadaction(XMLItem &xml,XMLItem *images);
};

struct SPPolyAction:SPAction {
  IObjArray<SPAction> items;
  void perform(SPBMP &bmp,int x0,int y0,int z0,double koef) {
    for(int i=0;i<items.count;i++) items[i]->perform(bmp,x0,y0,z0,koef);
  }
  string getname() {return "polyaction";}
  void save(XMLItem &xml) {
    xml.name="poly";
    for(int i=0;i<items.count;i++) items[i]->save(xml.add());
  }
  void load(XMLItem &xml,XMLItem *images) {
    items.clear();
    for(int i=0;i<xml.count();i++) if (SPAction *a=loadaction(xml[i],images)) items.add(a);
  }
  SPPolyAction(XMLItem &xml,XMLItem *images) {load(xml,images);}
  SPPolyAction() {}
};

struct SPBOX:SPAction {
  int x1,y1,z1,x2,y2,z2;
  int color;
  SPBOX() {x1=y1=z1=x2=y2=z2=color=0;}
  void editprops() {
    jsystem->editvalues("Vlastnosti krabice:",'i',&color,"barva",'i',&x1,"x1",'i',&y1,"y1",'i',&z1,"z1",'i',&x2,"x2",'i',&y2,"y2",'i',&z2,"z2",0);
  }
  void perform(SPBMP &bmp,int x0,int y0,int z0,double koef) {
    bmp.fillbox((x1+x0)*koef,(y1+y0)*koef,(z1+z0)*koef,(x2+x0)*koef,(y2+y0)*koef,(z2+z0)*koef,color);
  }
  string getname() {return fmtstr("box [%d,%d,%d] [%d,%d,%d]",x1,y1,z1,x2,y2,z2);}
  void save(XMLItem &xml) {
    xml.name="box";
    xml.vars.setvar("x1",x1);
    xml.vars.setvar("y1",y1);
    xml.vars.setvar("z1",z1);
    xml.vars.setvar("x2",x2);
    xml.vars.setvar("y2",y2);
    xml.vars.setvar("z2",z2);
    xml.vars.setvar("color",color);
    xml.doc->defines.setvar("box","x1,y1,z1,x2,y2,z2");
  }
  SPBOX(XMLItem &xml) {
    x1=xml.ivars["x1"];
    y1=xml.ivars["y1"];
    z1=xml.ivars["z1"];
    x2=xml.ivars["x2"];
    y2=xml.ivars["y2"];
    z2=xml.ivars["z2"];
    color=xml.ivars["color"];
  }
};

struct SPImageAction:SPAction {
  string file;
  XMLItem *images;
  int imageindex;//-1:ze souboru
  string imagename() {
    if (images and imageindex>=0) return (*images)[imageindex].svars["name"];
    return sFile(file).getfile();
  }
  void save(XMLItem &xml) {
    xml.vars.setvar("file",file);
    if (images and imageindex>=0) xml.setvar("image",(*images)[imageindex].svars["name"]);
  }
  SPImageAction(XMLItem *aimages) {file=basedir/"images"/".img";images=aimages;imageindex=-1;}
  SPImageAction(XMLItem &xml,XMLItem *aimages) {
    images=aimages;
    file=xml.svars["file"];
    imageindex=-1;
    if (images) {
      for(int i=0;i<images->count();i++)
        if (xml.svars["image"]==(*images)[i].svars["name"]) imageindex=i;
    }
  }
  void perform(SPBMP &bmp,int x0,int y0,int z0,double koef);
protected:
  virtual void perform(XMLItem &xml,SPBMP &bmp,int x0,int y0,int z0,double koef)=0;
};

struct SPCut:SPImageAction {
  int dir;

  void editprops();
  string getname() {
    char *dirs[]={"X","Y","Z"};
    return fmtstr("cut [%s] %s",dirs[dir],imagename().c_str());
  }
  void save(XMLItem &xml) {
    xml.name="cut";
    xml.vars.setvar("direction",dir);
    SPImageAction::save(xml);
  }
  SPCut(XMLItem &xml,XMLItem *aimages):SPImageAction(xml,aimages) {
    images=aimages;
    dir=xml.ivars["direction"];
  }
  SPCut(XMLItem *aimages):SPImageAction(aimages) {dir=0;}
protected:
  void perform(XMLItem &xml,SPBMP &bmp,int x0,int y0,int z0,double koef);
};

struct SPColor:SPImageAction {
  int dir;
  int numcolor;
  int linesasfill;

  void editprops();
  string getname() {
    char *dirs[]={"X-","X+","Y-","Y+","Z-","Z+"};
    return fmtstr("color [%s] %s",dirs[dir],imagename().c_str());
  }
  void save(XMLItem &xml) {
    xml.name="color";
    xml.vars.setvar("direction",dir);
    xml.vars.setvar("numcolor",numcolor);
    xml.vars.setvar("linesasfill",linesasfill);
    SPImageAction::save(xml);
  }
  SPColor(XMLItem &xml,XMLItem *aimages):SPImageAction(xml,aimages) {
    dir=xml.ivars["direction"];
    numcolor=xml.ivars["numcolor"];
    linesasfill=xml.ivars["linesasfill"];
  }
  void perform(XMLItem &xml,SPBMP &bmp,int x0,int y0,int z0,double koef);
  SPColor(XMLItem *aimages):SPImageAction(aimages) {dir=0;numcolor=1;linesasfill=false;}
};


struct SPBMPWrapper {
  SPPolyAction actions;
  int xs,ys,zs,x0,y0,z0;
  int color;
  double quality;
  double koef;
  SPBMP *redraw() {
    jsystem->showwait("Vytvari se model...");
    SPBMP *res=new SPBMP(xs*quality*koef,ys*quality*koef,zs*quality*koef,gr::clnnone);
    res->clear(color);
    actions.perform(*res,x0,y0,z0,quality*koef);
    jsystem->hidewait();
    return res;
  }
//  VOXELARRAY<uchar,float> *makevoxs();
  LVoxels *makevoxs();
  void editprops() {
    jsystem->editvalues("Vlastnosti modelu:",'d',&quality,"Kvalita",'d',&koef,"Zvetseni",'i',&xs,"Velikost X",'i',&ys,"Velikost Y",'i',&zs,"Velikost Z",'i',&x0,"X0",'i',&y0,"Y0",'i',&z0,"Z0",'i',&color,"Barva",0);
    if (quality<1) quality=1;if (quality>2) quality=2;
    if (koef<=0) koef=1;
  }
  SPBMPWrapper(int xsize,int ysize,int zsize) {
    xs=xsize;ys=ysize;zs=zsize;
    x0=0;y0=0;z0=0;quality=1.5;color=0;koef=1;
  }
  void save(jlib::XMLItem &xml) {
    actions.save(xml);
    xml.vars.setvar("x0",x0);
    xml.vars.setvar("y0",y0);
    xml.vars.setvar("z0",z0);
    xml.vars.setvar("xsize",xs);
    xml.vars.setvar("ysize",ys);
    xml.vars.setvar("zsize",zs);
    xml.vars.setvar("quality",quality);
    xml.vars.setvar("koef",koef);
    xml.vars.setvar("color",color);
  }
  void load(jlib::XMLItem &xml,XMLItem *images) {
    x0=xml.ivars["x0"];
    y0=xml.ivars["y0"];
    z0=xml.ivars["z0"];
    xs=xml.ivars["xsize"];
    ys=xml.ivars["ysize"];
    zs=xml.ivars["zsize"];
    quality=xml.fvars["quality"];
    koef=xml.fvars["koef"];
    color=xml.ivars["color"];
    if (quality<1) quality=1;if (quality>2) quality=2;
    if (koef<=0) koef=1;
    actions.load(xml,images);
  }
  void save(string file) {
    jlib::XMLDocument xml;
    save(xml.main);
    xml.save(file.c_str());
  }
  void load(string file,XMLItem *images) {
    jlib::XMLDocument xml;
    xml.load(file);
    load(xml.main,images);
  }
  void save(STROUT &fw) {
    jlib::XMLDocument xml;
    save(xml.main);
    xml.csave(fw);
  }
  void analyse(const char *text,XMLItem *images) {
    jlib::XMLDocument xml;
    xml.analyse(text);
    load(xml.main,images);
  }
};

extern sFile vxadir;

}
#endif
