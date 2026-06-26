#include <jlib.h>
#pragma hdrstop

#include "bmp3d.h"
#include "zbmp.h"
#include "voxels.h"

namespace zbuf {


template<class COLOR> template<class REAL> VOXELARRAY<COLOR,REAL> *BMP3D<COLOR>::shape(REAL dx,REAL dy,REAL dz,double koef) {
//   *pbuf=new VOXEL<COLOR,REAL>[xs*ys*zs],*w=pbuf;
  TCntVec<VOXEL<COLOR,REAL> > pbuf;
  for(int x=0;x<xs;x++)
    for(int y=0;y<=ys;y++)
      for(int z=0;z<=zs;z++)
        if (vget(x,y,z)!=transparent and hastransparent(x,y,z)) {
          pbuf.add(VOXEL<COLOR,REAL>(x*koef+dx,y*koef+dy,z*koef+dz,vget(x,y,z)));
//          if (COLOR c=vget(x+1,y,z)) pbuf.add(VOXEL<COLOR,REAL>(x+0.5,y,z,c));
//          if (COLOR c=vget(x,y+1,z)) pbuf.add(VOXEL<COLOR,REAL>(x,y+0.5,z,c));
//          if (COLOR c=vget(x,y,z+1)) pbuf.add(VOXEL<COLOR,REAL>(x,y,z+0.5,c));
        }
//          *w++=VOXEL<COLOR,REAL>(x,y,z,vget(x,y,z));
//  VOXELARRAY<COLOR,REAL> *res=new VOXELARRAY<COLOR,REAL>(pbuf,w-pbuf);
  VOXELARRAY<COLOR,REAL> *res=new VOXELARRAY<COLOR,REAL>(jsystem->tempblock(),pbuf.buf,pbuf.count);
//  delete []pbuf;
  return res;
}

template<class COLOR> void BMP3D<COLOR>::fillaround(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color) {
  for(int x=0;x<xs;x++)
    for(int y=0;y<ys;y++)
      for(int z=0;z<zs;z++)
        if (x<x1 or y<y1 or z<z1 or x>x2 or y>y2 or z>z2)
          at(x,y,z)=color;
}

template<class COLOR> void BMP3D<COLOR>::zcut(BYTEBMP<COLOR> &bmp,int dx,int dy,REMAP map) {
  int begx=max(dx,0),endx=min(bmp.W+dx,bmpsize.x(map.inv()));
  int begy=max(dy,0),endy=min(bmp.H+dy,bmpsize.y(map.inv()));
  for(int x=begx;x<endx;x++)
    for(int y=begy;y<endy;y++)
      if (bmp.scanline[y-dy][x-dx]==bmp.transparent)
        for(int z=0;z<bmpsize.z(map.inv());z++) vset(VPOINT(x,y,z),map,transparent);
  fillaround(VPOINT(begx,begy,0),VPOINT(endx-1,endy-1,bmpsize.z(map.inv())-1),map,transparent);
}
template<class COLOR> void BMP3D<COLOR>::zcolor(BYTEBMP<COLOR> &bmp,int dx,int dy,REMAP map,int numcolor) {
  int begx=max(dx,0),endx=min(bmp.W+dx,bmpsize.x(map.inv()));
  int begy=max(dy,0),endy=min(bmp.H+dy,bmpsize.y(map.inv()));
  for(int x=begx;x<endx;x++)
    for(int y=begy;y<endy;y++) {
      COLOR color=bmp.scanline[y-dy][x-dx];
      if (color!=bmp.transparent) {
        int num=numcolor;
        for(int z=0;(z<bmpsize.z(map.inv())) and num;z++) {
          if (vget(VPOINT(x,y,z),map)!=transparent) {
            vset(VPOINT(x,y,z),map,color);
            num--;
          }
        }
      }
    }
}
template<class COLOR> void BMP3D<COLOR>::zcolorback(BYTEBMP<COLOR> &bmp,int dx,int dy,REMAP map,int numcolor) {
  int begx=max(dx,0),endx=min(bmp.W+dx,bmpsize.x(map.inv()));
  int begy=max(dy,0),endy=min(bmp.H+dy,bmpsize.y(map.inv()));
  for(int x=begx;x<endx;x++)
    for(int y=begy;y<endy;y++) {
      COLOR color=bmp.scanline[y-dy][x-dx];
      if (color!=bmp.transparent) {
        int num=numcolor;
        for(int z=bmpsize.z(map.inv())-1;(z>=0) and num;z--) {
          if (vget(VPOINT(x,y,z),map)!=transparent) {
            vset(VPOINT(x,y,z),map,color);
            num--;
          }
        }
      }
    }
}

/*
template<class COLOR> void BMP3D<COLOR>::zcut(BYTEBMP<COLOR> &bmp,int dx,int dy) {
  int begx=max(dx,0),endx=min(bmp.W+dx,xs);
  int begy=max(dy,0),endy=min(bmp.H+dy,ys);
  for(int x=begx;x<endx;x++)
    for(int y=begy;y<endy;y++)
      if (bmp.scanline[y-dy][x-dx]==bmp.transparent)
        for(int z=0;z<zs;z++) vset(x,y,z,transparent);
  fillaround(begx,begy,0,endx-1,endy-1,zs-1,transparent);
}

template<class COLOR> template<class REAL> VOXELARRAY<COLOR,REAL> *BMP3D<COLOR>::qualityshape(int quality) {
  TCntVec<VOXEL<COLOR,REAL> > pbuf;
//  VOXEL<COLOR,REAL> *pbuf=new VOXEL<COLOR,REAL>[xs*ys*zs],*w=pbuf;
  for(int x=xmin();x<=xmax();x++)
    for(int y=ymin();y<=ymax();y++)
      for(int z=zmin();z<=zmax();z++)
        if (vget(x,y,z)!=transparent and hastransparent(x,y,z,quality))
          pbuf.add(VOXEL<COLOR,REAL>(x,y,z,vget(x,y,z)));
//          *w++=VOXEL<COLOR,REAL>(x,y,z,vget(x,y,z));
//  VOXELARRAY<COLOR,REAL> *res=new VOXELARRAY<COLOR,REAL>(pbuf,w-pbuf);
  VOXELARRAY<COLOR,REAL> *res=new VOXELARRAY<COLOR,REAL>(pbuf.buf,pbuf.count);
//  delete []pbuf;
  return res;
}

template<class COLOR> bool BMP3D<COLOR>::hastransparent(int x,int y,int z,int maxdist) {
  for(int i=x-maxdist;i<=x+maxdist;i++)
    for(int j=y-maxdist;j<=y+maxdist;j++)
      for(int k=z-maxdist;k<=z+maxdist;k++)
        if (i!=x or j!=y or k!=z) if (vget(i,j,k)==transparent) return true;
  return false;
}
*/

template<class COLOR> BMP3D<COLOR>::BMP3D(int xsize,int ysize,int zsize,COLOR transparentcolor):bmpsize(xsize,ysize,zsize),xs(bmpsize.crds[0]),ys(bmpsize.crds[1]),zs(bmpsize.crds[2]) {
  transparent=transparentcolor;
  bufsize=xs*ys*zs;
  buf=new COLOR[bufsize];
  xscanline=new COLOR*[ys*zs];
  scanline=new COLOR**[zs];
  COLOR *act=buf;
  COLOR **actx=xscanline;
  for(int z=0;z<zs;z++) {
    scanline[z]=actx;
    for(int y=0;y<ys;y++) {
      *actx=act;act+=xs;actx++;
    }
  }
  clear(transparent);
}

template<class COLOR> void BMP3D<COLOR>::fillbox(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color) {
  if (z1<0) z1=0;if (z2>=zs) z2=zs-1;
  if (y1<0) y1=0;if (y2>=ys) y2=ys-1;
  if (x1<0) x1=0;if (x2>=xs) x2=xs-1;
  for(int z=z1;z<=z2;z++)
    for(int y=y1;y<=y2;y++)
      for(COLOR *beg=scanline[z][y],*act=beg+x1,*end=beg+x2;act<=end;act++)
        *act=color;
}
template<class COLOR> void BMP3D<COLOR>::drawframe(int x1,int y1,int z1,int x2,int y2,int z2,COLOR color) {
  for(int x=x1;x<=x2;x++) vset(x,y1,z1,color),vset(x,y1,z2,color),vset(x,y2,z1,color),vset(x,y2,z2,color);
  for(int y=y1;y<=y2;y++) vset(x1,y,z1,color),vset(x1,y,z2,color),vset(x2,y,z1,color),vset(x2,y,z2,color);
  for(int z=z1;z<=z2;z++) vset(x1,y1,z,color),vset(x2,y1,z,color),vset(x1,y2,z,color),vset(x2,y2,z,color);
/*  for(int z=z1;z<=z2;z++) for(int y=y1;y<=y2;y++) vset(x1,y,z,color),vset(x2,y,z,color);
  for(int x=x1;x<=x2;x++) for(int y=y1;y<=y2;y++) vset(x,y,z1,color),vset(x,y,z1,color);
  for(int x=x1;x<=x2;x++) for(int z=z1;z<=z2;z++) vset(x,y1,z,color),vset(x,y2,z,color);*/
}



}

#pragma package(smart_init)

