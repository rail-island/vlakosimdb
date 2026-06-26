#ifndef gameuH
#define gameuH

#include "gmess.h"
#include "game0.h"

namespace jtyc {

/*
struct GameError {
  int destroy_time;
  string msg;
  bool changed;
  GameError() {changed=true;destroy_time=0;}
};
*/

extern Game *game;

struct GObjectHandler {
  virtual void oncreategobject(GObject *obj)=0;
  virtual void ondeletegobject(int id)=0;
  virtual void onchangegobject(GObject *obj)=0;
  virtual void ongobjectevent(GObject *obj,string event)=0;
  virtual void onpaint()=0;
};

struct Game {
  void iterate(int mx,int my);
  Game();
//  Game(int w,int h);
//  Game(XMLNode xml);
  ~Game();
//  void xmlsave(XMLNode xml);

  View *view;
  RRMap *rrmap;
  RoadMap *roadmap;
  HiMap *himap;
  Statics *statics;
  Mobiles *mobiles;
  SurMap *surmap;
  HiView *hiview;
  XMLNode saving;

  int W,H;
//  GameError error;

  XMLNode getsaving() {return saving;}
  void setstate(State *state);
  void addmsg(Message *msg) {messages.add(msg);}
  int gettime() {return lastclock/double(CLK_TCK);}
  void addvolatile(Volatile *v);
  void seterror(string s);
  void setinfo(string s);
  void stdstate();
  void creatinginst(GInstance *obj);
  void creatingobj(GObject *obj);
  void createdobj(GObject *obj);
  void deletedobj(GObject *obj);
  void deletedinst(GInstance *obj);
  GObject *findobj(int id);
  GObject *findobj(string name);
  void addhandler(GObjectHandler *handler) {handlers.add(handler);}
  void removehandler(GObjectHandler *handler) {handlers.remove(handler);}
  void changedobject(GObject *obj);
  void objectevent(GObject *obj,string event);
  void onpaint();
  void render(RenderContext &rc);
  void rendervolatiles(RenderContext &rc);
  void save(Archive &ar);
  void loaded();
  void removeobj(GObject *obj) {removing.add(obj);}
  string makeuniquename(string base);

  void init(int w,int h);
  void load(Archive &ar);
  int allochint(string msg);
  void freehint(int id);
  Mobile *iswatch(fvector3d *pos=NULL,fvector3d *diff=NULL);
//  Mobile *isdrive(fvector3d *pos=NULL,fvector3d *diff=NULL);
  Mobile *isdrive();
  void unwatch();
  void setwatch(Mobile *value);
  void setdrive(Mobile *value);
  void clearmobiles();
  double getdeltatime() {return lastdeltatime;}
//  void octalize();

  bool destroying;
  string getloadmessages() {return loadmessages.gettext();}
  void addloadmessage(string msg) {loadmessages.add(msg);}

  int run_block,keys_block,click_block,scroll_block;//zablakovani:pokud !=0
private:
  double lastdeltatime;
  Strings loadmessages;

  Mobile *watch;
  Mobile *drive;
  TCntVec<GObjectHandler*> handlers;
  void _init(int w,int h);
  int last_id;
  Volatile *volatiles;
  TCntVec<GObject*> gobjs;
  TCntVec<GObject*> removing;
  TCntVec<GInstance*> ginst;
  ListQeue<Message> messages;
//  static const double msc=CLK_TCK;
  time_t lastclock;
  void iterate(double deltatime,int mx,int my);
  void run(double deltatime);
  void processmessages(double deltatime);
  void clearinst();
};

}
#endif
