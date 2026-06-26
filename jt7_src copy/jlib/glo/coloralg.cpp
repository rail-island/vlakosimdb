#include <jlib.h>
#pragma hdrstop

#include "coloralg.h"
#include <jmath/pedralg.h>
#include <com/sorting.cpp>
#include <gram/gmanal.h>

namespace glo {

static const float epsilon=0.0001;

//vzajemna poloha dvou primek
//-1:neznama,0:totozky,1:rovnobezky,2:ruznobezky
static int linespos(fpoint a,fpoint b,fpoint c,fpoint d,double *p_t,double *p_s) {
  fvector u=b-a,v=d-c;
  double jm=u.fsize()*v.fsize();
  if (jm<epsilon) return -1;//jedna z primek je nulova
  double od=acos(fabs(u*v)/jm);
  if (od<epsilon) {//rovnobezky nebo totozky
    fvector w=c-a;
    double jm2=u.fsize()*w.fsize();
    if (jm2<epsilon) return 0;//totozky
    double od2=acos(fabs(u*w)/jm2);
    if (od2<epsilon) return 0;
    return 1;
  }

  double m=b.x-a.x,o=b.y-a.y,n=d.x-c.x,p=d.y-c.y;
  double s=(m*a.y-o*a.x-m*c.y+o*c.x)/(m*p-o*n);
  double t=(n*a.y-p*a.x-n*c.y+p*c.x)/(m*p-o*n);
  if (p_t) *p_t=t;if (p_s) *p_s=s;
  return 2;
}

///vrati, zda x je mezi a,b (predpoklada, ze jsou na jedne primce)
static bool between(const fpoint &x,const fpoint &a,const fpoint &b) {
  if (x.x>=a.x && x.y>=a.y && x.x<=b.x && x.y<=b.y) return true;
  if (x.x<=a.x && x.y<=a.y && x.x>=b.x && x.y>=b.y) return true;
  return false;
}

struct CPPR {
  float t,s;
};
int CPPR_compare(CPPR &a,CPPR &b) {
  if (a.t<b.t) return -1;
  if (a.t>b.t) return 1;
  return 0;
}

struct IPT {
  fpoint pt;
  bool in;
  IPT(fpoint pt):pt(pt) {in=false;}
};

static void condadd(TCntVec<IPT> &pts,fpoint pt) {
  for(int i=0;i<pts.count;i++) if (fdistance(pts[i].pt,pt)<epsilon) return;
  pts.add(pt); 
}

static void makecompts(TCntVec<fpoint> &pl1,TCntVec<fpoint> &pl2,TCntVec<IPT> &res) {
  for(int i=0;i<pl1.count;i++) {
    fpoint a=pl1[i],b=pl1[(i+1)%pl1.count];
    condadd(res,a);

    TCntVec<CPPR> coms;
    for(int j=0;j<pl2.count;j++) {
      fpoint c=pl2[j],d=pl2[(j+1)%pl2.count];
      double t=0,s=0;
      int pos=linespos(a,b,c,d,&t,&s);
      if (pos==0) {//totozky
        if (between(c,a,b)) {
          if (between(d,a,c)) condadd(res,d),condadd(res,c);
          else if (between(d,b,c)) condadd(res,c),condadd(res,d);
          else condadd(res,c);
        } else {
          if (between(d,a,b)) condadd(res,d);
        }
      }
      if (pos==2) {//ruznobezky
        if (t>=0 && s>=0 && t<=1 && s<=1) {
          CPPR &cp=coms.add();
          cp.t=t;
          cp.s=s;
        }
      }
    }
    //body se museji pridavat postupne kolem dokola polygonu
    heapsort(coms.buf,coms.count,CPPR_compare);

    fvector v=b-a;

    for(int k=0;k<coms.count;k++) condadd(res,a+v*coms[k].t);
  }
}

static bool isnear(TCntVec<IPT> &a1,int i1,TCntVec<IPT> &a2,int i2) {
  if (i1<0) i1+=a1.count;if (i1>=a1.count) i1-=a1.count;
  if (i2<0) i2+=a2.count;if (i2>=a2.count) i2-=a2.count;
  return fdistance(a1[i1].pt,a2[i2].pt)<epsilon;
}

static bool issoused(TCntVec<IPT> *p1,TCntVec<IPT> *p2,int i1,int i2) {
  TCntVec<IPT> &a1=*p1,&a2=*p2;
  if (p1==p2) {
    if (abs(i1-i2)<=1) return true;
    if (abs(i1=i2)==a1.count-1) return true;
    return false;
  } else {
    if (isnear(a1,i1-1,a2,i2)) return true;
    if (isnear(a1,i1+1,a2,i2)) return true;
    if (isnear(a1,i1,a2,i2-1)) return true;
    if (isnear(a1,i1,a2,i2+1)) return true;
    return false;
  }
}

static bool contains(Array<fpoint> &pts,fpoint pt) {
  for(int i=0;i<pts.count;i++) if (fdistance(pts[i],pt)<epsilon) return true;
  return false;
}

static void intersection(TCntVec<fpoint> &pl1,TCntVec<fpoint> &pl2,TCntVec<fpoint> &res) {
  pedr::Polygon pp1,pp2;
  for(int i=0;i<pl1.count;i++) pp1.vrs.add(pedr::point2(pl1[i].x,pl1[i].y));
  for(int i=0;i<pl2.count;i++) pp2.vrs.add(pedr::point2(pl2[i].x,pl2[i].y));

  TCntVec<IPT> p1,p2;//body i s pruseciky
  makecompts(pl1,pl2,p1);
  makecompts(pl2,pl1,p2);

  for(int i=0;i<p1.count;i++) p1[i].in=pp2.haspoint(pedr::point2(p1[i].pt.x,p1[i].pt.y));
  for(int i=0;i<p2.count;i++) p2[i].in=pp1.haspoint(pedr::point2(p2[i].pt.x,p2[i].pt.y));

  TCntVec<IPT> *a1=&p1,*a2=&p2;
  int idx=-1;//idx je vzdy index do a1

  for(int i=0;i<p1.count;i++) if (p1[i].in) {
    idx=i;break;
  }
  if (idx<0) for(int i=0;i<p2.count;i++) if (p2[i].in) {
    idx=i;exchange(a1,a2);break;
  }

  if (idx<0) return;

  res.add((*a1)[idx].pt);

  for(;;) {
    for(int i=0;i<a1->count;i++) {
      IPT &ip=(*a1)[i];
      if (!ip.in) continue;
      if (contains(res,ip.pt)) continue;
      if (issoused(a1,a1,i,idx)) {
        res.add((*a1)[i].pt);
        idx=i;
        goto nextfor;
      }
    }
    for(int i=0;i<a2->count;i++) {
      IPT &ip=(*a2)[i];
      if (!ip.in) continue;
      if (contains(res,ip.pt)) continue;
      if (issoused(a2,a1,i,idx)) {
        res.add((*a2)[i].pt);
        idx=i;exchange(a1,a2);
        goto nextfor;
      }
    }
    return;
    nextfor:
  }
}

/*
void ColorSideItem::load(int dir,XMLNode xml) {
  x1=xml.fvar("tex_x1");
  y1=xml.fvar("tex_y1");
  x2=xml.fvar("tex_x2");
  y2=xml.fvar("tex_y2");
  name=xml.svar("name");
  transparent=xml.ivar("transparent");
  afterload(dir);
}
*/

fvector3d normalfromdir(int dir) {
  fvector3d n;
  n.x=n.y=n.z=0;
  switch (dir) {
    case 0:n.x=1;break;
    case 1:n.x=-1;break;
    case 2:n.y=1;break;
    case 3:n.y=-1;break;
    case 4:n.z=1;break;
    case 5:n.z=-1;break;
  }
  return n;
}

void ColorSideItem::afterload() {
//  loaded=name!="";
  loaded=true;
  if (material) {
    if (fabs(xscale)<epsilon) xscale=1;
    if (fabs(yscale)<epsilon) yscale=1;
//    if (fabs(xscale)<epsilon || fabs(yscale)<epsilon) loaded=false;
  } else {
    if (fabs(x1-x2)<epsilon || fabs(y1-y2)<epsilon) loaded=false;
  }
  n=normalfromdir(dir);
}

void ColorSideItem::set(int adir,float ax1,float ay1,float ax2,float ay2,string texname,bool atransparent,string acond) {
  x1=ax1;y1=ay1;x2=ax2;y2=ay2;
  transparent=atransparent;
  name=texname;
  material=false;
  dir=adir;
  cond.load(acond);
  afterload();
}

void ColorSideItem::setmaterial(int adir,float adx,float ady,float axscale,float ayscale,string texname,bool atransparent,bool atransp,string acond) {
  transp=atransp;
  dx=adx;dy=ady;xscale=axscale;yscale=ayscale;
  transparent=atransparent;
  name=texname;
  material=true;
  dir=adir;
  cond.load(acond);
  afterload();
}

static void readpts(XMLNode polygon,TCntVec<fpoint3d> &pts) {
  for(int i=0;i<polygon.count();i++) {
    fpoint3d &pt=pts.add();
    pt.x=polygon[i].fvar("x");
    pt.y=polygon[i].fvar("y");
    pt.z=polygon[i].fvar("z");
  }
}

static float getx(fpoint3d pt,int dir) {
  switch (dir) {
    case 0:return pt.z;
    case 1:return pt.x;
    case 2:return pt.x;
  }
  return 0;
}
static float gety(fpoint3d pt,int dir) {
  switch (dir) {
    case 0:return pt.y;
    case 1:return pt.z;
    case 2:return pt.y;
  }
  return 0;
}
static float getz(fpoint3d pt,int dir) {
  switch (dir) {
    case 0:return pt.x;
    case 1:return pt.y;
    case 2:return pt.z;
  }
  return 0;
}
static void setx(fpoint3d &pt,int dir,float value) {
  switch (dir) {
    case 0:pt.z=value;break;
    case 1:pt.x=value;break;
    case 2:pt.x=value;break;
  }
}
static void sety(fpoint3d &pt,int dir,float value) {
  switch (dir) {
    case 0:pt.y=value;break;
    case 1:pt.z=value;break;
    case 2:pt.y=value;break;
  }
}
static void setz(fpoint3d &pt,int dir,float value) {
  switch (dir) {
    case 0:pt.x=value;break;
    case 1:pt.y=value;break;
    case 2:pt.z=value;break;
  }
}

static void projection(int dir,TCntVec<fpoint3d> &pts,TCntVec<fpoint> &pt2) {
  for(int i=0;i<pts.count;i++) {
    fpoint &p=pt2.add();
    fpoint3d v=pts[i];
    p.x=getx(v,dir);
    p.y=gety(v,dir);
  }
}

bool ColorAlg::run(XMLNode polygon) {
  fvector3d n;
  n.x=polygon.fvar("nx");n.y=polygon.fvar("ny");n.z=polygon.fvar("nz");
  if (n.size()<epsilon) return false;
  n=n.size1();
  //normala

  //nejdrive najdeme texturu, kterou se bude barvit
  int maxi=-1;
  float maxv=0;

  for(int i=0;i<items.count;i++) {
    if (!items[i]->loaded) continue;
    if (!items[i]->cond.accept(polygon.svar("faceid"))) continue;
    float act=items[i]->n*n;
    if (act>maxv) maxv=act,maxi=i;
  }

/*
  for(int i=0;i<6;i++) {
    if (colors[i].loaded) {
      float act=colors[i].n*n;
      if (act>maxv) maxv=act,maxi=i;
    }
  }
*/
  if (maxi<0) return false;
  if (maxv<0.7) return false;
  //o malo mensi cos(45 stupnu)

  TCntVec<fpoint3d> pts;
  readpts(polygon,pts);

  int dir=items[maxi]->dir/2;

  TCntVec<fpoint> pt2;
  projection(dir,pts,pt2);

  ColorSideItem &ci=*items[maxi];

  //muzeme barvit
  string texture=items[maxi]->name;
  if (ci.material) {
    if (texture!="") {
      for(int i=0;i<pt2.count;i++) {
        polygon[i].setvar(ci.transp?"t":"s",pt2[i].x*ci.xscale+ci.dx);
        polygon[i].setvar(ci.transp?"s":"t",pt2[i].y*ci.yscale+ci.dy);
      }
    }
  } else {
    for(int i=0;i<pt2.count;i++) {
      if (pt2[i].x<min(ci.x1,ci.x2)-epsilon) return false;
      if (pt2[i].x>max(ci.x1,ci.x2)+epsilon) return false;
      if (pt2[i].y<min(ci.y1,ci.y2)-epsilon) return false;
      if (pt2[i].y>max(ci.y1,ci.y2)+epsilon) return false;
    }
    for(int i=0;i<pts.count;i++) {
      polygon[i].setvar("s",(pt2[i].x-ci.x1)/(ci.x2-ci.x1));
      polygon[i].setvar("t",(pt2[i].y-ci.y1)/(ci.y2-ci.y1));
    }
  }
  if (texture!="") {
    polygon.setvar("texture",texture);
    polygon.setvar("texture.transparent",items[maxi]->transparent);
  }
  if (items[maxi]->denylight) polygon.setvar("denylight",1);
  polygon.setvar("r",items[maxi]->rkoef);
  polygon.setvar("g",items[maxi]->gkoef);
  polygon.setvar("b",items[maxi]->bkoef);

  return true;
}

bool StampAlg::run(XMLNode polygon,XMLNode addhere) {
  if (this->dist<0.01) this->dist=0.01;
  string faceid=polygon.svar("faceid");
  if (!cond.accept(faceid)) return false;
  if (fabs(x1-x2)<epsilon || fabs(y1-y2)<epsilon) return false;

  fvector3d n;
  n.x=polygon.fvar("nx");n.y=polygon.fvar("ny");n.z=polygon.fvar("nz");
  if (n.size()<epsilon) return false;
  n=n.size1();
  //normala

  TCntVec<fpoint3d> pts;
  readpts(polygon,pts);

  float dist=normalfromdir(dir)*n;
  //exit:odchylka>maxdist,cos(odchylka)=dist
  //=>cos(maxdist)>dist
  if (cos(maxdist)>=dist+epsilon) return false;

  TCntVec<fpoint> pt2,rect,inter;

  int d2=dir/2;
  projection(d2,pts,pt2);

  rect.add(fpoint(x1,y1));rect.add(fpoint(x2,y1));
  rect.add(fpoint(x2,y2));rect.add(fpoint(x1,y2));

  intersection(pt2,rect,inter);

  if (inter.count<3) return false;//prunik prazdny polygon - razitko se nemusi kreslit
  if (pts.count<3) return false;//nemelo by nastat

  //mame vysledny polygon, treti souradnice a texcoords se dopocitaji interpolaci
/*
  float minx=0x1000,minx_z,maxx=-0x1000,maxx_z;
  float miny=0x1000,miny_z,maxy=-0x1000,maxy_z;
  for(int i=0;i<pts.count;i++) {
    if (getx(pts[i],d2)<minx) minx=getx(pts[i],d2),minx_z=getz(pts[i],d2);
    if (getx(pts[i],d2)>maxx) maxx=getx(pts[i],d2),maxx_z=getz(pts[i],d2);
    if (gety(pts[i],d2)<miny) miny=gety(pts[i],d2),miny_z=getz(pts[i],d2);
    if (gety(pts[i],d2)>maxy) maxy=gety(pts[i],d2),maxy_z=getz(pts[i],d2);
  }

  float dzx=0,dzy=0;
  if (maxx-minx>epsilon) dzx=(maxx_z-minx_z)/(maxx-minx);
  if (maxy-miny>epsilon) dzy=(maxy_z-miny_z)/(maxy-miny);
*/
  fpoint3d ra,rb,rc;//ra,rb,rc ma virtualni souradnice
/*
  setx(ra,d2,pts[0].x);sety(ra,d2,pts[0].y);setz(ra,d2,pts[0].z);
  setx(rb,d2,pts[1].x);sety(rb,d2,pts[1].y);setz(rb,d2,pts[1].z);
*/
  ra.x=getx(pts[0],d2);ra.y=gety(pts[0],d2);ra.z=getz(pts[0],d2);
  rb.x=getx(pts[1],d2);rb.y=gety(pts[1],d2);rb.z=getz(pts[1],d2);
  fvector3d u,v;
  for(int i=2;i<pts.count;i++) {
//    setx(rc,d2,pts[i].x);sety(rc,d2,pts[i].y);setz(rc,d2,pts[i].z);
    rc.x=getx(pts[i],d2);rc.y=gety(pts[i],d2);rc.z=getz(pts[i],d2);
    u=ra-rb,v=rc-rb;
    if ((u%v).size()>epsilon) break;
  }
  if ((u%v).size()<epsilon) return false;

  fvector3d rov=u%v;
  float d=-(rov.x*ra.x+rov.y*ra.y+rov.z*ra.z);
  if (fabs(rov.z)<epsilon) return false;

  XMLNode np=addhere.add("polygon");

  float dz_visible=-this->dist;
  if (dir%2) dz_visible=-dz_visible;

  for(int i=0;i<inter.count;i++) {
    fpoint3d pt;
    fpoint pt2=inter[i];
    setx(pt,d2,pt2.x);
    sety(pt,d2,pt2.y);
//    setz(pt,d2,minx_z+(pt.x-minx)*dzx+miny_z+(pt.y-miny)*dzy+dz_visible);
    setz(pt,d2,-(rov.x*pt2.x+rov.y*pt2.y+d)/rov.z+dz_visible);
    float s=(pt2.x-x1)/(x2-x1),t=(pt2.y-y1)/(y2-y1);
    if (material) {
      if (xscale>0) s=(pt2.x-x1)/xscale;
      if (yscale>0) t=(pt2.y-y1)/yscale;
    }
    if (transp) exchange(s,t);
    if (reflectt) t=1-t;
    if (reflects) s=1-s;

    XMLNode ver=np.add("vertex");
    ver.setvar("x",pt.x);
    ver.setvar("y",pt.y);
    ver.setvar("z",pt.z);
    ver.setvar("t",t);
    ver.setvar("s",s);
  }
  if (texture!="") np.setvar("texture",texture);
  np.setvar("r",rkoef);
  np.setvar("g",gkoef);
  np.setvar("b",bkoef);

  return true;
}

void ColorAlg::addside(int dir,float x1,float y1,float x2,float y2,string texname,bool transparent,string cond,string rkoef,string gkoef,string bkoef,bool denylight) {
  if (dir>=0 && dir<6) {
    ColorSideItem *it=new ColorSideItem();
    items.add(it);
    it->set(dir,x1,y1,x2,y2,texname,transparent,cond);
    it->denylight=denylight;
    it->rkoef=rkoef;
    it->gkoef=gkoef;
    it->bkoef=bkoef;
  }
}
void ColorAlg::addmaterialside(int dir,float dx,float dy,float xscale,float yscale,string texname,bool transparent,string rkoef,string gkoef,string bkoef,bool transp,string cond,bool denylight) {
  if (dir>=0 && dir<6) {
    ColorSideItem *it=new ColorSideItem();
    items.add(it);
    it->setmaterial(dir,dx,dy,xscale,xscale,texname,transparent,transp,cond);
    it->denylight=denylight;
    it->rkoef=rkoef;
    it->gkoef=gkoef;
    it->bkoef=bkoef;
  }
}

bool ColorCond::accept(string faceid) {
  bool res=start;
  for(int i=0;i<items.count;i++) {
    ColorCondItem &it=*items[i];
    if (it.s==faceid) {
      if (it.canbe && !start) res=true;
      if (it.mustnot) return false;
    } else {
      if (it.mustbe) return false;
    }
  }
  return res;
}

void ColorCond::load(string s) {
  gram::Analyser a(s.c_str());
  while (a.jw(),*a) {
    if (*a=='*') {
      a++;
      start=true;
    } else if (*a=='^') {
      a++;
      start=false;
    } else if (*a=='+') {
      a++;
      ColorCondItem *it=new ColorCondItem();
      items.add(it);
      it->mustbe=true;
      it->s=a.readto(" \t\n\r",true);
    } else if (*a=='-') {
      a++;
      ColorCondItem *it=new ColorCondItem();
      items.add(it);
      it->mustnot=true;
      it->s=a.readto(" \t\n\r",true);
    } else {
      ColorCondItem *it=new ColorCondItem();
      items.add(it);
      it->canbe=true;
      it->s=a.readto(" \t\n\r",true);
    }
  }
}


}

