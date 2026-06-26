#include <jlib.h>
#pragma hdrstop

#include "awsstddlgs.h"
#include "awsform.h"
#include "awswindow.h"
#include "awssystem.h"
#include "awsextctrls.h"
#include "awslayout.h"
//#include "awsxmldialog.h"
//#include <ext/stdxmlnode.h>

namespace aws {


struct InputHandler:EventHandler {
  Window *win;Edit *ed;
  bool ok;
  void on_do(BaseEvent &ev) {
    ok=atoi(ev.getvalue().c_str());
    win->close();
  }
  void on_keydown(KeyEvent &key) {
    if (key.key==kt_enter) ok=true,win->close();
    if (key.key==kt_esc) ok=false,win->close();
  }
  void on_notify(NotifyEvent &ev) {
    if (ev(win,"onshow")) ed->selectall();
  }
  InputHandler() {ok=false;}
};

bool inputquery(System *system,string title,string msg,string &res) {
/*  XMLRef data=XMLNode::std().getnode();
//  MainXMLNode data;
  data.setsub("msg",msg);data.setsub("edit",res);
  bool b=runxmldialog(system,system->findrep("form.inputbox"),data);
  if (b) res=data.getsub("edit");
  return b;
  */
  XMLNode data=XMLNode::create();
/*  XMLNode xml =XMLNode::std().getnode();
  MEMSTROUT mso;
  mso<<"<?xml?><window layout='auto' text='"<<title<<"'><labedit root='@value' text='"
  <<msg<<"'/>"
  <<"<button onclick='@ok' text='OK'/>"<<"<button onclick='@cancel' text='Storno'/>"
  <<"</window>";
  xml.loadfromtext(mso.str());*/
  data.setvar("value",res);
  data.setvar("msg",msg);
//  bool bres=runxmldialog(system,xml,data);
  bool bres=runxmldialog(system,system->findrep("form.inputbox"),data);
  if (bres) res=data.svar("value");
//  Form *form=new Form(system,system);
/*  Window *win=form->loadwindow(xml);
  InputHandler ih;
  ih.win=win;
  win->addhandler(&ih);
  win->showmodal();
  delete form;
  return false;*/
  return bres;
}


struct MsgHandler:EventHandler {
  string res;Window *win;
  void on_do(BaseEvent &ev) {
    res=ev.getvalue();
    win->close();
  }
  MsgHandler() {res="-1";}
};

int msgbox(System *system,string title,string msg,string buttons) {
  MsgHandler eh;

  Window *win=new Window(system,system);
  eh.win=win;
//  Font *f=system->getsystemfont(true);
//  win->setsize(Size(5*f->getheight());
  win->setsize(300,180);
  win->settext(title);
  win->setposition(wp_centered);
  Label *lab=new Label(win,system);
  lab->setparent(win);
  lab->settext(msg);
  lab->setpos(Point(20,20));
  win->show();
  int cnt=buttons.itemcount(';');
  int butw=300/cnt;
  int act=0;
  for(lineiter li=buttons.getlines(';');*li;li++,act++) {
    Button *but=new Button(win,system);
    but->settext(*li);
    but->setparent(win);
    but->setsize(Size(butw*3/4,30));
    but->centerto(Rect(butw*act,110,butw,30));
    but->addhandler(&eh);
    but->ev_click.setvalue(itos(act));
  }
  win->showmodal();
  delete win;
  return atoi(eh.res.c_str());
}

struct XMLEvents:EventHandler {
  Window *win;bool res;
  XMLNode data;
  void on_do(BaseEvent &event) {
    string val=event.getvalue();
    if (val.begins("@")) {
      val.erase(0,1);
      if (val=="ok") {
        win->close();res=true;
        win->savedata(data);
      }
      if (val=="cancel") {
        win->close();res=false;
      }
      if (val=="apply") {
        win->savedata(data);
      }
      if (val=="reload") {
        win->loaddata(data);
      }
    }
  }
  void on_keydown(KeyEvent &key) {
    if (key.key==kt_enter) win->savedata(data),res=true,win->close();
    if (key.key==kt_esc) res=false,win->close();
  }
  XMLEvents(Window *win,XMLNode data,Form *form):win(win),data(data) {res=false;}
};

bool runxmldialog(System *system,XMLNode dialog,XMLNode data) {
  Form *form=new Form(system,system);
  Window *win=form->loadwindow(dialog);
  win->loaddata(data);
  XMLEvents events(win,data,form);
  form->addhandler(&events);
  win->showmodal();
  delete form;
  return events.res;
}

bool settingsdialog(System *system,XMLNode settings,XMLNode data) {
  Form *form=new Form(system,system);
  Settings *sc=dynamic_cast<Settings*>(form->load(settings));
  form->addinline("settings",sc);
  Window *win=form->loadwindow(system->findrep("form.settings"));
  win->loaddata(data);
  XMLEvents events(win,data,form);
  form->addhandler(&events);
  win->showmodal();
  delete form;
  return events.res;
}

WaitControl::WaitControl(WaitWindow *owner,System *system):PaintControl(owner,system) {
  win=owner;
}

void WaitControl::onpaint(Canvas &cnv,awsEvent &event) {
  PaintControl::onpaint(cnv,event);
  Rect r=getclient();
  cnv.fg(win->framecolor).bg(win->bgcolor).penwi(win->framewi).rect(r);
  Rect a=r,b=r;
  a.b.y=b.a.y=2*r.b.y/3;
  Font *f=system->createfont();
  f->setsize(20);
  f->setcolor(win->appcolor);
  cnv.font(f);
  cnv.centertext(a,win->appname,true,true);
  f->setsize(12);
  f->setcolor(win->msgcolor);
  cnv.font(f);
  cnv.centertext(b,win->msg,true,true);
  delete f;
}

WaitWindow::WaitWindow(Component *owner,System *system):Window(owner,system) {
  setstyle("simple");
  setposition(wp_centered);
  setlayout(new Layout1(this));
  setsize(300,100);
  ctrl=new WaitControl(this,system);
  ctrl->setparent(this);
  bgcolor=clnaqua;framecolor=clnnavy;framewi=3;
  appcolor=clngreen;msgcolor=clnblack;
  appname=system->gettitle();
}

void WaitWindow::setmsg(string value) {
  msg=lang_text_ch(value);
  ctrl->repaint();
  system->answer();
}

void WaitWindow::show() {
  Window::show();
}



}

