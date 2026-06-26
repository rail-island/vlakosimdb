#ifndef objs2dH
#define objs2dH

#include "comcnv.h"
#include "subcnv.h"
#include <ext/xmldoc.h>
#include <jmath/analgeom.h>

namespace gr {

class GRShapeEdit;class GRStateCom;class GRObject;

typedef void (*PointEvent)(XYREC &pt,void *data);
typedef XYREC (*TransformFunc)(void *data,XYREC);

struct GRObject {
  bool modified,selected;
  LOGICCOLOR fgcolor,bgcolor;
  bool isbg() {return bgcolor.l!=lcnone;}
  virtual void draw(CANVAS &cnv)=0;
  virtual ~GRObject() {}
  virtual void save(XMLItem &xml);
  virtual string name()=0;
  virtual string getname()=0;
  virtual void editprops() {}
  GRObject() {fgcolor=lcblack;bgcolor=lcwhite;modified=false;selected=false;}
  GRObject(XMLItem &xml);
  virtual GRShapeEdit *editstate(GRStateCom *state) {return NULL;}
//  virtual void translate(int dx,int dy)=0;
  virtual void forpoints(PointEvent pe,void *data) {}
  virtual void getselectpoints(TCntVec<XYREC> &pts)=0;//vrati zvyraznujici body
  void reflectx(int x0=0) {modified=true;forpoints(_reflectx,reinterpret_cast<void*>(x0));}
  void reflecty(int y0=0) {modified=true;forpoints(_reflecty,reinterpret_cast<void*>(y0));}
  void translate(int dx,int dy) {
    XYREC d(dx,dy);modified=true;
    forpoints(_translate,&d);
  }
  virtual GRObject *clone()=0;
  virtual bool ismodified() {return modified;}
  virtual void unmodify() {modified=false;}
  virtual bool contains(XYREC pt,TransformFunc ltop,void *data)=0;//pt je v fyzickych souradnicich
private:
  static void _reflectx(XYREC &pt,void *data) {pt.x=reinterpret_cast<int>(data)-pt.x;}
  static void _reflecty(XYREC &pt,void *data) {pt.y=reinterpret_cast<int>(data)-pt.y;}
  static void _translate(XYREC &pt,void *data) {
    XYREC *d=reinterpret_cast<XYREC*>(data);
    pt.x+=d->x;pt.y+=d->y;
  }
};

GRObject *loadobject(XMLItem &xml);

inline void savept(XMLItem &xml,XYREC pt,string name) {
  xml.vars.setvar(name+"_x",pt.x);
  xml.vars.setvar(name+"_y",pt.y);
}
inline XYREC loadpt(XMLItem &xml,string name) {
  XYREC res(0,0);
  res.x=xml.ivars[name+"_x"];
  res.y=xml.ivars[name+"_y"];
  return res;
}
inline void savept(XMLItem &xml,XYREC pt) {
  xml.name="point";
  xml.vars.setvar("x",pt.x);
  xml.vars.setvar("y",pt.y);
}
inline XYREC loadpt(XMLItem &xml) {
  XYREC res(0,0);
  res.x=xml.ivars["x"];
  res.y=xml.ivars["y"];
  return res;
}

struct GR2pts:GRObject {
  XYREC a,b;
  void save(XMLItem &xml) {GRObject::save(xml);savept(xml,a,"a");savept(xml,b,"b");}
  GR2pts(int x1,int y1,int x2,int y2) {a.x=x1;a.y=y1;b.x=x2;b.y=y2;}
  GR2pts(XMLItem &xml):GRObject(xml) {a=loadpt(xml,"a");b=loadpt(xml,"b");}
  GRShapeEdit *editstate(GRStateCom *state);
  void editprops() {
    jsystem->editvalues(fmtstr("Souradnice utvaru %s",name()),'i',&a.x,"x1",'i',&a.y,"y1",'i',&b.x,"x2",'i',&b.y,"y2",0);
    modified=true;
  }
  void forpoints(PointEvent pe,void *data) {
    pe(a,data);pe(b,data);
  }
};
struct GRLine:GR2pts {
  void draw(CANVAS &cnv) {cnv.fg(fgcolor).line(a.x,a.y,b.x,b.y);}
  virtual string name() {return "line";}
  GRLine(int x1,int y1,int x2,int y2):GR2pts(x1,y1,x2,y2) {}
  string getname() {return fmtstr("line [%d,%d],[%d,%d]",a.x,a.y,b.x,b.y);}
  GRLine(XMLItem &xml):GR2pts(xml) {}
  GRObject *clone() {return new GRLine(*this);}
  bool contains(XYREC pt,TransformFunc ltop,void *data);
  void getselectpoints(TCntVec<XYREC> &pts) {pts<<a<<b;}
};
struct GRRect:GR2pts {
  void draw(CANVAS &cnv) {
    cnv.fg(fgcolor).bg(bgcolor);
    if (bgcolor.isnone()) cnv.drect(a.x,a.y,b.x,b.y);
    else if (fgcolor.isnone()) cnv.frect(a.x,a.y,b.x,b.y);
    else cnv.rect(a.x,a.y,b.x,b.y);
  }
  virtual string name() {return "rect";}
  GRRect(int x1,int y1,int x2,int y2):GR2pts(x1,y1,x2,y2) {}
  string getname() {return fmtstr("rect [%d,%d],[%d,%d]",a.x,a.y,b.x,b.y);}
  GRRect(XMLItem &xml):GR2pts(xml) {}
  GRObject *clone() {return new GRRect(*this);}
  bool contains(XYREC pt,TransformFunc ltop,void *data);
  void getselectpoints(TCntVec<XYREC> &pts) {pts<<XYREC(a.x,a.y)<<XYREC(a.x,b.y)<<XYREC(b.x,a.y)<<XYREC(b.x,b.y);}
};
struct GREllipse:GR2pts {
  void draw(CANVAS &cnv) {
    cnv.fg(fgcolor).bg(bgcolor);
    if (bgcolor.isnone()) cnv.dell(a.x,a.y,b.x,b.y);
    else if (fgcolor.isnone()) cnv.fell(a.x,a.y,b.x,b.y);
    else cnv.ell(a.x,a.y,b.x,b.y);
  }
  virtual string name() {return "ellipse";}
  GREllipse(int x1,int y1,int x2,int y2):GR2pts(x1,y1,x2,y2) {}
  string getname() {return fmtstr("ellipse [%d,%d],[%d,%d]",a.x,a.y,b.x,b.y);}
  GREllipse(XMLItem &xml):GR2pts(xml) {}
  GRObject *clone() {return new GREllipse(*this);}
  bool contains(XYREC pt,TransformFunc ltop,void *data);
  void getselectpoints(TCntVec<XYREC> &pts) {
    XYREC mid=jmath::middle(a,b);
    pts<<XYREC(mid.x,a.y)<<XYREC(mid.x,b.y)<<XYREC(a.x,mid.y)<<XYREC(b.x,mid.y);
  }
};
struct GRCircle:GR2pts {
  float radius() {int dx=a.x-b.x,dy=a.y-b.y;return sqrt(dx*dx+dy*dy);}
  void draw(CANVAS &cnv) {
    int r=radius();cnv.fg(fgcolor).bg(bgcolor);
    if (bgcolor.isnone()) cnv.dEll(a.x-r,a.y-r,2*r,2*r);
    else if (fgcolor.isnone()) cnv.fEll(a.x-r,a.y-r,2*r,2*r);
    else cnv.Ell(a.x-r,a.y-r,2*r,2*r);
  }
  virtual string name() {return "circle";}
  GRCircle(int x1,int y1,int x2,int y2):GR2pts(x1,y1,x2,y2) {}
  string getname() {return fmtstr("circle [%d,%d],%d",a.x,b.x,(int)radius());}
  GRCircle(XMLItem &xml):GR2pts(xml) {}
  GRObject *clone() {return new GRCircle(*this);}
  bool contains(XYREC pt,TransformFunc ltop,void *data);
  void getselectpoints(TCntVec<XYREC> &pts) {
    int rad=radius();XYREC mid=a;
    pts<<XYREC(mid.x,mid.y-rad)<<XYREC(mid.x,mid.y+rad)<<XYREC(mid.x-rad,mid.y)<<XYREC(mid.x+rad,mid.y);
  }
};

struct GRPoly:GRObject {
  TCntVec<XYREC> pts;
  void save(XMLItem &xml) {
    GRObject::save(xml);
    for(int i=0;i<pts.count;i++) savept(xml.add(),pts[i]);
  }
  GRPoly() {}
  GRPoly(XMLItem &xml):GRObject(xml) {
    for(int i=0;i<xml.count();i++) pts.add(loadpt(xml[i]));
  }
  GRShapeEdit *editstate(GRStateCom *state);
  string getname() {
    string res=name();
    for(int i=0;i<pts.count;i++) res<<" ["<<pts[i].x<<","<<pts[i].y<<"]";
    return res;
  }
  void forpoints(PointEvent pe,void *data) {
    for(int i=0;i<pts.count;i++) pe(pts[i],data);
  }
  GRPoly(const GRPoly &src):GRObject(src) {
    pts.assign(src.pts);
  }
  void getselectpoints(TCntVec<XYREC> &pts) {pts.add(this->pts);}
};

struct GRPolyline:GRPoly {
  void draw(CANVAS &cnv) {
    cnv.fg(fgcolor).polyline(pts.buf,pts.count);
  }
  string name() {return "polyline";}
  GRPolyline() {}
  GRPolyline(XMLItem &xml):GRPoly(xml) {}
  GRObject *clone() {return new GRPolyline(*this);}
  bool contains(XYREC pt,TransformFunc ltop,void *data);
};
struct GRPolygon:GRPoly {
  void draw(CANVAS &cnv) {
    cnv.fg(fgcolor).bg(bgcolor);
    if (bgcolor.isnone()) cnv.dpolygon(pts.buf,pts.count);
    else if (fgcolor.isnone()) cnv.fpolygon(pts.buf,pts.count);
    else cnv.polygon(pts.buf,pts.count);
  }
  string name() {return "polygon";}
  GRPolygon() {}
  GRPolygon(XMLItem &xml):GRPoly(xml) {}
  GRObject *clone() {return new GRPolygon(*this);}
  bool contains(XYREC pt,TransformFunc ltop,void *data);
};

struct GRCompoud:GRObject {
  IObjArray<GRObject> items;
  int w,h;
  int x0,y0;//posun stredu o (x0,y0)
  void draw(CANVAS &cnv) {
    for(int i=0;i<items.count;i++) items[i]->draw(cnv);
  }
  virtual void save(XMLItem &xml) {
    xml.clear();
    for(int i=0;i<items.count;i++) items[i]->save(xml.add());
    xml.name="object";
    xml.vars.setvar("width",w);xml.vars.setvar("height",h);
    xml.vars.setvar("x0",x0);xml.vars.setvar("y0",y0);
  }
  virtual string name() {return "object";}
  string getname() {return "[object]";}
  GRCompoud(XMLItem &xml) {load(xml);}
  GRCompoud(int aw,int ah) {clear(aw,ah);}
  void load(XMLItem &xml) {
    items.clear();
    w=xml.ivars["width"];h=xml.ivars["height"];
    x0=xml.ivars["x0"];y0=xml.ivars["y0"];
    for(int i=0;i<xml.count();i++) if (GRObject *go=loadobject(xml[i])) items.add(go);
  }
  void editprops() {
    jsystem->editvalues("Vlastnosti obrazku",'i',&w,"sirka",'i',&h,"vyska",'i',&x0,"x0",'i',&y0,"y0",0);
    modified=true;
  }
  void clear(int aw,int ah) {
    items.clear();
    w=aw;h=ah;x0=w/2;y0=h/2;
  }
  void clear() {
    items.clear();
  }
  void forpoints(PointEvent pe,void *data) {
    for(int i=0;i<items.count;i++) items[i]->forpoints(pe,data);
  }
  void setmiddle(int x,int y) {
    x0+=x;y0+=y;translate(-x,-y);
  }
  GRCompoud(const GRCompoud &src) {
    w=src.w;h=src.h;x0=src.x0;y0=src.y0;
    items.wantcapa(src.items.count);
    for(int i=0;i<src.items.count;i++) items.add(const_cast<GRCompoud&>(src).items[0]->clone());
  }
  GRObject *clone() {return new GRCompoud(*this);}
  bool ismodified() {return modified or _is_mod();}
  void unmodify() {
    modified=false;
    for(int i=0;i<items.count;i++) items[i]->unmodify();
  }
  bool contains(XYREC pt,TransformFunc ltop,void *data) {return false;}
  GRObject *findobjat(XYREC pt,TransformFunc ltop,void *data) {
    for(int i=items.count-1;i>=0;i--) if (items[i]->contains(pt,ltop,data)) return items[i];
    return NULL;
  }
protected:
  bool _is_mod() {
    for(int i=0;i<items.count;i++) if (items[i]->ismodified()) return true;
    return false;
  }
  void getselectpoints(TCntVec<XYREC> &pts) {}
};

struct GRLink:GR2pts {
  GRCompoud *loaded;
  string file;
  GRLink(XMLItem &xml):GR2pts(xml) {
    file=xml.svars["file"];
    XMLDocument in;
    in.load(file);
    loaded=new GRCompoud(in.main);
  }
  GRLink(string afile,int x,int y):GR2pts(x,y,x,y) {
    file=afile;
    XMLDocument in;
    in.load(basedir/"images"/file);
    loaded=new GRCompoud(in.main);
    b.x=a.x+loaded->w;
    b.y=a.y+loaded->h;
  }
  void editprops() {
    jsystem->editvalues("Souradnice utvaru link",'i',&a.x,"x1",'i',&a.y,"y1",'i',&b.x,"x2",'i',&b.y,"y2",'o',&file,"soubor",0);
    modified=true;
  }
  void draw(CANVAS &cnv) {
    if (a.x!=b.x and a.y!=b.y) {
      SUBCANVAS sub(cnv,a.x,a.y,(b.x-a.x)/(double)loaded->w,(b.y-a.y)/(double)loaded->h);
      loaded->draw(sub);
    }
  }
  ~GRLink() {delete loaded;}
  void save(XMLItem &xml) {GR2pts::save(xml);xml.vars.setvar("file",sFile(file).relto(basedir/"images"));}
  string name() {return "link";}
  string getname() {return fmtstr("link [%s]",file.c_str());}
  GRLink(const GRLink &src):GR2pts(src) {
    file=src.file;
    loaded=(GRCompoud*)src.loaded->clone();
  }
  GRObject *clone() {return new GRLink(*this);}
  bool contains(XYREC pt,TransformFunc ltop,void *data);
  void getselectpoints(TCntVec<XYREC> &pts) {pts<<XYREC(a.x,a.y)<<XYREC(a.x,b.y)<<XYREC(b.x,a.y)<<XYREC(b.x,b.y);}
};

}

#endif
