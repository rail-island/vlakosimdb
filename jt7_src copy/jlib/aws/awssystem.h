#ifndef awssystemH
#define awssystemH

#include "awscomponent.h"
//#delbeg [ABAZ]
//#include <zbuf/bmpwrap.h>
//#delend [ABAZ]
#include <ext/xmlrep.h>
#include <ext/handleman.h>

namespace aws {
class Control;class RealWindow;class RealEdit;class Window;class Edit;
class RealListBox;class ListBox;class RealPackedList;class PackedList;class RealButton;class Button;
class RealPlace;class Place;class RealPageControl;class PageControl;class PageSheet;class RealPageSheet;
class Creator;class Label;class RealLabel;class CheckBox;class RealCheckBox;
class RadioGroup;class RealRadioGroup;class GroupBox;class RealGroupBox;
class ScrollBox;class RealScrollBox;class RealMemo;class Memo;class RealWrapLabel;class WrapLabel;
class PaintControl;class RealPaintControl;class Picture;class RealScrolledPainter;class ScrolledPainter;
class Font;class MenuItem;class Printer;class Layout;class ParentControl;class TrackBar;class RealTrackBar;

struct awsXMLNotFound:awsError {
  string request;
  awsXMLNotFound(string request):awsError(fmtstr("XML not found:%s",request)) {}
};

struct MidiOut {
  virtual bool message(int status,int param1,int param2)=0;
  virtual ~MidiOut() {}
};

struct SimpleTimerEvent {
  virtual void event_work()=0;
};

struct SystemOpts {
  bool canwindow;
  bool canedit;
  bool canlistbox;
  bool canpackedlist;
  bool canbutton;
  bool canplace;
  bool canpager;
  bool canlabel;
  bool cancheckbox;
  bool canradiogroup;
  bool cangroupbox;
  bool canscrollbox;
  bool canmemo;
  bool canwraplabel;
  bool canpaintcontrol;
  bool canscrolledpainter;
  bool cantrackbar;
  bool cantimer;
  bool canmenuitem;
  bool cansysmenu;

  int checklistindsize;
  SystemOpts();
};

struct System:Component,EventHandler {
  virtual ~System();
  virtual RealWindow *createWindow(Window *win) {throw JError("Cannot create instance of Window");}
  virtual RealEdit *createEdit(Edit *edit) {throw JError("Cannot create instance of Edit");}
  virtual RealListBox *createListBox(ListBox *list) {throw JError("Cannot create instance of ListBox");}
  virtual RealPackedList *createPackedList(PackedList *packedlist) {throw JError("Cannot create instance of PackedList");}
  virtual RealButton *createButton(Button *button) {throw JError("Cannot create instance of Button");}
  virtual RealPlace *createPlace(Place *place) {throw JError("Cannot create instance of Place");}
  virtual RealPageControl *createPageControl(PageControl *pager) {throw JError("Cannot create instance of PageControl");}
  virtual RealPageSheet *createPageSheet(PageControl *owner,PageSheet *page) {throw JError("Cannot create instance of PageSheet");}
  virtual RealLabel *createLabel(Label *label) {throw JError("Cannot create instance of Label");}
  virtual RealCheckBox *createCheckBox(CheckBox *checkbox) {throw JError("Cannot create instance of ChackBox");}
  virtual RealRadioGroup *createRadioGroup(RadioGroup *radiogroup) {throw JError("Cannot create instance of RadioGroup");}
  virtual RealGroupBox *createGroupBox(GroupBox *groupbox) {throw JError("Cannot create instance of GroupBox");}
  virtual RealScrollBox *createScrollBox(ScrollBox *scrollbox) {throw JError("Cannot create instance of ScrollBox");}
  virtual RealMemo *createMemo(Memo *memo) {throw JError("Cannot create instance of Memo");}
  virtual RealWrapLabel *createWrapLabel(WrapLabel *label) {throw JError("Cannot create instance of WarpLabel");}
  virtual RealPaintControl *createPaintControl(PaintControl *paintcontrol) {throw JError("Cannot create instance of PaintControl");}
  virtual RealScrolledPainter *createScrolledPainter(ScrolledPainter *scrolledpainter) {throw JError("Cannot create instance of ScrolledPainter");}
  virtual RealTrackBar *createTrackBar(TrackBar *trackbar) {throw JError("Cannot create instance of TrackBar");}

  virtual Timer *createtimer(Component *owner) {throw JError("Cannot create instance of Timer");}

  virtual void createsimpletimer(SimpleTimerEvent *event,float secs) {throw JError("Cannot run simple timer");}
  virtual Printer *createprinter() {throw JError("Cannot create instance of Printer");}
  virtual bool answer()=0;//ProcessMessages,vrati, zda byla nejaka zprava zpracovana
  virtual void run()=0;
  virtual void quit()=0;
  virtual bool isterminated()=0;
  virtual MenuItem *createmenuitem(Component *owner) {throw JError("Cannot create instance of MenuItem");}
//  virtual MenuBar *loadmenu(Component *owner,XMLNode xml)=0;
  virtual Font *createfont() {throw JError("Cannot create instance of Font");}
  virtual Font *getsystemfont(bool canbeproportional=true) {throw JError("Cannot retrieve system font");}
  virtual string gettitle() {return "";}
  virtual string getclipboardtext() {return "";}
  virtual void setclipboardtext(string value) {}
  virtual void settitle(string value) {}
  virtual void setmainwindow(Window *win) {}
  virtual Window *getmainwindow() {return NULL;}
  virtual int showpopupmenu(string items,int x,int y) {return 0;}//items:oddelene stredniky,
  virtual int showpopupmenu(string items) {return 0;}
  virtual Picture *createpicture() {throw JError("Cannot create instance of Picture");}
  virtual MidiOut *openmidiout() {return NULL;}
  virtual OpenDialog *createopendialog() {throw JError("Cannot create instance of OpenDialog");}
  virtual SaveDialog *createsavedialog() {throw JError("Cannot create instance of SaveDialog");}
  virtual ColorDialog *createcolordialog() {throw JError("Cannot create instance of ColorDialog");}
//#delbeg [ABAZ]
//  virtual zbuf::Bitmap *loadbitmap(const char *file,int colordepth=-1) {throw JError("Cannot create instance of zbug::Bitmap");}
//#delend [ABAZ]
  virtual Console *getstdcon() {throw JError("Cannot create instance of Console");}
  virtual void sleep(float secs) {throw JError("Cannot sleep");}
  virtual void sleepms(int ms) {throw JError("Cannot sleep ms");}
  virtual float getppmmx() {throw JError("Cannot getppmmx()");}
  virtual float getppmmy() {throw JError("Cannot getppmmy()");}
  virtual void setsysmenu(MenuItem *menu) {throw JError("System menu not supported");}
  virtual MenuItem *getsysmenu() {throw JError("System menu not supported");}
  virtual int getdesktopw() {return 0;}
  virtual int getdesktoph() {return 0;}
/*  virtual void showconsole()=0;
  virtual void hideconsole()=0;
  virtual string gets()=0;
  virtual void puts(string s)=0;*/
  //vrati poradi vybrane item, -1 pri stornovani
//  Creator *getcreator() {return creator;}
//  void setcreator(Creator *acreator);
  void addcreator(Creator *creator);
  Control *createcontrol(string name,Component *owner);
  Layout *createlayout(string name,ParentControl *owner);
  System();
  XMLRepositary *getrepos() {return rep;}
  XMLNode getopt() {return options;}
  HandleManager *gethm() {return handleman;}
  XMLNode findrep(string name);
  Strings searchpath;
  virtual void setoptionfile(string file);
  void on_defaulthandler(BaseEvent &event);
  string getidle() {return ev_idle.value;}
  string getdesktopmousedown() {return ev_desktopmousedown.value;}
  string getdesktopmouseup() {return ev_desktopmouseup.value;}
  string getdesktopmousemove() {return ev_desktopmousemove.value;}
  void setdesktopmousedown(string value) {ev_desktopmousedown.value=value;}
  void setdesktopmouseup(string value) {ev_desktopmouseup.value=value;}
  void setdesktopmousemove(string value) {ev_desktopmousemove.value=value;}
  void setidle(string value) {ev_idle.value=value;}
  void ondesktopmousedown(int x,int y,MouseButton button,ShiftState shift);
  void ondesktopmouseup(int x,int y,MouseButton button,ShiftState shift);
  void ondesktopmousemove(int x,int y,ShiftState shift);
  void onidle() {donotify("onidle",ev_idle.get());}
  void destroycomponent(Component *comp) {destroys.add(comp);}
  void notifyremove(Component *comp) {Component::notifyremove(comp);destroys.quickremove(comp);}
  void destroyplan() {while (destroys.count>0) delete destroys[0];}
  void loadlang(string file);

  SystemOpts *sysopt;

  ShiftState shiftstate;

  int argc;
  char **argv;
private:
  EventValue ev_idle,ev_desktopmousedown,ev_desktopmouseup,ev_desktopmousemove;

  SystemOpts systemopts;
  string optionfile;
//  Creator *creator;
  IObjArray<Creator> creators;
  XMLRepositary *rep;
  XMLNode options;
  HandleManager *handleman;
  TCntVec<Component*> destroys;
};

extern void awsmain(System *system,int argc,char **argv);

}

#endif
