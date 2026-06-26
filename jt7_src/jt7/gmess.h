#ifndef gmessH
#define gmessH

#include <com/memobjs.h>

namespace jtyc {


class Game;

/*
struct LQItem {
  LQItem *next;
  virtual ~LQItem() {}
  LQItem() {next=NULL;}
};

template<class T> struct ListQeue {
  T *first,*last;
  ListQeue() {first=NULL;last=NULL;}
  ~ListQeue() {clear();}
  void clear() {
    T *act=first;
    while (act) {
      T *act2=act;
      act=(T*)act->next;
      delete act2;
    }
    first=NULL;last=NULL;
  }
  void insert(T *item) {
    if (first) {
      item->next=first;
      first=item;
    } else init(item);
  }
  void add(T *item) {
    if (first) {
      last->next=item;
      item->next=NULL;
      last=item;
    } else init(item);
  }
  T *popfirst() {
    if (first) {
      T *res=first;
      first=(T*)res->next;
      if (first==NULL) last=NULL;
      return res;
    }
    return NULL;
  }
  void erasefirst() {delete popfirst();}
private:
  void init(T *item) {
    first=item;last=item;item->next=NULL;
  }
};
*/

struct Message:LQItem {//slouzi k uschovani zpravy(zpravy jsou zpracovany najednou)
  virtual void process(Game *game,double deltatime)=0;
};

/*struct MMouseMove:Message {
  int x,y;
  void process(Game *game,double deltatime);
  MMouseMove(int ax,int ay) {x=ax;y=ay;}
};*/

struct MMouseDown:Message {
//  int x,y;
  bool right;
  void process(Game *game,double deltatime);
  MMouseDown(bool right):right(right) {}
};
struct MMouseUp:Message {
//  int x,y;
  bool right;
  void process(Game *game,double deltatime);
  MMouseUp(bool right):right(right) {}
//  MMouseUp(int ax,int ay) {x=ax;y=ay;}
};


struct MResize:Message {
  int x,y;
  void process(Game *game,double deltatime);
  MResize(int ax,int ay) {
    x=ax;y=ay;
  }
};

struct MChangeSeeAngle:Message {
  double value;
  void process(Game *game,double deltatime);
  MChangeSeeAngle(double avalue) {value=avalue;}
};
struct MChangeViewAngle:Message {
  double value;
  void process(Game *game,double deltatime);
  MChangeViewAngle(double avalue) {value=avalue;}
};

}
#endif
