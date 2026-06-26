#include <jlib.h>
#pragma hdrstop

#include "awsglmodelview.h"

namespace aws {

void GLModelView::setlight(float ambient,float diffuse,float specular,float ydir,float xzangle) {
  this->ambient=ambient;
  this->diffuse=diffuse;
  this->specular=specular;
  this->ydir=ydir;
  this->xzangle=xzangle;
}

void GLModelView::setmapview() {
  float viewmatrix[16]={1,0,0,0,
                    0,cos(mv_seeangle/180*M_PI),1,0,
                    0,-sin(mv_seeangle/180*M_PI),0,0,
                    0,0,0,1};
  glDepthRange(100,0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glTranslatef(-mv_dx,mv_dy,0);
  glScalef(mv_scale,mv_scale*getkxy(),0.01);
//  glScalef(mv_scale,mv_scale,mv_scale);
  glMultMatrixf(viewmatrix);
  glRotatef(mv_viewangle,0,1,0);
}

float GLModelView::getkxy() {
  float kxy=1;
  Size size=getsize();
  if (size.h>0) kxy=(float)size.w/(float)size.h;
  return kxy;
}

void GLModelView::dopaintgl(awsEvent &event,RenderContext &rc) {
  if (ismapview) setmapview();
  GLView::dopaintgl(event,rc);
  rc.push();
  rc.vars=vars;

  if (lighting) {
    float light_ambient[]={ambient,ambient,ambient,1.0};
    float light_diffuse[]={diffuse,diffuse,diffuse,1.0};
    float light_specular[]={1,1,1,1.0};

    float light_position[]={cos((mv_viewangle+xzangle)/180*M_PI),ydir,sin((mv_viewangle+xzangle)/180*M_PI),0};
    float mat_specular[]={specular,specular,specular,1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);

    rc.setlighting(true);
    glEnable(GL_LIGHT0);
  }
  if (model) model->render(rc);
  rc.vars=NULL;
  rc.pop();
}

GLModelView::~GLModelView() {
  texman->deletedcontext(this);
}

void GLModelView::setvars(gloVariableList *value) {
  vars=value;
  render();
}

void GLModelView::mapview(float seeangle,float viewangle,float scale,float dx,float dy) {
  ismapview=true;
  mv_seeangle=seeangle;
  mv_viewangle=viewangle;
  mv_scale=scale;
  mv_dx=dx;
  mv_dy=dy;
  render();
}

GLModelView::GLModelView(Component *owner,System *system):GLView(owner,system) {
  ismapview=false;
  lighting=false;
  ambient=0.6;
  diffuse=0.7;
  specular=0.8;
  ydir=-1;
  xzangle=45;
  vars=new gloVariableList();
}

}


