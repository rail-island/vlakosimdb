#ifndef obj2deditH
#define obj2deditH

#include "gredit.h"
#include "objs2d.h"
#include "subcnv.h"
#include <com/memobjs.h>

namespace gr {

const double GRmaxdisttomove=5;
const int GRcircleradius=3;
const int GRcrosssize=10;

XYREC transform_ltop(void *data,XYREC src);
XYREC transform_ptol(void *data,XYREC src);
class GRStateCom;
typedef void (*StateEvent)(void *param,GRStateCom *sender);
typedef void (*MouseMoveEvent)(void *param,GRStateCom *sender,int x,int y);
typedef void (*HintEvent)(void *param,GRStateCom *sender,string hint);
typedef void (*ListChangeEvent)(void *param,GRStateCom *sender);


struct GRTransform {
  int x0,y0;
  double kx,ky;
  int jmpx,jmpy;
  int editwi,edithi;
  bool drawos;
  int dleft,dtop;
  GRTransform() {x0=0;y0=0;kx=1;ky=1;jmpx=1;jmpy=1;drawos=false;dleft=dtop=10;}
  void setjmpx(int value) {jmpx=max(1,value);}
  void setjmpy(int value) {jmpy=max(1,value);}
  void setkoef(double k) {
    if (fabs(k)<0.0001) k=1;
    kx=ky=k;
  }
  void ptol(int &x,int &y) {
    y=edithi-y;
    x=x/kx-x0;y=y/ky-y0;
  }
  void ltop(int &x,int &y) {
    x=(x+x0)*kx;y=(y+y0)*ky;
    y=edithi-y;
  }
  SUBCANVAS subcanvas(CANVAS &cnv) {return SUBCANVAS(cnv,x0*kx,edithi-y0*ky,kx,-ky);}
  TransformFunc ltop() {return transform_ltop;}
  TransformFunc ptol() {return transform_ptol;}
};

XYREC transform_ltop(void *data,XYREC src) {
  GRTransform *tr=reinterpret_cast<GRTransform*>(data);
  tr->ltop(src.x,src.y);
  return src;
}
XYREC transform_ptol(void *data,XYREC src) {
  GRTransform *tr=reinterpret_cast<GRTransform*>(data);
  tr->ptol(src.x,src.y);
  return src;
}

struct GRStateCom:GREditState {
  int mousex,mousey;
  int mouserealx,mouserealy;
  void *param;
  GRTransform *t;
  MouseMoveEvent onmmove;HintEvent onhint;ListChangeEvent onlistchng;
  GRStateCom(GRTransform *t,MouseMoveEvent onmmove,HintEvent onhint,ListChangeEvent onlistchng,void *aparam) {
    this->t=t;visiblecursor=false;
    this->onmmove=onmmove;
    this->onhint=onhint;
    this->onlistchng=onlistchng;
    param=aparam;
  }
  void showhint(string msg) {if (onhint) onhint(param,this,msg);}
  GRStateCom(GRStateCom *src) {
    t=src->t;
    onmmove=src->onmmove;
    onhint=src->onhint;
    onlistchng=src->onlistchng;
    param=src->param;
    visiblecursor=false;
  }
  double mousedist(XYREC pt) {return mousedist(pt.x,pt.y);}
  double mousedist(int x,int y) {
    t->ltop(x,y);
    int dx=x-mouserealx,dy=y-mouserealy;
    return sqrt(dx*dx+dy*dy);
  }
  void ltop(XYREC &pt) {t->ltop(pt.x,pt.y);}
  void drawcross(CANVAS &cnv,int x,int y) {
    t->ltop(x,y);
    cnv.fg(lcblack).cross(x,y,GRcrosssize);
    cnv.fg(lcred).cross(x,y,GRcrosssize/2);
    cnv.fg(lcblue).cross(x,y,GRcrosssize/4);
  }
  void changexy(GREditor *sender,int &x,int &y) {
    x-=t->dleft;y-=t->dtop;
    mouserealx=x;mouserealy=y;
    t->ptol(x,y);
    x-=t->jmpx/2;while (x%t->jmpx) x++;
    y-=t->jmpy/2;while (y%t->jmpy) y++;
    mousex=x;mousey=y;
  }
  void mdown(BUTTON bt,GREditor *sender,int x,int y,int shift) {
    changexy(sender,x,y);
    mousedown(bt,sender,x,y,shift);
    sender->repaint();
  }
  void mmove(GREditor *sender,int x,int y,int shift) {
    changexy(sender,x,y);
    mousemove(sender,x,y,shift);
    if (onmmove) onmmove(param,this,x,y);
    sender->repaint();
  }
  void mup(BUTTON bt,GREditor *sender,int x,int y,int shift) {
    changexy(sender,x,y);
    mouseup(bt,sender,x,y,shift);
    sender->repaint();
  }
  void drawptcircle(CANVAS &cnv,XYREC pt,LOGICCOLOR color) {drawptcircle(cnv,pt.x,pt.y,color);}
  void drawptcircle(CANVAS &cnv,int x,int y,LOGICCOLOR color) {
    t->ltop(x,y);
    cnv.fg(color).dell(x-GRcircleradius,y-GRcircleradius,x+GRcircleradius,y+GRcircleradius);
  }
  void drawptfcircle(CANVAS &cnv,XYREC pt,LOGICCOLOR color) {
    ltop(pt);
    cnv.bg(color).fell(pt.x-GRcircleradius,pt.y-GRcircleradius,pt.x+GRcircleradius,pt.y+GRcircleradius);
  }
  void repaint(GREditor *sender,CANVAS &cnv0) {
    SUBCANVAS cnv(cnv0,t->dleft,t->dtop);
    redraw(sender,t->subcanvas(cnv),cnv);
    if (t->drawos) {
      cnv.fg(lclred).linehor(t->edithi-t->y0*t->ky,0,t->editwi).linever(t->x0*t->kx,0,t->edithi);
      cnv.fg(lcblue).drect(0,0,t->editwi-1,t->edithi-1);
    }
    if (visiblecursor) drawcross(cnv,mousex,mousey);
  }
  virtual void redraw(GREditor *sender,CANVAS &cnv,CANVAS &orig)=0;
  virtual void mousedown(BUTTON bt,GREditor *sender,int x,int y,int shift) {}
  virtual void mouseup(BUTTON bt,GREditor *sender,int x,int y,int shift) {}
  virtual void mousemove(GREditor *sender,int x,int y,int shift) {}
  virtual bool cancelable()=0;
  void showcursor() {visiblecursor=true;}
  virtual void setselected(Array<GRObject*> &selected) { }
  virtual void getselected(TCntVec<GRObject*> &selected) {}
private:
  bool visiblecursor;
};

struct GRShapeEdit {
  GRStateCom *state;
  virtual bool mousedown(GRStateCom::BUTTON bt,GREditor *sender,int x,int y,int shift)=0;
  //vrati, zda melo toto kliknuti efekt(tj. zda uznelze vyuzit)
  virtual void redraw(GREditor *sender,CANVAS &cnv,CANVAS &orig)=0;
  virtual void mousemove(GREditor *sender,int x,int y,int shift)=0;
  virtual void mouseup(GRStateCom::BUTTON bt,GREditor *sender,int x,int y,int shift)=0;
  virtual ~GRShapeEdit() {}
  GRShapeEdit(GRStateCom *astate) {state=astate;}
};

struct GRStdState:GRStateCom {
  struct Moving {
    int lastx,lasty;
    Moving(int alastx,int alasty) {lastx=alastx;lasty=alasty;}
  };
  OWNER<Moving> moving;
  GRCompoud *obj;
  TCntVec<GRObject*> selected;
  OWNER<GRShapeEdit> editor;
  GRStdState(GRCompoud *aobj,GRStateCom *src):GRStateCom(src) {obj=aobj;}
  GRStdState(GRCompoud *aobj,GRTransform *t,MouseMoveEvent onmmove,HintEvent onhint,ListChangeEvent onlistchng,void *param):GRStateCom(t,onmmove,onhint,onlistchng,param) {obj=aobj;}
  void redraw(GREditor *sender,CANVAS &cnv,CANVAS &orig);
  bool cancelable() {return editor or selected.count;}
  void mousedown(BUTTON bt,GREditor *sender,int x,int y,int shift);
  void mousemove(GREditor *sender,int x,int y,int shift);
  void mouseup(BUTTON bt,GREditor *sender,int x,int y,int shift);
  void drawselectedobj(GRObject *obj,CANVAS &cnv,CANVAS &orig);

  void setselected(Array<GRObject*> &selected);
  void getselected(TCntVec<GRObject*> &selected) {selected.assign(this->selected);}
};

struct GRMiddle:GRStateCom {
  GRCompoud *obj;StateEvent done;
  GRMiddle(GRCompoud *aobj,GRStateCom *src,StateEvent adone):GRStateCom(src) {obj=aobj;showcursor();done=adone;}
  void redraw(GREditor *sender,CANVAS &cnv,CANVAS &orig) {obj->draw(cnv);}
  void mousedown(BUTTON bt,GREditor *sender,int x,int y,int shift);
  bool cancelable() {return true;}
};

struct GRCompoud;

struct GRCreatorState:GRStateCom {
  GRCompoud *obj;
  GRObject *created;
  GRCreatorState(GRCompoud *aobj,GRObject *acreated,StateEvent aoncreate,GRStateCom *src):GRStateCom(src) {
    obj=aobj;created=acreated;oncreate=aoncreate;
    showcursor();
  }
  StateEvent oncreate;
  void cancel(GREditor *sender);
protected:
  void create(GREditor *sender);
  bool cancelable() {return true;}
};

struct GR2Creator:GRCreatorState {
  bool down;
  GR2pts *gr2;
  GR2Creator(GRCompoud *obj,GR2pts *created,StateEvent oncreate,GRStateCom *src)
    :GRCreatorState(obj,created,oncreate,src) {gr2=created;down=false;}
  void mousedown(BUTTON bt,GREditor *sender,int x,int y,int shift) {
    if (bt==left) {
      gr2->a=XYREC(x,y);
      gr2->b=XYREC(x,y);
      sender->repaint();
      down=true;
    }
    if (bt==right) cancel(sender);
  }
  void mousemove(GREditor *sender,int x,int y,int shift) {
    gr2->b=XYREC(x,y);
    sender->repaint();
  }
  void mouseup(BUTTON bt,GREditor *sender,int x,int y,int shift) {
    gr2->b=XYREC(x,y);
    create(sender);
  }
  void redraw(GREditor *sender,CANVAS &cnv,CANVAS &orig);
};

struct GRPolyCreator:GRCreatorState {
  GRPoly *grp;
  GRPolyCreator(GRCompoud *obj,GRPoly *created,StateEvent oncreate,GRStateCom *src)
    :GRCreatorState(obj,created,oncreate,src) {grp=created;}
  void mousedown(BUTTON bt,GREditor *sender,int x,int y,int shift) {
    if (bt==left) {
      if (grp->pts.count==0) grp->pts.add(XYREC(x,y));
      grp->pts.add(XYREC(x,y));
      sender->repaint();
    } else if (bt==right) {
      if (grp->pts.count) {
        grp->pts.eraselast();
        create(sender);
      } else cancel(sender);
    }
  }
  void mousemove(GREditor *sender,int x,int y,int shift) {
    if (grp->pts.count) grp->pts.last()=XYREC(x,y);
    sender->repaint();
  }
  void redraw(GREditor *sender,CANVAS &cnv,CANVAS &orig);
};

struct GREditPt2:GRShapeEdit {
  GR2pts *gr2;
  XYREC *hpt;//zvyrazneny bod
  XYREC *movpt;//posunovany bod
  GREditPt2(GR2pts *edited,GRStateCom *state):GRShapeEdit(state) {
    gr2=edited;hpt=NULL;movpt=NULL;
  }
  void mousemove(GREditor *sender,int x,int y,int shift) {
    if (movpt) {
      *movpt=XYREC(x,y);
    } else {
      hpt=NULL;
      if (state->mousedist(gr2->a)<GRmaxdisttomove) hpt=&gr2->a;
      if (state->mousedist(gr2->b)<GRmaxdisttomove) hpt=&gr2->b;
    }
    sender->repaint();
  }
  bool mousedown(GRStateCom::BUTTON bt,GREditor *sender,int x,int y,int shift) {
    if (bt==GRStateCom::left and hpt) movpt=hpt,hpt=NULL,sender->setcursor(GREditor::hand);
    return movpt;
  }
  void mouseup(GRStateCom::BUTTON bt,GREditor *sender,int x,int y,int shift) {
    if (bt==GRStateCom::left) movpt=NULL,sender->setcursor(GREditor::normal);
  }
  void redraw(GREditor *sender,CANVAS &cnv,CANVAS &orig) {
//    obj->draw(cnv);
//    gr2->draw(cnv);
    state->drawptcircle(orig,gr2->a.x,gr2->a.y,lcblack);
    state->drawptcircle(orig,gr2->b.x,gr2->b.y,lcblack);
    if (hpt) state->drawptcircle(orig,hpt->x,hpt->y,lccyan);
    if (movpt) state->drawptcircle(orig,movpt->x,movpt->y,lcred);
  }
};

struct GREditPoly:GRShapeEdit {
  GRPoly *grp;
  int hpti;//zvyrazneny bod
  int movpti;//posunovany bod
  int linei;//index cary
  GREditPoly(GRPoly *edited,GRStateCom *state):GRShapeEdit(state) {
    grp=edited;hpti=-1;movpti=-1;linei=-1;
  }
  XYREC lpt1(int linei) {return grp->pts[linei];}
  XYREC lpt2(int linei) {return grp->pts[(linei+1)%grp->pts.count];}
  void mousemove(GREditor *sender,int x,int y,int shift) {
    if (movpti>=0) {
      grp->pts[movpti]=XYREC(x,y);
      grp->modified=true;
    } else {
      hpti=-1;linei=-1;
      for(int i=0;i<grp->pts.count;i++) if (state->mousedist(grp->pts[i])<GRmaxdisttomove) hpti=i;
      if (hpti<0) linei=nearline();
      if (linei>=0) state->showhint("Kliknutim vlozite bod mezi vyznacene body");
      else if (hpti>=0) state->showhint("Tazenim bodu jej prsunete, pravym tlacitkem vymazete");
      else state->showhint("");
    }
    sender->repaint();
  }
  bool mousedown(GRStateCom::BUTTON bt,GREditor *sender,int x,int y,int shift) {
    bool res=false;
    if (hpti>=0) {
      if (bt==GRStateCom::left) movpti=hpti,hpti=-1,sender->setcursor(GREditor::hand),res=true;
      if (bt==GRStateCom::right) grp->pts.erase(hpti),hpti=-1,movpti=-1,grp->modified=true,res=true;
    }
    if (linei>=0 and bt==GRStateCom::right) {
      grp->pts.insert(linei+1,XYREC(x,y));
      grp->modified=true;
      linei=-1;
      res=true;
    }
    return res;
  }
  void mouseup(GRStateCom::BUTTON bt,GREditor *sender,int x,int y,int shift) {
    if (bt==GRStateCom::left) movpti=-1,sender->setcursor(GREditor::normal);
  }
  void redraw(GREditor *sender,CANVAS &cnv,CANVAS &orig) {
//    grp->draw(cnv);
    for(int i=0;i<grp->pts.count;i++) state->drawptcircle(orig,grp->pts[i],lcblack);
    if (hpti>=0) state->drawptfcircle(orig,grp->pts[hpti],lclred);
    if (movpti>=0) state->drawptfcircle(orig,grp->pts[movpti],lclgreen);
    if (linei>=0) state->drawptfcircle(orig,lpt1(linei),lclblue),state->drawptfcircle(orig,lpt2(linei),lclblue);
  }
  void done(GREditor *sender) {
    sender->setcursor(GREditor::normal);
    state->showhint("");
  }
  int nearline();
};


}

#endif
