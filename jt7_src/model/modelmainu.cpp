#include <jlib.h>
#pragma hdrstop

#include <aws/awssystem.h>
#include <gram/gmlang.h>
#include <gram/ssmain.h>
#include <gram/sscm.h>
#include <zaba/zbrt.h>
#include <zaba/zbint.h>
#include <zaba/zbaws.h>
#include "../jt7/grrcom.h"
#include "mobjs.h"
#include <glo/glotextures.h>

using namespace gram;
using namespace zaba;
using namespace aws;

sFile libdir;

namespace zaba {
#include "lib/modelobjs.h"
}

namespace zp {

void deletezbwprop(void *param,Component *comp) {
  comp->deleteprop("zbwProp");
}
bool runzbc() {
  awssys->getrepos()->recload(basedir/"data"/"forms","*.xml");
  awssys->getrepos()->recload(basedir/"data"/"forms","*.frm");

  zaba::Runtime rt;
  rt.rootdir=basedir;
  initstdrt(rt);
  initawsrt(rt);
  init_aws_app(rt);
  init_modelobjs_rt(rt);
  try {
    rt.adddir(libdir);
    rt.loadedlib();
  } catch (JException &e) {
    e.show();
    return false;
  }
  awssys->foreach(0,deletezbwprop);
  return true;
}

}

void aws::awsmain(System *sys,int argc,char **argv) {
  using namespace zp;
  awssys=sys;
  awssys->getopt().loadfromfile(basedir/"options.xml");
  libdir=basedir/"data"/"modellib";
  zaba::registerComponent(new GLModelViewType());
  zaba::registerComponent(new GLViewType());
  init_tt_cons();
  init_texture_manager();
  texbasedir=basedir/"data"/"textures";
  matbasedir=basedir/"data"/"materials";
  runzbc();
  awssys->getopt().savetofile(basedir/"options.xml");
}
