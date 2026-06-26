#ifndef awsglviewH
#define awsglviewH

#include "awscontrol.h"
#include "awsreal.h"
#include <glo/gloobject.h>

//neni soucasti standardni knovny aws

namespace aws {
using namespace glo;

class GLView;

struct RealGLView {
  virtual RealControl *getrealctrl() {return NULL;}
  virtual void render() {}
};

extern RealGLView *createGLView(aws::System *system,GLView *glview);

struct GLView:Control {
  RefBase<RenderContextReference> rcref;

  GLView(Component *owner,System *system);
  void loadprops(XMLNode xml,Form *form);
  virtual void dopaintgl(awsEvent &event,RenderContext &rc);
  void onpaintgl(awsEvent &event);
  void render() {real->render();}

  RealGLView *real;
  EventValue ev_paintgl;
};

}

#endif
