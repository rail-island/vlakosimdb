#include <jlib.h>
#pragma hdrstop

#include "polyedr.h"

namespace jmath {

int prPolyedr::addv(fpoint3d pt,float epsilon) {
  for(int i=0;i<vrs.count;i++) {
    fpoint3d v=vrs[i];
    float dx=v.x-pt.x;
    float dy=v.y-pt.y;
    float dz=v.z-pt.z;
    if (sqrt(dx*dx+dy*dy+dz*dz)<epsilon) return i;
  }
  vrs.add(pt);
  return vrs.count-1;
}

void prPolyedr::load(XMLNode xml) {
  XMLNode v=xml["verteces"];
  for(int i=0;i<v.count();i++) {
    vrs.add(prVertex(v[i]));
  }
  XMLNode f=xml["faces"];
  for(int i=0;i<f.count();i++) {
    faces.add(new prFace(f[i]));
  }
  normalize_culling();
}

void prPolyedr::normalize_culling() {
  for(int i=0;i<faces.count;i++) faces[i]->normalize_culling(this);
}

void prFace::normalize_culling(prPolyedr *pedr) {
  fvector3d normala;
  float ncsize=-1;
  for(int i=0;i<vrs.count;i++) {
    fpoint3d x=pedr->vrs[vrs[(i-1+vrs.count)%vrs.count]];
    fpoint3d y=pedr->vrs[vrs[(i+vrs.count)%vrs.count]];
    fpoint3d z=pedr->vrs[vrs[(i+1+vrs.count)%vrs.count]];
    fvector3d a=x-y,b=z-y;
    float actsize=fabs(a*b);
    if (ncsize<0 || actsize<ncsize) {
      normala.x=a.y*b.z-a.z*b.y;
      normala.y=a.z*b.x-a.x*b.z;
      normala.z=a.x*b.y-a.y*b.x;
      ncsize=actsize;
    }
  }
  float mul=normala*n;
  if (mul<0) vrs.reverse();
}

void prPoint::load(XMLNode xml) {
  link=xml.ivar("link");
  t1=xml.fvar("t1");
  t2=xml.fvar("t2");
}
void prPoint::save(XMLNode xml) {
  xml.setvar("link",link);
  xml.setvar("t1",t1);
  xml.setvar("t2",t2);
}

prFace *prFace::clone() {
  prFace *res=new prFace();
  res->n=n;
  res->vrs.assign(vrs);
//  for(int i=0;i<vrs.count;i++) res->vrs.add(vrs[i]->clone());
  res->culling=culling;
  return res;
}

prFace::prFace(XMLNode xml) {
  culling=xml.ivar("culling");
  n.x=xml.fvar("nx");
  n.y=xml.fvar("ny");
  n.z=xml.fvar("nz");
  faceid=xml.svar("faceid");
  texture=xml.svar("texture");

  XMLNode vs=xml["verteces"];
  for(int i=0;i<vs.count();i++) vrs.add(vs[i]);
//  for(lineiter it=xml.svar("verteces").getlines(';');*it;it++) vrs.add(atoi(*it));
}

void prFace::savepoly(prPolyedr *pedr,XMLNode xml) {
  xml.setvar("nx",n.x);
  xml.setvar("ny",n.y);
  xml.setvar("nz",n.z);
  if (faceid!="") xml.setvar("faceid",faceid);
  if (texture!="") xml.setvar("texture",texture);
  for(int i=0;i<vrs.count;i++) {
    XMLNode pt=xml.add("vertex");
    pt.setvar("x",pedr->vrs[vrs[i]].x);
    pt.setvar("y",pedr->vrs[vrs[i]].y);
    pt.setvar("z",pedr->vrs[vrs[i]].z);
    pt.setvar("s",vrs[i].t1);
    pt.setvar("t",vrs[i].t2);
  }
  xml.setvar("culling",culling);
}

void prPolyedr::savepolys(XMLNode xml) {
  xml.clearsubs();
  for(int i=0;i<faces.count;i++) faces[i]->savepoly(this,xml.add("polygon"));
}

void prFace::save(XMLNode xml) {
//  bstring ids;
//  for(int j=0;j<vrs.count;j++) ids<<itos(vrs[j])<<";";
//  xml.setvar("verteces",ids);
  xml.setvar("nx",n.x);
  xml.setvar("ny",n.y);
  xml.setvar("nz",n.z);
  xml.setvar("culling",culling);
  if (faceid!="") xml.setvar("faceid",faceid);
  if (texture!="") xml.setvar("texture",texture);
  XMLNode vs=xml.forcesub("verteces");
  for(int i=0;i<vrs.count;i++) vrs[i].save(vs.add("vertex"));
}

void prPolyedr::save(XMLNode xml) {
  xml.clearsubs();

  XMLNode pts=xml.add("verteces");
  for(int i=0;i<vrs.count;i++) {
    XMLNode pt=pts.add("vertex");
    pt.setvar("x",vrs[i].x);
    pt.setvar("y",vrs[i].y);
    pt.setvar("z",vrs[i].z);
  }
  XMLNode fcs=xml.add("faces");
  for(int i=0;i<faces.count;i++) {
    faces[i]->save(fcs.add("face"));
  }
}


}



