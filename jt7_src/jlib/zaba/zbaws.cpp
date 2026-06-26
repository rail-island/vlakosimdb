#include <jlib.h>
#pragma hdrstop

#include "zbaws.h"
#include "zbrt.h"
//#include <aws/awsad.h>
#include <aws/awsstddlgs.h>

namespace zaba {

IObjArray<ComponentType> comptypes;
bool addedawscreator=false;

struct awsCreator:aws::Creator {
  Control *createcontrol(string name,Component *owner,System *system) {
    for(int i=0;i<comptypes.count;i++)
      if (comptypes[i]->getloadname()==name)
        return dynamic_cast<Control*>(comptypes[i]->createnew(owner,system));
    return NULL;
  }
};

void registerComponent(ComponentType *type) {
  comptypes.add(type);
  if (!addedawscreator) {
    addedawscreator=true;
    awssys->addcreator(new awsCreator());
  }
}

System *awssys=NULL;

void init_aws_app(Runtime &rt) {
  rt.addintf("ApplicationClass.instance");
  zbApplicationClass *res=new zbApplicationClass(&rt,getzbw(awssys));
  rt.getsys()->setprop("Application",res);
  res->release();
}

zbObject *zbApplicationClass::getshiftState() {
  zbShiftState *res=new zbShiftState(awssys->shiftstate,rt);
  return res;
}

zbCanvas *createCanvas(const Canvas &src,Runtime *rt) {
  return new zbCanvas(rt,src);
}
zbObject *createFont(Font *font,Runtime *rt) {
  return new zbFont(rt,font);
}

void zbPageDistribution::draw(int page,zbCanvas *cnv) {
  pages.pages[page]->meta.draw(cnv->cnv);
}

void zbPageDistribution::distrib(LogPages &lpages,Array<int> &ofsx,Array<int> &ofsy) {
  pd=new PageDistrib(ofsx.count,ofsy.count,lpages.vp_count(),0,false);
  pages.distrib(ofsx,ofsy,lpages,false);
}
void zbPageDistribution::distribLines(LogPages &lpages,Array<int> &ofsx,Array<int> &ofsy) {
  pd=new PageDistrib(ofsx.count,ofsy.count,lpages.vp_count(),0,true);
  pages.distrib(ofsx,ofsy,lpages,true);
}


/*
void zbXMLTableModel::sort() {
  XMLTableModel *model=getmodel();
  XMLSort sorting;
  sorting.xml=model->xml;
  model->xml=sorting.work();
  getmodel()->model_update();
}
*/

void zbCanvas::font(zbFont *fnt) {
  getlink()->font(fnt->font);
}

void zbCanvas::centerText(int x1,int y1,int x2,int y2,string text,zbStore *flags) {
  Structure f(flags);
  getlink()->centertext(Rect(Point(x1,y1),Point(x2,y2)),text,f.bvalue("centerx",1),
  f.bvalue("centery",1),f.bvalue("clip",1));
}

/*
zbObject *createxmltablemodel(zbXMLNode *xml,Runtime *rt) {
  XMLTableModel *model=new XMLTableModel();
  model->xml=xml->xml;
  return new zbXMLTableModel(rt,model);
}
*/

zbObject *zbPicture::clone() {
  Picture *res=awssys->createpicture();
  res->setw(picture->getw());
  res->seth(picture->geth());
  picture->blit(res->getcanvas());
  res->releasecanvas();
  return new zbPicture(rt,res);
}

zbObject *zbForm::getvar(STRING name) {
  if (Form *frm=getlink()) {
    if (Control *ctrl=frm->search(name)) {
      return createComponent(ctrl,rt);
    }
  }
  return NULL;
}

void _dozbevent(BaseEvent &event,zbObject *handler,Runtime *rt,zbssData *data) {
  if (!handler) return;
  string v=event.value->value;
  zaba::ssRunner ssr(rt);
  ssr.data=data;
  ssr.handler=handler;
  TCntVec<zbObject*> allocated;
  rtti::Class *cl=event.classtype;
  if (cl) {
    for(int i=0;i<cl->flds.count;i++) {
      rtti::FldDef &fd=cl->flds[i];
      if (fd.istype(CanvasClass)) {
        Canvas &cnv=*(Canvas*)rtti::get_ptr(&event,fd.offset);
        cnv.fg(clnnone).rect(cnv.clip()).fg(clnblack);
        zbCanvas *zbcnv=createCanvas(cnv,rt);// zbCanvas(rt,h);
        ssr.addglobal(fd.name,zbcnv);
        allocated.add(zbcnv);
      } else if (fd.istype(ColorClass)) {
        ssr.addglobal(fd.name,gr::nativecolortostr(rtti::get_int(&event,fd.offset)));
      } else if (fd.istype(rtti::IntClass)) {
        ssr.addglobal(fd.name,itos(rtti::get_int(&event,fd.offset)));
      } else if (fd.isarray(rtti::CharClass)) {
        int size=fd.type.count;
        char *array=new char[size];
        rtti::get_array(array,&event,fd.offset,size);
        if (size>0) array[size-1]=0,ssr.addglobal(fd.name,array);
        delete []array;
      } else if (fd.istype(rtti::CharClass)) {
        char ch=rtti::get_any<char>(&event,fd.offset);
        char s[2]={ch,0};
        ssr.addglobal(fd.name,s);
      } else if (fd.istype(KeyClass)) {
        zbKey *zbkey=new zbKey(rtti::get_any<awsKey>(&event,fd.offset),rt);
        ssr.addglobal(fd.name,zbkey);
        allocated.add(zbkey);
      } else if (fd.istype(MouseButtonClass)) {
        string type="left";
        MouseButton value=rtti::get_any<MouseButton>(&event,fd.offset);
        if (value==mb_right) type="right";
        if (value==mb_middle) type="middle";
        ssr.addglobal(fd.name,type);
      } else if (fd.istype(ShiftStateClass)) {
        zbShiftState *zbstate=new zbShiftState(rtti::get_any<ShiftState>(&event,fd.offset),rt);
        ssr.addglobal(fd.name,zbstate);
        allocated.add(zbstate);
      }
    }
  }
  ssr.run(v);
  if (cl) {
    for(int i=0;i<cl->flds.count;i++) {
      rtti::FldDef &fd=cl->flds[i];
      if (!(fd.attr&1)) continue;
      if (fd.istype(ColorClass)) {
        string res=ssr.getglobal(fd.name);
        rtti::set_int(&event,fd.offset,gr::strtonativecolor(res.c_str()));
      } else if (fd.istype(rtti::IntClass)) {
        string res=ssr.getglobal(fd.name);
        rtti::set_int(&event,fd.offset,res.ivalue());
      }
    }
  }
  for(int i=0;i<allocated.count;i++) allocated[i]->release();
}

void dozbevent(BaseEvent &event,zbObject *handler,Runtime *rt,zbssData *data) {
  try {
   _dozbevent(event,handler,rt,data);
  } catch (JException &e) {
    e.show();
  }
}

void zbForm::_on_do(BaseEvent &event) {
  _dozbevent(event,handler,rt,this);
}

void zbForm::on_do(BaseEvent &event) {
  dozbevent(event,handler,rt,this);
}
void zbApplicationClass::on_do(BaseEvent &event) {
  dozbevent(event,handler,rt,this);
}

struct zbSimpleTimerEvent:SimpleTimerEvent {
  ZabaObject handler;
  ObjectArray args;
  void event_work() {
    ZabaObject res=callMethod(handler,"op:call",args.buf,args.count);
    delete this;
  }
};

void newsimpletimer(float secs,zbObject *obj,zbObject **pars,int cnt) {
  zbSimpleTimerEvent *event=new zbSimpleTimerEvent();
  for(int i=0;i<cnt;i++) event->args.add(pars[i]->addref());
  event->handler=obj;
  awssys->createsimpletimer(event,secs);
}

void zbTimer::_on_notify(NotifyEvent &event) {
  if (handler && event.istype("ontimer")) {
    ZabaObject res=callMethod(handler,"op:call",0,0);
  }
}
void zbTimer::on_notify(NotifyEvent &event) {
  try {
   _on_notify(event);
  } catch (JException &e) {
    getlink()->stop();
    e.show();
    jsystem->showerror("Unhandled exception in timer,timer was deactivated");
  }
}

zbwProp::~zbwProp() {
  if (zbc) {
    zbc->prop=NULL;
    zbc->release();
  }
}

/*
zbObject *createWindow(Window *win,Runtime *rt) {
  zbwProp *prop=getzbw(win);
  if (prop->zbc) return prop->zbc->addref();
  zbWindow *res=new zbWindow(rt,prop);
  return res;
}
*/

/*
zbObject *getapphandler(Runtime *rt) {
  zbObject *res=rt->getsys()->getprop("@app_handler");
  if (res) return res->addref();
  return createVoid(rt);
}
void setapphandler(zbObject *value,Runtime *rt) {
  rt->getsys()->setprop("@app_handler",value);
}
*/


void zbControl::setparent(zbControl *parent) {
  if (ParentControl *pc=dynamic_cast<ParentControl*>(parent->getlink())) {
    getlink()->setparent(pc);
  }
}
zbObject *zbControl::getparent() {
  if (Control *ctrl=dynamic_cast<Control*>(getcomp()))
    return createComponent(ctrl->getparent(),rt);
  return createVoid(rt);
}


zbwProp *getzbw(Component *comp) {
  zbwProp *prop=dynamic_cast<zbwProp*>(comp->searchprop("zbwProp"));
  if (!prop) {
    prop=new zbwProp("zbwProp",comp);
    comp->setprop(prop);
  }
  return prop;
}
zbObject *createComponent(Component *comp,Runtime *rt) {
  if (!comp) return createVoid(rt);
  zbwProp *prop=getzbw(comp);
  if (prop->zbc) return prop->zbc->addref();
  for(int i=0;i<comptypes.count;i++) if (zbObject *res=comptypes[i]->createzb(comp,rt)) return res;
  if (dynamic_cast<Window*>(comp)) return new zbWindow(rt,prop);
  if (dynamic_cast<Form*>(comp)) return new zbForm(rt,prop);
  if (dynamic_cast<ListBox*>(comp)) return new zbListBox(rt,prop);
  if (dynamic_cast<PackedList*>(comp)) return new zbListBox(rt,prop);
  if (dynamic_cast<RadioGroup*>(comp)) return new zbRadioGroup(rt,prop);
  if (dynamic_cast<ColorTable*>(comp)) return new zbColorTable(rt,prop);
  if (dynamic_cast<Button*>(comp)) return new zbButton(rt,prop);
  if (dynamic_cast<SpeedButton*>(comp)) return new zbSpeedButton(rt,prop);
  if (dynamic_cast<SpeedGroup*>(comp)) return new zbSpeedGroup(rt,prop);
  if (dynamic_cast<Edit*>(comp)) return new zbEdit(rt,prop);
  if (dynamic_cast<Memo*>(comp)) return new zbMemo(rt,prop);
  if (dynamic_cast<Label*>(comp)) return new zbLabel(rt,prop);
  if (dynamic_cast<WrapLabel*>(comp)) return new zbWrapLabel(rt,prop);
  if (dynamic_cast<Table*>(comp)) return new zbTable(rt,prop);
  if (dynamic_cast<Tree*>(comp)) return new zbTree(rt,prop);
  if (dynamic_cast<MenuItem*>(comp)) return new zbMenuItem(rt,prop);
  if (dynamic_cast<Printer*>(comp)) return new zbPrinter(rt,prop);
  if (dynamic_cast<PaintControl*>(comp)) return new zbPaintBox(rt,prop);
  if (dynamic_cast<Timer*>(comp)) return new zbTimer(rt,prop);
  if (dynamic_cast<PropertyList*>(comp)) return new zbPropertyList(rt,prop);
  if (dynamic_cast<Console*>(comp)) return new zbConsole(rt,prop);
  if (dynamic_cast<PageControl*>(comp)) return new zbPager(rt,prop);
  if (dynamic_cast<TrackBar*>(comp)) return new zbTrackBar(rt,prop);
  if (dynamic_cast<CheckBox*>(comp)) return new zbCheckBox(rt,prop);
  if (dynamic_cast<HTMLView*>(comp)) return new zbHTMLView(rt,prop);
  if (dynamic_cast<CheckList*>(comp)) return new zbCheckList(rt,prop);
  if (dynamic_cast<ScrolledPainter*>(comp)) return new zbScrolledPainter(rt,prop);
  if (dynamic_cast<Control*>(comp)) return new zbControl(rt,prop);
  return new zbComponent(rt,prop);
}

#include "lib/winobjs.h"
//void initawsrt(Runtime &rt) {
//  list_winobjs(rt.addsub)
//}
void initawsrt(Runtime &rt) {init_winobjs_rt(rt);}

zbObject *newwindow(Runtime *rt) {
  Window *win=new Window(awssys,awssys);
  win->show();
  zbWindow *res=new zbWindow(rt,getzbw(win));
  return res;
}
zbObject *newwaitwindow(Runtime *rt) {
  WaitWindow *win=new WaitWindow(awssys,awssys);
  zbWaitWindow *res=new zbWaitWindow(rt,getzbw(win));
  return res;
}
zbObject *newspeedbutton(zbForm *form,Runtime *rt) {
  SpeedButton *awc=new SpeedButton(form->getlink(),awssys);
  awc->addhandler(form->getlink());
  zbSpeedButton *res=new zbSpeedButton(rt,getzbw(awc));
  return res;
}

void zbControl::setvisible(bool value) {
  Control *c=getlink();
  c->setvisible(value);
  ParentControl *p=c->getparent();
  if (p) p->relayout();
}

zbObject *zbForm::loadWindow(STRING repname,Runtime *rt) {
  Form *frm=getlink();
  Window *win=frm->loadwindow(awssys->findrep(repname));
  return createComponent(win,rt);
}

zbObject *newform(Runtime *rt) {
  Form *frm=new Form(awssys,awssys);
  zbForm *res=new zbForm(rt,getzbw(frm));
  return res;
}
zbObject *newform(zbComponent *comp,Runtime *rt) {
  Form *frm=new Form(comp->getlink(),awssys);
  zbForm *res=new zbForm(rt,getzbw(frm));
  return res;
}
zbObject *newmenuitem(Runtime *rt) {
  MenuItem *menu=awssys->createmenuitem(awssys);
  zbMenuItem *res=new zbMenuItem(rt,getzbw(menu));
  return res;
}
zbObject *zbApplicationClass::getsysmenu(Runtime *rt) {
  MenuItem *menu=awssys->getsysmenu();
  if (menu) {
    zbMenuItem *res=new zbMenuItem(rt,getzbw(menu));
    return res;
  }
  return createVoid(rt);
}
void zbApplicationClass::setsysmenu(zbObject *menu) {
  if (zbMenuItem *item=dynamic_cast<zbMenuItem*>(menu)) {
    awssys->setsysmenu(item->getlink());
  } else awssys->setsysmenu(NULL);
}


zbObject *newtimer(Runtime *rt) {
  Timer *tim=awssys->createtimer(awssys);
  zbTimer *res=new zbTimer(rt,getzbw(tim));
//  tim->addhandler(res);
  return res;
}

zbPrinter *newprinter(Runtime *rt) {
  Printer *prn=awssys->createprinter();
  zbPrinter *res=new zbPrinter(rt,getzbw(prn));
  return res;
}


/*
zbObject *zbTable::getmodel() {
  TableModel *res=getlink()->getmodel();
  if (res) return new zbTableModel(rt,res);
  return createVoid(rt);
}
*/

zbObject *alert(Runtime *rt,zbObject **pars,int cnt) {
  MEMSTROUT mso;
  for(int i=0;i<cnt;i++) {mso<<pars[i]->tostring();if (i<cnt-1) mso<<"\n";}
  jsystem->showinfo(mso.str());
  return createVoid(rt);
}

void zbApplicationClass::runapp(Interpreter *ir) {
  static bool running=false;
  if (running) ir->nativeerror("Application.run called twice");
  else {
    running=true;
    awssys->run();
  }
}
//void exitapp() {awssys->quit();}
//void runAD(XMLNode xml) {runad(xml,awssys);}
XMLNode zbApplicationClass::getopts() {return awssys->getopt();}
zbFont *newfont(Runtime *rt) {return new zbFont(rt,awssys->createfont());}
zbPicture *newpicture(Runtime *rt) {return new zbPicture(rt,awssys->createpicture());}
void zbApplicationClass::loadresource(string files) {
  for(lineiter it=files.getlines();*it;it++) {
    awssys->getrepos()->loadfile(*it);
  }
//  awssys->getrepos()->searchincludes();
}
zbPicture *createPicture(Picture *pic,Runtime *rt) {return new zbPicture(rt,pic);}
zbObject *newconsole(Runtime *rt) {
  Console *con=new awsConsole(awssys,awssys);
//  con->show();
  zbConsole *res=new zbConsole(rt,getzbw(con));
  return res;
}
zbObject *getstdcon(Runtime *rt) {
  return createComponent(awssys->getstdcon(),rt);
}

zbKey *newkey(string key,Runtime *rt) {return new zbKey(key,rt);}

//void addSearchPath(string file) {awssys->searchpath.add(file);}
//void setOptionFile(string file) {awssys->setoptionfile(file);}
zbObject *runopendialog(string filter,Runtime *rt) {
  string res;
  if (jsystem->getopenfn(filter,rt->rootdir,res)) return createString(res,rt);
  return createVoid(rt);
}
zbObject *runsavedialog(string filter,Runtime *rt) {
  string res;
  if (jsystem->getsavefn(filter,rt->rootdir,res)) return createString(res,rt);
  return createVoid(rt);
}
zbObject *runcolordialog(string input,Runtime *rt) {
  zbObject *res=NULL;
  ColorDialog *cd=awssys->createcolordialog();
  cd->setcolor(gr::strtonativecolor(input.c_str()));
  if (cd->execute()) {
    res=createString(gr::nativecolortostr(cd->getcolor()),rt);
  } else res=createVoid(rt);
  delete cd;
  return res;
}

zbObject *zbLogPages::complete(zbArray *ofsx,zbArray *ofsy) {
  TCntVec<int> ox,oy;
  for(int i=0;i<ofsx->items.count;i++) ox.add(ofsx->items[i]->toint());
  for(int i=0;i<ofsy->items.count;i++) oy.add(ofsy->items[i]->toint());
  zbPageDistribution *res=new zbPageDistribution(rt);
  res->distrib(*pages,ox,oy);
  return res;
}

zbObject *zbLogPages::completeLines(zbArray *ofsx,zbArray *ofsy) {
  TCntVec<int> ox,oy;
  for(int i=0;i<ofsx->items.count;i++) ox.add(ofsx->items[i]->toint());
  for(int i=0;i<ofsy->items.count;i++) oy.add(ofsy->items[i]->toint());
  zbPageDistribution *res=new zbPageDistribution(rt);
  res->distribLines(*pages,ox,oy);
  return res;
}

string zbKey::getkey() {
  string res;
  char *buf=res.reservebuffer(1);
  *buf=key.ch;
  buf[1]=0;
  res.updatelen();
  return res;
}

string zbmsgbox(string msg,string buts,Interpreter *ir) {
  if (buts=="yn") {
    bool res=jsystem->dialogyn(msg);
    return res?"y":"n";
  }
  if (buts=="ync") {
    int res=jsystem->dialogync(msg);
    switch (res) {
      case 0:return "y";
      case 1:return "n";
      case 2:return "c";
    }
    return res?"y":"n";
  }
  if (buts=="ar") {
    bool res=jsystem->dialogar(msg);
    return res?"r":"a";
  }
  if (buts=="oc") {
    bool res=jsystem->dialogoc(msg);
    return res?"o":"c";
  }
  if (buts=="o") {jsystem->showinfo(msg);return "o";}
  ir->nativeerror("Unknown argument for Window.msgbox:"+buts);
  return "";
}

int zbmsgwin(string title,string msg,string buttons) {
  return msgbox(awssys,title,msg,buttons);
}
zbObject *zbinputbox(string title,string msg,string def,Runtime *rt) {
  if (inputquery(awssys,title,msg,def)) return createString(def,rt);
  return createVoid(rt);
}

inline zbObject *newlogpages(Runtime *rt) {return new zbLogPages(rt);}
inline zbObject *newpanegrp(Runtime *rt) {return new zbPaneGrp(rt);}
inline zbObject *newresizecanvas(zbCanvas *cnv,float kx,float ky,Runtime *rt) {
  return new zbResizeCanvas(rt,cnv->cnv,kx,ky);
}
inline zbObject *newsubcanvas(zbCanvas *cnv,int dx,int dy,Runtime *rt) {
  return new zbSubCanvas(rt,cnv->cnv,dx,dy);
}
inline zbObject *newmetacanvas(Runtime *rt) {
  return new zbMetaCanvas(rt);
}
inline zbObject *newopendialog(Runtime *rt) {return new zbOpenDialog(rt);}
inline zbObject *newsavedialog(Runtime *rt) {return new zbSaveDialog(rt);}

void zbTableModel::getcols(TCntVec<TableCol> &res) {
  RefBase<zbTableCols> cols=new zbTableCols(rt);
  getCols(cols);
  res.assign(cols->res);
}
void zbControl::setfont(zbFont *value) {
  getlink()->setfont(value->font);
}
bool zbApplicationClass::runxmldialog(XMLNode dialog,XMLNode data) {return aws::runxmldialog(awssys,dialog,data);}
bool zbApplicationClass::runsettingsdialog(string dialog,XMLNode data) {return aws::settingsdialog(awssys,awssys->findrep(dialog),data);}
zbObject *zbApplicationClass::findresource(string name,Interpreter *ir) {return createXMLNode(awssys->findrep(name),ir->rt);}
zbObject *newtextcanvas(zbCanvas *cnv,int wi,Runtime *rt) {return new zbTextCanvas(cnv,wi,-1,rt);}
zbObject *newtextcanvas(zbCanvas *cnv,int wi,int pagehi,Runtime *rt) {return new zbTextCanvas(cnv,wi,pagehi,rt);}
zbObject *newmidiout(Runtime *rt) {return new zbMidiOut(rt);}

float getppmmx() {return awssys->getppmmx();}
float getppmmy() {return awssys->getppmmy();}
//string getapptitle() {return awssys->gettitle();}
//void setapptitle(string value) {awssys->settitle(value);}

#pragma option push -w-

#include "lib/winobjs.cc"

#pragma option pop

}


