
#include <jlib.h>
#pragma hdrstop

#include "spaceedit.h"
#include <gr/objs2d.h>
#include "bmp3d.cpp"
#include "bytebmp.cpp"

namespace zbuf {
using namespace gr;

sFile vxadir;

void SPColor::editprops() {
  string imgs="Ze souboru;";
  if (images) for(int i=0;i<images->count();i++) imgs<<(*images)[i].svars["name"]<<";";
  imageindex++;
  jsystem->editvalues("Obarveni",'c',&imageindex,"Zdroj",imgs.c_str(),'o',&file,"Barvici soubor",'c',&dir,"Smer barveni","zepredu;zezadu;zleva;zprava;zezdola;zeshora;",
  'i',&numcolor,"Pocet obarvovanych pixelu",'c',&linesasfill,"Cary jako vypln","Ne;Ano;",0);
  imageindex--;
}

void SPCut::editprops() {
  string imgs="Ze souboru;";
  if (images) for(int i=0;i<images->count();i++) imgs<<(*images)[i].svars["name"]<<";";
  imageindex++;
  jsystem->editvalues("Orezani",'c',&imageindex,"Zdroj",imgs.c_str(),'o',&file,"Orezavaci soubor",'c',&dir,"Smer orezavani","zepredu;zboku;zeshora;",0);
  imageindex--;
}


LVoxels *SPBMPWrapper::makevoxs() {
  SPBMP *bmp=redraw();
  VOXELARRAY<Lcolor,float> *r1=bmp->shape<float>(-x0*koef,-y0*koef,-z0*koef,1.0/quality);
  delete bmp;
  return r1;
}

SPAction *SPAction::loadaction(XMLItem &xml,XMLItem *images) {
  if (xml.name=="box") return new SPBOX(xml);
  if (xml.name=="poly") return new SPPolyAction(xml,images);
  if (xml.name=="cut") return new SPCut(xml,images);
  if (xml.name=="color") return new SPColor(xml,images);
  return NULL;
}

//static REMAP os_maps[3]={REMAP(1,2,0),REMAP(0,2,1),REMAP(0,1,2)};
//static REMAP os_maps[3]={REMAP(2,0,1),REMAP(0,2,1),REMAP(0,1,2)};

/*static void perform2dto3daction(SPBMP &bmp,int x0,int y0,int z0,double koef,
                REMAP map,void (BMP3D<uchar>::*action)(BYTEBMP<uchar>&,int,int,REMAP)
                ,string file) {
  if (file=="") return;
  VPOINT p0(x0,y0,z0);
  XMLDocument xml;
  if (xml.load(basedir/"images"/file)) {
    gr::GRCompoud grc(xml.main);

    BYTEBMP<uchar> bytes(grc.w*koef,grc.h*koef);
    BMPCANVAS<uchar> cnv(&bytes,logictopal16);

    grc.draw(SUBCANVAS(cnv,grc.x0*koef,grc.y0*koef,koef,koef));

    (bmp.*action)(bytes,(p0.x(map.inv())-grc.x0)*koef,(p0.y(map.inv())-grc.y0)*koef,map);
  }
}*/

void SPCut::perform(XMLItem &xml,SPBMP &bmp,int x0,int y0,int z0,double koef) {
  REMAP os_maps[3]={REMAP(2,0,1),REMAP(0,2,1),REMAP(0,1,2)};
  REMAP map=os_maps[dir];
  VPOINT p0(x0,y0,z0);
  if (dir<0 or dir>=3) return;
  gr::GRCompoud grc(xml);
  BYTEBMP<Lcolor> bytes(grc.w*koef,grc.h*koef,clnnone);
  BMPCANVAS<Lcolor> cnv(&bytes,logictofg,logictobg);
  grc.draw(SUBCANVAS(cnv,grc.x0*koef,grc.y0*koef,koef,koef));
  bmp.zcut(bytes,(p0.x(map.inv())-grc.x0)*koef,(p0.y(map.inv())-grc.y0)*koef,map);
}

void SPImageAction::perform(SPBMP &bmp,int x0,int y0,int z0,double koef) {
  if (imageindex>=0) {
    if (imageindex<images->count()) perform((*images)[imageindex]["object"],bmp,x0,y0,z0,koef);
  } else {
    if (file=="") return;
    XMLDocument xml;
    if (xml.load(basedir/"images"/file)) perform(xml.main,bmp,x0,y0,z0,koef);
  }
}

void SPColor::perform(XMLItem &xml,SPBMP &bmp,int x0,int y0,int z0,double koef) {
  REMAP os_maps[3]={REMAP(2,0,1),REMAP(0,2,1),REMAP(0,1,2)};
  if (dir<0 or dir>=6) return;
  REMAP map=os_maps[dir/2];
  VPOINT p0(x0,y0,z0);

  gr::GRCompoud grc(xml);

  BYTEBMP<Lcolor> bytes(grc.w*koef,grc.h*koef,clnnone);
  BMPCANVAS<Lcolor> cnv(&bytes,logictofg,logictobg);

  grc.draw(SUBCANVAS(cnv,grc.x0*koef,grc.y0*koef,koef,koef));

  if (linesasfill) {
    for(int x=0;x<bytes.W;x++) for(int y=0;y<bytes.H;y++)
      bytes.scanline[y][x].isline=false;
  }

  if (dir%2) bmp.zcolorback(bytes,(p0.x(map.inv())-grc.x0)*koef,(p0.y(map.inv())-grc.y0)*koef,map,numcolor);
  else bmp.zcolor(bytes,(p0.x(map.inv())-grc.x0)*koef,(p0.y(map.inv())-grc.y0)*koef,map,numcolor);

}


}
#pragma package(smart_init)
