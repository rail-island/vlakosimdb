#include <jlib.h>
#pragma hdrstop

#include "gaircraft.h"
#include "gairport.h"
#include "gview.h"
#include "gameu.h"
#include "ghimap.h"
#include "gsurmap.h"
#include "gimages.h"
#include "imachine.h"
#include "gstatic.h"

namespace jtyc {

//const float aircraft_speed_koef=0.02;
//const double max_aircraft_click_distance=1;

bool Aircraft::setdest(Airport *dest) {
  this->dest=dest;
  changed();
  return true;
}

void Aircraft::initCONST() {
  speed=beop.aircraft_speed_koef*type->speed;

  start_v=beop.aircraft_speed_koef*type->v_start;
  start_a=start_v*start_v/(2.0*type->start_len);

  rise_t=type->rise_len/(start_v+0.5*(speed-start_v));
  rise_zv=type->rise_hi/rise_t;
  rise_a=(speed-start_v)/rise_t;

  land_v=beop.aircraft_speed_koef*type->v_land;
  land_a=land_v*land_v/(2.0*type->land_len);

  lower_t=type->rise_len/(speed+0.5*(land_v-speed));
  lower_zv=type->lower_hi/lower_t;
  lower_a=(land_v-speed)/lower_t;
}

bool Aircraft::ismouseon(float x,float y) {
  fvector3d pos;
  float alfa=0,along=0;
  countpos(&pos,&alfa,&along);
  ZPoint za=game->view->ltop(fpoint3d(pos.x,pos.z,pos.y));
  if (fdistance(za,fpoint(x,y))<=beop.max_aircraft_click_distance*game->view->maptype.scale) return true;
  return false;
}

void Aircraft::settype(string value) {
  type=gimages->findaircraft(value);
}

Aircraft::Aircraft(XMLNode xml):Mobile(xml) {
  loadxml=xml;

  dest=dynamic_cast<Airport*>(game->findobj(xml.svar("destination")));
  startairport=dynamic_cast<Airport*>(game->findobj(xml.svar("startairport")));
  landairport=dynamic_cast<Airport*>(game->findobj(xml.svar("landairport")));
  blocking=dynamic_cast<Airport*>(game->findobj(xml.svar("blocking")));

  settype(xml.svar("type"));
  state=(State)xml.ivar("state");
  initCONST();
  traj.load(xml["traj"]);

  start_s=xml.fvar("start_s");
  cur_alfa=xml.fvar("cur_alfa");
  cur_v=xml.fvar("cur_v");
  rise_dh=xml.fvar("rise_dh");
  rise_h0=xml.fvar("rise_h0");
  land_s=xml.fvar("land_s");
  fly_t=xml.fvar("fly_t");
  cur_pos.x=xml.fvar("cur_pos.x");
  cur_pos.y=xml.fvar("cur_pos.y");
  cur_pos.z=xml.fvar("cur_pos.z");
}

void Aircraft::save(XMLNode xml) {
  Mobile::save(xml);
  xml.setname("aircraft");
  if (dest) xml.setvar("destination",dest->getname());
  if (startairport) xml.setvar("startairport",startairport->getname());
  if (landairport) xml.setvar("landairport",landairport->getname());
  if (blocking) xml.setvar("blocking",blocking->getname());
  xml.setvar("state",(int)state);
  xml.setvar("type",type->name);
  traj.save(xml.forcesub("traj"));

  xml.setvar("start_s",start_s);
  xml.setvar("cur_alfa",cur_alfa);
  xml.setvar("cur_v",cur_v);
  xml.setvar("rise_dh",rise_dh);
  xml.setvar("rise_h0",rise_h0);
  xml.setvar("land_s",land_s);
  xml.setvar("fly_t",fly_t);
  xml.setvar("cur_pos.x",cur_pos.x);
  xml.setvar("cur_pos.y",cur_pos.y);
  xml.setvar("cur_pos.z",cur_pos.z);

  game->saving=xml;
  event("save");
}

Aircraft::Aircraft(Airport *start) {
  dest=NULL;
  type=NULL;
  landairport=NULL;
  startairport=start;
  blocking=start;
  blocking->addair();
//  type=gimages->aircraft0;
  settype("");
  game->mobiles->items.add(this);
  setname(lang_text_ch("$Aircraft 1"));
  start->addwait(this);
  state=waiting;
  initCONST();
  created();
}

void Aircraft::destroy() {
  Mobile::destroy();
  if (blocking) blocking->freeair();
}

void Aircraft::findway() {
  traj.items.clear();
  if (dest) {
    dest->findway(traj,type->air_radius,cur_pos,cur_alfa,type->lower_len+type->land_len);
  } else {
//    traj.addarc(
  }
}

void Aircraft::run(double deltatime) {
  switch (state) {
    case waiting:{
      state=starting;
      start_s=0;
      cur_v=0;
    } break;
    case starting:{
      cur_v+=start_a*deltatime;
      start_s+=cur_v*deltatime;
      if (start_s>type->start_len) {
        countpos(&cur_pos,&cur_alfa,NULL);
        cur_v=start_v;
        rise_h0=cur_pos.z;
        rise_dh=0;
        state=rising;
        blocking->freeair();
        blocking=NULL;
      }
    } break;
    case rising:{
      float dh=rise_zv*deltatime;
      rise_dh+=dh;
      cur_v+=rise_a*deltatime;
      start_s+=cur_v*deltatime;
      countpos(&cur_pos,&cur_alfa,NULL);
      if (rise_dh>type->rise_hi) {
        startairport=NULL;
        findway();
        state=flying;
        fly_t=0;
      }
    } break;
    case flying:{
      fly_t+=cur_v*deltatime;
      if (fly_t>traj.len()) {
        if (dest) {
          if (dest->isfree()) {
            state=lowering;
            landairport=dest;
            land_s=0;
            rise_dh=0;
            rise_h0=landairport->z+type->lower_hi;
            cur_v=speed;

            blocking=dest;
            blocking->addair();
          } else {
            fpoint pos=traj.pos(traj.len());
            fvector diff=traj.diff(traj.len()),n=diff.getnormal();

            traj.items.clear();
            float angle=atan2(-n.y,-n.x);
            traj.addarc(pos+n*type->air_radius,type->air_radius,angle,angle+2*M_PI);
            fly_t=0;
          }
        } else findway();
      }
    } break;
    case lowering:{
      float dh=lower_zv*deltatime;
      rise_dh+=dh;
      land_s+=cur_v*deltatime;
      cur_v+=lower_a*deltatime;
      if (rise_dh>type->lower_hi) {
        state=landing;
        cur_v=land_v;
      }
    } break;
    case landing:{
      cur_v-=land_a*deltatime;
      land_s+=cur_v*deltatime;
      if (cur_v<0 || land_s>type->land_len+type->lower_len) {
        state=waiting;
        startairport=landairport;
        landairport=NULL;
        event("destreached");
      }
    } break;
  }
}

bool Aircraft::countpos(fvector3d *pos,float *alfa,float *along) {
  switch (state) {
    case starting:{
      startairport->countpos(start_s,pos,alfa);
      if (along) {
        if (start_s>type->start_len-type->start_angling)
          *along=(start_s-(type->start_len-type->start_angling))/type->start_angling*type->rise_angle;
      }
      return true;
    }
    case rising:{
      startairport->countpos(start_s,pos,alfa);
      if (pos) pos->z=rise_h0+rise_dh;
      if (along) {
        if (start_s>type->start_len+type->rise_len-type->start_angling)
          *along=(type->start_len+type->rise_len-start_s)/type->start_angling*type->rise_angle;
        else *along=type->rise_angle;
      }
      return true;
    }
    case flying:{
      if (pos) {
        pos->z=cur_pos.z;
        fpoint xy=traj.pos(fly_t);
        pos->x=xy.x;
        pos->y=xy.y;
      }
      if (alfa) {
        fvector diff=traj.diff(fly_t);
        *alfa=atan2(diff.y,diff.x)*180/M_PI;
      }
      return true;
    }
    case lowering:{
      landairport->countpos(landairport->runwaylen()-type->lower_len-type->land_len+land_s,pos,alfa);
      if (pos) pos->z=rise_h0-rise_dh;
      if (along) {
        if (land_s<type->land_angling)
          *along=land_s/type->land_angling*type->lower_angle;
        else *along=type->lower_angle;
      }
      return true;
    }
    case landing:{
      landairport->countpos(landairport->runwaylen()-type->lower_len-type->land_len+land_s,pos,alfa);
      if (along) {
        if (land_s<type->lower_len+type->land_angling)
          *along=(type->lower_len+type->land_angling-land_s)/type->land_angling*type->lower_angle;
      }
      return true;
    }
  }
  return false;
}

void Aircraft::render(RenderContext &rc) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  fvector3d pos;
  float alfa=0,along=0;
  countpos(&pos,&alfa,&along);
  glTranslatef(pos.x,pos.z,pos.y);
  glRotatef(180-alfa,0,1,0);
  glRotatef(-along,0,0,1);

  type->render(rc);
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

bool Aircraft::position(fvector3d *res,fvector3d *diff) {
  float alfa=0;
  return countpos(res,&alfa,NULL);
}

struct ALineItem:ATrajItem {
  fpoint p;
  fvector d;
  ALineItem(fpoint p,fvector d):p(p),d(d) {len=d.fsize();}
  fpoint pos(float t) {return p+d*(t/len);}
  fvector diff(float t) {return d.size1();}
  void save(XMLNode xml) {
    ATrajItem::save(xml);
    xml.setname("line");
    xml.setvar("p.x",p.x);xml.setvar("p.y",p.y);
    xml.setvar("d.x",d.x);xml.setvar("d.y",d.y);
  }
  ALineItem(XMLNode xml):ATrajItem(xml) {
    p.x=xml.fvar("p.x");p.y=xml.fvar("p.y");
    d.x=xml.fvar("d.x");d.y=xml.fvar("d.y");
  }
};

struct AArcItem:ATrajItem {
  fpoint p;
  float rad;
  float a1,a2;
  AArcItem(fpoint p,float rad,float a1,float a2):p(p),rad(rad),a1(a1),a2(a2) {
    repair();
    len=rad*(this->a2-this->a1);
  }
  AArcItem(XMLNode xml):ATrajItem(xml) {
    p.x=xml.fvar("p.x");p.y=xml.fvar("p.y");
    rad=xml.fvar("rad");a1=xml.fvar("a1");a2=xml.fvar("a2");
  }
  void save(XMLNode xml) {
    ATrajItem::save(xml);
    xml.setname("arc");
    xml.setvar("p.x",p.x);
    xml.setvar("p.y",p.y);
    xml.setvar("rad",rad);
    xml.setvar("a1",a1);
    xml.setvar("a2",a2);
  }
  void repair() {
    if (fabs(fabs(a2-a1)-2*M_PI)<0.001) return;
    while (a1>2*M_PI) a1-=2*M_PI;
    while (a1<0) a1+=2*M_PI;
    while (a2<a1) a2+=2*M_PI;
    while (a2-a1>2*M_PI) a2-=2*M_PI;
  }
  float recountt(float t) {
    t/=len;
    t*=(a2-a1);
    t+=a1;
    return t;
  }
  fpoint pos(float t) {
    t=recountt(t);
    return p+fvector(rad*cos(t),rad*sin(t));
  }
  fvector diff(float t) {
    t=recountt(t);
    return fvector(-sin(t),cos(t));
  }
};


/*
bool findtrajeq(fpoint s1,fpoint s2,float r,ATrajectory &traj) {//najde trajektorii ze seuhlasne orientovanych kruznic

}
*/

float ATrajectory::len() {
  float res=0;
  for(int i=0;i<items.count;i++) res+=items[i]->len;
  return res;
}
fpoint ATrajectory::pos(float t) {
  float t0=0;
  if (ATrajItem *it=finditem(t,&t0)) return it->pos(t-t0);
  return fpoint(0,0);
}
fvector ATrajectory::diff(float t) {
  float t0=0;
  if (ATrajItem *it=finditem(t,&t0)) return it->diff(t-t0);
  return fvector(1,0);
}
ATrajItem *ATrajectory::finditem(float t,float *t0) {
  float act=0;
  for(int i=0;i<items.count;act+=items[i]->len,i++) if (t>=act && t<=act+items[i]->len) {
    if (t0) *t0=act;
    return items[i];
  }
  if (items.count>0) {
    for(int i=0;i<items.count;act+=items[i]->len,i++) if (t>=act-0.01 && t<=act+items[i]->len+0.01) {
      if (t0) *t0=act;
      return items[i];
    }
  }
  return NULL;
}

void ATrajectory::addline(fpoint p,fvector d) {
  add(new ALineItem(p,d));
}
void ATrajectory::addarc(fpoint p,float rad,float a1,float a2) {
  add(new AArcItem(p,rad,a1,a2));
}

void ATrajectory::count(fpoint p1,float a1,fpoint p2,float a2,float r) {
  fvector d1(cos(a1),sin(a1)),d2(cos(a2),sin(a2));//smerove vektory
  fvector n1(-d1.y,d1.x),n2(-d2.y,d2.x);
  fpoint s1=p1+n1*r,s2=p2+n2*r;
  fvector line=s2-s1;
  if (line.fsize()<0.01) {//specialni zpracovani
    addarc(p1,r,a1+M_PI_2,a2+M_PI_2);
    return;
  }
  float d=atan2(line.y,line.x);
  fvector linen(line.y,-line.x);
  addarc(s1,r,a1-M_PI_2,d-M_PI_2);
  addline(s1+linen.size1()*r,line);
  addarc(s2,r,d-M_PI_2,a2-M_PI_2);
}

void ATrajectory::load(XMLNode xml) {
  for(int i=0;i<xml.count();i++) {
    XMLNode x=xml[i];
    if (x.hasname("line")) items.add(new ALineItem(x));
    if (x.hasname("arc")) items.add(new AArcItem(x));
  }
}
void ATrajectory::save(XMLNode xml) {
  for(int i=0;i<items.count;i++) items[i]->save(xml.add());
}


}

