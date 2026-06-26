#ifndef game0H
#define game0H

#include <jmath/analgeom.h>
#include <com/memobjs.h>
#include <jmath/geom3d.h>
#include <gr/comgr.h>
#include <ext/xmlnode.h>
#include <com/rcobj.h>
#include <com/ptrto.h>
#include <ext/jarchive.h>
#include <glo/glofont.h>
#include <glo/gloobject.h>
#include <glo/gloocttree.h>
#include "atrajectory.h"
#include "gltools.h"

namespace jtyc {
using namespace jmath;
using namespace glo;

typedef float rdist_t;
const float epsilon=0.0001;

//extern const rdist_t semaphore_dir_0_penalty;

class Game;class Message;class State;class View;class DrawInfo;
class RStation;class Station;class RRMap;class GObject;
class Mobile;class Mobiles;class Static;class Statics;
class Train;class HiMap;class Volatile;
class ColorManager;class jtMachine;class TrainCompose;
class jtBuilding;class RItem;class Images;
//class RDepo;
class jtSemaphore;class jtTerrain;class jtRail;
class jtBridge;class SqBlock;class SItem;class SurMap;class HiBlock;
class Semaphore;class SurRItem;class RstRItem;class BridgeRItem;
class TunnelRItem;class InnerDepoRItem;class OuterDepoRItem;
class DijRVr;class RDestination;class jtStation;class jtCab;
class TempPenaltyItems;//class Tree;
class RailBlock;
class StationInstance;class BridgeInstance;class BridgeItemInstance;
class RBridge;class RTunnel;
class Airport;class Aircraft;
class jtAircraft;class ATrajectory;
class HiView;
class TrackMap;class TrackItem;
class RoadMap;class RoadItem;
class TrackTubus;class TrackBridge;class TrackTunnel;
class MotoWay;class MEngine;
class jtRoad;

const float text_z=1E10,cursor_z=1.1E10;

using namespace gr;

class Game;
extern Game *game;
extern OctTree *gocttree;

struct GOTObject {
  virtual ~GOTObject() {}
  virtual void ot_render(int id) {}
  virtual void markhiview() {}
};

//dostava zpravy o ostatnich objektech
struct GInstance:GOTObject {
  GInstance();
  virtual ~GInstance();
  virtual void notifyremove(GInstance *gi) {}
  void kill() {destroy();delete this;}
protected:
  virtual void destroy() {}
};

struct EBlocked {
  virtual Volatile *createvolatile() {return NULL;}
};

//ma navic identifikaci (retezcovou i ciselnou)
struct GObject:GInstance {
  int id;
  XMLNode loadxml;
  void changed();
  void event(string type);
  virtual string type_name() {return "";}
  void created();
  GObject();
  virtual ~GObject();
  string getname() {return name;}
  void setname(string newname);
  XMLNode getloadxml() {return loadxml;}
  void loaded() {loadxml=NULL;}
  void remove();
  virtual bool canremove() {return true;}
private:
  string name;
};

template<class REAL> inline point3d<REAL> operator *(const agmatrix<REAL> &a,const point3d<REAL> &v) {
  return point3d<REAL>(a.a11*v.x+a.a12*v.y,a.a21*v.x+a.a22*v.y,v.z);
}

struct ZPoint {
  float x,y,h;
  ZPoint(float x,float y,float h):x(x),y(y),h(h) {}
  ZPoint() {x=y=h=0;}
};

inline ZPoint operator +(const ZPoint &a,const ZPoint &b) {return ZPoint(a.x+b.x,a.y+b.y,a.h+b.h);}
inline ZPoint operator *(const ZPoint &a,float k) {return ZPoint(k*a.x,k*a.y,k*a.h);}
inline ZPoint operator /(const ZPoint &a,float k) {return ZPoint(a.x/k,a.y/k,a.h/k);}

template<class T> struct TMatrix {
  T *buf;
  int w,h;
  int size;
  TMatrix() {w=0;h=w=0;buf=NULL;}
  void clear() {if (buf) delete []buf;w=0;h=w=0;buf=NULL;}
  void init(int aw,int ah) {clear();w=aw;h=ah;size=w*h;buf=new T[size];}
  T &at(int x,int y) {xcheckindex(x,w);xcheckindex(y,h);return buf[y*w+x];}
  ~TMatrix() {clear();}
  T &at(int index) {return buf[index];}
  T &operator () (int x,int y) {xcheckindex(x,w);xcheckindex(y,h);return buf[y*w+x];}
  T &operator [] (int index) {xcheckindex(index,size);return buf[index];}
  void assign(TMatrix<T> &src) {clear();init(src.w,src.h);memcpy(buf,src.buf,sizeof(T)*size);}
  void set(int x,int y,const T &value) {buf[y*w+x]=value;}
  void fill(const T &value) {for(int i=0;i<size;i++) buf[i]=value;}
  bool valid(int x,int y) {return x>=0 && y>=0 && x<w && y<h;}
  T *tryat(int x,int y) {if (valid(x,y)) return buf+y*w+x;return NULL;}
};

int loadtexture2d(STRING file,bool transparent=false);

struct GlobalData {
  glo::gloFont font;
  void init();
};

struct OpenGLOpts {
  bool lighting;
  bool heightlines;
  float specular,diffuse,ambient;
  float sunangle;
  float sunydir;
  bool drawtrees;
  
  void load(XMLNode xml);
/*
  float ptcursorsize2=8;//polovina velikosti bodoveho kurzoru

  float train_hi=1;

  double train_mez=0.08;
  double max_train_click_distance=1;
  double max_motocar_click_distance=1;

  void load(XMLNode xml);
*/
};

struct BehaviorOpts {
  float ptcursorsize2;//polovina velikosti bodoveho kurzoru

  float train_hi;

  double train_mez;
  double max_train_click_distance;
  double max_motocar_click_distance;
  float train_speed_koef;
  float motocar_speed_koef;
  rdist_t semaphore_dir_0_penalty;

  double min_train_speed,train_F_t;
  double train_break_koef;

  double train_air_koef_a,train_air_koef_b;

  bool right_sided_semaphores;
/*
  const double min_train_speed=0.3,F_t=0.000005,r_epsilon=0.01;
  const double train_break_koef=20;
*/

  double min_motocar_speed,motocar_F_t;
  double motocar_max_block_time;

  float aircraft_speed_koef;
  double max_aircraft_click_distance;

  float train_force_koef,motocar_force_koef;
/*
  const float aircraft_speed_koef=0.02;
  const double max_aircraft_click_distance=1;
*/
/*
const double min_motocar_speed=0.3,F_t=0;
const double motocar_max_block_time=8;
*/
  void load(XMLNode xml);
};

extern OpenGLOpts oglo;
extern BehaviorOpts beop;

extern GlobalData jtdata;

struct gcolor {
  float r,g,b;
  gcolor(float r,float g,float b):r(r),g(g),b(b) {}
  gcolor(int native_color) {r=nativervalue(native_color)/256.0;g=nativegvalue(native_color)/256.0;b=nativebvalue(native_color)/256.0;}
};

template <class T> inline T *unlink_from_unidir_list(T *list,T *item) {
  T **first=&list;
  while (*first!=item) first=&(*first)->next;
  *first=(*first)->next;
  return list;
}


inline int getflag(int byte) {return 1<<byte;}

struct Interval {
  float lo,hi;
  Interval() {lo=-0x1000;hi=0x1000;}
  bool isin(float x) {return x>=lo && x<=hi;}
  void intersect(Interval &i) {if (i.lo>lo) lo=i.lo;if (i.hi<hi) hi=i.hi;}
};
inline Interval operator *(const Interval &i1,const Interval &i2) {
  Interval res=i1;
  res.intersect(i2);
  return res;
}

extern XMLNode findresource(string name);

void glTextOut(float x,float y,string text,RenderContext &rc);

void rendertitle(RenderContext &rc,string title,float mapx,float mapy,float maphi,bool drawsq=false);
void rendertitle(RenderContext &rc,string title,float x,float y,bool drawsq=false);

//vrati priblizne secs zmenene o neco malo
float randpause(float secs);

#define mac_clear_dirty(type,list,nextname,indication) \
  type *act=*list;\
  *list=NULL;\
  while (act) {\
    type *next=act->nextname;\
    act->indication=false;\
    act->nextname=NULL;\
    act=next;\
  }

template<class T> void cleardirty(T **dirty_list) {
  mac_clear_dirty(T,dirty_list,nextdirty,dirty)
}

/*
template<class T> void cleardirty(T **dirty_list) {
  T *act=*dirty_list;
  *dirty_list=NULL;
  while (act) {
    T *next=act->nextdirty;
    act->dirty=false;
    act->nextdirty=NULL;
    act=next;
  }
}
*/

#define mac_insert_dirty(type,list,newitem,nextname,indication) {\
  type *act=newitem;\
  if (!act->indication) {\
    act->indication=true;\
    act->nextname=*list;\
    *list=act;\
  }\
}

template<class T> void insertdirty(T **dirty_list,T *new_dirty) {
  mac_insert_dirty(T,dirty_list,new_dirty,nextdirty,dirty)
}

/*
template<class T> void insertdirty(T **dirty_list,T *new_dirty) {
  if (!new_dirty->dirty) {
    new_dirty->dirty=true;
    new_dirty->nextdirty=*dirty_list;
    *dirty_list=new_dirty;
  }
}
*/

struct SmallVector3 {
  signed int x:10,y:10,z:10;
  SmallVector3() {x=y=z=0;}
  SmallVector3(const fvector3d &vec);
  fvector3d tofloat();
  operator fvector3d () {return tofloat();}
};

struct SmallFloat1 {
  signed char value;
  SmallFloat1() {value=0;}
  float tofloat() {return value/127.0;}
  operator float () {return tofloat();}
  SmallFloat1(float x) {if (x<-1) x=-1;if (x>1) x=1;value=x*127;}
};

extern float maxmaphi,maxelemhi,hcoordlimit;

namespace mapcolors {
  extern float h1;
  extern float r0,g0,b0;
  extern float r1,g1,b1;
  extern float r2,g2,b2;
}

struct DEST_REACHED {};

struct CommonEngine {
  double power;
  double maxspeed;
  double weight;
  double hi_koef;
  void repair();//zkontroluje nenuvolost, upravi
};


}

using namespace jtyc;
#endif
