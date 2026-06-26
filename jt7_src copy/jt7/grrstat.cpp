#include <jlib.h>
#pragma hdrstop

#include "grrstat.h"
#include "gview.h"
#include "gameu.h"
#include "grrmap.h"
#include "gtrain.h"
#include "ghimap.h"
#include "rdijskr.h"
#include "gsurmap.h"

namespace jtyc {

void RStation::clearvisibility() {
  partvisible=false;
  instance->clearvisibility();
}
void RStation::dooctalize() {
  instance->octalize(this,x,y,z,dir,gocttree);
}
void RStation::ot_render(int id) {
  partvisible=true;
  instance->ot_render(id);
}
void RStation::dodelize() {
  gocttree->removegroup(this);
}


RRSPOJ RStation::getsp(int track,int item) {
  if (track<0 || track>=trc || item<0 || item>=trl) throw JError("Bad RStatikn::getsp() index");
  switch (dir) {
    case 0:return RRSPOJ(game->rrmap->at(x+item,y+track,RItem::rst),Spojnice(-tt_24));
    case 1:return RRSPOJ(game->rrmap->at(x+track,y+item,RItem::rst),Spojnice(-tt_13));
    case 2:return RRSPOJ(game->rrmap->at(x+w-item-1,y+track,RItem::rst),Spojnice(tt_24));
    case 3:return RRSPOJ(game->rrmap->at(x+track,y+h-item-1,RItem::rst),Spojnice(tt_13));
  }
  return RRSPOJ();
}

bool RStation::getstarttrack(TCntVec<RRSPOJ> &start,float minlen) {
  if (trl<minlen) {
    game->seterror("$Train-is-longer-than-depo");
    return false;
  }
  for(int i=0;i<trc;i++) {
    if (getsp(i,0).ri->canaddtrain()) {
//      for(int j=trl-1;j>=0;j--) start.add(getsp(i,j));
      for(int j=0;j<trl;j++) start.add(getsp(i,j));
      return true;
    }
  }
  game->seterror("$Not-free-rail-detected");
  return false;
}

RStation::RStation(int x,int y,int adir,int atrc,int atrl,int w,int h,float z,bool aisdepo):Station(x,y,w,h) {
//  bmp=new StaticBmp(game->images->rst);
  dir=adir;
  isdepo=aisdepo;
  this->z=z;
  if (isdepo) setname(lang_text_ch("$Depot 1"));else setname(lang_text_ch("$Station 1"));
  trc=atrc;trl=atrl;//createfreetrack();
  setcomplex(NULL);
//  complex=game->images->simplestation;
  created(true);
}

RStation::RStation(XMLNode xml):Station(xml) {
//  bmp=new StaticBmp(game->images->rst);
  trc=xml.ivar("trc");
  trl=xml.ivar("trl");
  dir=xml.ivar("vertical");
  isdepo=xml.ivar("depo");
  z=xml.fvar("z");//createfreetrack();
  if (isdepo) setcomplex(gimages->finddepo(xml.svar("complex")));else setcomplex(gimages->findstation(xml.svar("complex")));

  for(lineiter it=xml.svar("items").getlines(';');*it;it++) {
    RstRItem *ri=dynamic_cast<RstRItem*>(game->rrmap->finditemid(atoi(*it)));
    if (!ri) throw JError("Error loading station "+getname());
    ri->rstation=this;
    ritems.add(ri);
  }

  created(false);

}

bool RStation::canremove() {
  bool res=true;
  for(int i=0;i<ritems.count;i++) if (ritems[i]->trains>0) res=false;
  if (!res) game->seterror("$Cannot-destroy-rstation");
  return res;
}

void RStation::created(bool createritems) {
  game->surmap->block(x,y,w,h);
  game->himap->block(x,y,w+1,h+1);
  if (createritems) {
    for(int cx=0;cx<w;cx++) for(int cy=0;cy<h;cy++) {
      bool vertical=dir%2;
      RstRItem *ri=new RstRItem(x+cx,y+cy,z,vertical,vertical?cx:cy,this);
      ritems.add(ri);
    }
  }
}


void RStation::notifyremove(GInstance *obj) {
  Station::notifyremove(obj);
//  if (RstRItem *ri=dynamic_cast<RstRItem*>(obj)) ritems.quickremove(ri);
}

void RStation::xmlsave(XMLNode xml) {
  Station::xmlsave(xml);
  xml.setname("RStation");
  xml.setvar("trc",trc);
  xml.setvar("trl",trl);
  xml.setvar("vertical",dir);
  xml.setvar("z",z);
  xml.setvar("complex",complex->name);
  xml.setvar("depo",isdepo);
  bstring its;
  for(int i=0;i<ritems.count;i++) its<<itos(ritems[i]->saveid)<<";";
  xml.setvar("items",its);
}

void RStation::setcomplex(jtStation *acomplex) {
  complex=acomplex;
  if (!complex) {
    if (isdepo) complex=gimages->depo0;
    else complex=gimages->station0;
  }
  instance=complex->complete(trc,trl);
}

void RStation::settype(string value) {
  delize();
  if (isdepo) setcomplex(gimages->finddepo(value));
  else setcomplex(gimages->findstation(value));
}

void RStation::destroy() {
  for(int i=0;i<ritems.count;i++) delete ritems[i];
  ritems.clear();
  game->surmap->unblock(x,y,w,h);
  game->himap->unblock(x,y,w+1,h+1);
  game->rrmap->modify();
  Station::destroy();
}

void RStation::rendercaptions(RenderContext &rc) {
  if (!partvisible) return;
  rendertitle(rc,getname(),x,y,z);
}

void RStation::getdijvrs(TCntVec<DijRVr*> &vrs) {
  if (dir%2) {
    for(int i=0;i<trc;i++) {
      vrs.add(RRVRCH(game->rrmap->at(x+i,y,RItem::rst),Vrchol(vr_3)).dijvr());
      vrs.add(RRVRCH(game->rrmap->at(x+i,y+h-1,RItem::rst),Vrchol(vr_1)).dijvr());
    }
  } else {
    for(int i=0;i<trc;i++) {
      vrs.add(RRVRCH(game->rrmap->at(x,y+i,RItem::rst),Vrchol(vr_4)).dijvr());
      vrs.add(RRVRCH(game->rrmap->at(x+w-1,y+i,RItem::rst),Vrchol(vr_2)).dijvr());
    }
  }
}

void RStation::render(RenderContext &rc) {
  instance->render(x,y,z,rc,dir);
}


}

