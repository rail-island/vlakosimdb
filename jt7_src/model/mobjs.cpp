#include <jlib.h>
#pragma hdrstop

#include "mobjs.h"


#include "mobjs.h"
#include <zaba/zbrt.h>
#include <zaba/zbint.h>
#include <aws/awssystem.h>
#include <aws/awspaint.h>
#include <glo/glotextures.h>
#include <jmath/polyedr.h>
#include <jmath/crookalg.h>
#include <glo/rotatesolid.h>

namespace zaba {
#include "lib/modelobjs.h"
}


namespace zaba {
  extern aws::System *awssys;
}

namespace jtyc {
using namespace zaba;
using namespace aws;

aws::Component *GLViewType::createnew(aws::Component *owner,aws::System *system) {
  GLView *res=new aws::GLView(owner,system);
  return res;
}

zbObject *GLViewType::createzb(Component *comp,Runtime *rt) {
  if (dynamic_cast<GLView*>(comp)) return new zbGLView(rt,getzbw(comp));
  return NULL;
}

aws::Component *GLModelViewType::createnew(aws::Component *owner,aws::System *system) {
  GLModelView *res=new aws::GLModelView(owner,system);
  return res;
}

zbObject *GLModelViewType::createzb(Component *comp,Runtime *rt) {
  if (dynamic_cast<GLModelView*>(comp)) return new zbGLModelView(rt,getzbw(comp));
  return NULL;
}

zbObject *newmodel(Runtime *rt) {
  return new zbModel(new gloModel(),rt);
}

void definetexture(string name,string file) {
  texman->gettexture(name,file,NULL);
}

void cleartextures() {
  texman->clear();
}

string gettexturefile(string texname) {
  return texman->getfile(texname);
}


/*
void runpedralg(zbXMLNode *output,zbObject *clipx,zbObject *clipy,zbObject *clipz) {
  jmath::pedr::PedrAlg alg;
  if (clipx->tobool()) alg.loadclipx(dynamic_cast<zbXMLNode*>(clipx)->xml);
  if (clipy->tobool()) alg.loadclipy(dynamic_cast<zbXMLNode*>(clipy)->xml);
  if (clipz->tobool()) alg.loadclipz(dynamic_cast<zbXMLNode*>(clipz)->xml);
  alg.initalg();
  alg.run();
  alg.saveoutput(output->xml);
}
*/

zbObject *zbGLView::getrenderContext() {
  GLView *v=getlink();
  return new zbRenderContext(v->rcref,rt);
}

zbGLView::zbGLView(Runtime *rt,zbwProp *prop,STRING intf):zbControl(rt,prop,intf) {
}

void zbPedrAlg::run(XMLNode xml) {
  alg.initalg();
  alg.run();
  alg.saveoutput(xml);
}

void zbPedrAlg::modifyRun(XMLNode input,XMLNode output) {
  alg.loadinput(input);
  alg.run();
  alg.saveoutput(output);
}

void pedrtopolys(zbXMLNode *polyedr,zbXMLNode *polygons) {
  prPolyedr p;
  p.load(polyedr->xml);
  polygons->xml.clearsubs();
  p.savepolys(polygons->xml);
}

void generateRotateSolid(XMLNode ptsxml,XMLNode pedr,int cnt,XMLNode colorize) {
  Strings faceids;
  prPolyedr res;
  TCntVec<fpoint3d> pts;
  for(int i=0;i<ptsxml.count();i++) {
    fpoint3d &pt=pts.add();
    pt.x=ptsxml[i].fvar("x");
    pt.y=ptsxml[i].fvar("y");
    pt.z=ptsxml[i].fvar("z");
  }
  for(int i=0;i<ptsxml.count()-1;i++) faceids.add(ptsxml[i].svar("faceid"));
  CylindricColorize cize;
  cize.load(colorize);
  generaterotatesolid(res,pts,cnt,cize,faceids);
  res.save(pedr);
}

void crookalg(zbXMLNode *polyedr,string args) {
  CrookAlg ca;
  ca.loadopts(args);
  ca.loadpedr(polyedr->xml);
  ca.run();
  polyedr->xml.clearsubs();
  ca.save(polyedr->xml);
}

struct awsProjectionCanvas:gloProjectionCanvas {
  aws::Canvas cnv;
  void polygon(Array<glopoint> &pts) {
    TCntVec<Point> p;p.wantcapa(pts.count);
    for(int i=0;i<pts.count;i++) p.add(Point(round(pts[i].x),round(pts[i].y)));
    cnv.polygon(p.buf,p.count);
  }
  void polyline(Array<glopoint> &pts,bool closed) {
    for(int i=1;i<pts.count;i++) cnv.line(round(pts[i-1].x),round(pts[i-1].y),round(pts[i].x),round(pts[i].y));
    if (closed && pts.count>=2) cnv.line(round(pts[0].x),round(pts[0].y),round(pts[pts.count-1].x),round(pts[pts.count-1].y));
  }
  awsProjectionCanvas(Canvas cnv):cnv(cnv) {}
};

void zbModel::drawProjection(zbCanvas *canvas,int dir,float sx,float sy,float tx,float ty) {
  awsProjectionCanvas apc(canvas->cnv);
  model->drawprojection(apc,dir,sx,sy,tx,ty);
}

/*
void drawprojshape(Canvas cnv,XMLNode polygon,int dir,float sx,float sy,int tx,int ty) {
  bool ispolygon=polygon.getname()=="polygon";
  bool ispolyline=polygon.getname()=="polyline";
  if (ispolygon || ispolyline) {
    const char *udirx[3]={"z","x","x"};
    const char *udiry[3]={"y","z","y"};
    TCntVec<Point> pts;
    pts.wantcapa(polygon.count());
    for(int i=0;i<polygon.count();i++)
      pts.add(Point(
        round(polygon[i].fvar(udirx[dir])*sx+tx),
        round(polygon[i].fvar(udiry[dir])*sy+ty)
      ));
    if (ispolygon) cnv.polygon(pts.buf,pts.count);
    else {
      for(int i=1;i<pts.count;i++) cnv.line(pts[i-1].x,pts[i-1].y,pts[i].x,pts[i].y);
      if (polygon.ivar("closed") && pts.count>=2) cnv.line(pts[0].x,pts[0].y,pts.last().x,pts.last().y);
    }
  }
}

void drawprojection(Canvas cnv,XMLNode shape,int dir,float sx,float sy,int tx,int ty) {
  for(int i=0;i<shape.count();i++) drawprojshape(cnv,shape[i],dir,sx,sy,tx,ty);
}

void drawprojection(zbCanvas *cnv,zbXMLNode *shape,int dir,float sx,float sy,int tx,int ty) {
  drawprojection(cnv->cnv,shape->xml,dir,sx,sy,tx,ty);
}
*/

void s_translate(XMLNode shape,float tx,float ty,float tz) {
  for(int i=0;i<shape.count();i++) {
    XMLNode p=shape[i];
    for(int j=0;j<p.count();j++) {
      p[j].setvar("x",p[j].fvar("x")+tx);
      p[j].setvar("y",p[j].fvar("y")+ty);
      p[j].setvar("z",p[j].fvar("z")+tz);
    }
  }
}
void s_scale(XMLNode shape,float sx,float sy,float sz) {
  for(int i=0;i<shape.count();i++) {
    XMLNode p=shape[i];
    for(int j=0;j<p.count();j++) {
      p[j].setvar("x",p[j].fvar("x")*sx);
      p[j].setvar("y",p[j].fvar("y")*sy);
      p[j].setvar("z",p[j].fvar("z")*sz);
    }
  }
}

void fillpattern(Canvas cnv,int x1,int y1,int x2,int y2,Picture *pattern) {
  Canvas pcnv=pattern->getcanvas();
  int w=pattern->getw(),h=pattern->geth();
  if (w==0 || h==0) return;  
  for(int x=x1;x<=x2;x++) for(int y=y1;y<=y2;y++) {
    int gx=(x-x1)%w,gy=(y-y1)%h;
    cnv.pixel(x,y,pcnv.getpixel(gx,gy));
  }
  pattern->releasecanvas();
}

void fillPattern(zbCanvas *cnv,int x1,int y1,int x2,int y2,zbPicture *pattern) {
  fillpattern(cnv->cnv,x1,y1,x2,y2,pattern->picture);
}

bool pointinpolygon(float x,float y,XMLNode polygon) {
  const float epsilon=0.0001;
  fpoint p(x,y);
  TCntVec<fpoint> pts;
  for(int i=0;i<polygon.count();i++) {
    fpoint &fp=pts.add();
    fp.x=polygon[i].fvar("x");
    fp.y=polygon[i].fvar("y");
  }
  if (pts.count<3) return false;
  for(int i=0;i<pts.count;i++) {
    fpoint a=pts[i],b=pts[(i+1)%pts.count];
    if (distance(a,p)+distance(p,b)<distance(a,b)+2*epsilon) return true;
  }
  int invert_cnt=0;
  for(int i=0;i<pts.count;i++) {
    fpoint a=pts[i],b=pts[(i+1)%pts.count];
    //bod blizko hrany
    if (a.y==b.y) continue;//test ROVNOSTI dvou realnych cisel,spravne
    if (a.y>b.y) exchange(a,b);
    if (p.y<a.y || p.y>=b.y) continue;//y jen mezi <a.y,b.y) (opet BEZ epsilonu)
    if (fabs(a.x-b.x)<epsilon) {
      if (p.x>=(a.x+b.x)/2) invert_cnt++;
      continue;
    }
    float k=(b.y-a.y)/(b.x-a.x);
    if (b.x>a.x) {
      if (k*(p.x-a.x)+a.y>p.y) invert_cnt++;
    } else {
      if (k*(p.x-a.x)+a.y<p.y) invert_cnt++;
    }
  }
  return invert_cnt&0x1;//test parity
}


zbObject *newcoloralg(Runtime *rt) {return new zbColorAlg(rt);}
zbObject *newpedralg(Runtime *rt) {return new zbPedrAlg(rt);}
zbObject *newstampalg(Runtime *rt) {return new zbStampAlg(rt);}

}

namespace zaba {
using namespace jtyc;

#include "lib/modelobjs.cc"
}


