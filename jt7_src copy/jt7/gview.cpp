#include <jlib.h>
#pragma hdrstop

#include "gview.h"
#include "gameu.h"
#include "ghimap.h"
#include "grrmap.h"
#include "groad.h"
#include <opengl.h>
#include <sdl/sdl.h>
#include "gstatic.h"
#include "gmobile.h"
#include "gimages.h"
#include "gsurmap.h"
#include "gparticle.h"
#include "ghiview.h"
#include "icab.h"

namespace jtyc {

const double keyinterval=0.2;
const float fpsinterval=3;
bool showfps=false;


View::View() {
  gocttree=ot=new OctTree();
  lastkeytime=0;
  mouseisinmiddle=false;
  ptcsize2x=ptcsize2y=0;
  pixw=0;
  pixh=0;
  scrmousepx=scrmousepy=0;
  vpmousepx=vpmousepy=0;
  mousesqx=mousesqy=0;
  lastmousesqx=lastmousesqy=0;
  direction=0;
  speedc=0;
  cab=NULL;
  type=&maptype;
  customrc=NULL;
}

View::~View() {
  delete ot;
  gocttree=NULL;
}

void View::setcab(jtCab *value) {
  cab=value;
  type->resized();
}


int View::getvpx() {
  if (cab) return pixw*cab->x;
  return 0;
}
int View::getvpy() {
  if (cab) return pixh*cab->y;
  return 0;
}
int View::getvpw() {
  if (cab) return pixw*cab->w;
  return pixw;
}
int View::getvph() {
  if (cab) return pixh*cab->h;
  return pixh;
}

void View::init() {
  ot->init(1,jmax(game->W,100,game->H));
}

bool View::isineq(const ZPoint &a,const ZPoint &b,const ZPoint &f,const ZPoint &g) {
  //vrati, zda f a g jsou ve stejne polorovine orcene primkou a,b(vse v 2D)
  float dx=b.x-a.x,dy=b.y-a.y;
  float c=a.y*dx-a.x*dy;
  float fp=f.x*dy-f.y*dx+c;
  float gp=g.x*dy-g.y*dx+c;
  return fp*gp>=0;
}
bool View::mousein(const ZPoint &a,const ZPoint &b,const ZPoint &c,const ZPoint &d) {
  ZPoint s((a.x+b.x+c.x+d.x)/4,(a.y+b.y+c.y+d.y)/4,0);
  ZPoint m(vpmousepx,vpmousepy,0);
  return isineq(a,b,s,m) && isineq(b,c,s,m) && isineq(c,d,s,m) && isineq(d,a,s,m)
  && isineq(a,b,c,d) && isineq(b,c,d,a) && isineq(c,d,a,b) && isineq(d,a,b,c);//zda je ctverecek "nepokrouceny"(viditelny)
}

void View::resize(int w,int h) {
  pixw=w;pixh=h;
  resized();
//  game->himap->recount(~0);
}

void View::resized() {
  ptcsize2x=(float)beop.ptcursorsize2/pixw*2;
  ptcsize2y=(float)beop.ptcursorsize2/pixh*2;
  type->resized();
}

bool View::visiblesq(int x,int y) {
//  return game->himap->vertexat(x,y).visible || game->himap->vertexat(x+1,y).visible ||
//         game->himap->vertexat(x,y+1).visible || game->himap->vertexat(x+1,y+1).visible;
//  return game->himap->faceat(x,y).partvisible;
  return game->himap->faceat(x,y).octvisible;
}

void View::getmousesq(int x0,int y0) {
  mouseisinmiddle=false;
  mousesqx=x0,mousesqy=y0;
  for(int x=game->himap->boundsqx1;x<=game->himap->boundsqx2;x++) {
    for(int y=game->himap->boundsqy1;y<=game->himap->boundsqy2;y++) {
      if (visiblesq(x,y)) {
        ZPoint a=game->hiview->vat(x,y).scrpt;
        ZPoint b=game->hiview->vat(x+1,y).scrpt;
        ZPoint c=game->hiview->vat(x+1,y+1).scrpt;
        ZPoint d=game->hiview->vat(x,y+1).scrpt;
        if (mousein(a,b,c,d)) {
          mousesqx=x;mousesqy=y;
          if (mousein((a+b)/2,(b+c)/2,(c+d)/2,(d+a)/2)) mouseisinmiddle=true;
        }
      }
    }
  }
}
void View::getmousev(int x0,int y0) {
  float mindist=1000000;
  mousevx=x0,mousevy=y0;
  int xmin=game->himap->boundvx1,xmax=game->himap->boundvx2;
  int ymin=game->himap->boundvy1,ymax=game->himap->boundvy2;
  for(int x=xmin;x<=xmax;x++) {
    for(int y=ymin;y<=ymax;y++) {
      HVVertex &vr=game->hiview->vat(x,y);
//      if (!vr.visible) continue;
      float dx=vr.scrpt.x-vpmousepx,dy=vr.scrpt.y-vpmousepy;
      float dist=sqrt(dx*dx+dy*dy);
      if (dist<mindist) mindist=dist,mousevx=x,mousevy=y;
    }
  }
}

fpoint3d View::sget(int x,int y) {
  return game->himap->getmax(x,y);
}

static fpoint3d mixpoint(const fpoint3d &a,const fpoint3d &b,const fpoint3d &c,const fpoint3d &d,float dx,float dy) {
  fvector3d dy1=d-a,dy2=c-b;
  fpoint3d e=a+dy1*dy,f=b+dy2*dy;
  fvector3d dxx=f-e;
  return e+dxx*dx;
}

void View::drawsqcursor(int x,int y,string type,int dir) {
  fpoint3d pa,pb,pc,pd;
  bool glbegin=false;
  switch (dir) {
    case 0:pa=sget(x,y),pb=sget(x+1,y),pc=sget(x+1,y+1),pd=sget(x,y+1);break;
    case 1:pa=sget(x+1,y),pb=sget(x+1,y+1),pc=sget(x,y+1),pd=sget(x,y);break;
    case 2:pa=sget(x+1,y+1),pb=sget(x,y+1),pc=sget(x,y),pd=sget(x+1,y);break;
    case 3:pa=sget(x,y+1),pb=sget(x,y),pc=sget(x+1,y),pd=sget(x+1,y+1);break;
  }
//  fpoint3d pa=sget(x,y),pb=sget(x+1,y),pc=sget(x+1,y+1),pd=sget(x,y+1);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  #define readpoint(var) \
    if (c[0]<'1' || c[0]>'9') goto end;\
    if (c[1]<'1' || c[1]>'9') goto end;\
    var=mixpoint(pa,pb,pc,pd,(c[0]-'1')/8.0,(c[1]-'1')/8.0);\
    c+=2;
  #define flushcolor \
    if (changedcolor) glColor4f(r,g,b,a);changedcolor=false;
  float r=1,g=1,b=1,a=1;
  bool changedcolor=true;
  for(const char *c=type.c_str();*c;) {
    switch (*c) {
      #define readcolor(name,chr) \
      case chr:{\
        c++;\
        if ((*c<'0' || *c>'9') && *c!='x') goto end;\
        if (*c=='x') name=1;else name=(*c-'0')/10.0;\
        changedcolor=true;\
        c++;\
      } break;
      readcolor(r,'r')
      readcolor(g,'g')
      readcolor(b,'b')
      readcolor(a,'a')
      case 'L':{
        c++;
        fpoint3d p1,p2;
        readpoint(p1)
        readpoint(p2)
        if (*c=='.') c++;
        flushcolor
        glBegin(GL_LINE_STRIP);
        glVertex3f(p1.x,p1.y,p1.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glEnd();
      } break;
      case 't':{
        c++;
        fpoint3d p1,p2,p3;
        readpoint(p1)
        readpoint(p2)
        readpoint(p3)
        if (*c=='.') c++;
        flushcolor
        glBegin(GL_LINE_STRIP);
        glVertex3f(p1.x,p1.y,p1.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glVertex3f(p3.x,p3.y,p3.z);
        glVertex3f(p1.x,p1.y,p1.z);
        glEnd();
      } break;
      case 'q':{
        c++;
        fpoint3d p1,p2,p3,p4;
        readpoint(p1)
        readpoint(p2)
        readpoint(p3)
        readpoint(p4)
        if (*c=='.') c++;
        flushcolor
        glBegin(GL_LINE_STRIP);
        glVertex3f(p1.x,p1.y,p1.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glVertex3f(p3.x,p3.y,p3.z);
        glVertex3f(p4.x,p4.y,p4.z);
        glVertex3f(p1.x,p1.y,p1.z);
        glEnd();
      } break;
      case 'T':{
        c++;
        fpoint3d p1,p2,p3;
        readpoint(p1)
        readpoint(p2)
        readpoint(p3)
        if (*c=='.') c++;
        flushcolor
        glBegin(GL_TRIANGLES);
        glVertex3f(p1.x,p1.y,p1.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glVertex3f(p3.x,p3.y,p3.z);
        glEnd();
      } break;
      case 'Q':{
        c++;
        fpoint3d p1,p2,p3,p4;
        readpoint(p1)
        readpoint(p2)
        readpoint(p3)
        readpoint(p4)
        if (*c=='.') c++;
        flushcolor
        glBegin(GL_QUADS);
        glVertex3f(p1.x,p1.y,p1.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glVertex3f(p3.x,p3.y,p3.z);
        glVertex3f(p4.x,p4.y,p4.z);
        glEnd();
      } break;
      case 'p':{
        c++;
        fpoint3d p1,pt;
        flushcolor
        glBegin(GL_LINE_STRIP);
        glbegin=true;
        readpoint(p1)
        glVertex3f(p1.x,p1.y,p1.z);
        while (*c && *c!='.') {
          readpoint(pt)
          glVertex3f(pt.x,pt.y,pt.z);
        }
        glVertex3f(p1.x,p1.y,p1.z);
        if (*c=='.') c++;
        glEnd();
        glbegin=false;
      } break;
      case 'P':{
        c++;
        fpoint3d pt;
        flushcolor
        glBegin(GL_POLYGON);
        glbegin=true;
        while (*c && *c!='.') {
          readpoint(pt)
          glVertex3f(pt.x,pt.y,pt.z);
        }
        if (*c=='.') c++;
        glEnd();
        glbegin=false;
      } break;
      default:goto end;break;
    }
  }
  end:
  if (glbegin) glEnd();
  glColor4f(1,1,1,1);
  #undef readpoint
  #undef readcolor
  #undef flushcolor
}

void View::drawsqcursor(int x,int y,gcolor color) {
  fpoint3d a=sget(x,y),b=sget(x+1,y),c=sget(x+1,y+1),d=sget(x,y+1);
//  ZPoint za=ltop(a),zb=ltop(b),zc=ltop(c),zd=ltop(d);
  glColor3f(color.r,color.g,color.b);
  glBegin(GL_LINE_STRIP);
/*  glVertex2f(za.x,za.y);
  glVertex2f(zb.x,zb.y);
  glVertex2f(zc.x,zc.y);
  glVertex2f(zd.x,zd.y);*/
  glVertex3f(a.x,a.y,a.z);
  glVertex3f(b.x,b.y,b.z);
  glVertex3f(c.x,c.y,c.z);
  glVertex3f(d.x,d.y,d.z);
  glVertex3f(a.x,a.y,a.z);
  glEnd();
//  za.z=cursor_z;zb.z=cursor_z;zc.z=cursor_z;zd.z=cursor_z;
//  zbmp->line(za,zb,color);zbmp->line(zb,zc,color);zbmp->line(zc,zd,color);zbmp->line(zd,za,color);
}
void View::drawsq2cursor(int x,int y,gcolor color) {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  fpoint3d a=sget(x,y),b=sget(x+1,y),c=sget(x+1,y+1),d=sget(x,y+1);
  ZPoint za=ltop(a),zb=ltop(b),zc=ltop(c),zd=ltop(d);
  ZPoint wa=(za+zb)/2,wb=(zb+zc)/2,wc=(zc+zd)/2,wd=(zd+za)/2;
  glColor3f(color.r,color.g,color.b);
  glBegin(GL_LINE_STRIP);
  glVertex2f(za.x,za.y);
  glVertex2f(zb.x,zb.y);
  glVertex2f(zc.x,zc.y);
  glVertex2f(zd.x,zd.y);
  glVertex2f(za.x,za.y);
  glEnd();
  glVertex2f(wa.x,wa.y);
  glVertex2f(wb.x,wb.y);
  glVertex2f(wc.x,wc.y);
  glVertex2f(wd.x,wd.y);
  glVertex2f(wa.x,wa.y);
  glEnd();
  glPopMatrix();
}
void View::drawsqlinecursor(int x,int y,bool vertical,gcolor framecolor,gcolor linecolor) {
  fpoint3d a=sget(x,y),b=sget(x+1,y),c=sget(x+1,y+1),d=sget(x,y+1);

  glColor3f(framecolor.r,framecolor.g,framecolor.b);
  glBegin(GL_LINE_STRIP);
  glVertex3f(a.x,a.y,a.z);
  glVertex3f(b.x,b.y,b.z);
  glVertex3f(c.x,c.y,c.z);
  glVertex3f(d.x,d.y,d.z);
  glVertex3f(a.x,a.y,a.z);
  glEnd();

  fpoint3d f,g;
  if (vertical) f=(fvector3d)(a+b)/2,g=(fvector3d)(c+d)/2;else f=(fvector3d)(a+d)/2,g=(fvector3d)(b+c)/2;
  glColor3f(linecolor.r,linecolor.g,linecolor.b);
  glBegin(GL_LINES);
  glVertex3f(f.x,f.y,f.z);
  glVertex3f(g.x,g.y,g.z);
  glEnd();
}
void View::drawptcursor(int x,int y,gcolor color) {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  fpoint3d a=sget(x,y);
  ZPoint za=ltop(a);
  glColor4f(color.r,color.g,color.b,1);
  glBegin(GL_LINE_STRIP);
  glVertex2f(za.x-ptcsize2x,za.y-ptcsize2y);
  glVertex2f(za.x+ptcsize2x,za.y-ptcsize2y);
  glVertex2f(za.x+ptcsize2x,za.y+ptcsize2y);
  glVertex2f(za.x-ptcsize2x,za.y+ptcsize2y);
  glVertex2f(za.x-ptcsize2x,za.y-ptcsize2y);
  glEnd();
  glPopMatrix();
}
void View::drawptcursors(int x1,int y1,int x2,int y2,gcolor color) {
  const int cursorlimit=1000;
  if ((x2-x1)*(y2-y1)<cursorlimit) for(int y=y1;y<=y2;y++) for(int x=x1;x<=x2;x++) drawptcursor(x,y,color);
  else {
    for(int x=x1;x<=x2;x++) drawptcursor(x,y1,color),drawptcursor(x,y2,color);
    for(int y=y1+1;y<=y2;y++) drawptcursor(x1,y,color),drawptcursor(x2,y,color);
  }
}
void View::drawsqcursors(int x1,int y1,int x2,int y2,string type,int dir) {
  const int cursorlimit=1000;
  if ((x2-x1)*(y2-y1)<cursorlimit) for(int y=y1;y<=y2;y++) for(int x=x1;x<=x2;x++) drawsqcursor(x,y,type,dir);
  else {
    for(int x=x1;x<=x2;x++) drawsqcursor(x,y1,type,dir),drawsqcursor(x,y2,type,dir);
    for(int y=y1+1;y<=y2;y++) drawsqcursor(x1,y,type,dir),drawsqcursor(x2,y,type,dir);
  }
}

void View::drawcircle(float x,float y,float xrad,float yrad,gcolor color) {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glColor3f(color.r,color.g,color.b);
  glBegin(GL_LINE_STRIP);

  int quality=50;
  float dt=2*M_PI/quality;
  float t=0;

  for(int i=0;i<=quality;i++,t+=dt) {
    glVertex2f(x+xrad*cos(t),y+yrad*sin(t));
  }

  glEnd();
  glPopMatrix();
}
void View::drawmapcircle(float x,float y,float xrad,float yrad,gcolor color) {
  glColor3f(color.r,color.g,color.b);
  glBegin(GL_LINE_STRIP);

  int quality=50;
  float dt=2*M_PI/quality;
  float t=0;

  for(int i=0;i<=quality;i++,t+=dt) {
    float cx=x+xrad*cos(t),cy=y+yrad*sin(t);
    float h=game->himap->interpolatehi(cx,cy);
    glVertex3f(cx,h,cy);
  }

  glEnd();
}

void View::keyup(int key) {
  if (key==SDLK_UP || key==SDLK_DOWN) lastkeytime=0;
}
void View::keydown(int key) {
  if (key==SDLK_F11) showfps=!showfps;
  if (Mobile *mob=game->isdrive()) {
    if (key==SDLK_LEFT) direction--;
    if (key==SDLK_RIGHT) direction++;
    if (key==SDLK_UP) speedc++,lastkeytime=0;
    if (key==SDLK_DOWN) speedc--,lastkeytime=0;
    if (direction<-2) direction=-2;
    if (direction>2) direction=2;
    if (speedc<-5) speedc=-5;
    if (speedc>5) speedc=5;
    mob->drivekeydown(key);
  }
}

void View::settype(string value) {
  if (value=="map") type=&maptype;
  if (value=="cart") type=&carttype;
  type->resized();
}

void View::iterate(double deltatime,int mx,int my) {

  int st0=clock();

  type->countprojmatrix();

  if (type->stdrender()) {
    game->himap->octalize();
    game->surmap->octalize();
    game->statics->octalize();

    game->mobiles->octalize();

    visibleitems.clear();
    ot->getvisible(visibleitems,*type);

    game->himap->clearvisibility();
    game->surmap->clearvisibility();

    game->mobiles->clearvisibility();
    game->statics->clearvisibility();

    int tim0=clock()-st0;
    float ft0=tim0/(float)CLK_TCK;


    for(int i=0;i<visibleitems.count;i++) ((GOTObject*)visibleitems[i].obj)->ot_render(visibleitems[i].id);
    game->himap->repairbounds();
    game->hiview->beforeiterate();
    for(int i=0;i<visibleitems.count;i++) ((GOTObject*)visibleitems[i].obj)->markhiview();

    gocttree->cleardynamics();
  }
  scrmousepx=mx/(float)pixw*2-1;
  scrmousepy=1-my/(float)pixh*2;
  if (cab) {
    int x=cab->x*pixw,y=cab->y*pixh,w=cab->w*pixw,h=cab->h*pixh;
    vpmousepx=(mx-x)/(float)w;
    vpmousepx*=2;vpmousepx-=1;
    vpmousepy=(my-y)/(float)h;
    vpmousepy*=-2;vpmousepy+=1;
  } else {
    vpmousepx=scrmousepx;
    vpmousepy=scrmousepy;
  }
  unsigned char *keystate=SDL_GetKeyState(NULL);
  type->iterate(keystate,deltatime);

  if (!game->keys_block) {
    if (game->isdrive()) {
      if (keystate[SDLK_UP]) {
        lastkeytime+=deltatime;
        if (lastkeytime>keyinterval) speedc++,lastkeytime=0;
      }
      if (keystate[SDLK_DOWN]) {
        lastkeytime+=deltatime;
        if (lastkeytime>keyinterval) speedc--,lastkeytime=0;
      }
      if (speedc<-5) speedc=-5;
      if (speedc>5) speedc=5;
    }
  }
  if (type->stdrender()) {
    getmousesq(lastmousesqx,lastmousesqy);
    lastmousesqx=mousesqx;lastmousesqy=mousesqy;
    getmousev(0,0);
  }
}

/*
bool View::nearvisible(const ZPoint &pt,float maxdist) {
  maxdist*=scale;
  maxdist*=2;
  return pt.x>=-1-maxdist && pt.y>=-1-maxdist && pt.x<1+maxdist && pt.y<1+maxdist;
}
*/

void View::xmlsave(XMLNode xml) {
  xml.setvar("seeangle",maptype.seeangle);
  xml.setvar("viewangle",maptype.viewangle);
  xml.setvar("dx",maptype.dx);
  xml.setvar("dy",maptype.dy);
  xml.setvar("scale",maptype.scale);

  xml.setvar("cart.dx",carttype.dx);
  xml.setvar("cart.dy",carttype.dy);
  xml.setvar("cart.scale",carttype.scale);
}

void View::lookat(fpoint3d pt) {
  if (type==&maptype) {
    ZPoint zpt=ltop(pt);
    maptype.dx+=zpt.x;
    maptype.dy-=zpt.y;
  }
  if (type==&carttype) {
    ZPoint zpt=ltop(pt);
    carttype.dx+=zpt.x;
    carttype.dy-=zpt.y;
  }
}

void View::xmlload(XMLNode xml) {
  maptype.seeangle=xml.fvar("seeangle");
  maptype.viewangle=xml.fvar("viewangle");
  maptype.dx=xml.fvar("dx");
  maptype.dy=xml.fvar("dy");
  maptype.scale=xml.fvar("scale");
  if (maptype.scale<maptype.minscale()) maptype.scale-maptype.minscale();

  carttype.dx=xml.fvar("cart.dx");
  carttype.dy=xml.fvar("cart.dy");
  carttype.scale=xml.fvar("cart.scale");
  if (carttype.scale<carttype.minscale()) carttype.scale=carttype.minscale();
}

void View::load(Archive &ar) {
  if (ar.findfile("view.xml")<0) return;
  MainXMLNode xml;
  ar.loadxml("view.xml",xml);
  xmlload(xml);
}

void View::save(Archive &ar) {
  MainXMLNode xml;
  xml.setname("view");
  xmlsave(xml);
  ar.savexml("view.xml",xml);
}

void ProjectiveMatrix::rotate(float angle,float x,float y,float z) {
  float s=sin(angle/180*M_PI),c=cos(angle/180*M_PI);
  if (x==1 && y==0 && z==0) {
    p[0][0]=1;p[0][1]=0;p[0][2]=0;p[0][3]=0;
    p[1][0]=0;p[1][1]=c;p[1][2]=-s;p[1][3]=0;
    p[2][0]=0;p[2][1]=s;p[2][2]=c;p[2][3]=0;
    p[3][0]=0;p[3][1]=0;p[3][2]=0;p[3][3]=1;
  } else if (x==0 && y==1 && z==0) {
    p[0][0]=c;p[0][1]=0;p[0][2]=s;p[0][3]=0;
    p[1][0]=0;p[1][1]=1;p[1][2]=0;p[1][3]=0;
    p[2][0]=-s;p[2][1]=0;p[2][2]=c;p[2][3]=0;
    p[3][0]=0;p[3][1]=0;p[3][2]=0;p[3][3]=1;
  } else if (x==0 && y==0 && z==1) {
    p[0][0]=c;p[0][1]=-s;p[0][2]=0;p[0][3]=0;
    p[1][0]=s;p[1][1]=c;p[1][2]=0;p[1][3]=0;
    p[2][0]=0;p[2][1]=0;p[2][2]=1;p[2][3]=0;
    p[3][0]=0;p[3][1]=0;p[3][2]=0;p[3][3]=1;
  } else throw JError("cannot rotate");
}
void ProjectiveMatrix::multiply(ProjectiveMatrix &a,const ProjectiveMatrix &b) {
  #define mul(i,j) p[i][j]=a.p[i][0]*b.p[0][j]+a.p[i][1]*b.p[1][j]+a.p[i][2]*b.p[2][j]+a.p[i][3]*b.p[3][j]
  mul(0,0);mul(0,1);mul(0,2);mul(0,3);
  mul(1,0);mul(1,1);mul(1,2);mul(1,3);
  mul(2,0);mul(2,1);mul(2,2);mul(2,3);
  mul(3,0);mul(3,1);mul(3,2);mul(3,3);
  #undef mul
}
void ProjectiveMatrix::identity() {
  p[0][0]=1;p[0][1]=0;p[0][2]=0;p[0][3]=0;
  p[1][0]=0;p[1][1]=1;p[1][2]=0;p[1][3]=0;
  p[2][0]=0;p[2][1]=0;p[2][2]=1;p[2][3]=0;
  p[3][0]=0;p[3][1]=0;p[3][2]=0;p[3][3]=1;
}
void ProjectiveMatrix::translate(float x,float y,float z) {
  identity();
  p[0][3]=x;p[1][3]=y;p[2][3]=z;
}

void ProjectiveMatrix::frustum(float left,float right,float bottom,float top,float near,float far) {
  p[0][0]=2*near/(right-left);
  p[0][1]=0;
  p[0][2]=(right+left)/(right-left);
  p[0][3]=0;

  p[1][0]=0;
  p[1][1]=2*near/(top-bottom);
  p[1][2]=(top+bottom)/(top-bottom);
  p[1][3]=0;

  p[2][0]=0;
  p[2][1]=0;
  p[2][2]=-(far+near)/(far-near);
  p[2][3]=-(2*far*near)/(far-near);

  p[3][0]=0;
  p[3][1]=0;
  p[3][2]=-1;
  p[3][3]=0;
}

void ProjectiveMatrix::perspective(float fovy,float aspect,float znear,float zfar) {
  float x=znear*sin(fovy/180*M_PI);
  float y=znear*sin(fovy/aspect/180*M_PI);
  frustum(-x,x,-y,y,znear,zfar);
}

void gljPerspective(float fovy,float aspect,float znear,float zfar) {
  float x=znear*sin(fovy/180*M_PI);
  float y=znear*sin(fovy/aspect/180*M_PI);
  glFrustum(-x,x,-y,y,znear,zfar);
}

static float sqsx=0.02,sqsy=0.02;

static void drawrect(float x1,float y1,float x2,float y2) {
  glBegin(GL_LINE_STRIP);
  glVertex2f(x1,y1);
  glVertex2f(x2,y1);
  glVertex2f(x2,y2);
  glVertex2f(x1,y2);
  glVertex2f(x1,y1);
  glEnd();
}

static void drawline(float x1,float y1,float x2,float y2,gcolor color) {
  glColor3f(color.r,color.g,color.b);
  glBegin(GL_LINES);
  glVertex2f(x1,y1);
  glVertex2f(x2,y2);
  glEnd();
  drawrect(x2-sqsx/2,y2+sqsy,x2+sqsx/2,y2);
}

void drawdircontrol(float x,float y,float w,float h,int dir) {
  gcolor act=gcolor(clnlime),pas=gcolor(clnred);
  float s=x+w/2;
  float w4=w/4;
  drawline(s,y,x,y+h,dir==-2?act:pas);
  drawline(s,y,x+w4,y+h,dir==-1?act:pas);
  drawline(s,y,s,y+h,dir==-0?act:pas);
  drawline(s,y,s+w4,y+h,dir==1?act:pas);
  drawline(s,y,x+w,y+h,dir==2?act:pas);
  glBegin(GL_LINE);
}

static void drawtriangle(float x1,float y1,float x2,float y2) {
  glBegin(GL_LINE_STRIP);
  glVertex2f(x1,y1);
  glVertex2f(x2,y1);
  glVertex2f((x1+x2)/2,y2);
  glVertex2f(x1,y1);
  glEnd();
}

void drawspeedcontrol(float x,float y,float w,float h,int speed) {
  glColor3f(0,0,1);
  drawrect(x,y,x+w,y+h);
  float s=w/2,x0=x+w/4,yd=w/4;
  if (speed==0) {
    drawrect(x0,y+h/2-s/2,x0+s,y+h/2+s/2);
  }
  for(int i=0;i<speed;i++) {
    drawtriangle(x0,y+s*i+yd,x0+s,y+s*i+yd+s);
  }
  for(int i=0;i>speed;i--) {
    drawtriangle(x0,y+h+s*i-yd,x0+s,y+h+s*i-yd-s);
  }
}


//static bool isdynamic(otItem &item,void *data) {
//  return ((GOTObject*)item.obj)->ot_isdynamic();
//}

bool TrainDriveViewType::countprojmatrix() {
  jtCab *cab=game->view->getcab();
  if (!cab) cab=gimages->cab0;
  cabhi=cab->hi;
  if (Mobile *mob=game->iswatch(&perpos,&perdif)) {
    canapply=true;
//    game->himap->changed();
    fovy=cab->fovy;
    pos=fpoint(perpos.x,perpos.y);
    see=fvector(perdif.x,perdif.y);
    posz=perpos.z;
    maxdist=perspective_maxdist();
    aspect=(float)game->view->getvpw()/(float)game->view->getvph();
    near=0.1,far=100;
    angle=atan2(perdif.y,perdif.x)/M_PI*180+90;

    ProjectiveMatrix p,t,r,res;
    p.perspective(fovy,aspect,near,far);
    r.rotate(angle,0,1,0);
    t.translate(-perpos.x,-perpos.z-cab->hi,-perpos.y);
    res=p;
    mat.multiply(res,r);
    res=mat;mat.multiply(res,t);

    clv=perpos;
    fvector3d see3=fvector3d(see.x,see.y,0).size1();
    fvector3d see3n=fvector3d(-see.y,see.x,0).size1();
    fpoint3d horizont=clv+see3*maxdist;
    fvector3d sver=(see3%see3n).size1();
    fvector3d shor=see3n.size1();

    float xsize=maxdist*sin(fovy/180*M_PI);
    float ysize=maxdist*sin(fovy/aspect/180*M_PI);

    cla=horizont+shor*xsize+sver*ysize;
    clb=horizont-shor*xsize+sver*ysize;
    clc=horizont-shor*xsize-sver*ysize;
    cld=horizont+shor*xsize-sver*ysize;

    clbase=fcomplane(cla,clb,clc);clbase.want(clv);
    clab=fcomplane(cla,clb,clv);clab.want(clc);
    clbc=fcomplane(clb,clc,clv);clbc.want(cld);
    clcd=fcomplane(clc,cld,clv);clcd.want(cla);
    clda=fcomplane(cld,cla,clv);clda.want(clb);

    return true;
  }
  canapply=false;
  return false;
}

int TrainDriveViewType::visiblecube(float coords[3],float size) {
  fpoint3d a(coords[0],coords[2],coords[1]);
  fpoint3d b(coords[0],coords[2],coords[1]+size);
  fpoint3d c(coords[0],coords[2]+size,coords[1]);
  fpoint3d d(coords[0]+size,coords[2],coords[1]);
  fpoint3d e(coords[0]+size,coords[2]+size,coords[1]);
  fpoint3d f(coords[0]+size,coords[2],coords[1]+size);
  fpoint3d g(coords[0],coords[2]+size,coords[1]+size);
  fpoint3d h(coords[0]+size,coords[2]+size,coords[1]+size);
  bool basea=clbase(a)<0,baseb=clbase(b)<0,basec=clbase(c)<0,based=clbase(d)<0,basee=clbase(e)<0,basef=clbase(f)<0,baseg=clbase(g)<0,baseh=clbase(h)<0;
  if (basea && baseb && basec && based && basee && basef && baseg && baseh) return -1;
  bool aba=clab(a)<0,abb=clab(b)<0,abc=clab(c)<0,abd=clab(d)<0,abe=clab(e)<0,abf=clab(f)<0,abg=clab(g)<0,abh=clab(h)<0;
  if (aba && abb && abc && abd && abe && abf && abg && abh) return -1;
  bool bca=clbc(a)<0,bcb=clbc(b)<0,bcc=clbc(c)<0,bcd=clbc(d)<0,bce=clbc(e)<0,bcf=clbc(f)<0,bcg=clbc(g)<0,bch=clbc(h)<0;
  if (bca && bcb && bcc && bcd && bce && bcf && bcg && bch) return -1;
  bool cda=clcd(a)<0,cdb=clcd(b)<0,cdc=clcd(c)<0,cdd=clcd(d)<0,cde=clcd(e)<0,cdf=clcd(f)<0,cdg=clcd(g)<0,cdh=clcd(h)<0;
  if (cda && cdb && cdc && cdd && cde && cdf && cdg && cdh) return -1;
  bool daa=clda(a)<0,dab=clda(b)<0,dac=clda(c)<0,dad=clda(d)<0,dae=clda(e)<0,daf=clda(f)<0,dag=clda(g)<0,dah=clda(h)<0;
  if (daa && dab && dac && dad && dae && daf && dag && dah) return -1;

  if (!basea && !baseb && !basec && !based && !basee && !basef && !baseg && !baseh
    && !aba && !abb && !abc && !abd && !abe && !abf && !abg && !abh
    && !bca && !bcb && !bcc && !bcd && !bce && !bcf && !bcg && !bch
    && !cda && !cdb && !cdc && !cdd && !cde && !cdf && !cdg && !cdh
    && !daa && !dab && !dac && !dad && !dae && !daf && !dag && !dah
  ) return 1;

//  if (clab(a)<0 && clab(b)<0 && clab(c)<0 && clab(d)<0 && clab(e)<0 && clab(f)<0 && clab(g)<0 && clab(h)<0) return -1;
//  if (clbc(a)<0 && clbc(b)<0 && clbc(c)<0 && clbc(d)<0 && clbc(e)<0 && clbc(f)<0 && clbc(g)<0 && clbc(h)<0) return -1;
//  if (clcd(a)<0 && clcd(b)<0 && clcd(c)<0 && clcd(d)<0 && clcd(e)<0 && clcd(f)<0 && clcd(g)<0 && clcd(h)<0) return -1;
//  if (clda(a)<0 && clda(b)<0 && clda(c)<0 && clda(d)<0 && clda(e)<0 && clda(f)<0 && clda(g)<0 && clda(h)<0) return -1;

  return 0;
}

bool TrainDriveViewType::applyprojmatrix() {
  if (!canapply) return false;
  glDepthRange(0,100);
  gljPerspective(fovy,aspect,near,far);
  glRotatef(angle,0,1,0);
  glTranslatef(-perpos.x,-perpos.z-cabhi,-perpos.y);
  return true;
}

void TrainDriveViewType::clear() {
  glClearColor(0.3,0.7,1,1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void View::setmapview() {
  type=&maptype;
}
void View::setdriveview() {
  type=&traindrivetype;
}
void TrainDriveViewType::iterate(unsigned char *keystate,double deltatime) {
//  game->himap->recount(CF_MOVE);
}
void TrainDriveViewType::project(float in[3],float out[2]) {
  float d0=in[0]-pos.x,d1=in[1]-pos.y,d2=in[2]-posz;
//  if (sqrt(d0*d0+d1*d1+d2*d2)>maxdist) {
//    out[0]=2;out[1]=0;
//    return;
//  }
  float vec[4]={in[0],in[1],in[2],1};
  float res[4];
  mat.multiply(res,vec);
  out[0]=res[0];
  out[1]=res[1];
//  if (res[2]*res[3]<0) {
//    out[0]=2;out[1]=0;
//    return;
//  }
  if (fabs(res[3])>0.01) {
    out[0]/=res[3];
    out[1]/=res[3];
  }
}
void ProjectiveMatrix::multiply(float res[4],float vec[4]) {
  res[0]=p[0][0]*vec[0]+p[0][1]*vec[1]+p[0][2]*vec[2]+p[0][3]*vec[3];
  res[1]=p[1][0]*vec[0]+p[1][1]*vec[1]+p[1][2]*vec[2]+p[1][3]*vec[3];
  res[2]=p[2][0]*vec[0]+p[2][1]*vec[1]+p[2][2]*vec[2]+p[2][3]*vec[3];
  res[3]=p[3][0]*vec[0]+p[3][1]*vec[1]+p[3][2]*vec[2]+p[3][3]*vec[3];
}
ZPoint TrainDriveViewType::ltop(const fpoint3d &pt) {
  float in[3]={pt.x,pt.y,pt.z},out[2]={0,0};
  project(in,out);
  return ZPoint(out[0],out[1],0);
}

void View::render(RenderContext &rc) {
  customrc=&rc;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  ViewType *lasttype=type;

  if (!type->applyprojmatrix()) {
    type=&maptype;
    type->countprojmatrix();
    type->applyprojmatrix();
  }

  if (cab) {
    int x=cab->x*pixw,y=cab->y*pixh,w=cab->w*pixw,h=cab->h*pixh;
    glViewport(x,pixh-h-y,w,h);
  }

  type->clear();


/*
  jtCab *cab=NULL;

  fvector3d perpos,perdif;
  if (Mobile *mob=game->isdrive(&perpos,&perdif)) {
    cab=mob->getcab();
    if (cab) {
      int x=cab->x*pixw,y=cab->y*pixh,w=cab->w*pixw,h=cab->h*pixh;
      glViewport(x,pixh-h-y,w,h);
      glClearColor(0.3,0.7,1,1);
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      game->himap->changed();
      TrainDriveViewType *t=new TrainDriveViewType();
      t->fovy=cab->fovy;
      t->pos=fpoint(perpos.x,perpos.y);
      t->see=fpoint(perdif.x,perdif.y);
      t->maxdist=perspective_maxdist();
      type=t;
      glDepthRange(0,100);
//    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//      glTranslatef(cab->x*2,2-cab->y*2,0);
//      glScalef(cab->w,cab->h,1);
      gljPerspective(45,(float)w/(float)h,0.1,100);
//      gljPerspective(45,(float)pixw/(float)pixh,0.1,100);
//    gluPerspective(45,(float)pixw/(float)pixh,0.1,100);
      glRotatef(atan2(perdif.y,perdif.x)/M_PI*180+90,0,1,0);
      glTranslatef(-perpos.x,-perpos.z-cab->hi,-perpos.y);
    }
  }
*/
/*
  if (!cab) {
    game->view->type=NULL;
    glDepthRange(100,0);
    float viewmatrix[16]={1,0,0,0,
                    0,cos(seeangle/180*M_PI),1,0,
                    0,-sin(seeangle/180*M_PI),0,0,
                    0,0,0,1};

    glTranslatef(-dx,dy,0);
    glScalef(scalex,scaley,0.01);
    glMultMatrixf(viewmatrix);
    glRotatef(viewangle,0,1,0);
  }
*/
  if (type->stdrender()) {
    rc.setdepthtest(true);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    rc.setculling(false);

    if (oglo.lighting) {
      float light_ambient[]={oglo.ambient,oglo.ambient,oglo.ambient,1.0};
      float light_diffuse[]={oglo.diffuse,oglo.diffuse,oglo.diffuse,1.0};
      float light_specular[]={oglo.specular,oglo.specular,oglo.specular,1.0};

      float light_position[]={cos((getviewangle()+oglo.sunangle)/180*M_PI),oglo.sunydir,sin((getviewangle()+oglo.sunangle)/180*M_PI),0};

      glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
      glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
      glLightfv(GL_LIGHT0, GL_POSITION, light_position);

      rc.setlighting(true);
      glEnable(GL_LIGHT0);
    }

    game->mobiles->render(rc);
    game->statics->render(rc);
    rc.setlighting(false);
    game->surmap->render(rc);
    rc.setsmoothshade(true);
    game->hiview->render(rc);
    rc.setsmoothshade(false);
    game->rrmap->render(rc);
    game->roadmap->render(rc);
    game->mobiles->rendertransparent(rc);
    if (type->paintgame()) game->statics->rendercaptions(rc);

/*
  game->mobiles->render(rc);
  game->statics->render(rc);

  rc.setlighting(false);

  game->surmap->render(rc);

  rc.setsmoothshade(true);
  game->himap->render(rc);
  rc.setsmoothshade(false);
  game->rrmap->render(rc);
  game->mobiles->rendertransparent(rc);

  game->statics->rendercaptions(rc);
*/
//  int st=clock();
//  gocttree->removegroup(isdynamic);
//  int tim=clock()-st;
//  float ft=tim/(float)CLK_TCK;

  } else type->dorender(rc);

  if (cab) {
    XMLNode xml;
    if (Mobile *m=game->isdrive()) xml=m->getcabprops();else xml=XMLNode::create();
    glViewport(0,0,pixw,pixh);
    cab->render(rc,xml);
  }
/*
  if (cab) {
    glViewport(0,0,pixw,pixh);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    rc.settexture2d(true);
    rc.setdepthtest(false);
    if (cab->bind(rc)) {
      glColor4f(1,1,1,1);
      rc.setalphatest(true);
      glBegin(GL_QUADS);
      glTexCoord2f(0,0);glVertex2f(-1,-1);
      glTexCoord2f(1,0);glVertex2f(1,-1);
      glTexCoord2f(1,1);glVertex2f(1,1);
      glTexCoord2f(0,1);glVertex2f(-1,1);
      glEnd();
      rc.setalphatest(false);
    }
    glColor3f(1,1,1);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    rc.settexture2d(false);
  }
*/
  if (type->stdrender()) {
    rc.setculling(false);
    rc.setlighting(false);
  }
  glColor3f(1,1,1);

  if (type->paintgame()) {
    rc.push();
    glMatrixMode(GL_PROJECTION);
    rc.setdepthtest(false);
    rc.setblending(true);
    rc.setlighting(false);
//    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    if (cab) {
      int x=cab->x*pixw,y=cab->y*pixh,w=cab->w*pixw,h=cab->h*pixh;
      glViewport(x,pixh-h-y,w,h);
    }
    game->rendervolatiles(rc);
    game->onpaint();
    if (cab) glViewport(0,0,pixw,pixh);
    rc.pop();
//    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }

  if (game->isdrive()) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    drawdircontrol(0.7,0.7,0.2,0.2,direction);
    drawspeedcontrol(0.6,0.7,0.07,0.2,speedc);
    glPopMatrix();
  }

  static double lastfpstime=0,fps=0;
  static int flips=0;
  flips++;
  if (lastfpstime==0) {
    lastfpstime=clock()/(double)CLK_TCK;
  }
  double acttime=clock()/(double)CLK_TCK;
  if (acttime-lastfpstime>fpsinterval) {
    fps=flips/(acttime-lastfpstime);
    lastfpstime=acttime;
    flips=0;
  }
  if (showfps) rendertitle(rc,"fps:"+ftos(round(fps*100)/100)+";rec:"+itos(visibleitems.count),-1,1,true);
//  glFlush();
  type=lasttype;

  customrc=NULL;
}

/*
bool TrainDriveViewType::visible(fpoint3d &pt) {
  if (fdistance(pt,pos)>maxdist) return false;
  fvector act=fpoint(pt.x,pt.y)-pos;
  float jm=act.fsize()*see.fsize();
  if (jm<0.01) return true;
  if (cos(fovy)>fabs(act*see)/jm) return false;
//  float dangle=acos(fabs(act*see)/jm);
//  if (dangle>fovy) return false;
  return true;
}
*/

}

