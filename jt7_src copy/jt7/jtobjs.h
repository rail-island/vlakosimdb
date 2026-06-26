#ifndef jtobjsH
#define jtobjsH

#include <zaba/zbrt.h>
#include <zaba/zbint.h>
#include <zaba/zbobj.h>
#include <zaba/zbext.h>
#include "game0.h"
#include "gameu.h"
#include "gview.h"
#include "gmess.h"
#include <sdw/sdwsysext.h>
#include <aws/awscomponent.h>
#include "gstatic.h"
#include "gtrain.h"
#include "grrstat.h"
#include "grrbuild.h"
#include "gimages.h"
#include "gmobile.h"
#include "ghimap.h"
#include "grtubus.h"
#include "gsurmap.h"
#include "gairport.h"
#include "gaircraft.h"
#include "groad.h"
#include "gbusstop.h"
#include "gmotocar.h"
#include "jtmobjs.h"

#define enum_types \
  enum_type(jtMachine,machines)\
  enum_type(jtMotocar,motocars)\
  enum_type(jtBuilding,blds)\
  enum_type(jtBridge,bridges)\
  enum_type(jtTunnel,tunnels)\
  enum_type(jtDepo,depos)\
  enum_type(jtSemaphore,sems)\
  enum_type(jtTerrain,terrains)\
  enum_type(jtStation,stations)\
  enum_type(jtTree,trees)\
  enum_type(jtRail,rails)\
  enum_type(jtAircraft,aircrafts)\
  enum_type(jtRoad,roads)\
  enum_type(jtBusStop,busstops)

namespace jtyc {
using namespace zaba;

class zbGame;
extern zbGame *zbgame;
extern sdw::sdwSystemExtension *sysext;

struct zbGameObject:zbStore {
  int id;
  string name;
  string getname();
  void setname(string value);
  GObject *getlink();
  zbGameObject(GObject *obj,Runtime *rt,STRING intf="GameObject.instance");
  bool valid() {return getlink();}
  string tostring () {return getname();}
};

struct zbProxyGameObject:zbGameObject {
  zbProxyGameObject(Runtime *rt,STRING intf="ProxyGameObject.instance"):zbGameObject(new GObject(),rt,intf) {}
};
inline zbObject *newproxygameobject(Runtime *rt) {return new zbProxyGameObject(rt);}

struct zbGame:zbObject,aws::EventHandler,jtyc::GObjectHandler,sdw::SDLEventListener {
  zaba::TObjectArray<zbGameObject> objs;
  zaba::zbObject *handler;

#define enum_type(type,array) zaba::zbArray *zb##array;
enum_types
#undef enum_type

/*
  zaba::zbArray *zbmachines;
  zaba::zbArray *zbblds;
  zaba::zbArray *zbbridges;
  zaba::zbArray *zbtunnels;
  zaba::zbArray *zbdepos;
  zaba::zbArray *zbsems;
  zaba::zbArray *zbterrains;
  zaba::zbArray *zbrsts;
  zaba::zbArray *zbtrees;
  zaba::zbArray *zbrails;
*/

  Game *getlink() {return game;}

  zbGame(Runtime *rt,STRING intf="Game.instance");
  ~zbGame();
  void on_notify(aws::NotifyEvent &event);
  void on_mousemove(aws::MouseMoveEvent &event);
  void on_mouseup(aws::MouseEvent &event);
  void on_mousedown(aws::MouseEvent &event);
  void end();

  void create(int w,int h);
  void load(Archive &ar);
  void init();

  void oncreategobject(GObject *obj);
  void ondeletegobject(int id);
  void onchangegobject(GObject *obj);
  void ongobjectevent(GObject *obj,string event);

  void save(Archive &ar);

  void onpaint();

  zbObject *getobject(int id);
  zbGameObject *convertobj(GObject *obj);
  zbObject *createobject(GObject *obj);
  zbObject *gethandler() {if (handler) return handler->addref();return createVoid(rt);}
  void sethandler(zbObject *value);

  int getmapX() {return game->view->mousesqx;}
  int getmapY() {return game->view->mousesqy;}

  float getmouseX() {return game->view->vpmousepx;}
  float getmouseY() {return game->view->vpmousepy;}

  int getvertexX() {return game->view->mousevx;}
  int getvertexY() {return game->view->mousevy;}

  bool getmouseInMiddle() {return game->view->mouseisinmiddle;}

  string objectType(int id);
  zbObject *objectFromId(int id);
  zbObject *objectFromName(string name);

  void drawCursor(int x,int y,string color);
  void drawCustomCursor(int x,int y,string color,int dir) {game->view->drawsqcursor(x,y,color,dir);}
  void drawCustomCursors(int x1,int y1,int x2,int y2,string color,int dir) {game->view->drawsqcursors(x1,y1,x2,y2,color,dir);}
  void drawComplexCursor(int x,int y,string color);
  void drawPointCursor(int x,int y,string color);
  void drawPointCursors(int x1,int y1,int x2,int y2,string color);
  void drawSqLineCursor(int x,int y,bool vertical,string framecolor,string linecolor);
  void handle_event(SDL_Event &event);

  void block(string what);
  void unblock(string what);

  void buildDepo(int x1,int y1,int x2,int y2,int dir);
  void buildRStation(int x1,int y1,int x2,int y2,int dir);
  void buildBusStop(int x,int y) {game->statics->buildbusstop(x,y);}
  void lookAt(int x,int y) {game->view->lookat(fpoint3d(x,game->himap->interpolatehi(x+0.5,y+0.5),y));}

  zbObject *staticAt(int mapx,int mapy);
  zbObject *mobileAt(float mousex,float mousey);
  zbObject *getstatics();
  zbObject *getmobiles();

  void generateCircles(int steps,float mindisp,float maxdisp,float minrad,float maxrad) {game->himap->circles(steps,mindisp,maxdisp,minrad,maxrad);}
  void clearHeights(float value) {game->himap->clearhis(value);}

  void buildSemaphore(int x,int y,string type) {game->statics->buildsemaphore(x,y,type);}

  float getHi(int x,int y) {HItem *h=game->himap->hat(x,y);if (h) return h->hi;return 0;}
  void setHi(int x,int y,int w,int h,float value);
  void repairHeights(int x,int y,int w,int h,float mhd);
  void eraseRail(int x,int y) {game->rrmap->erase(x,y);}
  void fillWater(int vx,int vy,float waterhi) {game->himap->fillwater(vx,vy,waterhi);}
  void dryWater(int vx,int vy) {game->himap->drywater(vx,vy);}
  void buildBridgeExact(int x,int y,int len,bool ver,float hi,float dhisum,string type);
  void buildBridge(int x,int y,int len,bool ver,float dhielem,string dhimode,string type);
  void buildTunnelExact(int x,int y,int len,bool ver,float hi,float dhisum,string type);
  void buildTunnel(int x,int y,int len,bool ver,float dhielem,string dhimode,string type);
  void buildBuilding(int x,int y,int dir,zbjtBuilding *type);
  void buildBuildingExact(int x,int y,int dir,float hi,zbjtBuilding *type);
  void dryAll() {game->himap->dryall();}
  void plant(int x,int y,string tree) {game->surmap->plant(x,y,tree);}
  void setTerrain(int x,int y,string terrain) {game->surmap->setterrain(x,y,terrain);}
  void setRail(int x,int y,string rail) {game->rrmap->setrailtype(x,y,rail);}
  void setRoad(int x,int y,string road) {game->roadmap->setroadtype(x,y,road);}
  void buildRunway(int x,int y,bool ver,int len) {game->statics->buildrunway(x,y,ver,len);}

  zbObject *getmachines() {return zbmachines->addref();}
  zbObject *getmotocars() {return zbmotocars->addref();}
  zbObject *getbridgeTypes() {return zbbridges->addref();}
  zbObject *gettunnelTypes() {return zbtunnels->addref();}
  zbObject *getstationTypes() {return zbstations->addref();}
  zbObject *getdepoTypes() {return zbdepos->addref();}
  zbObject *gettreeTypes() {return zbtrees->addref();}
  zbObject *getsemaphoreTypes() {return zbsems->addref();}
  zbObject *getterrainTypes() {return zbterrains->addref();}
  zbObject *getrailTypes() {return zbrails->addref();}
  zbObject *getroadTypes() {return zbroads->addref();}
  zbObject *getbusStopTypes() {return zbbusstops->addref();}
  zbObject *getbuildingTypes() {return zbblds->addref();}
  zbObject *getaircraftTypes() {return zbaircrafts->addref();}

  zbObject *getmotocar0();
  zbObject *getmachine0();

  zbObject *searchMachine(string name);

  zbObject *ltop(float x,float y,float h);
  void drawCircle(float x,float y,float xrad,float yrad,string color);
  void drawMapCircle(float x,float y,float xrad,float yrad,string color);
  float getscreenMouseX() {return game->view->scrmousepx;}
  float getscreenMouseY() {return game->view->scrmousepy;}
  float getviewportMouseX() {return game->view->vpmousepx;}
  float getviewportMouseY() {return game->view->vpmousepy;}
  void riseCircle(float x,float y,float rad,float dhi);
  float getdeltaTime() {return game->getdeltatime();}

  string getviewType() {return game->view->gettype();}
  void setviewType(string value) {return game->view->settype(value);}

  void storeRRTemplate(int x,int y,int w,int h,XMLNode xml) {game->rrmap->storerrtemplate(x,y,w,h,xml);}
  void useRRTemplate(int x,int y,int dir8,XMLNode xml) {game->rrmap->userrtemplate(x,y,dir8,xml);}
  void drawRRTemplate(int x,int y,int dir8,XMLNode xml) {game->rrmap->drawrrtemplate(x,y,dir8,xml);}
  void buildRoad(int x,int y,float dhi,string mode) {game->roadmap->build(x,y,dhi,roadmodefromstr(mode));} 
  void eraseRoad(int x,int y) {game->roadmap->erase(x,y);}
};

struct zbjtTerrain:zbjtObject {
  zbjtTerrain(jtTerrain *obj,Runtime *rt,STRING intf="jtTerrain.instance"):zbjtObject(obj,rt,intf) {}
};
struct zbjtTree:zbjtObject {
  jtyc::jtTree *tree;
  string getname() {return getlink()->getname();}
  string gettitle() {return getlink()->gettitle();}
  zbjtTree(jtyc::jtTree *obj,Runtime *rt,STRING intf="jtTree.instance"):zbjtObject(NULL,rt,intf) {tree=obj;}
  jtyc::jtTree *getlink() {return tree;}
};
struct zbjtRail:zbjtObject {
  zbjtRail(jtRail *obj,Runtime *rt,STRING intf="jtRail.instance"):zbjtObject(obj,rt,intf) {}
};
struct zbjtRoad:zbjtObject {
  zbjtRoad(jtRoad *obj,Runtime *rt,STRING intf="jtRoad.instance"):zbjtObject(obj,rt,intf) {}
};
struct zbjtSemaphore:zbjtObject {
  zbjtSemaphore(jtSemaphore *obj,Runtime *rt,STRING intf="jtSemaphore.instance"):zbjtObject(obj,rt,intf) {}
};

struct zbRRBuilder:zbObject {
  PtrTo<RRBuilder> bld;
  RRBuilder *getlink() {return bld;}
  zbRRBuilder(int x,int y,Runtime *rt,STRING intf="RRBuilder.instance");
  void build(int x,int y) {bld->buildto(x,y);}
  int getX() {return bld->lastx;}
  int getY() {return bld->lasty;}
//  zbObject *getdHeight() {int res=bld->getdhi();if (res==NOT_USE_DHI) return createVoid(rt);return createNumber(res,rt);}
//  void setdHeight(zbObject *value) {if (value->tobool()) bld->setdhi(value->toint());else bld->setdhi(NOT_USE_DHI);}
};

struct zbStation:zbGameObject {
  Station *getlink() {return (Station*)zbGameObject::getlink();}
  zbStation(GObject *obj,Runtime *rt,STRING intf="Station.instance"):zbGameObject(obj,rt,intf) {}
};

struct zbAirport:zbStation {
  Airport *getlink() {return (Airport*)zbStation::getlink();}
  zbAirport(GObject *obj,Runtime *rt,STRING intf="Airport.instance"):zbStation(obj,rt,intf) {}
};

struct zbRStation:zbStation {
  RStation *getlink() {return (RStation*)zbStation::getlink();}
  zbRStation(GObject *obj,Runtime *rt,STRING intf="RStation.instance"):zbStation(obj,rt,intf) {}
};
struct zbRDepo:zbRStation {
//  RDepo *getlink() {return (RDepo*)zbGameObject::getlink();}
  zbRDepo(GObject *obj,Runtime *rt,STRING intf="RDepo.instance"):zbRStation(obj,rt,intf) {}
//  zbObject *gettrains();
};
struct zbBridge:zbGameObject {
  TrackBridge *getlink() {return (TrackBridge*)zbGameObject::getlink();}
  zbBridge(GObject *obj,Runtime *rt,STRING intf="Bridge.instance"):zbGameObject(obj,rt,intf) {}
};

struct zbTunnel:zbGameObject {
  TrackTunnel *getlink() {return (TrackTunnel*)zbGameObject::getlink();}
  zbTunnel(GObject *obj,Runtime *rt,STRING intf="Tunnel.instance"):zbGameObject(obj,rt,intf) {}
};

struct zbBuilding:zbGameObject {
  Building *getlink() {return (Building*)zbGameObject::getlink();}
  zbObject *gettype() {return new zbjtBuilding(getlink()->gettype(),rt);}
  zbBuilding(GObject *obj,Runtime *rt,STRING intf="Building.instance"):zbGameObject(obj,rt,intf) {}
};


/*
struct zbTrack:zbGameObject {
  Linka *getlink() {return (Linka*)zbGameObject::getlink();}
  zbTrack(GObject *obj,Runtime *rt,STRING intf="Track.instance"):zbGameObject(obj,rt,intf) {}
};
struct zbRTrack:zbTrack {
  RLinka *getlink() {return (RLinka*)zbGameObject::getlink();}
  zbRTrack(GObject *obj,Runtime *rt,STRING intf="RTrack.instance"):zbTrack(obj,rt,intf) {}
//  zbObject *createMobile(zbRDepo *depo);
};
*/
struct zbMobile:zbGameObject {
  Mobile *getlink() {return (Mobile*)zbGameObject::getlink();}
  zbMobile(GObject *obj,Runtime *rt,STRING intf="Mobile.instance"):zbGameObject(obj,rt,intf) {}
//  zbObject *gettrack();
};
struct zbTrain:zbMobile {
  Train *getlink() {return (Train*)zbMobile::getlink();}
//  void run() {getlink()->unstop();}// setstopped(false);}
  bool goTo(zbRStation *st);
//  zbObject *getheadingTo();
  zbTrain(GObject *obj,Runtime *rt,STRING intf="Train.instance"):zbMobile(obj,rt,intf) {}
  bool start(zbRDepo *depo);
//  bool start(zbRStation *station);
};

struct zbAircraft:zbMobile {
  Aircraft *getlink() {return (Aircraft*)zbMobile::getlink();}
  bool goTo(zbAirport *st);
  zbAircraft(GObject *obj,Runtime *rt,STRING intf="Aircraft.instance"):zbMobile(obj,rt,intf) {}
};

struct zbMotocar:zbMobile {
  Motocar *getlink() {return (Motocar*)zbMobile::getlink();}
  bool goTo(zbBusStop *st);
  zbMotocar(GObject *obj,Runtime *rt,STRING intf="Motocar.instance"):zbMobile(obj,rt,intf) {}
  void start(zbBusStop *stop);
};

struct zbBusStop:zbStation {
  BusStop *getlink() {return (BusStop*)zbStation::getlink();}
  zbBusStop(GObject *obj,Runtime *rt,STRING intf="BusStop.instance"):zbStation(obj,rt,intf) {}
};

/*
struct zbGame:zbObject,aws::EventHandler,jtyc::GObjectHandler {
  PtrTo<Game> game;
  sdw::sdwSystemExtension *sysext;
  zaba::TObjectArray<zbGameObject> objs;
  zaba::zbObject *handler;

  Game *getlink() {return game;}

  zbGame(Runtime *rt,STRING intf="Game.instance");
  ~zbGame();
  void on_notify(aws::NotifyEvent &event);
  void end();

  void create(int w,int h);
  void create(XMLNode xml);
  void init();

  void oncreategobject(GObject *obj);
  void ondeletegobject(int id);
  void onchangegobject(GObject *obj);
  void ongobjectevent(GObject *obj,string event);

  void onpaint();

  zbObject *getobject(int id);
  zbGameObject *convertobj(GObject *obj);
  zbObject *createobject(GObject *obj);
  int getmapX() {return game->view->mousesqx;}
  int getmapY() {return game->view->mousesqy;}
  int getmouseX() {return game->view->mousepx;}
  int getmouseY() {return game->view->mousepy;}

  number_t getviewAngle() {return game->view->viewangle;}
  number_t getseeAngle() {return game->view->seeangle;}
  void setviewAngle(number_t value) {game->addmsg(new MChangeViewAngle(value));}
  void setseeAngle(number_t value) {game->addmsg(new MChangeSeeAngle(value));}
  zbObject *staticAt(int mapx,int mapy);
  zbObject *mobileAt(int mousex,int mousey);
  zbObject *getstatics();
  zbObject *getmobiles();
  zbObject *gettracks();
  zbObject *gethandler() {if (handler) return handler->addref();return createVoid(rt);}
  void sethandler(zbObject *value);
  bool getmouseInMiddle() {return game->view->mouseisinmiddle;}

  string objectType(int id);
  zbObject *objectFromId(int id);
  void drawCursor(int x,int y,string color);
  void drawComplexCursor(int x,int y,string color);
  void drawPointCursor(int x,int y,string color);
};
*/

}

#endif
