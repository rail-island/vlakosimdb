#include <jlib.h>
#pragma hdrstop

#include "awspager.h"

namespace aws {

awsHeader::awsHeader(awsPager *apager,System *system):PaintControl(apager,system) {
  pager=apager;
}

awsPageSheet::awsPageSheet(awsPager *apager,System *system):Place(apager,system) {
  pager=apager;
  wi=-1;textwi=-1;
}

void awsHeader::onmouseup(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  PaintControl::onmouseup(x,y,button,shift,event);
  int actx=0;
  for(int i=0;i<pager->sheets.count;i++) {
    awsPageSheet *sh=pager->sheets[i];
    if (x>=actx && x<actx+sh->wi && y>=0 && y<pager->hdrsize) pager->selectpage(i);
    actx+=sh->wi;
  }
}

void awsHeader::onpaint(Canvas &cnv,awsEvent &event) {
  PaintControl::onpaint(cnv,event);
  pager->drawhdr(cnv,getsize());
}

void awsPager::drawhdr(Canvas cnv,Size s) {
  cnv.fg(clnnone).bg(clnlime).rect(0,0,s.w,s.h);
  int actx=0;
  for(int i=0;i<sheets.count;i++) {
    awsPageSheet *sh=sheets[i];
    sh->textwi=cnv.textwi(sh->text);
    sh->wi=sh->textwi+alignleft+alignright;
    cnv.drawframe(Rect(actx,0,sh->wi,hdrsize),i==cur);
    cnv.centertext(Rect(actx,0,sh->wi,hdrsize),sh->text,true,true,true);
    actx+=sh->wi;
  }
}

void awsPager::notifyremove(Component *obj) {
  Place::notifyremove(obj);
  if (obj==hdr) hdr=NULL;
  if (awsPageSheet *sh=dynamic_cast<awsPageSheet*>(obj)) sheets.remove(sh);
}

void awsHeader::notifyremove(Component *obj) {
  PaintControl::notifyremove(obj);
  if (obj==pager) pager=NULL;
}

void awsPageSheet::notifyremove(Component *obj) {
  Place::notifyremove(obj);
  if (obj==pager) pager=NULL;
}

awsPager::awsPager(Component *owner,System *system):Place(owner,system) {
  hdrsize=20;cur=-1;alignleft=5;alignright=5;
  hdr=new awsHeader(this,system);
  hdr->setparent(this);
}

void awsPager::onsize(int w,int h) {
  Place::onsize(w,h);
  for(int i=0;i<sheets.count;i++) {
    awsPageSheet *sh=sheets[i];
    sh->setpos(Point(0,hdrsize),Size(w,h-hdrsize));
  }
  hdr->setpos(0,0,w,hdrsize);
}

void awsPager::selectpage(int page) {
  if (cur>=0) sheets[cur]->setvisible(false);
  cur=page;
  if (cur>=sheets.count) cur=-1;
  if (cur>=0) sheets[cur]->setvisible(true);
}

void awsPager::loadprops(XMLNode xml,Form *form) {
  denyloadchilds=true;
  Place::loadprops(xml,form);
  for(int i=0;i<xml.count();i++) {
    cur=0;
    awsPageSheet *sh=new awsPageSheet(this,system);
    sheets.add(sh);
    sh->setvisible(i==0);
    sh->loadprops(xml[i],form);
    sh->setparent(this);
  }
  onsize(getsize().h,getsize().w);
}

void awsPageSheet::loadprops(XMLNode xml,Form *form) {
  Place::loadprops(xml,form);
  text=lang_text_ch(xml.svar("text"));
}


}

