#include <jlib.h>
#pragma hdrstop

#include "game0.h"
#include "gameu.h"
#include <sdl/sdl.h>
#include <opengl.h>
#include "gview.h"
#include "imachine.h"
#include "istatic.h"
#include "imotocar.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)

namespace jtyc {

namespace mapcolors {
  float h1;
  float r0,g0,b0;
  float r1,g1,b1;
  float r2,g2,b2;
}

float maxmaphi=32,maxelemhi=8,hcoordlimit=maxmaphi+maxelemhi;

GlobalData jtdata;
OpenGLOpts oglo;
BehaviorOpts beop;

//const rdist_t semaphore_dir_0_penalty=10;
OctTree *gocttree;

SmallVector3::SmallVector3(const fvector3d &vec) {
  float fx=vec.x,fy=vec.y,fz=vec.z;
  if (fx<-1) fx=-1;if (fx>1) fx=1;
  if (fy<-1) fy=-1;if (fy>1) fy=1;
  if (fz<-1) fz=-1;if (fz>1) fz=1;
  x=fx*511;
  y=fy*511;
  z=fz*511;
}
fvector3d SmallVector3::tofloat() {
  return fvector3d(x/511.0,y/511.0,z/511.0);
}

void OpenGLOpts::load(XMLNode xml) {
  lighting=xml.ivar("lighting");
  heightlines=xml.ivar("heightlines");
  specular=xml.fvar("specular");
  ambient=xml.fvar("ambient");
  diffuse=xml.fvar("diffuse");
  sunangle=xml.fvar("sunangle");
  sunydir=xml.fvar("sunydir");
  drawtrees=xml.ivar("drawtrees");

/*
  ptcursorsize2=xml.fvar("ptcursorsize2"); =8;//polovina velikosti bodoveho kurzoru

  train_hi=xml.fvar("train_hi"); 1;

  train_mez=xml.fvar("train_mez"); 0.08;
  max_train_click_distance=xml.fvar("max_train_click_distance");  =1;
  max_motocar_click_distance=xml.fvar("max_motocar_click_distance"); =1;
*/
}

void BehaviorOpts::load(XMLNode xml) {
  ptcursorsize2=xml.fvar("ptcursorsize2");
  if (ptcursorsize2<1) ptcursorsize2=1;

  train_hi=xml.fvar("train_hi");
  if (train_hi<0.1) train_hi=0.1;

  train_mez=xml.fvar("train_mez");
  if (train_mez<0) train_mez=0;

  max_train_click_distance=xml.fvar("max_train_click_distance");

  max_motocar_click_distance=xml.fvar("max_motocar_click_distance");

  train_speed_koef=xml.fvar("train_speed_koef");
  if (train_speed_koef<0.001) train_speed_koef=0.001;

  motocar_speed_koef=xml.fvar("motocar_speed_koef");
  if (motocar_speed_koef<0.001) motocar_speed_koef=0.001;

  semaphore_dir_0_penalty=xml.fvar("semaphore_dir_0_penalty");
  if (semaphore_dir_0_penalty<0) semaphore_dir_0_penalty=0;

  min_train_speed=xml.fvar("min_train_speed");
  if (min_train_speed<0.01) min_train_speed=0.01;

  train_F_t=xml.fvar("train_F_t");
  if (train_F_t<0) train_F_t=0;

  train_break_koef=xml.fvar("train_break_koef");
  if (train_break_koef<0.1) train_break_koef=0.1;

  min_motocar_speed=xml.fvar("min_motocar_speed");
  if (min_motocar_speed<0.01) min_motocar_speed=0.01;

  motocar_F_t=xml.fvar("motocar_F_t");
  if (motocar_F_t<0) motocar_F_t=0;

  motocar_max_block_time=xml.fvar("motocar_max_block_time");
  if (motocar_max_block_time<1) motocar_max_block_time=1;
  if (motocar_max_block_time>100) motocar_max_block_time=100;

  aircraft_speed_koef=xml.fvar("aircraft_speed_koef");
  if (aircraft_speed_koef<0.001) aircraft_speed_koef=0.001;

  max_aircraft_click_distance=xml.fvar("max_aircraft_click_distance");

  right_sided_semaphores=xml.ivar("right_sided_semaphores");

  train_air_koef_a=xml.fvar("train_air_koef_a");
  train_air_koef_b=xml.fvar("train_air_koef_b");

  BSF=xml.fvar("BSF");
  MSF=xml.fvar("MSF");
  ASF=xml.fvar("ASF");
  CSF=xml.fvar("CSF");

  train_force_koef=xml.fvar("train_force_koef");
  if (train_force_koef<0.01) train_force_koef=0.01;

  motocar_force_koef=xml.fvar("motocar_force_koef");
  if (motocar_force_koef<0.01) motocar_force_koef=0.01;
}

float randpause(float secs) {
  float r=rand()/(float)RAND_MAX;
  return secs-secs/10+r*secs/5;
}

void glTextOut(float x,float y,string text,RenderContext &rc) {
  jtdata.font.render(x,y,text,rc);
}

void GlobalData::init() {
  MainXMLNode fontxml;fontxml.loadfromfile(basedir/"data"/"font_sizes.xml");
  font.load(fontxml,basedir/"data"/"font_images.bmp");
}


GInstance::GInstance() {
  game->creatinginst(this);
}
GInstance::~GInstance() {
  game->deletedinst(this);
}
void GObject::changed() {
  game->changedobject(this);
}
void GObject::event(string type) {
  game->objectevent(this,type);
}
void GObject::created() {
  game->createdobj(this);
}
GObject::GObject() {
  game->creatingobj(this);
  setname("object");
}
GObject::~GObject() {
  game->deletedobj(this);
}
void GObject::setname(string newname) {
  name="";//nutne
  name=game->makeuniquename(newname);
}
void GObject::remove() {
  if (canremove()) game->removeobj(this);
}


void rendertitle(RenderContext &rc,string title,float x,float y,bool drawsq) {
  rc.push();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  rc.setdepthtest(false);
  glLoadIdentity();
  glTranslatef(x,y,0);
  glScalef(0.5,0.5,0.5);
  rc.settexture2d(false);

/*
  if (drawsq) {
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glVertex2f(0,0);
    glVertex2f(jtdata.font.textwi(title),0);
    glVertex2f(jtdata.font.textwi(title),jtdata.font.texthi(title));
    glVertex2f(0,jtdata.font.texthi(title));
    glEnd();
  }
*/


  if (!drawsq) rc.setalphatest(true);
  rc.settexture2d(true);

  glColor3f(1,1,1);
  glTextOut(0,0,title,rc);

  glPopMatrix();
  glColor3f(1,1,1);
  rc.pop();
}

void rendertitle(RenderContext &rc,string title,float mapx,float mapy,float maphi,bool drawsq) {
  ZPoint zpt=game->view->ltop(fpoint3d(mapx,maphi,mapy));
  rendertitle(rc,title,zpt.x,zpt.y);
}

void CommonEngine::repair() {
  if (power<1) power=1;
  if (maxspeed<1) maxspeed=1;
  if (weight<1) weight=1;
}


/*GObject::GObject(Game *agame) {
  game=agame;
  gid=game->gidcnt++;
}
GObject::GObject(BINREAD &fr) {
  game=frgame(fr);
  fr>>gid;
  frloader(fr).addobj(this);
}*/

template struct agpoint<double>;
template struct agline<double>;
template struct agvector<double>;

}

using namespace jtyc;
