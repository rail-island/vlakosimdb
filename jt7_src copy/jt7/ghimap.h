#ifndef ghimapH
#define ghimapH

#include "game0.h"
#include <com/dvslist.h>

namespace jtyc {

struct HMBorders {
  int x1,y1,x2,y2;
  void extend(int x,int y) {if (x<x1) x1=x;if (x>x2) x2=x;if (y<y1) y1=y;if (y>y2) y2=y;}
  void repair(int w,int h) {if (x1<0) x1=0;if (y1<0) y1=0;if (x2>=w) x2=w-1;if (y2>=h) y2=h-1;}
  HMBorders() {x1=y1=0x10000;x2=y2=-1;}
  HMBorders(int x1,int y1,int x2,int y2):x1(x1),y1(y1),x2(x2),y2(y2) {} 
};

struct HiBlocked:EBlocked {
  int x,y;
  HiBlocked(int ax,int ay) {x=ax;y=ay;}
  Volatile *createvolatile();
};

struct HiBlock:DVSElem {
  HiBlock *next;
  int x,y;
  HiBlock(int ax,int ay);
  ~HiBlock();
  virtual bool allowsethi(float hi)=0;
  virtual void makeinterval(Interval &i) {}
};

struct MaxHiBlock:HiBlock {//pro mosty
  float maxhi;
  MaxHiBlock(int x,int y,float amaxhi):HiBlock(x,y) {maxhi=amaxhi;}
  bool allowsethi(float hi) {return hi<=maxhi;}
  void makeinterval(Interval &i) {if (i.hi>maxhi) i.hi=maxhi;}
};
struct MinHiBlock:HiBlock {//pro tunely
  float minhi;
  MinHiBlock(int x,int y,float aminhi):HiBlock(x,y) {minhi=aminhi;}
  bool allowsethi(float hi) {return hi>=minhi;}
  void makeinterval(Interval &i) {if (i.lo<minhi) i.lo=minhi;}
};
struct FixedHiBlock:HiBlock {//pomocne-zjednodusuje praci
  float fixedhi;
  FixedHiBlock(int x,int y,float afixedhi):HiBlock(x,y) {fixedhi=afixedhi;}
  bool allowsethi(float hi) {return hi==fixedhi;}
  void makeinterval(Interval &i) {if (i.lo<fixedhi) i.lo=fixedhi;if (i.hi>fixedhi) i.hi=fixedhi;}
};

struct VertexStruct {
  float hi;
//  float intensity;
//  SmallVector3 n;
//  fvector3d n;
  short x,y;

  float waterhi;//vyska vodni hladiny, absolutne,-1:neni voda

  unsigned char floodfilled;//pomocna promenna pro floodfill
  bool final;//pomocne

  bool iswater() {return waterhi>=hi;}

//  ZPoint _scrpt;
//  bool _visible;

//  bool counted;
//  bool tmpbool;
  int blocked;


  ZPoint countscrpt();

//  void countvr();
//  void countintensity(const fvector3d &L);
//  ZPoint getscrpt() {if (!counted) countvr();return _scrpt;}
//  void movevr(float dx,float dy);
//  void uncount() {counted=false;} //visible=counted=false;}
  void sethi(float newhi);
  void changedwater() {if (hi>waterhi) waterhi=-1;}
  float maxhi() {return max(hi,waterhi);}

//  VertexStruct *nextdirty;
//  VertexStruct *tmpnext;
//  bool dirty;

//  VertexStruct() {hi=0;waterhi=-1;blocked=0;x=y=-1;intensity=1;final=false;nextdirty=NULL;dirty=false;_visible=false;counted=false;}
  VertexStruct() {hi=0;waterhi=-1;blocked=0;final=false;x=y=-1;floodfilled=0;}
};
typedef VertexStruct HItem;

/*
struct FaceFlags {
  int dirty:1;
  int octvisible:1;
  int broken:1;
  signed int tunneldir:4;
};
*/

struct FaceStruct {
//  SmallVector3 n;
//  fvector3d n;
//  FaceStruct *nextdirty;
//  bool dirty;
  bool octvisible;
  bool broken;
  otNode *octalized;
//  signed char tunneldir;//-1:neni tunel
//  SmallFloat1 tunneldhi;
//  SmallFloat1 tshor,tsver;//proster pro tunel horizontalne, vertikalne
  short x,y;

  FaceStruct *next_broken;
//  float tunneldhi;
//  float tshor,tsver;///proster pro tunel horizontalne, vertikalne

//  bool fullvisible;
//  bool partvisible;
//  bool visibleneightboor;
  ///less:zda je vchod do tunelu(jede se z mensich souradnic do vetsich)
//  void settunnel(bool less,bool vertical,float dhi);
//  void unsettunnel() {tunneldir=-1;}
  void setbroken();
//  FaceStruct() {tunneldir=-1;tshor=0.1;tsver=0.3;tunneldhi=0;octvisible=false;nextdirty=NULL;dirty=false;broken=false;next_broken=NULL;octalized=NULL;}
//  FaceStruct() {tunneldir=-1;tshor=0.1;tsver=0.3;tunneldhi=0;octvisible=false;broken=false;next_broken=NULL;octalized=NULL;}
  FaceStruct() {octvisible=false;broken=false;next_broken=NULL;octalized=NULL;x=y=-1;}
};

///abstraktni trida-vyplnovac, predava se do HiMap::floodfill, vyplni oblast ohranicenou isborder() funkci fill()
struct FloodFiller {
  FloodFiller() {hm=NULL;fillborder=false;}

  virtual bool isborder(VertexStruct &v)=0;
  virtual void fill(VertexStruct &v)=0;

//private:
  HiMap *hm;//vyppnuje fce floodfill
  bool fillborder;
  friend class HiMap;

  ///vraci, zda ma byt vyplneno (tedy zda neni okraj)
  bool operator () (int x,int y);
//  bool ncond(int x,int y);//bere jen podminku, nekouka na floodfilled, vraci negaci operatoru () (isborder)

  bool floodfilled(int x,int y);
};

struct FloodFillStack;
struct HiMap:GOTObject {
  TMatrix<VertexStruct> verteces;
  TMatrix<FaceStruct> faces;
  TMatrix<HiBlock*> mtx;
  DVSList blocks;
//  VertexStruct *verteces;
//  FaceStruct *faces;
//  float *his;
//  f_vector3d *normals;
//  int w,h;
  VertexStruct *dirtyv;
  FaceStruct *dirtyf;
  FaceStruct *broken_list;

  void floodfill(int vx,int vy,FloodFiller &fw,bool fillborder,HMBorders &brd);
  void clear();
//  HiMap() {verteces=NULL;faces=NULL;w=h=0;textured=true;counteddx=counteddy=0;}
  HiMap() {textured=true;denyrepairwater=0;dirtyv=NULL;dirtyf=NULL;broken_list=NULL;boundsqx1=boundsqy1=0;boundsqx2=boundsqy2=-1;}//counteddx=counteddy=0;
  ~HiMap() {clear();}
//  void checkvertex(int x,int y) {xassert(x>=0);xassert(x<w);xassert(y>=0);xassert(y<h);}
//  void checkface(int x,int y) {xassert(x>=0);xassert(x<w-1);xassert(y>=0);xassert(y<h-1);}
  bool loadtga(const char *file);
//  VertexStruct &vertexat(int x,int y) {checkvertex(x,y);return verteces[y*w+x];}
  VertexStruct &vertexat(int x,int y) {return verteces(x,y);}
//  float &hiat(int x,int y) {checkvertex(x,y);return verteces[y*w+x].hi;}
  float interpolatehi(float x,float y);
  float &hiat(int x,int y) {return verteces(x,y).hi;}
  float &whiat(int x,int y) {return verteces(x,y).waterhi;}
//  FaceStruct &faceat(int x,int y) {checkface(x,y);return faces[y*(w-1)+x];}
  FaceStruct &faceat(int x,int y) {return faces(x,y);}
  void risecircle(float x,float y,float rad,float dhi);
  void circle(float mx,float my,float size,float disp);
//  void fault(float k,float x0);//k je smernice(0..1),x0
  void init();
  void doinit(int aw,int ah);
  void randomize(int initrand=-1);
  void circles(int steps,float mindisp,float maxdisp,float minrad,float maxrad);
  void clearhis(float hi0);
  void faultalg();
  fpoint3d get(int mapx,int mapy) {return fpoint3d(mapx,hiat(mapx,mapy),mapy);}
  ///vrati maximum z vysky a vodni vysky
  fpoint3d getmax(int mapx,int mapy) {return fpoint3d(mapx,max(hiat(mapx,mapy),whiat(mapx,mapy)),mapy);}
//  fpoint3d get(const MapPt &pt) {return get(pt.x,pt.y);}
//  void recount(int flags);
//  void uncountvrs();
  bool checkdxdy();
//  void assign(HiMap &src);

  bool valid(int x,int y) {return verteces.valid(x,y);}
  bool blocked(int x,int y) {HItem *h=hat(x,y);return h?h->blocked:true;}
  HItem *hat(int x,int y) {return verteces.tryat(x,y);}
  float gethi(int x,int y) {HItem *res=verteces.tryat(x,y);return res?res->hi:0;}
  float getwhi(int x,int y) {HItem *res=verteces.tryat(x,y);return res?res->waterhi:-1;}
  void createdblock(HiBlock *bl);
  void erasedblock(HiBlock *bl);
  void block(int x,int y) {if (HItem *h=hat(x,y)) h->blocked++;}
  void block(int x,int y,int w,int h);
  void unblock(int x,int y) {if (HItem *h=hat(x,y)) h->blocked--;}
  void unblock(int x,int y,int w,int h);
  void sethi(int x,int y,float hi);
  bool allowset(int x,int y,float h);//jen zepta se blockeru(nezkouma HItem::blocked)
  void makeinterval(int x,int y,Interval &i);
  void sethi(int x,int y,int w,int h,float hi);
  bool checkplane(int x0,int y0,int w,int h,float *h0);

  void repairpoint(int x,int y,float mhd);//opravi vysky bodu, aby nebyla porusena pravidla
  void repairrect(int rx,int ry,int rw,int rh,float mhd);
  void clearfinal(HMBorders brd);
  void repair(int startx,int starty,float mhd,HMBorders &brd);
  bool validate(int x,int y,float mhd);//vraci, zda neco zmenilo

  void save(Archive &ar);
  void load(Archive &ar);

  void fillwater(int x,int y,float waterhi);
  void drywater(int x,int y);
  void dryall();
  bool drysq(int x,int y);
  ///vrati prumernou vysku oblasti, ktera je nastavitelna pro vsechny body, -1,
  ///pokud body nemuzou mit vsechny stejnou vysku, w,h jsou ctverecky
  float getfreehi(int x,int y,int w,int h,bool *test=NULL);//test je pole velikosti (w+1)*(h+1) 

  void repairwater(HMBorders brd);//opravi vodu po provedeni terreni upravy
  //vodni uprava se provadi tak, ay po jejim provedeni bylo vse v poradku
  //xi,yi jsou vertex-souradnice

  void repairwater(int x,int y);//opravi vodu
  //1/pokud je voda pod urovni terenu,vysusit
  //2/pokud je voda v okoli vys, tak prelit sem
  //3/pokud je voda vys nez v okoli, vyplnit okoli

  //vrati, zda teren je uplne volny (bez bloku)
  bool cangenerate();
//  void changed() {modified=true;}
  void octalize();
//  void delize();
  void ot_render(int id);
  void clearvisibility();

  void countbroken();

  void modify(int x1,int y1,int x2,int y2);
  void modifyall() {modify(0,0,faces.w-1,faces.h-1);}

//  void countbounds();
  void repairbounds();

  int boundsqx1,boundsqy1,boundsqx2,boundsqy2;
  int boundvx1,boundvy1,boundvx2,boundvy2;
private:
//  void countvisiblefaces();
//  void countvrs();
//  void movevrs();
//  void countfacenormals();
//  void countvertexnormals();
//  void countintensity();
  int denyrepairwater;

  bool textured;
//  bool modified;
//  bool octalized;
//  float counteddx,counteddy;
  void ff_search(FloodFillStack &stack,FloodFiller &fw,int xmin,int xmax,int y);
  void changedwater(HMBorders brd);

  HiMap(const HiMap &src);//vorbidden
};

struct HiMapBackup {
  float *buf;
  HiMap *map;
  ~HiMapBackup() {delete []buf;}
  HiMapBackup(HiMap *map):map(map) {buf=new float[map->verteces.w*map->verteces.h];}
  void storemap();
  void restoremap();
};


}

#endif
