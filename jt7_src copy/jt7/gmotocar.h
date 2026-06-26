#ifndef gmotocarH
#define gmotocarH

#include "gmobile.h"
#include "gimages.h"
#include "imotocar.h"
#include "groad.h"
#include "gmess.h"
#include "mdijskr.h"

namespace jtyc {

struct Motocar:Mobile {
  bool exists;
  bool stopped;
  MEngine engine;
  float waitremain;
  MotoTraj *traj;

  Motocar();
  Motocar(XMLNode xml);

  void run(double deltatime);
  void render(RenderContext &rc);

  bool init(RoadItem *ri,int dir);//vrati, zda bylo uspesne, jinak nastavi game->seterror()

  void stop();
  void unstop();
  bool reverse();
//  void beforerun();
  bool ismouseon(float x,float y);

  string type_name() {return "Motocar";}

  bool setdest(RoadItem *dest);
  string getstatestr();
  void save(XMLNode xml);
  void load(XMLNode xml);
  void wait(float secs);
  bool position(fvector3d *res,fvector3d *diff=NULL);
  float getspeed();
  void changed();
  void destroy();
//  jtCab *getcab();
  bool getmanual();
  void setmanual(bool value);
  void octalize();
  void clearvisibility();
  void ot_render(int id);
  string gettype() {return type->name;}
  void settype(string value);
  void start(MWayElem ri);
  void countengine(MEngine &eng);
  void ignorecross();

  jtMotocar *type;
private:
  float tonextchange;//jak dlouho zbyva do dalsi povinne zmeny
  bool visible;
};


}

#endif
