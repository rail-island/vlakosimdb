#ifndef hexstrmH
#define hexstrmH

namespace jlib {

struct HEXWRITE:BINWRITE {
  STROUT *so;
  int bytesperline;//-1:nedeli na radky
  int byteatactline;//kolikaty byte na aktualni radce
  int writedata(const void *buf,int size);
  void writebyte(unsigned char x);
  HEXWRITE (STROUT *aso) {so=aso;bytesperline=16;byteatactline=0;}
};

struct MEMHEXWRITE:HEXWRITE {
  MEMSTROUT mso;
  string str() {return mso.str();}
  MEMHEXWRITE():HEXWRITE(&mso) {}
};

struct HEXREAD:BINREAD {
  HEXREAD(string as) {s=as;s.unique();act=s.c_str();eof=false;}
  int readdata(void *buf,int size);
  void save(BINWRITE &fw);//ulozi vsechna zbyvajici data do fw
private:
  uchar readbyte();
  const char *act;
  string s;
};

};

#endif
