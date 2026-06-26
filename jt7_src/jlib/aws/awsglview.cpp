#include <jlib.h>
#pragma hdrstop

#include "awsglview.h"
#include "awsreal.h"
#include <opengl.h>

namespace aws {

GLView::GLView(Component *owner,System *system):Control(owner,system) {
  real=createGLView(system,this);
  if (real) if (RealControl *rc=real->getrealctrl()) setreal(rc);
  rcref=new RenderContextReference();
}

void GLView::onpaintgl(awsEvent &event) {
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  RenderContext rc;rc.ctxid=this;
  rcref->rc=&rc;
  dopaintgl(event,rc);
  rcref->rc=NULL;
}

void GLView::dopaintgl(awsEvent &event,RenderContext &rc) {
  donotify("onpaintgl",ev_paintgl.get());
}

void GLView::loadprops(XMLNode xml,Form *form) {
  Control::loadprops(xml,form);
  ev_paintgl.load(xml,"onpaintgl");
}

}
