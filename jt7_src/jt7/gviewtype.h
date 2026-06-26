#ifndef gviewtypeH
#define gviewtypeH

#include "game0.h"

namespace jtyc {

struct ViewType:otClasifier,otProjector {
//  virtual bool visible(fpoint3d &pt)=0;
  virtual ZPoint ltop(const fpoint3d &pt)=0;
  virtual void resized() {}
  virtual void iterate(unsigned char *keystate,double deltatime) {}
  virtual bool countprojmatrix()=0;
  virtual bool applyprojmatrix()=0;
  virtual bool paintgame() {return false;}
  virtual void clear() {}
  void project(float in[3],float out[2]);
  virtual bool stdrender() {return true;}//zda se renderuje normalne (ne u cartview)
  virtual void dorender(RenderContext &rc) {}//vola se, kdyz pokud stdrender() vrati false
  virtual string name() {return "";}
};

struct MapCartViewType:ViewType {
  float dx,dy;
  float scale;
  float scalex,scaley;
  int scrolldist;
  float leftmove,topmove,rightmove,bottommove;
  float movevelocity;
  float scalechangevelocity;

  void updatescalexy();
  void clear();
  void resized();
  virtual void changedtr() {}
  void iterate(unsigned char *keystate,double deltatime);

  virtual float minscale()=0;
  virtual float maxscale()=0;

  MapCartViewType();
};

struct MapViewType:MapCartViewType {
  float seeangle;//0..90
  float viewangle;//0..360
  float viewrotatevelocity;
  float seerotatevelocity;

  agmatrix<float> rotmat,rotinvmat;

  float fz,fy;

  ZPoint ltop(const fpoint3d &pt);

  MapViewType();
  void changedtr();
  void iterate(unsigned char *keystate,double deltatime);
  bool countprojmatrix();
  bool applyprojmatrix();
  bool paintgame() {return true;}
  void project(float in[3],float out[2]);
  int visiblecube(float coords[3],float size) {return ot_map_visible_cube(*this,coords,size);}
  string name() {return "map";}

  float minscale() {return 0.05;}
  float maxscale() {return 1;}
};

struct CartViewType:MapCartViewType {
  CartViewType();

  ZPoint ltop(const fpoint3d &pt);
  void iterate(unsigned char *keystate,double deltatime);
  bool paintgame() {return true;}
  int visiblecube(float coords[3],float size);
  bool stdrender() {return false;}
  void dorender(RenderContext &rc);
  string name() {return "cart";}
  bool countprojmatrix();
  bool applyprojmatrix();

  float minscale() {return 0.001;}
  float maxscale() {return 0.1;}
};


}

#endif


