#include <jlib.h>
#pragma hdrstop

#include "gameu.h"
#include "gview.h"
#include "ghimap.h"
#include "gimages.h"
#include "gsurmap.h"
//#include "gsystem.h"
#include "grrmap.h"
#include "groad.h"
#include "gstatic.h"
//#include "glines.h"
#include "gmobile.h"
#include "gvolatile.h"
#include "rdijskr.h"
#include "ghiview.h"
#include <sdw/sdwsysext.h>

namespace jtyc {

extern sdw::sdwSystemExtension *sysext;

Game *game=NULL;

Game::~Game() {
  clearinst();
}

void Game::iterate(int mx,int my) {
  time_t t=clock();
  int time=gettime();
//  if (time>error.destroy_time) error.msg="",error.changed=true;
  iterate((t-lastclock)/double(CLK_TCK),mx,my);
  lastclock=t;
}

Mobile *Game::iswatch(fvector3d *pos,fvector3d *diff) {
  fvector3d res;
  if (watch && watch->position(&res,diff)) {
    if (pos) *pos=res;
    return watch;
  }
  return NULL;
}
Mobile *Game::isdrive() {
  return drive;
}
/*
Mobile *Game::isdrive(fvector3d *pos,fvector3d *diff) {
  fvector3d res;
  if (drive && drive->position(&res,diff)) {
    if (pos) *pos=res;
    return drive;
  }
  return NULL;
}
*/

void Game::unwatch() {
  watch=NULL;
  if (drive) drive->setmanual(false);
  drive=NULL;
  view->setmapview();
  view->setcab(NULL);
//  himap->changed();
}

void Game::iterate(double deltatime,int mx,int my) {
  lastdeltatime=deltatime;
  run(deltatime);
  processmessages(deltatime);
  view->iterate(deltatime,mx,my);
  Volatile *act=volatiles,*last=NULL;
  while (act) {
    if (act->iterate(deltatime)) {
      Volatile *next=act->next;
      if (last) last->next=next;else volatiles=next;
      delete act;
      act=next;
    } else last=act,act=act->next;
  }
  while (removing.count>0) {
    if (removing[0]->canremove()) removing[0]->kill();
    else removing.erase(0);
  }
}

void Game::rendervolatiles(RenderContext &rc) {
  Volatile *act=volatiles,*last=NULL;
  while (act) {
    act->render(rc);
    act=act->next;
  }
}

string Game::makeuniquename(string base) {
  if (!findobj(base)) return base;
  if (base[base.length()-1]=='1') base.erase(base.length()-1,1);
  int idx=1;
  for (;;) {
    string test=base+itos(idx);
    if (!findobj(test)) return test;
    idx++;
  }
}

void Game::clearinst() {
  destroying=true;
  for(int i=0;i<ginst.count;i++) delete ginst[i];
//  for(int i=0;i<ginst.count;i++) {
//    eprintf("~%s()\n",typeid(*ginst[i]).name());
//    if (!dynamic_cast<TrainWay*>(ginst[i]))
//    delete ginst[i];
//  }
//  delete ginst.last();
//  while (ginst.count>0) delete ginst[0];
  destroying=false;
}

void Game::addvolatile(Volatile *v) {
  v->next=volatiles;
  volatiles=v;
}
void Game::creatingobj(GObject *obj) {
  obj->id=++last_id;
  gobjs.add(obj);
}
void Game::creatinginst(GInstance *obj) {
  ginst.add(obj);
}
void Game::deletedinst(GInstance *obj) {
  if (!destroying) {
    if (obj==watch) watch=NULL;
    if (obj==drive) drive=NULL;
    ginst.quickremove(obj);
    for(int i=0;i<ginst.count;i++) ginst[i]->notifyremove(obj);
  }
}
void Game::createdobj(GObject *obj) {
  for(int i=0;i<handlers.count;i++) handlers[i]->oncreategobject(obj);
}
void Game::deletedobj(GObject *obj) {
  gobjs.quickremove(obj);
  removing.quickremove(obj);
  for(int i=0;i<handlers.count;i++) handlers[i]->ondeletegobject(obj->id);
}
void Game::changedobject(GObject *obj) {
  for(int i=0;i<handlers.count;i++) handlers[i]->onchangegobject(obj);
}
void Game::objectevent(GObject *obj,string event) {
  for(int i=0;i<handlers.count;i++) handlers[i]->ongobjectevent(obj,event);
}
void Game::onpaint() {
  for(int i=0;i<handlers.count;i++) handlers[i]->onpaint();
}


GObject *Game::findobj(int id) {
  for(int i=0;i<gobjs.count;i++) if (gobjs[i]->id==id) return gobjs[i];
  return NULL;
}
GObject *Game::findobj(string name) {
  for(int i=0;i<gobjs.count;i++) if (gobjs[i]->getname()==name) return gobjs[i];
  return NULL;
}

void Game::_init(int w,int h) {
  W=w;H=h;
  himap->init();
  view->init();
  rrmap->init();
  roadmap->init();
  surmap->init();
  statics->init();
  mobiles->init();
  lastclock=clock();
  hiview->init();
}

void Game::init(int w,int h) {
  _init(w,h);
//  octalize();
}

void Game::load(Archive &ar) {
  MainXMLNode xml;
  ar.loadxml("main.xml",xml);
  _init(xml.ivar("W"),xml.ivar("H"));
  himap->load(ar);
  rrmap->load(ar);
  roadmap->load(ar);
  statics->load(ar);
  mobiles->load(ar);
  view->load(ar);
  surmap->load(ar);

  rrmap->afterloadlink();
  roadmap->afterloadlink();
  statics->afterloadlink();
  lastclock=clock();
  watch=dynamic_cast<Mobile*>(findobj(xml.svar("watch")));
  drive=dynamic_cast<Mobile*>(findobj(xml.svar("drive")));

//  octalize();
}

/*
void Game::octalize() {
  himap->octalize();
}
*/

void Game::loaded() {
  for(int i=0;i<gobjs.count;i++) gobjs[i]->loaded();
  lastclock=clock();
}

void Game::clearmobiles() {
  mobiles->clearall();
  rrmap->clearmobiles();
  roadmap->clearmobiles();
  statics->clearmobiles();
}

Game::Game() {
  watch=NULL;
  drive=NULL;
  lastdeltatime=0;
  last_id=0;W=H=0;volatiles=NULL;
  run_block=keys_block=click_block=scroll_block=0;
  saving=XMLNode::create();
  game=this;
  destroying=false;
  view=new View();
  hiview=new HiView();
  himap=new HiMap();
  rrmap=new RRMap();
  roadmap=new RoadMap();
  surmap=new SurMap();
  statics=new Statics();
  mobiles=new Mobiles();
}

void Game::render(RenderContext &rc) {
  view->render(rc);
}

void Game::setwatch(Mobile *value) {
//  himap->changed();
  view->setmapview();
  view->setcab(NULL);
//  view->type=NULL;
  setdrive(NULL);
  watch=value;
}
void Game::setdrive(Mobile *value) {
  if (drive) drive->setmanual(false);
//  view->type=NULL;
//  himap->changed();
  drive=value;watch=value;
  if (drive) {
    drive->setmanual(true);
    view->setdriveview();
    view->setcab(drive->getcab());
  }
}

void Game::save(Archive &ar) {
  MainXMLNode xml;
  xml.setname("terrain");
  xml.setvar("W",W);
  xml.setvar("H",H);
  if (watch) xml.setvar("watch",watch->getname());
  if (drive) xml.setvar("drive",drive->getname());
  ar.savexml("main.xml",xml);
  himap->save(ar);
  rrmap->save(ar);
  roadmap->save(ar);
  statics->save(ar);
  view->save(ar);
  mobiles->save(ar);
  surmap->save(ar);
}

int Game::allochint(string msg) {
//  error.msg=s;
//  error.destroy_time=gettime()+2000;
//  error.changed=true;
  sdw::DesktopOpts dop;
  sysext->getdesktopopts(dop);
  sysext->sethint(0,dop.h-8,0,0,msg);
  return 0;
}

void Game::seterror(string s) {
  addvolatile(new ErrorMessage(lang_text_ch(s)));
}
void Game::setinfo(string s) {
  addvolatile(new InfoMessage(s));
}
void Game::freehint(int id) {
  sysext->closehint(id);
}

void Game::run(double deltatime) {
  mobiles->beforerun();
  statics->run(deltatime);
  mobiles->run(deltatime);
  rrmap->run(deltatime);
  roadmap->run(deltatime);
//  wmap->run(deltatime);
}
void Game::processmessages(double deltatime) {
  while (Message *m=messages.popfirst()) {
    m->process(this,deltatime);
  }
}



}
//#pragma package(smart_init)

