#include <jlib.h>
#pragma hdrstop

#include "awsextctrls.h"
#include "awslayout.h"
#include "awssystem.h"
#include "awsform.h"
#include "awstree.h"
#include "awsprint.h"

namespace aws {

ConsoleImpl::~ConsoleImpl() {
}

void ConsoleImpl::print(string what) {
  memo->settext(memo->gettext()+what);
}

void ConsoleImpl::wait() {
  waiting=true;
  while (waiting && ! system->isterminated() ) system->answer();
}

string ConsoleImpl::readln() {
  memo->setfocus();
  memo->setcur(memo->gettext().length());
  reading=true;
  while (reading && ! system->isterminated() ) system->answer();
  string text=memo->gettext();
  int cnt=text.itemcount('\n');
  return text.stringat(cnt-1,'\n');
}

void ConsoleImpl::on_keydown(KeyEvent &ev) {
  if (waiting) {
    waiting=false;
    ev.event->done();
    return;
  }
  if (ev.key==kt_enter) reading=false;
  if (ev.key==kt_up) {
    ev.event->done();
  }
}

ConsoleImpl::ConsoleImpl(Component *owner,System *system):Window(owner,system) {
  setlayout(new Layout1(this));
  waiting=reading=false;
  memo=new Memo(this,system);
  memo->setparent(this);
  memo->setfont(system->getsystemfont(false));
  memo->addhandler(this);
  show();
}

void Panel::onpaint(Canvas &cnv,awsEvent &event) {
  Rect r=getclient();
  cnv.drawframe(r,false);
  PaintControl::onpaint(cnv,event);
}

void SpeedButton::onmouseup(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  PaintControl::onmouseup(x,y,button,shift,event);
  if (pushed) doclick();
  pushed=false;mdown=false;
  repaint();
}
void SpeedButton::onmouseleave(awsEvent &event) {
  PaintControl::onmouseleave(event);mousein=false;
  if (pushed) pushed=false,repaint();
  else if (flat) repaint();
}
void SpeedButton::onmouseenter(awsEvent &event) {
  PaintControl::onmouseenter(event);mousein=true;
  if (mdown) pushed=true,repaint();
  else if (flat) repaint();
}
void SpeedButton::onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  PaintControl::onmousedown(x,y,button,shift,event);
  pushed=true;mdown=true;
  repaint();
}
void SpeedButton::universal_paint(Canvas cnv,int state) {
  Rect r=getclient();
  r-=Point(state,state);
  if (state) cnv.drawframe(r,state==-1);
  if (color!=clnnone) {
    cnv.bg(color).fg(clnnone);
    Rect cr=r;
    cr.inflate(-2,-2);
    cnv.rect(cr);
  }
  if (image) {
    int w=image->getw(),h=image->geth();
    image->blit(Canvas(cnv,(r.a.x+r.b.x)/2-w/2,(r.a.y+r.b.y)/2-h/2));
  } else {
    cnv.font(getfont());
    cnv.centertext(r,text);
  }
}
void SpeedButton::onpaint(Canvas &cnv,awsEvent &event) {
  if (group) {
    universal_paint(cnv,group->ischoosed(this)?-1:1);
  } else if (isswitch) {
    universal_paint(cnv,switched?-1:1);
  } else {
    if (pushed) universal_paint(cnv,-1);
    else if (flat && mousein || !flat) universal_paint(cnv,1);
    else universal_paint(cnv,0);
  }
  PaintControl::onpaint(cnv,event);
}

SpeedButton::SpeedButton(Component *owner,System *system):PaintControl(owner,system) {
  group=NULL;isswitch=false;
  mousein=mdown=pushed=flat=switched=false;setsize(20,20);
  color=clnnone;
}

void SpeedButton::notifyremove(Component *comp) {
  PaintControl::notifyremove(comp);
  if (comp==group) group=NULL;
}

void SpeedButton::setgroup(SpeedGroup *value) {
  group=value;
}

void SpeedButton::doclick() {
  if (group) group->choose(this);
  if (isswitch) switched=!switched;
  donotify("onclick",ev_click.get());
}

void SpeedButton::loadprops(XMLNode xml,Form *form) {
  PaintControl::loadprops(xml,form);
  settext(xml.svar("text"));
  setflat(xml.ivar("flat"));
  setswitch(xml.ivar("switch"));
  color=gr::strtonativecolor(xml.svar("color").c_str());
  if (xml.validvar("image")) {
    string imgf=xml.svar("image");
    for(int i=0;i<system->searchpath.count;i++) {
      string file=sFile(system->searchpath[i])/imgf;
      if (FILE *fr=fopen(file.c_str(),"rb")) {
        fclose(fr);
        image=system->createpicture();
        image->load(file);
      }
    }
  }
  if (xml.isvar("switched")) setswitched(xml.ivar("switched"));
  ev_click.load(xml,"onclick");
}

void SpeedGroup::loadprops(XMLNode xml,Form *form) {
  NullControl::loadprops(xml,form);
  for(int i=0;i<xml.count();i++) {
    if (Control *ctrl=form->loadcontrol(xml[i],getparent())) {
      if (SpeedButton *sb=dynamic_cast<SpeedButton*>(ctrl)) {
        sb->setgroup(this);
        items.add(sb);
        if (sb->gethint()=="") {
          sb->sethint(xml.svar("hint"));
        }
      }
    }
  }
  if (xml.svar("cur")!="") setcur(xml.ivar("cur"));
  ev_change.load(xml,"onchange");
}

void SpeedGroup::notifyremove(Component *comp) {
  NullControl::notifyremove(comp);
  items.quickremove(dynamic_cast<SpeedButton*>(comp));
  if (comp==choosen) choosen=NULL;
}

void SpeedGroup::choose(SpeedButton *but) {
  if (choosen) choosen->repaint();
  choosen=but;
  if (choosen) choosen->repaint();
  donotify("onchange",ev_change.get());
}

bool SpeedGroup::ischoosed(SpeedButton *but) {
  return but==choosen;
}

int SpeedGroup::getcur() {
  return items.search(choosen);
}
void SpeedGroup::setcur(int index) {
  if (index>=0 && index<items.count) choose(items[index]);
  else choose(NULL);
}

SpeedGroup::SpeedGroup(Component *owner,System *system):NullControl(owner,system) {
  choosen=NULL;
}

LabeledEdit::LabeledEdit(Component *owner,System *system):Place(owner,system) {
  label=new Label(this,system);edit=new Edit(this,system);
  label->setparent(this);edit->setparent(this);
  edit->setpos(0,label->getsize().h,100,0);
  setsize(100,label->getsize().h+edit->getsize().h);
}

void LabeledEdit::notifyremove(Component *comp) {
  if (comp==label) label=NULL;
  if (comp==edit) edit=NULL;
  Place::notifyremove(comp);
}

void LabeledEdit::loadprops(XMLNode xml,Form *form) {
  Place::loadprops(xml,form);
  label->loadprops(xml,form);
  edit->setroot(XMLPath(xml.svar("root")));
//  label->settext(xml.svar("label"));
//  edit->settext(xml.svar("value"));
}

void FontButton::edit() {
  Font *f=getfont()->clone();
  if (f->edit()) setfont(f);
  delete f;
}

FontButton::FontButton(Component *owner,System *system):Button(owner,system) {
}

void FontButton::loaddata(XMLNode xml) {
  Font *f=system->createfont();
  f->load(xml.getat(getroot(xml)));
  setfont(f);
  delete f;
}
void FontButton::savedata(XMLNode xml) {
  getfont()->save(xml.getat(getroot(xml)));
}

PrinterOptions::PrinterOptions(Component *owner,System *system):Button(owner,system) {
  props=XMLNode::create();
  props.setname("printer");
}
void PrinterOptions::edit() {
  Printer *prn=system->createprinter();
  prn->loadprops(props);
  prn->setup();
  prn->saveprops(props);
  delete prn;
}

struct SettingsNode:TreeSource {
  Settings *ss;
  XMLNode xml;
  XMLNode data;
  bool autoexpand;

  string gettext() {return xml.svar("text");}
  int getsubs() {if (isplace()) return 0;return xml.count();}
  RefBase<TreeSource> nodeat(int index) {
    wantsubs();
    return nodes[index];
  }
  void wantsubs() {
    if (nodes.count==0) {
      for(int i=0;i<xml.count();i++) nodes.add(new SettingsNode(xml[i],ss));
      if (data.getnode()) {
        XMLNode sub=data.getat(root);
        for(int i=0;i<nodes.count;i++) nodes[i]->loaddata(sub);
      }
    }
  }
  bool isplace() {return xml.getname()=="place";}
  SettingsNode(XMLNode axml,Settings *ss):xml(axml),ss(ss),data(NULL) {
    refcnt=0;page=NULL;
    if (xml.hasname("rep.include"))
      xml=ss->system->findrep(xml.svar("name"));
    autoexpand=xml.ivar("expand");
    if (xml.getname()=="item") root=XMLPath(xml.svar("root"));
    else if (xml.getname()=="root") {
      while (xml.count()>0 && xml.getname()=="root") {
        root=root.child(xml.svar("root"));
        xml=xml[0];
      }
      if (xml.hasname("rep.include"))
        xml=ss->system->findrep(xml.svar("name"));
    }
  }
  void createall() {
    wantsubs();
    if (isplace() && !page) makepage();
    if (page) page->hide();
    for(int i=0;i<nodes.count;i++) nodes[i]->createall();
    ss->showpage(ss->visiblepage);
  }
  void clicked() {
    if (isplace() && !page) makepage();
    ss->showpage(page);
  }
  void makepage() {
    Component *own=ss->form;
    if (!own) own=ss;
    page=ss->system->createcontrol(xml.getname(),own);
    page->loadprops(xml,ss->form);
    page->setparent(ss->panel);
    if (data.getnode()) page->loaddata(data.getat(root));
  }
  void loaddata(XMLNode xml) {
    data=xml;
    if (!data.getnode()) return;
    XMLNode sub=data.getat(root);
    for(int i=0;i<nodes.count;i++) nodes[i]->loaddata(sub);
    if (page) page->loaddata(sub);
  }
  void savedata(XMLNode xml) {
    XMLNode sub=xml.getat(root);
    for(int i=0;i<nodes.count;i++) nodes[i]->savedata(sub);
    if (page) page->savedata(sub);
  }
private:
  TRCArray<SettingsNode> nodes;
  XMLPath root;
  Control *page;
  int refcnt;
public:
  void addref() {refcnt++;}
  void release() {if (!--refcnt) delete this;}
};


static void expandtree(NodeImage *image) {
  SettingsNode *node=dynamic_cast<SettingsNode*>(image->getsource());
  xassert(node);
  if (node->autoexpand) {
    image->expand();
    image->updatesubs();
//    if (node->xml.count()==image->subs.count) {
    for(int i=0;i<image->subs.count;i++) expandtree(image->subs[i]);
//    }
  }
}

void Settings::createall() {
  main->createall();
}

void Settings::loadprops(XMLNode xml,Form *form) {
  Place::loadprops(xml,form);
  bcreateall=xml.ivar("createall");
  this->form=form;
  if (xml.count()>0) {
    XMLNode l=xml[0];
    if (l.hasname("rep.include"))
      l=system->findrep(l.svar("name"));
    dsgn.assign(l);
  }
  expandtree(tree->main);
  if (bcreateall) createall();
}

void Settings::notifyremove(Component *comp) {
  Place::notifyremove(comp);
  if (comp==tree) tree=NULL;
  if (comp==panel) panel=NULL;
  if (comp==form) form=NULL;
  if (comp==visiblepage) visiblepage=NULL;
}

void Settings::onsize(int w,int h) {
  Place::onsize(w,h);
  tree->setpos(0,0,w/4,h);
  panel->setpos(w/4,0,3*w/4,h);
}

Settings::Settings(Component *owner,System *system):Place(owner,system) {
  denyloadchilds=true;bcreateall=false;
  form=NULL;visiblepage=NULL;
  dsgn=XMLNode::create();
  data=XMLNode::create();
  tree=new Tree(this,system);
  panel=new Panel(this,system);
  tree->setparent(this);
  tree->addhandler(this);
  panel->setparent(this);
  main=new SettingsNode(dsgn,this);
  main->addref();
  tree->setsource(main);
  main->autoexpand=true;
}
Settings::~Settings() {
  main->release();
}

void Settings::showpage(Control *page) {
  if (visiblepage) visiblepage->hide();
  visiblepage=page;
  if (visiblepage) {
    visiblepage->show();
    visiblepage->setsize(panel->getlayoutrect().size());
  }
}

void Settings::clickednode(SettingsNode *node) {
  node->clicked();
}

void Settings::on_notify(NotifyEvent &ev) {
  if (ev(tree,"onnodeclick")) clickednode((SettingsNode*)((NodeClickEvent&)ev).node->getsource());
}

void Settings::loaddata(XMLNode xml) {
  data.assign(xml);
  main->loaddata(data);
}
void Settings::savedata(XMLNode xml) {
  xml.assign(data);
  main->savedata(xml);
}


}


