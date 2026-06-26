#ifndef crookalgH
#define crookalgH

#include "polyedr.h"
#include "geom3d.h"

namespace jmath {

struct CrookAlg {
  prPolyedr pedr;
  TCntVec<fpoint> pline;//sesikmovaci polyline
  int mdir;//mdir:hlavni smer (delani esa:y)
  int cdir;//cdir-smer, v jakem se posunuje (eso:x)
  int sgn;//sgn:jestli zepredu nebo zezadu (0|1)
//  int odir;//odir:!=mdir && !=cdir

  float readm(fpoint3d pt);
  float readc(fpoint3d pt);

  float readm(int vrid) {return readm(pedr[vrid]);}
  float readc(int vrid) {return readc(pedr[vrid]);}

  void writec(int vrid,float c) {writec(pedr[vrid],c);}
  void writec(fpoint3d &pt,float c);

  CrookAlg() {mdir=cdir=sgn=-1;}

  void dividefaces();
  void crookfaces();

  void loadopts(string s);//s ve tveru:mdir,cdir,sgn|x1,y1;x2,y2;..;xn,yn
  void loadpedr(XMLNode xml) {pedr.load(xml);}
  void loadpolyline(XMLNode xml);
  void save(XMLNode xml) {pedr.save(xml);}
  void run();
  bool checkinput();
};

}

#endif
