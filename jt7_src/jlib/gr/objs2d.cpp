#include <jlib.h>
#pragma hdrstop

#include "objs2d.h"
#include "obj2dedit.h"
#include <jmath/analgeom.cpp>

namespace gr {
using namespace jmath;

static char hexdigits[]="0123456789ABCDEF";
inline int readhexdigit(char c) {
  if (c>='0' and c<='9') return c-'0';
  if (c>='a' and c<='f') return c-'a'+10;
  if (c>='A' and c<='F') return c-'A'+10;
  return 0;
}
inline void writebyte(char *&w,int x) {
  w[1]=hexdigits[x&0xF];
  x>>=4;
  w[0]=hexdigits[x&0xF];
  w+=2;
}
inline int readbyte(char *&s) {
  if (s[0]==0 or s[1]==0) return 0;
  int res=readhexdigit(s[0]);
  res<<=4;
  res|=readhexdigit(s[1]);
  s+=2;
  return res;
}



static void savecolor(XMLItem &xml,const char *name,LOGICCOLOR color) {
  if (color.isnone()) xml.setvar(name,"none");
  else {
    char res[0x10],*w=res;
    *w++='#';
    writebyte(w,color.getr());
    writebyte(w,color.getg());
    writebyte(w,color.getb());
    *w=0;
    xml.setvar(name,res);
  }
//  if (color.l==lcnative) xml.setvar(name,string("@")+itos(color.native));else xml.setvar(name,color.l);
}
static LOGICCOLOR loadcolor(XMLItem &xml,const char *name) {
  const char *c=xml.cvars[name];
  if (!c) return LOGICCOLOR(lcnone);
  if (*c=='#') {
    char *s=(char*)c;s++;
    int r=readbyte(s),g=readbyte(s),b=readbyte(s);
    return LOGICCOLOR(makergb(r,g,b),NATIVE_COLOR());
  }
  if (*c=='@') return LOGICCOLOR(atoi(c+1),NATIVE_COLOR());
  if (eqstr(c,"none")) return LOGICCOLOR(lcnone);
  return LOGICCOLOR(atoi(c));
}

void GRObject::save(XMLItem &xml) {
  xml.clear();
  savecolor(xml,"fgcolor",fgcolor);
  savecolor(xml,"bgcolor",bgcolor);
  xml.name=name();
}
GRObject::GRObject(XMLItem &xml) {
  fgcolor=loadcolor(xml,"fgcolor");
  bgcolor=loadcolor(xml,"bgcolor");
  modified=false;selected=false;
}


static bool isinpolygon(XYREC pt,const XYREC *pts,int count) {
  if (count<=2) return false;
  int minx=INT_MAX,miny=INT_MAX,maxx=INT_MIN,maxy=INT_MIN;
  for(int i=0;i<count;i++) {
    if (pts[i].x<minx) minx=pts[i].x;
    if (pts[i].y<miny) miny=pts[i].y;
    if (pts[i].x>maxx) maxx=pts[i].x;
    if (pts[i].y>maxy) maxy=pts[i].y;
  }
  if (minx==maxx or miny==maxy) return false;

  struct LINES {
    const XYREC *pts;
    int count;
    LINES(const XYREC *apts,int acount) {pts=apts;count=acount;}
    const XYREC &pt(int index) {return pts[index%count];}
    int dy0(int index) {
      int res;
      do {
        res=pt(index+1).y-pt(index).y;
        index++;
      } while (res==0);
      return res;
    }
  } lines(pts,count);

  bool result=false;

  for(int i=0;i<count;i++) {
    XYREC a=lines.pt(i),b=lines.pt(i+1);
    int dx=b.x-a.x,dy=b.y-a.y;
    if (dy!=0) {
      int sgny=dy>0?1:-1;
      double k=double(dx)/dy;
      for(int y=a.y;y!=b.y;y+=sgny) {
        if (pt.y==y and pt.x>=a.x+k*(y-a.y)) result=not result;
      }
      if (lines.dy0(i+1)*dy<0) {
        if (pt.y==b.y and pt.x>=b.x) result=not result;
      }
    }
  }

  return result;
}
inline void makeless(int &a,int &b) {if (a>b) {int pom=a;a=b;b=pom;}}

static bool line_contain(XYREC a,XYREC b,XYREC pt,TransformFunc ltop,void *data) {
  return agline<float>(agpoint<float>(ltop(data,a)),agpoint<float>(ltop(data,b))).pdistfromline(pt)<=2;
}

bool GRLine::contains(XYREC pt,TransformFunc ltop,void *data) {
  return line_contain(a,b,pt,ltop,data);
}
bool GRRect::contains(XYREC pt,TransformFunc ltop,void *data) {
  if (isbg()) {
    XYREC a=ltop(data,this->a),b=ltop(data,this->b);
    int x1=a.x,y1=a.y,x2=b.x,y2=b.y;
    makeless(x1,x2);makeless(y1,y2);
    return pt.x>=x1 and pt.x<=x2 and pt.y>=y1 and pt.y<=y2;
  } else {
    int x1=a.x,y1=a.y,x2=b.x,y2=b.y;
    makeless(x1,x2);makeless(y1,y2);
    XYREC a=XYREC(x1,y1),b=XYREC(x2,y1),c=XYREC(x2,y2),d=XYREC(x1,y2);
    return line_contain(a,b,pt,ltop,data) or line_contain(b,c,pt,ltop,data)
           or line_contain(c,d,pt,ltop,data) or line_contain(d,a,pt,ltop,data);
  }
}
bool GRCircle::contains(XYREC pt,TransformFunc ltop,void *data) {
  double rad=distance(ltop(data,a),ltop(data,b));
  double nrad=distance(pt,ltop(data,a));
  if (isbg()) return nrad<=rad;
  return fabs(nrad-rad)<=2;
}
bool GREllipse::contains(XYREC pt,TransformFunc ltop,void *data) {
  XYREC a=ltop(data,this->a),b=ltop(data,this->b);
  double ra=(a.x-b.x)/2,rb=(a.y-b.y)/2;
  double a2=ra*ra,b2=rb*rb;
  XYREC mid=middle(a,b);
  int dx=pt.x-mid.x,dy=pt.y-mid.y;
  double arg=dx*dx/a2+dy*dy/b2;
  if (isbg()) return arg<=1;
//  double d=1.0/(2*M_PI*max(a2,b2));
//  for(double t=0;t<2*M_PI;t+=d) if (distance(XYREC(mid.x+ra*cos(t),mid.y+rb*sin(t)),pt)<=2) return true;
//  return false;
  return (fabs(arg-1)<=20/(fabs(ra)+fabs(rb)));
}
bool GRLink::contains(XYREC pt,TransformFunc ltop,void *data) {
  XYREC a=ltop(data,this->a),b=ltop(data,this->b);
  int x1=a.x,y1=a.y,x2=b.x,y2=b.y;
  makeless(x1,x2);makeless(y1,y2);
  return pt.x>=x1 and pt.x<=x2 and pt.y>=y1 and pt.y<=y2;
}
bool GRPolyline::contains(XYREC pt,TransformFunc ltop,void *data) {
  for(int i=1;i<pts.count;i++) if (line_contain(pts[i-1],pts[i],pt,ltop,data)) return true;
  return false;
}
bool GRPolygon::contains(XYREC pt,TransformFunc ltop,void *data) {
  if (isbg()) {
    TCntVec<XYREC> mypts;
    for(int i=0;i<pts.count;i++) mypts.add(ltop(data,pts[i]));
    return isinpolygon(pt,mypts.buf,mypts.count);
  } else {
    for(int i=1;i<pts.count;i++) if (line_contain(pts[i-1],pts[i],pt,ltop,data)) return true;
    if (pts.count>=2) return (line_contain(pts[pts.count-1],pts[0],pt,ltop,data));
  }
}

GRShapeEdit *GR2pts::editstate(GRStateCom *state) {
  return new GREditPt2(this,state);
}
GRShapeEdit *GRPoly::editstate(GRStateCom *state) {
  return new GREditPoly(this,state);
}

GRObject *loadobject(XMLItem &xml) {
  if (xml.name=="object") return new GRCompoud(xml);
  if (xml.name=="line") return new GRLine(xml);
  if (xml.name=="rect") return new GRRect(xml);
  if (xml.name=="circle") return new GRCircle(xml);
  if (xml.name=="ellipse") return new GREllipse(xml);
  if (xml.name=="polygon") return new GRPolygon(xml);
  if (xml.name=="polyline") return new GRPolyline(xml);
  if (xml.name=="link") return new GRLink(xml);
  return NULL;
}



}

#pragma package(smart_init)
