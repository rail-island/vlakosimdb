#include <jlib.h>
#pragma hdrstop

#include "pedralg.h"

namespace jmath { namespace pedr {

static real dist2(const point2 &a,const point2 &b) {
  real dx=a.x-b.x,dy=a.y-b.y;
  return sqrt(dx*dx+dy*dy);
}

static real dist(const vector &a,const vector &b) {
  real dx=a.x-b.x,dy=a.y-b.y,dz=a.z-b.z;
  return sqrt(dx*dx+dy*dy+dz*dz);
}


//rozdelena stena
struct DivFace {
  vrid v1,v2;//vrcholy, ktere byly pridany, je to prusecik
  int face;//index steny
};


//aby se nemuselo pocitat znova prusecik (a aby mel stejne vrid)
struct ComPoint {
  vrid v1,v2;
  VrRecord p;//prusecik s rovinou
};

struct Double {
  vrid v1,v2;
  vrid other(vrid v) {if (v==v1) return v2;if (v==v2) return v1;return -1;}
};

struct Doubles:TCntVec<Double> {
  int getd(vrid v1,vrid v2) {//vrati index nebo -1
    for(int i=0;i<count;i++)
      if ( (buf[i].v1==v1 && buf[i].v2==v2) || (buf[i].v1==v2 && buf[i].v2==v1) )
        return i;
    return -1;
  }
  void add(vrid v1,vrid v2) {
    if (getd(v1,v2)<0) {
      Double &d=TCntVec<Double>::add();
      d.v1=v1;d.v2=v2;
    }
  }
  vrid next(vrid v) {
    for(int i=0;i<count;i++) {
      if (buf[i].v1==v) return buf[i].v2;
      if (buf[i].v2==v) return buf[i].v1;
    }
    return -1;
  }
};

struct ComPointProtocol:TCntVec<ComPoint> {
  Doubles dbls;//dvojice vrcholu lezici na jedne hrane a na delici rovine

  VrRecord getb(vrid v1,vrid v2) {//vrati prusecik nebo -1
    for(int i=0;i<count;i++)
      if ( (buf[i].v1==v1 && buf[i].v2==v2) || (buf[i].v1==v2 && buf[i].v2==v1) )
        return buf[i].p;
    return -1;
  }
  void add(vrid v1,vrid v2,VrRecord p) {
    ComPoint &cp=TCntVec<ComPoint>::add();
    cp.v1=v1;cp.v2=v2;cp.p=p;
  }
};

//prusecik roviny a hrany
struct LinePoint {
  int vridx;//index vrcholu do faces, za kterym to je
  VrRecord p;//prusecik
};

bool Face::prusecik(int faceindex,PedrAlg &alg,TCntVec<DivFace> &fms,real a,real b,real c,real d,TCntVec<Face*> &faces,ComPointProtocol &cpp) {
  //pokud najde prusecik, prida do DivFace a rozdeli stenu na 2 dily
  //tyto prida do faces (pokud neni rozdelena, prida tam svoji kopii)
  //zaronev prida do cpp.dbls dvojice kvuli hledani spolecne steny
  int bp1=-1,bp2=-1;//prusecik s rovinou je vrchol
  for(int i=0;i<vrs.count;i++) {
    vector v=alg[vr(i)];
    if (fabs((a*v.x+b*v.y+c*v.z+d))/sqrt(a*a+b*b+c*c)<epsilon) {
      if (bp1<0) bp1=i;else bp2=i;
    }
  }
  //ted se najdou pruseciky roviny s hranami

  LinePoint prs[2],*prw=prs;//pruseciky s

  for(int i=0;i<vrs.count;i++) {
    vrid v1=vr(i),v2=vr(i+1);
    VrRecord vr1=vr(i),vr2=vr(i+1);
    if (i==bp1 || i+1==bp1) continue;
    if (i==bp2 || i+1==bp2) continue;
    //tato hrana sa preskakuje, protoze je vedle vercholu lezicich na rovine

    VrRecord p=cpp.getb(v1,v2);
    if (p>=0) {
      prw->p=p;prw->vridx=i;prw++;
      continue;
    }

    //pokud nebyl prusecik pocitan uz drive
    vector A=alg[v1],B=alg[v2];
    vector D=B-A;
    //(A,B):zkoumana hrana
    real jm=a*D.x+b*D.y+c*D.z;
    if (fabs(jm)<epsilon) continue;//spolecny bod neni, rovina je rovnobezna s primkou
    real cit=a*A.x+b*A.y+c*A.z+d;
    real t=-cit/jm;
    if (t<epsilon || t>1-epsilon) continue;//primka se s rovinou protina, ale neni to v ramci usecky
    vector newpt=A+D*t;
    p=alg.addvr(newpt);
    p.t1=vr1.t1+(vr2.t1-vr1.t1)*t;
    p.t2=vr1.t2+(vr2.t2-vr1.t2)*t;
    prw->p=p;prw->vridx=i;
    prw++;
    cpp.add(v1,v2,p);
  }

  xassert(prw-prs<=2);
  //ted zbyva analyzovat vysladna data, pripadne provest rozdeleni steny

  if (bp2>=0) {//stena je rezana ve dvou vrcholech
    if (abs(bp1-bp2)==1 || abs(bp1-bp2)==vrs.count-1) {//stena se nereze, jsou to sousedni body
      cpp.dbls.add(vr(bp1),vr(bp2));
      faces.add(clone());
      return false;
    }
    Face *f1=new Face(faceid),*f2=new Face(faceid);
    f1->texture=texture,f2->texture=texture;
    f1->n=n;f2->n=n;
    for(int i=bp1;i<=bp2;i++) f1->vrs.add(vr(i));
    for(int i=bp2;i<=bp1+vrs.count;i++) f2->vrs.add(vr(i));
    faces.add(f1);faces.add(f2);

    DivFace &fm=fms.add();
    fm.v1=vr(bp1);//pruseciky nastavime jako vrcholy steny
    fm.v2=vr(bp2);
    fm.face=faceindex;

    cpp.dbls.add(fm.v1,fm.v2);

    return true;
  } else if (bp1>=0) {//stena je rezana v jednom vrcholu
    if (prw==prs) {faces.add(clone());return false;}//bod byl jen nahodou blizko, neni zadny prusecik s hranou
    xassert(prw-prs==1);
    Face *f1=new Face(faceid),*f2=new Face(faceid);
    f1->texture=texture,f2->texture=texture;
    f1->n=n;f2->n=n;
    //*
    //  bp1  .  .  .  .
    //    . *       .
    //      .   *.p
    //   vridx.     *
    // ..... stena , ****** protinaci rovina

    int cnt1=0,cnt2=0;
    if (prs->vridx<bp1) cnt2=vrs.count;else cnt1=vrs.count;

    f1->vrs.add(prs->p);
    for(int i=prs->vridx+1;i<=bp1+cnt1;i++) f1->vrs.add(vr(i));

    for(int i=bp1;i<=prs->vridx+cnt2;i++) f2->vrs.add(vr(i));
    f2->vrs.add(prs->p);

    faces.add(f1);faces.add(f2);

    DivFace &fm=fms.add();
    fm.v1=vr(bp1);//pruseciky nastavime jako vrchol steny aprusecik s hranou
    fm.v2=prs->p;
    fm.face=faceindex;

    cpp.dbls.add(fm.v1,fm.v2);//kvuli hledani spolecne steny

    return true;
  } else if (prw==prs+2) {//dva pruseciky mimo vrcholy
    //        * prs[1]->p
    //   .......*..........prs[1]->vridx
    //    .       *         .
    //     .        *         .
    //      .         *    .
    //       .         .*prs[0]->p
    //        .    .      *
    //         .prs[0]->vridx
    Face *f1=new Face(faceid),*f2=new Face(faceid);
    f1->texture=texture,f2->texture=texture;
    f1->n=n;f2->n=n;
    int cnt1=0,cnt2=0;
    if (prs[0].vridx<prs[1].vridx) cnt2=vrs.count;else cnt1=vrs.count;

    f1->vrs.add(prs[0].p);
    for(int i=prs[0].vridx+1;i<=prs[1].vridx+cnt1;i++) f1->vrs.add(vr(i));
    f1->vrs.add(prs[1].p);

    f2->vrs.add(prs[1].p);
    for(int i=prs[1].vridx+1;i<=prs[0].vridx+cnt2;i++) f2->vrs.add(vr(i));
    f2->vrs.add(prs[0].p);

    faces.add(f1);faces.add(f2);

    DivFace &fm=fms.add();
    fm.v1=prs[0].p;//pruseciky nastavime jako pruseciky s hranami
    fm.v2=prs[1].p;
    fm.face=faceindex;

    cpp.dbls.add(fm.v1,fm.v2);//kvuli hledani spolecne steny

    return true;
  } else faces.add(clone());//bez pruseciku
  return false;
}

bool Face::gt(PedrAlg &alg,real a,real b,real c,real d) {
  for(int i=0;i<vrs.count;i++) {
    vector p=alg[vrs[i]];
    real ask=a*p.x+b*p.y+c*p.z+d;
    if (ask>epsilon) return true;
  }
  return false;
}

//rekurzivni backtracking-hledani oddelujici steny (zadana dvojice vrcholu, ukol najit kruznici)
//muze tam byt neco jako
//    c
//    .  .
//    .   .  b . . . . d
//    a
//kruznice je a,b,c, nejde linearnim vyhledavanim, protoze by mohlo skoncit v d
//cfv=common face vrs

static bool findnextvdbl(vrid startv,vrid v,TCntVec<VrRecord> &cfv,ComPointProtocol &cpp) {
  if (v==startv && cfv.count>=3) return true;
  if (cfv.search(v)>=0) return false;
  cfv.add(v);
  for(int i=0;i<cpp.dbls.count;i++) {
    vrid nextv=cpp.dbls[i].other(v);
    if (nextv>=0) if (findnextvdbl(startv,nextv,cfv,cpp)) return true;
  }
  cfv.eraselast();
  return false;
}

/*
static bool findnextv(vrid startv,int act,vrid v,TCntVec<VrRecord> &cfv,TCntVec<DivFace> &fms,ComPointProtocol &cpp) {
  if (cfv.search(v)>=0) return false;
  cfv.add(v);
  v=fms[act].other(v);
  if (v==startv) return true;
  for(int i=0;i<fms.count;i++) if (fms[i].face!=fms[act].face && fms[i].has(v)) {
    if (findnextv(startv,i,v,cfv,fms,cpp)) return true;
  }
  //nenasli jsme prusecnici, jeste muze vest primo po hrane
  vrid nextv=cpp.dbls.next(v);//hrany, ktere jsou identicke s rovinou
  for(int i=0;i<fms.count;i++) if (fms[i].has(nextv)) {
    cfv.add(v);
    if (findnextv(startv,i,nextv,cfv,fms,cpp)) return true;
    cfv.eraselast();
  }
  cfv.eraselast();
  return false;
}
*/

bool Face::violates(PedrAlg &alg,int *dir,real a,real b,real c,real d) {
  for(int i=0;i<vrs.count;i++) {
    vector v=alg[vrs[i]];
    float x=a*v.x+b*v.y+c*v.z+d;
    if (*dir) {
      if (*dir*x<-epsilon) return true;
    } else {
      if (x<-epsilon) *dir=-1;
      if (x>epsilon) *dir=1;
    }
  }
  return false;
}

void Polyedr::cut(PedrAlg &alg,IObjArray<Polyedr> &pds,real a,real b,real c,real d,ComPointProtocol &cpp,string faceid) {
  if (faces.count==0) {
    pds.add(clone());
    return;
  }

  int sidedir=0;
  for(int i=0;i<faces.count;i++) {
    if (faces[i]->violates(alg,&sidedir,a,b,c,d)) goto must_cut;
  }

  //polyedr je na jedne strane od roviny => nereze se
  pds.add(clone());
  return;

  must_cut:
  //zde vime, ze rovina protina polyedr => ma smysl dalsi vypocet

  TCntVec<DivFace> fms;
  TCntVec<Face*> newfaces;

  for(int facei=0;facei<faces.count;facei++) faces[facei]->prusecik(facei,alg,fms,a,b,c,d,newfaces,cpp);

  Face *comface=new Face(faceid);//spolecna stena dvou casti rozdeleneho polyedru
  //jeste musime vyrobit comface-ta se sklada ze vsech pruseciku
  comface->n=vector(a,b,c).size1();

  bool res=false;

  for(int i=0;i<cpp.dbls.count;i++) {
    if (findnextvdbl(cpp.dbls[i].v1,cpp.dbls[i].v1,comface->vrs,cpp)) {res=true;break;}
    if (findnextvdbl(cpp.dbls[i].v2,cpp.dbls[i].v2,comface->vrs,cpp)) {res=true;break;}
  }

  if (!res) {
    //Chyba??
    delete comface;
    pds.add(clone());
    return;
  }

  //ted rozdelime newfaces na dva polyedry, do ktereho patri se pozna dosazeni bodu
  //do rovnice roviny (<0 nebo >0)
  Polyedr *res1=new Polyedr(),*res2=new Polyedr();
  for(int i=0;i<newfaces.count;i++) {
    if (newfaces[i]->gt(alg,a,b,c,d)) res1->faces.add(newfaces[i]);else res2->faces.add(newfaces[i]);
  }

  //nakonec jeste polyedry oba dostanou kopii comface
  res1->faces.add(comface->clone());
  res2->faces.add(comface);
  pds.add(res1);
  pds.add(res2);
}

void PedrAlg::cut(real a,real b,real c,real d,string faceid) {
  Polyedrs newpds;
  ComPointProtocol cpp;
  for(int i=0;i<polys.count;i++) {
    cpp.dbls.clear();
    polys[i]->cut(*this,newpds,a,b,c,d,cpp,faceid);
  }
  exchangemem(newpds,polys);
}

bool Polygon::haspoint(point2 p) {
  if (vrs.count<3) return false;
  for(int i=0;i<vrs.count;i++) {
    point2 a=vr(i),b=vr(i+1);
    if (dist2(a,p)+dist2(p,b)<dist2(a,b)+2*epsilon) return true;
  }
  int invert_cnt=0;
  for(int i=0;i<vrs.count;i++) {
    point2 a=vr(i),b=vr(i+1);
    //bod blizko hrany
    if (a.y==b.y) continue;//test ROVNOSTI dvou realnych cisel,spravne
    if (a.y>b.y) exchange(a,b);
    if (p.y<a.y || p.y>=b.y) continue;//y jen mezi <a.y,b.y) (opet BEZ epsilonu)
    if (fabs(a.x-b.x)<epsilon) {
      if (p.x>=(a.x+b.x)/2) invert_cnt++;
      continue;
    }
    real k=(b.y-a.y)/(b.x-a.x);
    if (b.x>a.x) {
      if (k*(p.x-a.x)+a.y>p.y) invert_cnt++;
    } else {
      if (k*(p.x-a.x)+a.y<p.y) invert_cnt++;
    }
  }
  return invert_cnt&0x1;//test parity
}

bool Polyedr::isinpolygon(int dir,PedrAlg &alg,Polygon &poly) {
  //zjistuje pro kazdy bod algoritmem "inverzniho" vyplnovani
  //kazdy bod musi byt unvitr mnohouhelniku
  bool *tested=new bool[alg.vrs.count];
  //abychom 2x netestovali stejny bod
  for(int i=0;i<alg.vrs.count;i++) tested[i]=false;

  for(int facei=0;facei<faces.count;facei++) {
    Face &face=*faces[facei];
    for(int vri=0;vri<face.vrs.count;vri++) {
      int vr=face.vrs[vri];
      if (tested[vr]) continue;
      tested[vr]=true;
      vector v=alg[vr];
      point2 p;
      switch (dir) {
        case 0:p.x=v.z;p.y=v.y;break;
        case 1:p.x=v.x;p.y=v.z;break;
        case 2:p.x=v.x;p.y=v.y;break;
      }
      if (!poly.haspoint(p)) {
        delete []tested;
        return false;
      }
    }
  }
  delete []tested;
  return true;
}


void Face::normalize_culling(PedrAlg &alg) {
  vector normala;
  real ncsize=-1;
  for(int i=0;i<vrs.count;i++) {
    vector x=alg[vr(i-1)],y=alg[vr(i)],z=alg[vr(i+1)];
    vector a=x-y,b=z-y;
    real actsize=fabs(a*b);
    if (ncsize<0 || actsize<ncsize) {
      normala.x=a.y*b.z-a.z*b.y;
      normala.y=a.z*b.x-a.x*b.z;
      normala.z=a.x*b.y-a.y*b.x;
      ncsize=actsize;
    }
  }
  real mul=normala*n;
  if (mul<0)
    vrs.reverse();
}


void Face::normalize(PedrAlg &alg,Polyedr &poly) {
  //prerovnani vrcholu
  int minidx=0,minvalue=vrs[0];
  for(int i=1;i<vrs.count;i++) {
    if (vrs[i]<minvalue) minidx=i,minvalue=vrs[i];
  }
  int d=1;
  if (vr(minidx-1)<vr(minidx+1)) d=-1;
  VrRecord *buf=new VrRecord[vrs.count];
  for(int i=0;i<vrs.count;i++) buf[i]=vr(minidx+i*d);
  memcpy(vrs.buf,buf,sizeof(VrRecord)*vrs.count);
  delete []buf;

  vector v0=alg[vrs[0]];

  //nastaveni orientace
  real sum=0;
  for(int facei=0;facei<poly.faces.count;facei++) {
    Face &face=*poly.faces[facei];
    for(int vri=0;vri<face.vrs.count;vri++) {
      vector v=alg[face.vrs[vri]];
      sum+=(v.x-v0.x)*n.x + (v.y-v0.y)*n.y + (v.z-v0.z)*n.z;
    }
  }
  if (sum<0) n.x=-n.x,n.y=-n.y,n.z=-n.z;
}

bool Face::equalto(Face &face) {
  if (face.vrs.count!=vrs.count) return false;
  for(int i=0;i<face.vrs.count;i++) if (face.vrs[i].id!=vrs[i].id) return false;
  return true;
//  return face.vrs.count==vrs.count && memcmp(face.vrs.buf,vrs.buf,vrs.count*sizeof(VrRecord))==0;
}

void Polyedr::normalize(PedrAlg &alg) {
  for(int i=0;i<faces.count;i++) faces[i]->normalize(alg,*this);
}
void PedrAlg::normalize() {
  for(int i=0;i<polys.count;i++) polys[i]->normalize(*this);
}

bool Polyedr::hasface(Face &face) {
  for(int i=0;i<faces.count;i++) if (faces[i]->equalto(face)) return true;
  return false;
}

void PedrAlg::uniquefaces() {
  for(int i=0;i<polys.count;i++) {
    Polyedr &poly=*polys[i];
    for(int j=0;j<poly.faces.count;j++) {
      Face &face=*poly.faces[j];
      for(int k=i+1;k<polys.count;k++) if (polys[k]->hasface(face)) goto nextface;
      faces.add(face.clone());
      nextface:
    }
  }
}

void PedrAlg::filter(int dir,Polygon &poly) {
  for(int i=0;i<polys.count;i++) if (!polys[i]->isinpolygon(dir,*this,poly)) {
    delete polys[i];
    polys[i]=NULL;
  }
  polys.packnull();
}

void Polyedrs::addpolys(Polyedrs &src) {
  for(int i=0;i<src.count;i++) add(src[i]->clone());
}

void PedrAlg::makeside(int dir,Polygons &cutp) {
  if (cutp.count>0) {
    Polyedrs polys0;
    polys0.assign(polys);
    Polyedrs respolys;
    for(int i=0;i<cutp.count;i++) {
      polys.assign(polys0);
      cut(dir,*cutp[i]);
      filter(dir,*cutp[i]);
      respolys.addpolys(polys);
    }
    polys.assign(respolys);
  }
}

void PedrAlg::run() {
  makeside(0,clipx);
  makeside(1,clipy);
  makeside(2,clipz);
  normalize();
  uniquefaces();
  normalize_culling();
}


void PedrAlg::normalize_culling() {
  for(int i=0;i<faces.count;i++)
    faces[i]->normalize_culling(*this);
}


void PedrAlg::cut(int dir,Polygon &poly) {
  for(int i=0;i<poly.vrs.count;i++) {
    point2 A=poly[i],B=poly[i+1];
    real a=B.y-A.y,b=A.x-B.x;
    //a,b:koeficienty do obecne rovnice primky v rovine
    real c=-(a*A.x+b*A.y);
    //potrebujeme obecnou rovnici roviny v prostoru, a to tak,
    //ze jedna slozka bude nulova (dle dir)
    switch (dir) {
      case 0:cut(0,b,a,c,poly.faceids[i]);break;
      case 1:cut(a,0,b,c,poly.faceids[i]);break;
      case 2:cut(a,b,0,c,poly.faceids[i]);break;
    }
  }
}
vrid PedrAlg::addvr(const vector &vr) {
  for(int i=0;i<vrs.count;i++)
    if (dist(vr,vrs[i])<epsilon) return i;
  vrs.add(vr);return vrs.count-1;
}

void Face::countnormal(PedrAlg &alg) {
  vector a=alg[vrs[0]]-alg[vrs[1]],b=alg[vrs[0]]-alg[vrs[2]];
  n.x=a.y*b.z-a.z*b.y;
  n.y=a.z*b.x-a.x*b.z;
  n.z=a.x*b.y-a.y*b.x;
  n=n.size1();
}

void PedrAlg::loadinput(XMLNode xml) {
  XMLNode pts=xml["verteces"],fcs=xml["faces"];
  for(int i=0;i<pts.count();i++) vrs.add(vector(pts[i].fvar("x"),pts[i].fvar("y"),pts[i].fvar("z")));

  Polyedr *poly=new Polyedr();
  for(int i=0;i<fcs.count();i++) {
    XMLNode fx=fcs[i];
    Face *f=new Face(fx.svar("faceid"));
    f->texture=fx.svar("texture");
    XMLNode vx=fx["verteces"];
    for(int i=0;i<vx.count();i++) f->vrs.add(VrRecord(vx[i]));
    poly->faces.add(f);
  }

  for(int i=0;i<poly->faces.count;i++) poly->faces[i]->countnormal(*this);
  polys.add(poly);
}

void PedrAlg::initalg(real left,real right,real bottom,real top,real deep,real tome) {
  vrs.clear();polys.clear();
//vepredu:
//4  3
//1  2
//vzadu
//8  7
//5  6
  int v1=addvr(vector(left,bottom,tome));
  int v2=addvr(vector(right,bottom,tome));
  int v3=addvr(vector(right,top,tome));
  int v4=addvr(vector(left,top,tome));

  int v5=addvr(vector(left,bottom,deep));
  int v6=addvr(vector(right,bottom,deep));
  int v7=addvr(vector(right,top,deep));
  int v8=addvr(vector(left,top,deep));

  Polyedr *poly=new Polyedr();
  poly->faces.add(new Face("",v1,v2,v3,v4));//predni
  poly->faces.add(new Face("",v5,v6,v7,v8));//zadni
  poly->faces.add(new Face("",v1,v2,v6,v5));//spodni
  poly->faces.add(new Face("",v4,v3,v7,v8));//vrchni
  poly->faces.add(new Face("",v1,v4,v8,v5));//leva
  poly->faces.add(new Face("",v2,v3,v7,v6));//prava
  polys.add(poly);
  for(int i=0;i<poly->faces.count;i++) poly->faces[i]->countnormal(*this);
}

static void loadpolygon(Polygon &poly,XMLNode xml) {
  for(int i=0;i<xml.count();i++) {
    point2 &pt=poly.vrs.add();
    pt.x=xml[i].fvar("x");
    pt.y=xml[i].fvar("y");
    poly.faceids.add(xml[i].svar("faceid"));
  }
}

static void greatedge(real &edge,Polygon &poly) {
  for(int i=0;i<poly.vrs.count;i++) {
    if (fabs(poly.vrs[i].x)>edge) edge=fabs(poly.vrs[i].x);
    if (fabs(poly.vrs[i].y)>edge) edge=fabs(poly.vrs[i].y);
  }
}

static void greatedge(real &edge,Polygons &polys) {
  for(int i=0;i<polys.count;i++) greatedge(edge,*polys[i]);
}

void PedrAlg::initalg() {
  real edge=0;
  greatedge(edge,clipx);
  greatedge(edge,clipy);
  greatedge(edge,clipz);
  edge+=1;
  initalg(-edge,edge,-edge,edge,-edge,edge);
}

void PedrAlg::addclip(int dir,XMLNode xml) {
  Polygon *p=new Polygon();
  loadpolygon(*p,xml);
  switch (dir) {
    case 0:clipx.add(p);break;
    case 1:clipy.add(p);break;
    case 2:clipz.add(p);break;
  }
}

void PedrAlg::saveoutput(XMLNode xml) {
  bool *used=new bool[vrs.count];
  int *useindex=new int[vrs.count];
  fillarray(used,vrs.count,false);
  fillarray(useindex,vrs.count,0);
  for(int i=0;i<faces.count;i++) {
    Face &face=*faces[i];
    for(int j=0;j<face.vrs.count;j++) used[face.vrs[j]]=true;
  }
  int lastindex=0;
  for(int i=0;i<vrs.count;i++) if (used[i]) useindex[i]=lastindex++;

  XMLNode pts=xml.add("verteces");
  for(int i=0;i<vrs.count;i++) if (used[i]) {
    XMLNode pt=pts.add("vertex");
    pt.setvar("x",vrs[i].x);
    pt.setvar("y",vrs[i].y);
    pt.setvar("z",vrs[i].z);
  }
  XMLNode fcs=xml.add("faces");
  for(int i=0;i<faces.count;i++) {
    XMLNode fx=fcs.add("face");
    Face &face=*faces[i];
//    bstring ids;
//    for(int j=0;j<face.vrs.count;j++) ids<<itos(useindex[face.vrs[j]])<<";";
//    fx.setvar("verteces",ids);
    XMLNode vx=fx.forcesub("verteces");
    for(int j=0;j<face.vrs.count;j++) {
      XMLNode x=vx.add("vertex");
      face.vrs[j].save_nolink(x);
      x.setvar("link",useindex[face.vrs[j]]);
    }
    fx.setvar("nx",face.n.x);
    fx.setvar("ny",face.n.y);
    fx.setvar("nz",face.n.z);
    if (face.faceid!="") fx.setvar("faceid",face.faceid);
    fx.setvar("culling",1);
    if (face.texture!="") fx.setvar("texture",face.texture);
  }
  delete []used;
  delete []useindex;
}


} }

