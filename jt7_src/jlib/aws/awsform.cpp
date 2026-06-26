#include <jlib.h>
#pragma hdrstop

#include "awsform.h"
#include "awslayout.h"
#include "awswindow.h"
#include "awssystem.h"
#include "awspaint.h"
#include "awsproplist.h"
#include "awsextctrls.h"
#include "awstable.h"
#include "awstree.h"
#include "awshtmlview.h"
#include "awschecklist.h"
#include "awscolortable.h"
#include "awspager.h"

namespace aws {

struct InlineControl:Place {
  InlineControl(Component *owner,System *system):Place(owner,system) {
    setlayout(new Layout1(this));
  }
  void loadprops(XMLNode xml,Form *form) {
    Place::loadprops(xml,form);
    if (Control *content=form->searchinline(xml.svar("inlined"))) content->setparent(this);
  }
};

void Form::addinline(STRING name,Control *control) {
  inlines.add(new InlinePacker(name,control));
}

void Form::loaddata(XMLNode xml) {
//  for(int i=0;i<slaves.count;i++) slaves[i]->loaddata(xml);
}
void Form::savedata(XMLNode xml) {
//  for(int i=0;i<slaves.count;i++) slaves[i]->savedata(xml);
}

Window *Form::loadwindow(XMLNode xml) {
  Window *res=dynamic_cast<Window*>(load(xml));
  if (!res) throw JError("Form::loadwindow:loaded object is not window");
//  xassert(res);
  res->addhandler(this);
  return res;
}

Control *StdCreator::createcontrol(string name,Component *owner,System *system) {
  if (name=="button") {
    if (system->sysopt->canbutton) return new Button(owner,system);
    return new SpeedButton(owner,system);
  }
  if (name=="listbox" or name=="list") {
    if (system->sysopt->canlistbox) return new ListBox(owner,system);
    CheckList *res=new CheckList(owner,system);
    res->setvisiblechecks(false);
    return res;
  }
  if (name=="packedlist" or name=="combobox" or name=="combo") return new PackedList(owner,system);
  if (name=="place") return new Place(owner,system);
  if (name=="panel") return new Panel(owner,system);
  if (name=="edit") return new Edit(owner,system);
  if (name=="label") return new Label(owner,system);
  if (name=="labedit") return new LabeledEdit(owner,system);
  if (name=="fontbut") return new FontButton(owner,system);
  if (name=="printopt") return new PrinterOptions(owner,system);
  if (name=="settings") return new Settings(owner,system);
  if (name=="wraplabel") return new WrapLabel(owner,system);
  if (name=="memo") return new Memo(owner,system);
  if (name=="radiogroup") return new RadioGroup(owner,system);
  if (name=="checkbox") return new CheckBox(owner,system);
  if (name=="groupbox") return new GroupBox(owner,system);
  if (name=="scrollbox") return new ScrollBox(owner,system);
  if (name=="window") return new Window(owner,system);
  if (name=="paintbox") return new PaintControl(owner,system);
  if (name=="scrpaint") return new ScrolledPainter(owner,system);
  if (name=="wraplabel") return new WrapLabel(owner,system);
  if (name=="memo") return new Memo(owner,system);
  if (name=="proplist") return new PropertyList(owner,system);
  if (name=="inline") return new InlineControl(owner,system);
  if (name=="speedbutton") return new SpeedButton(owner,system);
  if (name=="pager" or name=="pagecontrol") {
    if (system->sysopt->canpager) return new PageControl(owner,system);
    return new awsPager(owner,system);
  }
  if (name=="table") return new Table(owner,system);
  if (name=="tree") return new Tree(owner,system);
  if (name=="speedgroup") return new SpeedGroup(owner,system);
  if (name=="trackbar") return new TrackBar(owner,system);
  if (name=="html") return new HTMLView(owner,system);
  if (name=="checklist") return new CheckList(owner,system);
  if (name=="colortable") return new ColorTable(owner,system);
  return NULL;
}
Layout *StdCreator::createlayout(string name,ParentControl *owner) {
  if (name=="grid") return new GridLayout(owner);
  if (name=="vlist") return new VListLayout(owner);
  if (name=="hlist") return new HListLayout(owner);
  if (name=="hflow") return new HFlowLayout(owner);
  if (name=="1") return new Layout1(owner);
  if (name=="auto") return new AutoLayout(owner);
  return NULL;
}


Form::Form(Component *owner,System *system):Component(owner,system) {
//  can_call_direct_es=true;
//  creator=system->getcreator();
}

/*
struct awsEnv:esEnv {
  void getvalue(esObject &obj) {}
  void callmethod(const char *obj,const char *name,esObject &res,esPars &pars) {
    if (Component *comp=form->search(obj)) comp->callmethod(name,res,pars);
  }
  void getprop(const char *obj,const char *name,esObject &res) {
    if (Component *comp=form->search(obj)) comp->getprop(name,res);
  }
  void callfunc(const char *name,esObject &res,esPars &pars) {
    CallEvent ce;ce.name="oncall";ce.type=et_call;ce.pars=pars;
    ce.sender=form;ce.value=NULL;ce.event=NULL;
    ce.fnname=name;
    form->do_dispatch(ce);
  }
public:
  Form *form;
};
*/

void Form::on_defaulthandler(BaseEvent &event) {
  do_dispatch(event);
/*  if (can_call_direct_es && event.iscall()) {
    esParser par;par.setsrc(event.getvalue());
    awsEnv env;env.form=this;
    esRuntime rt(&env);
    esObject res;
    try {
      esrunexpr(par,rt,res);
    } catch (esError &e) {
      jsystem->showerror(e.getmsg());
    }
  } else do_dispatch(event);
*/
//  else {
//    string value=event.getvalue();
//    if (value.begins("@")) {
//      if (value=="@close") win->close();
//    }
//    do_dispatch(event);
//  }
}

static Control *search_control(Component *owner,STRING name) {
  for(int i=0;i<owner->slavecnt();i++) {
    Control *ctrl=dynamic_cast<Control*>(owner->getslave(i));
    if (ctrl and ctrl->getname()==name) return ctrl;
    if (Control *res=search_control(owner->getslave(i),name)) return res;
  }
  return NULL;
}

Control *Form::search(STRING name) {
  return search_control(this,name);
/*  for(int i=0;i<slaves.count;i++) {
    Control *ctrl=dynamic_cast<Control*>(slaves[i]);
    if (ctrl and ctrl->getname()==name) return ctrl;
  }
  return NULL;*/
}

void Form::loadcontrol(Control *loading,XMLNode xml,ParentControl *parent) {
  if (loading) {
    if (parent) loading->setparent(parent);
    loading->loadprops(xml,this);loading->addhandler(this);
    if (parent) parent->loadlayoutchild(xml,loading);
  }
}

Control *Form::loadcontrol(XMLNode xml,ParentControl *parent) {
  if (xml.hasname("rep.include"))
    return loadcontrol(system->findrep(xml.svar("name")),parent);
  Control *loading=system->createcontrol(xml.getname(),this);
  loadcontrol(loading,xml,parent);
  return loading;
}

void Form::loadchilds(XMLNode xml,ParentControl *parent) {
  for(int i=0;i<xml.count();i++) loadcontrol(xml[i],parent);
}

Control *Form::load(XMLNode xml) {
  Control *loaded=system->createcontrol(xml.getname(),this);
  if (loaded) loaded->loadprops(xml,this),loaded->addhandler(this);
  return loaded;
}

}

