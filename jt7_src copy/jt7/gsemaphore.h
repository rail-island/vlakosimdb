#ifndef gsemaphoreH
#define gsemaphoreH

#include "game0.h"
#include "grrcom.h"

namespace jtyc {

struct Sem_Rec {
  RItem *ri;
  Vrchol in;
  bool blocked;
};

struct Semaphore:GObject {
  int dir;//obousmerny,1:jen v kladnem smeru,-1:jen v zapornem smeru
  bool pfree,nfree;//zda je volno z pozitivni,resp. negativni strany
//  BackRef<RItem> ri;
  RItem *ri;
  Spojnice tt;
  bool blocked;//true,pokud vlak prejel uz semafor, na false nastavi trainout
  //pokud vlak ceka na semafor, je false

  void update();
  //pri prejezdu vlaku pres tento semafor:upravi sebe a prilehle strany sousednich semaforu
  void trainin() {update();}
  void trainout() {blocked=false;update();}
  void setfree(bool isfree,Vrchol v);
  void settype(jtSemaphore *atype);
  void run(double deltatime);
  Semaphore(RItem *ri,Spojnice tt,int adir,jtSemaphore *atype);
  Semaphore(RItem *ri,XMLNode xml);
  bool isfree(Vrchol v);
  bool isblocked() {return blocked;}
  void trainover() {blocked=true;update();}
  static void updatesems(RItem *ri);
  void xmlsave(XMLNode xml);
  void render(RenderContext &rc);
  void buildnext();
  void notifyremove(GInstance *obj);
  void clearmobiles() {nfree=pfree=true;blocked=false;}
  rdist_t dijskr_penalty(Spojnice tt);
  void get_neightboors(TCntVec<Sem_Rec> &res,Vrchol v);
  jtSemaphore *gettype() {return type;}
private:
  jtSemaphore *type;
  float gradt;
  float lastgradtime;
  float gradsum;
  float pgrad,ngrad;
  void _render(bool isfree,fpoint pos,fvector diff,bool positivedir,float gradrest,RenderContext &rc);
//  void _redraw(View *v,bool isfree,fpoint pos,fvector diff,ZBitmap *bmp);
};

bool free_sem_square(RItem *ri); 

bool sem_isinverse(Spojnice tt);

}


#endif
