#ifndef jsetH
#define jsetH

namespace jlib {

struct Set32 {
  int flags;
  Set32 &operator <<(int elem) {flags|=(1<<elem);return *this;}
  Set32 &operator >>(int elem) {flags&=(~(1<<elem));return *this;}
  bool operator [](int elem) {return flags&(1<<elem);}
  Set32(int aflags=0) {flags=aflags;}
  void clear() {flags=0;}
  void operator +=(Set32 set) {flags|=set.flags;}
  void operator -=(Set32 set) {flags&=~set.flags;}
};

inline Set32 operator +(Set32 a,Set32 b) {return Set32(a.flags|b.flags);}
inline Set32 operator -(Set32 a,Set32 b) {return Set32(a.flags&(~b.flags));}
inline Set32 operator -(Set32 a) {return Set32(~a.flags);}

}

#endif
