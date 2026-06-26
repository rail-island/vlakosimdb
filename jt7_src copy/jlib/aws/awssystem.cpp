#include <jlib.h>
#pragma hdrstop

#include "awssystem.h"
#include "awsform.h"
#include "awstable.h"
#include "awspaint.h"
#include "awshtmlview.h"
#include "awschecklist.h"

namespace aws {

SystemOpts::SystemOpts() {
  canwindow=false;
  canedit=false;
  canlistbox=false;
  canpackedlist=false;
  canbutton=false;
  canplace=false;
  canpager=false;
  canlabel=false;
  cancheckbox=false;
  canradiogroup=false;
  cangroupbox=false;
  canscrollbox=false;
  canmemo=false;
  canwraplabel=false;
  canpaintcontrol=false;
  canscrolledpainter=false;
  cantrackbar=false;
  cantimer=false;
  canmenuitem=false;
  cansysmenu=false;
  checklistindsize=32;
}

void System::addcreator(Creator *creator) {
  creators.add(creator);
}

Control *System::createcontrol(string name,Component *owner) {
  for(int i=0;i<creators.count;i++) {
    Control *res=creators[i]->createcontrol(name,owner,this);
    if (res) return res;
  }
  return NULL;
}
Layout *System::createlayout(string name,ParentControl *owner) {
  for(int i=0;i<creators.count;i++) {
    Layout *res=creators[i]->createlayout(name,owner);
    if (res) return res;
  }
  return NULL;
}

void System::on_defaulthandler(BaseEvent &event) {
  do_dispatch(event);
}

/*
void System::setcreator(Creator *acreator) {
  delete creator;
  creator=acreator;
}
*/

void System::setoptionfile(string file) {
  optionfile=file;
  options.loadfromfile(file);
  string langfile=options.svar("lang");
  for(int i=0;i<argc;i++) {
    string a=string(argv[i]);
    if (a.begins("-language:")) langfile=a.copy(10,0x100);
  }
  loadlang(basedir/"data"/"lang"/langfile);
  options.setvar("langid",langman->langid);
}

System::~System() {
  if (optionfile!="") options.savetofile(optionfile);
//  delete creator;
  delete rep;
  delete handleman;
}

XMLNode System::findrep(string name) {
  if (XMLDataNode *res=rep->search(name)) return res;
  throw awsXMLNotFound(name);
}

System::System():Component(NULL,NULL) {
//  creator=new StdCreator();
  argc=0;argv=NULL;
  sysopt=&systemopts;
  init_aws_rtti();
  init_aws_table_rtti();
  init_aws_paint_rtti();
  init_aws_html_rtti();
  init_aws_checklist_rtti();
  addcreator(new StdCreator());
  rep=new XMLRepositary();
  options=XMLNode::create();
  handleman=new HandleManager();
}

void System::ondesktopmousedown(int x,int y,MouseButton button,ShiftState shift) {
  MouseEvent ev;
  ev.type=et_mousedown;ev.name="ondesktopmousedown";ev.value=ev_desktopmousedown.get();
  ev.x=x;ev.y=y;ev.button=button;ev.shift=shift;
  do_event(ev);
}
void System::ondesktopmouseup(int x,int y,MouseButton button,ShiftState shift) {
  MouseEvent ev;
  ev.type=et_mouseup;ev.name="ondesktopmouseup";ev.value=ev_desktopmouseup.get();
  ev.x=x;ev.y=y;ev.button=button;ev.shift=shift;
  do_event(ev);
}
void System::ondesktopmousemove(int x,int y,ShiftState shift) {
  MouseMoveEvent ev;
  ev.type=et_mousemove;ev.name="ondesktopmousemove";ev.value=ev_desktopmousemove.get();
  ev.x=x;ev.y=y;ev.shift=shift;
  do_event(ev);
}
void System::loadlang(string file) {
  load_lang_file(file);
}


}

