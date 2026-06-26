#include <jlib.h>
#pragma hdrstop

#include "glovrml.h"
#include "gloobject.h"
#include "dir.h"
#include <freeimage.h>

namespace glo {

sFile VRMLTextures::add(Texture *texture) {
  if (VRMLTexture *t=items.psearchv(texture)) return textdir/t->file;
  VRMLTexture *n=new VRMLTexture();
  n->texture=texture;
  items.add(n);
  n->file="texture"+itos(items.count)+".png";
  return textdir/n->file;
}

void VRMLTextures::save() {
  mkdir(textdir.c_str());
  for(int i=0;i<items.count;i++) items[i]->save(textdir/items[i]->file);
}

void VRMLTexture::save(string file) {
  FIBITMAP *dib=FreeImage_Allocate(texture->getw(),texture->geth(),32);
  void *mem=FreeImage_GetBits(dib);
  int size=texture->getw()*texture->geth()*4;

  memmove(mem,texture->getbits(),size);

  char *mbits=(char*)mem;

  for(int i=0;i<size;i+=4) {
    exchange(mbits[i+0],mbits[i+2]);
    mbits[i+3]=255;
  }

  FreeImage_SavePNG(dib,file.c_str());
  FreeImage_Free(dib);
}


}

