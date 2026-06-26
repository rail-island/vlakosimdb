#include <jlib.h>
#pragma hdrstop

#include "awscontrol.h"
#include "awssystem.h"
#include "awsreal.h"
#include "awslayout.h"
#include "awsform.h"

namespace aws {

/*
SaveType Control::loadsavetype(XMLNode xml,SaveType def) {
  string type=xml.svar("savestyle");
  if (type=="var" || type=="variable") return st_variable;
  if (type=="sub" || type=="child") return st_child;
  return def;
}
STRING Control::getxmlsavedprop(XMLNode xml,STRING xmlname,SaveType st) {
  switch (st) {
    case st_child:return xml.getsub(xmlname);
    case st_variable:return xml.cvar(xmlname);
  }
  return "";
}
void Control::setxmlsavedprop(XMLNode xml,STRING xmlname,SaveType st,STRING value) {
  switch (st) {
    case st_child:xml.setsub(xmlname,(string)value);break;
    case st_variable:xml.setvar(xmlname,(string)value);break;
  }
}
*/

/*
void Control::getprop(STRING name,esObject &res) {
  if (name=="x") {Point pos=getpos();res=pos.x;}
  if (name=="y") {Point pos=getpos();res=pos.y;}
  if (name=="w") {Size size=getsize();res=size.w;}
  if (name=="h") {Size size=getsize();res=size.h;}
  if (name=="parent") {Control *parent=getparent();if (parent) res.setidf(parent->getname().c_str());}
}
*/

void Control::onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  MouseEvent ev;
  ev.type=et_mousedown;ev.name="onmousedown";ev.value=ev_mousedown.get();ev.event=&event;
  ev.x=x;ev.y=y;ev.button=button;ev.shift=shift;
  do_event(ev);
}
void Control::onmouseup(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  MouseEvent ev;
  ev.type=et_mouseup;ev.name="onmouseup";ev.value=ev_mouseup.get();ev.event=&event;
  ev.x=x;ev.y=y;ev.button=button;ev.shift=shift;
  do_event(ev);
}
void Control::onkeydown(awsKey key,awsEvent &event) {
  KeyEvent ev;
  ev.type=et_keydown;ev.name="onkeydown";ev.value=ev_keydown.get();ev.event=&event;
  ev.key=key;
  do_event(ev);
}
void Control::onchar(char ch,awsEvent &event) {
  CharEvent ev;
  ev.ch=ch;
  donotify(ev,"onchar",ev_char.get());
}

void Control::onmousewheel(int x,int y,int delta,ShiftState shift,awsEvent &event) {
  MouseWheelEvent ev;
  ev.x=x;ev.y=y;ev.delta=delta;ev.shift=shift;
  donotify(ev,"onmousewheel",ev_mousewheel.get());
}

void Control::onkeyup(awsKey key,awsEvent &event) {
  KeyEvent ev;
  ev.type=et_keyup;ev.name="onkeyup";ev.value=ev_keyup.get();ev.event=&event;
  ev.key=key;
  do_event(ev);
}
void Control::focused(awsEvent &event) {
  NotifyEvent ev;
  ev.type=et_notify;ev.name="onfocus";ev.value=ev_focus.get();ev.event=&event;
  do_event(ev);
}
void Control::unfocused(awsEvent &event) {
  NotifyEvent ev;
  ev.type=et_notify;ev.name="onkillfocus";ev.value=ev_killfocus.get();ev.event=&event;
  do_event(ev);
}
void Control::onmousemove(int x,int y,ShiftState shift,awsEvent &event) {
  MouseMoveEvent ev;
  ev.type=et_mousemove;ev.name="onmousemove";ev.value=ev_mousemove.get();ev.event=&event;
  ev.x=x;ev.y=y;ev.shift=shift;
  do_event(ev);
}
void Control::onmouseleave(awsEvent &event) {
  NotifyEvent ev;
  ev.type=et_mouseleave;ev.name="onmouseleave";ev.value=ev_mouseleave.get();ev.event=&event;
  do_event(ev);
}
void Control::onmouseenter(awsEvent &event) {
  NotifyEvent ev;
  ev.type=et_mouseenter;ev.name="onmouseenter";ev.value=ev_mouseenter.get();ev.event=&event;
  do_event(ev);
}



void Control::centerto(Rect r) {
  Size s=getsize(),big=r.size();
  setpos(r.left()+big.w/2-s.w/2,r.top()+big.h/2-s.h/2);
}

void Control::loadprops(XMLNode xml,Form *form) {
  Component::loadprops(xml,form);
  Point pos=getpos();Size size=getsize();
  if (xml.isvar("x")) pos.x=xml.ivar("x");
  if (xml.isvar("y")) pos.y=xml.ivar("y");
  if (xml.isvar("w")) size.w=xml.ivar("w");
  if (xml.isvar("h")) size.h=xml.ivar("h");
  if (xml.ivar("focused")) setfocus();
  if (xml.isvar("visible")) setvisible(xml.ivar("visible"));
  settag(xml.svar("tag"));
  sethint(xml.svar("hint"));
  setpos(pos,size);
  setname(xml.svar("name"));
  ev_mouseup.load(xml,"onmouseup");
  ev_mousedown.load(xml,"onmousedown");
  ev_mousemove.load(xml,"onmousemove");
  ev_mouseenter.load(xml,"onmouseenter");
  ev_mouseleave.load(xml,"onmouseleave");
  ev_keyup.load(xml,"onkeyup");
  ev_keydown.load(xml,"onkeydown");
  ev_focus.load(xml,"onfocus");
  ev_killfocus.load(xml,"onkillfocus");
  ev_resize.load(xml,"onresize");
  ev_mousewheel.load(xml,"onmousewheel");
  ev_char.load(xml,"onchar");
}

void Control::setsdp(string name,string value) {getreal()->setsdp(name,value);}
string Control::getsdp(string name) {return getreal()->getsdp(name);}
bool Control::supportsdp(string name) {return getreal()->supportsdp(name);}
Rect Control::getclient() {return getreal()->getclient();}

void Control::setfocus() {getreal()->setfocus();}
void Edit::selectall() {getreal()->selectall();}

string Button::gettext() {return getreal()->gettext();}
void Button::settext(string value) {return getreal()->settext(lang_text_ch(value));}
//property_d(Button,text,string)

string Label::gettext() {return getreal()->gettext();}
void Label::settext(string value) {return getreal()->settext(lang_text_ch(value));}
//property_d(Label,text,string)

string PageSheet::gettext() {return getreal()->gettext();}
void PageSheet::settext(string value) {return getreal()->settext(lang_text_ch(value));}
//property_d(PageSheet,text,string)

string RadioGroup::gettext() {return getreal()->gettext();}
void RadioGroup::settext(string value) {return getreal()->settext(lang_text_ch(value));}
//property_d(RadioGroup,text,string)

string CheckBox::gettext() {return getreal()->gettext();}
void CheckBox::settext(string value) {return getreal()->settext(lang_text_ch(value));}
//property_d(CheckBox,text,string)

string GroupBox::gettext() {return getreal()->gettext();}
void GroupBox::settext(string value) {return getreal()->settext(lang_text_ch(value));}
//property_d(GroupBox,text,string)

string WrapLabel::gettext() {return getreal()->gettext();}
void WrapLabel::settext(string value) {return getreal()->settext(lang_text_ch(value));}
//property_d(WrapLabel,text,string)

string Control::gethint() {return getreal()->gethint();}
void Control::sethint(string value) {return getreal()->sethint(lang_text_ch(value));}
//property_d(Control,hint,string)

property_d(RadioGroup,items,string)
property_d(CheckBox,checked,bool)
property_d(PackedList,dropdowncount,int)
property_d(ListControl,cur,int)
property_d(ListControl,items,string)
property_d(Edit,text,string)
property_d(Memo,text,string)
property_d(ScrollBox,scrollpos,Point)
property_d(RadioGroup,cur,int)
property_d(Memo,cur,int)
property_d(Memo,modified,bool)
property_d(Edit,modified,bool)
property_d(PageControl,cur,int)
property_d(TrackBar,min,int)
property_d(TrackBar,max,int)
property_d(TrackBar,cur,int)
property_d(TrackBar,frequency,int)
property_d(TrackBar,vertical,bool)
property_d(Control,enabled,bool)
property_d(Control,visible,bool)
property_d(Control,cursor,Cursor)
property_d(ListBox,multiselect,bool)
property_d(ListBox,selected,string)


object_d(Label,ChildControl)
object_d(CheckBox,ChildControl)
object_d(GroupBox,ParentControl)
object_d(RadioGroup,ChildControl)
object_d(Edit,ChildControl)
object_d(ListBox,ListControl)
object_d(PackedList,ListControl)
object_d(Button,ChildControl)
object_d(Place,ParentControl)
object_d(PageControl,ChildControl)
object_d(ScrollBox,ParentControl)
object_d(Memo,ChildControl)
object_d(WrapLabel,ChildControl)
object_d(TrackBar,ChildControl)

PageSheet::PageSheet(PageControl *owner):ParentControl(owner,owner->system) {
  setreal(system->createPageSheet(owner,this));
}

bool ScrollBox::scrollinview(const Rect &r) {return getreal()->scrollinview(r);}

void ListBox::loadprops(XMLNode xml,Form *form) {
  ListControl::loadprops(xml,form);
  setmultiselect(xml.ivar("multiselect"));
}

void ParentControl::clearchilds() {
  for(;;) {
    TCntVec<Control*> childs;
    getchilds(childs);
    if (childs.count==0) return;
    delete childs[0];
  }
}

void ParentControl::loadprops(XMLNode xml,Form *form) {
  Control::loadprops(xml,form);
  Layout *l2=system->createlayout(xml.svar("layout"),this);
  if (l2) {delete layout;layout=l2;}
  if (layout) layout->loadprops(xml);
  if (!denyloadchilds) if (!layout || !layout->isauto()) form->loadchilds(xml,this);
  relayout();
}

void ParentControl::autolayout() {
  TCntVec<Control*> childs;
  getchilds(childs);
  for(int i=0;i<childs.count;i++) childs[i]->autolayout();

  const int groupdy=5;

  Margins m;getmargins(m);
  int acty=m.top;
  int w=0,h=0;
  for(int i=0;i<childs.count;i++) {
    childs[i]->setpos(m.left,acty);
    Size size=childs[i]->getsize();
    int dy=size.h;
    dy+=groupdy;
    h+=dy;acty+=dy;
    if (size.w>w) w=size.w;
  }
  w+=m.left+m.right;h+=m.top+m.bottom;

  setsize(w,h);
}

void ParentControl::loaddata(XMLNode xml) {
  Control::loaddata(xml);
  XMLNode sub=xml.getat(getroot(xml));
  TCntVec<Control*> childs;
  getchilds(childs);
  for(int i=0;i<childs.count;i++) childs[i]->loaddata(sub);
//  jsystem->showinfo(typeid(*this).name());
}
void ParentControl::savedata(XMLNode xml) {
  Control::savedata(xml);
  XMLNode sub=xml.getat(getroot(xml));
  TCntVec<Control*> childs;
  getchilds(childs);
  for(int i=0;i<childs.count;i++) childs[i]->savedata(sub);
}

void PageControl::loadprops(XMLNode xml,Form *form) {
  ChildControl::loadprops(xml,form);
  ev_change.load(xml,"onchange");
  for(int i=0;i<xml.count();i++) {
    if (xml[i].getname()=="page") {
      PageSheet *page=new PageSheet(this);
      form->loadcontrol(page,xml[i],NULL);
    }
  }
}
void PageControl::loaddata(XMLNode xml) {
  ChildControl::loaddata(xml);
  XMLNode sub=xml.getat(getroot(xml));
  TCntVec<PageSheet*> pages;
  getpages(pages);
  for(int i=0;i<pages.count;i++) pages[i]->loaddata(sub);
}
void PageControl::savedata(XMLNode xml) {
  ChildControl::savedata(xml);
  XMLNode sub=xml.getat(getroot(xml));
  TCntVec<PageSheet*> pages;
  getpages(pages);
  for(int i=0;i<pages.count;i++) pages[i]->savedata(sub);
}
void PageControl::getpages(TCntVec<PageSheet*> &pages) {
  getreal()->getpages(pages);
}

void PageSheet::loadprops(XMLNode xml,Form *form) {
  ParentControl::loadprops(xml,form);
  settext(xml.svar("text"));
}

void ParentControl::loadlayoutchild(XMLNode xml,Control *child) {
  if (layout) layout->loadchild(xml,child);
}

ParentControl::~ParentControl() {
  delete layout;
}

Control::~Control() {
  delete realcontrol;
}
void Control::beginupdate() {getreal()->beginupdate();}
bool Control::wantanswer() {return getreal()->wantanswer();}
void Control::endupdate() {getreal()->endupdate();}

void WrapLabel::loadprops(XMLNode xml,Form *form) {
  ChildControl::loadprops(xml,form);
  settext(xml.svar("text"));
//  xmlname=xml.svar("xmlname");
  if (xml.validvar("model")) {
    model=xml.svar("model");
    setmodel(dynamic_cast<LabelModel*>(form->searchmodel(model)));
  }
}

void Memo::loadprops(XMLNode xml,Form *form) {
  ChildControl::loadprops(xml,form);
  settext(xml.svar("text"));
  ev_change.load(xml,"onchange");
//  xmlname=xml.svar("xmlname");
}

void ParentControl::getchilds(TCntVec<Control*> &childs) {
  getreal()->getchilds(childs);
}

void Control::onsize(int w,int h) {
  donotify("onresize",ev_resize.get());
}

void ParentControl::onsize(int w,int h) {
  Control::onsize(w,h);
  if (wantanswer()) relayout();
}

void ParentControl::relayout() {
  if (layout) layout->work();
}

void ParentControl::setlayout(Layout *alayout) {
  delete layout;
  layout=alayout;
  relayout();
  if (wantanswer()) relayout();
}

void Control::notifyremove(Component *component) {
  Component::notifyremove(component);
  realcontrol->notifyremove(component);
}

void Label::changed_model() {
  if (LabelModel *m=getmodel()) settext(m->label_gettext(model));
}
void WrapLabel::changed_model() {
  if (LabelModel *m=getmodel()) settext(m->label_gettext(model));
}


void ListControl::updatemodelitems() {
  string items;
  ListModel *model=getmodel();
  if (model) {
    int cnt=model->lm_count();
    for(int i=0;i<cnt;i++) items<<model->lm_item(i)<<"\n";
    setitems(items);
  } else setitems("");
  updatemodelcur();
}

Control::Control(Component *owner,System *system):Component(owner,system) {
  realcontrol=NULL;
}
void Control::show() {getreal()->show();}
void Control::hide() {getreal()->hide();}

void Control::setpos(Point pos) {
  setpos(pos,getsize());
//  realcontrol->setpos(pos);
}
void Control::setsize(Size size) {
  setpos(getpos(),size);
//  realcontrol->setsize(size);
}
void Control::setpos(Point pos,Size size) {
  realcontrol->setpos(pos,size);
}
Point Control::getpos() {
  return realcontrol->getpos();
}
Point Control::getscreenpos() {
  return realcontrol->getscreenpos();
}
Size Control::getsize() {
  return realcontrol->getsize();
}
void Control::setparent(ParentControl *parent) {
  realcontrol->setparent(parent);
}
ParentControl *Control::getparent() {
  return getreal()->getparent();
}
Font *Control::getfont() {
  return getreal()->getfont();
}
void Control::setfont(Font *font) {
  getreal()->setfont(font);
}
void Control::getmargins(Margins &m) {
  getreal()->getmargins(m);
}
Size Control::getbestsize() {
  return getreal()->getbestsize();
}

void TrackBar::loadprops(XMLNode xml,Form *form) {
  ChildControl::loadprops(xml,form);
  setmin(xml.ivar("min"));
  setmax(xml.ivar("max"));
  setfrequency(xml.ivar("frequency"));
  setvertical(xml.ivar("vertical"));
  if (xml.isvar("cur")) setcur(xml.ivar("cur"));
  ev_change.load(xml,"onchange");
}


void Edit::loadprops(XMLNode xml,Form *form) {
  ChildControl::loadprops(xml,form);
  settext(xml.svar("text"));
  ev_change.load(xml,"onchange");
//  xmlname=xml.svar("xmlname");
}
void Edit::autolayout() {
  setsize(100,0);
}

void Button::loadprops(XMLNode xml,Form *form) {
  ChildControl::loadprops(xml,form);
  settext(xml.svar("text"));
  ev_click.load(xml,"onclick");
}
void Button::autolayout() {
  setsize(100,40);
}


void ListControl::loadprops(XMLNode xml,Form *form) {
  ChildControl::loadprops(xml,form);
  ev_change.load(xml,"onchange");
//  xmlname=xml.svar("xmlname");
  saveas=xml.svar("saveas");
  loaditems=xml.svar("load.items");
  string its=xml.svar("items");
  its.replaceall(";","\n");
  setitems(lang_text_ch(its));
  if (xml.isvar("cur")) setcur(xml.ivar("cur"));
}

void ListControl::loaddata(XMLNode xml) {
  ChildControl::loaddata(xml);
  if (isroot(xml)) {
    if (loaditems!="" && xml.exists(XMLPath(loaditems))) setitems(xml.getvalue(XMLPath(loaditems)));

    if (saveas=="index") setcur(xml.getvalue(getroot(xml)).ivalue());
    if (saveas=="value") {
      string its=getitems();
      string value=xml.getvalue(getroot(xml));
      int pos=its.searchinlines(value.c_str(),'\n');
      setcur(pos);
    }
  }
}
void ListControl::savedata(XMLNode xml) {
  ChildControl::savedata(xml);
  if (isroot(xml)) {
    if (saveas=="index") xml.setvalue(getroot(xml),itos(getcur()));
    if (saveas=="value") {
      string its=getitems();
      int cur=getcur();
      string value=its.stringat(cur,'\n');
      xml.setvalue(getroot(xml),value);
    }
  }
}

void Label::loadprops(XMLNode xml,Form *form) {
  ChildControl::loadprops(xml,form);
  settext(xml.svar("text"));
//  xmlname=xml.svar("xmlname");
  if (xml.validvar("model")) {
    model=xml.svar("model");
    setmodel(dynamic_cast<LabelModel*>(form->searchmodel(model)));
  }
}
void RadioGroup::loadprops(XMLNode xml,Form *form) {
  ChildControl::loadprops(xml,form);
  settext(xml.svar("text"));
  string its=xml.svar("items");
  its.replaceall(";","\n");
  setitems(its);
  ev_change.load(xml,"onchange");
//  xmlname=xml.svar("xmlname");
  if (xml.isvar("cur")) setcur(xml.ivar("cur"));
}
void CheckBox::loadprops(XMLNode xml,Form *form) {
  ChildControl::loadprops(xml,form);
  settext(xml.svar("text"));
  ev_click.load(xml,"onclick");
  if (xml.svar("checked")!="") setchecked(xml.ivar("checked"));
//  xmlname=xml.svar("xmlname");
//  savetype=loadsavetype(xml,st_child);
}
void CheckBox::autolayout() {
}

void GroupBox::loadprops(XMLNode xml,Form *form) {
  ParentControl::loadprops(xml,form);
  settext(xml.svar("text"));
}

/*
void ListControl::getprop(STRING name,esObject &res) {
  if (name=="cur") res.setnumber(getcur());
}
*/


}

