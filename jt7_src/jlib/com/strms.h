#ifndef HSTRMS
#define HSTRMS

#include <com/com0.h>
#ifndef SEEK_SET
#define SEEK_SET 0
#endif

namespace jlib {

SimpleExceptType(XBinStream,XError);
SimpleExceptType(XBINREAD,XError);
SimpleExceptType(XBINWRITE,XError);

class BINWRITE;

struct BINREAD {
  void *data;//pomocna data, normalne nevyuzito

  bool eof;
  virtual int readdata(void *buf,int size)=0;
  virtual void jump(int n);//preskoci n bytu

//specializovane fce
  long readl() {long l;readdata(&l,sizeof(long));return l;}
  long readb() {bool b;readdata(&b,sizeof(bool));return b;}
  char readc() {char c;readdata(&c,sizeof(char));return c;}
  short readw() {short w;readdata(&w,sizeof(short));return w;}
  int readi() {int i;readdata(&i,sizeof(int));return i;}
  void reads(char *s) {int l=readi();readdata(s,l);s[l]=0;}
  string readstring();
  char *readandallocs();
  string readstreamtostring();//nacte TEXTOVY soubor(x readstring()) do retezce

//pro zaznamy
  template<class T> bool readrec(T& rec) {return readdata(&rec,sizeof(T))==sizeof(T);}
  template<class T> BINREAD &rr(T &data) {int readed=readdata(&data,sizeof(T));xxassert(readed==sizeof(T),XBINREAD);return *this;}

  BINREAD &operator >>(char *s) {reads(s);return *this;}
  BINREAD &operator >>(string &s) {s=readstring();return *this;}
  BINREAD &operator >>(int &i) {return rr(i);}
  BINREAD &operator >>(bool &b) {return rr(b);}
  BINREAD &operator >>(XYREC &p) {return rr(p);}
  BINREAD &operator >>(float &x) {return rr(x);}
  BINREAD &operator >>(double &x) {return rr(x);}
  int readtostream(int bytes,BINWRITE &fw);
  template<class T> BINREAD &operator >>(TCntVec<T> &a) {
    int elemsize=readi();xxassert(sizeof(T)==elemsize,XBINREAD);
    int cnt=readi();a.wantcapa(cnt);readdata(a.buf,cnt*sizeof(T));
    a.count=cnt;
    return *this;
  }
  BINREAD &magic(int magic) {
    int i;rr(i);xassert(magic==i);return *this;
  }
  BINREAD() {data=NULL;}
};

struct BINWRITE {
  void *data;

  virtual int writedata(const void *buf,int size)=0;

//typove procedury
  void writei(int i) {writedata(&i,sizeof(int));}
  void writel(long l) {writedata(&l,sizeof(long));}
  void writes(const char *s) {int l=strlen(s);writei(l);writedata(s,l);}
  void writeb(bool b) {writedata(&b,sizeof(bool));}
  void writec(char c) {writedata(&c,sizeof(char));}
  void writew(short w) {writedata(&w,sizeof(short));}
  //zapise s prvnim nejvyznamnejsim bytem
  void writew1(short w);
  void writel1(long l);
  void writestring(string s);

//pro struktury dane odkazem
  template<class T> void writerec(const T& rec) {writedata(&rec,sizeof(T));}
  template<class T> BINWRITE &wr(const T& rec) {writedata(&rec,sizeof(T));return *this;}

  virtual void writefromstream(BINREAD &stream,int count);

  BINWRITE &operator <<(const char *s) {writes(s);return *this;}
  BINWRITE &operator <<(string s) {writestring(s);return *this;}
  BINWRITE &operator <<(STRING s) {writes(s);return *this;}
  BINWRITE &operator <<(int i) {return wr(i);}
  BINWRITE &operator <<(bool b) {return wr(b);}
  BINWRITE &operator <<(const XYREC &p) {return wr(p);}
  BINWRITE &operator <<(float x) {return wr(x);}
  BINWRITE &operator <<(double x) {return wr(x);}
  template<class T> BINWRITE &operator <<(TCntVec<T> &a) {writei(sizeof(T));writei(a.count);writedata(a.buf,a.count*sizeof(T));return *this;}
  BINWRITE() {data=NULL;}
};

class BINRFILE:public BINREAD {
public:
  FILE *fr;
  BINRFILE(FILE *afr) {fr=afr;eof=false;}
  BINRFILE(const char *file,const char *openmode="rb");
  int readdata(void *buf,int size);
  ~BINRFILE() {if (fr) fclose(fr);}
  virtual void jump(int n);
  ///nacte az do konce souboru,vrati pocet bytu v size
  void *readtoeof(int *size=NULL);
};

class BINWFILE:public BINWRITE
{
public:
  FILE *fw;
  BINWFILE(FILE *afw) {fw=afw;}
  BINWFILE(const char *file,const char *openmode="wb");
  ~BINWFILE() {close();}
  int writedata(const void *buf,int size);

  //specialni funkce
  BINWFILE() {fw=NULL;}
  bool open(const char *file,const char *openmode="wb");
  void close() {if (fw) fclose(fw);fw=NULL;}
  void seekbeg(int ofs) {if (fw) fseek(fw,ofs,SEEK_SET);}
};

class MEMORYREAD:public BINREAD
{
  char *buffer,*readpos,*afterend;
public:
  MEMORYREAD(char *buf,int len,bool copyintoprivatebuf=false);
  MEMORYREAD(BINREAD &fr);
  ~MEMORYREAD() {if (buffer) free(buffer);}
  int readdata(void *buf,int size);
};

class MEMORYWRITE:public BINWRITE
{
  static const int jmpcapa=0x100;
  char *buffer;
  int capa,writepos;
  void wantcapa(int newcapa) {if (newcapa>capa) buffer=chrealloc(buffer,capa=newcapa+jmpcapa);}
public:
  int size() {return writepos;}
  void *getbuf() {return buffer;}
  MEMORYWRITE() {buffer=NULL;capa=0;writepos=0;}
  ~MEMORYWRITE() {free(buffer);}
  int writedata(const void *buf,int size);
  void save(BINWRITE &fw) {fw<<size();fw.writedata(buffer,size());}
};


class MEMORYSTREAM:public BINWRITE,public BINREAD {
  static const int jmpcapa=0x100;
  char *buffer;
  int capa,writepos,readpos;
  void wantcapa(int newcapa) {if (newcapa>capa) buffer=chrealloc(buffer,capa=newcapa+jmpcapa);}


  void _init() {buffer=NULL;capa=0;readpos=0;writepos=0;}
  void _changeddata() {eof=readpos>=writepos;}
public:
  int writedata(const void *buf,int size);
  int readdata(void *buf,int size);

  int size() {return writepos;}
  void savedata(BINWRITE &fw) {fw.writedata(buffer,size());}
  void save(BINWRITE &fw) {fw<<size();savedata(fw);}

  int load(BINREAD &fr);//nacte z fr,vrati velikost streamu, pokud neco bylo v memorystreamu,
  //bude to prepsano
  const void *getbuf() {return buffer;}

  void reset() {readpos=0;_changeddata();}
  void rewrite() {readpos=0;writepos=0;_changeddata();}

  MEMORYSTREAM() {_init();}
  ~MEMORYSTREAM() {if (buffer) free(buffer);}

  bool equaldata(MEMORYSTREAM &s2) {return size()==s2.size() and memcmp(buffer,s2.buffer,size())==0;}
};

struct BUFWRITER:BINWRITE {
  char *buf;
  int size;
  char *act;
  BUFWRITER(char *abuf,int asize) {buf=abuf;size=asize;act=buf;}
  int writedata(const void *abuf,int asize) {
    xassert(asize<=(size-(act-buf)));
    memmove(act,abuf,asize);
    act+=asize;
    return asize;
  }
};

}
#endif
