#ifndef grrbuildH
#define grrbuildH

#include "game0.h"
#include "grrcom.h"
#include "grrmap.h"

namespace jtyc {

struct RRMap;struct BridgeState;

struct ComRBuilder {
  int lastx,lasty;
  bool autofirstandlast;
  Vrchol lastin;
  RRMap *map;
  void buildstep(int x,int y);
  ComRBuilder(RRMap *amap,int ax,int ay) {lastx=ax;lasty=ay;map=amap;autofirstandlast=false;}
  void buildto(int x,int y);
  void erased();
  virtual void buildrr(int x,int y,Spojnice rr)=0;
  virtual ~ComRBuilder();
  bool getautofirstandlast() {return autofirstandlast;}
  void setautofirstandlast(bool value) {autofirstandlast=value;}
};
/*
struct ComRState:State {
  int actx,acty;
  ComRState(Game *game):State(game) {bld=NULL;actx=0;acty=0;}
  void redraw(View *v);

  void mdown(bool right);
  void mup(bool right);
  ComRBuilder *bld;

  virtual ComRBuilder *createbld(RRMap *amap,int ax,int ay)=0;
};
*/

struct RRBuilder:ComRBuilder {
  float dhi;
//  bool denyhichng;
  BuildType mode;
  RRBuilder(RRMap *amap,int ax,int ay):ComRBuilder(amap,ax,ay) {
    dhi=0;
    mode=bt_change;
//    denyhichng=false;
    autofirstandlast=false;
  }
  void buildrr(int x,int y,Spojnice rr);
//  bool getdenychange() {return denyhichng;}
//  void setdenychange(bool value) {denyhichng=value;}
  ///dhi=NOT_USE_DHI:nepouziva dhi, bere automaticky
  float getdhi() {return dhi;}
  void setdhi(float value) {dhi=value;}
  string getmode();
  void setmode(string mode);
};


/*
struct RRState:ComRState {
  int *dhi;bool denyhichng;
  RRState(Game *game,int *adhi,bool adenyhichng):ComRState(game) {dhi=adhi;denyhichng=adenyhichng;}
  ComRBuilder *createbld(RRMap *amap,int ax,int ay) {return new RRBuilder(amap,ax,ay,dhi,denyhichng);}
};

class Tubus;
typedef Tubus *(*TubusCreator)(Game *game,XYREC beg,XYREC end);

//Tubus *createbridgetubus(Game *game,XYREC beg,XYREC end);
//Tubus *createtunneltubus(Game *game,XYREC beg,XYREC end);
Tubus *createtunneltubus(Game *game);
Tubus *createbridgetubus(Game *game,Bridge *type=NULL);

struct RBridgeState:State {
//  TubusCreator tubcr;
  OWNER<Tubus> tubus;
  OWNER<XYREC> start;
  RBridgeState(Game *game,Tubus *tubus):State(game),tubus(tubus) {}
  void mdown(bool right);
  bool getend(XYREC &xy);
  void redraw(View *v);
};

struct Tubus;

//bool buildbridge(Game *game,XYREC beg,XYREC end,TubusCreator tc);
bool buildbridge(Game *game,XYREC beg,XYREC end,Tubus *tubus,bool deletetubus=false);
*/


}

#endif
