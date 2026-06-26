#include <jlib.h>
#pragma hdrstop

#include "bytebmp.h"
//#include <zbuf/zbmp.h>

#define bassert(condition) {if(!(condition)) throw "BError";}

namespace zbuf {

template<class COLOR> BYTEBMP<COLOR>::BYTEBMP(int w,int h,COLOR transparentcolor,bool areversey,int alignelements) {
  if (w<0) w=0;if (h<0) h=0;
  reversey=areversey;
  transparent=transparentcolor;
  W=w;H=h;
  if (alignelements<1) alignelements=1;
  alignedW=W;
  while (alignedW%alignelements) alignedW++;
  size=H*alignedW;
  buf=new COLOR[size];
  scanline=new COLOR*[H];
  if (reversey) for(int i=H-1,ofs=0;i>=0;i--,ofs+=alignedW) scanline[i]=buf+ofs;
  else for(int i=0,ofs=0;i<H;i++,ofs+=alignedW) scanline[i]=buf+ofs;
  clear(transparent);
}
template<class COLOR> BYTEBMP<COLOR>::~BYTEBMP() {
  delete []scanline;delete []buf;
}

template<class COLOR> void BYTEBMP<COLOR>::
  copyrect(const BYTEBMP<COLOR> &src,int destx,int desty,int srcx,int srcy,int w,int h) {
  xassert(srcx>=0);xassert(srcy>=0);//xassert(destx>=0);xassert(desty>=0);
  if (destx<0) w+=destx,srcx-=destx,destx=0;
  if (desty<0) h+=desty,srcy-=desty,desty=0;

  if (W-destx<w) w=W-destx;if (src.W-srcx<w) w=src.W-srcx;
  if (H-desty<h) h=H-desty;if (src.H-srcy<h) h=src.H-srcy;
  if (w>0) for(int y=0;y<h;y++) {
    memcpy(scanline[y+desty]+destx,src.scanline[y+srcy]+srcx,sizeof(COLOR)*w);
  }
}

template<class COLOR> void BYTEBMP<COLOR>::
  transparentcopyrect(const BYTEBMP<COLOR> &src,int destx,int desty,int srcx,int srcy,int w,int h) {
  xassert(srcx>=0);xassert(srcy>=0);
  if (destx<0) w+=destx,srcx-=destx,destx=0;
  if (desty<0) h+=desty,srcy-=desty,desty=0;
//  xassert(destx>=0);xassert(desty>=0);
  if (W-destx<w) w=W-destx;if (src.W-srcx<w) w=src.W-srcx;
  if (H-desty<h) h=H-desty;if (src.H-srcy<h) h=src.H-srcy;
  register COLOR transparent=src.transparent;
  if (transparent) {
    if (w>0) for(int y=0;y<h;y++) {
      COLOR *from=src.scanline[y+srcy]+srcx,*to=scanline[y+desty]+destx;
      for(int x=0;x<w;x++,from++,to++) if (*from!=transparent) *to=*from;
    }
  } else {
    if (w>0) for(int y=0;y<h;y++) {
      COLOR *from=src.scanline[y+srcy]+srcx,*to=scanline[y+desty]+destx;
      for(int x=0;x<w;x++,from++,to++) if (*from) *to=*from;
    }
  }
}

template<class COLOR> template<class INDEXCOLOR> void BYTEBMP<COLOR>::
  transparentcopyrectpalette(const BYTEBMP<INDEXCOLOR> &src,int destx,int desty,int srcx,int srcy,int w,int h,COLOR colors[]) {
  xassert(srcx>=0);xassert(srcy>=0);xassert(destx>=0);xassert(desty>=0);
  if (W-destx<w) w=W-destx;if (src.W-srcx<w) w=src.W-srcx;
  if (H-desty<h) h=H-desty;if (src.H-srcy<h) h=src.H-srcy;
  register COLOR transparent=src.transparent;
  if (transparent) {
    if (w>0) for(int y=0;y<h;y++) {
      INDEXCOLOR *from=src.scanline[y+srcy]+srcx;
      COLOR *to=scanline[y+desty]+destx;
      for(int x=0;x<w;x++,from++,to++) if (*from!=transparent) *to=colors[*from];
    }
  } else {
    if (w>0) for(int y=0;y<h;y++) {
      INDEXCOLOR *from=src.scanline[y+srcy]+srcx;
      COLOR *to=scanline[y+desty]+destx;
      for(int x=0;x<w;x++,from++,to++) if (*from) *to=colors[*from];
    }
  }
}


template<class COLOR> void BYTEBMP<COLOR>::assign(BYTEBMP<COLOR> &srcbmp) {
  xassert(size==srcbmp.size);
//  xassert(W==srcbmp.W);xassert(H==srcbmp.H);
  memcpy(buf,srcbmp.buf,size*sizeof(COLOR));
}

template<class COLOR> void BYTEBMP<COLOR>::blit(BYTEBMP &src,int dx,int dy) {
//  copyrect(src,dx,dy,0,0,src.W,src.H);

  int xbeg=-dx;if (xbeg<0) xbeg=0;
  int xend=W-dx;if (xend>src.W) xend=src.W;

  int ybeg=-dy;if (ybeg<0) ybeg=0;
  int yend=H-dy;if (yend>src.H) yend=src.H;

  for(int y=ybeg;y<yend;y++) {
    COLOR *ci=src.scanline[y]+xbeg;
    COLOR *co=scanline[y+dy]+xbeg+dx;
    for(int x=xbeg;x<xend;x++) *co++=*ci++;
  }
}



/*template<class COLOR,class REAL> PIXELARRAY<COLOR,REAL>::PIXELARRAY(pixel_t *data,int acount) {
  count=acount;
  pixels=new pixel_t[count];
  memcpy(pixels,data,count*sizeof(pixel_t));
}*/

template<class COLOR> void BYTEBMP<COLOR>::circle(int mx,int my,int radius,COLOR color,double start,double stop) {
  for(double t=start,dt=1/(fabs(start-stop)*radius);t<stop+dt;t+=dt) putpixel(mx+radius*cos(t),my+radius*sin(t),color);
}

template<class COLOR> void BYTEBMP<COLOR>::fellipse(double mx,double my,double radiusx,double radiusy,COLOR color,double start,double stop) {
  if (radiusx>0 and radiusy>0) {
    for(double t=start,dt=1/(fabs(start-stop)*max(radiusx,radiusy));t<stop+dt;t+=dt)   {
      double x=mx+radiusx*cos(t),y=my+radiusy*sin(t);
      putpixel(x,y,color);
    }
  }
}
/*template<class COLOR> void BYTEBMP<COLOR>::ellipse(int x1,int y1,int x2,int y2,COLOR color) {
  for(double t=0,dt=1/(fabs(start-stop)*max(radiusx,radiusy));t<stop+dt;t+=dt)   {
    double x=mx+radiusx*cos(t),y=my+radiusy*sin(t);
    putpixel(x,y,color);
  }
  int midx=(x1+x2)/2,midy=(y1+y2)/2;
  double a=(x2-x1)/2.0+0.5,b=(y2-y1)/2.0+0.5,a2=a*a,b2=b*b;
  for(int x=x1;x<=x2;x++) for(int y=y1;y<=y2;y++) {
    int dx=x-midx,dy=y-midy;
    if (dx*dx/a2+dy*dy/b2<=1.0) putpixel(x,y,color);
  }
}*/

template<class COLOR> void BYTEBMP<COLOR>::fline(double x1,double y1,double x2,double y2,COLOR color) {
  double dx=x2-x1,dy=y2-y1;
  if (dx!=0 or dy!=0) {
    double step;
    if (dx==0) step=fabs(1/dy);
    else if (dy==0) step=fabs(1/dx);
    else step=min(fabs(1/dx),fabs(1/dy));
    for(double t=0;t<=1;t+=step) putpixel(x1+dx*t,y1+dy*t,color);
  }
}

template<class COLOR> void BYTEBMP<COLOR>::line(int x1,int y1,int x2,int y2,COLOR color) {
  int Dx=x2-x1,Dy=y2-y1;
  if (Dx or Dy) {//neni to jen bod
    if (abs(Dx)>=abs(Dy)) {
      if (x1>x2) line_de_x(x2,y2,x1,y1,color);else line_de_x(x1,y1,x2,y2,color);
    } else {
      if (y1>y2) line_de_y(x2,y2,x1,y1,color);else line_de_y(x1,y1,x2,y2,color);
    }
  }
}

template<class COLOR> void BYTEBMP<COLOR>::line_de_x(int x1,int y1,int x2,int y2,COLOR color) {
  int Dx=x2-x1;bassert(Dx>0);
  int Dy=y2-y1;
  int dy=(Dy<<10)/Dx;

    int end=Dx;if (end+x1>=W) end=W-x1-1;
    for(int t=max(0,-x1);t<=end;t++)
    {
      int acty=y1+((dy*t)>>10);
      if (acty<0 or acty>=H) continue;
      scanline[acty][x1+t]=color;
    }
}

template<class COLOR> void BYTEBMP<COLOR>::line_de_y(int x1,int y1,int x2,int y2,COLOR color) {
  int Dy=y2-y1;bassert(Dy>0);
  int Dx=x2-x1;
  int dx=(Dx<<10)/Dy;
    int end=Dy;if (end+y1>=H) end=H-y1-1;
    for(int t=max(0,-y1);t<=end;t++)
    {
      int actx=x1+((dx*t)>>10);
      if (actx<0 or actx>=W) continue;
      scanline[t+y1][actx]=color;
    }
}

template<class COLOR> void BYTEBMP<COLOR>::fillrect(int x1,int y1,int x2,int y2,COLOR color) {
  makeless(x1,x2);makeless(y1,y2);
  if (x1<0) x1=0;if (y1<0) y1=0;if (x2>=W) x2=W-1;if (y2>=H) y2=H-1;
  for(int y=y1;y<=y2;y++)
    for(register COLOR *c=scanline[y]+x1,*cend=c+x2-x1;c<=cend;c++) *c=color;
}

template<class COLOR> void BYTEBMP<COLOR>::rectangle(int x1,int y1,int x2,int y2,COLOR color) {
  int X1=x1,X2=x2,Y1=y1,Y2=y2;
  makeless(X1,X2);makeless(Y1,Y2);
  if (X1<0) X1=0;if (Y1<0) Y1=0;if (X2>=W) X2=W-1;if (Y2>=H) Y2=H-1;
  COLOR *c;int t;
  int dW=reversey?-alignedW:alignedW;
  if (y1>=0 and y1<H and X1<X2)
    for(c=scanline[y1]+X1,t=X1;t<=X2;t++,c++) *c=color;
  if (y2>=0 and y2<H and X1<X2)
    for(c=scanline[y2]+X1,t=X1;t<=X2;t++,c++) *c=color;
  if (x1>=0 and x1<W and Y1<Y2)
    for(c=scanline[Y1]+x1,t=Y1;t<=Y2;t++,c+=dW) *c=color;
  if (x2>=0 and x2<W and Y1<Y2)
    for(c=scanline[Y1]+x2,t=Y1;t<=Y2;t++,c+=dW) *c=color;
}

template<class COLOR> void BYTEBMP<COLOR>::transp() {
  int Nw=H,Nh=W;
  COLOR *Nbuf=new COLOR[Nw*Nh];
  for(int y=0;y<H;y++) {
    for(COLOR *c=scanline[y],*w=Nbuf+y,*cend=c+W;c<cend;c++,w+=Nw) *w=*c;
  }
  if (W!=H) {delete []scanline;scanline=new COLOR*[Nh];}
  for(int i=0,ofs=0;i<Nh;i++,ofs+=Nw) scanline[i]=Nbuf+ofs;
  delete []buf;
  buf=Nbuf;
  W=Nw;H=Nh;
}

template<class COLOR> void BYTEBMP<COLOR>::reflectx() {
  for(int y=0;y<H;y++) {
    for(register COLOR *left=scanline[y],*right=left+W-1;left<right;left++,right--) {
      register COLOR pom=*left;*left=*right;*right=pom;
    }
  }
}
template<class COLOR> void BYTEBMP<COLOR>::reflecty() {
  for(int x=0;x<W;x++) {
    for(register ytop=0,ybot=H-1;ytop<ybot;ytop++,ybot--) {
      register COLOR pom=scanline[ytop][x];scanline[ytop][x]=scanline[ybot][x];scanline[ybot][x]=pom;
    }
  }
}

/*template<class COLOR,class REAL> PIXELARRAY<COLOR,REAL>::PIXELARRAY(BYTEBMP<COLOR> *bmp,int x0,int y0) {
  count=0;
  for(COLOR *c=bmp->buf,*cend=c+bmp->W*bmp->H;c<cend;c++)
    if (*c) count++;
  pixels=new pixel_t[count];
  pixel_t *wpix=pixels;
  for(int y=0;y<bmp->H;y++) {
    register COLOR *c=bmp->scanline[y],*cend=c+bmp->W;
    for(int x=0;c<cend;x++,c++) if (*c) {
      wpix->x=x-x0;wpix->y=y-y0;wpix->color=*c;wpix++;
    }
  }
}
template<class COLOR,class REAL> PIXELARRAY<COLOR,REAL>::PIXELARRAY(BYTEBMP<COLOR> *bmp,int x0,int y0,float kx,float ky) {
  count=0;
  for(COLOR *c=bmp->buf,*cend=c+bmp->W*bmp->H;c<cend;c++)
    if (*c) count++;
  pixels=new pixel_t[count];
  pixel_t *wpix=pixels;
  for(int y=0;y<bmp->H;y++) {
    register COLOR *c=bmp->scanline[y],*cend=c+bmp->W;
    for(int x=0;c<cend;x++,c++) if (*c) {
      wpix->x=x*kx-x0;wpix->y=y*ky-y0;wpix->color=*c;wpix++;
    }
  }
}*/

/*
template<class COLOR,class REAL> template<class HI> void PIXELARRAY<COLOR,REAL>::blittozbmp(ZBMP<COLOR,HI> *zbmp,int x0,int y0,HI z) {
  for(pixel_t *act=pixels,*stop=pixels+count;act<stop;act++)
    zbmp->putpixel(x0+act->x,y0+act->y,z,act->color);
//    zbmp->putpixel(x0+round(act->x),y0+round(act->y),z,act->color);
}
template<class COLOR,class REAL> template<class HI>
  void PIXELARRAY<COLOR,REAL>::blittozbmp(
    ZBMP<COLOR,HI> *zbmp,int x0,int y0,HI z0,float kzx,float kzy,const agmatrix<float> &mat,const Projector<HI> &prj
  ) {
  for(pixel_t *act=pixels,*stop=pixels+count;act<stop;act++) {
    agpoint<float> pt(act->x,act->y);
    HI z=z0+act->x*kzx+act->y*kzy;
    pt=mat*pt;
    point3d<float> pt3(pt,z);
    XYHREC<HI> xyh=prj(pt3);
    zbmp->putpixel(xyh.x+x0,xyh.y+y0,z,act->color);
//    zbmp->putpixel(round(pt.x)+x0,round(pt.y)+y0,z,act->color);
  }
}


template<class COLOR,class REAL> template<class HI> void PIXELARRAY<COLOR,REAL>::blittozbmp(ZBMP<COLOR,HI> *zbmp,int x0,int y0,int ky,HI z) {
  for(pixel_t *act=pixels,*stop=pixels+count;act<stop;act++)
    zbmp->putpixel(x0+act->x,y0+((act->y*ky)>>10),z,act->color);
}*/

template<class COLOR> void BYTEBMP<COLOR>::fillellipse(int x1,int y1,int x2,int y2,COLOR color) {
  makeless(x1,x2);makeless(y1,y2);
  double midx=(x1+x2)/2.0,midy=(y1+y2)/2.0;
  double a=(x2-x1)/2.0,b=(y2-y1)/2.0,a2=a*a,b2=b*b;
  if (a2>0 and b2>0) {
    for(int x=x1;x<=x2;x++) for(int y=y1;y<=y2;y++) {
      double dx=x-midx,dy=y-midy;
      if (dx*dx/a2+dy*dy/b2<=1.0) putpixel(x,y,color);
    }
  }
  fellipse(midx,midy,a,b,color);
}
template<class COLOR> void BYTEBMP<COLOR>::drawpolygon(const XYREC *pts,int count,COLOR color,bool onlypolyline) {
  for(int i=0;i<count-1;i++) line(pts[i],pts[i+1],color);
  if (count>2 and not onlypolyline) line(pts[0],pts[count-1],color);
}
template<class COLOR> void BYTEBMP<COLOR>::fillpolygon(const XYREC *pts,int count,COLOR color) {
  struct PBUF {
    bool *buf;
    int w,h;
    int minx,miny,maxx,maxy;
    PBUF(int aminx,int aminy,int amaxx,int amaxy) {
      minx=aminx;maxx=amaxx;miny=aminy;maxy=amaxy;
      w=maxx-minx+1,h=maxy-miny+1;
      buf=new bool[w*h];
      for(int i=0;i<w*h;i++) buf[i]=false;
    }
    ~PBUF() {delete []buf;}
    bool &at(int x,int y) {
      int idx=(y-miny)*w+x-minx;
      return buf[idx];
    }
    void hline(int x0,int y0) {
      for(int x=x0;x<=maxx;x++) at(x,y0)=not at(x,y0);
    }
  };
  struct LINES {
    const XYREC *pts;
    int count;
    LINES(const XYREC *apts,int acount) {pts=apts;count=acount;}
    const XYREC &pt(int index) {return pts[index%count];}
    int dy0(int index) {
      int res;
      do {
        res=pt(index+1).y-pt(index).y;
        index++;
      } while (res==0);
      return res;
    }
  } lines(pts,count);

  drawpolygon(pts,count,color);

  if (count<=2) return;
  int minx=INT_MAX,miny=INT_MAX,maxx=INT_MIN,maxy=INT_MIN;
  for(int i=0;i<count;i++) {
    if (pts[i].x<minx) minx=pts[i].x;
    if (pts[i].y<miny) miny=pts[i].y;
    if (pts[i].x>maxx) maxx=pts[i].x;
    if (pts[i].y>maxy) maxy=pts[i].y;
  }
  if (minx==maxx or miny==maxy) return;
  PBUF pbuf(minx,miny,maxx,maxy);
//  int horliney=-1,leftx;
  for(int i=0;i<count;i++) {
    XYREC a=lines.pt(i),b=lines.pt(i+1);
    int dx=b.x-a.x,dy=b.y-a.y;
    if (dy!=0) {
//      if (horliney>=0) pbuf.hline(leftx,horliney),horliney=-1;
      int sgny=dy>0?1:-1;
      double k=double(dx)/dy;
      for(int y=a.y;y!=b.y;y+=sgny) {
        pbuf.hline(a.x+k*(y-a.y),y);
      }
      if (lines.dy0(i+1)*dy<0) pbuf.hline(b.x,b.y);
    } /*else {
      if (horliney<0) horliney=a.y,leftx=a.x;
      if (a.x<leftx) leftx=a.x;
      if (b.x<leftx) leftx=b.x;
    }*/
  }
//  if (horliney>=0) pbuf.hline(leftx,horliney);
  for(int x=minx;x<=maxx;x++)
    for(int y=miny;y<=maxy;y++)
      if (pbuf.at(x,y)) putpixel(x,y,color);
}


}

#pragma package(smart_init)
