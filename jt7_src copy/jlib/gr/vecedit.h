#ifndef veceditH
#define veceditH

#include "vecobj.h"
#include "subcnv.h"

namespace gr {
using namespace jlib;


class VECEDIT {//trida slouzici k uprave VECOBJ
  VECOBJ *background;
  int editptidx;
public:
  int jmpx,jmpy;
  void setjmpx(int ax) {jmpx=max(ax,1);}
  void setjmpy(int ay) {jmpy=max(ay,1);}
  void setjmp(int a) {jmpx=jmpy=max(a,1);}
  bool setbackground(VECOBJ *abg,const ABREC &r);//nastavi pozadi tak, ze je cely viditelny
  //v vyrezu r ve fyzickych souradnicich sc
  int mousex,mousey;//fyzicke souradnice(vuci sc) aktualni pozice mysi
  int logmousex,logmousey;//logicke souradnice mysi
  VECELEM *actelem;
  int crosssize,ptsize,ptmaxdist;
  enum STATE {none,point1,point2,point0,addpoint,view,edit,movepoint,editaddpoint,erasepoint,insertpoint} state;
  //none:nic se nedeje,point1:vybira se prvni bod pro objekt typu VEC2PTS
  //point2:vybira se druhy bod(skonceni vybirani:mouse up)
  //point0:vybira se prvni bod pro obj. typu VECPOLY,addpoint:pridavani bodu k VECPOLY
  FIXSUBCANVAS *sc;
  VECOBJ *obj;
  VECEDIT(FIXSUBCANVAS *asc) {background=NULL;sc=asc;state=none;crosssize=10;ptsize=6;ptmaxdist=5;actelem=NULL;obj=NULL;jmpx=1;jmpy=1;}
  void drawcross(int logx,int logy) {sc->getparent().fg(lcblack).penxnor().cross(sc->xltop(logx),sc->yltop(logy),crosssize/2).pencopy();}
  void newobj(VECELEM *elem,STATE newstate);
  void selectobj(int index,STATE newstate);//vybere aktualni objekt,-1:zadny vybrany
  void draw();//vykresli na sc
  int nearpoint(int x,int y,VECELEM *elem);//vrati index nejblizsiho bodu,pokud je dost blizko,
  //x,y jsou FYZICKE souradnice
  int nearline(int x,int y,VECELEM *elem);//vraci index nejblizsi cary
  //-1:nic,0:cara spojujici bod 0 a bod 1....,cnt-1:cara spojujici bod cnt-1 a 0
  void mousemoved(int mx,int my);
  bool mousedown(int mx,int my);//vraci,zda zmenilo seznam objektu
  bool mouseup(int mx,int my);
  void cancel();//zrusi kresleni prave kresleneho objektu
  void drawpts(VECELEM *elem);
  void drawpts2(VECELEM *elem);
  void drawpt(const XYREC &p,LOGICCOLOR color);
  void set0(XYREC p0) {sc->set0(p0.x,p0.y);}
  void setkoef(double koef) {sc->setkoef(koef>0?koef:1);}
};


}
#endif
