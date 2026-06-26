#ifndef gmtcomH
#define gmtcomH

#include "game0.h"

namespace jtyc {

/*
    3

4       2

    1
*/

enum {mv_none,mv_1,mv_2,mv_3,mv_4};
enum {mt_none,mt_13,mt_24,mt_12,mt_23,mt_34,mt_14};

const int mv_cnt=4;
const int mt_cnt=6;

const int mv_dxs[mv_cnt+1]={0,0,1,0,-1};
const int mv_dys[mv_cnt+1]={0,1,0,-1,0};
//indexovano vracholy,
//vr_dxs[vr_x]=-1:vrchol je nalevo od sredu,+1:naprovo od stredu,0:nad nebo pod stredem
//analogicky vr_dys(-1:nad)

//pole popisujici vztah tt_xy a vr_xy:
//toto pole je indexovano tt_xy, hodnoty jsou vr_x
const int mt_begs_data[mt_cnt+1]={mv_none,mv_1,mv_2, mv_1,mv_2,mv_3,mv_1};
//toto pole je indexovano tt_xy, hodnoty jsou  vr_y
const int mt_ends_data[mt_cnt+1]={mv_none,mv_3,mv_4, mv_2,mv_3,mv_4,mv_4};

//indexovano dvojici vrcholu,hodnoty tt_cons[x][y] jsou cesta z x do y
//toto pole je definavano a inicializovano v souboru gconst.cpp
extern int mt_cons[mv_cnt+1][mv_cnt+1];

extern bool mt_cross[2*mt_cnt+1][2*mt_cnt+1];//obsahuje informaci, zda se 2 spojnice krizi

//pokud vstoupim do ctverecku vrcholem x, vystupil jsme z minuleho vrcholem vr_direct[x]
const int mv_direct[mv_cnt+1]={mv_none,mv_3,mv_4,mv_1,mv_2};

const int mvrcholy_data[3][3]={{mv_none,mv_3,mv_none},{mv_4,mv_none,mv_2},{mv_none,mv_1,mv_none}};

struct MVrchol {
  static const int count=mv_cnt;
  int data;
  MVrchol(int dx,int dy) {data=mvrcholy_data[dy+1][dx+1];}
  MVrchol() {data=mv_none;}
  int dx() {return mv_dxs[data];}
  int dy() {return mv_dys[data];}
  MVrchol direct() {return MVrchol(mv_direct[data]);}//vrati vrchol naproti

  explicit MVrchol(int adata) {data=adata;}
  bool valid() {return data;}
};
inline bool operator ==(MVrchol a,MVrchol b) {return a.data==b.data;}
inline bool operator !=(MVrchol a,MVrchol b) {return a.data!=b.data;}


inline double mlen1(double dist) {
  return (0.5-dist)*M_SQRT2;
}
inline double mlen2(double dist) {
  return (0.5+dist)*M_SQRT2;
}

fpoint mcountnone(double t,double dist) {return fpoint(0,0);}
//rovne:
fpoint mcount13(double t,double dist);
fpoint mcount24(double t,double dist);
//opacne
fpoint mcount31(double t,double dist);
fpoint mcount42(double t,double dist);
//pres uhel
fpoint mcount12(double t,double dist);
fpoint mcount23(double t,double dist);
fpoint mcount34(double t,double dist);
fpoint mcount14(double t,double dist);
//opacne
fpoint mcount21(double t,double dist);
fpoint mcount32(double t,double dist);
fpoint mcount43(double t,double dist);
fpoint mcount41(double t,double dist);


fpoint (*const mcount_pos[])(double t,double dist)={
mcount41,mcount43,mcount32,mcount21, mcount42,mcount31,
mcountnone,
mcount13,mcount24, mcount12,mcount23,mcount34,mcount14
};

struct MSpojnice {
  static const int count=mt_cnt;
  int data;

//  double len() const {return mt_lens[abs(data)];}//vrati delku, nezavisi na sqsize
  MVrchol beg() const {return MVrchol(data>=0?mt_begs_data[data]:mt_ends_data[-data]);}
  MVrchol end() const {return MVrchol(data>=0?mt_ends_data[data]:mt_begs_data[-data]);}
  MSpojnice(MVrchol a,MVrchol b) {data=mt_cons[a.data][b.data];}
  MSpojnice() {data=mt_none;}
  explicit MSpojnice(int adata) {data=adata;}

  fpoint pos(double t,double dist) const {
    return (mcount_pos[data+mt_cnt])(t,dist);
  }//spocita polohu v ramci cterecku,0<=t<=len()
  bool valid() const {return data;}

  bool test(int adata) const {return abs(adata)==abs(data);}

  MVrchol other(MVrchol v) const {if (v==beg()) return end();if (v==end()) return beg();return MVrchol(mv_none);}
  void save(XMLNode xml) {xml.setvar("mt.data",data);}
  void load(XMLNode xml) {data=xml.ivar("mt.data");}

  double len(double dist);

  bool cross(MSpojnice mt) {return mt_cross[mt.data+mt_cnt][data+mt_cnt];}
};
inline bool operator ==(MSpojnice a,MSpojnice b) {return a.data==b.data;}
inline bool operator !=(MSpojnice a,MSpojnice b) {return a.data!=b.data;}

void init_mt_cons();

void draw_gravel(RoadItem *ri,float dhi,RenderContext &rc,float d1,float d2,float d3);
void draw_midline(RoadItem *ri,float dhi,RenderContext &rc,float wi);
void draw_tramrail(RoadItem *ri,float dhi,RenderContext &rc,float gauge2,float wi,float motodist);
void draw_roadwire(RoadItem *ri,float dhi,RenderContext &rc,float gauge2,float wi,float motodist);

};

#endif
