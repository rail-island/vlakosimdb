#ifndef gaircraftH
#define gaircraftH

#include "game0.h"
#include "gmobile.h"
#include "imachine.h"

namespace jtyc {

struct ATrajItem {
  float len;
  virtual fpoint pos(float t0)=0;
  virtual fvector diff(float t0)=0;
  virtual void save(XMLNode xml) {xml.setvar("len",len);}
  ATrajItem(XMLNode xml) {len=xml.fvar("len");}
  ATrajItem() {len=-1;}
};
struct ATrajectory {
  IObjArray<ATrajItem> items;
  void count(fpoint p1,float a1,fpoint p2,float a2,float r);
  void addline(fpoint p,fvector d);
  void addarc(fpoint p,float rad,float a1,float a2);
  float len();
  fpoint pos(float t);
  fvector diff(float t);

  void load(XMLNode xml);
  void save(XMLNode xml);
private:
  ATrajItem *finditem(float t,float *t0);
  void add(ATrajItem *item) {items.add(item);}
};


struct Aircraft:Mobile {
  void run(double deltatime);
  void render(RenderContext &rc);
  Aircraft(Airport *start);
  Aircraft(XMLNode xml);
  void save(XMLNode xml);
  bool position(fvector3d *res,fvector3d *diff);
  bool setdest(Airport *dest);
  string type_name() {return "Aircraft";}
  void settype(string type);
  string gettype() {return type->name;}
  bool ismouseon(float x,float y);
  void destroy();

  void findway();//najde cestu do dest, prip. jinam, pokud dest neex.
private:
  Airport *dest;

  Airport *blocking;

  enum State {waiting,starting,rising,flying,lowering,landing} state;
  bool countpos(fvector3d *pos,float *alfa,float *along);

  Airport *startairport;
  float start_s;//startovaci draha
  float start_a;//startovaci zrychleni, CONST

  jtAircraft *type;

  float cur_alfa;
  fvector3d cur_pos;
  float cur_v;

  float rise_dh;//o kolik se uz vystupalo (nebo sestupilo)
  float rise_h0;//odkud se stoupa (resp. klesa) (vyska letiste)

  float rise_t;//CONST
  float rise_zv;//CONST
  float rise_a;//CONST

  float lower_t;//CONST
  float lower_zv;//CONST
  float lower_a;//CONST

  float speed;//CONST

  float start_v;//CONST
  float land_v;//CONST

  float land_a;//pristavaci zrychleni, CONST

  float land_s;//pristavaci draha (>0)

  float fly_t;

  Airport *landairport;

  void initCONST();

  ATrajectory traj;
};

}

#endif
