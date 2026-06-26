#include <jlib.h>
#pragma hdrstop

#include "grrcom.h"
#include "grrmap.h"
#include <opengl.h>

namespace jtyc {

int tt_cons[vr_cnt+1][vr_cnt+1];

inline fpoint countac(double t) {
  double tt=t/M_SQRT2;
  return fpoint(tt,1-tt);
}
inline fpoint countbd(double t) {
  double tt=t/M_SQRT2;
  return fpoint(1-tt,1-tt);
}
inline fpoint count24(double t) {
  return fpoint(1-t,0.5);
}
inline fpoint count13(double t) {
  return count24(t).transp();
}
inline fpoint count34(double t) {
  return fpoint(0.5*cos(t*2),0.5*sin(t*2));
}
inline fpoint count12(double t) {
  return count34(t).reflectxy(1,1);
}
inline fpoint count14(double t) {
  return count34(t).reflecty(1);
}
inline fpoint count23(double t) {
  return count34(curve1len-t).reflectx(1);
}
inline fpoint count3b(double t) {
  if (t<bcc::arclen) {
    double tt=t/bcc::arclen*M_PI_4;
    return fpoint(bcc::r*cos(tt+M_PI)+bcc::r+0.5,bcc::r*sin(tt));
  } else {
    double tt=(t-bcc::arclen)/bcc::linelen;
    double res=0.5+bcc::arcdx+tt*bcc::linedx;
    return fpoint(res,res);
  }
}
inline fpoint count1c(double t) {
  return count3b(t).reflecty(1);
}
inline fpoint count1d(double t) {
  return count3b(t).reflectxy(1,1);
}
inline fpoint count4c(double t) {
  return count3b(t).reflectx(1).transp();
}
inline fpoint count4b(double t) {
  return count3b(t).transp();
}
inline fpoint count2d(double t) {
  return count3b(t).transp().reflectxy(1,1);
}
inline fpoint count2a(double t) {
  return count3b(t).transp().reflectx(1);
}
inline fpoint count3a(double t) {
  return count3b(t).reflectx(1);
}

inline fpoint countbc(double t) {
  double tt=(curve2len-t)/curve2len*M_PI_2;
  return fpoint(M_SQRT_2*cos(tt+3*M_PI_4)+1.5,M_SQRT_2*sin(tt-M_PI_4)+0.5);
}
inline fpoint countcd(double t) {
  return countbc(t).transp().reflecty(1);
}
inline fpoint countad(double t) {
  return countbc(curve2len-t).reflectx(1);
}
inline fpoint countab(double t) {
  return countbc(curve2len-t).transp();
}



inline fvector dcount13(double t) {return fvector(0,-1);}
inline fvector dcount24(double t) {return fvector(-1,0);}

inline fvector dcount34(double t) {
  return fvector(-sin(t*2),cos(t*2)).size1();
}
inline fvector dcount12(double t) {
  return dcount34(t).reflectxy();
}
inline fvector dcount14(double t) {
  return dcount34(t).reflecty();
}
inline fvector dcount23(double t) {
  return dcount34(curve1len-t).reflectx();
}

inline fvector dcount3b(double t) {
  if (t<bcc::arclen) {
    double tt=t/bcc::arclen*M_PI_4;
    return fvector(-sin(tt+M_PI),cos(tt)).size1();
  } else {
    return fvector(M_SQRT_2,M_SQRT_2);
  }
}
inline fvector dcount1c(double t) {
  return dcount3b(t).reflecty();
}
inline fvector dcount1d(double t) {
  return dcount3b(t).reflectxy();
}
inline fvector dcount4c(double t) {
  return dcount3b(t).reflectx().transp();
}
inline fvector dcount4b(double t) {
  return dcount3b(t).transp();
}
inline fvector dcount2d(double t) {
  return dcount3b(t).transp().reflectxy();
}
inline fvector dcount2a(double t) {
  return dcount3b(t).transp().reflectx();
}
inline fvector dcount3a(double t) {
  return dcount3b(t).reflectx();
}

fvector dcountac(double t) {
  return fvector(M_SQRT_2,-M_SQRT_2);
}
fvector dcountbd(double t) {
  return fvector(-M_SQRT_2,-M_SQRT_2);
}

inline fvector dcountbc(double t) {
  double tt=(curve2len-t)/curve2len*M_PI_2;
  return fvector(-sin(tt+3*M_PI_4),cos(tt-M_PI_4)).size1();
}
inline fvector dcountcd(double t) {
  return dcountbc(t).transp().reflecty();
}
inline fvector dcountad(double t) {
  return dcountbc(curve2len-t).reflectx();
}
inline fvector dcountab(double t) {
  return dcountbc(curve2len-t).transp();
}


void init_tt_cons() {
  for(int i=0;i<=vr_cnt;i++) for(int j=0;j<=vr_cnt;j++) tt_cons[i][j]=tt_none;
  for(int i=-tt_cnt;i<=tt_cnt;i++) {//pro vsechny druhy cest
    int beg=Spojnice(i).beg().data;
    int end=Spojnice(i).end().data;
    tt_cons[beg][end]=i;
  }
}

static void countpts(Spojnice tt,float t,float rad1,float rad2,fpoint &p1,fpoint &p2) {
  fpoint p=tt.pos(t);
  fvector d=tt.diff(t);
  fvector k=d.getnormal();
  p1=p+k*rad1;p2=p+k*rad2;
}

static void render_point(fpoint p,float ha,float hb,float hc,float hd,float x0,float y0) {
  glVertex3f(p.x+x0,ha+p.x*(hb-ha)+p.y*(hd-ha),p.y+y0);
}

#define count_xyhs\
  float ha=ri->za+dhi,hb=ri->za+ri->dzx+dhi,hc=ri->za+ri->dzx+ri->dzy+dhi,hd=ri->za+ri->dzy+dhi;\
  float x0=ri->x,y0=ri->y;

void draw_gravel(Spojnice tt,RItem *ri,float dhi,RenderContext &rc,float grav_w,float quality) {
  count_xyhs

  double len=tt.len();
  double d=len/round((len/quality+1));

  if (tt.streight()) d=len;

  fpoint old1,old2;
  countpts(tt,0,grav_w/2,-grav_w/2,old1,old2);

  for(double t=d;t<len+epsilon;t+=d) {
    fpoint p1,p2;
    countpts(tt,t,grav_w/2,-grav_w/2,p1,p2);
    glBegin(GL_QUADS);
      glTexCoord2f(old1.x,old1.y);render_point(old1,ha,hb,hc,hd,x0,y0);
      glTexCoord2f(old2.x,old2.y);render_point(old2,ha,hb,hc,hd,x0,y0);
      glTexCoord2f(p2.x,p2.y);render_point(p2,ha,hb,hc,hd,x0,y0);
      glTexCoord2f(p1.x,p1.y);render_point(p1,ha,hb,hc,hd,x0,y0);
    glEnd();
    old1=p1;old2=p2;
    if (tt.streight()) break;
  }
}

void draw_sleepers(Spojnice tt,RItem *ri,float dhi,RenderContext &rc,float sl_len,float sl_wi,float sl_cover) {
  count_xyhs

  double len=tt.len();
  int cnt=round(len/sl_cover);
  if (cnt<=0) return;
  double d=len/cnt;

  for(double t=d/2;t<len;t+=d) {
    fpoint p=tt.pos(t);
    fvector d=tt.diff(t);
    fvector k=d.getnormal();
    k*=sl_len/2;
    fpoint p1=p+k,p2=p-k;
    d*=sl_wi/2;
    fpoint p1a=p1+d,p1b=p1-d,p2a=p2+d,p2b=p2-d;
    glBegin(GL_QUADS);
      glTexCoord2f(0,0);render_point(p1a,ha,hb,hc,hd,x0,y0);
      glTexCoord2f(0,1);render_point(p1b,ha,hb,hc,hd,x0,y0);
      glTexCoord2f(1,1);render_point(p2b,ha,hb,hc,hd,x0,y0);
      glTexCoord2f(1,0);render_point(p2a,ha,hb,hc,hd,x0,y0);
    glEnd();
  }
}

void draw_rail(Spojnice tt,RItem *ri,float dhi,RenderContext &rc,float quality,float rail_w,float gauge_2) {
  count_xyhs

  double len=tt.len();
  double d=len/round((len/quality+1));

  if (tt.streight()) d=len;

  float r1=gauge_2+rail_w/2;
  float r2=gauge_2-rail_w/2;

  fpoint old1,old2;
  countpts(tt,0,r1,r2,old1,old2);
  float oldt=0;

  for(double t=d;t<len+epsilon;t+=d) {
    fpoint p1,p2;
    countpts(tt,t,r1,r2,p1,p2);
    glBegin(GL_QUADS);
      glTexCoord2f(oldt,0);render_point(old1,ha,hb,hc,hd,x0,y0);
      glTexCoord2f(oldt,1);render_point(old2,ha,hb,hc,hd,x0,y0);
      glTexCoord2f(t,1);render_point(p2,ha,hb,hc,hd,x0,y0);
      glTexCoord2f(t,0);render_point(p1,ha,hb,hc,hd,x0,y0);
    glEnd();
    old1=p1;old2=p2;oldt=t;
    if (tt.streight()) break;
  }
}

static fpoint3d convertpt(float x0,float y0,float ha,float hb,float hc,float hd,fpoint pt) {
  fpoint3d res;
  res.x=pt.x+x0;
  res.y=ha+pt.x*(hb-ha)+pt.y*(hd-ha);
  res.z=pt.y+y0;
  return res;
}

void draw_wire(Spojnice tt,RItem *ri,float dhi,RenderContext &rc,float quality,float gauge_2,float linewi,bool texture) {
  count_xyhs

  double len=tt.len();
  double d=len/round((len/quality+1));

  if (tt.streight()) d=len;

  if (texture) {
    fpoint3d old;
    fpoint pt2;
    countpts(tt,0,gauge_2,gauge_2,pt2,pt2);
    old=convertpt(x0,y0,ha,hb,hc,hd,pt2);
    float oldt=0;
    for(double t=d;t<len+epsilon;t+=d) {
      fpoint3d p;
      countpts(tt,t,gauge_2,gauge_2,pt2,pt2);
      p=convertpt(x0,y0,ha,hb,hc,hd,pt2);
      glTexLine(rc,oldt,t,linewi,old,p);
      old=p;oldt=t;
      if (tt.streight()) break;
    }
  } else {
    fpoint old;
    countpts(tt,0,gauge_2,gauge_2,old,old);
    float oldt=0;


    glBegin(GL_LINE_STRIP);
    render_point(old,ha,hb,hc,hd,x0,y0);
    for(double t=d;t<len+epsilon;t+=d) {
      fpoint p;
      countpts(tt,t,gauge_2,gauge_2,p,p);
      render_point(p,ha,hb,hc,hd,x0,y0);
      if (tt.streight()) break;
    }
    glEnd();
  }

}


}