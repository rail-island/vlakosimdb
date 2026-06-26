#ifndef gvolatileH
#define gvolatileH

#include "game0.h"
namespace jtyc {

struct Volatile {//nestale objekty(cislo, kolik se vydela penez...)
  Volatile *next;
  virtual bool iterate(double deltatime)=0;//vraci true, pokud objekt uz "vyprchal"
  virtual void render(RenderContext &rc) {}

  Volatile() {next=NULL;}
  virtual ~Volatile() {}
};

struct TimeVolatile:Volatile {
  double destroytime;
  TimeVolatile(double lifetime);
  bool iterate(double deltatime);
//  virtual void work(double deltatime)=0;
  virtual void work(double deltatime) {}
};

struct BadVr:TimeVolatile {
  int x,y;
  BadVr(int ax,int ay):TimeVolatile(2) {x=ax;y=ay;}
//  void work(double deltatime);
  void render(RenderContext &rc);
};

struct BadSq:TimeVolatile {
  int x,y;
  BadSq(int ax,int ay):TimeVolatile(2) {x=ax;y=ay;}
//  void work(double deltatime);
  void render(RenderContext &rc);
};

struct ErrorMessage:TimeVolatile {
  int id;
  ErrorMessage(string msg);
  ~ErrorMessage();
};

struct InfoMessage:TimeVolatile {
  int id;
  InfoMessage(string msg);
  ~InfoMessage();
};

}

#endif
