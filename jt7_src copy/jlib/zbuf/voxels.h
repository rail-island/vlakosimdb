//---------------------------------------------------------------------------

#ifndef voxelsH
#define voxelsH

#include <jmath/geom3d.h>
#include "bytebmp.h"
#include "bmp3d.h"

namespace zbuf {
template<class COLOR,class REAL> struct VOXELARRAY;
template<class COLOR,class HI> struct ZBMP;


struct PortVoxel {
  float x,y,z;
  uchar r,g,b,flags;
};

struct STEAL_MEM {};
template<class COLOR,class REAL> struct VOXELARRAY {
  typedef VOXEL<COLOR,REAL> voxel_t;
  typedef void (*ConvertVoxel)(void *data,PortVoxel &portvoxel,voxel_t &voxel);
//  voxel_t *buf;
  int size;JMemBlock *block;
  VOXELARRAY(JMemBlock *ablock,voxel_t *buf,int count) {
    block=ablock;size=count;
    block->resize(sizeof(voxel_t)*size);
    void *ptr=block->lock();
    memcpy(ptr,buf,sizeof(voxel_t)*count);
    block->unlock();
  }
  VOXELARRAY(JMemBlock *ablock) {block=ablock;size=0;}
  ~VOXELARRAY() {delete block;}// if (buf) delete []buf;}
  template<class HI,class KREAL> void blittozbmp(ZBMP<COLOR,HI> *bmp,int x0,int y0,HI z0,KREAL ky,KREAL kz);//"tycconsky" pohled
  template<class HI,class KREAL> void blittozbmp(ZBMP<COLOR,HI> *bmp,int x0,int y0,HI z0,matrix3d<REAL> m,KREAL ky,KREAL kz);//"tycconsky" pohled

  //dalsi dve funkce predpokladaji, za COLOR ma slozky isline a color, cary kresli o line_dhi vyse(aby prekryly ostatni)
  template<class HI,class KREAL,class COLOR2> void lineblittozbmp(ZBMP<COLOR2,HI> *bmp,int x0,int y0,HI z0,KREAL ky,KREAL kz);
  template<class HI,class KREAL,class COLOR2> void lineblittozbmp(ZBMP<COLOR2,HI> *bmp,int x0,int y0,HI z0,matrix3d<REAL> m,KREAL ky,KREAL kz);

  void translate(REAL dx,REAL dy,REAL dz);
  void save(string file,ConvertVoxel nativetoport,void *data) {GZWRITE fw(file.c_str());save(fw,nativetoport,data);}
  void save(BINWRITE &fw,ConvertVoxel nativetoport,void *data);
  VOXELARRAY(JMemBlock *ablock,BINREAD &fr,ConvertVoxel porttonative,void *data) {block=ablock;load(fr,porttonative,data);}
  VOXELARRAY(JMemBlock *ablock,string file,ConvertVoxel porttonative,void *data) {block=ablock;GZREAD fr(file.c_str());load(fr,porttonative,data);}
  void load(BINREAD &fr,ConvertVoxel porttonative,void *data);
  template<class HI> VOXELARRAY(JMemBlock *ablock,ZBMP<COLOR,HI> *bmp,int x0,int y0,HI z0,float kx,float ky) {
    block=ablock;size=0;fromzbmp(bmp,x0,y0,z0,kz,ky);
  }
  template<class HI> void fromzbmp(ZBMP<COLOR,HI> *bmp,int x0,int y0,HI z0,float kx,float ky);
  VOXELARRAY<COLOR,REAL> *clone() {
    return new VOXELARRAY<COLOR,REAL>(block->clone(),size,STEAL_MEM());
  }
  VOXELARRAY(const VOXELARRAY<COLOR,REAL> &src) {
    block=src.block->clone();size=src.size;
  }
  void add(VOXELARRAY<COLOR,REAL> *src,const transform3d<float> &t);
  void add(VOXELARRAY<COLOR,REAL> *src);
  point3d<REAL> mincoords();
  point3d<REAL> maxcoords();

  void reflectx(REAL x0) {
    voxel_t *buf=(voxel_t*)block->lock();
    for(int i=0;i<size;i++) buf[i].pt.x=x0-buf[i].pt.x;
    block->unlock();
  }
  void reflecty(REAL y0) {
    voxel_t *buf=(voxel_t*)block->lock();
    for(int i=0;i<size;i++) buf[i].pt.y=y0-buf[i].pt.y;
    block->unlock();
  }
  void reflectz(REAL z0) {
    voxel_t *buf=(voxel_t*)block->lock();
    for(int i=0;i<size;i++) buf[i].pt.z=z0-buf[i].pt.z;
    block->unlock();
  }
  void transp(REMAP map);//x prejde na map[0],y na map[1],z na map[2]

  template<class COLOR2> VOXELARRAY<COLOR2,REAL> *linestoend();
  //COLOR musi mit polozku color a isline,vyrobi VOXELARRAY z polozek

//  VOXELARRAY(voxel_t *abuf,int count,STEAL_MEM) {buf=abuf;size=count;}
  VOXELARRAY(JMemBlock *ablock,int count,STEAL_MEM) {block=ablock;size=count;}
  void foreach(void (*fn)(REAL &x,REAL &y,REAL &z,void *data),void *data) {
    voxel_t *buf=(voxel_t*)block->lock();
    for(int i=0;i<size;i++) fn(buf[i].pt.x,buf[i].pt.y,buf[i].pt.z,data);
    block->unlock();
  }
  REAL diam() {//horni odhad pro prumer
    point3d<REAL> min=mincoords(),max=maxcoords();
    REAL dx=min.x-max.x,dy=min.y-max.y,dz=min.z-max.z;
    return sqrt(dx*dx+dy*dy+dz*dz);
  }
};


}
//---------------------------------------------------------------------------
#endif
