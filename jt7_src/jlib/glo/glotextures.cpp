#include <jlib.h>
#pragma hdrstop

#include "glotextures.h"
#include <ext/jbmpfile.h>
#include <gr/comgr.h>

namespace glo {

/*
void checkglerror() {
  int error=glGetError();
  if (error!=GL_NO_ERROR) throw JGLError(string("OpenGL error:")+gluErrorString(error));
}
*/

Texture::Texture(string name,string file,PersistentObject *atextures):name(name),file(file) {
//  texid=0;
  transparent=true;
  repeat_s=false;
  repeat_t=false;
  textures=atextures;
}
void Texture::settexid(void *ctx,unsigned int id) {
  for(int i=0;i<ids.count;i++) if (ids[i].ctx==ctx) {
    ids[i].id=id;
    return;
  }
  TextureId &newid=ids.add();
  newid.ctx=ctx;newid.id=id;
}
unsigned int Texture::gettexid(void *ctx) {
  for(int i=0;i<ids.count;i++) if (ids[i].ctx==ctx) return ids[i].id;
  return 0;
}
void Texture::deletedcontext(void *ctx) {
  for(int i=0;i<ids.count;i++) if (ids[i].ctx==ctx) {
    ids.erase(i);
    return;
  }
}


void init_texture_manager() {
  texman=new TextureManager();
}

TextureManager *texman;
sFile texbasedir;
sFile matbasedir;

void MemoryTexture::load(string file) {
  BMPData bd;
  bd.load(file);
  w=bd.w;h=bd.h;
  buf=(unsigned char*)malloc(4*w*h);
  bd.fill32((int*)buf,true);
}

struct NoiseElem {
  int color;
  float r,g,b;
};

static unsigned char ch_color(unsigned char color,float f) {
  float rnd=1-2*rand()/(float)RAND_MAX;
  int res=color+f*rnd*(color+16);
  if (res<0) res=0;
  if (res>255) res=255;
  return (unsigned char)res;
}

void Texture::loadtomemory() {
  memtex=new MemoryTexture();
  XMLNode des=XMLNode::create();
  try {
    if (textures) {
      if (textures->hasfile(sFile(file)%"xds"))
        textures->loadxml(sFile(file)%"xds",des);
      string filename;
      PersistentObject *tchild=textures->findchildwithfile(file,&filename);
      string path=tchild->getfile(filename);
      memtex->load(path);
      tchild->closedgetfile(path,filename);
    } else {
      memtex->load(file);
    }
  } catch (...) {
    memtex=NULL;
    throw;
  }
  int size=memtex->w*memtex->h;
  unsigned char *cbuf=memtex->buf;
  if (des["filter"].searchsub("noise")>=0) {
    TCntVec<NoiseElem> noise;
    XMLNode xn=des["filter"]["noise"];
    for(int i=0;i<xn.count();i++) {
      NoiseElem &ne=noise.add();
      if (xn[i].svar("srccolor")=="*") ne.color=-1;
      else ne.color=gr::strtonativecolor(xn[i].cvar("srccolor"));
      ne.r=xn[i].ivar("per_r")/100.0;
      ne.g=xn[i].ivar("per_g")/100.0;
      ne.b=xn[i].ivar("per_b")/100.0;
    }
    int *ib=(int*)cbuf;
    unsigned char *cb=memtex->buf;
    for(int i=0;i<size;i++,ib++,cb+=4) {
      for(int j=0;j<noise.count;j++) {
        if (*ib==noise[j].color || noise[j].color==-1) {
          cb[0]=ch_color(cb[0],noise[j].r);
          cb[1]=ch_color(cb[1],noise[j].r);
          cb[2]=ch_color(cb[2],noise[j].r);
        }
      }
    }
  }
  int *ibuf=(int*)cbuf;
  if (transparent) {
    int color0=(*ibuf)&0xFFFFFF;
    for(int i=0;i<size;i++,ibuf++,cbuf+=4) {
      if (((*ibuf)&0xFFFFFF)==color0) cbuf[3]=0;else cbuf[3]=255;
    }
  } else {
    for(int i=0;i<size;i++,cbuf+=4) cbuf[3]=255;
  }
}

void Texture::unload() {
  memtex=NULL;
//  if (texid) glDeleteTextures(1,&texid);
  unsigned int texid=gettexid(NULL);
  if (texid) glDeleteTextures(1,&texid);
  texid=0;
}

void Texture::loadtovideo(void *ctx) {
//  checkglerror();
//  glEnable(GL_TEXTURE_2D);
//  checkglerror();

  wantinmemory();
  unsigned int texid=0;
  glGenTextures(1,&texid);
  settexid(ctx,texid);
//  texid=2;
//  glDeleteTextures(1,&texid);
//  glGenTextures(1,&texid);
  if (!texid) throw JGLError("glGenTextures()");
  glBindTexture(GL_TEXTURE_2D,texid);
//  int fmt=GL_RGB;if (transparent) fmt=GL_RGBA;
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
  if (repeat_s) glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  if (repeat_t) glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  
  gluBuild2DMipmaps(GL_TEXTURE_2D,4,memtex->w,memtex->h,GL_RGBA,GL_UNSIGNED_BYTE,memtex->buf);
}

static bool gettexturefile(string texname,string &file,bool *repeat=NULL,bool *pack=NULL) {
  if (texname.begins("file:")) {
    file=texbasedir/texname.copy(5,texname.length()-5);
    return true;
  }
  if (texname.begins("material:")) {
    file=matbasedir/texname.copy(9,texname.length()-9);
    if (repeat) *repeat=true;
    return true;
  }
  if (texname.begins("pack:")) {
    file=texname.copy(5,texname.length()-5);
    if (pack) *pack=true;
    return true;
  }
  if (texname.begins("*pack:")) {
    file=texname.copy(6,texname.length()-6);
    if (pack) *pack=true;
    if (repeat) *repeat=true;
    return true;
  }
  return false;
}

Texture *TextureManager::searchtexture(string name,PersistentObject *atextures) {
  for(int i=0;i<textures.count;i++) if (textures[i]->name==name && textures[i]->textures==atextures) return textures[i];
  return NULL;
}

Texture *TextureManager::tryfindtexture(string name,PersistentObject *textures) {
  if (Texture *res=searchtexture(name,textures)) return res;
  bool repeat=false,pack=false;string file;
  ///textury bez symbolickeho jmena neni treba predem definovat
  if (gettexturefile(name,file,&repeat,&pack)) {
    Texture *res=gettexture(name,file,pack?textures:NULL);
    res->repeat_s=res->repeat_t=repeat;
    return res;
  }
  return NULL;
}

Texture *TextureManager::findtexture(string name,PersistentObject *textures) {
  Texture *res=tryfindtexture(name,textures);
  if (!res) throw JGLError("Texture not found:"+name);
  return res;
}

Texture *TextureManager::gettexture(string name,string file,PersistentObject *atextures) {
  Texture *res=searchtexture(name,atextures);
  if (res) return res;
  res=new Texture(name,file,atextures);
  textures.add(res);
  return res;
}

string TextureManager::getfile(string texname) {
  string file;
  if (gettexturefile(texname,file)) return file;
  Texture *tex=searchtexture(texname,NULL);
  if (tex) return tex->file;
  return "";
}

}

