#ifndef bmsH
#define bmsH

#include "strms.h"

namespace jlib {

struct BLOCKMEMORYSTREAM:BINREAD,BINWRITE {
  int minblocksize;
  BLOCKMEMORYSTREAM(int aminblocksize=0x100) {minblocksize=aminblocksize;first=NULL;last=NULL;readpos=0;writepos=0;readblock=NULL;}
  ~BLOCKMEMORYSTREAM() {clear();}

  int size() {return writepos;}
  void reset() {readblock=first;readpos=0;}
  void rewrite() {clear();}

  void savedata(BINWRITE &fw);
  void save(BINWRITE &fw) {fw<<size();savedata(fw);}
  int load(BINREAD &fr);//vrati velikost dat v streamu

  int writedata(const void *buf,int size);
  int readdata(void *buf,int size);
protected:
  struct BLOCK {
    BLOCK *next;
    int capa;
    int writepos;//relativne vuci bloku
    int offset;//offset prvniho byte
    char *data() {return reinterpret_cast<char*>(this+1);}
    void write(const char *buf,int size) {
      xassert(size<=capa-writepos);
      memmove(data()+writepos,buf,size);
      writepos+=size;
    }
    void write(BINREAD &fr,int size) {
      xassert(size<=capa-writepos);
      fr.readdata(data()+writepos,size);
      writepos+=size;
    }
    void init(int acapa,int aoffset) {capa=acapa;next=NULL;writepos=0;offset=aoffset;}
  };

  struct READPROPS {
    BLOCK *rblock;int rpos;
  };

  int writepos;
  int readpos;

  void clear();

private:
  BLOCK *first,*last,*readblock;
  int freebytes();//vrati pocet okamzite dostupne(uz alokovane) pameti
  void allocblock(int size);//alokuje blok o velikosti minimalne size
  void newblock(int datasize);
  int availforread();//vrati pocet bytu primo prectitelnych(v aktualnim bloku)
public:
  READPROPS getreadprops() {READPROPS r;r.rblock=readblock;r.rpos=readpos;return r;}
  void setreadprops(const READPROPS &r) {readblock=r.rblock;readpos=r.rpos;}
};
//---------------------------------------------------------------------------

}
#endif
