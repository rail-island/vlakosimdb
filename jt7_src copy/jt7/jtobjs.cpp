#include <jlib.h>
#pragma hdrstop

#include "jtobjs.h"
#include <zaba/zbrt.h>
#include <zaba/zbint.h>
#include <sdw/sdwsysext.h>
#include <aws/awssystem.h>
#include "gvolatile.h"

namespace zaba {
#include "lib/jtobjs.h"
}

namespace zaba {
  extern aws::System *awssys;
}

namespace jtyc {
using namespace zaba;
using namespace sdw;
using namespace aws;

zbGame *zbgame=NULL;
sdw::sdwSystemExtension *sysext=NULL;

float perspective_maxdist() {
  return awssys->getopt()["opengl"].fvar("perspective.maxdist");
}

zbGameObject::zbGameObject(GObject *obj,Runtime *rt,STRING intf):zbStore(rt,intf) {
  id=obj->id;
  name=obj->getname();
}

bool zbAircraft::goTo(zbAirport *st) {
  return getlink()->setdest(st->getlink());
}

bool zbMotocar::goTo(zbBusStop *st) {
  return getlink()->setdest(st->getlink()->ri);
}

void zbMotocar::start(zbBusStop *stop) {
  getlink()->start(stop->getlink()->getstartelem());
}

bool zbTrain::goTo(zbRStation *st) {
  return getlink()->setdest(st->getlink());
}

/*
zbObject *zbGame::staticAt(int mapx,int mapy) {
  Static *st=game->statics->search(mapx,mapy);
  return createobject(st);
}

zbObject *zbGame::mobileAt(int mousex,int mousey) {
  Mobile *mob=game->mobiles->search(mousex,mousey);
  return createobject(mob);
}

//zbObject *zbMobile::gettrack() {
//  return zbgame->createobject(getlink()->line);
//}

//zbObject *zbTrain::getheadingTo() {
//  RStation *st=dynamic_cast<RStation*>(getlink()->getdest());
//  return zbgame->createobject(st);
//}
*/


string zbGame::objectType(int id) {
  GObject *obj=game->findobj(id);
  if (obj) return obj->type_name();
  return "";
}
zbObject *zbGame::objectFromId(int id) {
  return getobject(id);
}
zbObject *zbGame::objectFromName(string name) {
  if (GObject *res=game->findobj(name)) return getobject(res->id);
  return createVoid(rt);
}

void zbGame::sethandler(zbObject *value) {
  if (handler) handler->release();
  handler=NULL;
  if (value->tobool()) handler=value->addref();
}

void zbGame::oncreategobject(GObject *obj) {
  ObjectArray pars;
  pars.add(new zbNumber(obj->id,rt));
  if (handler) callMethod(handler,"onCreateGameObject",pars.buf,pars.count);
}
void zbGame::onpaint() {
  ObjectArray pars;
  if (handler) callMethod(handler,"onPaint",pars.buf,pars.count);
}

void zbGame::drawCursor(int x,int y,string color) {
  game->view->drawsqcursor(x,y,gr::strtonativecolor(color.c_str()));
}
void zbGame::drawComplexCursor(int x,int y,string color) {
  game->view->drawsq2cursor(x,y,gr::strtonativecolor(color.c_str()));
}
void zbGame::drawPointCursor(int x,int y,string color) {
  game->view->drawptcursor(x,y,gr::strtonativecolor(color.c_str()));
}
void zbGame::drawPointCursors(int x1,int y1,int x2,int y2,string color) {
  game->view->drawptcursors(x1,y1,x2,y2,gr::strtonativecolor(color.c_str()));
}
void zbGame::drawSqLineCursor(int x,int y,bool vertical,string framecolor,string linecolor) {
  game->view->drawsqlinecursor(x,y,vertical,gr::strtonativecolor(framecolor.c_str()),gr::strtonativecolor(linecolor.c_str()));
}
void zbGame::repairHeights(int x,int y,int w,int h,float mhd) {
  try {
    game->himap->repairrect(x,y,w,h,mhd);
  } catch (HiBlocked &hb) {
    game->addvolatile(new BadVr(hb.x,hb.y));
  }
}
void zbGame::setHi(int x,int y,int w,int h,float value) {
  try {
    game->himap->sethi(x,y,w,h,value);
  } catch (HiBlocked &hb) {
    game->addvolatile(new BadVr(hb.x,hb.y));
  }
}

zbObject *zbGame::ltop(float x,float y,float h) {
  zbStore *res=new zbStore(rt);
  zbNumber *xn=new zbNumber(x,rt);
  zbNumber *yn=new zbNumber(y,rt);
  res->setprop("x",xn);
  res->setprop("y",yn);
  xn->release();
  yn->release();
  return res;
}
void zbGame::drawCircle(float x,float y,float xrad,float yrad,string color) {
  game->view->drawcircle(x,y,xrad,yrad,gr::strtonativecolor(color.c_str()));
}
void zbGame::riseCircle(float x,float y,float rad,float dhi) {
  game->himap->risecircle(x,y,rad,dhi);
}
void zbGame::drawMapCircle(float x,float y,float xrad,float yrad,string color) {
  game->view->drawmapcircle(x,y,xrad,yrad,gr::strtonativecolor(color.c_str()));
}


/*
string zbGame::getbridgeTypes() {
  bstring res;
  for(int i=0;i<gimages->bridges.count;i++) res<<gimages->bridges[i]->name<<"\n";
  return res;
}
string zbGame::gettunnelTypes() {
  bstring res;
  for(int i=0;i<gimages->tunnels.count;i++) res<<gimages->tunnels[i]->name<<"\n";
  return res;
}
string zbGame::getsemaphoreTypes() {
  bstring res;
  for(int i=0;i<gimages->sems.count;i++) res<<gimages->sems[i]->name<<"\n";
  return res;
}
string zbGame::getrstTypes() {
  bstring res;
  for(int i=0;i<gimages->stations.count;i++) res<<gimages->stations[i]->name<<"\n";
  return res;
}
string zbGame::getdepoTypes() {
  bstring res;
//  for(int i=0;i<gimages->depos.count;i++) res<<gimages->depos[i]->name<<"\n";
  return res;
}
string zbGame::gettreeTypes() {
  bstring res;
  for(int i=0;i<gimages->trees.count;i++) res<<gimages->trees[i]->getname()<<"\n";
  return res;
}
string zbGame::getterrainTypes() {
  bstring res;
  for(int i=0;i<gimages->terrains.count;i++) res<<gimages->terrains[i]->getname()<<"\n";
  return res;
}
string zbGame::getrailTypes() {
  bstring res;
  for(int i=0;i<gimages->rails.count;i++) res<<gimages->rails[i]->getname()<<"\n";
  return res;
}
*/
void zbGame::ondeletegobject(int id) {
}
void zbGame::onchangegobject(GObject *obj) {
  int idx=-1;
  for(int i=0;i<objs.count;i++) if (objs[i]->id==obj->id) idx=i;
  if (idx<0) return;
  zbObject *zbo=objs[idx]->getprop("handler");
  if (zbo) zaba::callMethod(zbo,"changedObject",NULL,0);
}
void zbGame::ongobjectevent(GObject *obj,string event) {
  int idx=-1;
  for(int i=0;i<objs.count;i++) if (objs[i]->id==obj->id) idx=i;
  if (idx<0) return;
  zbObject *zbo=objs[idx]->getprop("handler");
  if (zbo) {
    zbObject *str=new zbString(event,rt);
    zaba::callMethod(zbo,"objectEvent",&str,1);
    str->release();
  }
}

zbGameObject *zbGame::convertobj(GObject *obj) {
  if (RStation *res=dynamic_cast<RStation*>(obj)) {
    if (res->isdepo) return new zbRDepo(res,rt);
    return new zbRStation(res,rt);
  }
//  if (RDepo *res=dynamic_cast<RDepo*>(obj)) return new zbRDepo(res,rt);
  if (Building *res=dynamic_cast<Building*>(obj)) return new zbBuilding(res,rt);
  if (TrackTunnel *res=dynamic_cast<TrackTunnel*>(obj)) return new zbTunnel(res,rt);
  if (TrackBridge *res=dynamic_cast<TrackBridge*>(obj)) return new zbBridge(res,rt);
  if (Train *res=dynamic_cast<Train*>(obj)) return new zbTrain(obj,rt);
  if (Aircraft *res=dynamic_cast<Aircraft*>(obj)) return new zbAircraft(obj,rt);
  if (Airport *res=dynamic_cast<Airport*>(obj)) return new zbAirport(obj,rt);
  if (BusStop *res=dynamic_cast<BusStop*>(obj)) return new zbBusStop(obj,rt);
  if (Motocar *res=dynamic_cast<Motocar*>(obj)) return new zbMotocar(obj,rt);
//  if (RLinka *res=dynamic_cast<RLinka*>(obj)) return new zbRTrack(res,rt);

  if (Mobile *res=dynamic_cast<Mobile*>(obj)) return new zbMobile(obj,rt);
//  if (Linka *res=dynamic_cast<Linka*>(obj)) return new zbTrack(obj,rt);
  return NULL;
}

zbObject *zbGame::createobject(GObject *obj) {
  if (!obj) return createVoid(rt);
  return getobject(obj->id);
}

zbObject *zbGame::getobject(int id) {
  for(int i=0;i<objs.count;i++) if (objs[i]->id==id) return objs[i]->addref();
  GObject *o=game->findobj(id);
  if (o) {
    zbGameObject *res=convertobj(o);
    if (res) {
      objs.add(res);res->addref();
      return res;
    }
  }
  return createVoid(rt);
}

zbGame::~zbGame() {
#define enum_type(type,array) if (zb##array) zb##array->release();
enum_types
#undef enum_type
/*
  if (zbmachines) zbmachines->release();
  if (zbblds) zbblds->release();
  if (zbbridges) zbbridges->release();
  if (zbtunnels) zbtunnels->release();
  if (zbdepos) zbdepos->release();
  if (zbsems) zbsems->release();
  if (zbterrains) zbterrains->release();
  if (zbrsts) zbrsts->release();
  if (zbtrees) zbtrees->release();
  if (zbrails) zbrails->release();
*/
  if (handler) handler->release();
  if (game) game->removehandler(this);
  awssys->removehandler(this);
  sysext->set_listener(NULL);
}

void zbGame::end() {
  zbgame=NULL;
  awssys->removehandler(this);
  delete game;
  game=NULL;
}

void zbGame::buildTunnelExact(int x,int y,int len,bool ver,float hi,float dhisum,string type) {
  try {
    game->statics->buildtunnel(x,y,len,ver,hi,dhisum,type);
  } catch (EBlocked &b) {
    if (Volatile *v=b.createvolatile()) game->addvolatile(v);
    game->seterror("$cannot-build-tunnel");
  }
}
void zbGame::buildTunnel(int x,int y,int len,bool ver,float dhielem,string dhimode,string type) {
  try {
    game->statics->buildtunnel(x,y,len,ver,dhielem,dhimode,type);
  } catch (EBlocked &b) {
    if (Volatile *v=b.createvolatile()) game->addvolatile(v);
    game->seterror("$cannot-build-tunnel");
  }
}
void zbGame::buildBuilding(int x,int y,int dir,zbjtBuilding *type) {
  try {
    game->statics->buildbuilding(x,y,dir,type->getlink());
  } catch (EBlocked &b) {
    if (Volatile *v=b.createvolatile()) game->addvolatile(v);
    game->seterror("$cannot-build-building");
  }
}
void zbGame::buildBuildingExact(int x,int y,int dir,float hi,zbjtBuilding *type) {
  try {
    game->statics->buildbuilding(x,y,dir,hi,type->getlink());
  } catch (EBlocked &b) {
    if (Volatile *v=b.createvolatile()) game->addvolatile(v);
    game->seterror("$cannot-build-building");
  }
}
/*
zbObject *zbGame::getbuildingTypes() {
  zbArray *res=new zbArray(rt);
  for(int i=0;i<gimages->blds.count;i++) res->items.add(new zbjtBuilding(gimages->blds[i],rt));
  return res;
}
*/

void zbGame::buildBridgeExact(int x,int y,int len,bool ver,float hi,float dhisum,string type) {
  try {
    game->statics->buildbridge(x,y,len,ver,hi,dhisum,type);
  } catch (EBlocked &b) {
    if (Volatile *v=b.createvolatile()) game->addvolatile(v);
    game->seterror("$cannot-build-bridge");
  }
}
void zbGame::buildBridge(int x,int y,int len,bool ver,float dhielem,string dhimode,string type) {
  try {
    game->statics->buildbridge(x,y,len,ver,dhielem,dhimode,type);
  } catch (EBlocked &b) {
    if (Volatile *v=b.createvolatile()) game->addvolatile(v);
    game->seterror("$cannot-build-bridge");
  }
}

void zbGame::buildDepo(int x1,int y1,int x2,int y2,int dir) {
  game->statics->builddepo(x1,y1,x2,y2,dir);
}
void zbGame::buildRStation(int x1,int y1,int x2,int y2,int dir) {
  game->statics->buildrstation(x1,y1,x2,y2,dir);
}

zbObject *zbGame::getmotocar0() {
  return new zbjtMotocar(gimages->motocar0,rt);
}
zbObject *zbGame::getmachine0() {
  return new zbjtMachine(gimages->machine0,rt);
}
zbObject *zbGame::searchMachine(string name) {
  return new zbjtMachine(gimages->findmachine(name),rt);
}


string zbGameObject::getname() {
  GObject *obj=getlink();
  if (obj) return obj->getname();
  return name;
}

void zbGameObject::setname(string value) {
  GObject *obj=getlink();
  if (obj) {
    obj->setname(value);
    name=obj->getname();
  }
}

GObject *zbGameObject::getlink() {
  GObject *res=game->findobj(id);
  if (res) return res;
  res=game->findobj(name);
  if (res) id=res->id;
  return res;
}

void zbGame::handle_event(SDL_Event &event) {
  if (event.type==SDL_KEYDOWN) game->view->keydown(event.key.keysym.sym);
  if (event.type==SDL_KEYUP) game->view->keyup(event.key.keysym.sym);
}

zbGame::zbGame(Runtime *rt,STRING intf):zbObject(rt,intf) {
#define enum_type(type,array) zb##array=NULL;
enum_types
#undef enum_type
/*
  zbmachines=NULL;
  zbblds=NULL;
  zbbridges=NULL;
  zbtunnels=NULL;
  zbdepos=NULL;
  zbsems=NULL;
  zbterrains=NULL;
  zbrsts=NULL;
  zbtrees=NULL;
  zbrails=NULL;
*/
  zbgame=this;
  handler=NULL;
  sysext->set_listener(this);
//  gsystem->setbits(32,false);
}

void zbGame::create(int w,int h) {
  game=new Game();
  game->init(w,h);
}
void zbGame::load(Archive &ar) {
  game=new Game();
//  Archive ar;
//  ar.open(file);
  game->load(ar);
//  ar.close();
}

void definetexture(string name,string file) {
  texman->gettexture(name,file,NULL);
}

void cleartextures() {
  texman->clear();
}


void zbGame::init() {
  game->addhandler(this);
//  init_tt_cons();
//  init_tt_bmps(2);
  DesktopOpts dop;
  sysext->getdesktopopts(dop);
  game->addmsg(new MResize(dop.w,dop.h));
  game->addmsg(new MChangeSeeAngle(M_PI/3));
  game->addmsg(new MChangeViewAngle(M_PI/4));
  awssys->addhandler(this);
//  typedef Tree jtTree;
#define enum_type(type,array) \
  zb##array=new zbArray(rt);\
  for(int i=0;i<gimages->array.count;i++) {\
    type *m=gimages->array[i];\
    zb##array->items.add(new zb##type(m,rt));\
  }
  enum_types
#undef enum_type
}

static int *find_block(string what) {
  if (what=="scroll") return &game->scroll_block;
  if (what=="keys") return &game->keys_block;
  if (what=="click") return &game->click_block;
  if (what=="run") return &game->run_block;
  return NULL;
}

void zbGame::block(string what) {
  if (int *b=find_block(what)) (*b)++;
}

void zbGame::unblock(string what) {
  if (int *b=find_block(what)) (*b)--;
}

void zbGame::save(Archive &ar) {
//  Archive ar;
//  ar.create(file);
  game->save(ar);
//  ar.close();
}


#define checknogame\
  if (zbgame) {\
    ir->nativeerror("Game can have only one instance");\
    return createVoid(ir->rt);\
  }

zbObject *newgame(int w,int h,Interpreter *ir) {
  checknogame
  beop.load(awssys->getopt()["behavior"]);
  want_images();
  zbGame *res=new zbGame(ir->rt);
  res->create(w,h);
  res->init();
  return res;
}
zbObject *loadgame(Archive &ar,Interpreter *ir) {
  checknogame
  beop.load(awssys->getopt()["behavior"]);
  want_images();
  zbGame *res=new zbGame(ir->rt);
  res->load(ar);
  res->init();
  return res;
}


void zbGame::on_mousemove(aws::MouseMoveEvent &event) {
  if (event.istype("ondesktopmousemove")) {
    ObjectArray pars;
    if (handler) callMethod(handler,"onMouseMove",pars.buf,pars.count);
  }
}

static string buttontostr(MouseButton b) {
  if (b==mb_right) return "right";
  if (b==mb_left) return "left";
  return "left";
}

void zbGame::on_mouseup(aws::MouseEvent &event) {
  if (event.istype("ondesktopmouseup")) {
    ObjectArray pars;
    pars.add(createString(buttontostr(event.button),rt));
    if (handler) callMethod(handler,"onMouseUp",pars.buf,pars.count);
  }
}
void zbGame::on_mousedown(aws::MouseEvent &event) {
  if (event.istype("ondesktopmousedown")) {
    ObjectArray pars;
    pars.add(createString(buttontostr(event.button),rt));
    if (handler) callMethod(handler,"onMouseDown",pars.buf,pars.count);
  }
}

void zbGame::on_notify(aws::NotifyEvent &event) {
  if (event.istype("onidle")) {
    int x,y;
    beop.load(awssys->getopt()["behavior"]);
    sysext->getmousepos(&x,&y);
    game->iterate(x,y);
    sysext->repaintgl();
  } else if (event.istype("ondrawgl")) {
    RenderContext rc;
    rc.push();
//    glAlphaFunc(GL_NOTEQUAL,0.0);
    glAlphaFunc(GL_GEQUAL,0.5);
    glDepthFunc(GL_LEQUAL);
    oglo.load(awssys->getopt()["opengl"]);
    beop.load(awssys->getopt()["behavior"]);
    game->render(rc);
    rc.pop();
  }
}

zbObject *newrrbuilder(int x,int y,Runtime *rt) {return new zbRRBuilder(x,y,rt);}

zbRRBuilder::zbRRBuilder(int x,int y,Runtime *rt,STRING intf):zbObject(rt,intf) {
  bld=new RRBuilder(game->rrmap,x,y);
}


zbObject *zbGame::getstatics() {
  zbArray *res=new zbArray(rt);
  for(int i=0;i<game->statics->items.count;i++) res->items.add(createobject(game->statics->items[i]));
  return res;
}

zbObject *zbGame::getmobiles() {
  zbArray *res=new zbArray(rt);
  for(int i=0;i<game->mobiles->items.count;i++) res->items.add(createobject(game->mobiles->items[i]));
  return res;
}

zbObject *zbGame::staticAt(int mapx,int mapy) {
  Static *st=game->statics->search(mapx,mapy);
  return createobject(st);
}

zbObject *zbGame::mobileAt(float mousex,float mousey) {
  Mobile *mob=game->mobiles->search(mousex,mousey);
  return createobject(mob);
}


zbObject *newtrain(Runtime *rt) {
  Train *t=new Train();
  return zbgame->createobject(t);
}

bool zbTrain::start(zbRDepo *depo) {
  return getlink()->init(depo->getlink());
}


zbObject *newaircraft(zbAirport *airport,Runtime *rt) {
  Aircraft *a=new Aircraft(airport->getlink());
  return zbgame->createobject(a);
}

zbObject *newmotocar(Runtime *rt) {
  Motocar *a=new Motocar();
  return zbgame->createobject(a);
}

/*
zbObject *zbRDepo::gettrains() {
  zbArray *res=new zbArray(rt);
  TCntVec<Train*> trains;
  getlink()->gettrains(trains);
  for(int i=0;i<trains.count;i++) res->items.add(zbgame->createobject(trains[i]));
  return res;
}
*/


/*

//void zbTrain::start(zbRDepo *depo) {
//  getlink()->start(depo->getlink());
//}

zbObject *zbGame::getstatics() {
  zbArray *res=new zbArray(rt);
  for(int i=0;i<game->statics->items.count;i++) res->items.add(createobject(game->statics->items[i]));
  return res;
}

zbObject *zbGame::getmobiles() {
  zbArray *res=new zbArray(rt);
  for(int i=0;i<game->mobiles->items.count;i++) res->items.add(createobject(game->mobiles->items[i]));
  return res;
}

zbObject *zbGame::gettracks() {
  zbArray *res=new zbArray(rt);
  for(int i=0;i<game->lines->items.count;i++) res->items.add(createobject(game->lines->items[i]));
  return res;
}

*/

/*
zbObject *zbRTrack::createMobile(zbRDepo *depo) {
  RLinka *l=getlink();
  Mobile *train=l->createmobile(depo->getlink());
  return zbgame->createobject(train);
}
*/


}

namespace zaba {
using namespace jtyc;

#include "lib/jtobjs.cc"
}

