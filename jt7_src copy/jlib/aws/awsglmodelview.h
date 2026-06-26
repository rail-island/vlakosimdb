#ifndef awsglmodelviewH
#define awsglmodelviewH

//toto neni soucasti knihovny aws

#include "awsglview.h"
#include <glo/gloobject.h>

namespace aws {
using namespace glo;

struct GLModelView:GLView {
  RefBase<gloVariableList> vars;

  GLModelView(Component *owner,System *system);
//  GLModelView();
  ~GLModelView();
  gloModel *getmodel() {return model;}
  void setmodel(gloModel *value) {model=value;}
  void mapview(float seeangle,float viewangle,float scale,float dx,float dy);
  void dopaintgl(awsEvent &event,RenderContext &rc);
  bool getlighting() {return lighting;}
  void setlighting(bool value) {lighting=value;}
  void setlight(float ambient,float diffuse,float specular,float ydir,float xzangle);

  void setvars(gloVariableList *value);
  gloVariableList *getvars() {return vars;}
private:
  void setmapview();
  float getkxy();

  RefBase<gloModel> model;

  bool ismapview;
  float mv_seeangle,mv_viewangle,mv_scale,mv_dx,mv_dy;

  bool lighting;
  float ambient,diffuse,specular,ydir,xzangle;
};

}

#endif
