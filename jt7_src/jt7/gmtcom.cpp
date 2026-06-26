#include <jlib.h>
#pragma hdrstop

#include "gmtcom.h"
#include <jmath/analgeom.cpp>
#include "groad.h"
#include "gimages.h"

namespace jtyc {

int mt_cons[mv_cnt+1][mv_cnt+1];
bool mt_cross[2*mt_cnt+1][2*mt_cnt+1];

void init_mt_cons() {
  for(int i=0;i<=mv_cnt;i++) for(int j=0;j<=mv_cnt;j++) mt_cons[i][j]=mt_none;
  for(int i=-mt_cnt;i<=mt_cnt;i++) {//pro vsechny druhy cest
    int beg=MSpojnice(i).beg().data;
    int end=MSpojnice(i).end().data;
    mt_cons[beg][end]=i;
  }
  float mdist=0.2;
  for(int i=-mt_cnt;i<=mt_cnt;i++) for(int j=-mt_cnt;j<=mt_cnt;j++) {
    bool cr=true;
    if (i && j && i!=j) {
      MSpojnice si(i),sj(j);
      fpoint a1=si.pos(0,mdist),a2=si.pos(si.len(mdist),mdist);
      fpoint b1=sj.pos(0,mdist),b2=sj.pos(sj.len(mdist),mdist);
      fvector da=a2-a1,db=b2-b1;
      agline<float> a(a1,da),b(b1,db);
      if (distance(da.size1(),db.size1())<jtyc::epsilon && a.pdist(b1)<jtyc::epsilon) cr=true;
      else {
        fpoint com;
        float t1=0,t2=0;
        cr=a.compoint(b,com,&t1) || b.compoint(a,com,&t2);
        if (cr) cr=t1>=0 && t1<=1 && t2>=0 && t2<=1;
      }
    }
    mt_cross[i+mt_cnt][j+mt_cnt]=cr;
  }
}
//rovne
fpoint mcount13(double t,double dist) {
  return fpoint(0.5+dist,1-t);
}
fpoint mcount24(double t,double dist) {
  return fpoint(1-t,0.5-dist);
}
//opacne
fpoint mcount31(double t,double dist) {
  return fpoint(0.5-dist,t);
}
fpoint mcount42(double t,double dist) {
  return fpoint(t,0.5+dist);
}
//pres uhel
fpoint mcount12(double t,double dist) {
  double tt=t/mlen1(dist);
  return fpoint(0.5+dist+tt*(0.5-dist),1-tt*(0.5-dist));
}
fpoint mcount23(double t,double dist) {
  return mcount12(mlen1(dist)-t,dist).reflecty(1);
}
fpoint mcount34(double t,double dist) {
  return mcount23(mlen1(dist)-t,dist).reflectx(1);
}
fpoint mcount14(double t,double dist) {
  double tt=t/mlen2(dist);
  return fpoint(0.5+dist-tt*(0.5+dist),1-tt*(0.5+dist));
}
//opacne
fpoint mcount21(double t,double dist) {
  return mcount14(mlen2(dist)-t,dist).reflectx(1);
}
fpoint mcount32(double t,double dist) {
  return mcount21(mlen2(dist)-t,dist).reflecty(1);
}
fpoint mcount43(double t,double dist) {
  return mcount14(mlen2(dist)-t,dist).reflecty(1);
}
fpoint mcount41(double t,double dist) {
  return mcount12(mlen1(dist)-t,dist).reflectx(1);
}

double MSpojnice::len(double dist) {
  switch (data) {
    case mt_13:case mt_24:case -mt_13:case -mt_24:return 1;
    case mt_12:case mt_23:case mt_34:case -mt_14:return (0.5-dist)*M_SQRT2;
    case -mt_12:case -mt_23:case -mt_34:case mt_14:return (0.5+dist)*M_SQRT2;
  }
  return 0;
}


static float rpt_dzx,rpt_dzy;

static float rpt_dy=0;

static void renderpos(float x,float y) {
  glVertex3f(x,x*rpt_dzx+y*rpt_dzy+rpt_dy,y);
}

static void renderpt(float x,float y) {
  glTexCoord2f(x,y);
  renderpos(x,y);
}

static void renderquad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4) {
  glTexCoord2f(0,0);renderpos(x1,y1);
  glTexCoord2f(0,1);renderpos(x2,y2);
  glTexCoord2f(1,1);renderpos(x3,y3);
  glTexCoord2f(1,0);renderpos(x4,y4);
}

void draw_tramrail(RoadItem *ri,float dhi,RenderContext &rc,float gauge2,float wi,float motodist) {
  rpt_dzx=ri->dzx;rpt_dzy=ri->dzy;
  rpt_dy=dhi;

  #define accept(a,b) (ri->a && ri->a->road->hastramrail && ri->b && ri->b->road->hastramrail)

  glBegin(GL_QUADS);
  if (accept(ns,nj)) {
    renderquad(0.5+motodist+gauge2-wi,0,  0.5+motodist+gauge2+wi,0, 0.5+motodist+gauge2+wi,1, 0.5+motodist+gauge2-wi,1);
    renderquad(0.5+motodist-gauge2-wi,0,  0.5+motodist-gauge2+wi,0, 0.5+motodist-gauge2+wi,1, 0.5+motodist-gauge2-wi,1);

    renderquad(0.5-motodist+gauge2-wi,0,  0.5-motodist+gauge2+wi,0, 0.5-motodist+gauge2+wi,1, 0.5-motodist+gauge2-wi,1);
    renderquad(0.5-motodist-gauge2-wi,0,  0.5-motodist-gauge2+wi,0, 0.5-motodist-gauge2+wi,1, 0.5-motodist-gauge2-wi,1);
  }

  if (accept(nv,nz)) {
    renderquad(0,0.5+motodist+gauge2-wi,  0,0.5+motodist+gauge2+wi, 1,0.5+motodist+gauge2+wi, 1,0.5+motodist+gauge2-wi);
    renderquad(0,0.5+motodist-gauge2-wi,  0,0.5+motodist-gauge2+wi, 1,0.5+motodist-gauge2+wi, 1,0.5+motodist-gauge2-wi);

    renderquad(0,0.5-motodist+gauge2-wi,  0,0.5-motodist+gauge2+wi, 1,0.5-motodist+gauge2+wi, 1,0.5-motodist+gauge2-wi);
    renderquad(0,0.5-motodist-gauge2-wi,  0,0.5-motodist-gauge2+wi, 1,0.5-motodist-gauge2+wi, 1,0.5-motodist-gauge2-wi);
  }

//=================================

  if (accept(ns,nv)) {
    renderquad(0.5+motodist+gauge2-wi,0,  0.5+motodist+gauge2+wi,0, 1,0.5-motodist-gauge2+wi, 1,0.5-motodist-gauge2-wi);
    renderquad(0.5+motodist-gauge2-wi,0,  0.5+motodist-gauge2+wi,0, 1,0.5-motodist+gauge2+wi, 1,0.5-motodist+gauge2-wi);

    renderquad(0.5-motodist+gauge2-wi,0,  0.5-motodist+gauge2+wi,0, 1,0.5+motodist-gauge2+wi, 1,0.5+motodist-gauge2-wi);
    renderquad(0.5-motodist-gauge2-wi,0,  0.5-motodist-gauge2+wi,0, 1,0.5+motodist+gauge2+wi, 1,0.5+motodist+gauge2-wi);
  }

  if (accept(nj,nv)) {
    renderquad(0.5+motodist+gauge2+wi,1, 0.5+motodist+gauge2-wi,1, 1,0.5+motodist+gauge2+wi, 1,0.5+motodist+gauge2-wi);
    renderquad(0.5+motodist-gauge2+wi,1, 0.5+motodist-gauge2-wi,1, 1,0.5+motodist-gauge2+wi, 1,0.5+motodist-gauge2-wi);

    renderquad(0.5-motodist+gauge2+wi,1, 0.5-motodist+gauge2-wi,1, 1,0.5-motodist+gauge2+wi, 1,0.5-motodist+gauge2-wi);
    renderquad(0.5-motodist-gauge2+wi,1, 0.5-motodist-gauge2-wi,1, 1,0.5-motodist-gauge2+wi, 1,0.5-motodist-gauge2-wi);
  }

  if (accept(ns,nz)) {
    renderquad(0.5+motodist+gauge2+wi,0, 0.5+motodist+gauge2-wi,0, 0,0.5+motodist+gauge2+wi, 0,0.5+motodist+gauge2-wi);
    renderquad(0.5+motodist-gauge2+wi,0, 0.5+motodist-gauge2-wi,0, 0,0.5+motodist-gauge2+wi, 0,0.5+motodist-gauge2-wi);

    renderquad(0.5-motodist+gauge2+wi,0, 0.5-motodist+gauge2-wi,0, 0,0.5-motodist+gauge2+wi, 0,0.5-motodist+gauge2-wi);
    renderquad(0.5-motodist-gauge2+wi,0, 0.5-motodist-gauge2-wi,0, 0,0.5-motodist-gauge2+wi, 0,0.5-motodist-gauge2-wi);
  }

  if (accept(nj,nz)) {
    renderquad(0.5+motodist+gauge2+wi,1, 0.5+motodist+gauge2-wi,1, 0,0.5-motodist-gauge2+wi, 0,0.5-motodist-gauge2-wi);
    renderquad(0.5+motodist-gauge2+wi,1, 0.5+motodist-gauge2-wi,1, 0,0.5-motodist+gauge2+wi, 0,0.5-motodist+gauge2-wi);

    renderquad(0.5-motodist+gauge2+wi,1, 0.5-motodist+gauge2-wi,1, 0,0.5+motodist-gauge2+wi, 0,0.5+motodist-gauge2-wi);
    renderquad(0.5-motodist-gauge2+wi,1, 0.5-motodist-gauge2-wi,1, 0,0.5+motodist+gauge2+wi, 0,0.5+motodist+gauge2-wi);
  }

  glEnd();

  #undef accept
}

void draw_roadwire(RoadItem *ri,float dhi,RenderContext &rc,float gauge2,float wi,float motodist) {
  #define accept(a,b) (ri->a && ri->a->road->haswire && ri->b && ri->b->road->haswire)

  if (accept(ns,nj)) {
    glTexLine(rc,0,1,wi,fpoint3d(0.5+motodist+gauge2,dhi,0),fpoint3d(0.5+motodist+gauge2,dhi+ri->dzy,1));
    glTexLine(rc,0,1,wi,fpoint3d(0.5-motodist+gauge2,dhi,0),fpoint3d(0.5-motodist+gauge2,dhi+ri->dzy,1));
  }

  if (accept(nv,nz)) {
    glTexLine(rc,0,1,wi,fpoint3d(0,dhi,0.5+motodist+gauge2),fpoint3d(1,dhi+ri->dzx,0.5+motodist+gauge2));
    glTexLine(rc,0,1,wi,fpoint3d(0,dhi,0.5-motodist+gauge2),fpoint3d(1,dhi+ri->dzx,0.5-motodist+gauge2));
  }
  //============

  if (accept(ns,nv)) {
    glTexLine(rc,0,1,wi,fpoint3d(0.5+motodist+gauge2,dhi,0),fpoint3d(1,dhi,0.5-motodist+gauge2));
    glTexLine(rc,0,1,wi,fpoint3d(0.5-motodist+gauge2,dhi,0),fpoint3d(1,dhi,0.5+motodist+gauge2));
  }

  if (accept(nj,nv)) {
    glTexLine(rc,0,1,wi,fpoint3d(0.5+motodist+gauge2,dhi,1),fpoint3d(1,dhi,0.5+motodist+gauge2));
    glTexLine(rc,0,1,wi,fpoint3d(0.5-motodist+gauge2,dhi,1),fpoint3d(1,dhi,0.5-motodist+gauge2));
  }

  if (accept(ns,nz)) {
    glTexLine(rc,0,1,wi,fpoint3d(0.5+motodist+gauge2,dhi,0),fpoint3d(0,dhi,0.5+motodist+gauge2));
    glTexLine(rc,0,1,wi,fpoint3d(0.5-motodist+gauge2,dhi,0),fpoint3d(0,dhi,0.5-motodist+gauge2));
  }

  if (accept(nj,nz)) {
    glTexLine(rc,0,1,wi,fpoint3d(0.5+motodist+gauge2,dhi,1),fpoint3d(0,dhi,0.5-motodist+gauge2));
    glTexLine(rc,0,1,wi,fpoint3d(0.5-motodist+gauge2,dhi,1),fpoint3d(0,dhi,0.5+motodist+gauge2));
  }

  #undef accept
}

void draw_gravel(RoadItem *ri,float dhi,RenderContext &rc,float d1,float d2,float d3) {
  rpt_dzx=ri->dzx;rpt_dzy=ri->dzy;
  rpt_dy=dhi;

  if (ri->ns && ri->nv && !ri->nj && !ri->nz) {
    glBegin(GL_QUADS);
    renderpt(d1,0);
    renderpt(1-d1,0);
    renderpt(1,d1);
    renderpt(1,1-d1);
    glEnd();
  } else if (ri->ns && ri->nz && !ri->nj && !ri->nv) {
    glBegin(GL_QUADS);
    renderpt(d1,0);
    renderpt(1-d1,0);
    renderpt(0,1-d1);
    renderpt(0,d1);
    glEnd();
  } else if (ri->nj && ri->nz && !ri->ns && !ri->nv) {
    glBegin(GL_QUADS);
    renderpt(d1,1);
    renderpt(1-d1,1);
    renderpt(0,d1);
    renderpt(0,1-d1);
    glEnd();
  } else if (ri->nj && ri->nv && !ri->ns && !ri->nz) {
    glBegin(GL_QUADS);
    renderpt(d1,1);
    renderpt(1-d1,1);
    renderpt(1,1-d1);
    renderpt(1,d1);
    glEnd();
  } else {
    glBegin(GL_POLYGON);

    renderpt(d1+d2,d1);
    renderpt(1-d1-d2,d1);
    renderpt(1-d1,d1+d2);
    renderpt(1-d1,1-d1-d2);
    renderpt(1-d1-d2,1-d1);
    renderpt(d1+d2,1-d1);
    renderpt(d1,1-d1-d2);
    renderpt(d1,d1+d2);


    glEnd();


    glBegin(GL_QUADS);

    if (ri->nz) {
      renderpt(0,d1);
      renderpt(d1,d1);
      renderpt(d1,1-d1);
      renderpt(0,1-d1);
    }

    if (ri->nv) {
      renderpt(1,d1);
      renderpt(1-d1,d1);
      renderpt(1-d1,1-d1);
      renderpt(1,1-d1);
    }
  
    if (ri->ns) {
      renderpt(d1,0);
      renderpt(d1,d1);
      renderpt(1-d1,d1);
      renderpt(1-d1,0);
    }
  
    if (ri->nj) {
      renderpt(d1,1);
      renderpt(d1,1-d1);
      renderpt(1-d1,1-d1);
      renderpt(1-d1,1);
    }
  
    glEnd();
  
    glBegin(GL_TRIANGLES);
  
    if (ri->ns && ri->nz) {
      renderpt(d1-d3,d1);
      renderpt(d1,d1-d3);
      renderpt(d1,d1);
    }
  
    if (ri->ns && ri->nv) {
      renderpt(1-d1+d3,d1);
      renderpt(1-d1,d1-d3);
      renderpt(1-d1,d1);
    }
  
    if (ri->nj && ri->nz) {
      renderpt(d1-d3,1-d1);
      renderpt(d1,1-d1+d3);
      renderpt(d1,1-d1);
    }
  
    if (ri->nj && ri->nv) {
      renderpt(1-d1+d3,1-d1);
      renderpt(1-d1,1-d1+d3);
      renderpt(1-d1,1-d1);
    }
  
    if (ri->ns || ri->nz) {
      renderpt(d1+d2,d1);
      renderpt(d1,d1+d2);
      renderpt(d1,d1);
    }
  
    if (ri->ns || ri->nv) {
      renderpt(1-d1-d2,d1);
      renderpt(1-d1,d1+d2);
      renderpt(1-d1,d1);
    }
  
    if (ri->nj || ri->nz) {
      renderpt(d1+d2,1-d1);
      renderpt(d1,1-d1-d2);
      renderpt(d1,1-d1);
    }
  
    if (ri->nj || ri->nv) {
      renderpt(1-d1-d2,1-d1);
      renderpt(1-d1,1-d1-d2);
      renderpt(1-d1,1-d1);
    }
  
    glEnd();
  }
}

void draw_midline(RoadItem *ri,float dhi,RenderContext &rc,float wi) {
  rpt_dzx=ri->dzx;rpt_dzy=ri->dzy;
  rpt_dy=dhi;

  if (ri->ns && ri->nj && !ri->nz && !ri->nv) {
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);renderpos(0.5-wi/2,0);
    glTexCoord2f(0,1);renderpos(0.5+wi/2,0);
    glTexCoord2f(1,1);renderpos(0.5+wi/2,1);
    glTexCoord2f(1,0);renderpos(0.5-wi/2,1);
    glEnd();
  }
  if (!ri->ns && !ri->nj && ri->nz && ri->nv) {
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);renderpos(0,0.5-wi/2);
    glTexCoord2f(0,1);renderpos(0,0.5+wi/2);
    glTexCoord2f(1,1);renderpos(1,0.5+wi/2);
    glTexCoord2f(1,0);renderpos(1,0.5-wi/2);
    glEnd();
  }
}


};

