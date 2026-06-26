#include <jlib.h>
#pragma hdrstop

#include "crookalg.h"
#include <gram/gmanal.h>

namespace jmath {

const float epsilon=0.0001;

float CrookAlg::readm(fpoint3d pt) {
  switch (mdir) {
    case 0:return pt.x;
    case 1:return pt.y;
    case 2:return pt.z;
  }
  return 0;
}
void CrookAlg::writec(fpoint3d &pt,float c) {
  switch (cdir) {
    case 0:pt.x=c;break;
    case 1:pt.y=c;break;
    case 2:pt.z=c;break;
  }
}
float CrookAlg::readc(fpoint3d pt) {
  switch (cdir) {
    case 0:return pt.x;
    case 1:return pt.y;
    case 2:return pt.z;
  }
  return 0;
}

void CrookAlg::dividefaces() {
  IObjArray<prFace> newfaces;
  for(int facei=0;facei<pedr.faces.count;facei++) {
    prFace &face=*pedr.faces[facei];
    if (face.vrs.count<1) continue;
    int mini=0,maxi=0;//indexy vrcholu s nejmensim a nejvetsim main
    float minv=readm(pedr[face.vrs[0]]),maxv=minv;
    for(int vrj=1;vrj<face.vrs.count;vrj++) {
      float act=readm(face.vrs[vrj]);
      if (act<minv) mini=vrj,minv=act;
      if (act>maxv) maxi=vrj,maxv=act;
    }
    //v mini,maxi indexy s extremni souradnici main
    int pti=0;
    for(;pti<pline.count;pti++) {
      if (pline[pti].y>minv) break;
    }
    if (pti==pline.count) {//polygon nad vsemi carami
      newfaces.add(face.clone());
      continue;
    }
    int left=mini,right=mini;

    fpoint3d lastl=pedr[face.vrs[mini]],lastr=pedr[face.vrs[mini]];

    TCntVec<int> lvrs,rvrs;
    lvrs.add(face.vrs[mini]);

    left--;if (left<0) left=face.vrs.count-1;
    right++;if (right>=face.vrs.count) right=0;

    for(;pti<pline.count;pti++) {
      float acty=pline[pti].y;

      while (left!=maxi && readm(face.vrs[left])<acty+epsilon) {
        lvrs.add(face.vrs[left]);
        lastl=pedr[face.vrs[left]];
        left--;if (left<0) left=face.vrs.count-1;
//        if (readm(face.vrs[left])<acty+epsilon) lvrs.add(left);
//        if (left==maxi) break;//zarazka
      }
      while (right!=maxi && readm(face.vrs[right])<acty+epsilon) {
        rvrs.add(face.vrs[right]);
        lastr=pedr[face.vrs[right]];
        right++;if (right>=face.vrs.count) right=0;
//        if (right==maxi) break;//zarazka
//        if (readm(face.vrs[right])<acty+epsilon) rvrs.add(right);
      }
      if (left==maxi && right==maxi) {//konec teto steny:jsme na nejvyssim vrcholu
/*        if (right!=maxi) {
          fpoint3d ppp=pedr[face.vrs[right]];
          xassert(right==maxi);//vyplyva z konvexity
        }*/
        prFace *f=new prFace();f->n=face.n;
        f->culling=face.culling;
        f->add(lvrs);f->vrs.add(face.vrs[maxi]);
        rvrs.reverse();f->add(rvrs);
        newfaces.add(f);
        goto nextface;
      }
      //rozdeleni steny
      fpoint3d leftp,rightp;//souradnice novych bodu
      float dm_left=readm(face.vrs[left])-readm(lastl);
      float tl=(acty-readm(lastl))/dm_left;
      fpoint3d leftv=pedr.vrs[face.vrs[left]];
      leftp.x=(leftv.x-lastl.x)*tl+lastl.x;
      leftp.y=(leftv.y-lastl.y)*tl+lastl.y;
      leftp.z=(leftv.z-lastl.z)*tl+lastl.z;

      float dm_right=readm(face.vrs[right])-readm(lastr);
      float tr=(acty-readm(lastr))/dm_right;
      fpoint3d rightv=pedr.vrs[face.vrs[right]];
      rightp.x=(rightv.x-lastr.x)*tr+lastr.x;
      rightp.y=(rightv.y-lastr.y)*tr+lastr.y;
      rightp.z=(rightv.z-lastr.z)*tr+lastr.z;

      int newl=pedr.addv(leftp,epsilon),newr=pedr.addv(rightp,epsilon);

      if (lvrs.search(newl)<0) lvrs.add(newl);
      if (rvrs.search(newr)<0) rvrs.add(newr);

      prFace *f=new prFace();f->n=face.n;
      f->culling=face.culling;
      f->add(lvrs);
      rvrs.reverse();f->add(rvrs);
      newfaces.add(f);

      lvrs.clear();rvrs.clear();
      lvrs.add(newl);rvrs.add(newr);

    }
    while (left!=maxi) {
      lvrs.add(face.vrs[left]);
      left--;if (left<0) left=face.vrs.count-1;
    }
    while (right!=maxi) {
      rvrs.add(face.vrs[right]);
      right++;if (right>=face.vrs.count) right=0;
    }
    lvrs.add(face.vrs[maxi]);
    prFace *f=new prFace();f->n=face.n;
    f->culling=face.culling;
    f->add(lvrs);
    rvrs.reverse();f->add(rvrs);
    newfaces.add(f);

    nextface:
  }
  exchangemem(pedr.faces,newfaces);
  for(int i=0;i<pedr.faces.count;i++) pedr.faces[i]->normalize_culling(&pedr);
}

void CrookAlg::crookfaces() {
  bool *cvrs=new bool[pedr.vrs.count];
  //kterych vrcholu se tyka nasikmeni
  fillarray(cvrs,pedr.vrs.count,false);
  for(int facei=0;facei<pedr.faces.count;facei++) {
    prFace &face=*pedr.faces[facei];
//    face.culling=false;
    if (sgn) {//posunovat jen ty spravne
      if (readc(face.n)>-epsilon) continue;
    } else {
      if (readc(face.n)<epsilon) continue;
    }
    for(int vrj=0;vrj<face.vrs.count;vrj++) {
      cvrs[face.vrs[vrj]]=true;
    }
  }
  //nakonec posunout vsechny vrcholy z cvrs
  for(int vri=0;vri<pedr.vrs.count;vri++) if (cvrs[vri]) {
    float m=readm(vri);
    for(int pti=1;pti<pline.count;pti++) {
      fpoint p1=pline[pti-1],p2=pline[pti];
      if (m>=p1.y && m<=p2.y) {
        float t=(m-p1.y)/(p2.y-p1.y);
        writec(vri,readc(vri)+(1-2*sgn)*(t*(p2.x-p1.x)+p1.x));
        break;
      }
    }
    //ted najdeme index
  }
  delete []cvrs;
}

bool CrookAlg::checkinput() {
  if (mdir==cdir) return false;
  if (mdir<0 || mdir>2) return false;
  if (cdir<0 || cdir>2) return false;
  if (sgn!=0 && sgn!=1) return false;
  for(int i=1;i<pline.count;i++) if (pline[i].y<=pline[i-1].y) return false;
  return true;
}

void CrookAlg::run() {
  if (!checkinput()) throw JError("CrookAlg:bad input");
  dividefaces();
  crookfaces();
}

void CrookAlg::loadopts(string s) {
  gram::Analyser a(s.c_str());
  mdir=a.readint();
  a.jsp();a.f_char(',');a.jsp();
  cdir=a.readint();
  a.jsp();a.f_char(',');a.jsp();
  a.jsp();sgn=a.readint();a.jsp();
  a.f_char('|');
  pline.clear();
  while (*a) {
    fpoint &pt=pline.add();
    a.jsp();
    pt.x=a.readdouble();
    a.jsp();a.f_char(',');a.jsp();
    pt.y=a.readdouble();a.jsp();
    if (*a==';') a++;
    a.jsp();
  }
}

void CrookAlg::loadpolyline(XMLNode xml) {
  pline.clear();
  for(int i=0;i<xml.count();i++) {
    fpoint pt=pline.add();
    pt.x=xml.fvar("x");
    pt.y=xml.fvar("y");
  }
}

}

