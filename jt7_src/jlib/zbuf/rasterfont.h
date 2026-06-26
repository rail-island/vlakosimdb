#ifndef rasterfontH
#define rasterfontH

namespace zbuf {

class ZBitmap;class Bitmap;

struct RasterMask {
  void *map;
  int W,H;
  int size;
  void countsize() {size=W*H/8+4;}
  RasterMask(int w,int h) {
    W=w;H=h;countsize();
    map=malloc(size);
    for(int i=0;i<size;i++) ((char*)map)[i]=0;
  }
  ~RasterMask() {free(map);}
  void set(int x,int y) {if (x>=0 and y>=0 and x<W and y<H) setbit1(map,y*W+x);}
  void draw(ZBitmap *dest,int x0,int y0,float z,int color);
  void draw(Bitmap *dest,int x0,int y0,int color);
  void save(BINWRITE &fw);
  RasterMask(BINREAD &fr);
};


struct RasterFont {
  RasterMask *types[0x100];
  int wis[0x100];
  int hi;
  RasterFont(int ahi) {
    for(int i=0;i<0x100;i++) types[i]=NULL,wis[i]=0;hi=ahi;
  }
  ~RasterFont() {for(int i=0;i<0x100;i++) if (types[i]) delete types[i];}
  int text(ZBitmap *dest,int x0,int y0,float z,int color,const char *text);//vrati sirku textu
  int text(Bitmap *dest,int x0,int y0,int color,const char *text);
  RasterMask *setchar(char c,int w) {
    wis[uchar(c)]=w;return types[uchar(c)]=new RasterMask(w,hi);
  }
  RasterFont(BINREAD &fr);
  void save(BINWRITE &fw);
  int textwi(const char *text);
  int texthi() {return hi;}
};


}

#endif
