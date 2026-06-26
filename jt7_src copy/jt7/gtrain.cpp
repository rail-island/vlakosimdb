#include <jlib.h>
#pragma hdrstop

#include "gtrain.h"
#include "grrstat.h"
#include "gview.h"
#include "gameu.h"
#include "grrmap.h"
//#include <zbuf/bmp3d.cpp>
#include "gimages.h"
#include <jmath/geom3d.cpp>
#include <sdl/sdl.h>
#include "imachine.h"
#include "ismoke.h"
//#include "gcrash.h"
//#include "gsystem.h"

namespace jtyc {

void ComposeItem::xmlsave(XMLNode xml) {
  xml.setvar("name",machine->name);
  xml.setvar("reverse",reversed);
}

void TrainCompose::xmlsave(XMLNode xml) {
  xml.setname("compose");
  for(int i=0;i<items.count;i++) items[i].xmlsave(xml.add("item"));
}
TrainCompose::TrainCompose(XMLNode xml) {
  for(int i=0;i<xml.count();i++) {
    string name=xml[i].svar("name");
    if (jtMachine *m=gimages->findmachine(name)) add(m,xml[i].ivar("reverse"));
  }
}
void TrainCompose::add(jtMachine *m,bool reverse) {
  ComposeItem &ci=items.add();
  ci.machine=m;ci.reversed=reverse;
}

void TrainCompose::countengine(REngine &eng) {
  eng.maxspeed=1000*beop.train_speed_koef;eng.power=0;eng.weight=0;
  eng.hi_koef=0.0001;
  for(int i=0;i<items.count;i++) {
    eng.power+=items[i].machine->power*1;
    eng.weight+=items[i].machine->weight*15;
    eng.maxspeed=min(eng.maxspeed,items[i].machine->maxspeed*beop.train_speed_koef);
  }
  eng.repair();
}
float Train::getspeed() {
  return way->realtraj->speed/beop.train_speed_koef;
}

double TrainCompose::len() {
  double res=0;
  for(int i=0;i<items.count;i++) res+=items[i].machine->len;
  return res;
}
void Train::composechanged() {
  if (compose) {
    compose->countengine(engine);
    way->settrainlen(compose->len()+beop.train_mez*(compose->items.count-1));
  }
}

bool Train::init(TCntVec<RRSPOJ> &start) {
  if (!compose || compose->len()<0.01) {
    game->seterror("$Train-is-empty");
    return false;
  }
  exists=true;
  way->init(start);
  return true;
}

bool Train::init(RStation *rst) {
  TCntVec<RRSPOJ> start;
  if (!compose) {
    game->seterror("$No-train-compose");
    return false;
  }
  if (!rst->getstarttrack(start,compose->len())) {
//    game->seterror("$Not-free-rail-detected");
    return false;
  }
  return init(start);
}

void Train::updatecabprops() {
  cabprops.setvar("speed",round(getspeed()));
}

Train::Train() {
  cabprops=XMLNode::create();
//  smokeimpulstime=0;
  priority=0;
//  smokestarted=false;
  waitremain=0;
  tonextchange=0;
  exists=false;
//  depoblocked=false;
  setname(lang_text_ch("$Train 1"));
  game->mobiles->items.add(this);
  way=new TrainWay(this);
  created();
//  startdepo->trains.add(this);
  stopped=true;
}
void Train::crashefect() {
  double act=0;
  for(int i=0;i<compose->items.count;i++) {
    double actlen=compose->items[i].machine->len;
    crashvagon(act+actlen/2);
    act+=actlen;
    act+=beop.train_mez;
  }
  way->clear();
}

string Train::getstatestr() {
  if (stopped) return "stopped";
  if (waitremain>0) return "waiting";
  if (way->headingtodepo) return "todepo";
  if (way->gotox>=0) return "xy";
  if (way->realtraj->no_way) return "noway";
  return "go";
}

void Train::ignoresems() {
  way->ignoresems();
  changed();
}

void Train::wait(float secs) {
  bool was=waitremain>0;
  waitremain+=secs;
  if (was) changed();
}

void Train::stop() {
  stopped=true;way->stop();
  changed();
}

bool Train::reverse() {
  way->realtraj->trim();
  bool res=way->reverse();
  changed();
  return res;
}

bool Train::todepo() {
//  headingto=NULL;
  if (way->headingtodepo) {
    way->notodepo();
    changed();
    return true;
  }
  if (way->todepo()) {
    changed();
    return true;
  }
  game->seterror("$Not-reachable-depo");
  changed();
//    changedstate();
//    stop();
//    return false;
//  }
//  game->seterror("$Not-reachable-depo");
  return false;
}
bool Train::ismouseon(float x,float y) {
  double act=0;
  if (!compose) return false;
  for(int i=0;i<compose->items.count;i++) {
    double actlen=compose->items[i].machine->len;
    point3d<float> pt;
    try {
      pt=way->count(act+actlen/2);
    } catch (TRAIN_NOT_VISIBLE) {
      break;
    }
    ZPoint za=game->view->ltop(fpoint3d(pt.x,pt.z,pt.y));
    if (fdistance(za,fpoint(x,y))<=beop.max_train_click_distance*game->view->maptype.scale) return true;
    act+=actlen;
    act+=beop.train_mez;
  }
  return false;
}

void Train::crash() {
//  Mobile::crash();
}
bool Train::ocuppy(RItem *ri) {
  return way->realtraj->items.searchtype(ri)>=0;
}
void Train::run(double deltatime) {
  if (!exists || !compose) return;
  if (!smoke && compose->items.count>0 && compose->items[0].machine->smoke) {
    jtMachine *m=compose->items[0].machine;
    smoke=m->smoke->create();
/*
    smoke=new ParticleEngine();
    smoke->data=*compose->items[0].machine->smoke;
    smoke->start();
*/
  }
  try {
    jtMachine *m=compose->items[0].machine;
    fvector3d diff;
    fpoint3d pt=way->count(m->smoke_x,&diff);
    float sp=getspeed()*beop.train_speed_koef;
    diff=diff*sp;
    exchange(pt.y,pt.z);
    exchange(diff.y,diff.z);
    pt.y+=m->smoke_y;
    if (smoke) smoke->iterate(deltatime,pt,diff);
  } catch (...) {
  }
/*
  if (smoke) {
    smokeimpulstime+=deltatime;
    if (smokeimpulstime>smoke->data.interval) smoke->impuls(),smokeimpulstime=0;
    try {
      fvector3d diff;
      fpoint3d pt=way->count(smoke->data.x0,&diff);

      smoke->x0=pt.x;
      smoke->y0=pt.z+smoke->data.y0;
      smoke->z0=pt.y;

      float sp=getspeed()*train_speed_koef;
      smoke->vx=diff.x*sp;
      smoke->vy=diff.z*sp;
      smoke->vz=diff.y*sp;

      if (!smokestarted) {
        smoke->start();
        smokestarted=true;
      }
    } catch (...) {
      smoke->vx=0;
      smoke->vy=0;
      smoke->vz=0;
    }
    smoke->iterate(deltatime);
  }
*/
//  if (depoblocked) return;
  bool waswaiting=waitremain>0;
  if (waitremain>0) {
    waitremain-=deltatime;
    if (waitremain<0) {
      deltatime+=waitremain;
      waitremain=0;
    } else return;
  }
  if (waswaiting) changed();
  retry:
  if (!stopped || getmanual())
  try {
//    way.iterate(deltatime/500.0);
    way->iterate(deltatime,engine);
    if (tonextchange<=0) changed();
    tonextchange-=deltatime;
//  } catch (NO_WAY) {
//    event("noway");
//    goto retry;
//    todepo();return;
  } catch (DEST_REACHED) {
    event("destreached");
    changed();
//    goto ]retry;
//    nextstation();goto retry;
  } catch (NO_DEST) {
    event("nodest");
    changed();
  }

}

void Train::crashvagon(double d) {
  point3d<float> a;
  try {
    a=way->count(d);
  } catch (TRAIN_NOT_VISIBLE) {
    return;
  }
//  game->addvolatile(new Crash(game,a,4));
}

//void Train::updatestate() {
//  if (stopped) showstate("Vlak stoji");else showstate("Vlak jede");
//}

/*
void Train::reacheddepo() {
  stop();
  way->clear();
  way->realtraj->clear();
  changed();
}
*/

void Train::unstop() {
  if (!exists) return;
  
  if (!stopped) return;

  stopped=false;
  changed();

/*
  if (way->hasposition()) {
    stopped=false;
    changed();
  } else {
    RDepo *depo=stayingin();
    if (depo) {
      stopped=false;
      depo->starttrain(this);
      changed();
    }
//    else return;
  }
*/
//  updatestate();
}

/*
void Train::refindway(const RRSPOJ &tt0,const RRVRCH &pos) {
  way->refindway(tt0,pos);
}
*/

void Train::save(XMLNode xml) {
  Mobile::save(xml);
  xml.setname("train");
  if (compose) compose->xmlsave(xml.add("compose"));else xml.add("compose");
  if (stopped) xml.setvar("stopped",stopped);
//  if (depoblocked) xml.setvar("depoblocked",depoblocked);
  xml.setvar("priority",priority);
  xml.setvar("waitremain",waitremain);
  xml.setvar("exists",exists);
  way->save(xml.add("way"));
  game->saving=xml;
  event("save");
//  engine.save(xml.add("engine"));
}
Train::Train(XMLNode xml):Mobile(xml) {
  cabprops=XMLNode::create();
//  smokeimpulstime=0;
  priority=0;
//  smokestarted=false;
  way=new TrainWay(this);
  waitremain=0;
  tonextchange=0;
  load(xml);
}
void Train::load(XMLNode xml) {
  loadxml=xml;
  _loadcompose(xml["compose"]);
  waitremain=xml.fvar("waitremain");
  stopped=xml.ivar("stopped");
//  depoblocked=xml.ivar("depoblocked");
  priority=xml.ivar("priority");
  if (xml.svar("exists")=="") exists=true;else exists=xml.ivar("exists");
  way->load(xml["way"]);
  composechanged();
}

jtCab *Train::getcab() {
  if (compose && compose->items.count>0) return compose->items[0].machine->cab;
  return NULL;
}

bool Train::position(fvector3d *res,fvector3d *diff) {
  try {
    *res=way->count(0,diff);
    return true;
  } catch (TRAIN_NOT_VISIBLE) {
    return false;
  }
}

bool Train::getmanual() {
  if (way) return way->getmanual();
  return false;
}
void Train::setmanual(bool value) {
  if (way) way->setmanual(value);
}

void Train::octalize() {
  if (!exists || !compose) return;
  double act=0;
  try {
    for(int i=0;i<compose->items.count;i++) {
      jtMachine *m=compose->items[i].machine;
      fpoint3d pt=way->count(act+m->d1);
      gloBoundBox bnd=m->bound;
      bnd.extendrotate();
      bnd.translate(pt.x,pt.z,pt.y);
      float a[3]={bnd.x1,bnd.y1,bnd.z1},b[3]={bnd.x2,bnd.y2,bnd.z2};
      gocttree->add(otItem(this,i),a,b,true);
      act+=compose->items[i].machine->len;
      act+=beop.train_mez;
    }
  } catch (...) {
  }
}
void Train::ot_render(int id) {
  if (compose) compose->items[id].visible=true;
}
/*
void Train::delize() {
  gocttree->removegroup(this);
}
*/

void TrainCompose::clearvisibility() {
  for(int i=0;i<items.count;i++) items[i].visible=false;
}

void Train::clearvisibility() {
  if (compose) compose->clearvisibility();
}

void Train::beforerun() {
  way->beforerun();
}

void Train::rendervagon(jtMachine *m,double d,bool reverse,RenderContext &rc) {
  point3d<float> a,b,mid;
  try {
    a=way->count(d+m->d1);
    b=way->count(d+m->d2);
    mid=way->count(d+m->len/2);
  } catch (TRAIN_NOT_VISIBLE) {
    return;
  }
//  if (game->view->type) {
//    if (! game->view->type->visible(fpoint3d(mid.x,mid.z,mid.y))) return;
//  } else {
//    if (! game->view->nearvisible(game->view->ltop(fpoint3d(mid.x,mid.z,mid.y)),m->diam)) return;
//  }
  fvector3d e1=b-a;
  fvector3d e2(xyzrec<float>(agvector<float>(e1.xy()).getnormal(),0));

  e2=e2.size1();
  e1=e1.size1();
  vector3d<float> e3=(e1%e2).size1();
  matrix3d<float> mat;
  mat.tobase(e1,e2,e3);//a je matice prechodu od konanicke baze k bazi aktualniho natoceni

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  glTranslatef(a.x,a.z,a.y);
  glMultiplySwapYZ(mat);

  if (reverse) {
    glTranslatef(m->len-2*m->d1,0,0);
    glRotatef(180,0,1,0);
//    glScalef(-1,1,1);
  }

  m->render(rc);
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void Train::changed() {
  Mobile::changed();
  tonextchange=randpause(2);
}

/*
bool Train::preparedtorunfromdepo() {
  return compose && !stopped;// && headingto;
}
*/

void Train::rendertransparent(RenderContext &rc) {
//  for(int i=0;i<way->items.count;i++) {
//    RItem *ri=way->items[i]->tt.ri;
//    game->view->drawsqcursor(ri->x,ri->y,gcolor(0,1,1));
//  }
  if (smoke) smoke->render(rc);
}

void Train::render(RenderContext &rc) {
  if (!compose) return;
  if (!way->canredraw()) return;
  double act=0;
  for(int i=0;i<compose->items.count;i++) {
    if (compose->items[i].visible) rendervagon(compose->items[i].machine,act,compose->items[i].reversed,rc);
    act+=compose->items[i].machine->len;
    act+=beop.train_mez;
  }
}
//bool Train::_start(const RRSPOJ &tt0,RRVRCH pos) {
//  xassert(headingto);
//  try {
//  way.start(tt0,headingto->dest(),pos);
//  headingto=NULL;
//    headingto=NULL;
//  } catch(NO_WAY) {
//    showstate("Nenalezena cesta");
//    return false;
//  }
/*  heading_to=0;
  if (line->items.count<1) return false;
  try {
    showdest();
    way.start(tt0,dynamic_cast<RStation&>(*line->items[heading_to]).dest(),pos);
  } catch(NO_WAY) {showstate("Nenalezena cesta");return false;}*/
//  return true;
//}

void Train::drivekeydown(int key) {
  if (key==SDLK_r) reverse();
}

void Train::destroy() {
  Mobile::destroy();
  way->kill();
  way=NULL;
}

/*
RDepo *Train::stayingin() {
  for(int i=0;i<game->statics->items.count;i++)
    if (RDepo *depo=dynamic_cast<RDepo*>(game->statics->items[i]))
      if (depo->trains.search(this)>=0)
        return depo;
  return NULL;
}
*/

bool Train::gotoxy(int x,int y) {
  if (!exists) return false;
  bool res=way->gotoxy(x,y);
  changed();
  return res;
}

bool Train::setdest(RStation *dest) {
  if (!exists) return false;
  bool res=way->setdest(dest);
//  bool res=way->setdest(dest->dest());
  changed();
  return res;
/*
  if (RDepo *depo=stayingin()) {
    bool res=way->start(depo->trainstartspoj(),dest->dest(),depo->trainstartvr());
    changed();
    return res;
//  way.start(tt0,headingto->dest(),pos);

  } else {
    bool res=way->setdest(dest->dest());
    changed();
    return res;
  }
*/
}

/*
void Train::nextstation() {
  heading_to++;if (heading_to>=line->items.count) heading_to=0;
  RStation &st=dynamic_cast<RStation&>(*line->items[heading_to]);
  showdest();
  way.setdest(st.dest());
}
*/
/*
string Train::getcompose() {
  string res;
  for(int i=0;i<compose->items.count;i++) res<<compose->items[i]->name<<";";
  return res;
}
void Train::setcompose(string value) {
  Strings s;
  compose=new TrainCompose();
//  compose->items.clear();
  s.fromtext(value,';');
  for(int i=0;i<s.count;i++) {
    if (Machine *m=gimages->machines.searchvalue(s[i].c_str())) compose->add(m);
  }
  if (compose->items.count==0) compose=NULL;
  composechanged();
}
*/
void Train::_loadcompose(XMLNode xml) {
  compose=new TrainCompose(xml);
  if (compose->items.count==0) compose=NULL;
  composechanged();
}

void Train::loadcompose(XMLNode xml) {
  if (exists) {
    game->seterror("$Train-exists");
    return;
  }
  _loadcompose(xml);
}



}

//#pragma package(smart_init)


