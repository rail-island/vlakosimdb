#include <jlib.h>
#pragma hdrstop

#include "voxels.h"

namespace zbuf {
#define voxel_tpl template<class COLOR,class REAL>
#define voxel_array VOXELARRAY<COLOR,REAL>

voxel_tpl void voxel_array::save(BINWRITE &fw,ConvertVoxel nativetoport,void *data) {
  fw.writedata("VXA",4);fw<<0<<size;//0:cislo verze
  voxel_t *act=(voxel_t*)block->lock();
  PortVoxel pv;
  for(int i=0;i<size;i++,act++) {
    nativetoport(data,pv,*act);
    fw.writedata(&pv,sizeof(pv));
  }
  block->unlock();
}
voxel_tpl void voxel_array::load(BINREAD &fr,ConvertVoxel porttonative,void *data) {
  char s[0x5];fr.readdata(s,4);s[4]=0;xassert(eqstr(s,"VXA"));
  int version;fr>>version;xassert(version==0);
  fr>>size;block->resize(size*sizeof(voxel_t));
  voxel_t *act=(voxel_t*)block->lock();
  PortVoxel pv;
  for(int i=0;i<size;i++,act++) {
    fr.readdata(&pv,sizeof(pv));
    porttonative(data,pv,*act);
  }
  block->unlock();
}


template<class COLOR,class REAL> template<class COLOR2> VOXELARRAY<COLOR2,REAL> *VOXELARRAY<COLOR,REAL>::linestoend() {
  VOXEL<COLOR2,REAL> *res=new VOXEL<COLOR2,REAL>[size],*w=res;
  for(int i=0;i<size;i++) if (not buf[i].color.isline) w->pt=buf[i].pt,w->color=buf[i].color.color,w++;
  for(int i=0;i<size;i++) if (buf[i].color.isline) w->pt=buf[i].pt,w->color=buf[i].color.color,w++;
  return new VOXELARRAY<COLOR2,REAL>(res,size,STEAL_MEM());
}

template<class COLOR,class REAL> void VOXELARRAY<COLOR,REAL>::transp(REMAP map) {
  voxel_t *buf=(voxel_t*)block->lock();
  for(int i=0;i<size;i++) {
    point3d<REAL> pt=buf[i].pt;
    buf[i].pt.coord(map[0])=pt.x;
    buf[i].pt.coord(map[1])=pt.y;
    buf[i].pt.coord(map[2])=pt.z;
  }
  block->unlock();
}

template<class COLOR,class REAL> point3d<REAL> VOXELARRAY<COLOR,REAL>::mincoords() {
  point3d<REAL> res;
  if (size==0) return res;
  voxel_t *buf=(voxel_t*)block->lock();
  res=buf[0].pt;
  for(int i=1;i<size;i++) {
    if (buf[i].pt.x<res.x) res.x=buf[i].pt.x;
    if (buf[i].pt.y<res.y) res.y=buf[i].pt.y;
    if (buf[i].pt.z<res.z) res.z=buf[i].pt.z;
  }
  block->unlock();
  return res;
}
template<class COLOR,class REAL> point3d<REAL> VOXELARRAY<COLOR,REAL>::maxcoords() {
  point3d<REAL> res;
  if (size==0) return res;
  voxel_t *buf=(voxel_t*)block->lock();
  res=buf[0].pt;
  for(int i=1;i<size;i++) {
    if (buf[i].pt.x>res.x) res.x=buf[i].pt.x;
    if (buf[i].pt.y>res.y) res.y=buf[i].pt.y;
    if (buf[i].pt.z>res.z) res.z=buf[i].pt.z;
  }
  block->unlock();
  return res;
}

template<class COLOR,class REAL> void VOXELARRAY<COLOR,REAL>::add(VOXELARRAY<COLOR,REAL> *src) {
  block->resize((size+src->size)*sizeof(voxel_t));
  voxel_t *dest=(voxel_t*)block->lock(),*psrc=(voxel_t*)src->block->lock();
  memcpy(dest+size,psrc,src->size*sizeof(voxel_t));
  src->block->unlock();block->unlock();
  size+=src->size;
}
template<class COLOR,class REAL> void VOXELARRAY<COLOR,REAL>::add(VOXELARRAY<COLOR,REAL> *src,const transform3d<float> &t) {
  block->resize((size+src->size)*sizeof(voxel_t));
  voxel_t *dest=(voxel_t*)block->lock(),*psrc=(voxel_t*)src->block->lock();
  dest+=size;
  for(int i=0;i<src->size;i++) dest[i].pt=t*psrc[i].pt,dest[i].color=psrc[i].color;
  src->block->unlock();block->unlock();
  size+=src->size;
}

template<class COLOR,class REAL> template<class HI> void VOXELARRAY<COLOR,REAL>::fromzbmp(ZBMP<COLOR,HI> *bmp,int x0,int y0,HI z0,float kx,float ky) {
  size=0;COLOR transparent=bmp->transparent;
  for(COLOR *c=bmp->buf,*cend=c+bmp->W*bmp->H;c<cend;c++)
    if (*c!=transparent) size++;
  block->resize(size*sizeof(voxel_t));
  voxel_t *wpix=(voxel_t*)block->lock();
  for(int y=0;y<bmp->H;y++) {
    register COLOR *c=bmp->scanline[y],*cend=c+bmp->W;
    register HI *h=bmp->zscanline[y],*hend=h+bmp->W;
    for(int x=0;c<cend;x++,c++,h++) if (*c!=transparent) {
      wpix->pt.x=x*kx-x0;wpix->pt.y=y*ky-y0;wpix->pt.z=z0+*h;
      wpix->color=*c;
      wpix++;
    }
  }
  block->unlock();
}

template<class COLOR,class REAL> template<class HI,class KREAL> void VOXELARRAY<COLOR,REAL>::blittozbmp(ZBMP<COLOR,HI> *bmp,int x0,int y0,HI z0,KREAL ky,KREAL kz) {
  voxel_t *buf=(voxel_t*)block->lock();
  for(voxel_t *act=buf,*end=buf+size;act<end;act++) {
    bmp->putpixel(x0+act->pt.x,y0+act->pt.y*ky-act->pt.z*kz,z0+act->pt.z,act->color);
  }
  block->unlock();
}
template<class COLOR,class REAL> template<class HI,class KREAL> void VOXELARRAY<COLOR,REAL>::blittozbmp(ZBMP<COLOR,HI> *bmp,int x0,int y0,HI z0,matrix3d<REAL> m,KREAL ky,KREAL kz) {
  voxel_t *buf=(voxel_t*)block->lock();
  for(voxel_t *act=buf,*end=buf+size;act<end;act++) {
    point3d<REAL> pt2=m*act->pt;
    bmp->putpixel(x0+pt2.x,y0+pt2.y*ky-pt2.z*kz,z0+pt2.z,act->color);
  }
  block->unlock();
}
const float line_dhi=0.7;
//const float line_dhi=0;
template<class COLOR,class REAL> template<class HI,class KREAL,class COLOR2>
  void VOXELARRAY<COLOR,REAL>::lineblittozbmp(ZBMP<COLOR2,HI> *bmp,int x0,int y0,HI z0,KREAL ky,KREAL kz) {
  voxel_t *buf=(voxel_t*)block->lock();
  for(voxel_t *act=buf,*end=buf+size;act<end;act++) {
    bmp->putpixel(x0+act->pt.x,y0+act->pt.y*ky-act->pt.z*kz,z0+act->pt.z+act->color.isline*line_dhi,act->color.color);
  }
  block->unlock();
}
template<class COLOR,class REAL> template<class HI,class KREAL,class COLOR2>
  void VOXELARRAY<COLOR,REAL>::lineblittozbmp(ZBMP<COLOR2,HI> *bmp,int x0,int y0,HI z0,matrix3d<REAL> m,KREAL ky,KREAL kz) {
  voxel_t *buf=(voxel_t*)block->lock();
  for(voxel_t *act=buf,*end=buf+size;act<end;act++) {
//    xassert(not act->color.isline);
    point3d<REAL> pt2=m*act->pt;
    bmp->putpixel(x0+pt2.x,y0+pt2.y*ky-pt2.z*kz,z0+pt2.z+act->color.isline*line_dhi,act->color.color);
  }
  block->unlock();
}



template<class COLOR,class REAL> void VOXELARRAY<COLOR,REAL>::translate(REAL dx,REAL dy,REAL dz) {
  voxel_t *buf=(voxel_t*)block->lock();
  for(voxel_t *act=buf,*end=buf+size;act<end;act++) act->pt.x+=dx,act->pt.y+=dy,act->pt.z+=dz;
  block->unlock();
}


#undef voxel_tpl
#undef voxel_array


}
#pragma package(smart_init)
