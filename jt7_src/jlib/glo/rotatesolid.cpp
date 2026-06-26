#include <jlib.h>
#pragma hdrstop

#include "rotatesolid.h"
#include <jmath/geom3d.cpp>

namespace glo {

void CylindricColorize::load(XMLNode xml) {
  for(int i=0;i<xml.count();i++) {
    CylindricColorizeItem *item=new CylindricColorizeItem();
    item->load(xml[i]);
    items.add(item);
  }
}

void CylindricColorizeItem::load(XMLNode xml) {
  texture=xml.svar("texture");
  cond.load(xml.svar("cond"));
  material=xml.ivar("material");
  xscale=xml.fvar("xscale");
  yscale=xml.fvar("yscale");
  transp=xml.ivar("transp");
  reflectx=xml.ivar("reflectx");
  reflecty=xml.ivar("reflecty");
  if (xscale<0.00001) xscale=1;
  if (yscale<0.00001) yscale=1;
}

CylindricColorizeItem *CylindricColorize::find(string faceid) {
  for(int i=0;i<items.count;i++) if (items[i]->cond.accept(faceid)) return items[i];
  return NULL;
}

float dist3d(fpoint3d p1,fpoint3d p2) {
  fvector3d d=p1-p2;
  float s=d.x*d.x+d.y*d.y+d.z*d.z;
  return sqrt(s);
}

fvector3d _countnormal(fpoint3d a,fpoint3d b,fpoint3d c,fpoint3d mid,fpoint3d ospt) {
  fvector3d s1=b-a,s2=b-c,n=s1%s2;
  fvector3d v=ospt-mid;
  if (v*n<0) n=-n;
  return n.size1();
}

fvector3d _countnormal(TCntVec<prVertex> &pts,TCntVec<prPoint> &vrs,fpoint3d ospt) {
  int cnt=vrs.count;
  fpoint3d buf[4];
  if (cnt>0) buf[0]=pts[vrs[0]];
  if (cnt>1) buf[1]=pts[vrs[1]];
  if (cnt>2) buf[2]=pts[vrs[2]];
  if (cnt>3) buf[3]=pts[vrs[3]];
  if (cnt==3) return _countnormal(buf[0],buf[1],buf[2],(fvector3d)(buf[0]+buf[1]+buf[2])/3,ospt);
  if (cnt==4) return _countnormal(buf[0],buf[1],buf[2],(fvector3d)(buf[0]+buf[1]+buf[2]+buf[3])/4,ospt);
  return fpoint3d();
}

void repaircolorize(prFace *face,CylindricColorizeItem *c) {
  if (c->transp) for(int i=0;i<face->vrs.count;i++) exchange(face->vrs[i].t1,face->vrs[i].t2);
  if (c->reflectx) for(int i=0;i<face->vrs.count;i++) face->vrs[i].t1=1-face->vrs[i].t1;
  if (c->reflecty) for(int i=0;i<face->vrs.count;i++) face->vrs[i].t2=1-face->vrs[i].t2;
}

void generaterotatesolid(prPolyedr &pedr,TCntVec<fpoint3d> &pts,int cnt,CylindricColorize &cize,Strings &faceids) {
  if (pts.count<3) return;
  if (cnt<3) return;
  if (pedr.vrs.count || pedr.faces.count) throw JError("prPolyedr::generaterotatesolid():Polyedr must be empty");
  if (faceids.count!=pts.count-1) throw JError("prPolyedr::generaterotatesolid():Bad faceids.count");

  float angle=0,dangle=2*M_PI/(float)cnt;

  fvector3d os=pts.last()-pts[0];

  fpoint3d ospt=(fvector3d)(pts.last()+pts[0])*0.5;

  pedr.vrs.add(pts[0]);
  pedr.vrs.add(pts.last());
  bool culling=true;

  for(int i=0;i<cnt;i++,angle+=dangle) {
    transform3d<float> rot,tran1,tran2,hom;
    rot.m.dorotate(os,angle);
    tran1.b=pts[0];tran2.b=-(fvector3d)pts[0];
    hom=tran1*rot*tran2;
    for(int j=1;j<pts.count-1;j++) {//krome prvniho a posledniho bodu - ty nerotuji
      fpoint3d pt=pts[j];
      pt=hom*pt;
      pedr.vrs.add(pt);
    }
  }

  float sumdist=0;
  for(int i=0;i<pts.count-1;i++) sumdist+=dist3d(pts[i],pts[i+1]);

  int A=pts.count-2;

  for(int i=0;i<cnt;i++) {
    int i1=((i+1)%cnt);
    {//horni
      prFace *face=new prFace();
      face->culling=culling;
      pedr.faces.add(face);
      face->vrs.add(0);
      face->vrs.add(2+i*A);
      face->vrs.add(2+i1*A);
      face->n=_countnormal(pedr.vrs,face->vrs,ospt);
      if (CylindricColorizeItem *c=cize.find(faceids[0])) {
//        float prPoint::*p1=&prPoint::t1;
        face->faceid=faceids[0];
        face->texture=c->texture;
        float tpom=dist3d(pts[0],pts[1])/sumdist;
        face->vrs[0].t1=(float)(i+0.5)/cnt*c->xscale;face->vrs[0].t2=0*c->yscale;
        face->vrs[1].t1=(float)i/cnt*c->xscale;face->vrs[1].t2=tpom*c->yscale;
        face->vrs[2].t1=(float)(i+1)/cnt*c->xscale;face->vrs[2].t2=tpom*c->yscale;
        repaircolorize(face,c);
      }
    }
    {//spodni
      prFace *face=new prFace();
      face->culling=culling;
      pedr.faces.add(face);
      face->vrs.add(1);
      face->vrs.add(2+i*A+ A-1);
      face->vrs.add(2+i1*A+ A-1);
      face->n=_countnormal(pedr.vrs,face->vrs,ospt);
      if (CylindricColorizeItem *c=cize.find(faceids.last())) {
        face->faceid=faceids.last();
        face->texture=c->texture;
        float tpom=1-dist3d(pts[pts.count-2],pts[pts.count-1])/sumdist;
        face->vrs[0].t1=(float)(i+0.5)/cnt*c->xscale;face->vrs[0].t2=1*c->yscale;
        face->vrs[1].t1=(float)i/cnt*c->xscale;face->vrs[1].t2=tpom*c->yscale;
        face->vrs[2].t1=(float)(i+1)/cnt*c->xscale;face->vrs[2].t2=tpom*c->yscale;
        repaircolorize(face,c);
      }
    }

    float pomarg=dist3d(pts[0],pts[1]);

    for(int j=0;j<A-1;j++) {
      prFace *face=new prFace();
      face->culling=culling;
      pedr.faces.add(face);
      face->vrs.add(2+ i*A + j);
      face->vrs.add(2+ i1 * A + j);
      face->vrs.add(2+ i1 * A + j+1);
      face->vrs.add(2+ i*A + j+1);
      face->n=_countnormal(pedr.vrs,face->vrs,ospt);
      if (CylindricColorizeItem *c=cize.find(faceids[j+1])) {
        face->faceid=faceids[j+1];
        face->texture=c->texture;
        float dpom=dist3d(pts[j+1],pts[j+2]);
        float tp1=pomarg/sumdist,tp2=(pomarg+dpom)/sumdist;
        face->vrs[0].t1=(float)i/cnt*c->xscale;face->vrs[0].t2=tp1*c->yscale;
        face->vrs[1].t1=(float)(i+1)/cnt*c->xscale;face->vrs[1].t2=tp1*c->yscale;
        face->vrs[2].t1=(float)(i+1)/cnt*c->xscale;face->vrs[2].t2=tp2*c->yscale;
        face->vrs[3].t1=(float)i/cnt*c->xscale;face->vrs[3].t2=tp2*c->yscale;
        pomarg+=dpom;
        repaircolorize(face,c);
      }
    }

  }

  pedr.normalize_culling();
}


}


