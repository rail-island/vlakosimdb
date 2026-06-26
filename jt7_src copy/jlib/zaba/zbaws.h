#ifndef zbawsH
#define zbawsH

#include "zbobj.h"
#include <aws/awscontrol.h>
#include <aws/awssystem.h>
#include <aws/awswindow.h>
#include <aws/awsform.h>
#include <aws/awspaint.h>
#include <aws/awstable.h>
#include <aws/awstree.h>
#include <aws/awschecklist.h>
#include "zbss.h"
#include <gr/comgr.h>
#include <ext/xmldb.h>
#include <aws/awsprint.h>
#include <aws/awsextctrls.h>
#include <aws/awsproplist.h>
#include "zbstd.h"
#include <aws/awsstddlgs.h>
#include <aws/awstextcnv.h>
#include "zbint.h"
#include <aws/awshtmlview.h>
#include <aws/awscolortable.h>

namespace zaba {
using namespace aws;

void init_aws_app(Runtime &rt);

extern System *awssys;

class zbComponent;

zbObject *createComponent(Component *comp,Runtime *rt);

struct ComponentType {
  virtual string getloadname()=0;
  virtual zbObject *createzb(Component *comp,Runtime *rt) {return NULL;}
  virtual aws::Component *createnew(aws::Component *owner,aws::System *system)=0;
};
void registerComponent(ComponentType *type);

struct zbwProp:awsProp {
  Component *awsc;
  zbComponent *zbc;
  zbwProp(STRING name,Component *awsc):awsProp(name),awsc(awsc) {zbc=NULL;}
  ~zbwProp();
};

zbwProp *getzbw(Component *comp);

struct zbComponent:zbObject,EventHandler {
  zbwProp *prop;
//  Component *link;
//  void notifyremove(Component *obj) {if (obj==link) link=NULL;Component::notifyremove(obj);}
  zbComponent(Runtime *rt,zbwProp *prop,STRING intf="Component.instance"):prop(prop),zbObject(rt,intf) {addref();prop->zbc=this;prop->handler=this;}
  Component *getcomp() {return prop?prop->awsc:NULL;}
  Component *getlink() {return getcomp();}
};

zbObject *createFont(Font *f,Runtime *rt);

class zbFont;
struct zbControl:zbComponent {
  zbControl(Runtime *rt,zbwProp *prop,STRING intf="Control.instance"):zbComponent(rt,prop,intf) {}
  int getX() {return getlink()->getpos().x;}
  void setX(int value) {getlink()->setpos(value,getY());}
  int getY() {return getlink()->getpos().y;}
  void setY(int value) {getlink()->setpos(getX(),value);}
  int getW() {return getlink()->getsize().w;}
  void setW(int value) {getlink()->setsize(value,getH());}
  int getH() {return getlink()->getsize().h;}
  void setH(int value) {getlink()->setsize(getW(),value);}
  void setvisible(bool value);
  Control *getlink() {return (Control*)getcomp();}
  void destroy() {getlink()->destroy();}
  zbObject *getparent();
  void setparent(zbControl *parent);
  zbObject *getfont() {return createFont(getlink()->getfont()->clone(),rt);}
  void setfont(zbFont *value);
};

struct zbWindow:zbControl {
  Window *getlink() {return (Window*)getcomp();}
  zbWindow(Runtime *rt,zbwProp *prop,STRING intf="Window.instance"):zbControl(rt,prop,intf) {}
  void show() {getlink()->show();}
  void showModal() {getlink()->showmodal();}
  int getX() {return getlink()->getpos().x;}
  void setX(int value) {getlink()->setpos(value,getY());}
  int getY() {return getlink()->getpos().y;}
  void setY(int value) {getlink()->setpos(getX(),value);}
  int getW() {return getlink()->getsize().w;}
  void setW(int value) {getlink()->setsize(value,getH());}
  int getH() {return getlink()->getsize().h;}
  void setH(int value) {getlink()->setsize(getW(),value);}
};

struct zbSettings:zbControl {
  Settings *getlink() {return (Settings*)getcomp();}
  zbSettings(Runtime *rt,zbwProp *prop,STRING intf="Settings.instance"):zbControl(rt,prop,intf) {}
};

struct zbConsole:zbComponent {
  Console *getlink() {return (Console*)getcomp();}
  zbConsole(Runtime *rt,zbwProp *prop,STRING intf="Console.instance"):zbComponent(rt,prop,intf) {}
  void print(zbObject **pars,int parcnt) {MEMSTROUT str;for(int i=0;i<parcnt;i++) str<<pars[i]->tostring();getlink()->print(str.str());}
  void println(zbObject **pars,int parcnt) {MEMSTROUT str;for(int i=0;i<parcnt;i++) str<<pars[i]->tostring();str<<"\n";getlink()->print(str.str());}
  string readln() {return getlink()->readln();}
  void destroy() {getlink()->destroy();}
};

struct zbMenuItem:zbComponent {
  MenuItem *getlink() {return (MenuItem*)getcomp();}
  zbMenuItem(Runtime *rt,zbwProp *prop,STRING intf="MenuItem.instance"):zbComponent(rt,prop,intf) {}
  void load(zbXMLNode *xml) {getlink()->loadprops(xml->xml,NULL);}
};

struct zbButton:zbControl {
  Button *getlink() {return (Button*)getcomp();}
  zbButton(Runtime *rt,zbwProp *prop,STRING intf="Button.instance"):zbControl(rt,prop,intf) {}
  void setonclick(string onclick) {getlink()->ev_click.value=onclick;}
  string getonclick() {return getlink()->ev_click.value;}
};

struct zbSpeedButton:zbControl {
  SpeedButton *getlink() {return (SpeedButton*)getcomp();}
  zbSpeedButton(Runtime *rt,zbwProp *prop,STRING intf="SpeedButton.instance"):zbControl(rt,prop,intf) {}
  void setonclick(string onclick) {getlink()->ev_click.value=onclick;}
  string getonclick() {return getlink()->ev_click.value;}
  string getcolor() {return gr::nativecolortostr(getlink()->getcolor());}
  void setcolor(string color) {getlink()->setcolor(gr::strtonativecolor(color.c_str()));}
};
struct zbSpeedGroup:zbControl {
  SpeedGroup *getlink() {return (SpeedGroup*)getcomp();}
  zbSpeedGroup(Runtime *rt,zbwProp *prop,STRING intf="SpeedGroup.instance"):zbControl(rt,prop,intf) {}
  void setcur(int value) {getlink()->setcur(value);}
  int getcur() {return getlink()->getcur();}
};

struct zbColorTable:zbControl {
  ColorTable *getlink() {return (ColorTable*)getcomp();}
  zbColorTable(Runtime *rt,zbwProp *prop,STRING intf="ColorTable.instance"):zbControl(rt,prop,intf) {}
  string getcolor(int row,int col) {return gr::nativecolortostr(getlink()->getcolor(row,col));}
  string setcolor(int row,int col,string color) {getlink()->setcolor(row,col,gr::strtonativecolor(color.c_str()));return color;}
};

struct zbListBox:zbControl {
  ListBox *getlink() {return (ListBox*)getcomp();}
  zbListBox(Runtime *rt,zbwProp *prop,STRING intf="ListBox.instance"):zbControl(rt,prop,intf) {}
};

struct zbPackedList:zbControl {
  PackedList *getlink() {return (PackedList*)getcomp();}
  zbPackedList(Runtime *rt,zbwProp *prop,STRING intf="PackedList.instance"):zbControl(rt,prop,intf) {}
};

struct zbRadioGroup:zbControl {
  RadioGroup *getlink() {return (RadioGroup*)getcomp();}
  zbRadioGroup(Runtime *rt,zbwProp *prop,STRING intf="RadioGroup.instance"):zbControl(rt,prop,intf) {}
};

struct zbCheckBox:zbControl {
  CheckBox *getlink() {return (CheckBox*)getcomp();}
  zbCheckBox(Runtime *rt,zbwProp *prop,STRING intf="CheckBox.instance"):zbControl(rt,prop,intf) {}
};

struct zbEdit:zbControl {
  Edit *getlink() {return (Edit*)getcomp();}
  zbEdit(Runtime *rt,zbwProp *prop,STRING intf="Edit.instance"):zbControl(rt,prop,intf) {}
//  void setonclick(string onclick) {getlink()->ev_click.value=onclick;}
//  string getonclick() {return getlink()->ev_click.value;}
};

struct zbMemo:zbControl {
  Memo *getlink() {return (Memo*)getcomp();}
  zbMemo(Runtime *rt,zbwProp *prop,STRING intf="Memo.instance"):zbControl(rt,prop,intf) {}
};

struct zbTrackBar:zbControl {
  TrackBar *getlink() {return (TrackBar*)getcomp();}
  zbTrackBar(Runtime *rt,zbwProp *prop,STRING intf="TrackBar.instance"):zbControl(rt,prop,intf) {}
};

struct zbLabel:zbControl {
  Label *getlink() {return (Label*)getcomp();}
  zbLabel(Runtime *rt,zbwProp *prop,STRING intf="Label.instance"):zbControl(rt,prop,intf) {}
};

struct zbWrapLabel:zbControl {
  WrapLabel *getlink() {return (WrapLabel*)getcomp();}
  zbWrapLabel(Runtime *rt,zbwProp *prop,STRING intf="WrapLabel.instance"):zbControl(rt,prop,intf) {}
};

struct zbPaintBox:zbControl {
  PaintControl *getlink() {return (PaintControl*)getcomp();}
  zbPaintBox(Runtime *rt,zbwProp *prop,STRING intf="PaintBox.instance"):zbControl(rt,prop,intf) {}
  void setonpaint(string onclick) {getlink()->ev_paint.value=onclick;}
  string getonpaint() {return getlink()->ev_paint.value;}
};

struct zbScrolledPainter:zbControl {
  ScrolledPainter *getlink() {return (ScrolledPainter*)getcomp();}
  zbScrolledPainter(Runtime *rt,zbwProp *prop,STRING intf="ScrolledPainter.instance"):zbControl(rt,prop,intf) {}
  void setonpaint(string onclick) {getlink()->ev_paint.value=onclick;}
  string getonpaint() {return getlink()->ev_paint.value;}
  void setInSize(int w,int h) {getlink()->setinsize(Size(w,h));}
};

class zbFont;
struct zbCanvas:zbObject {
  Canvas cnv;
//  int handle;
//  Canvas *getlink() {return (Canvas*)awssys->gethm()->get(handle);}
  Canvas *getlink() {if (!cnv.cnv.isnull()) return &cnv;return NULL;}
  Canvas *cvalue() {return getlink();}
  zbCanvas(Runtime *rt,const Canvas &cnv,STRING intf="Canvas.instance"):zbObject(rt,intf),cnv(cnv) {}
  ~zbCanvas() {}
//  zbObject *line(int x1,int y1,int x2,int y2) {getlink()->line(x1,y1,x2,y2);return addref();}
//  zbObject *rectangle(int x1,int y1,int x2,int y2) {getlink()->rect(x1,y1,x2,y2);return addref();}
//  zbObject *ellipse(int x1,int y1,int x2,int y2) {getlink()->ellipse(x1,y1,x2,y2);return addref();}
//  zbObject *fg(zbObject *color) {getlink()->fg(gr::strtonativecolor(color->tostring().c_str()));return addref();}
//  zbObject *bg(zbObject *color) {getlink()->bg(gr::strtonativecolor(color->tostring().c_str()));return addref();}
  void fg(string color) {getlink()->fg(gr::strtonativecolor(color.c_str()));}
  void bg(string color) {getlink()->bg(gr::strtonativecolor(color.c_str()));}
  void floodfill(int x,int y,string mode,string color) {getlink()->floodfill(x,y,mode,gr::strtonativecolor(color.c_str()));}
  void drawFrame(int x1,int y1,int x2,int y2,string type) {getlink()->drawframe(Rect(Point(x1,y1),Point(x2,y2)),type=="sunken");}
  void centerText(int x1,int y1,int x2,int y2,string text,zbStore *flags=NULL);
//  void text(int x,int y,zbObject *str) {getlink()->text(x,y,str->tostring());}
//  int texthi(zbObject *str) {return getlink()->texthi(str->tostring());}
//  int textwi(zbObject *str) {return getlink()->textwi(str->tostring());}
  void font(zbFont *fnt);
  zbObject *nextBand(int hi) {Canvas res=getlink()->nextband(hi);return new zbCanvas(rt,res);}
  zbObject *nextBand(int hi,string type) {Canvas res=getlink()->nextband(hi,type);return new zbCanvas(rt,res);}
  string putpixel(int x,int y,string color) {cnv.pixel(x,y,gr::strtonativecolor(color.c_str()));return color;}
  string getpixel(int x,int y) {return gr::nativecolortostr(cnv.getpixel(x,y));}
  void drawSelection(int x1,int y1,int x2,int y2) {cnv.drawselection(Rect(Point(x1,y1),Point(x2,y2)));}
};

struct zbForm:zbComponent,zbssData {
  zbObject *handler;
  Form *getlink() {return (Form*)getcomp();}
  Form *cvalue() {return getlink();}
  zbForm(Runtime *rt,zbwProp *prop,STRING intf="Form.instance"):zbComponent(rt,prop,intf) {handler=NULL;}
  zbObject *loadWindow(STRING repname,Runtime *rt);
  void on_do(BaseEvent &event);
  void sethandler(zbObject *hnd) {if (handler) handler->release();handler=hnd->addref();}
  zbObject *gethandler() {if (handler) return handler->addref();return createVoid(rt);}
  ~zbForm() {if (handler) handler->release();}
  void enum_refs_and_add_inrefs() {zbComponent::enum_refs_and_add_inrefs();if (handler) handler->gc_add_in_ref();}
  void enum_refs_and_validate() {zbComponent::enum_refs_and_validate();if (handler) handler->gc_validate();}
  void unlink_refs() {zbComponent::unlink_refs();if (handler) handler->gc_release(),handler=NULL;}
  zbObject *getvar(STRING name);
  void destroy() {getlink()->destroy();}
private:
  void _on_do(BaseEvent &event);
};

struct zbTimer:zbComponent {
  RefBase<zbObject> handler;
  Timer *getlink() {return (Timer*)getcomp();}
  zbTimer(Runtime *rt,zbwProp *prop,STRING intf="Timer.instance"):zbComponent(rt,prop,intf) {}
  void on_notify(NotifyEvent &event);
  zbObject *gethandler() {if (handler) return handler->addref();return createVoid(rt);}
  void sethandler(zbObject *obj) {if (obj->tobool()) handler=obj;else handler=NULL;}
private:
  void _on_notify(NotifyEvent &event);
};

struct zbTableCols:zbObject {
  TCntVec<TableCol> res;
  zbTableCols(Runtime *rt,STRING intf="TableCols.instance"):zbObject(rt,intf) {}
  void add(string fldid,string text,string type="text") {
    TableColType tct=tct_text;
    if (type=="check") tct=tct_check;
    res.add(TableCol(text,fldid.c_str(),tct));
  }
};

struct zbTableModel:zbInstance,TableModel {
  virtual int getRowCount()=0;
  virtual string getCellText(string fldid,int row)=0;
  virtual void getCols(zbTableCols *cols)=0;

  zbTableModel(Runtime *rt,STRING intf):zbInstance(rt,intf) {}

  int getrowcount() {return getRowCount();}
  string getcelltext(const char *fldid,int row) {return getCellText(fldid,row);}
  override void getcols(TCntVec<TableCol> &res);
};

/*
struct zbTableModel:zbObject {
  TableModel *model;
  zbTableModel(Runtime *rt,TableModel *model,STRING intf=""):zbObject(rt,intf),model(model) {model->addref();}
  ~zbTableModel() {model->release();}
};
*/

/*
struct zbXMLTableModel:zbTableModel {
  virtual int getRowCount() {return 0;}
  virtual string getCellText(string fldid,int row) =0;
  virtual void getCols(zbTableCols *cols)=0;

  XMLTableModel *getmodel() {return (XMLTableModel*)model;}
  zbXMLTableModel(Runtime *rt,XMLTableModel *model,STRING intf="XMLTableModel.instance"):zbTableModel(rt,model,intf) {}
  void clear() {getmodel()->clear();}
  void add(STRING name,STRING text,bool sub) {getmodel()->add(name,text,sub);}
//  void sort();
  XMLRef getxml() {return getmodel()->xml;}
  void setxml(XMLRef xml) {getmodel()->xml=xml;getmodel()->model_update();}
};
*/

struct zbTable:zbControl {
  zbTable(Runtime *rt,zbwProp *prop,STRING intf="Table.instance"):zbControl(rt,prop,intf) {}
  Table *getlink() {return (Table*)getcomp();}
  void setmodel(zbTableModel *model) {getlink()->setmodel(model);}
//  void setlineColor(string color) {getlink()->setlinecolor(gr::strtonativecolor(color.c_str()));}
//  zbObject *getmodel();
};

struct zbPager:zbControl {
  zbPager(Runtime *rt,zbwProp *prop,STRING intf="Pager.instance"):zbControl(rt,prop,intf) {}
  PageControl *getlink() {return (PageControl*)getcomp();}
};

struct zbTreeSource:zbInstance,TreeSource {
  virtual int getCount()=0;
  virtual string getText()=0;
  virtual bool equalTo(zbTreeSource *right)=0;
  virtual RefBase<zbTreeSource> getNodeAt(int index)=0;
  zbTreeSource(Runtime *rt,STRING intf):zbInstance(rt,intf) {}

  override string gettext() {return getText();}
  override int getsubs() {return getCount();}
  override TreeSrc nodeat(int index) {return (zbTreeSource*)getNodeAt(index);}
  override void addref() {zbObject::addref();}
  override void release() {zbObject::release();}
  override bool equalto(TreeSource *right) {
    if (zbTreeSource *r=dynamic_cast<zbTreeSource*>(right)) return equalTo(r);
    return false;
  }
};

struct zbTree:zbControl {
  zbTree(Runtime *rt,zbwProp *prop,STRING intf="Tree.instance"):zbControl(rt,prop,intf) {}
  Tree *getlink() {return (Tree*)getcomp();}
  void setsource(zbTreeSource *source) {getlink()->setsource(source);}
  zbObject *getactive() {TCntVec<int> res;if (getlink()->getactive(res)) return createMultiIndex(res,rt);return createVoid(rt);}
  void setactive(zbMultiIndex *mi) {getlink()->setactive(mi->items);}
  void expand(zbMultiIndex *idx) {NodeImage *act=getlink()->indextonode(idx->items);act->expand();}
  void collapse(zbMultiIndex *idx) {NodeImage *act=getlink()->indextonode(idx->items);act->collapse();}
};

struct zbPropertyList:zbControl {
  zbPropertyList(Runtime *rt,zbwProp *prop,STRING intf="PropertyList.instance"):zbControl(rt,prop,intf) {}
  PropertyList *getlink() {return (PropertyList*)getcomp();}
  void save(VarList &vars) {getlink()->getvars(&vars);}
  void load(VarList &vars) {getlink()->setvars(&vars,true);}
  void addVar(string name) {getlink()->addvar(name,"",true);}
  void eraseVar() {getlink()->deleteactvar();}
};

struct zbFont:zbObject {
  PtrTo<Font> font;
  zbFont(Runtime *rt,Font *afont,STRING intf="Font.instance"):zbObject(rt,intf) {font=afont;}
//  ~zbFont() {delete font;}
  Font *getlink() {return font;}
  Font *cvalue() {return font;}
  string tostring() {return fmtstr("%s %d",font->getname().c_str(),font->getsize());}
};

struct zbPicture:zbObject {
  RefBase<Picture> picture;
  zbPicture(Runtime *rt,Picture *picture,STRING intf="Picture.instance"):zbObject(rt,intf),picture(picture) {}
  Picture *getlink() {return picture;}
  Picture *cvalue() {return picture;}
  void draw(int x,int y,Canvas *cnv) {picture->blit(Canvas(*cnv,x,y));}
  void stretch(int x,int y,int w,int h,Canvas *cnv) {picture->stretchblt(*cnv,Rect(x,y,w,h),Rect(0,0,picture->getw(),picture->geth()),"copy");}
  void stretchRectangle(int x,int y,int w,int h,int srcx,int srcy,int srcw,int srch,Canvas *cnv) {
    picture->stretchblt(*cnv,Rect(x,y,w,h),Rect(srcx,srcy,srcw,srch),"copy");
  }
  zbCanvas *getcanvas() {return new zbCanvas(rt,picture->getcanvas());}
  zbObject *clone();
};

struct zbKey:zbObject {
  awsKey key;
  zbKey(string name,Runtime *rt,STRING intf="Key.instance"):zbObject(rt,intf) {key.fromstring(name);}
  zbKey(awsKey key,Runtime *rt,STRING intf="Key.instance"):zbObject(rt,intf),key(key) {}
  bool opeq(string name) {awsKey key2;key2.fromstring(name);return key2==key;}
  bool opneq(string name) {return ! opeq(name);}
  string tostring() {return key.tostring();}
  bool getshift() {return key.shift&ks_shift;}
  bool getalt() {return key.shift&ks_alt;}
  bool getctrl() {return key.shift&ks_ctrl;}
  bool getlshift() {return key.shift.teststate&kst_lshift;}
  bool getrshift() {return key.shift.teststate&kst_rshift;}
  string getkey();
};

struct zbShiftState:zbObject {
  ShiftState shift;
  zbShiftState(ShiftState ashift,Runtime *rt,STRING intf="ShiftState.instance"):zbObject(rt,intf) {
    shift=ashift;
  }
  bool getshift() {return shift&ks_shift;}
  bool getalt() {return shift&ks_alt;}
  bool getctrl() {return shift&ks_ctrl;}
  bool getlshift() {return shift.teststate&kst_lshift;}
  bool getrshift() {return shift.teststate&kst_rshift;}
};

/*
struct zbPageable:zbComponent {
  zbPrinter(Runtime *rt,zbwProp *prop,STRING intf="Pageable.instance"):zbComponent(rt,prop,intf) {
  }
  Printer *getlink() {return (Printer*)getcomp();}
  void beginDoc(string name) {getlink()->begindoc(name,"");}
  void destroy() {delete getlink();}
//  Canvas getInfo() {return getlink()->getinfo();}
};
*/

struct zbPrinter:zbComponent {
  zbPrinter(Runtime *rt,zbwProp *prop,STRING intf="Printer.instance"):zbComponent(rt,prop,intf) {
  }
  Printer *getlink() {return (Printer*)getcomp();}
  Printer *cvalue() {return (Printer*)getcomp();}
  void beginDoc(string name) {getlink()->begindoc(name,"");}
  void destroy() {getlink()->destroy();}
//  Canvas getInfo() {return getlink()->getinfo();}
};

struct zbPageDistribution:zbObject {
  RealPages pages;
  PtrTo<PageDistrib> pd;
  zbPageDistribution(Runtime *rt,STRING intf="PageDistribution.instance"):zbObject(rt,intf) {}
  int getsheetCount() {return pd->sheetcnt();}
  int getfreePageCount() {return pd->freepgcnt();}
  void draw(int page,zbCanvas *cnv);
  void distrib(LogPages &lpages,Array<int> &ofsx,Array<int> &ofsy);
  void distribLines(LogPages &lpages,Array<int> &ofsx,Array<int> &ofsy);
};

class zbLogPage;
struct zbLogPages:zbObject {
  RefBase<LogPages> pages;
  zbLogPages(Runtime *rt,STRING intf="LogPages.instance"):zbObject(rt,intf) {pages=new LogPages();}
  LogPages *cvalue() {return pages;}
  LogPages *getlink() {return pages;}
  zbObject *complete(zbArray *ofsx,zbArray *ofsy);
  zbObject *completeLines(zbArray *ofsx,zbArray *ofsy);
  zbLogPage *getpage(int index);
  zbLogPage *addPage() {pages->addpage();return getlast();}
  int getcount() {return pages->pages.count;}
  zbLogPage *getlast() {return getpage(pages->pages.count-1);}
};

class zbPane;

struct zbLogPage:zbObject {
  RefBase<LogPages> logpages;
  int index;
  zbLogPage(Runtime *rt,LogPages *pages,int index,STRING intf="LogPage.instance"):zbObject(rt,intf),logpages(pages),index(index) {}
  LogPage *cvalue() {return getlink();}
  LogPage *getlink() {return logpages->pages[index];}
  int getcount() {return getlink()->panes.count;}
  zbPane *getpane(int index);
  void draw(zbCanvas *cnv) {getlink()->draw(cnv->cnv);}
};
inline zbLogPage *zbLogPages::getpage(int index) {return new zbLogPage(rt,pages,index);}


struct zbPaneGrp:zbObject {
  RefBase<PaneGrp> panegrp;
  zbPaneGrp(Runtime *rt,PaneGrp *apanegrp=NULL,STRING intf="PaneGrp.instance"):zbObject(rt,intf) {
    if (apanegrp) panegrp=apanegrp;else panegrp=new PaneGrp();
  }
  PaneGrp *cvalue() {return panegrp;}
  PaneGrp *getlink() {return panegrp;}
  zbObject *getcanvas() {return new zbCanvas(rt,panegrp->getcnv());}
  void beginDoc(zbCanvas *cnv) {panegrp->begindoc(cnv->cnv.cnv);}
  int getcount() {return panegrp->panes.count;}
  zbPane *getpane(int index);
  int sheetCount(int pagehi) {return 0;}//panegrp->sheetcount(pagehi);}
  void replaceText(string oldtext,string newtext) {
    for(int i=0;i<panegrp->panes.count;i++)
      panegrp->panes[i]->meta.replacetext(oldtext,newtext);
  }
};
struct zbPane:zbObject {
  RefBase<Pane> pane;
  Pane *cvalue() {return pane;}
  Pane *getlink() {return pane;}
  zbPane(Runtime *rt,Pane *pane,STRING intf="Pane.instance"):zbObject(rt,intf),pane(pane) {}
  zbPaneGrp *getparent() {return new zbPaneGrp(rt,pane->panegrp);}
  zbCanvas *getcanvas() {return new zbCanvas(rt,Canvas(&pane->meta,0,0));}
};
inline zbPane *zbPaneGrp::getpane(int index) {return new zbPane(rt,panegrp->panes[index]);}
inline zbPane *zbLogPage::getpane(int index) {return new zbPane(rt,getlink()->panes[index]);}


struct zbMetaCanvas:zbObject {
  MetaCanvas meta;
  zbMetaCanvas(Runtime *rt,STRING intf="MetaCanvas.instance"):zbObject(rt,intf) {}
  void draw(zbCanvas *cnv) {meta.draw(cnv->cnv);}
  zbObject *getcanvas() {return new zbCanvas(rt,Canvas(&meta,0,0));}
  void replaceText(string oldtext,string newtext) {meta.replacetext(oldtext,newtext);}
};

struct zbTextCanvas:zbObject {
  TextCanvas cnv;
  zbTextCanvas(zbCanvas *cnv,int wi,int pagehi,Runtime *rt,STRING intf="TextCanvas.instance"):zbObject(rt,intf),cnv(cnv->cnv,wi,pagehi) {}
  TextCanvas *getlink() {return &cnv;}
  void text(string s,Interpreter *ir) {if (!cnv.text(s)) ir->nativeerror("TextCanvasPage");}
  void textElem(int space,string s,Interpreter *ir) {if (!cnv.textelem(space,s)) ir->nativeerror("TextCanvasPage");}
  void textElemX(int space,int x,string s,Interpreter *ir) {if (!cnv.textelemx(space,x,s)) ir->nativeerror("TextCanvasPage");}
};

struct zbResizeCanvas:zbObject {
  ResizeCanvas resize;
  int dx,dy;
  zbResizeCanvas(Runtime *rt,Canvas cnv,float kx,float ky,STRING intf="ResizeCanvas.instance"):zbObject(rt,intf),resize(cnv.cnv,kx,ky) {
    dx=cnv.dx/kx;dy=cnv.dy/ky;
  }
  zbObject *getcanvas() {return new zbCanvas(rt,Canvas(&resize,dx,dy));}
};

struct zbSubCanvas:zbObject {
  Canvas cnv;
  zbSubCanvas(Runtime *rt,Canvas cnv,int dx,int dy,STRING intf="SubCanvas.instance"):zbObject(rt,intf),cnv(cnv,dx,dy) {
  }
  zbObject *getcanvas() {return new zbCanvas(rt,cnv);}
};

struct zbOpenDialog:zbObject {
  PtrTo<OpenDialog> dialog;
  zbOpenDialog(Runtime *rt,STRING intf="OpenDialog.instance"):zbObject(rt,intf) {
    dialog=awssys->createopendialog();
  }
  OpenDialog *getlink() {return dialog;}
};

struct zbSaveDialog:zbObject {
  PtrTo<SaveDialog> dialog;
  zbSaveDialog(Runtime *rt,STRING intf="SaveDialog.instance"):zbObject(rt,intf) {
    dialog=awssys->createsavedialog();
  }
  SaveDialog *getlink() {return dialog;}
};

struct zbWaitWindow:zbComponent {
  zbWaitWindow(Runtime *rt,zbwProp *prop,STRING intf="WaitWindow.instance"):zbComponent(rt,prop,intf) {
  }
  WaitWindow *getlink() {return (WaitWindow*)getcomp();}
  void destroy() {getlink()->destroy();}
};

struct zbHTMLView:zbControl {
  zbHTMLView(Runtime *rt,zbwProp *prop,STRING intf="HTMLView.instance"):zbControl(rt,prop,intf) {}
  HTMLView *getlink() {return (HTMLView*)getcomp();}
  void searchedPicture(string src,zbPicture *pic) {getlink()->searchedpicture(src,pic->picture);}
};

struct zbCheckList:zbControl {
  zbCheckList(Runtime *rt,zbwProp *prop,STRING intf="CheckList.instance"):zbControl(rt,prop,intf) {}
  CheckList *getlink() {return (CheckList*)getcomp();}
};

struct zbMidiOut:zbObject {
  static const int NOTEON=144,NOTEOFF=128,PROGRAMCHANGE=0xC0;
  PtrTo<MidiOut> mo;
  zbMidiOut(Runtime *rt,STRING intf="MidiOut.instance"):zbObject(rt,intf) {
  }
  void open(Interpreter *ir) {
    mo=awssys->openmidiout();
    if (!mo) ir->nativeerror("Midi device cannot be opened"); 
  }
  void close() {mo=NULL;}
  MidiOut *getlink() {return mo;}
  void noteOn(int channel,int midihi,int velocity) {mo->message(NOTEON+channel,midihi,velocity);}
  void noteOff(int channel,int midihi,int velocity) {mo->message(NOTEOFF+channel,midihi,velocity);}
  void setInstrument(int channel,int instrumentId) {mo->message(PROGRAMCHANGE+channel,instrumentId,0);}
};

struct zbApplicationClass:zbComponent,zbssData {
  zbObject *handler;
  aws::System *getlink() {return awssys;}
  zbApplicationClass(Runtime *rt,zbwProp *prop,STRING intf="ApplicationClass.instance"):zbComponent(rt,prop,intf) {handler=NULL;}
  void on_do(BaseEvent &event);
  void sethandler(zbObject *hnd) {if (handler) handler->release();handler=hnd->addref();}
  zbObject *gethandler() {if (handler) return handler->addref();return createVoid(rt);}
  ~zbApplicationClass() {if (handler) handler->release();}
  void destroy() {getlink()->destroy();}
  void runapp(Interpreter *ir);
  void loadresource(string files);
  XMLNode getopts();
  void addSearchPath(string file) {awssys->searchpath.add(file);}
  void setOptionFile(string file) {awssys->setoptionfile(file);}
  bool runxmldialog(XMLNode dialog,XMLNode data);
  bool runsettingsdialog(string dialog,XMLNode data);
  zbObject *findresource(string name,Interpreter *ir);
  zbObject *getsysmenu(Runtime *rt);
  zbObject *getshiftState();
  void setsysmenu(zbObject *menu);
//  string getonIdle() {return awssys->onidle.value;}
//  void setonIdle(string value) {awssys->onidle.value=value;}
};

void initawsrt(Runtime &rt);

zbObject *createComponent(Component *comp,Runtime *rt);

}

#endif
