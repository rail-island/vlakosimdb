#ifndef coloralgH
#define coloralgH

#include <ext/xmlnode.h>
#include <jmath/geom3d.h>

namespace glo {
using namespace jmath;

struct ColorCondItem {
  string s;
  bool mustnot,mustbe,canbe;
  ColorCondItem() {mustnot=false,mustbe=false,canbe=false;}
};

struct ColorCond {
  bool start;
  ColorCond() {start=true;}
  IObjArray<ColorCondItem> items;
  bool accept(string faceid);
  void load(string s);
};

struct ColorSideItem {
  ColorCond cond;
  int dir;

  //case material:
  //false:
  float x1,y1,x2,y2;//umisteni textury
  //true:
  float dx,dy,xscale,yscale;
  string rkoef,gkoef,bkoef;//zabarveni textury materialu
  bool transp;
  bool denylight;

  bool material;

  fvector3d n;
  bool loaded;
  string name;
  bool transparent;
  ColorSideItem() {denylight=false;loaded=false;transparent=false;material=false;dir=0;rkoef="1";gkoef="1";bkoef="1";}
  void afterload();
//  void load(int dir,XMLNode xml);
  void set(int dir,float x1,float y1,float x2,float y2,string texname,bool transparent,string acond);
  void setmaterial(int dir,float dx,float dy,float xscale,float yscale,string texname,bool transparent,bool transp,string acond);
};

struct ColorAlg {
//  void loadcolor(int dir,XMLNode xml) {if (dir>=0 && dir<6) colors[dir].load(dir,xml);}
  void addside(int dir,float x1,float y1,float x2,float y2,string texname,bool transparent,string cond,string rkoef,string gkoef,string bkoef,bool denylight);
  void addmaterialside(int dir,float dx,float dy,float xscale,float yscale,string texname,bool transparent,string rkoef,string gkoef,string bkoef,bool transp,string cond,bool denylight);
  bool run(XMLNode polygon);

  IObjArray<ColorSideItem> items;
//  ColorSideItem colors[6];
};

struct StampAlg {
  ColorCond cond;
  string condstr;
  StampAlg() {x1=y1=x2=y2=0;dir=0;maxdist=M_PI_2;transp=reflects=reflectt=false;xscale=1;yscale=1;material=false;rkoef="1";bkoef="1";gkoef="1";dist=0;}

  direct_property(string,texture)
  direct_property(float,x1)
  direct_property(float,x2)
  direct_property(float,y1)
  direct_property(float,y2)
  direct_property(float,maxdist) //maximalni odchylka normaly polygonu od smeru razitek ve stupnich
  direct_property(bool,transp)
  direct_property(bool,reflectt)
  direct_property(bool,reflects)
  direct_property(int,dir)
  direct_property(bool,material)
  direct_property(float,xscale)
  direct_property(float,yscale)
  direct_property(string,rkoef)
  direct_property(string,gkoef)
  direct_property(string,bkoef)
  direct_property(float,dist)
  void setcondition(string value) {cond.load(value);condstr=value;}
  string getcondition() {return condstr;}

  bool run(XMLNode polygon,XMLNode addhere);
};

}

#endif
