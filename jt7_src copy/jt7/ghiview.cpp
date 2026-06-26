#include <jlib.h>
#pragma hdrstop

#include "ghiview.h"
#include "ghimap.h"
#include "gsurmap.h"
#include "gameu.h"
#include "gimages.h"
#include "gview.h"
#include "istatic.h"

namespace jtyc {

#define mac_vertex(x,y) verteces(x-x0,y-y0)
#define mac_face(x,y) faces(x-x0,y-y0)
#define mac_hiat(x,y) mac_vertex(x,y).hi
#define mac_whiat(x,y) mac_vertex(x,y).waterhi

const bool textured=true;

HiView::HiView() {
  x0=y0=0;
  w=h=0;
  boundsqx1=boundsqy1=boundsqx2=boundsqy2=0;
  boundvx1=boundvy1=boundvx2=boundvy2=0;
}

void HiView::init() {

}

void HVFace::settunnel(bool less,bool vertical,float dhi) {
  if (vertical) {
    if (less) tunneldir=0;else tunneldir=2;
    tunneldhi=-dhi;
  } else {
    tunneldhi=dhi;
    if (less) tunneldir=1;else tunneldir=3;
  }
}

void HVVertex::countintensity(const fvector3d &L) {
  fvector3d N=n;
  if (N.y<=0) N=-N;
  float k=N*L;
  k/=2;
  k+=1;
  intensity=k;
}

void HiView::beforeiterate() {
  boundsqx1=game->himap->boundsqx1;
  boundsqy1=game->himap->boundsqy1;
  boundsqx2=game->himap->boundsqx2;
  boundsqy2=game->himap->boundsqy2;

  boundvx1=game->himap->boundvx1;
  boundvy1=game->himap->boundvy1;
  boundvx2=game->himap->boundvx2;
  boundvy2=game->himap->boundvy2;

  x0=boundsqx1-1,y0=boundsqy1-1;
  w=boundsqx2-boundsqx1+3;
  h=boundsqy2-boundsqy1+3;
  faces.init(w,h);
  verteces.init(w+1,h+1);
  for(int y=y0;y<=y0+h;y++) for(int x=x0;x<=x0+w;x++) {
    HVVertex &v=mac_vertex(x,y);
    v.x=x;v.y=y;
    if (VertexStruct *vsrc=game->himap->verteces.tryat(x,y)) {
      v.hi=vsrc->hi;
      v.waterhi=vsrc->waterhi;
    }
  }
  for(int y=y0;y<y0+h;y++) for(int x=x0;x<x0+w;x++) {
    HVFace &f=mac_face(x,y);
    f.x=x;f.y=y;
    f.terrain=gimages->terrain0;
    if (FaceStruct *fsrc=game->himap->faces.tryat(x,y)) {
      f.octvisible=fsrc->octvisible;
//      f.tunneldir=fsrc->tunneldir;
//      f.tshor=fsrc->tshor;
//      f.tsver=fsrc->tsver;
//      f.tunneldhi=fsrc->tunneldhi;
      f.terrain=game->surmap->map(x,y).terrain;
    }
  }
  for(int y=y0+1;y<y0+h;y++) for(int x=x0+1;x<x0+w;x++) {
    mac_vertex(x,y).scrpt=game->himap->vertexat(x,y).countscrpt();
  }

  int st0=clock();

  for(int y=y0;y<y0+h;y++) for(int x=x0;x<x0+w;x++) {
    HVFace &f=mac_face(x,y);
    float ha=mac_vertex(x+1,y+1).hi,hb=mac_vertex(x,y+1).hi,hc=mac_vertex(x+1,y).hi;
    fpoint3d a(x+1,ha,y+1),b(x,hb,y+1),c(x+1,hc,y);
    fvector3d u=a-b,v=a-c;
    fvector3d normal=u%v;//normala
    f.n=normal.size1();
  }
  for(int y=y0+1;y<y0+h;y++) for(int x=x0+1;x<x0+w;x++) {
    fvector3d v1=mac_face(x-1,y-1).n,
              v2=mac_face(x,y-1).n,
              v3=mac_face(x-1,y).n,
              v4=mac_face(x,y).n;
    mac_vertex(x,y).n=(v1+v2+v3+v4)*0.25;
  }

  fvector3d sun(1,-1,0);
  fmatrix3d rot;rot.dorotatey(game->view->getviewangle()/180*M_PI);
  sun=rot*sun;
  fvector3d L=sun.size1();

  for(int y=y0;y<=y0+h;y++) for(int x=x0;x<=x0+w;x++) {
    HVVertex &v=mac_vertex(x,y);
    v.countintensity(L);
  }

  int tim0=clock()-st0;
  float ft0=tim0/(float)CLK_TCK;

  //=========================================
  //countdges()

//  for(int y=y0;y<y0+h;y++) for(int x=x0;x<x0+w;x++) {
//    map(x,y).ea=map(x,y).eb=map(x,y).ec=map(x,y).ed=NULL;
//  }
  for(int y=y0+1;y<y0+h-1;y++) for(int x=x0+1;x<x0+w-1;x++) {
    HVFace &it=mac_face(x,y);
    {
      HVFace *d=&mac_face(x+1,y+1);
      if (d->terrain==it.terrain) {
        HVFace &n1=mac_face(x,y+1),&n2=mac_face(x+1,y);
        if (n1.terrain!=it.terrain && n2.terrain==it.terrain) n1.ec=it.terrain;
        if (n2.terrain!=it.terrain && n1.terrain==it.terrain) n2.ea=it.terrain;
      }
    }
    {
      HVFace *d=&mac_face(x+1,y-1);
      if (d->terrain==it.terrain) {
        HVFace &n1=mac_face(x,y-1),&n2=mac_face(x+1,y);
        if (n1.terrain!=it.terrain && n2.terrain==it.terrain) n1.eb=it.terrain;
        if (n2.terrain!=it.terrain && n1.terrain==it.terrain) n2.ed=it.terrain;
      }
    }
  }

  for(int y=y0;y<y0+h;y++) for(int x=x0;x<x0+w-1;x++) {
    HVFace &a1=mac_face(x,y),&a2=mac_face(x+1,y);
    if (a1.terrain==a2.terrain) {
      if (y>y0) {
        HVFace &b1=mac_face(x,y-1),&b2=mac_face(x+1,y-1);
        if (b1.terrain!=b2.terrain) {
          if (!b1.eb) b1.eb=a1.terrain;
          if (!b2.ea) b2.ea=a1.terrain;
        }
      }
      if (y<y0+h-1) {
        HVFace &b1=mac_face(x,y+1),&b2=mac_face(x+1,y+1);
        if (b1.terrain!=b2.terrain) {
          if (!b1.ec) b1.ec=a1.terrain;
          if (!b2.ed) b2.ed=a1.terrain;
        }
      }
    }
  }

  for(int y=y0;y<y0+h-1;y++) for(int x=x0;x<x0+w;x++) {
    HVFace &a1=mac_face(x,y),&a2=mac_face(x,y+1);
    if (a1.terrain==a2.terrain) {
      if (x>x0) {
        HVFace &b1=mac_face(x-1,y),&b2=mac_face(x-1,y+1);
        if (b1.terrain!=b2.terrain) {
          if (!b1.eb) b1.eb=a1.terrain;
          if (!b2.ec) b2.ec=a1.terrain;
        }
      }
      if (x<x0+w-1) {
        HVFace &b1=mac_face(x+1,y),&b2=mac_face(x+1,y+1);
        if (b1.terrain!=b2.terrain) {
          if (!b1.ea) b1.ea=a1.terrain;
          if (!b2.ed) b2.ed=a1.terrain;
        }
      }
    }
  }
}








inline void fcolor(float color) {glColor3f(color,color,color);}

bool HiMap::drysq(int x,int y) {
  return verteces(x,y).waterhi<0 && verteces(x+1,y).waterhi<0 && verteces(x,y+1).waterhi<0 && verteces(x+1,y+1).waterhi<0;
}

struct PointStruct {
  float i;//intenzita svetla
  float t;//tex-coord
  float s;//tex-coord
  float x;//map-coord
  float h;//vyska
  float y;//map-coord
  PointStruct(float ai,float at,float as,float ax,float ah,float ay) {
    i=ai;t=at;s=as;x=ax;h=ah;y=ay;
  }
  void render() {
    fcolor(i);glTexCoord2f(t,s);glVertex3f(x,h,y);
  }
};

PointStruct mixpt(PointStruct &a,float k,PointStruct &b,float l) {
  return PointStruct(a.i*k+b.i*l,a.t*k+b.t*l,a.s*k+b.s*l,a.x*k+b.x*l,a.h*k+b.h*l,a.y*k+b.y*l);
}

jtTerrain *curglter;

/*
1     3


      2
*/
static void triangle(//1,2 je diagonala
    RenderContext &rc,
    PointStruct &p1,jtTerrain *e1,
    PointStruct &p2,jtTerrain *e2,
    PointStruct &p3,jtTerrain *e3
  ) {
  //na barvy i vysky se uziva linearni interpolace (vazene prumery)
//  fcolor(i1*0.75+i2*0.25);glTexCoord(
  if (!e1 && !e2 && !e3) {
    p1.render();
    p2.render();
    p3.render();
    return;
  }

  glEnd();
  if (e1) e1->bind(rc);else curglter->bind(rc);
  glBegin(GL_TRIANGLES);

  p1.render();
  mixpt(p1,0.75,p2,0.25).render();
  mixpt(p1,0.5,p3,0.5).render();

  glEnd();
  if (e2) e2->bind(rc);else curglter->bind(rc);
  glBegin(GL_TRIANGLES);

  p2.render();
  mixpt(p2,0.75,p1,0.25).render();
  mixpt(p2,0.5,p3,0.5).render();

  glEnd();
  if (e3) e3->bind(rc);else curglter->bind(rc);
  glBegin(GL_TRIANGLES);

  p3.render();
  mixpt(p1,0.5,p3,0.5).render();
  mixpt(p2,0.5,p3,0.5).render();

  glEnd();
  curglter->bind(rc);
  glBegin(GL_QUADS);

  mixpt(p1,0.75,p2,0.25).render();
  mixpt(p1,0.5,p3,0.5).render();
  mixpt(p3,0.5,p2,0.5).render();
  mixpt(p2,0.75,p1,0.25).render();

  glEnd();
  glBegin(GL_TRIANGLES);
}

bool HiView::drawwaterface(int x,int y) {
  return verteces(x-x0,y-y0).iswater() || verteces(x+1-x0,y-y0).iswater()
  || verteces(x-x0,y+1-y0).iswater() || verteces(x+1-x0,y+1-y0).iswater();
}

void HiView::render(RenderContext &rc) {
/*
  int part=0,oct=0;
  for(int i=0;i<faces.size;i++) {
    if (faces[i].octvisible) oct++;
    if (faces[i].partvisible) part++;
  }
*/
//  game->surmap->wantedges();
  rc.setculling(false);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  if (textured) {
    bool savet2d=rc.settexture2d(true);
//    glBindTexture(GL_TEXTURE_2D,jtdata.grasstexture);
//    game->surmap
//    gimages->terrain0->bind(rc);
    curglter=gimages->terrain0;
    curglter->bind(rc);
    glBegin(GL_TRIANGLES);
//    glEnable(GL_BLEND);
    for(int y=boundsqy1;y<=boundsqy2;y++) {
      for(int x=boundsqx1;x<=boundsqx2;x++) {
//        if (!mac_face(x,y).partvisible) continue;
/*
        if (y==boundvy1) {
          if (x==boundvx1) mac_vertex(x,y).countintensity(L);
          if (x<boundvx2) mac_vertex(x+1,y).countintensity(L);
        }
        if (y<boundvy2) {
          if (x==boundvx1) mac_vertex(x,y+1).countintensity(L);
          if (x<boundvx2) mac_vertex(x+1,y+1).countintensity(L);
        }
*/
        if (!mac_face(x,y).octvisible) continue;
        float h1=mac_hiat(x,y),h2=mac_hiat(x+1,y),h3=mac_hiat(x+1,y+1),h4=mac_hiat(x,y+1);
        float w1=mac_whiat(x,y),w2=mac_whiat(x+1,y),w3=mac_whiat(x+1,y+1),w4=mac_whiat(x,y+1);
        if (w1>0 && w2>0 && w3>0 && w4>0) continue;

        HVFace &face=mac_face(x,y);
//        SItem *sitem=game->surmap->at(x,y);
        jtTerrain *actter=face.terrain;
        if (actter!=curglter) {
          curglter=actter;
          glEnd();
          curglter->bind(rc);
          glBegin(GL_TRIANGLES);
        }
        float i1=mac_vertex(x,y).intensity,i2=mac_vertex(x+1,y).intensity,
              i3=mac_vertex(x+1,y+1).intensity,i4=mac_vertex(x,y+1).intensity;
        if (face.tunneldir>=0) {//tunel
          glEnd();
          int dir=mac_face(x,y).tunneldir;
          float dhi=mac_face(x,y).tunneldhi;
          float tshor=mac_face(x,y).tshor,tsver=mac_face(x,y).tsver;
          float iavg=(i1+i2+i3+i4)/4;
//          float is0[4]={i1,i2,i3,i4};
//          float is[4]={is0[(0+dir)%4],is0[(1+dir)%4],is0[(2+dir)%4],is0[(3+dir)%4]};
          float hlo=jmin(h1,h2,h3,h4),hhi=jmax(h1,h2,h3,h4);
          glMatrixMode(GL_MODELVIEW);
          glPushMatrix();
          glTranslatef(x,hlo,y);
          jt_transformate(dir,1,1);
//          glMapCrook(mac_face(x,y).tunneldir%2,mac_face(x,y).tunneldhi);
          glMapCrook(true,dir==1||dir==2?dhi:-dhi);
          glBegin(GL_QUADS);
          fcolor(iavg);glTexCoord2f(0,0);glVertex3f(0,0,0);
          fcolor(iavg);glTexCoord2f(0,1);glVertex3f(0,1,1);
          fcolor(iavg);glTexCoord2f(tshor,1);glVertex3f(tshor,1,1);
          fcolor(iavg);glTexCoord2f(tshor,0);glVertex3f(tshor,0,0);

          fcolor(iavg);glTexCoord2f(1,0);glVertex3f(1,0,0);
          fcolor(iavg);glTexCoord2f(1,1);glVertex3f(1,1,1);
          fcolor(iavg);glTexCoord2f(1-tshor,1);glVertex3f(1-tshor,1,1);
          fcolor(iavg);glTexCoord2f(1-tshor,0);glVertex3f(1-tshor,0,0);

          fcolor(iavg);glTexCoord2f(tshor,1-tsver);glVertex3f(tshor,1-tsver,1-tsver);
          fcolor(iavg);glTexCoord2f(tshor,1);glVertex3f(tshor,1,1);
          fcolor(iavg);glTexCoord2f(1-tshor,1);glVertex3f(1-tshor,1,1);
          fcolor(iavg);glTexCoord2f(1-tshor,1-tsver);glVertex3f(1-tshor,1-tsver,1-tsver);
          glEnd();

/*
          if (!game->view->type) {
            glColor3f(0,0,0);
            rc.settexture2d(false);
            glBegin(GL_QUADS);
            glVertex3f(0,0,1);
            glVertex3f(0,1,1);
            glVertex3f(1,1,1);
            glVertex3f(1,0,1);
            glEnd();
            rc.settexture2d(true);
          }
*/

          glPopMatrix();
          glBegin(GL_TRIANGLES);
        } else if (fabs(h1-h3)<fabs(h2-h4)) {//trojuhelniky dle hlavni diagonaly
          triangle(rc,
            PointStruct(i1,0,0,x,h1,y),face.ed,
            PointStruct(i3,1,1,x+1,h3,y+1),face.eb,
            PointStruct(i2,1,0,x+1,h2,y),face.ec
          );
          triangle(rc,
            PointStruct(i1,0,0,x,h1,y),face.ed,
            PointStruct(i3,1,1,x+1,h3,y+1),face.eb,
            PointStruct(i4,0,1,x,h4,y+1),face.ea
          );
        } else {
          triangle(rc,
            PointStruct(i2,1,0,x+1,h2,y),face.ec,
            PointStruct(i4,0,1,x,h4,y+1),face.ea,
            PointStruct(i1,0,0,x,h1,y),face.ed
          );
          triangle(rc,
            PointStruct(i2,1,0,x+1,h2,y),face.ec,
            PointStruct(i4,0,1,x,h4,y+1),face.ea,
            PointStruct(i3,1,1,x+1,h3,y+1),face.eb
          );
        }

/*
        glColor3f(0.3,mac_vertex(x,y).intensity,0.3);
        glTexCoord2f(0,0);
        glVertex3f(x,hiat(x,y),y);

        glColor3f(0.3,mac_vertex(x+1,y).intensity,0.3);
        glTexCoord2f(1,0);
        glVertex3f(x+1,hiat(x+1,y),y);

        glColor3f(0.3,mac_vertex(x+1,y+1).intensity,0.3);
        glTexCoord2f(1,1);
        glVertex3f(x+1,hiat(x+1,y+1),y+1);

        glColor3f(0.3,mac_vertex(x,y+1).intensity,0.3);
        glTexCoord2f(0,1);
        glVertex3f(x,hiat(x,y+1),y+1);
*/
      }
    }
    glEnd();
//    glDisable(GL_BLEND);
    rc.settexture2d(savet2d);
  } else {
    glBegin(GL_QUADS);
    for(int y=boundsqy1;y<=boundsqy2;y++) {
      for(int x=boundsqx1;x<=boundsqx2;x++) {
//        if (!mac_face(x,y).partvisible) continue;
        if (!mac_face(x,y).octvisible) continue;
        float h1=mac_hiat(x,y),h2=mac_hiat(x+1,y),h3=mac_hiat(x+1,y+1),h4=mac_hiat(x,y+1);
        float i1=mac_vertex(x,y).intensity,i2=mac_vertex(x+1,y).intensity,
              i3=mac_vertex(x+1,y+1).intensity,i4=mac_vertex(x,y+1).intensity;

        glColor3f(0.3,i1,0.3);glVertex3f(x,h1,y);
        glColor3f(0.3,i2,0.3);glVertex3f(x+1,h2,y);
        glColor3f(0.3,i3,0.3);glVertex3f(x+1,h3,y+1);
        glColor3f(0.3,i4,0.3);glVertex3f(x,h4,y+1);

/*        glColor3f(0.3,mac_vertex(x,y).intensity,0.3);
        glVertex3f(x,hiat(x,y),y);

        glColor3f(0.3,mac_vertex(x,y-1).intensity,0.3);
        glVertex3f(x,hiat(x,y-1),y-1);

        glColor3f(0.3,mac_vertex(x-1,y-1).intensity,0.3);
        glVertex3f(x-1,hiat(x-1,y-1),y-1);

        glColor3f(0.3,mac_vertex(x-1,y).intensity,0.3);
        glVertex3f(x-1,hiat(x-1,y),y);*/
      }
    }
    glEnd();
/*    for(int y=1;y<h;y++) {
      glBegin(GL_QUAD_STRIP);
      for(int x=0;x<w;x++) {
        glColor3f(0.3,mac_vertex(x,y).intensity,0.3);
        glVertex3f(x,hiat(x,y),y);
        glTexCoord2f(0,1);

        glColor3f(0.3,mac_vertex(x,y-1).intensity,0.3);
        glVertex3f(x,hiat(x,y-1),y-1);
        glTexCoord2f(0,0);
      }
      glEnd();
    }*/
  }
  if (oglo.heightlines) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDisable(GL_DEPTH_TEST);
    glColor3f(0.1,0.4,0.1);
    glBegin(GL_QUADS);
    for(int y=boundvy1;y<=boundvy2;y++) {
      for(int x=boundvx1;x<=boundvx2;x++) {
        if (!mac_face(x-1,y-1).octvisible) continue;
        glVertex3f(x,mac_hiat(x,y),y);
        glVertex3f(x,mac_hiat(x,y-1),y-1);
        glVertex3f(x-1,mac_hiat(x-1,y-1),y-1);
        glVertex3f(x-1,mac_hiat(x-1,y),y);
      }
    }
    glEnd();
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }

  if (textured) {
    bool savet2d=rc.settexture2d(true);
    gimages->water->bind(rc);
//    glBindTexture(GL_TEXTURE_2D,jtdata.watertexture);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    for(int y=boundsqy1;y<=boundsqy2;y++) {
      for(int x=boundsqx1;x<=boundsqx2;x++) {
//        if (!mac_face(x,y).partvisible) continue;
        if (!mac_face(x,y).octvisible) continue;
        if (!drawwaterface(x,y)) continue;
        float h1=mac_whiat(x,y),h2=mac_whiat(x+1,y),h3=mac_whiat(x+1,y+1),h4=mac_whiat(x,y+1);
        float h=jmax(h1,h2,h3,h4);
        glTexCoord2f(0,0);glVertex3f(x,h,y);
        glTexCoord2f(1,0);glVertex3f(x+1,h,y);
        glTexCoord2f(1,1);glVertex3f(x+1,h,y+1);
        glTexCoord2f(0,1);glVertex3f(x,h,y+1);
      }
    }
    glEnd();
    rc.settexture2d(savet2d);
  }



/*
  glColor4f(0.6,0.6,0.6,1);

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GEQUAL,0.5);

  glBindTexture(GL_TEXTURE_2D,tt_bmps[tt_13].tex[0]);
  glBegin(GL_QUADS);

  tt_bmps[tt_13].draw(0,1,5,0,0,0,0);
  tt_bmps[tt_13].draw(0,1,1,0,0,0,0);

  for(int y=1;y<verteces.h;y++) {
    for(int x=1;x<verteces.w;x++) {
      if (y%3>0) continue;
      if (!mac_face(x-1,y-1).partvisible) continue;
      tt_bmps[tt_13].draw(0,x,y,hiat(x,y),hiat(x,y-1),hiat(x-1,y-1),hiat(x-1,y));
        glVertex3f(x,hiat(x,y),y);
        glTexCoord2f(1,1);
        glVertex3f(x,hiat(x,y-1),y-1);
        glTexCoord2f(1,0);
        glVertex3f(x-1,hiat(x-1,y-1),y-1);
        glTexCoord2f(0,0);
        glVertex3f(x-1,hiat(x-1,y),y);
        glTexCoord2f(0,1);
    }
  }
  glEnd();

  glBindTexture(GL_TEXTURE_2D,tt_bmps[tt_13].tex[1]);
  glBegin(GL_QUADS);
  for(int y=1;y<verteces.h;y++) {
    for(int x=1;x<verteces.w;x++) {
      if (y%3>0) continue;
      if (!mac_face(x-1,y-1).partvisible) continue;
        glVertex3f(x,hiat(x,y),y);
        glTexCoord2f(1,1);
        glVertex3f(x,hiat(x,y-1),y-1);
        glTexCoord2f(1,0);
        glVertex3f(x-1,hiat(x-1,y-1),y-1);
        glTexCoord2f(0,0);
        glVertex3f(x-1,hiat(x-1,y),y);
        glTexCoord2f(0,1);
    }
  }
  glEnd();

  glBindTexture(GL_TEXTURE_2D,tt_bmps[tt_13].tex[2]);
  glBegin(GL_QUADS);
  for(int y=1;y<verteces.h;y++) {
    for(int x=1;x<verteces.w;x++) {
      if (y%3>0) continue;
      if (!mac_face(x-1,y-1).partvisible) continue;
        glVertex3f(x,hiat(x,y),y);
        glTexCoord2f(1,1);
        glVertex3f(x,hiat(x,y-1),y-1);
        glTexCoord2f(1,0);
        glVertex3f(x-1,hiat(x-1,y-1),y-1);
        glTexCoord2f(0,0);
        glVertex3f(x-1,hiat(x-1,y),y);
        glTexCoord2f(0,1);
    }
  }

  glEnd();

  glDisable(GL_TEXTURE_2D);


*/
  curglter=NULL;
}







}

