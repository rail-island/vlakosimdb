#include <jlib.h>
#pragma hdrstop

#include <aws/awssystem.h>
#include <gram/gmlang.h>
#include <gram/ssmain.h>
#include <gram/sscm.h>
#include <zaba/zbrt.h>
#include <zaba/zbint.h>
#include <zaba/zbaws.h>
#include <zaba/zbext.h>
//#include "../jt7/grrcom.h"
#include "mobjs.h"
#include <glo/glotextures.h>

//#include "coloralg.h"

using namespace gram;
using namespace zaba;
using namespace aws;

sFile libdir;

namespace zaba {
#include "lib/modelobjs.h"
#include "jtmlib/jtmobjs.h"
}

namespace jtyc {

void deletezbwprop(void *param,Component *comp) {
  comp->deleteprop("zbwProp");
}
bool runzbc() {
//  Archive ar;
//  string file=basedir/"objects"/"pokus.zip";
//  ar.open(file);
//  jsystem->showinfo(ar.loadtext("index.xml"));
//  ar.close();

  awssys->getrepos()->recload(basedir/"data"/"forms","*.xml");
  awssys->getrepos()->recload(basedir/"data"/"forms","*.frm");

  zaba::Runtime rt;
  rt.rootdir=basedir;
  initstdrt(rt);
  initextrt(rt);
  initawsrt(rt);
  init_aws_app(rt);
  init_modelobjs_rt(rt);
  init_jtmobjs_rt(rt);
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
  using namespace jtyc;

/*
  TCntVec<fpoint> a,b,res;
  a.add(fpoint(0,0));
  a.add(fpoint(1,0));
  a.add(fpoint(0,1));

  b.add(fpoint(0,0));
  b.add(fpoint(1,0));
  b.add(fpoint(1,1));

  intersection(a,b,res);
  return;
*/

  awssys=sys;
//  awssys->getopt().loadfromfile(basedir/"options.xml");
  awssys->setoptionfile(basedir/"poptions.xml");
  libdir=basedir/"data"/"modellib";
  zaba::registerComponent(new GLModelViewType());
  zaba::registerComponent(new GLViewType());
//  init_tt_cons();
  init_texture_manager();
  texbasedir=basedir/"data"/"textures";
  matbasedir=basedir/"data"/"materials";
  runzbc();
//  awssys->getopt().savetofile(basedir/"options.xml");
}
