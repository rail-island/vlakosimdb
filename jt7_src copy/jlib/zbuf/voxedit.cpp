#include <jlib.h>
#pragma hdrstop

#include "voxedit.h"
#include <jmath/geom3d.cpp>
#include <zbuf/bmp3d.cpp>
#include <zbuf/voxels.cpp>

namespace zbuf {

void VXACompoud::blittovxa(Voxels *dest,VXAEdit *edit) {
  for(int i=0;i<items.count;i++) {
    VoxItem *it=items[i];
    transform3d<float> t;
    if (edit and edit->item==it) {
      if (edit->hidden) continue;
      t=edit->gett();
    }
    it->blit(dest,t);
  }
}

static VoxItem *loadvxa(XMLItem &xml) {
  if (xml.name=="insert") return new VXAItem(xml);
  return NULL;
}

void VXACompoud::load(XMLItem &xml) {
  items.clear();
  for(int i=0;i<xml.count();i++) if (VoxItem *vox=loadvxa(xml[i])) items.add(vox);
}
void VXACompoud::save(XMLItem &xml) {
  xml.name="vxa_compoud";
  for(int i=0;i<items.count;i++) items[i]->save(xml.add());
}


}

#pragma package(smart_init)
