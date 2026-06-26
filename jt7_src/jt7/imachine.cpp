#include <jlib.h>
#pragma hdrstop

#include "imachine.h"
#include "icab.h"
#include "ismoke.h"

namespace jtyc {

float MSF=0.08;//machinesizefactor
float ASF=0.05;//aircraftsizefactor

jtMachine::~jtMachine() {
}
void jtAircraft::load(PersistentObject *data,XMLNode xml) {
  model=jt_loadmodel(data,"model.zip");
  isdefault=xml.ivar("default");
  name=xml.svar("name");
  title=lang_text_ch(xml.svar("title"));

  v_start=xml.fvar("v_start");
  start_len=xml.fvar("start_len");
  rise_hi=xml.fvar("rise_hi");
  rise_len=xml.fvar("rise_len");

  lower_hi=xml.fvar("lower_hi");
  lower_len=xml.fvar("lower_len");
  v_land=xml.fvar("v_land");
  land_len=xml.fvar("land_len");

  speed=xml.fvar("speed");

  air_radius=xml.fvar("air_radius");

  rise_angle=xml.fvar("rise_angle");
  lower_angle=xml.fvar("lower_angle");
  start_angling=xml.fvar("start_angling");
  land_angling=xml.fvar("land_angling");

  if (v_start<1) v_start=1;
  if (start_len<1) start_len=1;
  if (rise_hi<0.1) rise_hi=0.1;
  if (rise_len<1) rise_hi=1;

  if (v_land<1) v_land=1;
  if (land_len<1) land_len=1;
  if (lower_hi<0.1) lower_hi=0.1;
  if (lower_len<1) lower_hi=1;

  if (speed<1) speed=1;
  if (air_radius<1) air_radius=1;

  Extrems e;
  model->getextrems(e);
  float len=e.xmax-e.xmin;
  len*=ASF;

  float d=3.0*len/4.0;

  model->translate(e.xmin-d/ASF,0,0);
  model->scale(ASF,ASF,ASF);
}

void jtMachine::load(PersistentObject *data,XMLNode xml) {
  model=jt_loadmodel(data,"model.zip");
  isdefault=xml.ivar("default");
  zipobj=data;

  if (xml.svar("cab")!="") {
    PersistentObject *child=data->openchildpath(xml.svar("cab"),true);
    if (child) {
      cab=new jtCab();
      MainXMLNode cx;
      child->loadxml("index.xml",cx);
      cab->load(child,cx);
    }
  }
  if (xml.svar("smoke")!="") {
    PersistentObject *child=data->openchildpath(xml.svar("smoke"),true);
    if (child) {
      smoke=new jtSmoke();
      MainXMLNode cx;
      child->loadxml("index.xml",cx);
      smoke->load(child,cx);
    }
  }
  smoke_x=xml.fvar("smoke_x");
  smoke_y=xml.fvar("smoke_y");
/*
  if (xml.searchsub("smoke")>=0) {
    smoke=new jtParticle();
    smoke->load(data,xml["smoke"]);
  }
*/
  name=xml.svar("name");
  path=xml.svar("path");
  title=lang_text_ch(xml.svar("title"));
  author=xml.svar("author");

  power=xml.fvar("power");
  maxspeed=xml.fvar("maxspeed");
  weight=xml.fvar("weight");

  Extrems e;
  model->getextrems(e);
  len=e.xmax-e.xmin;
  len*=MSF;

  d1=jlib::min(len/4.0,0.1);
  d2=len-d1;

  model->translate(e.xmin-d1/MSF,0,0);
  model->scale(MSF,MSF,MSF);
  bound=model->bound();
}



}

