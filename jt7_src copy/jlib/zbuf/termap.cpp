#include <cblib.h>
#pragma hdrstop

#include "termap.h"

namespace zbuf {


TERMAP::TERMAP(int xsize,int ysize,int astep,int ahistep)
{
  W=xsize;H=ysize;zbuf=new uchar[W*H];seth0(0);step=astep;histep=ahistep;
  setseeangle(M_PI_4);
}

void TERMAP::blittozbmp(ZBMP &bmp,int left,int top,vector3d<double> sun,int x0,int y0)
{
  int xcnt=min((bmp.W-x0)/step+1,W-left-1);//pocet obdelnicku
  //tedy pocet delicich car je o jedno vetsi
  int ymax=top;//souradnice v mape
  int ybot=y0+bmp.H;//souradnice v obrazku,2D souradnice!!!
  int ybot1024=ybot<<10;

  for(int y=top;y<H;y++)
  {
    for(int x=0;x<=xcnt;x++)//<=xcnt,protoze pocet delicich car=pocet obdelnicku+1
    {
      if ( y*Ky-_geth(x+left,y)*Kz<=ybot1024) ymax=y;
    }
  }
  ymax++;
  if (ymax>=H) ymax=H-1;
  int ycnt=ymax-top;
  blittozbmp(bmp,left,top,x0,y0,xcnt,ycnt,sun);
}

static uchar spocitej_intenzitu(int z_nahore,int z_dole,int z_naproti,
                              int Dy,
                              int Dx,vector3d<double> &sun,bool dolni)
{
  int imz;
  if (dolni) imz=z_dole;else imz=z_nahore;
  vector3d<double> smerx(Dx,0,z_naproti-imz),smery(0,Dy,z_dole-z_nahore);
  vector3d<double> normala=smerx%smery;
  if (normala.z<0) normala=-normala;
  float odchylka=normala/sun;
  float fintenzita=odchylka/M_PI_2;
  uchar intenzita=96+fintenzita*32;
  return intenzita;
}


void TERMAP::blittozbmp(ZBMP &bmp,int left,int top,int x0,int y0,int xcnt,int ycnt,vector3d<double> sun)
{
  int xl=x0,cy=y0-top*step;
  //cy je tokave,ze top*step+cy=0
  for(int x=0;x<xcnt;x++,xl+=step)
  {
    for(int y=0;y<ycnt;y++)
    {
      int z1=_geth(left+x,top+y),z2=_geth(left+x,top+y+1),z3=_geth(left+x+1,top+y),z4=_geth(left+x+1,top+y+1);
      //Rozmisteni bodu:
      //1   3
      //    4
      //2
      int Y1=(y*Ky)>>10,Y2=((y+1)*Ky)>>10,Y3=(y*Ky)>>10,Y4=((y+1)*Ky)>>10;//3D souradnice
      int y1=Y1-((z1*Kz)>>10),y2=Y2-((z2*Kz)>>10),y3=Y3-((z3*Kz)>>10),y4=Y4-((z4*Kz)>>10);//2D souradnice
      int hlu=z1+z4,veu=z2+z3;
      if (hlu<=veu)
      {//rozdelime na 2 trojuhelniky podle hlavni diagonaly
        {//dolni trojuhelnik
          bmp.xlefttriangle(xl,xl+step,y1-cy,y2-cy,y4-cy,z1,z2,z4,spocitej_intenzitu(z1,z2,z4,step,step,sun,true));
        }
        {//horni trojuhelnik
          bmp.xrighttriangle(xl,xl+step,y3-cy,y4-cy,y1-cy,z3,z4,z1,spocitej_intenzitu(z3,z4,z1,step,-step,sun,false));
        }
      }else
      {//rozdelime na 2 trojuhelniky podle vedlejsi diagonaly
        {//horni trojuhelnik
          bmp.xlefttriangle(xl,xl+step,y1-cy,y2-cy,y3-cy,z1,z2,z3,spocitej_intenzitu(z1,z2,z3,step,step,sun,false));
        }
        {//dolni trojuhelnik
          bmp.xrighttriangle(xl,xl+step,y3-cy,y4-cy,y2-cy,z3,z4,z2,spocitej_intenzitu(z3,z4,z2,step,-step,sun,true));
        }
      }
//      bmp.rectangle(xl,y*step*ky,xl+step,(y+1)*step*ky,255,0);
    }
  }
}

TERMAP::~TERMAP()
{
  delete []zbuf;
}

}

#pragma package(smart_init)
