#ifndef mobjsH
#define mobjsH

#include <zaba/zbrt.h>
#include <zaba/zbint.h>
#include <zaba/zbobj.h>
#include <aws/awsglview.h>
#include <aws/awsglmodelview.h>
#include <zaba/zbaws.h>
#include <zaba/zbext.h>
#include "coloralg.h"
#include "jtmobjs.h"
#include <jmath/pedralg.h>

namespace jtyc {
using namespace zaba;
class zbModelVariables;

struct GLViewType:ComponentType {
  virtual string getloadname() {return "glview";}
  virtual zbObject *createzb(Component *comp,Runtime *rt);
  virtual aws::Component *createnew(aws::Component *owner,aws::System *system);
};

struct GLModelViewType:ComponentType {
  virtual string getloadname() {return "glmodelview";}
  virtual zbObject *createzb(Component *comp,Runtime *rt);
  virtual aws::Component *createnew(aws::Component *owner,aws::System *system);
};


struct zbGLView:zbControl {
  GLView *getlink() {return (GLView*)getcomp();}
  zbObject *getrenderContext();
  zbGLView(Runtime *rt,zbwProp *prop,STRING intf="GLView.instance");
};

struct zbModel:zbObject {
  RefBase<gloModel> model;
  gloModel *getlink() {return model;}
  zbModel(gloModel *amodel,Runtime *rt,STRING intf="Model.instance"):zbObject(rt,intf) {model=amodel;}
  void drawProjection(zbCanvas *canvas,int dir,float sx,float sy,float tx,float ty);
};

struct zbGLModelView:zbGLView {
  GLModelView *getlink() {return (GLModelView*)getcomp();}
  zbGLModelView(Runtime *rt,zbwProp *prop,STRING intf="GLModelView.instance"):zbGLView(rt,prop,intf) {}
  zbObject *getmodel() {if (gloModel *m=getlink()->getmodel()) return new zbModel(m,rt);return createVoid(rt);}
  void setmodel(zbObject *value) {if (zbModel *m=dynamic_cast<zbModel*>(value)) getlink()->setmodel(m->model);else getlink()->setmodel(NULL);}
  void setvars(zbModelVariables *value) {getlink()->setvars(value->vars);}
  zbObject *getvars() {return new zbModelVariables(getlink()->getvars(),rt);}
};

struct zbColorAlg:zbObject {
  ColorAlg alg;
  ColorAlg *getlink() {return &alg;}
  zbColorAlg(Runtime *rt,STRING intf="ColorAlg.instance"):zbObject(rt,intf) {}
};

struct zbStampAlg:zbObject {
  StampAlg alg;
  StampAlg *getlink() {return &alg;}
  zbStampAlg(Runtime *rt,STRING intf="StampAlg.instance"):zbObject(rt,intf) {}
};

struct zbPedrAlg:zbObject {
  pedr::PedrAlg alg;
  pedr::PedrAlg *getlink() {return &alg;}
  zbPedrAlg(Runtime *rt,STRING intf="PedrAlg.instance"):zbObject(rt,intf) {}
  void run(XMLNode xml);
  void modifyRun(XMLNode input,XMLNode output);
};

}

#endif
