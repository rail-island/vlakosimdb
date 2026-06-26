#ifndef voxeditH
#define voxeditH

#include "bmp3d.h"
#include "voxels.h"
#include <jmath/geom3d.h>
#include <ext/xmldoc.h>
#include <com/memobjs.h>

namespace zbuf {

extern sFile vxadir;


typedef VOXEL<COLORANDLINE<int>,float> VVoxel;
typedef VOXELARRAY<COLORANDLINE<int>,float> VVoxels;


inline void portvoxeltonativevoxel(void *,PortVoxel &pv,VVoxel &vox) {
  vox.color.color=gr::reversergb(gr::makergb(pv.r,pv.g,pv.b));
  vox.pt.x=pv.x;
  vox.pt.y=pv.y;
  vox.pt.z=pv.z;
  vox.color.isline=pv.flags;
}
inline void nativevoxeltoportvoxel(void *,PortVoxel &pv,VVoxel &vox) {
  pv.x=vox.pt.x;
  pv.y=vox.pt.y;
  pv.z=vox.pt.z;
  pv.flags=vox.color.isline;
  int bgr=gr::reversergb(vox.color.color);
  pv.r=gr::nativervalue(bgr);
  pv.g=gr::nativegvalue(bgr);
  pv.b=gr::nativebvalue(bgr);
}


struct VoxItem {
//  typedef VOXELARRAY<uchar,float> Voxels;
  typedef VVoxels Voxels;
  transform3d<float> t;
  Voxels *voxs;
  void blit(Voxels *dest,const transform3d<float> &edit_t) {if (voxs) dest->add(voxs,edit_t*t);}
  VoxItem() {voxs=NULL;}
  virtual void editprops() {}
  VoxItem(XMLItem &xml) {voxs=NULL;t.load(xml["transform3d"]);}
  virtual string getname()=0;
  ~VoxItem() {delete voxs;}
  virtual void save(XMLItem &xml) {t.save(xml.add());}
};

struct VXAItem:VoxItem {
  string file;
  VXAItem(string afile) {
    file=afile;
    voxs=new Voxels(jsystem->tempblock(),file,portvoxeltonativevoxel,0);
  }
  void editprops() {
    jsystem->editvalues("Vlastnosti vlozeneho VXA",'o',&file,"Soubor",0);
  }
  string getname() {return fmtstr("insert %s",sFile(file).getfile().c_str());}
  void save(XMLItem &xml) {
    VoxItem::save(xml);
    xml.name="insert";xml.vars.setvar("file",sFile(file).relto(vxadir));
  }
  VXAItem(XMLItem &xml):VoxItem(xml) {
    file=xml.svars["file"];
    voxs=new Voxels(jsystem->tempblock(),vxadir/file,portvoxeltonativevoxel,0);
  }
};

struct VXAEdit {
  bool hidden;
  VoxItem *item;
  float dx,dy,dz;
  float angle;
  int os;//0:kolem x,1:kolem y,2:kolem z
  transform3d<float> gett() {
    matrix3d<float> mat;
    if (os==0) mat.dorotatex(angle);
    if (os==1) mat.dorotatey(angle);
    if (os==2) mat.dorotatez(angle);
    return transform3d<float>(mat,vector3d<float>(dx,dy,dz));
  }
  void init() {dx=dy=dz=0;angle=0;os=-1;item=NULL;hidden=false;}
};

struct VXACompoud {
//  typedef VOXELARRAY<uchar,float> Voxels;
  typedef VoxItem::Voxels Voxels;
  OWNER<Voxels> voxs;
  IObjArray<VoxItem> items;
  void blittovxa(Voxels *dest,VXAEdit *edit=NULL);
  void load(XMLItem &xml);
  void save(XMLItem &xml);
};

}

#endif
