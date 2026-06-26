#ifndef ghiviewH
#define ghiviewH

#include "game0.h"

namespace jtyc {

struct HVFace {
  fvector3d n;
  int x,y;
  bool octvisible;
  int tunneldir;
  float tshor;
  float tsver;
  float tunneldhi;

  jtTerrain
  *ed,  *ec,
  *ea,  *eb;//edges, counted by countedges()

  jtTerrain *terrain;
  void settunnel(bool less,bool vertical,float dhi);

  HVFace() {x=y=-1;octvisible=false;tshor=tsver=tunneldhi=0;tunneldir=-1;ea=eb=ec=ed=NULL;terrain=NULL;}
};

struct HVVertex {
  float hi;
  float waterhi;
  float intensity;
  int x,y;
  fvector3d n;
  ZPoint scrpt;
  HVVertex() {hi=0;waterhi=-1;x=y=-1;intensity=1;}
  void countintensity(const fvector3d &L);
  bool iswater() {return waterhi>=hi;}
};

struct HiView {
  TMatrix<HVFace> faces;
  TMatrix<HVVertex> verteces;
  int x0,y0;
  int w,h;//pocet faces
  int boundsqx1,boundsqy1,boundsqx2,boundsqy2;
  int boundvx1,boundvy1,boundvx2,boundvy2;

  HVVertex &vat(int x,int y) {return verteces(x-x0,y-y0);}
  HVFace *tryface(int x,int y) {return faces.tryat(x-x0,y-y0);}

  HiView();
  void init();
  void beforeiterate();
  void render(RenderContext &rc);
  bool drawwaterface(int x,int y);
};

}

#endif
