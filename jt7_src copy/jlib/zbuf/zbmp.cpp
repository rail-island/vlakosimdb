#include <jlib.h>
#pragma hdrstop

#include "zbmp.h"
//---------------------------------------------------------------------------

#define zassert(condition) {if(!(condition)) throw "ZError";}

namespace zbuf {

//template class ZBMP<uchar,float>;

template<class COLOR,class HI> ZBMP<COLOR,HI>::ZBMP(int w,int h,COLOR transparentcolor,HI aminimalhi,bool areversey,int alignelements) {
  reversey=areversey;minimalhi=aminimalhi;
  transparent=transparentcolor;
  if (w<0) w=0;if (h<0) h=0;
  W=w;H=h;
  if (alignelements<1) alignelements=1;
  alignedW=W;
  while (alignedW%alignelements) alignedW++;
  size=H*alignedW;

  buf=new COLOR[size+1];zbuf=new HI[size+1];
  scanline=new COLOR*[H+1];zscanline=new HI*[H+1];
  lineofs=new int[H];
  if (reversey) for(int i=H-1,ofs=0;i>=0;i--,ofs+=alignedW) lineofs[i]=ofs;
  else for(int i=0,ofs=0;i<H;i++,ofs+=alignedW) lineofs[i]=ofs;
  for(int i=0,ofs=0;i<H;i++) {
    scanline[i]=buf+lineofs[i];
    zscanline[i]=zbuf+lineofs[i];
  }
  clear();
}

template<class COLOR,class HI> void ZBMP<COLOR,HI>::fillellipse(int x1,int y1,int x2,int y2,HI z,COLOR color) {
  makeless(x1,x2);makeless(y1,y2);
  double midx=(x1+x2)/2.0,midy=(y1+y2)/2.0;
  double a=(x2-x1)/2.0,b=(y2-y1)/2.0,a2=a*a,b2=b*b;
  if (a2>0 and b2>0) {
    for(int x=x1;x<=x2;x++) for(int y=y1;y<=y2;y++) {
      double dx=x-midx,dy=y-midy;
      if (dx*dx/a2+dy*dy/b2<=1.0) putpixel(x,y,z,color);
    }
  }
  fellipse(midx,midy,a,b,z,color);
}

template<class COLOR,class HI> void ZBMP<COLOR,HI>::fellipse(double mx,double my,double radiusx,double radiusy,HI z,COLOR color,double start,double stop) {
  if (radiusx>0 and radiusy>0) {
    for(double t=start,dt=1/(fabs(start-stop)*max(radiusx,radiusy));t<stop+dt;t+=dt)   {
      double x=mx+radiusx*cos(t),y=my+radiusy*sin(t);
      putpixel(x,y,z,color);
    }
  }
}

template<class COLOR,class HI> void ZBMP<COLOR,HI>::move(int dx,int dy) {
  int absdy=abs(dy),absdx=abs(dx);
  if (absdx>=W or absdy>=H) return;
  int movx=W-absdx;
  int dxplus=max(dx,0),dxminus=max(-dx,0);
  if (dy>0) {
    for(int y=H-absdy-1;y>=0;y--) {
      memcpy(scanline[y+absdy]+dxplus,scanline[y]+dxminus,movx*sizeof(COLOR));
      memcpy(zscanline[y+absdy]+dxplus,zscanline[y]+dxminus,movx*sizeof(HI));
    }
  }
  if (dy<0) {
    for(int y=absdy;y<H;y++) {
      memcpy(scanline[y-absdy]+dxplus,scanline[y]+dxminus,movx*sizeof(COLOR));
      memcpy(zscanline[y-absdy]+dxplus,zscanline[y]+dxminus,movx*sizeof(HI));
    }
  }
  if (dy==0) {
    for(int y=0;y<H;y++) {
      memmove(scanline[y]+dxplus,scanline[y]+dxminus,movx*sizeof(COLOR));
      memmove(zscanline[y]+dxplus,zscanline[y]+dxminus,movx*sizeof(HI));
    }
  }
}

template<class COLOR,class HI> void ZBMP<COLOR,HI>::
  copyrect(const ZBMP<COLOR,HI> &src,int destx,int desty,int srcx,int srcy,int w,int h) {
  xassert(srcx>=0);xassert(srcy>=0);xassert(destx>=0);xassert(desty>=0);
//  if (destx<0) destx=0;if (desty<0) desty=0;
//  if (srcx<0) srcx=0;if (srcy<0) srcy=0;
  if (W-destx<w) w=W-destx;if (src.W-srcx<w) w=src.W-srcx;
  if (H-desty<h) h=H-desty;if (src.H-srcy<h) h=src.H-srcy;
  if (w>0) for(int y=0;y<h;y++) {
    memcpy(scanline[y+desty]+destx,src.scanline[y+srcy]+srcx,sizeof(COLOR)*w);
    memcpy(zscanline[y+desty]+destx,src.zscanline[y+srcy]+srcx,sizeof(HI)*w);
  }
}

template<class COLOR,class HI> void ZBMP<COLOR,HI>::assign(const ZBMP<COLOR,HI> &srcbmp) {
  xassert(size==srcbmp.size);
//  xassert(W==srcbmp.W);xassert(H==srcbmp.H);
//  int count=W*H;int i;
//  i=0;
//  for(COLOR *src=srcbmp.buf,*dest=buf;i<count;i++) dest[i]=src[i];
//  i=0;
//  for(HI *src=srcbmp.zbuf,*dest=zbuf;i<count;i++) dest[i]=src[i];

//  i=0;
//  for(COLOR *src=srcbmp.buf,*dest=buf;i<count;i++) *dest++=*src++;
//  i=0;
//  for(HI *src=srcbmp.zbuf,*dest=zbuf;i<count;i++) *dest++=*src++;

//  memcpy(buf,srcbmp.buf,W*H*sizeof(COLOR));
//  memcpy(zbuf,srcbmp.zbuf,W*H*sizeof(HI));
  memcpy(buf,srcbmp.buf,size*sizeof(COLOR));
  memcpy(zbuf,srcbmp.zbuf,size*sizeof(HI));
}

template<class COLOR,class HI> void ZBMP<COLOR,HI>::minmaxcoords(XYREC &min,XYREC &max) {
  min.x=W;min.y=H;max.x=-1;max.y=-1;
  for(int y=0;y<H;y++) {
    COLOR *act=scanline[y];
    for(int x=0;x<W;x++,act++)
      if (*act!=transparent)
        updatemmxy(XYREC(x,y),min.x,min.y,max.x,max.y);
  }
}

template<class COLOR,class HI> ZBMP<COLOR,HI>::~ZBMP() {
  delete []buf;delete []zbuf;
  delete []scanline;delete []zscanline;
  delete []lineofs;
}

template<class COLOR,class HI> void ZBMP<COLOR,HI>::rectangle(int x1,int y1,int x2,int y2,HI zrec,COLOR color) {
  int X1=x1,X2=x2,Y1=y1,Y2=y2;
  if (X1<0) X1=0;if (Y1<0) Y1=0;if (X2>=W) X2=W-1;if (Y2>=H) Y2=H-1;
  COLOR *c;HI *z;
  int t;int dW=reversey?-alignedW:alignedW;
  if (y1>=0 and y1<H and X1<X2)
    for(c=scanline[y1]+X1,z=zscanline[y1]+X1,t=X1;t<=X2;t++,z++,c++)
      if (zrec>=*z) *c=color,*z=zrec;
  if (y2>=0 and y2<H and X1<X2)
    for(c=scanline[y2]+X1,z=zscanline[y2]+X1,t=X1;t<=X2;t++,z++,c++)
      if (zrec>=*z) *c=color,*z=zrec;
  if (x1>=0 and x1<W and Y1<Y2)
    for(c=scanline[Y1]+x1,z=zscanline[Y1]+x1,t=Y1;t<=Y2;t++,z+=dW,c+=dW)
      if (zrec>=*z) *c=color,*z=zrec;
  if (x2>=0 and x2<W and Y1<Y2)
    for(c=scanline[Y1]+x2,z=zscanline[Y1]+x2,t=Y1;t<=Y2;t++,z+=dW,c+=dW)
      if (zrec>=*z) *c=color,*z=zrec;
}

template<class COLOR,class HI> void ZBMP<COLOR,HI>::fillrect(int x1,int y1,int x2,int y2,HI zrec,COLOR color) {
  makeless(x1,x2);makeless(y1,y2);
  if (x1<0) x1=0;if (y1<0) y1=0;if (x2>=W) x2=W-1;if (y2>=H) y2=H-1;
  for(int y=y1;y<=y2;y++) {
    register HI *z=zscanline[y]+x1;
    for(register COLOR *c=scanline[y]+x1,*cend=c+(x2-x1);c<=cend;c++,z++)
      if (zrec>=*z) *c=color,*z=zrec;
  }
}

template<class COLOR,class HI> void ZBMP<COLOR,HI>::clearrect(int x1,int y1,int x2,int y2,COLOR color) {
  makeless(x1,x2);makeless(y1,y2);
  if (x1<0) x1=0;if (y1<0) y1=0;if (x2>=W) x2=W-1;if (y2>=H) y2=H-1;
  for(int y=y1;y<=y2;y++) {
    register HI *z=zscanline[y]+x1;
    register COLOR *c=scanline[y]+x1;
    for(COLOR *cend=c+(x2-x1);c<=cend;*c++=color,*z++=minimalhi);
  }
}

template<class COLOR,class HI> void ZBMP<COLOR,HI>::line(int x1,int y1,HI z1,int x2,int y2,HI z2,COLOR color) {
  int Dx=x2-x1,Dy=y2-y1;
  float len=sqrt(Dx*Dx+Dy*Dy);
  if (len<=0) return;
  float dx=Dx/len,dy=Dy/len;
  int cnt=len;
  float x=x1,y=y1;
  HI Dz=z2-z1,dz=Dz/len,z=z1;
  for(int i=0;i<=len;i++,x+=dx,y+=dy,z+=dz) putpixel(round(x),round(y),z,color);
}

/*
#define getx(i_) (pts[i_<<1])
#define gety(i_) (pts[(i_<<1)+1])

//ma spocitano lastx,lasty,pocita nextx,nexty
static void nextside(int pts[],int pointscount,int &lastx,int &lasty,int &nextx,int &nexty,
              int &i,int &d,int incdec,int &posun)
{
  int acty;
  lastx=nextx;lasty=nexty;
  for(;;)
  {
    i+=incdec;if (i<0) i=pointscount-1;if (i>=pointscount) i=0;
    posun++;
    if (posun>pointscount) return;
    acty=gety(i);
    zassert(acty>=lasty);
    if (acty>lasty) break;
    lastx=getx(i);
  }
  nexty=acty;nextx=getx(i);
  d=((nextx-lastx)<<10)/(nexty-lasty);
}

void ZBMP::polygon(int pts[],int pointscount,int dzx,int dzy,point3d<int> fix,COLOR color) {
  int miny=gety(0),minyidx=0;
  for(int i=1;i<pointscount;i++) {int ay=gety(i);if (ay<miny) miny=ay,minyidx=i;}

  int posun=0;

  int lefti=minyidx,righti=minyidx;
  int lastlefty=gety(lefti),lastrighty=gety(righti),lastleftx=getx(lefti),lastrightx=getx(righti);
  int dleft=0,dright=0,nextrightx=lastrightx,nextrighty=lastrighty,nextleftx=lastleftx,nextlefty=lastlefty;

  nextside(pts,pointscount,lastleftx,lastlefty,nextleftx,nextlefty,lefti,dleft,-1,posun);
  nextside(pts,pointscount,lastrightx,lastrighty,nextrightx,nextrighty,righti,dright,+1,posun);

  int yreltofix=miny-fix.y;
  for(int y=miny;posun<=pointscount and y<H;y++,yreltofix++)
  {
    int zy=fix.z+((yreltofix*dzy)>>10);
    if (y==nextlefty)
     nextside(pts,pointscount,lastleftx,lastlefty,nextleftx,nextlefty,lefti,dleft,-1,posun);
    if (y==nextrighty)
     nextside(pts,pointscount,lastrightx,lastrighty,nextrightx,nextrighty,righti,dright,+1,posun);
    if (y>=0)
    {
      int xbeg=lastleftx+(((y-lastlefty)*dleft)>>10);
      int xend=lastrightx+(((y-lastrighty)*dright)>>10);
      if (xend<xbeg) {int pom=xend;xend=xbeg;xbeg=pom;}
      if (xbeg<0) xbeg=0;if (xend>=W) xend=W-1;
      int xreltofix=xbeg-fix.x;
      REAL *z=zscanline[y]+xbeg;
      for(COLOR *cbeg=scanline[y]+xbeg,*cend=cbeg+(xend-xbeg+1),*c=cbeg;
              c<=cend;c++,z++,xreltofix++)
      {
        int actz=zy+((xreltofix*dzx)>>10);
        if (actz>=*z) *z=actz,*c=color;
      }
    }
  }
}


void ZBMP::line(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color) {
  int Dx=x2-x1,Dy=y2-y1;
  if (Dx or Dy)//neni to jen bod
  {
    if (abs(Dx)>=abs(Dy))
    {
      if (x1>x2) line_de_x(x2,y2,z2,x1,y1,z1,color);else line_de_x(x1,y1,z1,x2,y2,z2,color);
    }
    else
    {
      if (y1>y2) line_de_y(x2,y2,z2,x1,y1,z1,color);else line_de_y(x1,y1,z1,x2,y2,z2,color);
    }
  }
}

void ZBMP::line_de_x(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color)
{
  int Dx=x2-x1;zassert(Dx>0);
  int Dy=y2-y1;
  int Dz=z2-z1;
  int dy=(Dy<<10)/Dx;
  int dz=(Dz<<10)/Dx;

    int end=Dx;if (end+x1>=W) end=W-x1-1;
    for(int t=max(0,-x1);t<=end;t++)
    {
      int acty=y1+((dy*t)>>10);
      if (acty<0 or acty>=H) continue;
      int actz=z1+((dz*t)>>10);
      int ofs=lineofs[acty]+t+x1;
      REAL *pz=zbuf+ofs;
      if (actz>=*pz) *pz=actz,buf[ofs]=color;
    }
}

void ZBMP::line_de_y(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color)
{
  int Dy=y2-y1;zassert(Dy>0);
  int Dx=x2-x1;
  int Dz=z2-z1;
  int dx=(Dx<<10)/Dy;
  int dz=(Dz<<10)/Dy;
    int end=Dy;if (end+y1>=H) end=H-y1-1;
    for(int t=max(0,-y1);t<=end;t++)
    {
      int actx=x1+((dx*t)>>10);
      if (actx<0 or actx>=W) continue;
      int actz=z1+((dz*t)>>10);
      int ofs=lineofs[t+y1]+actx;
      REAL *pz=zbuf+ofs;
      if (actz>=*pz) *pz=actz,buf[ofs]=color;
    }
}

void ZBMP::yline(int x,int y1,int y2,int z1,int z2,COLOR color)
{
  int Dy=y2-y1,Dz=z2-z1;
  if (Dy==0) return;
  register COLOR *c=scanline[y1]+x;
  register REAL *z=zscanline[y1]+x;
  int dz=(Dz<<10)/Dy;
  {
    int end=Dy;if (end+y1>=H) end=H-y1-1;
    for(int t=max(0,-y1);t<=end;c+=W,z+=W,t++)
    {
      int actz=z1+((dz*t)>>10);
      if (actz>=*z) {*z=actz;*c=color;}
    }
  }
}

void ZBMP::xline(int y,int x1,int x2,int z1,int z2,COLOR color)
{
  int Dx=x2-x1,Dz=z2-z1;
  if (Dx==0) return;
  register COLOR *c=scanline[y]+x1;
  register REAL *z=zscanline[y]+x1;
  int dz=(Dz<<10)/Dx;
  {
    int t=max(0,-x1);
    int end=Dx;if (end+x1>=W) end=W-x1-1;
    for(c+=t;t<end;c++,z++,t++)
    {
      int actz=z1+((dz*t)>>10);
      if (actz>=*z) {*z=actz;*c=color;}
    }
  }
}


//imz je zetova souradnice na urovni y3
#define triangle(y_beg,y_end)\
{\
  zassert(y1<=y2 and x1<=x2);\
\
  if (y1==y2) {line(x1,y1,z1,x2,y3,z3,color);return;}\
  if (x1==x2) {yline(x1,y1<y3?y1:y3,y2,y1<y3?z1:z3 ,z2,color);return;}\
\
  int Dy12=y2-y1,Dx=x2-x1,Dzy=z2-z1,dzy=(Dzy<<10)/Dy12;\
\
  int imz=z1 - (((y1-y3)*dzy)>>10);\
\
\
  int Dzx=z3-imz,dzx=(Dzx<<10)/Dx;\
  int Dy13=y1-y3,dy1=(Dy13<<10)/Dx;\
  int Dy23=y2-y3,dy2=(Dy23<<10)/Dx;\
\
    int xend=min(x2,W-1);\
    for(int x=max(0,x1);x<=xend;x++)\
    {\
      int xt=x-x1;\
      int z0=z1+((dzx*xt)>>10);\
      int ybeg=y_beg ((dy1*xt)>>10);\
      int yend=y_end ((dy2*xt)>>10);\
      if (ybeg<0) ybeg=0;\
      if (yend>=H) yend=H-1;\
      COLOR *cbeg=scanline[ybeg]+x;\
\
      int yt=ybeg;\
      COLOR *c=cbeg;REAL *z=zscanline[ybeg]+x;\
      for(;yt<=yend;c+=W,z+=W,yt++)\
      {\
        int actz=z0+(((yt-y1)*dzy)>>10);\
        if (actz>=*z) *z=actz,*c=color;\
      }\
    }\
}
*/

template<class COLOR,class HI> void ZBMP<COLOR,HI>::blit(ZBMP &src,int dx,int dy,HI dz) {

  int xbeg=-dx;if (xbeg<0) xbeg=0;
  int xend=W-dx;if (xend>src.W) xend=src.W;

  int ybeg=-dy;if (ybeg<0) ybeg=0;
  int yend=H-dy;if (yend>src.H) yend=src.H;

  for(int y=ybeg;y<yend;y++) {
    COLOR *ci=src.scanline[y]+xbeg;
    HI *hi=src.zscanline[y]+xbeg;
    COLOR *co=scanline[y+dy]+xbeg+dx;
    HI *ho=zscanline[y+dy]+xbeg+dx;
    for(int x=xbeg;x<xend;x++,hi++,ho++,ci++,co++) {
      HI newhi=*hi+dz;
      if (newhi>=*ho) *ho=newhi,*co=*ci;
/*      if (*ci!=src.transparent) {
        HI newhi=*hi+dz;
        if (*co==transparent or newhi>=*ho) *ho=newhi,*co=*ci;
      }*/
    }
  }
/*
  for(int x=0;x<src.W;x++) for(int y=0;y<src.H;y++)
    if (src.scanline[y][x])
      putpixel(x+dx,y+dy,src.zscanline[y][x]+dz,src.scanline[y][x]);
*/
}

template<class COLOR,class HI> template<class COLORER> void ZBMP<COLOR,HI>::triangle(XYHREC<HI> a,XYHREC<HI> b,XYHREC<HI> c,COLORER color) {
  if (a.x>b.x) exchange(a,b);
  if (b.x>c.x) exchange(b,c);
  if (a.x>b.x) exchange(a,b);
  //a,b,c srovnane podle osy x
  if (a.x==b.x) {
    if (a.y>b.y) exchange(a,b);
    xlefttriangle(a.x,c.x,a.y,b.y,c.y,a.z,b.z,c.z,color);
  }
  else if (b.x==c.x) {
    if (b.y>c.y) exchange(b,c);
    xrighttriangle(a.x,c.x,b.y,c.y,a.y,b.z,c.z,a.z,color);
  }
  else {
    int by=double(c.y-a.y)/double(c.x-a.x)*(b.x-a.x)+a.y;
    HI bz=double(c.z-a.z)/double(c.x-a.x)*(b.x-a.x)+a.z;
    if (by<b.y) {
      xrighttriangle(a.x,b.x,by,b.y,a.y,bz,b.z,a.z,color);
      xlefttriangle(b.x,c.x,by,b.y,c.y,bz,b.z,c.z,color);
    } else {
      xrighttriangle(a.x,b.x,b.y,by,a.y,b.z,bz,a.z,color);
      xlefttriangle(b.x,c.x,b.y,by,c.y,b.z,bz,c.z,color);
    }
  }
}
template<class COLOR,class HI> template<class COLORER> void ZBMP<COLOR,HI>::xrighttriangle(int x1,int x2,int y1,int y2,int y3,HI z1,HI z2,HI z3,COLORER color) {
  if (y1>=y2) return;
  float kyx1=float(y3-y1)/float(x2-x1);
  float kyx2=float(y3-y2)/float(x2-x1);
  float kzy=float(z2-z1)/float(y2-y1);
  HI imz=z1+kzy*(y3-y1);
  float kzx=float(z3-imz)/float(x2-x1);
  for(int x=x1;x<=x2;x++) {
    int dx=x-x1;
    int begy=y1+kyx1*dx,endy=y2+kyx2*dx;
    for(int y=begy;y<=endy;y++) {
      putpixel(x2-dx,y,imz+dx*kzx+(y-y3)*kzy,color);
    }
  }
}

template<class COLOR,class HI> template<class COLORER> void ZBMP<COLOR,HI>::xlefttriangle(int x1,int x2,int y1,int y2,int y3,HI z1,HI z2,HI z3,COLORER color){
  if (y1>=y2) return;
  float kyx1=float(y3-y1)/float(x2-x1);
  float kyx2=float(y3-y2)/float(x2-x1);
  float kzy=float(z2-z1)/float(y2-y1);
  HI imz=z1+kzy*(y3-y1);
  float kzx=float(z3-imz)/float(x2-x1);
  for(int x=x1;x<=x2;x++) {
    int dx=x-x1;
    int begy=y1+kyx1*dx,endy=y2+kyx2*dx;
    for(int y=begy;y<=endy;y++) {
      putpixel(x,y,imz+dx*kzx+(y-y3)*kzy,color);
    }
  }
}
template<class COLOR,class HI> void ZBMP<COLOR,HI>::reflectx() {
  for(int y=0;y<H;y++) {
    for(COLOR *left=scanline[y],*right=left+W-1;left<right;left++,right--) {
      COLOR pom=*left;*left=*right;*right=pom;
    }
    for(HI *left=zscanline[y],*right=left+W-1;left<right;left++,right--) {
      HI pom=*left;*left=*right;*right=pom;
    }
  }
}
template<class COLOR,class HI> void ZBMP<COLOR,HI>::reflecty() {
  for(int x=0;x<W;x++) {
    for(register ytop=0,ybot=H-1;ytop<ybot;ytop++,ybot--) {
      register COLOR pom=scanline[ytop][x];scanline[ytop][x]=scanline[ybot][x];scanline[ybot][x]=pom;
    }
    for(register ytop=0,ybot=H-1;ytop<ybot;ytop++,ybot--) {
      register HI pom=zscanline[ytop][x];zscanline[ytop][x]=zscanline[ybot][x];zscanline[ybot][x]=pom;
    }
  }
}


//template<class COLOR,class HI> void ZBMP<COLOR,HI>::xlefttriangle(int x1,int x2,int y1,int y2,int y3,HI z1,HI z2,HI z3,COLOR color)
//triangle(y1-,y2-)
//#undef triangle


}


#pragma package(smart_init)
