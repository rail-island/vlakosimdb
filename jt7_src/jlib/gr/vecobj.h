#ifndef vecobjH
#define vecobjH
//---------------------------------------------------------------------------
#include "comgr.h"
#include "comcnv.h"

namespace gr {
using namespace jlib;


const int ve_line=1,ve_drect=2,ve_polyline=3,ve_polygon=4,ve_dpolygon=5,ve_fpolygon=6,
ve_rect=7,ve_frect=8,ve_ell=9,ve_fell=10,ve_dell=11;

struct EVEC:public Ecommon {
  EVEC(const string &msg):Ecommon(msg.c_str()) {}
  EVEC() {}
};
struct EVEC_Loaded_Unknown_Object:public EVEC {};

class VECELEM {
public:
  virtual void draw(const CANVAS &cnv)=0;
  virtual void edraw(const CANVAS &cnv) {draw(cnv);}//kresleni pri editace
  virtual bool size(ABREC &r)=0;
  //vrati,zda objekt je videt,pokud ano,v r je vyrez,z ktereho nepresahuje
  virtual void save(BINWRITE &fw)=0;
  virtual int pointcount()=0;
  virtual XYREC point(int index)=0;
  virtual void setpoint(int index,const XYREC &point)=0;
  virtual void caption(char *buf)=0;
  virtual LOGICCOLOR getfgcolor() {return lcblack;}
  virtual LOGICCOLOR getbgcolor() {return lcblack;}
  virtual void setfgcolor(LOGICCOLOR color) {}
  virtual void setbgcolor(LOGICCOLOR color) {}
  virtual bool addpoint(const XYREC &pt) {return false;}
  virtual bool deletepoint(int index) {return false;}
  void setlastpoint(const XYREC &pt) {setpoint(pointcount()-1,pt);}
  void deletelastpoint() {deletepoint(pointcount()-1);}
  virtual bool insertpoint(int index,const XYREC &pt) {return false;}//vrati,zda uspelo
  //vlozi pt ZA indexaty bod
};

class VEC2PTS:public VECELEM {
public:
  XYREC a,b;
  bool size(ABREC &r)
  {r.a.x=min(a.x,b.x);r.a.y=min(a.y,b.y);r.b.x=r.a.x+abs(a.x-b.x);r.b.y=r.a.y+abs(a.y-b.y);return true;}
  int pointcount() {return 2;}
  XYREC point(int index) {if (index) return b;return a;}
  void setpoint(int index,const XYREC &point) {if (index) b=point;else a=point;}
  VEC2PTS() {a=XYREC(0,0);b=XYREC(0,0);}
};

class VELINE:public VEC2PTS {
public:
  LOGICCOLOR color;
  void draw(const CANVAS &cnv) {cnv.fg(color).line(a,b);}
  void save(BINWRITE &fw) {fw<<ve_line<<a<<b<<color;}
  VELINE(BINREAD &fr) {fr>>a>>b>>color;}
  VELINE() {color=lcblack;}
  void caption(char *buf) {sprintf(buf,"line(%d,%d,%d,%d)",a.x,a.y,b.x,b.y);}
  LOGICCOLOR getfgcolor() {return color;}
  void setfgcolor(LOGICCOLOR color) {this->color=color;}
};

class VEDRAWRECTANGLE:public VEC2PTS {
public:
  LOGICCOLOR color;
  void draw(const CANVAS &cnv) {cnv.fg(color).drect(a.x,a.y,b.x,b.y);}
  void save(BINWRITE &fw) {fw<<ve_drect<<a<<b<<color;}
  VEDRAWRECTANGLE(BINREAD &fr) {fr>>a>>b>>color;}
  VEDRAWRECTANGLE() {color=lcblack;}
  void caption(char *buf) {sprintf(buf,"drect(%d,%d,%d,%d)",a.x,a.y,b.x,b.y);}
  LOGICCOLOR getfgcolor() {return color;}
  void setfgcolor(LOGICCOLOR color) {this->color=color;}
};
class VEFILLRECTANGLE:public VEC2PTS {
public:
  LOGICCOLOR color;
  void draw(const CANVAS &cnv) {cnv.bg(color).frect(a.x,a.y,b.x,b.y);}
  void save(BINWRITE &fw) {fw<<ve_frect<<a<<b<<color;}
  VEFILLRECTANGLE(BINREAD &fr) {fr>>a>>b>>color;}
  VEFILLRECTANGLE() {color=lcblack;}
  void caption(char *buf) {sprintf(buf,"frect(%d,%d,%d,%d)",a.x,a.y,b.x,b.y);}
  LOGICCOLOR getbgcolor() {return color;}
  void setbgcolor(LOGICCOLOR color) {this->color=color;}
};
class VERECTANGLE:public VEC2PTS {
public:
  LOGICCOLOR bg,fg;
  LOGICCOLOR getfgcolor() {return fg;}
  void setfgcolor(LOGICCOLOR color) {fg=color;}
  LOGICCOLOR getbgcolor() {return bg;}
  void setbgcolor(LOGICCOLOR color) {bg=color;}
  void draw(const CANVAS &cnv) {cnv.fg(fg).bg(bg).rect(a.x,a.y,b.x,b.y);}
  void save(BINWRITE &fw) {fw<<ve_rect<<a<<b<<fg<<bg;}
  VERECTANGLE(BINREAD &fr) {fr>>a>>b>>fg>>bg;}
  VERECTANGLE() {fg=lcblack;bg=lcwhite;}
  void caption(char *buf) {sprintf(buf,"rect(%d,%d,%d,%d)",a.x,a.y,b.x,b.y);}
};

class VEDRAWELLIPSE:public VEC2PTS {
public:
  LOGICCOLOR color;
  void draw(const CANVAS &cnv) {cnv.fg(color).dell(a.x,a.y,b.x,b.y);}
  void save(BINWRITE &fw) {fw<<ve_dell<<a<<b<<color;}
  VEDRAWELLIPSE(BINREAD &fr) {fr>>a>>b>>color;}
  VEDRAWELLIPSE() {color=lcblack;}
  void caption(char *buf) {sprintf(buf,"dell(%d,%d,%d,%d)",a.x,a.y,b.x,b.y);}
  LOGICCOLOR getfgcolor() {return color;}
  void setfgcolor(LOGICCOLOR color) {this->color=color;}
};
class VEFILLELLIPSE:public VEC2PTS {
public:
  LOGICCOLOR color;
  void draw(const CANVAS &cnv) {cnv.bg(color).fell(a.x,a.y,b.x,b.y);}
  void save(BINWRITE &fw) {fw<<ve_fell<<a<<b<<color;}
  VEFILLELLIPSE(BINREAD &fr) {fr>>a>>b>>color;}
  VEFILLELLIPSE() {color=lcblack;}
  void caption(char *buf) {sprintf(buf,"fell(%d,%d,%d,%d)",a.x,a.y,b.x,b.y);}
  LOGICCOLOR getbgcolor() {return color;}
  void setbgcolor(LOGICCOLOR color) {this->color=color;}
};
class VEELLIPSE:public VEC2PTS {
public:
  LOGICCOLOR bg,fg;
  LOGICCOLOR getfgcolor() {return fg;}
  void setfgcolor(LOGICCOLOR color) {fg=color;}
  LOGICCOLOR getbgcolor() {return bg;}
  void setbgcolor(LOGICCOLOR color) {bg=color;}
  void draw(const CANVAS &cnv) {cnv.fg(fg).bg(bg).ell(a.x,a.y,b.x,b.y);}
  void save(BINWRITE &fw) {fw<<ve_ell<<a<<b<<fg<<bg;}
  VEELLIPSE(BINREAD &fr) {fr>>a>>b>>fg>>bg;}
  VEELLIPSE() {fg=lcblack;bg=lcwhite;}
  void caption(char *buf) {sprintf(buf,"ell(%d,%d,%d,%d)",a.x,a.y,b.x,b.y);}
};


class VEPOLY:public VECELEM {
public:
  char cpt[0x100];
  TCntVec<XYREC> pts;
  bool size(ABREC &r);
  int pointcount() {return pts.count;}
  XYREC point(int index) {return pts[index];}
  void setpoint(int index,const XYREC &point) {pts[index]=point;;}
  void savepts(BINWRITE &fw);
  void loadpts(BINREAD &fr);
  VEPOLY(const char *acpt) {strcpy(cpt,acpt);}
  void caption(char *buf);
  bool addpoint(const XYREC &pt) {pts+=pt;return true;}
  bool deletepoint(int index) {pts.erase(index);return true;}
  bool insertpoint(int index,const XYREC &pt) {pts.insert((index+1)%pts.count,pt);return true;}
};

class VEPOLYLINE:public VEPOLY {
public:
  LOGICCOLOR color;
  LOGICCOLOR getfgcolor() {return color;}
  void setfgcolor(LOGICCOLOR color) {this->color=color;}
  void draw(const CANVAS &cnv) {cnv.fg(color).polyline(pts.buf,pts.count);}
  void save(BINWRITE &fw) {fw<<ve_polyline<<color;savepts(fw);}
  VEPOLYLINE(BINREAD &fr):VEPOLY("polyline") {fr>>color;loadpts(fr);}
  VEPOLYLINE():VEPOLY("polyline") {color=lcblack;}
};
class VEDRAWPOLYGON:public VEPOLY {
public:
  LOGICCOLOR color;
  LOGICCOLOR getfgcolor() {return color;}
  void setfgcolor(LOGICCOLOR color) {this->color=color;}
  void draw(const CANVAS &cnv) {cnv.fg(color).dpolygon(pts.buf,pts.count);}
  void save(BINWRITE &fw) {fw<<ve_dpolygon<<color;savepts(fw);}
  VEDRAWPOLYGON(BINREAD &fr):VEPOLY("dpolygon") {fr>>color;loadpts(fr);}
  VEDRAWPOLYGON():VEPOLY("dpolygon") {color=lcblack;}
};
class VEFILLPOLYGON:public VEPOLY {
public:
  LOGICCOLOR color;
  LOGICCOLOR getbgcolor() {return color;}
  void setbgcolor(LOGICCOLOR color) {this->color=color;}
  void draw(const CANVAS &cnv) {cnv.bg(color).fpolygon(pts.buf,pts.count);}
  void save(BINWRITE &fw) {fw<<ve_fpolygon<<color;savepts(fw);}
  VEFILLPOLYGON(BINREAD &fr):VEPOLY("fpolygon") {fr>>color;loadpts(fr);}
  VEFILLPOLYGON():VEPOLY("fpolygon") {color=lcblack;}
};
class VEPOLYGON:public VEPOLY {
public:
  LOGICCOLOR bg,fg;
  LOGICCOLOR getfgcolor() {return fg;}
  void setfgcolor(LOGICCOLOR color) {fg=color;}
  LOGICCOLOR getbgcolor() {return bg;}
  void setbgcolor(LOGICCOLOR color) {bg=color;}
  void draw(const CANVAS &cnv) {cnv.bg(bg).fg(fg).polygon(pts.buf,pts.count);}
  void save(BINWRITE &fw) {fw<<ve_polygon<<fg<<bg;savepts(fw);}
  VEPOLYGON(BINREAD &fr):VEPOLY("polygon") {fr>>fg>>bg;loadpts(fr);}
  VEPOLYGON():VEPOLY("polygon") {fg=lcblack;bg=lcwhite;}
};


class VECOBJ {
protected:
  VECELEM *readelem(BINREAD &fr);
public:
  IObjArray<VECELEM> elems;
  void save(BINWRITE &fw);
  bool size(ABREC &r);//vrati,zda objekt je videt,pokud ano,v r je vyrez,z ktereho nepresahuje
  VECOBJ(BINREAD &fr);
  VECOBJ() {}
  void draw(const CANVAS &cnv,VECELEM *notdraw=NULL);//notdraw nebude vykresleno
};

struct VECCOLELEM {
  VECOBJ *obj;
  char name[0x100];
  VECCOLELEM(VECOBJ *aobj,const char *aname) {obj=aobj;strcpy(name,aname);}
  VECCOLELEM(const char *aname) {obj=new VECOBJ;strcpy(name,aname);}

  void save(BINWRITE &fw);
  VECCOLELEM(BINREAD &fr);
};

class VECCOLLECTION {
public:
  IObjArray<VECCOLELEM> objs;
  void save(BINWRITE &fw);
  void save(const char *file);

  void load(BINREAD &fr);
  void load(const char *file);
  VECOBJ *objbyname(const char *name);
};

}

#endif
