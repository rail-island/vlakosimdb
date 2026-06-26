#ifndef glotexturesH
#define glotexturesH

#include <com/rcobj.h>
#include <com/ptrto.h>
#include <opengl.h>
#include <ext/persistentobject.h>

namespace glo {

struct JGLError:JError {
  JGLError(string msg):JError(msg) {}
};

struct MemoryTexture {
  int w,h;
  unsigned char *buf;
  MemoryTexture() {buf=NULL;w=h=0;}
  ~MemoryTexture() {clear();}
  void clear() {if (buf) free(buf);buf=NULL;w=h=0;}
  void load(string file);
};

struct TextureId {
  void *ctx;
  unsigned int id;
};

struct Texture:RCObject {
  string name;
  RefBase<PersistentObject> textures;
  Texture(string name,string file,PersistentObject *atextures);
  ~Texture() {unload();}
  void unload();
  void bind(void *ctx) {wantinvideo(ctx);glBindTexture(GL_TEXTURE_2D,gettexid(ctx));}
  float ratiohw() {wantinmemory();if (memtex->w>0) return (float)memtex->h/(float)memtex->w;return 0;}
  float ratiowh() {wantinmemory();if (memtex->h>0) return (float)memtex->w/(float)memtex->h;return 0;}
  int getw() {wantinmemory();return memtex->w;}
  int geth() {wantinmemory();return memtex->h;}
  void deletedcontext(void *ctx);
  void *getbits() {wantinmemory();return memtex->buf;}
  void setrepeat_s(bool value) {repeat_s=value;}
  void setrepeat_t(bool value) {repeat_t=value;}
  bool getrepeat_s() {return repeat_s;}
  bool getrepeat_t() {return repeat_t;}
  void settransparent(bool value) {transparent=value;}
  bool gettransparent() {return transparent;}
private:
  string file;
  bool transparent;
  bool repeat_s,repeat_t;
  PtrTo<MemoryTexture> memtex;
  TCntVec<TextureId> ids;
  void settexid(void *ctx,unsigned int id);
  unsigned int gettexid(void *ctx);
//  unsigned int texid;//identifikace textury v ramci OpenGL

  void wantinmemory() {if (!memtex) loadtomemory();}
  void wantinvideo(void *ctx) {if (!gettexid(ctx)) loadtovideo(ctx);}
  void loadtomemory();//nacte z bmp souboru
  void loadtovideo(void *ctx);//nacte z pameti do videopameti

  friend class TextureManager;
};

struct TextureManager {
  TRCArray<Texture> textures;
  //pokud uz takova textura existuje, vrati ji, jinak vytvori novou
  Texture *gettexture(string name,string file,PersistentObject *textures);
  Texture *findtexture(string name,PersistentObject *textures);//v pripade nenalezeni hlasi chyby
  Texture *tryfindtexture(string name,PersistentObject *textures);//v pripade nenalezeni vrati NULL
  void clear() {textures.clear();}
  string getfile(string texname);
  void deletedcontext(void *ctx) {for(int i=0;i<textures.count;i++) textures[i]->deletedcontext(ctx);}
private:
  Texture *searchtexture(string name,PersistentObject *textures);
};

extern TextureManager *texman;
extern sFile texbasedir;//kde se hledaji texture
extern sFile matbasedir;//kde se hledaji materialy

void init_texture_manager();

void checkglerror();

}

#endif
