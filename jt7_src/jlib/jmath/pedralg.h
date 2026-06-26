#ifndef pedralgH
#define pedralgH

#include <ext/xmlnode.h>

/*
Popis algoritmu:
Vstup:trojice obecnych (i nekonvexnich) polygonu
Vystup:polyedru zadany povrchovymi polygony
tento polyedr vznikne z krychle uriznutim okraju podle polygonu ze tri stran

datova reprezentace:
behem algormitmu je vysledne teleso reprezentovano jako sjednoceni konvexnich
polyedru

postup:
1. inicializace:naplneni krychli
2. rozrezou se vsechny konvexni polyedry podle kazde roviny urcene rezacim polygonem
3. odstraneji se to polyedry, ktere nejsou unvitr polygonu (diky kroku 2 je poluedr bud
   cely unvitr nebo cely venku)
4. kroky 2,3 se opakuji pro kazdou strany, ze ktere se orezava (X,Y,Z)
5. vysledne steny jsou ty, ktere se ve zbylych polyedrech veskytuji prave jednou

konvexni uprava(neni udelana!!!):
pro casovou slozitost a jednoduchost vysledku se vstupni opbecny polygon rozdeli
ne konexvni polygony, algoritmus se pousti na ne

*/

namespace jmath { namespace pedr {
typedef double real;
struct PedrAlg;
struct DivFace;
struct Polyedr;
struct ComPointProtocol;
typedef int vrid;//index do PedrAlg::vrs
const real epsilon=0.0001;

struct vector {
  real x,y,z;
  vector() {x=y=z=0;}
  vector(real x,real y,real z):x(x),y(y),z(z) {}
  vector size1() {vector n=*this;real size=sqrt(n.x*n.x+n.y*n.y+n.z*n.z);if (size>0) n.x/=size,n.y/=size,n.z/=size;return n;}
};
inline vector operator -(const vector &a,const vector &b) {return vector(a.x-b.x,a.y-b.y,a.z-b.z);}
inline vector operator +(const vector &a,const vector &b) {return vector(a.x+b.x,a.y+b.y,a.z+b.z);}
inline vector operator *(const vector &a,real k) {return vector(a.x*k,a.y*k,a.z*k);}
inline vector operator /(const vector &a,real k) {return vector(a.x/k,a.y/k,a.z/k);}
inline real operator *(const vector &a,const vector &b) {return a.x*b.x+a.y*b.y+a.z*b.z;}

struct point2 {
  real x,y;
  point2() {x=y=0;}
  point2(real x,real y):x(x),y(y) {}
  point2 n() {return point2(y,-x);}
};
inline point2 operator -(const point2 &a,const point2 &b) {return point2(a.x-b.x,a.y-b.y);}
inline point2 operator +(const point2 &a,const point2 &b) {return point2(a.x+b.x,a.y+b.y);}
inline point2 operator *(const point2 &a,real k) {return point2(a.x*k,a.y*k);}
inline point2 operator /(const point2 &a,real k) {return point2(a.x/k,a.y/k);}

inline real operator *(const point2 &a,const point2 &b) {return a.x*b.x+a.y*b.y;}

struct VrRecord {
  vrid id;
  float t1,t2;
  operator int () {return id;}
  VrRecord(int aid) {id=aid;t1=0;t2=0;}
  VrRecord(XMLNode xml) {id=xml.ivar("link");t1=xml.fvar("t1");t2=xml.fvar("t2");}
  VrRecord() {}
  void save_nolink(XMLNode xml) {xml.setvar("t1",t1);xml.setvar("t2",t2);}
};

struct Face {
  TCntVec<VrRecord> vrs;
  string faceid;
  string texture;
  vector n;//normalovy vektor/tento algoritmus nepouziva, jen je potreba pro
  //dalsi navazujici algoritmy, je to presnejsi, nez kdyby se pak pocitalo
  //normala je orientovana dovnitr, orientace je nastavena v Polyedr::normalize()
  void countnormal(PedrAlg &alg);//spocita normalu z prvnich 3 vrcholu
  //prerovna vrs tak, aby se mohlo pustit na CULL_FACING
  void normalize_culling(PedrAlg &alg);

  VrRecord vr(int index) {return vrs[(index+vrs.count*16)%vrs.count];}
  int operator [] (int index) {return vr(index);}
  Face *clone() {Face *res=new Face(faceid);res->vrs.assign(this->vrs);res->n=n;res->texture=texture;return res;}
  Face(string fid) {faceid=fid;}
  Face(string fid,vrid v1,vrid v2,vrid v3,vrid v4=-1) {faceid=fid;vrs.add(v1);vrs.add(v2);vrs.add(v3);if (v4>=0) vrs.add(v4);}

  //spocita pruseciky s rovinou ax+by+cz+d=0, vrati, zda byl neja ky nalezen
  bool prusecik(int faceindex,PedrAlg &alg,TCntVec<DivFace> &fms,real a,real b,real c,real d,TCntVec<Face*> &faces,ComPointProtocol &cpp);
  //spocita,zda je ax+by+cz+d>0 pro nejaky bod steny (predp: stena je cela ne jedne strane od roviny)
  bool gt(PedrAlg &alg,real a,real b,real c,real d);

  //prerovna tak, aby se dalo jednoznacne poznat odlisne hrany (pomoci equalto)
  //zaroven nastavi orientaci u n
  void normalize(PedrAlg &alg,Polyedr &poly);
  bool equalto(Face &face);

  //vrati, zda face protina rovinu a,b,c,d nebo je na druhe strane, nez je ulozeno v *dir
  bool violates(PedrAlg &alg,int *dir,real a,real b,real c,real d);
};

struct Polygon {
  TCntVec<point2> vrs;
  Strings faceids;//faceids[i] : hrana mezi vrs[i] a vrs[i+1] 
  point2 &vr(int index) {return vrs[index%vrs.count];}
  point2 &operator [] (int index) {return vr(index);}
  //vrati, zda hod je uvnitr polygonu
  bool haspoint(point2 p);
};

struct Polygons:IObjArray<Polygon> {
};

struct Polyedr {
  IObjArray<Face> faces;
  void cut(PedrAlg &alg,IObjArray<Polyedr> &pds,real a,real b,real c,real d,ComPointProtocol &cpp,string faceid);
  //rozrizne polyedr podle roviny ax+by+cz+d=0, vysledek prida da pds
  Polyedr *clone() {Polyedr *res=new Polyedr();for(int i=0;i<faces.count;i++) res->faces.add(faces[i]->clone());return res;}
  bool isinpolygon(int dir,PedrAlg &alg,Polygon &poly);
  void normalize(PedrAlg &alg);
  bool hasface(Face &face);//face musi byt normalizovano (tentom polyedr take)
};

struct Polyedrs:IObjArray<Polyedr> {
  void assign(Polyedrs &src) {clear();addpolys(src);}
  void addpolys(Polyedrs &src);
};

struct PedrAlg {
  TCntVec<vector> vrs;//index do tohoto pole:vrid
  Polyedrs polys;
  IObjArray<Face> faces;
  Polygons clipx,clipy,clipz;

  //dir:0=X,1=Y,2=Z
  void cut(int dir,Polygon &poly);
  ///rozrizne vsechny polyedry podle roviny ax+by+cz+d=0, ulozi do newpolys
  void cut(real a,real b,real c,real d,string faceid);

  vector &operator [] (int index) {return vrs[index];}
  //prida vrchol, pokud uz existuje epsilon-podobny, vrati index existijiciho
  vrid addvr(const vector &vr);

  void initalg(real left,real right,real bottom,real top,real deep,real tome);//inicializuje polys a vrs jako krabici
  void initalg();//rozmery si zjisti z clip, mely by byt zadany pruzery ze vsech stran

  //odfiltruje polyedry, ktere nejsou ivnitr polygonu
  //(dir ma stejny vyznam jako u cut)
  void filter(int dir,Polygon &poly);

  void uniquefaces();//naplni strukturu faces temi stenami, ktere se vyskytuji prave jednou

  void normalize();//normuje vsechny polyedry (jednoznacne usporada vecholy sten, nastavi orientaci)

  void normalize_culling();//normuje vsechny polyedry - usporada vrcholy sten

  void run();//predpoklada volani initalg a naplneni cplix,clipy a clipz

  void loadinput(XMLNode xml);
  void saveoutput(XMLNode xml);
//  void saveoutput_process(XMLNode xml);

//  void loadclipx(XMLNode xml);
//  void loadclipy(XMLNode xml);
//  void loadclipz(XMLNode xml);
  //udela operaci pro jednu stranu
  void makeside(int dir,Polygons &cut);

  void addclip(int dir,XMLNode xml);
};


} }

#endif
