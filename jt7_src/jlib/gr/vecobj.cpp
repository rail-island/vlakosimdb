#include <jlib.h>
#pragma hdrstop

#include "vecobj.h"
#include "comgr.h"
#include <com/mzip.h>

namespace gr {
using namespace jlib;


VECELEM *VECOBJ::readelem(BINREAD &fr) {
  int type;fr>>type;
  switch (type) {
    case ve_line:return new VELINE(fr);
    case ve_drect:return new VEDRAWRECTANGLE(fr);
    case ve_frect:return new VEFILLRECTANGLE(fr);
    case ve_rect:return new VERECTANGLE(fr);
    case ve_polyline:return new VEPOLYLINE(fr);
    case ve_polygon:return new VEPOLYGON(fr);
    case ve_fpolygon:return new VEFILLPOLYGON(fr);
    case ve_dpolygon:return new VEDRAWPOLYGON(fr);
    case ve_ell:return new VEELLIPSE(fr);
    case ve_fell:return new VEFILLELLIPSE(fr);
    case ve_dell:return new VEDRAWELLIPSE(fr);
  }
  throw EVEC_Loaded_Unknown_Object();
}

void VEPOLY::caption(char *buf) {
  CHARADD ca(buf,0x100);
  ca<<cpt<<"(";
  for(int i=1;i<pts.count;i++) ca<<pts[i].x<<","<<pts[i].y<<"  ";
  ca<<")";
}

bool VEPOLY::size(ABREC &r) {
  if (pts.count==0) return false;
  r.a.x=r.b.x=pts[0].x;r.a.y=r.b.y=pts[0].y;
  for(int i=1;i<pts.count;i++) {
    int x=pts[i].x,y=pts[i].y;
    if (x<r.a.x) r.a.x=x;
    if (x>r.b.x) r.b.x=x;
    if (y<r.a.y) r.a.y=y;
    if (y>r.b.y) r.b.y=y;
  }
  return true;
}

void VEPOLY::savepts(BINWRITE &fw) {
  fw<<pts.count;fw.writedata(pts.buf,sizeof(XYREC)*pts.count);
}
void VEPOLY::loadpts(BINREAD &fr) {
  int cnt;fr>>cnt;pts.wantcapa(cnt);
  fr.readdata(pts.buf,sizeof(XYREC)*cnt);
  pts.count=cnt;
}

void VECOBJ::save(BINWRITE &fw) {
  fw<<elems.count;
  for(int i=0;i<elems.count;i++) elems[i]->save(fw);
}

VECOBJ::VECOBJ(BINREAD &fr) {
  int cnt;fr>>cnt;
  for(int i=0;i<cnt;i++) elems+=readelem(fr);
}

void VECOBJ::draw(const CANVAS &cnv,VECELEM *notdraw) {
  for(int i=0;i<elems.count;i++) if (elems[i]!=notdraw) elems[i]->draw(cnv);
}

void VECCOLLECTION::save(BINWRITE &fw) {
  fw<<objs.count;
  for(int i=0;i<objs.count;i++) objs[i]->save(fw);
}

void VECCOLLECTION::save(const char *file) {
  GZWRITE fw(file);save(fw);
}

void VECCOLLECTION::load(BINREAD &fr) {
  objs.clear();
  int cnt;fr>>cnt;
  for(int i=0;i<cnt;i++) objs+=new VECCOLELEM(fr);
}
void VECCOLLECTION::load(const char *file) {
  GZREAD fr(file);load(fr);
}

void VECCOLELEM::save(BINWRITE &fw) {
  fw<<name;obj->save(fw);
}
VECCOLELEM::VECCOLELEM(BINREAD &fr) {
  fr>>name;obj=new VECOBJ(fr);
}

bool VECOBJ::size(ABREC &r) {
  bool res=false;
  for(int i=0;i<elems.count;i++) {
    ABREC r2;
    bool res2=elems[i]->size(r2);
    if (res2) {
      if (res) {
        r.a.x=min(r.a.x,r2.a.x);
        r.a.y=min(r.a.y,r2.a.y);

        r.b.x=max(r.b.x,r2.b.x);
        r.b.y=max(r.b.y,r2.b.y);
      } else res=true,r=r2;
    }
  }
  return res;
}

VECOBJ *VECCOLLECTION::objbyname(const char *name) {
  for(int i=0;i<objs.count;i++) if (eqstr(name,objs[i]->name)) return objs[i]->obj;
  return NULL;
}


}
#pragma package(smart_init)
