#ifndef gtrainH
#define gtrainH

#include "gmobile.h"
#include "grrcom.h"
#include "rdijskr.h"
#include "gmess.h"

namespace jtyc {
class SmokeInstance;

struct ComposeItem {
  jtMachine *machine;
  bool reversed;
  void xmlsave(XMLNode xml);

  bool visible;
};

struct TrainCompose {
  TrainCompose *clone() {return new TrainCompose(*this);}
  TCntVec<ComposeItem> items;
  TrainCompose(const TrainCompose &src) {items.assign(src.items);}
  TrainCompose() {}
  double len();
//  void save(BINWRITE &fw);
//  void load(BINREAD &fr);
  void xmlsave(XMLNode xml);
  TrainCompose(XMLNode xml);
  void countengine(REngine &eng);
  void add(jtMachine *m,bool reverse);
  void clearvisibility();
private:
//  jws::PolyForm *createform() {return createtraincomposeform(this);}
//  string gettext() {return "Slozeni vlaku";}

//  int getcount() {return items.count;}
//  string stringat(int index);
//  void erase(int index) {items.erase(index);}
//  void exchange(int index1,int index2) {items.exchange(index1,index2);}
};

struct Train:Mobile {
//  int heading_to;
  bool exists;//zda opravdu existuje (stoji nebo jede nekde na kolejich)
  bool stopped;
  REngine engine;
  float waitremain;

  OWNER<TrainCompose> compose;
  TrainWay *way;

//  Train(BINREAD &fr);
  Train();//kam se zezacatku postavi
  Train(XMLNode xml);
//  void save(BINWRITE &fw);
  void run(double deltatime);
  void render(RenderContext &rc);
  void rendertransparent(RenderContext &rc);
  void rendervagon(jtMachine *m,double d,bool reverse,RenderContext &rc);//d:vzdalenost od zacatku vlaku
//  bool _start(const RRSPOJ &tt0,RRVRCH pos);//vrati, zda existuje cesta
//  void nextstation();
  bool ocuppy(RItem *ri);
  void crash();
  void crashvagon(double d);
  void crashefect();
  void ignoresems();
  bool init(TCntVec<RRSPOJ> &start);//vrati, zda bylo uspesne, jinak nastavi game->seterror()
  bool init(RStation *rst);//vrati, zda bylo uspesne, jinak nastavi game->seterror()
//  void click(float x,float y);
//  void clickon();
  bool todepo();
  void stop();
  void unstop();
  bool reverse();
  void beforerun();
  bool ismouseon(float x,float y);
//  bool getstopped() {return stopped;}
//  void setstopped(bool value) {if (stopped!=value) {if (value) stop();else unstop();}}
//  string getcompose();
//  void setcompose(string value);
  void loadcompose(XMLNode xml);
  void savecompose(XMLNode xml) {compose->xmlsave(xml);}
  string type_name() {return "Train";}
  void composechanged();
//  void start(RDepo *depo);
//  bool preparedtorunfromdepo();
  bool setdest(RStation *dest);
  bool gotoxy(int x,int y);
//  RStation *getdest() {return NULL;}//headingto?headingto:(RDestination*)way.dest;}
//  string getstatestr() {return statestr;}
  string getstatestr();
//  string getdeststr() {return deststr;}
//  void reacheddepo();
  void save(XMLNode xml);
  void load(XMLNode xml);
  void wait(float secs);
  bool position(fvector3d *res,fvector3d *diff=NULL);
  ///znovu nalezne cestu do cile (vymaze tu aktualne nalezenou)
//  void refindway(const RRSPOJ &tt0,const RRVRCH &pos);
  float getspeed();
  void changed();
  void destroy();
  jtCab *getcab();
  bool getmanual();
  void setmanual(bool value);
  void octalize();
//  void delize();
  void clearvisibility();
  void ot_render(int id);
  Train *totrain() {return this;}
  int getpriority() {return priority;}
  void setpriority(int value) {priority=value;}

//  bool depoblocked;//zda stoji v depu a je blokovan (nemuze vyjet z depa)
  void drivekeydown(int key);
  void updatecabprops();
  XMLNode getcabprops() {updatecabprops();return cabprops;}
private:
  XMLNode cabprops;
  int priority;
//  float smokeimpulstime;
//  bool smokestarted;
  PtrTo<SmokeInstance> smoke;
//  RStation *headingto;
  float tonextchange;//jak dlouho zbyva do dalsi povinne zmeny
  void _loadcompose(XMLNode xml);
//  RDepo *stayingin();
//  void showdest(string adeststr) {deststr=adeststr;changed();}
//  void showstate(string astatestr) {statestr=astatestr;changed();}
//  void updatestate();
//  string deststr,statestr;
//  string statestr;
//  bool restingindepo;
//  jws::PolyForm *createform() {return createtrainform(this);}
//  string gettext() {return "Vlak";}
//  string getactivelabeltext(string id) {
//    if (id=="dest") return deststr;
//    if (id=="state") return statestr;
//    return "";
//  }
};

struct TrainReverse:Message {
  Train *train;
  void process(double deltatime) {train->reverse();}
  TrainReverse(Train *atrain) {train=atrain;}
};

//pokud to jde, vytvori vlak, jinak vrati null a vola game->seterror
//Train *createtrain(RStation *rst);

}

#endif
