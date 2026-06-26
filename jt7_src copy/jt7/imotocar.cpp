#include <jlib.h>
#pragma hdrstop

#include "imotocar.h"
#include "icab.h"
#include "gltools.h"
#include <jmath/geom3d.cpp>

namespace jtyc {

const float epsilon=0.0001;
float CSF=0.08;//carsizefactor

void addinterval(interval *&set,float a,float b) {
  if (a>=b) return;
  interval **act=&set;
  #define r (*act)
  if (!r) {
    r=new interval(a,b,NULL);
    return;
  }
  while (r->next && r->b<a) act=&r->next;//intervaly jsou vlevo od (a,b)
  if (!r->next) {//r je posledni interval
    if (a>r->b) {//pridany interval bude posledni
      r->next=new interval(a,b,NULL);
      return;
    }
    if (b<r->a) {//pridany interval bude predposledni
      r=new interval(a,b,r);
    }
    //rozsireni intervalu
    if (a<r->a) r->a=a;
    if (b>r->b) r->b=b;
    return;
  }

  //r neni posledni interval, ale neprolina se s predchozimi useky

  if (b<r->a) {//vlozeni intervalu pred aktualni, za aktualni se nemusi osetrovat
    r=new interval(a,b,r);
    return;
  }

  if (a<r->a) r->a=a;
  if (r->b>b) return;//hotovo:(a,b) se vejde do rozsireneho aktualniho

  while (r->next) {
    if (r->next->b<=b) {
      interval *bak=r->next;
      r->next=r->next->next;
      delete bak;
    } else break;
  }

  if (!r->next) {
    r->b=b;
    return;//hotovo, dalsi interval neni
  }

  if (r->next->a>b) {
    r->b=b;
    return;//hotovo, dalsi interval je disjunkni s <a,b>
  }
  r->b=r->next->b;
  if (b>r->b) r->b=b;
  delete r->next;
  r->next=NULL;

  #undef r
}

bool MT_PasCommon::load(XMLNode xml,gloModel *model,PersistentObject *data) {
  if (gloPolyObject *poly=dynamic_cast<gloPolyObject*>((gloObject*)model->obj)) {
    interval *list=NULL;
    for(int i=0;i<poly->childs.count;i++) if (gloPolygon *pg=dynamic_cast<gloPolygon*>(poly->childs[i])) {
      if (pg->verteces.count>0) {
        float minx=pg->verteces[0]->position.x,maxx=minx;
        for(int j=1;j<pg->verteces.count;j++) {
          gloVertex *v=pg->verteces[j];
          float x=v->position.x;
          if (x>maxx) maxx=x;if (x<minx) minx=x;
        }
        //pridani <minx,maxx> do seznamu
        addinterval(list,minx,maxx);
      }
    }
    return dopasload(xml,model,data,poly,list);
  }
  return false;
}


void MD_PasData::load(XMLNode xml,PersistentObject *data,float beg,float end) {
  zdec=texman->findtexture("pack:"+xml.svar("zdecfile"),data);
  zinc=texman->findtexture("pack:"+xml.svar("zincfile"),data);
  ydec=texman->findtexture("pack:"+xml.svar("ydecfile"),data);

  zd_t1=xml.fvar("zd_t1");zd_t2=xml.fvar("zd_t2");zd_s1=xml.fvar("zd_s1");zd_s2=xml.fvar("zd_s2");
  zi_t1=xml.fvar("zi_t1");zi_t2=xml.fvar("zi_t2");zi_s1=xml.fvar("zi_s1");zi_s2=xml.fvar("zi_s2");
  yd_t1=xml.fvar("yd_t1");yd_t2=xml.fvar("yd_t2");yd_s1=xml.fvar("yd_s1");yd_s2=xml.fvar("yd_s2");

  float pwi=xml.fvar("pas_wi"),py1=xml.fvar("pas_y1"),py2=xml.fvar("pas_y2");
  pas_wi=pwi*CSF;
  pas_y1=py1*CSF;
  pas_y2=py2*CSF;

  if (xml.ivar("zd_rel")) {
    if (float tsize=zd_t2-zd_t1) {
      zd_t2=(end-zd_t1)/tsize;
      zd_t1=(beg-zd_t1)/tsize;
    }

    if (float ssize=zd_s2-zd_s1) {
      zd_s2=(py2-zd_s1)/ssize;
      zd_s1=(py1-zd_s1)/ssize;
    }
  }

  if (xml.ivar("zi_rel")) {
    if (float tsize=zi_t2-zi_t1) {
      zi_t2=(end-zi_t1)/tsize;
      zi_t1=(beg-zi_t1)/tsize;
    }

    if (float ssize=zi_s2-zi_s1) {
      zi_s2=(py2-zi_s1)/ssize;
      zi_s1=(py1-zi_s1)/ssize;
    }
  }

  if (xml.ivar("yd_rel")) {
    if (float tsize=yd_t2-yd_t1) {
      yd_t2=(end-yd_t1)/tsize;
      yd_t1=(beg-yd_t1)/tsize;
    }

    if (float ssize=yd_s2-yd_s1) {
      yd_s2=(-pwi/2-yd_s1)/ssize;
      yd_s1=(+pwi/2-yd_s1)/ssize;
    }
  }
}

void MT_Bus2c::makehead() {
  gloModel *model=head;

  Extrems e;
  model->getextrems(e);
  float len=e.xmax-e.xmin;
  len*=CSF;

  head_d1=jlib::min(len/4.0,0.1);
  head_d2=len-head_d1;

  model->translate(e.xmin-head_d1/CSF,0,0);
  model->scale(CSF,CSF,CSF);
}

void MT_Bus2c::maketail(float tailx) {
  gloModel *model=tail;
  tail_d1=headlen+spacelen/2;
  tail_d2=headlen+spacelen+4*taillen/5;

  model->translate(-tailx,0,0);
  model->scale(CSF,CSF,CSF);
}

bool MT_Bus2c::dopasload(XMLNode xml,gloModel *model,PersistentObject *data,gloPolyObject *poly,interval *list) {
  if (list && list->next && !list->next->next) {//dva nespojite useky
    gloPolyObject *o1=new gloPolyObject(),*o2=new gloPolyObject();
    for(int i=0;i<poly->childs.count;i++) if (gloPolygon *pg=dynamic_cast<gloPolygon*>(poly->childs[i])) {
      if (pg->verteces.count>0) {
        gloVertex *v=pg->verteces[0];
        float x=v->position.x;
        if (x<=list->b) o1->childs.add(pg->clone());
        else o2->childs.add(pg->clone());
      }
    }
    headlen=list->b-list->a;
    spacelen=list->next->a-list->b;
    taillen=list->next->b-list->next->a;
    headlen*=CSF;spacelen*=CSF;taillen*=CSF;
    head=new gloModel();head->obj=o1;
    tail=new gloModel();tail->obj=o2;
    makehead();
    maketail(list->next->a);
    mcar->bound=model->bound();
    mcar->bound.scale(CSF,CSF,CSF);

    float beg=list->b,end=list->next->a;
    pas_begx=beg*CSF,pas_endx=end*CSF;

    pas.load(xml,data,beg,end);

    return true;
  }
  return false;
}

void MT_PasTram::makemember(gloModel *model,float &d1,float &d2) {
  Extrems e;
  e.xmin=0x100;
  model->getextrems(e);
  float len=e.xmax-e.xmin;
  len*=CSF;

  d1=jlib::min(e.xmin*CSF+len/4.0,e.xmin*CSF+0.1);
  d2=e.xmax*CSF-jlib::min(0.1,len/4.0);

  model->translate(-d1/CSF,0,0);
  model->scale(CSF,CSF,CSF);
}

bool extract_coord(gloObject *obj,float *x_coord) {
  if (gloPolygon *pg=dynamic_cast<gloPolygon*>(obj)) {
    if (pg->verteces.count>0) {
      gloVertex *v=pg->verteces[0];
      *x_coord=v->position.x;
      return true;
    }
    return false;
  }
  if (gloInline *il=dynamic_cast<gloInline*>(obj)) {
    *x_coord=il->tx;
    return true;
  }
  return false;
}

bool MT_Tram2c::dopasload(XMLNode xml,gloModel *model,PersistentObject *data,gloPolyObject *poly,interval *list) {
  if (list && list->next && !list->next->next) {//dva nespojite useky
    gloPolyObject *o1=new gloPolyObject(),*o2=new gloPolyObject();
    for(int i=0;i<poly->childs.count;i++) {
      float x=0;
      if (extract_coord(poly->childs[i],&x)) {
        gloObject *clone=poly->childs[i]->clone();
        if (clone) {
          if (x<=list->b) o1->childs.add(clone);
          else o2->childs.add(clone);
        }
      }
    }
    headlen=list->b-list->a;
    spacelen=list->next->a-list->b;
    taillen=list->next->b-list->next->a;
    headlen*=CSF;spacelen*=CSF;taillen*=CSF;
    head=new gloModel();head->obj=o1;
    tail=new gloModel();tail->obj=o2;
    makemember(head,head_d1,head_d2);
    makemember(tail,tail_d1,tail_d2);
    mcar->bound=model->bound();
    mcar->bound.scale(CSF,CSF,CSF);

    float beg=list->b,end=list->next->a;
    pas_begx=beg*CSF,pas_endx=end*CSF;

    pas.load(xml,data,beg,end);

    return true;
  }
  return false;
}

bool MT_Tram3c::dopasload(XMLNode xml,gloModel *model,PersistentObject *data,gloPolyObject *poly,interval *list) {
  if (list && list->next && list->next->next && !list->next->next->next) {//tri nespojite useky
    beg1=list->a;end1=list->b;
    beg2=list->next->a;end2=list->next->b;
    beg3=list->next->next->a;end3=list->next->next->b;
    gloPolyObject *o1=new gloPolyObject(),*o2=new gloPolyObject(),*o3=new gloPolyObject();

    for(int i=0;i<poly->childs.count;i++) {
      float x=0;
      if (extract_coord(poly->childs[i],&x)) {
        gloObject *clone=poly->childs[i]->clone();
        if (clone) {
          if (x<=end1) o1->childs.add(clone);
          else if (x<=end2) o2->childs.add(clone);
          else o3->childs.add(clone);
        }
      }
    }
/*
    for(int i=0;i<poly->childs.count;i++) if (gloPolygon *pg=dynamic_cast<gloPolygon*>(poly->childs[i])) {
      if (pg->verteces.count>0) {
        gloVertex *v=pg->verteces[0];
        float x=v->position.x;
        if (x<=end1) o1->childs.add(pg->clone());
        else if (x<=end2) o2->childs.add(pg->clone());
        else o3->childs.add(pg->clone());
      }
    }
*/
    mem1=end1-beg1;spa1=beg2-end1;
    mem2=end2-beg2;spa2=beg3-end2;
    mem3=end3-beg3;

    mem1*=CSF;mem2*=CSF;mem3*=CSF;
    spa1*=CSF;spa2*=CSF;

    m1=new gloModel();m1->obj=o1;
    m2=new gloModel();m2->obj=o2;
    m3=new gloModel();m3->obj=o3;

    makemember(m1,m1_d1,m1_d2);
    makemember(m2,m2_d1,m2_d2);
    makemember(m3,m3_d1,m3_d2);

    mcar->bound=model->bound();
    mcar->bound.scale(CSF,CSF,CSF);


    pas1.load(xml,data,end1,beg2);
    pas2.load(xml,data,end2,beg3);

    beg1*=CSF;end1*=CSF;
    beg2*=CSF;end2*=CSF;
    beg3*=CSF;end3*=CSF;

    return true;
  }
  return false;
}

void jtMotocar::load(PersistentObject *data,XMLNode xml) {
  isdefault=xml.ivar("default");
  roadstyle=xml.svar("roadstyle");

  string style=xml.svar("style");

  name=xml.svar("name");
  path=xml.svar("path");
  title=lang_text_ch(xml.svar("title"));
  power=xml.fvar("power");
  maxspeed=xml.fvar("maxspeed");
  weight=xml.fvar("weight");

  gloModel *model=jt_loadmodel(data,"model.zip");

  if (style=="bus2c") obj=new MT_Bus2c(this);
  if (style=="tram2v") obj=new MT_Tram2v(this);
  if (style=="tram2c") obj=new MT_Tram2c(this);
  if (style=="tram3c") obj=new MT_Tram3c(this);
  if (!obj) obj=new MT_Solid(this);
  if (!obj->load(xml,model,data)) {
    obj=new MT_Solid(this);
    obj->load(xml,model,data);
  }

  if (xml.svar("cab")!="") {
    PersistentObject *child=data->openchild(xml.svar("cab"));
    cab=new jtCab();
    MainXMLNode cx;
    child->loadxml("index.xml",cx);
    cab->load(child,cx);
    owncab=true;
  }
}

bool MT_Solid::load(XMLNode xml,gloModel *amodel,PersistentObject *data) {
  model=amodel;
  Extrems e;
  model->getextrems(e);
  len=e.xmax-e.xmin;
  len*=CSF;

  d1=jlib::min(len/4.0,0.1);
  d2=len-d1;

  model->translate(e.xmin-d1/CSF,0,0);
  model->scale(CSF,CSF,CSF);
  mcar->bound=model->bound();
  return true;
}

void MT_Solid::render(RenderContext &rc,AbstractTrajectory &traj) {
  point3d<float> a,b;
  a=traj.at_count_pos(d1);
  b=traj.at_count_pos(d2);

  fvector3d e1=b-a;
  if (e1.size()<epsilon) return;
  fvector3d e2(xyzrec<float>(agvector<float>(e1.xy()).getnormal(),0));

  e2=e2.size1();
  e1=e1.size1();
  vector3d<float> e3=(e1%e2).size1();
  matrix3d<float> mat;
  mat.tobase(e1,e2,e3);//a je matice prechodu od konanicke baze k bazi aktualniho natoceni

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  glTranslatef(a.x,a.z,a.y);
  glMultiplySwapYZ(mat);

  model->render(rc);
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}



void MT_Tram2v::render(RenderContext &rc,AbstractTrajectory &traj) {
  solid->render(rc,traj);
  TrajectoryTranslation tt(traj,solid->len+space);
  solid->render(rc,tt);
}

bool MT_Tram2v::load(XMLNode xml,gloModel *model,PersistentObject *data) {
  space=xml.fvar("tramspace");
  solid=new MT_Solid(mcar);
  solid->load(xml,model,data);
  return true;
}

static void renderpt(const fpoint3d &pt) {
  glVertex3f(pt.x,pt.z,pt.y);
}

void MT_Common::render_solid(RenderContext &rc,gloModel *model,fpoint3d a,fpoint3d b,transform3d<float> &tr) {
  fvector3d e1=b-a;
  if (e1.size()<epsilon) return;
  fvector3d e2(xyzrec<float>(agvector<float>(e1.xy()).getnormal(),0));

  e2=e2.size1();
  e1=e1.size1();
  vector3d<float> e3=(e1%e2).size1();
  matrix3d<float> mat;
  mat.tobase(e1,e2,e3);//a je matice prechodu od konanicke baze k bazi aktualniho natoceni

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  glTranslatef(a.x,a.z,a.y);
  glMultiplySwapYZ(mat);

  model->render(rc);
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  tr.m=mat;tr.b=a;
}


void MT_Tram2c::render(RenderContext &rc,AbstractTrajectory &traj) {
  fpoint3d a,b;
  a=traj.at_count_pos(head_d1);
  b=traj.at_count_pos(head_d2);

  fpoint3d c,d;
  c=traj.at_count_pos(tail_d1);
  d=traj.at_count_pos(tail_d2);

  transform3d<float> t1,t2;

  render_solid(rc,head,a,b,t1);
  render_solid(rc,tail,c,d,t2);

  pas.render(rc,t1,t2,pas_begx-head_d1,pas_endx-tail_d1);
}

void MT_Tram3c::render(RenderContext &rc,AbstractTrajectory &traj) {
  fpoint3d a,b;
  a=traj.at_count_pos(m1_d1);
  b=traj.at_count_pos(m1_d2);

  fpoint3d c,d;
  c=traj.at_count_pos(m2_d1);
  d=traj.at_count_pos(m2_d2);

  fpoint3d e,f;
  e=traj.at_count_pos(m3_d1);
  f=traj.at_count_pos(m3_d2);

  transform3d<float> t1,t2,t3;

  render_solid(rc,m1,a,b,t1);
  render_solid(rc,m2,c,d,t2);
  render_solid(rc,m3,e,f,t3);

  pas1.render(rc,t1,t2,end1-m1_d1,beg2-m2_d1);
  pas2.render(rc,t2,t3,end2-m2_d1,beg3-m3_d1);
}

void MT_Bus2c::render(RenderContext &rc,AbstractTrajectory &traj) {
  fpoint3d a,b;
  a=traj.at_count_pos(head_d1);
  b=traj.at_count_pos(head_d2);
  fvector3d e1=b-a;
  if (e1.size()<epsilon) return;
  e1=e1.size1();

  fpoint3d d,c1,c2,c;
  c1=a+e1*(tail_d1);//bod uprostred pasu
  c2=traj.at_count_pos(headlen+spacelen);
  c=((fvector3d)c1+(fvector3d)c2)/2;
  d=traj.at_count_pos(tail_d2);

  transform3d<float> t1,t2;

  render_solid(rc,head,a,b,t1);
  render_solid(rc,tail,c,d,t2);

  pas.render(rc,t1,t2,pas_begx-head_d1,0);
}

void MD_PasData::render(RenderContext &rc,const transform3d<float> &t1,const transform3d<float> &t2,float beg1,float end2) {
  rc.push();
  glColor4f(1,1,1,1);
  rc.setculling(false);
  rc.settexture2d(true);
  rc.setlighting(false);
  //t1==transformace prvni casti autobusu, t2 druhe

  ydec->bind(rc.ctxid);
  glBegin(GL_QUADS);
  glTexCoord2f(yd_t1,yd_s1);renderpt(t1*fpoint3d(beg1,-pas_wi/2,pas_y2));
  glTexCoord2f(yd_t1,yd_s2);renderpt(t1*fpoint3d(beg1,+pas_wi/2,pas_y2));
  glTexCoord2f(yd_t2,yd_s2);renderpt(t2*fpoint3d(end2,+pas_wi/2,pas_y2));
  glTexCoord2f(yd_t2,yd_s1);renderpt(t2*fpoint3d(end2,-pas_wi/2,pas_y2));
  glEnd();

  zdec->bind(rc.ctxid);
  glBegin(GL_QUADS);
  glTexCoord2f(zd_t1,zd_s1);renderpt(t1*fpoint3d(beg1,-pas_wi/2,pas_y1));
  glTexCoord2f(zd_t1,zd_s2);renderpt(t1*fpoint3d(beg1,-pas_wi/2,pas_y2));
  glTexCoord2f(zd_t2,zd_s2);renderpt(t2*fpoint3d(end2,-pas_wi/2,pas_y2));
  glTexCoord2f(zd_t2,zd_s1);renderpt(t2*fpoint3d(end2,-pas_wi/2,pas_y1));
  glEnd();

  zinc->bind(rc.ctxid);
  glBegin(GL_QUADS);
  glTexCoord2f(zi_t1,zi_s1);renderpt(t1*fpoint3d(beg1,pas_wi/2,pas_y1));
  glTexCoord2f(zi_t1,zi_s2);renderpt(t1*fpoint3d(beg1,pas_wi/2,pas_y2));
  glTexCoord2f(zi_t2,zi_s2);renderpt(t2*fpoint3d(end2,pas_wi/2,pas_y2));
  glTexCoord2f(zi_t2,zi_s1);renderpt(t2*fpoint3d(end2,pas_wi/2,pas_y1));
  glEnd();

  rc.pop();
}

void jtMotocar::render(RenderContext &rc,AbstractTrajectory &traj) {
  obj->render(rc,traj);
}

jtMotocar::~jtMotocar() {
  if (owncab) delete cab;
}



}

