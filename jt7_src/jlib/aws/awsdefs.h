#ifndef awsdefsH
#define awsdefsH

namespace aws {

struct Size {
  int w,h;
  Size(int w,int h):w(w),h(h) {}
  Size():w(0),h(0) {}
};
inline Size operator +(const Size &a,const Size &b) {return Size(a.w+b.w,a.h+b.h);}

struct Point {
  int x,y;
  Point(int x,int y):x(x),y(y) {}
  Point():x(0),y(0) {}
  void operator +=(Point pt) {x+=pt.x;y+=pt.y;}
  void operator -=(Point pt) {x-=pt.x;y-=pt.y;}
  void operator *=(float k) {x*=k;y*=k;}
  Point operator +(Point pt) const {return Point(x+pt.x,y+pt.y);}
  Point operator -(Point pt) const {return Point(x-pt.x,y-pt.y);}
};
struct Rect {
  Point a,b;
  Size size() {return Size(b.x-a.x+1,b.y-a.y+1);}
  int left() {return a.x;}
  int top() {return a.y;}
  Rect(Point a,Point b):a(a),b(b) {}
  Rect(int x,int y,int w,int h):a(x,y),b(x+w-1,y+h-1) {}
  Rect(Point a,Size s):a(a),b(a.x+s.w-1,a.y+s.h-1) {}
  void inflate(int dx,int dy) {a.x-=dx;a.y-=dy;b.x+=dx;b.y+=dy;}
  void operator +=(Point p) {a+=p;b+=p;}
  void operator -=(Point p) {a-=p;b-=p;}
  void operator *=(float k) {a*=k;b*=k;}
  void operator +=(const Rect &r);//sjednoceni
  void operator *=(const Rect &r);//prunik
  bool contain(Point p) {return p.x>=a.x && p.y>=a.y && p.x<=b.x && p.y<=b.y;}
  Rect() {}
};
inline Rect operator -(const Rect &r,const Point &pt) {
  return Rect(Point(r.a.x-pt.x,r.a.y-pt.y),Point(r.b.x-pt.x,r.b.y-pt.y));
}
inline Rect operator +(const Rect &r,const Point &pt) {
  return Rect(Point(r.a.x+pt.x,r.a.y+pt.y),Point(r.b.x+pt.x,r.b.y+pt.y));
}
inline bool operator ==(const Point &p1,const Point &p2) {return p1.x==p2.x && p1.y==p2.y;}
inline bool operator !=(const Point &p1,const Point &p2) {return p1.x!=p2.x || p1.y!=p2.y;}

/*
enum {ks_shift=0x1000,ks_alt=0x2000,ks_ctrl=0x4000};
enum KeyType {kc_none=0,kc_enter,kc_esc,kc_space,kc_f1,kc_f2,kc_f3,kc_f4,kc_f5,kc_f6,kc_f7,kc_f8,kc_f9,kc_f10,kc_f11,kc_f12,
kc_up,kc_left,kc_down,kc_right,kc_home,kc_end,kc_pgup,kc_pgdn,kc_bs,kc_delete,kc_char};

const int kc_key_mask=0xFFF;
*/

enum KeyType {kt_none=0,kt_char,kt_enter,kt_esc,kt_space,kt_f1,kt_f2,kt_f3,kt_f4,kt_f5,kt_f6,kt_f7,kt_f8,kt_f9,kt_f10,kt_f11,kt_f12,
kt_up,kt_left,kt_down,kt_right,kt_home,kt_end,kt_pgup,kt_pgdn,kt_bs,kt_delete,kt_insert,kt_tab};

enum ShiftEnum {ks_shift=0x1,ks_alt=0x2,ks_ctrl=0x4};
                //tyto stavy uz nemaji vliv na rovnost stavu (slouzi jen pro testovani)
enum ShiftTestEnum {kst_lshift=0x1,kst_rshift=0x2,kst_lctrl=0x4,kst_rctrl=0x8,
                    kst_lalt=0x10,kst_ralt=0x20};
struct ShiftState {
  unsigned char state;
  unsigned char teststate;
  bool operator &(ShiftEnum e) const {return state&e;}
  bool empty() const {return state==0;}
  ShiftState() {state=0;teststate=0;}
  ShiftState(ShiftEnum shift1) {state=shift1;teststate=0;}
  ShiftState(ShiftEnum shift1,ShiftEnum shift2) {state=shift1|shift2;teststate=0;}
  ShiftState(ShiftEnum shift1,ShiftEnum shift2,ShiftEnum shift3) {state=shift1|shift2|shift3;teststate=0;}
  ShiftState(ShiftEnum shift1,ShiftEnum shift2,ShiftEnum shift3,ShiftEnum shift4) {state=shift1|shift2|shift3|shift4;teststate=0;}
  bool operator ==(ShiftState right) const {return (state&0x7)==(right.state&0x7);}
  void operator=(unsigned char astate) {state=astate;}
  void operator |=(ShiftEnum se) {state|=se;}
  void clear() {state=0;teststate=0;}
  void exclude(ShiftEnum se) {state&=~se;}
//  operator unsigned char () {return state;}
};

struct awsKey {
  KeyType type;
  ShiftState shift;
  char ch;
  char key;//=toupper(ch)

  awsKey() {clear();}
  awsKey(KeyType atype) {clear();type=atype;}
  void setchar(char c) {type=kt_char;ch=c;key=toupper(c);}
//  operator bool () {return type!=kt_none;}
  void setnone() {clear();}
  void operator =(KeyType atype) {type=atype;shift=0;}
  void operator |=(ShiftEnum se) {shift.state|=se;}
  void settype(KeyType atype) {type=atype;}
  void clear() {shift.clear();type=kt_none;ch=0;key=0;}

  bool valid() const {return type!=kt_none;}
  bool operator ==(awsKey b) const {return type==b.type && shift==b.shift && ch==b.ch && key==b.key;}
  bool eqkey(awsKey b) const {return type==b.type && shift==b.shift && key==b.key;}
  bool operator ==(KeyType atype) const {return type==atype && shift.empty();}
  bool operator ()(KeyType atype,ShiftState ashift) const {return type==atype && shift==ashift;}
  bool operator ==(char c) const {return type==kt_char && c==key;}
  bool operator ==(const char *s) const {awsKey key;key.fromstring(s);return *this==key;}
  bool ischar() {return type==kt_char;}

  string tostring() {char buffer[0x20];tostring(buffer);xcheckstrlen(buffer);return string(buffer);}
  void tostring(char buffer[0x20]);
  void fromstring(STRING s);
};


/*
struct awsKey {
  KeyType type;
  KeyShift shift;
  char ch;
  bool operator ==(KeyType atype) const {return type==atype ;}
};
*/

enum Cursor {cr_default,cr_none,cr_arrow,cr_cross,cr_beam,cr_wait,cr_hsplit,cr_vsplit,cr_hand};


}

#endif
