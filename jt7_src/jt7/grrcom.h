#ifndef grrcomH
#define grrcomH

#include "game0.h"

namespace jtyc {

/*Ctverec:
 *d    3    c
 *
 *4         2
 *
 *a    1    b
 *tt_xy je spojnice mezi x a y
 */


//#define track_constant_trackw 0.15
//#define track_constant_prazmez 0.1
//#define track_constant_prazw 1.5*track_constant_trackw
//#define track_constant_sterkw 1.8*track_constant_trackw
//#define track_constant_semdist 0.9*track_constant_trackw

//const double trackw=track_constant_trackw,//rozchod koleji
//prazmez=track_constant_prazmez,//mezera mezi prazci
//prazw=track_constant_prazw,//prazw je delka prazcu
//sterkw=track_constant_sterkw,//sirka sterkoveho loze
//semdist=track_constant_semdist;//vzdalenost semaforu od stredu koleji

const float maxspeed1=50,maxspeed2=100,maxspeed3=150;

enum {vr_none,vr_1,vr_2,vr_3,vr_4,vr_a,vr_b,vr_c,vr_d};
const int vr_cnt=8;
enum {tt_none,
  tt_13,tt_24,tt_12,tt_14,
  tt_34,tt_23,tt_ac,tt_bd,
  tt_1c,tt_1d,tt_4c,tt_4b,
  tt_2d,tt_2a,tt_3a,tt_3b,
  tt_ab,tt_bc,tt_cd,tt_ad
};
const int tt_cnt=20;

const double horlen=1.0,diaglen=M_SQRT2,curve1len=M_PI_4;
#define bcc_constant_tan_M_PI_8 0.414213562
#define bcc_constant_r 1/(2*(bcc_constant_tan_M_PI_8))
#define bcc_constant_arcdx 1/(2*M_SQRT2)
#define bcc_constant_linedx 0.5-(bcc_constant_arcdx)
#define bcc_constant_linelen sqrt(2*(bcc_constant_linedx)*(bcc_constant_linedx))
#define bcc_constant_arclen M_PI_4*(bcc_constant_r)

namespace bcc {
  const double
    r=bcc_constant_r,
    arcdx=bcc_constant_arcdx,
    linedx=bcc_constant_linedx,
    linelen=bcc_constant_linedx,
    arclen=bcc_constant_arclen
    ;
}
//curve3 je zatacka s nejvetsim polomerem(vse v bcc se tyka ji), sklada se z
//oblouku a rozvne cary, arclen je delka oblouku, linelen delka rovne cary

const double curve2len=M_SQRT2*M_PI_4,curve3len=bcc::arclen+bcc::linelen;

const double tt_lens[tt_cnt+1]={0,horlen,horlen,curve1len,curve1len,curve1len,curve1len,
diaglen,diaglen,
curve3len,curve3len,curve3len,curve3len,curve3len,curve3len,curve3len,curve3len,
curve2len,curve2len,curve2len,curve2len};

const bool tt_streight[tt_cnt+1]={0,true,true,false,false,false,false,true,true,
false,false,false,false,false,false,false,false,false,false,false,false};


const int vr_dxs[vr_cnt+1]={0,0,1,0,-1,-1,1,1,-1};
const int vr_dys[vr_cnt+1]={0,1,0,-1,0,1,1,-1,-1};
//indexovano vracholy,
//vr_dxs[vr_x]=-1:vrchol je nalevo od sredu,+1:naprovo od stredu,0:nad nebo pod stredem
//analogicky vr_dys(-1:nad)

//pole popisujici vztah tt_xy a vr_xy:
//toto pole je indexovano tt_xy, hodnoty jsou vr_x
const int tt_begs_data[tt_cnt+1]={vr_none,vr_1,vr_2,vr_1,vr_1,vr_3,vr_2,vr_a,vr_b,  vr_1,vr_1,vr_4,vr_4,vr_2,vr_2,vr_3,vr_3,  vr_a,vr_b,vr_c,vr_a};
//toto pole je indexovano tt_xy, hodnoty jsou  vr_y
const int tt_ends_data[tt_cnt+1]={vr_none,vr_3,vr_4,vr_2,vr_4,vr_4,vr_3,vr_c,vr_d,  vr_c,vr_d,vr_c,vr_b,vr_d,vr_a,vr_a,vr_b,  vr_b,vr_c,vr_d,vr_d};

//indexovano dvojici vrcholu,hodnoty tt_cons[x][y] jsou cesta z x do y
//toto pole je definavano a inicializovano v souboru gconst.cpp
extern int tt_cons[vr_cnt+1][vr_cnt+1];

//pokud vstoupim do ctverecku vrcholem x, vystupil jsme z minuleho vrcholem vr_direct[x]
const int vr_direct[vr_cnt+1]={vr_none,vr_3,vr_4,vr_1,vr_2,vr_c,vr_d,vr_a,vr_b};

const int vrcholy_data[3][3]={{vr_d,vr_3,vr_c},{vr_4,vr_none,vr_2},{vr_a,vr_1,vr_b}};

//extern int tt_other[vr_cnt+1][2*tt_cnt+1];
//hodnota tt_other[a][b] je druhy vrchol spojnice b nez a(nebo 0)

//maximalni rychlosti
const float tt_maxspeeds[tt_cnt+1]={0,
  0,0,maxspeed1,maxspeed1,
  maxspeed1,maxspeed1,0,0,
  maxspeed3,maxspeed3,maxspeed3,maxspeed3,
  maxspeed3,maxspeed3,maxspeed3,maxspeed3,
  maxspeed2,maxspeed2,maxspeed2,maxspeed2
};

struct Vrchol {
  static const int count=vr_cnt;
  int data;
  Vrchol(int dx,int dy) {data=vrcholy_data[dy+1][dx+1];}
  Vrchol() {data=vr_none;}
  int dx() {return vr_dxs[data];}
  int dy() {return vr_dys[data];}
  Vrchol direct() {return Vrchol(vr_direct[data]);}//vrati vrchol naproti

  explicit Vrchol(int adata) {data=adata;}
  bool valid() {return data;}

};
inline bool operator ==(Vrchol a,Vrchol b) {return a.data==b.data;}
inline bool operator !=(Vrchol a,Vrchol b) {return a.data!=b.data;}

fpoint countnone(double t) {return fpoint(0,0);}
//rovne:
fpoint count13(double t);
fpoint count24(double t);
//male zatacky:
fpoint count12(double t);
fpoint count14(double t);
fpoint count34(double t);
fpoint count23(double t);
//uhlopricky:
fpoint countac(double t);
fpoint countbd(double t);
//velke zatacky:
fpoint count1c(double t);
fpoint count1d(double t);
fpoint count4c(double t);
fpoint count4b(double t);
fpoint count2d(double t);
fpoint count2a(double t);
fpoint count3a(double t);
fpoint count3b(double t);
//stredni zatacky
fpoint countab(double t);
fpoint countbc(double t);
fpoint countcd(double t);
fpoint countad(double t);



fvector dcountnone(double t) {return fvector(0,0);}
//rovne:
fvector dcount13(double t);
fvector dcount24(double t);
//male zatacky:
fvector dcount12(double t);
fvector dcount14(double t);
fvector dcount34(double t);
fvector dcount23(double t);
//uhlopricky:
fvector dcountac(double t);
fvector dcountbd(double t);
//velke zatacky:
fvector dcount1c(double t);
fvector dcount1d(double t);
fvector dcount4c(double t);
fvector dcount4b(double t);
fvector dcount2d(double t);
fvector dcount2a(double t);
fvector dcount3a(double t);
fvector dcount3b(double t);
//stredni zatacky
fvector dcountab(double t);
fvector dcountbc(double t);
fvector dcountcd(double t);
fvector dcountad(double t);



fpoint (*const count_pos[])(double t)={countnone,count13,count24,
count12,count14,count34,count23,
countac,countbd,
count1c,count1d,count4c,count4b,count2d,count2a,count3a,count3b,
countab,countbc,countcd,countad};

fvector (*const dcount_pos[])(double t)={dcountnone,dcount13,dcount24,
dcount12,dcount14,dcount34,dcount23,
dcountac,dcountbd,
dcount1c,dcount1d,dcount4c,dcount4b,dcount2d,dcount2a,dcount3a,dcount3b,
dcountab,dcountbc,dcountcd,dcountad};

//extern RRTexture tt_bmps[];


struct Spojnice {
  static const int count=tt_cnt;
  int data;

  double len() const {return tt_lens[abs(data)];}//vrati delku, nezavisi na sqsize
  bool streight() const {return tt_streight[abs(data)];}//vrati, zda je rovne
  Vrchol beg() const {return Vrchol(data>=0?tt_begs_data[data]:tt_ends_data[-data]);}
  Vrchol end() const {return Vrchol(data>=0?tt_ends_data[data]:tt_begs_data[-data]);}
  Spojnice(Vrchol a,Vrchol b) {data=tt_cons[a.data][b.data];}
  Spojnice() {data=tt_none;}
  explicit Spojnice(int adata) {data=adata;}

  fpoint pos(double t) const {
    if (data>0) return (count_pos[data])(t);
    else return (count_pos[-data])(len()-t);
  }//spocita polohu v ramci cterecku,0<=t<=len()
  fvector diff(double t) const {
    if (data>0) return (dcount_pos[data])(t);
    else return -(dcount_pos[-data])(len()-t);
  }//spocita JEDNOTKOVY smerovy vektor tecny,0<=t<=len()

  int flag() const {return getflag(abs(data)-1);}

  bool valid() const {return data;}

  bool test(int adata) const {return abs(adata)==abs(data);}
  float maxspeed() const {return tt_maxspeeds[abs(data)];}

  Vrchol other(Vrchol v) const {if (v==beg()) return end();if (v==end()) return beg();return Vrchol(vr_none);}
  void save(XMLNode xml) {xml.setvar("tt.data",data);}
  void load(XMLNode xml) {data=xml.ivar("tt.data");}
};
inline bool operator ==(Spojnice a,Spojnice b) {return a.data==b.data;}
inline bool operator !=(Spojnice a,Spojnice b) {return a.data!=b.data;}

void init_tt_cons();

void draw_gravel(Spojnice tt,RItem *ri,float dhi,RenderContext &rc,float grav_w,float quality);
void draw_sleepers(Spojnice tt,RItem *ri,float dhi,RenderContext &rc,float sl_len,float sl_wi,float sl_cover);
void draw_rail(Spojnice tt,RItem *ri,float dhi,RenderContext &rc,float quality,float rail_w,float gauge_2);
void draw_wire(Spojnice tt,RItem *ri,float dhi,RenderContext &rc,float quality,float gauge_2,float linewi,bool texture);


}

#endif
