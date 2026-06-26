#ifndef gviewH
#define gviewH

#include "game0.h"
#include "gviewtype.h"

namespace jtyc {

struct ProjectiveMatrix {
  float p[4][4];
  ProjectiveMatrix() {identity();}
  void identity();
  void translate(float x,float y,float z);
  void multiply(float res[4],float vec[4]);
  void perspective(float fovy,float aspect,float znear,float zfar);
  void frustum(float left,float right,float bottom,float top,float near,float far);
  void rotate(float angle,float x,float y,float z);
  void multiply(ProjectiveMatrix &a,const ProjectiveMatrix &b);
};

struct TrainDriveViewType:ViewType {
  fvector see;
  fpoint pos;
  float posz;
  float maxdist;
  float fovy;

  bool canapply;
  fvector3d perpos,perdif;
  fpoint3d cla,clb,clc,cld,clv;//orezavaci hehlan
  fcomplane clbase,clab,clbc,clcd,clda;
  float aspect,near,far,angle;
  float cabhi;
  ProjectiveMatrix mat;
  bool countprojmatrix();
  bool applyprojmatrix();
  TrainDriveViewType() {maxdist=fovy=0;}
  ZPoint ltop(const fpoint3d &pt);
  void project(float in[3],float out[2]);
  void clear();
  void iterate(unsigned char *keystate,double deltatime);
  int visiblecube(float coords[3],float size);
  string name() {return "drive";}
};

struct View {
  int pixw,pixh;
  float scrmousepx,scrmousepy;//-1..1, souradnice kurzoru mysi vuci obrazovce
  float vpmousepx,vpmousepy;//-1..1, souradnice kurzoru mysi vuci vyrezu
  int mousesqx,mousesqy;//ctverecek, na kterem je mys
  int mousevx,mousevy;//vrchol, na kterem je mys
  bool mouseisinmiddle;
  float ptcsize2x,ptcsize2y;//polovina velikosti ctvereckoveho kurzoru

  ZPoint ltop(const fpoint3d &pt) {return type->ltop(pt);}

  void resize(int w,int h);
  void iterate(double deltatime,int mx,int my);
  void render(RenderContext &rc);
  void resized();
//  bool nearvisible(const ZPoint &pt,float maxdist);

  void drawcircle(float x,float y,float xrad,float yrad,gcolor color);
  void drawmapcircle(float x,float y,float xrad,float yrad,gcolor color);

  void drawsqcursor(int x,int y,string type,int dir);
  void drawsqcursors(int x1,int y1,int x2,int y2,string type,int dir);
  void drawsqcursor(int x,int y,gcolor color);
  void drawsq2cursor(int x,int y,gcolor color);
  void drawsqlinecursor(int x,int y,bool vertical,gcolor framecolor,gcolor linecolor);
  void drawptcursor(int x,int y,gcolor color);
  void drawptcursors(int x1,int y1,int x2,int y2,gcolor color);
  void lookat(fpoint3d pt);

  void init();

  void load(Archive &ar);
  void save(Archive &ar);
  void xmlsave(XMLNode xml);
  void xmlload(XMLNode xml);
  void keydown(int key);
  void keyup(int key);

  View();
  ~View();

  ViewType *type;

  int getdirection() {return direction;}
  float getpower() {return speedc/5.0;}
  float getviewangle() {return maptype.viewangle;}

  OctTree *ot;

  MapViewType maptype;
  TrainDriveViewType traindrivetype;
  CartViewType carttype;

  void setcab(jtCab *value);
  jtCab *getcab() {return cab;}

  int getvpx();
  int getvpy();
  int getvpw();
  int getvph();

  void setmapview();
  void setdriveview();

  string gettype() {return type->name();}
  void settype(string value);

  RenderContext *customrc;
private:

  //vrati, zda f a g jsou ve stejne polorovine orcene primkou a,b(vse v 2D)
  bool isineq(const ZPoint &a,const ZPoint &b,const ZPoint &f,const ZPoint &g);
  bool mousein(const ZPoint &a,const ZPoint &b,const ZPoint &c,const ZPoint &d);
  void getmousesq(int x0,int y0);
  void getmousev(int x0,int y0);
  bool visiblesq(int x,int y);
  fpoint3d sget(int x,int y);
  int direction;
  int speedc;
  double lastkeytime;

  jtCab *cab;
  int lastmousesqx,lastmousesqy;

  TCntVec<otItem> visibleitems;
};

extern float perspective_maxdist();

}

#endif
