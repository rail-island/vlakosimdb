#ifndef analgeomH
#define analgeomH

namespace jmath {
using namespace jlib;

//typedef int agnum;

template<class agnum> struct agxyrec {
  agnum x,y;
  agxyrec(agnum ax,agnum ay) {x=ax;y=ay;}
  agxyrec reflectx(agnum x0=0) const {return agxyrec<agnum>(x0-x,y);}//zrcadleni podle osy x=x0
  agxyrec reflecty(agnum y0=0) const {return agxyrec<agnum>(x,y0-y);}//zrcadleni podle osy y=y0
  agxyrec reflectxy(agnum x0=0,agnum y0=0) const {return agxyrec<agnum>(x0-x,y0-y);}//zrcadleni podle osy x=x0 a y=y0
  agxyrec transp() const {return agxyrec<agnum>(y,x);}
  operator XYREC () const {return XYREC(x,y);}
  agxyrec<agnum> multiply(agnum kx,agnum ky) const {return agxyrec<agnum>(x*kx,y*ky);}
protected:
  agxyrec() {}
};
template<class agnum> inline agxyrec<agnum> operator +(const agxyrec<agnum> &a,const agxyrec<agnum> &b) {
  return agxyrec<agnum>(a.x+b.x,a.y+b.y);
}
template<class agnum> inline agxyrec<agnum> operator *(const agxyrec<agnum> &a,agnum k) {
  return agxyrec<agnum>(a.x*k,a.y*k);
}

template<class agnum> struct agpoint:agxyrec<agnum> {
  agpoint(agnum ax,agnum ay) {x=ax;y=ay;}
  agpoint(const XYREC &src) {x=src.x;y=src.y;}
  agpoint(const agxyrec<agnum> &src) {x=src.x;y=src.y;}
  agpoint() {x=y=0;}
  agpoint<agnum> translate(agnum dx,agnum dy) const {return agpoint<agnum>(x+dx,y+dy);}
};

typedef agpoint<float> fpoint;

template<class agnum> struct agangle {
protected:
  unsigned char data;
public:
  double pi2() const {return data/(M_PI/128.0);}
  double pi1() const {signed char d=data;return d/(M_PI/128.0);}
  agangle(double src) {data=(double)src/M_PI*128.0;}
};

template<class agnum> struct agvector:agxyrec<agnum> {
  agvector(const agpoint<agnum> &a,const agpoint<agnum> &b) {x=b.x-a.x;y=b.y-a.y;}
  agvector(agnum dx,agnum dy) {x=dx;y=dy;}
  agvector getnormal() const {return agvector<agnum>(-y,x);}
  agvector size1() const {double size=fsize();return agvector<agnum>(x/size,y/size);}
  agvector(const agxyrec<agnum> &src) {x=src.x;y=src.y;}
  agvector() {x=0;y=0;}
  double fsize() const {return sqrt(x*x+y*y);}
  void operator *=(agnum koef) {x*=koef;y*=koef;}
  double fangle() const {if (x==0) return y<0?M_PI_2*3:M_PI_2;return atan(y/x);}
};

typedef agvector<float> fvector;

template<class agnum> agpoint<agnum> inline operator +(const agpoint<agnum> &pt,const agvector<agnum> &v) {return agpoint<agnum>(pt.x+v.x,pt.y+v.y);}
template<class agnum> agpoint<agnum> inline operator -(const agpoint<agnum> &pt,const agvector<agnum> &v) {return agpoint<agnum>(pt.x-v.x,pt.y-v.y);}

template<class agnum> agvector<agnum> inline operator -(const agpoint<agnum> &a,const agpoint<agnum> &b) {return agvector<agnum>(a.x-b.x,a.y-b.y);}
template<class agnum> agvector<agnum> inline operator -(const agpoint<agnum> &v) {return agvector<agnum>(-v.x,-v.y);}

template<class agnum> agnum inline operator *(const agvector<agnum> &a,const agvector<agnum> &b) {return a.x*b.x+a.y*b.y;}

template<class agnum> struct agline {
  agpoint<agnum> a;
  agvector<agnum> d;
  agline(const agpoint<agnum> &a,const agvector<agnum> &d):a(a),d(d) {}
  double pdist(const agpoint<agnum> &pt) const;//vzdalenost od primky
  double pdistfromline(const agpoint<agnum> &pt) const;//vrati vzdalenost od usecky
  void makeimplicitfunc(double &a,double &b,double &c) const;
  //vyrobi obecnou rovnoci primky ve tvaru a*x^2+b*x+c=0
  agline(const agpoint<agnum> &a,const agpoint<agnum> &b):d(b.x-a.x,b.y-a.y),a(a) {}
  agline getrev() const {return agline(getb(),a);};//vrati caru,ktera ma stejne krajni body,ale opacnou orientaci
  agpoint<agnum> getb() const {return agpoint<agnum>(a.x+d.x,a.y+d.y);}//vrati druhy bod nez a
  agline getvert() const {return agline<agnum>(a,d.getnormal());}
  agline getvertb() const {return agline<agnum>(getb(),d.getnormal());}
  ///do arg da parametr l, kterym je parametrizovan spolecny bod
  bool compoint(const agline &l,agpoint<agnum> &pt,agnum *arg=NULL) const;
  //vrati,zda existuje spolecny bod l a teto cary,v pt je tento bod
  double fsize() const {return d.fsize();}
};

template<class agnum> struct agpline {
  fvector n;
  agnum c;
  agpline() {c=0;}
  agpline(const agvector<agnum> &n,const agpoint<agnum> &pt):n(n) {
    c=-n.x*pt.x-n.y*pt.y;
  }
  agnum operator() (const agxyrec<agnum> &pt) {return n.x*pt.x+n.y*pt.y+c;}
  void reverse() {n=-n;c=-c;}  
};

typedef agpline<float> fpline;

template<class agnum> struct agmatrix {
  agnum a11,a12,a21,a22;
  agmatrix() {a11=a22=1;a12=a21=0;}
  void dorotate(double angle) {
    agnum c=cos(angle),s=sin(angle);
    a11=c;a12=s;
    a21=-s;a22=c;
  }
  agmatrix(agnum b11,agnum b12,agnum b21,agnum b22) {a11=b11;a12=b12;a21=b21;a22=b22;}
  agmatrix<agnum> transp() {return agmatrix<agnum>(a11,a21,a12,a22);}
  void dotransp() {exchange(a12,a21);}
};
typedef agmatrix<float> fmatrix;

template<class agnum> inline agxyrec<agnum> operator -(const agxyrec<agnum> &xy) {return agxyrec<agnum>(-xy.x,-xy.y);}

template<class agnum> struct agtransform {
  agmatrix<agnum> a;
  agvector<agnum> d;
  void doinvert() {
    a.dotransp();
    d=-(a*d);
  }
  agxyrec<agnum> operator () (const agxyrec<agnum> &v) const {
    return a*v+d;
  }
  void dotranslate(agnum dx,agnum dy) {d.x+=dx;d.y+=dy;}
  agtransform(const agmatrix<agnum> &A,const agxyrec<agnum> &V) {a=A;d=V;}
  agtransform() {}
};

template<class agnum> inline agxyrec<agnum> operator *(const agmatrix<agnum> &a,const agxyrec<agnum> &v) {
  return agxyrec<agnum>(a.a11*v.x+a.a12*v.y,a.a21*v.x+a.a22*v.y);
}
template<class agnum> inline agxyrec<agnum> operator *(const agtransform<agnum> &a,const agxyrec<agnum> &v) {
  return agxyrec<agnum>(a(v));
}
template<class agnum> inline agmatrix<agnum> operator *(const agmatrix<agnum> &a,const agmatrix<agnum> &b) {
  return agmatrix<agnum>(
    a.a11*b.a11+a.a12*b.a21,
    a.a11*b.a12+a.a12*b.a22,
    a.a21*b.a11+a.a22*b.a21,
    a.a21*b.a12+a.a22*b.a22
  );
}
template<class agnum> inline agtransform<agnum> operator *(const agtransform<agnum> &a,const agtransform<agnum> &b) {
  return agtransform<agnum>(a.a*b.a,a.a*b.d+a.d);
}
template<class agnum> inline double distance(const agxyrec<agnum> &a,const agxyrec<agnum> &b) {agnum dx=a.x-b.x,dy=a.y-b.y;return sqrt(dx*dx+dy*dy);}
template<class T,class S> inline double fdistance(const T &a,const S &b) {double dx=a.x-b.x,dy=a.y-b.y;return sqrt(dx*dx+dy*dy);}
inline double distance(const XYREC &a,const XYREC &b) {int dx=a.x-b.x,dy=a.y-b.y;return sqrt(dx*dx+dy*dy);}

template<class agnum> inline agxyrec<agnum> middle(const agxyrec<agnum> &a,const agxyrec<agnum> &b) {
  return agxyrec<agnum>((a.x+b.x)/2,(a.y+b.y)/2);
}
inline XYREC middle(const XYREC &a,const XYREC &b) {
  return XYREC((a.x+b.x)/2,(a.y+b.y)/2);
}

}

#endif
