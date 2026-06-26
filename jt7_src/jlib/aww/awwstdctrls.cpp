#include <winlib.h>
#pragma hdrstop

#include "awwstdctrls.h"
#include "awwsystem.h"
#include <win/w32keys.h>
#include "awwpaint.h"

namespace aww {

string makewintext(string text) {
  text=makectext(text.c_str());
  int cnt=0;
  for(const char *c=text.c_str();*c;c++) if (*c=='\n') cnt++;
  string res;
  char *w=res.reservebuffer(text.length()+cnt);
  for(const char *c=text.c_str();*c;c++) {
    if (*c=='\n') *w++=13;
    *w++=*c;
  }
  *w=0;
  res.updatelen();
  return res;
}

void awwMemo::onkeyup(awwMessage &msg,awsKey key) {
  awwControl::onkeyup(msg,key);
  memo->dochange();
}

void awwMemo::settext(string atext) {
  atext=makewintext(atext);
  text=atext;
  updatetext();
}

void awwTextControl::setchecked(bool achecked) {
  checked=achecked;
  if (win) WCheckBox(win).setchecked(checked);
}

void awwTextControl::settext(string atext) {
  text=atext;
  if (win) {
    repairsize();
    win.settext(text.c_str());
    updatepos();
  }
}

void awwTextControl::setfont(Font *afont) {
  awwControl::setfont(afont);
  repairsize();
  updatepos();
}
void awwTextControl::createwindow() {
  repairsize();
  awwControl::createwindow();
  win.settext(text.c_str());
  setchecked(checked);
}
void awwTextControl::ondestroywnd() {
  awwControl::ondestroywnd();
  updatemychecked();updatemytext();
}

void awwTextControl::setpos(Point pos,Size size) {
  awwControl::setpos(pos,size);
  repairsize();
  updatepos();
}
void awwTextControl::repairsize() {
  TEXTMETRIC metric;
  HDC dc=GetDC(0);
  HFONT oldfont=SelectObject(dc,font);
  GetTextMetrics(dc, &metric);
  int newh=metric.tmHeight;
  SIZE s;
  GetTextExtentPoint(dc,text.c_str(),text.length(),&s);
  int neww=s.cx;
  SelectObject(dc,oldfont);
  ReleaseDC(0,dc);
  _repairsize(neww,newh);
}

void awwTextControl::updatemypos() {
  if (win) awwControl::updatemypos();else repairsize();
}

void awwGroupBox::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  strcpy(pars.classname,"BUTTON");
  pars.style|=WS_CHILD|BS_GROUPBOX;
}

awwLabel::awwLabel(Label *label):awwTextControl(label) {}
void awwLabel::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  strcpy(pars.classname,"STATIC");
  pars.style|=WS_CHILD;
}

void awwCheckBox::onmouseup(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift) {
  awwTextControl::onmouseup(msg,x,y,button,shift);
  setchecked(!getchecked());
  doclick();
}

awwCheckBox::awwCheckBox(CheckBox *label):awwTextControl(label) {}
void awwCheckBox::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  strcpy(pars.classname,"BUTTON");
//  pars.style|=WS_CHILD|BS_AUTOCHECKBOX;
  pars.style|=WS_CHILD|BS_CHECKBOX;
}
void awwEdit::repairsize() {
  h=counteditheight(font->gethandle());
}
void awwEdit::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  strcpy(pars.classname,"EDIT");
  pars.style|=WS_CHILD|ES_AUTOHSCROLL;
  pars.exstyle|=WS_EX_CLIENTEDGE;
}
awwEdit::awwEdit(Edit *edit):awwTextControl(edit) {
  this->edit=edit;
}
void awwButton::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  strcpy(pars.classname,"BUTTON");
  pars.style|=WS_CHILD;
//  pars.exstyle|=WS_EX_CLIENTEDGE;
}
void awwButton::createwindow() {
  awwControl::createwindow();
  win.settext(text.c_str());
}
void awwButton::onmouseup(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift) {
  awwControl::onmouseup(msg,x,y,button,shift);
  BaseEvent ev;
  if (button==mb_left and x>=0 and y>=0 and x<w and y<h) this->button->doclick();
}

void awwEdit::onkeyup(awwMessage &msg,awsKey key) {
  awwTextControl::onkeyup(msg,key);
  edit->dochange();
}

void awwMemo::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  strcpy(pars.classname,"EDIT");
  pars.style|=WS_CHILD|ES_AUTOHSCROLL|ES_AUTOVSCROLL|WS_HSCROLL|WS_VSCROLL|ES_MULTILINE;
  pars.exstyle|=WS_EX_CLIENTEDGE;
}

void awwWrapLabel::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  strcpy(pars.classname,"STATIC");
  pars.style|=WS_CHILD;
}

template<class T> static void update_items(string items,T l) {
  if (!l) return;
  l.clear();
  for(lineiter s=items.getlines('\n');s.aredata();s++) l.add(*s);
}
void awwListBox::updateitems() {
  update_items(items,WListBox(win));
//  awsEvent event;
//  ((ListControl*)ctrl)->onchange(event);
}
void awwPackedList::updateitems() {
  update_items(items,WPackList(win));
}
void awwListControl::createwindow() {
  awwControl::createwindow();
  updateitems();updatecur();
}
void awwListBox::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  strcpy(pars.classname,"LISTBOX");
  pars.style|=WS_CHILD|LBS_NOTIFY;
  if (multicol) pars.style|=WS_HSCROLL;else pars.style|=WS_VSCROLL;
  if (multiselect) pars.style|=LBS_MULTIPLESEL|LBS_EXTENDEDSEL;
  if (multicol) pars.style|=LBS_MULTICOLUMN;
  pars.exstyle|=WS_EX_CLIENTEDGE;
}
string awwListBox::getselected() {
  updatemysels();
  MEMSTROUT res;
  for(int i=0;i<selitems.count;i++) res<<itos(selitems[i])<<";";
  return res.str();
}
void awwListBox::setselected(string value) {
  selitems.clear();
  for(lineiter it=value.getlines(';');*it;it++) selitems.add(atoi(*it));
  updatesels();
}
void awwListBox::setsdp(string name,string value) {
  if (name=="multicol") multicol=value.ivalue();
  else if (name=="colwi") colwi=value.ivalue();
  else awwListControl::setsdp(name,value);
}
string awwListBox::getsdp(string name) {
  if (name=="multicol") return itos(multicol);
  else if (name=="colwi") return itos(colwi);
  return awwListControl::getsdp(name);
}
bool awwListBox::supportsdp(string name) {
  if (name=="multicol") return true;
  else if (name=="colwi") return true;
  return awwListControl::supportsdp(name);
}


void awwListBox::createwindow() {
  awwListControl::createwindow();
  updatesels();
  if (colwi>0) SendMessage(win,LB_SETCOLUMNWIDTH,colwi,0);
}
void awwListBox::updatemysels() {
  if (!win) return;
  int cnt=SendMessage(win,LB_GETSELCOUNT,0,0);
  selitems.wantcapa(cnt);
  selitems.count=SendMessage(win,LB_GETSELITEMS,cnt,(LPARAM)selitems.buf);
}
void awwListBox::updatesels() {
  if (!win) return;
  int cnt=SendMessage(win,LB_GETCOUNT,0,0);
  for(int i=0;i<cnt;i++) SendMessage(win,LB_SETSEL,selitems.search(i)>=0,i);
}

void awwPackedList::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  strcpy(pars.classname,"COMBOBOX");
  pars.style|=WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST;
  pars.exstyle|=WS_EX_CLIENTEDGE;
}
void awwPackedList::redropdown() {
  if (!win) return;
  WPackList l(win);
//  h=l.getlistitemheight()*dropdowncount+l.geteditheight();
//  h=l.getlistitemheight()*dropdowncount+counteditheight(font->handle);
  dropdownh=font->getheight()*dropdowncount+counteditheight(font->gethandle())+2;
  updatepos();
}
void awwPackedList::updatemypos() {
  awwControl::updatemypos();
  h=counteditheight(font->gethandle());
}
void awwListControl::oncommand(awwMessage &msg,int id,int notifycode) {
  awwControl::oncommand(msg,id,notifycode);
  if (notifycode==changecode) {
//    int old=current;
    updatemycur();
//    if (old!=current)
    dochange();
  }
}


/*
void awwListControl::onmousedown(awwMessage &msg,int x,int y,int button) {
  awwControl::onmousedown(msg,x,y,button);
  if (button==0) {
    defproc(msg);
    int old=current;
    updatemycur();
    if (old!=current) ((ListControl*)ctrl)->onchange(msg);
  }
}
*/
void awwPlace::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  pars.style|=WS_CHILD;
  strcpy(pars.classname,"awwWindow");
}

void awwGroupBox::getmargins(Margins &m) {
  m.left=m.right=m.bottom=5;
  m.top=2+font->getheight();
}

Rect awwGroupBox::getclient() {
  RECT r;
  GetClientRect(win,&r);
  r.top+=font->getheight();
  InflateRect(&r,-2,-2);
  return Rect(Point(r.left,r.top),Point(r.right,r.bottom));
}


}


