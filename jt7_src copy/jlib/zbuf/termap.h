//---------------------------------------------------------------------------

#ifndef termapH
#define termapH

#include <zbuf/zbmp.h>
#include <math.h>

namespace zbuf {

class TERMAP
{
private:
  uchar _geth(int x,int y) {return zbuf[y*W+x];}
  void blittozbmp(ZBMP &bmp,int left,int top,int x0,int y0,int xcnt,int ycnt,vector3d<double> sun);
  //predpoklad:xcnt,ycnt v mezich (zajisti se volanim blittozbmp(ZBMP&,int,int,int3dvector,int=,int=);)
public:
  int W,H;
  int kz,ky;
  int Kz,//=kz*histep
  Ky;//=ky*step
  float fkz,fky;
  float fKz,fKy;
  uchar *zbuf;
  int step,histep;
  void seth0(uchar h) {fillarray(zbuf,W*H,h);}
  void setkoefs() {kz=fkz*1024;ky=fky*1024;Kz=kz*histep;Ky=ky*step;fKz=fkz*histep;fKy=fky*step;}
  void setseeangle(float angle) {fkz=cos(angle);fky=sin(angle);setkoefs();}
  float getseeangle() {return atan2((double)ky/1024,(double)kz/1024);}
  TERMAP(int xsize,int ysize,int astep,int ahistep);
  void blittozbmp(ZBMP &bmp,int left,int top,vector3d<double> sun,int x0=0,int y0=0);
  //left,top jsou sour. v TERMAP,x0,y0 sour. v ZBMP
  //pokusi se vykrelit maximum,co se vejde do bmp
  //souradnice [left,top] bude v bode [x0,y0]
  ~TERMAP();
  void seth(int x,int y,uchar h) {if (x>=0 and x<W and y>=0 and y<H) zbuf[y*W+x]=h;}
};


}
#endif
