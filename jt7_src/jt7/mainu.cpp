#include <jlib.h>
#pragma hdrstop

#include <aws/awssystem.h>
#include <aws/awswindow.h>
#include <aws/awslayout.h>
#include <aws/awsextctrls.h>
#include <zaba/zbrt.h>
#include <zaba/zbint.h>
#include <zaba/zbaws.h>
#include <sdw/sdwsysext.h>
#include "jtobjs.h"
#include "grrcom.h"
#include "gmtcom.h"
#include <glo/glotextures.h>

//aws::System *awssys;

using namespace aws;
using namespace zaba;

sFile libdir;


namespace zaba {
#include "lib/jtobjs.h"
#include "../model/jtmlib/jtmobjs.h"
}

void deletezbwprop(void *param,Component *comp) {
  comp->deleteprop("zbwProp");
}
bool runzbc() {
//  gsystem=creategsystem();

  awssys->getrepos()->recload(basedir/"data"/"forms","*.xml");
  awssys->getrepos()->recload(basedir/"data"/"forms","*.frm");
  awssys->getrepos()->recload(basedir/"data"/"glomodels","*.xml");

  zaba::Runtime rt;
  rt.rootdir=basedir;
  initstdrt(rt);
  initextrt(rt);
  initawsrt(rt);
  init_aws_app(rt);
  init_jtobjs_rt(rt);
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

XMLNode jtyc::findresource(string name) {
  return awssys->findrep(name);
}

static void init_map_colors(XMLNode xml) {
  using namespace mapcolors;
  h1=xml.fvar("h1");
  r0=xml.fvar("r0");g0=xml.fvar("g0");b0=xml.fvar("b0");
  r1=xml.fvar("r1");g1=xml.fvar("g1");b1=xml.fvar("b1");
  r2=xml.fvar("r2");g2=xml.fvar("g2");b2=xml.fvar("b2");
}

void aws::awsmain(System *sys,int argc,char **argv) {
  try {
    awssys=sys;
    sysext=dynamic_cast<sdw::sdwSystemExtension*>(awssys);
    awssys->setoptionfile(basedir/"goptions.xml");
//    awssys->getopt().loadfromfile(basedir/"goptions.xml");
    sysext->setwantgl(true);
    libdir=basedir/"data"/"lib";
    texbasedir=basedir/"data"/"textures";
    matbasedir=basedir/"data"/"materials";
    glo::init_texture_manager();
    jtdata.init();
    init_tt_cons();
    init_mt_cons();
    init_map_colors(sys->getopt()["map_colors"]);
//    init_tt_bmps();
    runzbc();
//    awssys->getopt().savetofile(basedir/"options.xml");
  } catch (JException &e) {
    e.show();
  }
}





