#ifndef polyedrH
#define polyedrH

#include "geom3d.h"
#include <ext/xmlnode.h>

namespace jmath {

class prPolyedr;

struct prVertex:fpoint3d {
  prVertex(XMLNode xml) {x=xml.fvar("x");y=xml.fvar("y");z=xml.fvar("z");}
  prVertex(fpoint3d pt):fpoint3d(pt) {}
};

struct prPoint {
  int link;
  float t1,t2;
  prPoint(int alink=-1) {t1=t2=0;link=alink;}
  operator int() {return link;}
  void load(XMLNode xml);
  void save(XMLNode xml);
  prPoint *clone() {prPoint *res=new prPoint(link);res->t1=t1;res->t2=t2;return res;}
  prPoint(XMLNode xml) {load(xml);}
};

struct prFace {
  TCntVec<prPoint> vrs;
  string faceid;
  string texture;
  fvector3d n;
  bool culling;
  prFace(XMLNode xml);
  prFace() {culling=false;}
  void savepoly(prPolyedr *pedr,XMLNode xml);
  void save(XMLNode xml);
  void normalize_culling(prPolyedr *pedr);
  prFace *clone();
  void add(Array<int> &vrs) {for(int i=0;i<vrs.count;i++) this->vrs.add(vrs[i]);}
};

struct prPolyedr {
  TCntVec<prVertex> vrs;
  IObjArray<prFace> faces;
  void load(XMLNode xml);
  void savepolys(XMLNode xml);
  void save(XMLNode xml);
  prVertex &operator [] (int index) {return vrs[index];}
  int addv(fpoint3d pt,float epsilon);//vrati id bodu
  void normalize_culling();
};

}

#endif
