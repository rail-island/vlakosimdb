#include <jlib.h>
#pragma hdrstop

#include "awsproplist.h"
#include "awsreal.h"
#include "awssystem.h"
#include <com/sorting.cpp>

namespace aws {

PropertyEdit::PropertyEdit(PropertyListInner *owner,System *system):Edit(owner,system),proplist(owner) {
  hide();
}

void PropertyListInner::save() {
  if (editline>=0) vars[editline]->value=edit->gettext(),proplist->modify();
  proplist->dochange();
}

PropertyListInner::PropertyListInner(PropertyList *owner,System *system):PaintControl(owner,system),proplist(owner) {
  valleft=itemhi=wi=0;
  edit=new PropertyEdit(this,system);
  edit->setparent(this);
  editline=-1;
//  RealControl *c=getreal();
//  const char *name=typeid(*c).name();
}

PropertyList::PropertyList(Component *owner,System *system):ScrollBox(owner,system) {
  inner=new PropertyListInner(this,system);
  inner->setparent(this);
  modified=false;
}

void PropertyListInner::cancel() {
  if (editline>=0) edit->settext(vars[editline]->value),edit->selectall();
}
void PropertyListInner::ok() {
  if (editline>=0) save(),edit->selectall(),proplist->modify();
}

void PropertyEdit::onkeydown(awsKey key,awsEvent &event) {
  if (key==kt_down) {
    event.done();
    proplist->nextprop();
  }
  if (key==kt_up) {
    event.done();
    proplist->prevprop();
  }
  if (key==kt_esc) {
    event.done();
    proplist->cancel();
  }
  if (key==kt_enter) {
    event.done();
    proplist->ok();
  }
}

void PropertyListInner::showedit(int line) {
  save();
  editline=line;
  if (line>=0) {
    edit->setpos(valleft,line*itemhi);
    edit->settext(vars[line]->value);
    edit->show();
    edit->setfocus();
    edit->selectall();
    proplist->scrollinview(edit);
  } else {
    edit->hide();
  }
//  repaint();
}

void PropertyListInner::onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  PaintControl::onmousedown(x,y,button,shift,event);
  if (button!=mb_left) return;
  int item=y/itemhi;
  showedit(item);
  if (item>=0 and item<vars.count) showedit(item);
}


void PropertyListInner::updatesize() {
//  RealControl *c=getreal();
//  const char *name=typeid(*c).name();
  int w=proplist->getclient().size().w;
  int fonthi=8;
  if (Font *fnt=getfont()) fonthi=getfont()->getheight();
  itemhi=fonthi+8;
  int h=itemhi*vars.count;
  valleft=w/2;wi=w;
//  Size s=c->getsize();
  setsize(w,h);
  edit->setsize(wi/2-2,itemhi);
  showedit(editline);
}

void PropertyListInner::onpaint(Canvas &cnv,awsEvent &event) {
  PaintControl::onpaint(cnv,event);
  cnv.font(getfont());
  Rect clip=cnv.clip();
  cnv.fg(clnnone).bg(clnltgray).rect(clip);
  cnv.fg(clngray);
  for(int i=0;i<vars.count;i++) {
//    if (editline==i) cnv.drawframe(Rect(0,i*itemhi,valleft,itemhi),true);
    cnv.text(2,i*itemhi+4,vars[i]->name);
    cnv.text(valleft+2,i*itemhi+4,vars[i]->value);
    cnv.line(0,i*itemhi,wi,i*itemhi);
  }
  cnv.line(valleft,0,valleft,itemhi*vars.count);
  cnv.line(0,vars.count*itemhi-1,wi,vars.count*itemhi-1);
}

void PropertyList::loadprops(XMLNode xml,Form *form) {
  ScrollBox::loadprops(xml,form);
  XMLNode vars=xml["vars"];
  vars.savevars(inner->vars);
  ev_change.load(xml,"onchange");
}

static int compare_vars(VarRecord *a,VarRecord *b) {
  return strcmp(a->name.c_str(),b->name.c_str());
}

void PropertyList::setvars(VarList *vars,bool sort) {
  inner->vars.assign(*vars);
  if (sort) heapsort(inner->vars.buf,inner->vars.count,compare_vars);
  update();
}

void PropertyList::addvar(string name,string value,bool sort) {
  modify();
  inner->vars.setvar(name,value);
  if (sort) heapsort(inner->vars.buf,inner->vars.count,compare_vars);
  update();
  int index=inner->vars.searchtype(name);
  if (index>=0) inner->showedit(index);
}
void PropertyList::deleteactvar() {
  if (inner->editline>=0) {
    modify();
    inner->vars.erase(inner->editline);
    if (inner->editline>=inner->vars.count) inner->editline--;
    update();
  }
}


}


