#ifndef jbmpfileH
#define jbmpfileH

namespace jlib {

struct bmpRGB {
  unsigned char r,g,b,a;
  bmpRGB() {r=g=b=a=0;}
  bmpRGB(unsigned char r,unsigned char g,unsigned char b,unsigned char a):r(r),g(g),b(b),a(a) {}
};

struct BMPData {
  int w,h;
  bmpRGB *palete;
  int pallen;
  void *data;
  int bits;
  int linelen;//delka radky v bytech

  BMPData() {w=h=0;palete=NULL;pallen=0;data=NULL;linelen=0;}
  ~BMPData() {clear();}

  void load(string file);
  void clear() {if (palete) free(palete);if (data) free(data);w=h=0;palete=NULL;data=NULL;linelen=0;}
  int _getpixel(int x,int y);//vrati primo bity parvy (napr. index do palety),nekontroluje meze
  int getpixel(int x,int y) {if (x>=0 && y>=0 && x<w && y<h) return _getpixel(x,y);return -1;}
  bmpRGB _getcolor(int x,int y);
  bmpRGB getcolor(int x,int y) {if (x>=0 && y>=0 && x<w && y<h) return _getcolor(x,y);return bmpRGB();}
  ///zapise do bufferu ve 32-bitovem formatu
  void fill32(int *colors,bool reversey);
};//nacteny BMP soubor

}

#endif
