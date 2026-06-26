#include <jlib.h>
#pragma hdrstop

#include "gview.h"
#include "gameu.h"
#include "ghimap.h"
#include "grrmap.h"
#include <opengl.h>
#include <sdl/sdl.h>
#include "gstatic.h"
#include "gmobile.h"
#include "gimages.h"
#include "gsurmap.h"
#include "gparticle.h"
#include "ghiview.h"
#include "gviewtype.h"

namespace jtyc {

MapCartViewType::MapCartViewType() {
  scale=0.1;
  scalex=scaley=scale;
  leftmove=rightmove=topmove=bottommove=0;
  movevelocity=1;
  dx=dy=0;
  scalechangevelocity=3;
  scrolldist=10;
}

CartViewType::CartViewType() {
  scale=0.01;
  scalex=scaley=scale;
}

MapViewType::MapViewType() {
  seeangle=45;
  viewangle=45;
  viewrotatevelocity=90;
  seerotatevelocity=45;
}


ZPoint CartViewType::ltop(const fpoint3d &pt) {
  return ZPoint(pt.x*scalex-dx,-pt.z*scaley+dy,pt.y/hcoordlimit);
}
void MapCartViewType::updatescalexy() {
  scalex=scaley=scale;
  scalex*=((float)game->view->getvph()/(float)game->view->getvpw());
}

int CartViewType::visiblecube(float coords[3],float size) {
  return 0;
}


void MapViewType::project(float in[3],float out[2]) {
  fpoint a(in[0],in[2]);
  a=rotmat*a;
  out[0]=-dx+scalex*a.x;
  out[1]=dy-scaley*(a.y*fz-in[1]*fy);
}
void ViewType::project(float in[3],float out[2]) {
  ZPoint pt=ltop(fpoint3d(in[0],in[1],in[2]));
  out[0]=pt.x;
  out[1]=pt.y;
}

void MapCartViewType::resized() {
  float sx=(float)scrolldist/game->view->pixw*2;
  float sy=(float)scrolldist/game->view->pixh*2;
  leftmove=-1+sx;rightmove=1-sx;
  topmove=-1+sy;bottommove=1-sy;
  changedtr();
  updatescalexy();
}

ZPoint MapViewType::ltop(const fpoint3d &pt) {
  fpoint a(pt.x,pt.z);
  a=rotmat*a;
  return ZPoint(-dx+scalex*a.x,dy-scaley*(a.y*fz-pt.y*fy),scale*pt.y);
}

void MapViewType::changedtr() {
  rotmat.dorotate(viewangle/180*M_PI);
  rotinvmat.dorotate(-viewangle/180*M_PI);
  fz=sin(seeangle/180*M_PI);
  fy=cos(seeangle/180*M_PI);
}

void MapCartViewType::iterate(unsigned char *keystate,double deltatime) {
  bool moved=false;
  bool scaled=false;

  float oldscalex=scalex,oldscaley=scaley;
  if (!game->scroll_block) {
    if (game->view->scrmousepx<leftmove) dx-=movevelocity*deltatime,moved=true;
    if (game->view->scrmousepy>bottommove) dy-=movevelocity*deltatime,moved=true;
    if (game->view->scrmousepx>rightmove) dx+=movevelocity*deltatime,moved=true;
    if (game->view->scrmousepy<topmove) dy+=movevelocity*deltatime,moved=true;
  }
  if (!game->keys_block) {
    if (keystate[SDLK_KP_PLUS]) scale*=pow(scalechangevelocity,deltatime),scaled=true;
    if (keystate[SDLK_KP_MINUS]) scale/=pow(scalechangevelocity,deltatime),scaled=true;
  }
  if (scale<minscale()) scale=minscale();
  if (scale>maxscale()) scale=maxscale();
  updatescalexy();
  dx*=scalex/oldscalex;
  dy*=scaley/oldscaley;

  changedtr();
//  if (scaled) resized();
//  int flags=0;
//  if (rotated) flags|=CF_ROTATE;
//  if (moved) flags|=CF_MOVE;
//  if (seeangled) flags|=CF_SEEANGLE;
//  if (scaled) flags|=CF_SCALE;

//  game->himap->recount(flags);
//  if (game->himap->checkdxdy()) game->himap->recount(CF_MOVE);
//  game->rrmap->countvisible();

//  lastchjp+=deltatime;
//  if (lastchjp>3) jper=!jper,lastchjp=0;

}


void MapViewType::iterate(unsigned char *keystate,double deltatime) {
  MapCartViewType::iterate(keystate,deltatime);
  bool rotated=false;
  bool seeangled=false;
  float oldviewangle=viewangle;
  float oldseeangle=seeangle;
  if (!game->keys_block) {
    if (keystate[SDLK_HOME]) viewangle+=viewrotatevelocity*deltatime,rotated=true;
    if (keystate[SDLK_END]) viewangle-=viewrotatevelocity*deltatime,rotated=true;
    if (keystate[SDLK_PAGEUP]) seeangle+=seerotatevelocity*deltatime,seeangled=true;
    if (keystate[SDLK_PAGEDOWN]) seeangle-=seerotatevelocity*deltatime,seeangled=true;
  }

  fvector3d pos;
  if (game->iswatch(&pos)) {
    exchange(pos.y,pos.z);
    ZPoint pt=ltop(pos);
    pt.x+=dx;pt.y-=dy;
    dx=pt.x;dy=-pt.y;
  }

  {
    agmatrix<float> rot;
    rot.dorotate((viewangle-oldviewangle)/180*M_PI);
    fpoint pt=rot*fpoint(dx/scalex,dy/fz/scaley);
    dx=pt.x*scalex;dy=pt.y*fz*scaley;
  }

  while (viewangle>360) viewangle-=360;
  while (viewangle<0) viewangle+=360;
  if (seeangle<10) seeangle=10;
  if (seeangle>90) seeangle=90;

  {
    float fz1=sin(oldseeangle/180*M_PI),fz2=sin(seeangle/180*M_PI);
    dy-=dy*(fz1-fz2);
  }

  changedtr();
//  if (scaled)
  resized();

  game->himap->checkdxdy();

  if (rotated) game->setinfo(lang_text_ch("$View-angle:")+itos(round(viewangle)));
  if (seeangled) game->setinfo(lang_text_ch("$See-angle:")+itos(round(seeangle)));

/*
  bool rotated=false;
  bool moved=false;
  bool seeangled=false;
  bool scaled=false;

  float oldscalex=scalex,oldscaley=scaley;
  float oldviewangle=viewangle;
  float oldseeangle=seeangle;
  if (!game->scroll_block) {
    if (game->view->scrmousepx<leftmove) dx-=movevelocity*deltatime,moved=true;
    if (game->view->scrmousepy>bottommove) dy-=movevelocity*deltatime,moved=true;
    if (game->view->scrmousepx>rightmove) dx+=movevelocity*deltatime,moved=true;
    if (game->view->scrmousepy<topmove) dy+=movevelocity*deltatime,moved=true;
  }
  if (!game->keys_block) {
    if (keystate[SDLK_HOME]) viewangle+=viewrotatevelocity*deltatime,rotated=true;
    if (keystate[SDLK_END]) viewangle-=viewrotatevelocity*deltatime,rotated=true;
    if (keystate[SDLK_PAGEUP]) seeangle+=seerotatevelocity*deltatime,seeangled=true;
    if (keystate[SDLK_PAGEDOWN]) seeangle-=seerotatevelocity*deltatime,seeangled=true;
    if (keystate[SDLK_KP_PLUS]) scale*=pow(scalechangevelocity,deltatime),scaled=true;
    if (keystate[SDLK_KP_MINUS]) scale/=pow(scalechangevelocity,deltatime),scaled=true;
  }
  if (scale<0.01) scale=0.01;
  if (scale>1) scale=1;
  updatescalexy();
  dx*=scalex/oldscalex;
  dy*=scaley/oldscaley;

  fvector3d pos;
  if (game->iswatch(&pos)) {
    exchange(pos.y,pos.z);
    ZPoint pt=ltop(pos);
    pt.x+=dx;pt.y-=dy;
    dx=pt.x;dy=-pt.y;
    moved=true;
  }

  {
    agmatrix<float> rot;
    rot.dorotate((viewangle-oldviewangle)/180*M_PI);
    fpoint pt=rot*fpoint(dx/scalex,dy/fz/scaley);
    dx=pt.x*scalex;dy=pt.y*fz*scaley;
  }

  while (viewangle>360) viewangle-=360;
  while (viewangle<0) viewangle+=360;
  if (seeangle<20) seeangle=20;
  if (seeangle>90) seeangle=90;

  {
    float fz1=sin(oldseeangle/180*M_PI),fz2=sin(seeangle/180*M_PI);
    dy-=dy*(fz1-fz2);
  }

  changedtr();
  if (scaled) resized();
  int flags=0;
  if (rotated) flags|=CF_ROTATE;
  if (moved) flags|=CF_MOVE;
  if (seeangled) flags|=CF_SEEANGLE;
  if (scaled) flags|=CF_SCALE;

  game->himap->checkdxdy();
//  game->himap->recount(flags);
//  if (game->himap->checkdxdy()) game->himap->recount(CF_MOVE);
//  game->rrmap->countvisible();

//  lastchjp+=deltatime;
//  if (lastchjp>3) jper=!jper,lastchjp=0;

  if (rotated) game->setinfo("Rotace:"+itos(round(viewangle)));
  if (seeangled) game->setinfo("Uhel pohledu:"+itos(round(seeangle)));
*/
}


bool MapViewType::countprojmatrix() {
  return true;
}

bool MapViewType::applyprojmatrix() {
  glDepthRange(1,0);
  float viewmatrix[16]={1,0,0,0,
                  0,cos(seeangle/180*M_PI),1,0,
                  0,-sin(seeangle/180*M_PI),0,0,
                  0,0,0,1};
  glTranslatef(-dx,dy,0);
  glScalef(scalex,scaley,1/hcoordlimit);
  glMultMatrixf(viewmatrix);
  glRotatef(viewangle,0,1,0);
  return true;
}

void MapCartViewType::clear() {
  glClearColor(0.1,0.4,0.1,1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

#define mac_vertex(_x,_y) game->himap->verteces(_x,_y)

void sethicolor(float hi) {
  using namespace mapcolors;
  if (hi<h1) {
    float k=hi/h1;
    glColor3f((r1-r0)*k+r0,(g1-g0)*k+g0,(b1-b0)*k+b0);
  } else {
    float k=(hi-h1)/(1-h1);
    glColor3f((r2-r1)*k+r1,(g2-g1)*k+g1,(b2-b1)*k+b1);
  }
}

void CartViewType::iterate(unsigned char *keystate,double deltatime) {
  MapCartViewType::iterate(keystate,deltatime);
  int w=game->himap->verteces.w-1,h=game->himap->verteces.h-1;
  if (w*scalex<2) dx=w*scalex/2;
  else {
    if (dx<1) dx=1;
    if (dx>-1+w*scalex) dx=-1+w*scalex;
  }
/*
  if (h*scaley<2) dy=h*scaley/2;
  else {
    if (dy>-1) dy=-1;
    if (dy<1-h*scaley) dy=1-h*scaley;
  }
*/
  if (h*scaley<2) dy=h*scaley/2;
  else {
    if (dy<1) dy=1;
    if (dy>-1+h*scaley) dy=-1+h*scaley;
  }


  game->view->mousesqx=(game->view->vpmousepx+dx)/scalex;
  game->view->mousesqy=-(game->view->vpmousepy-dy)/scaley;
  game->view->mousevx=(game->view->vpmousepx+dx)/scalex+0.5;
  game->view->mousevy=-(game->view->vpmousepy-dy)/scaley+0.5;

  if (game->view->mousevx<0) game->view->mousevx=0;
  if (game->view->mousevy<0) game->view->mousevy=0;
  if (game->view->mousevx>w) game->view->mousevx=w;
  if (game->view->mousevy>h) game->view->mousevy=h;

  if (game->view->mousesqx<0) game->view->mousesqx=0;
  if (game->view->mousesqy<0) game->view->mousesqy=0;
  if (game->view->mousesqx>=w) game->view->mousesqx=w-1;
  if (game->view->mousesqy>=h) game->view->mousesqy=h-1;

  float ddx=(game->view->vpmousepx+dx)/scalex-game->view->mousesqx;
  float ddy=-(game->view->vpmousepy-dy)/scaley-game->view->mousesqy;

  game->view->mouseisinmiddle=0.5-ddx-ddy<0 && ddx-1.5-ddy<0 && ddx+0.5-ddy>0 && 1.5-ddx-ddy>0;
}

void CartViewType::dorender(RenderContext &rc) {
  rc.push();
//  glDepthFunc(GL_GEQUAL);
  rc.setdepthtest(false);
  rc.setsmoothshade(true);

  int w=game->himap->verteces.w,h=game->himap->verteces.h;

//  int minx=(-1+dx)/scalex-1,maxx=(1+dx)/scalex+1;
//  int miny=(-1-dy)/scaley-1,maxy=(1-dy)/scaley+1;

  int minx=(-1+dx)/scalex-1,maxx=(1+dx)/scalex+1;
  int miny=(-1+dy)/scaley-1,maxy=(1+dy)/scaley+1;

  if (minx<0) minx=0;if (miny<0) miny=0;
  if (maxx>=w) maxx=w-1;if (maxy>=h) maxy=h-1;

  int dx=(maxx-minx)/128.0,dy=(maxy-miny)/128.0;
  if (dx<1) dx=1;if (dy<1) dy=1;

  for(int y=miny+dy;y<=maxy;y+=dy) {
    glBegin(GL_QUAD_STRIP);
    for(int x=minx;x<=maxx;x+=dx) {
      sethicolor(mac_vertex(x,y).hi/maxmaphi);
      glVertex3f(x,mac_vertex(x,y).hi,y);

      sethicolor(mac_vertex(x,y-dy).hi/maxmaphi);
      glVertex3f(x,mac_vertex(x,y-dy).hi,y-dy);
    }
    glEnd();
  }

  rc.setdepthtest(true);
  rc.setsmoothshade(false);

  glBegin(GL_QUADS);
  for(int y=miny;y<=maxy;y+=dy) {
    for(int x=minx;x<=maxx;x+=dx) {
      float whi=mac_vertex(x,y).waterhi;
      if (whi>=0) {
        glColor3f(0,0,1);
        glVertex3f(x,whi,y);
        glVertex3f(x+dx,whi,y);
        glVertex3f(x+dx,whi,y+dy);
        glVertex3f(x,whi,y+dy);
      }
    }
  }
  glEnd();

  rc.setdepthtest(false);
  rc.setblending(false);

  game->rrmap->drawmap(minx,miny,maxx,maxy);
/*
  rc.setdepthtest(false);
  rc.setblending(false);

  glColor4f(0,0,0,1);
  glBegin(GL_LINES);
  for(RItem *ri=game->rrmap->ri_list;ri;ri=ri->next_ri) {
    if (ri->x<minx || ri->x>=maxx || ri->y<miny || ri->y>=maxy) continue;
    for(int i=1;i<=tt_cnt;i++) {
      Spojnice tt(i);
      if (!(ri->flags&tt.flag())) continue;
      glVertex3f(ri->x+0.5+0.5*tt.beg().dx(),0,ri->y+0.5+0.5*tt.beg().dy());
      glVertex3f(ri->x+0.5+0.5*tt.end().dx(),0,ri->y+0.5+0.5*tt.end().dy());
    }
  }
  glEnd();
*/
  rc.pop();
}

bool CartViewType::countprojmatrix() {
  return true;
}
bool CartViewType::applyprojmatrix() {
  float m[16]={1,0,0,0,
               0,0,1,0,
               0,1,0,0,
               0,0,0,1};

  glDepthRange(0,1);
  glTranslatef(-dx,dy,0);
  glScalef(scalex,scaley,1/hcoordlimit);
  glMultMatrixf(m);
  glScalef(1,1,-1);
  return true;
}

}


