#include <jlib.h>
#pragma hdrstop

#include "awschecklist.h"
#include "awssystem.h"

namespace aws {

void CheckList::loadprops(XMLNode xml,Form *form) {
  ScrolledPainter::loadprops(xml,form);
  ev_change.load(xml,"onchange");
  ev_check.load(xml,"oncheck");
  if (xml.isvar("visiblecheck")) drawchecks=xml.ivar("visiblecheck");
  string its=xml.svar("items");
  its.replaceall(";","\n");
  setitems(lang_text_ch(its));
}

void CheckList::loaddata(XMLNode xml) {
  ScrolledPainter::loaddata(xml);
  setchecked(xml.getvalue(getroot(xml)));
}
void CheckList::savedata(XMLNode xml) {
  ScrolledPainter::savedata(xml);
  xml.setvalue(getroot(xml),getchecked());
}

string CheckList::getchecked() {
  MEMSTROUT res;
  for(int i=0;i<checked.count;i++) res<<itos((int)checked[i])<<";";
  return res.str();
}
void CheckList::setchecked(string value) {
  int index=0;
  for(lineiter it=value.getlines(';');*it && index<items.count;it++,index++)
    checked[index]=atoi(*it);
  repaint();
}

void CheckList::setcur(int value) {
  cur=value;
  if (cur<0) cur=0;
  if (cur>=items.count) cur=items.count;
  repaint();
}
void CheckList::setitems(string value) {
  items.clear();
  for(lineiter it=value.getlines('\n');*it;it++) items.add(*it);
  checked.clear();
  for(int i=0;i<items.count;i++) checked.add(false);
  updateinsize();
  repaint();
}
void CheckList::onsize(int w,int h) {
  ScrolledPainter::onsize(w,h);
  updateinsize();
}
void CheckList::updateinsize() {
  Size s=getsize();
  setinsize(Size(s.w-system->sysopt->checklistindsize,linehi*items.count));
}

string CheckList::getitems() {
  MEMSTROUT res;
  for(int i=0;i<items.count;i++) res<<items[i]<<"\n";
  return res.str();
}

void CheckList::onpaint(Canvas &cnv,awsEvent &event) {
  Rect clip=cnv.clip();
  cnv.fg(clnnone).bg(clnwhite).rect(clip);
  cnv.font(getfont());
  int lh2=linehi;
  linehi=cnv.texthi("A");
  if (lh2!=linehi) updateinsize();
  int x0=0;
  if (drawchecks) x0=16;
  for(int i=0;i<items.count;i++) {
    if (i==cur) cnv.bg(clnlime).rect(0,i*linehi,clip.b.x,i*linehi+linehi);
    cnv.text(x0,i*linehi,items[i]);
    if (drawchecks) cnv.drawcheck(8,i*linehi+linehi/2,checked[i]);
  }
}

void CheckList::onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  ScrolledPainter::onmousedown(x,y,button,shift,event);
  int line=y/linehi;
  cur=line;
  if (cur<0) cur=0;if (cur>=items.count) cur=items.count-1;
  if (x<16) _check(line);
  setfocus();
  dochange();
  repaint();
}

void CheckList::_check(int line) {
  if (line>=0 && line<checked.count) {
    checked[line]=!checked[line];
    docheck(line,checked[line]);
  }
}

void CheckList::onkeydown(awsKey key,awsEvent &event) {
  bool pnt=false;
  if (key==kt_down) cur++,pnt=true;
  if (key==kt_up) cur--,pnt=true;
  if (key==kt_right) cur++,pnt=true;
  if (key==kt_left) cur--,pnt=true;
  if (key==kt_space) _check(cur),pnt=true;
  if (key==kt_pgup) cur-=getsize().h/linehi,cur++,pnt=true;
  if (key==kt_pgdn) cur+=getsize().h/linehi,cur--,pnt=true;
  if (pnt) {
    if (cur<0) cur=0;
    if (cur>=items.count) cur=items.count-1;
    dochange();
    scrollinview(currect());
    repaint();
  }
  ScrolledPainter::onkeydown(key,event);
}

CheckList::CheckList(Component *owner,System *system):ScrolledPainter(owner,system) {
  drawchecks=true;
  cur=-1;
  Font *f=getfont();
  linehi=f?f->getheight():1;
}

rtti_define(CheckedEvent)

void init_aws_checklist_rtti() {
  using namespace rtti;
  
  rtti_impl(CheckedEvent)

  CheckedEventClass->init_rec<CheckedEvent>("CheckedEvent");

  rtti_def_fld(CheckedEvent,line,Int);
  rtti_def_fld(CheckedEvent,checked,Int);
}


}
