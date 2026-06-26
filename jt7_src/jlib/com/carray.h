#ifndef carrayH
#define carrayH

namespace jlib {

const int cycle_flags[]={0x0,0x1,0x3,0x7,0xF,0x1F,0x3F,0x7F,0xFF,
                     0x1FF,0x3FF,0x7FF,0xFFF,0x1FFF,0x3FFF,0x7FFF,0xFFFF};

template<class T,int MOV> struct CycleArray {
  static const int SIZE=1<<MOV;

  T buf[SIZE];
  int beg,end,count;
  void addlast(const T &src) {
    xassert(count<SIZE);
    buf[end++]=src;end&=cycle_flags[MOV];count++;
  }
  void addfirst(const T &src) {
    xassert(count<SIZE);
    buf[beg--]=src;beg&=cycle_flags[MOV];count++;
  }
  void erasefirst() {
    xassert(count>0);
    beg++;beg&=cycle_flags[MOV];count--;
  }
  void eraselast() {
    xassert(count>0);
    end--;end&=cycle_flags[MOV];count--;
  }
  CycleArray() {beg=end=count=0;}

  struct Iter {
    void operator ++(int) {
      pos++;
      pos&=cycle_flags[MOV];
    }
    CycleArray *parent;
    int pos;
    Iter(CycleArray *aparent) {parent=aparent;pos=parent->beg;}
    bool valid() {return pos!=parent->end;}
    T & operator *() {return parent->buf[pos];}
  };

  Iter iter() {return Iter(this);}
  T &operator [](int index) {
    xobjcheckindex(index,count);
    return buf[(index+beg)&cycle_flags[MOV]];
  }

  T &last() {xassert(count>0);return buf[(end-1)&cycle_flags[MOV]];}
  T &first() {xassert(count>0);return buf[beg];}

  void reverse() {
    T pom[SIZE];
    for(int i=0;i<count;i++) pom[i]=operator[](i);
    for(int i=0;i<count;i++) operator[](i)=pom[count-i-1];
  }
  int search(T item) {//vraci index od 0
    for(int i=0;i<count;i++) if (item==operator[](i)) return i;
    return -1;
  }
  template<class S> int searchtype(S item) {//vraci index od 0
    for(int i=0;i<count;i++) if (operator[](i).match(item)) return i;
    return -1;
  }
};
/*Cyklicke pole,
2 zpusoby prochazeni:
CycleArray<T,MOV> ca;
1)
  for(CycleArray<T,MOV>::Iter i=ca.iter();i.valid();i++) do_anything(*i);
2)
  for(int i=0;i<ca.count;i++) do_anything(ca[i]);
*/



}

#endif
