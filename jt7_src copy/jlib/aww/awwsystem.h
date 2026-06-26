#ifndef awwsystemH
#define awwsystemH

#include <aws/awssystem.h>
//#include <win/w32menu.h>
#include "awwobject.h"
#include "awwfont.h"

namespace aww {
using namespace aws;
class awwFont;class awwPrintDriver;

/*
struct awwMenuBar:aws::MenuBar,wlib::MenuHandler {
  wlib::winMenuBar menu;
  awwMenuBar(Component *owner,System *system):aws::MenuBar(owner,system) {menu.handler=this;}//handler=NULL;}
  void load(XMLNode xml) {menu.load(xml);}
  void run_menu_command(const char *name) {
    docommand(name);
  }
};
*/


class awwWindow;class awwHintWindow;
struct awwSystem:System,awwObject {
  awwWindow *processedwin;//okno, ktereho zprava se zpracovava

  RealWindow *createWindow(Window *win);
  RealEdit *createEdit(Edit *edit);
  RealListBox *createListBox(ListBox *list);
  RealPackedList *createPackedList(PackedList *packedlist);
  RealButton *createButton(Button *button);
  RealPlace *createPlace(Place *place);
  RealPageControl *createPageControl(PageControl *pager);
  RealPageSheet *createPageSheet(PageControl *owner,PageSheet *page);
  RealLabel *createLabel(Label *label);
  RealCheckBox *createCheckBox(CheckBox *checkbox);
  RealRadioGroup *createRadioGroup(RadioGroup *radiogroup);
  RealGroupBox *createGroupBox(GroupBox *groupbox);
  RealScrollBox *createScrollBox(ScrollBox *scrollbox);
  RealMemo *createMemo(Memo *memo);
  RealWrapLabel *createWrapLabel(WrapLabel *label);
  RealPaintControl *createPaintControl(PaintControl *paintcontrol);
  RealScrolledPainter *createScrolledPainter(ScrolledPainter *scrolledpainter);
  RealTrackBar *createTrackBar(TrackBar *trackbar);
  Console *getstdcon() {return stdcon;}

  Timer *createtimer(Component *owner);
/*  void showconsole();
  void hideconsole();
  string gets();
  void puts(string s);*/

  bool answer();//vrati, zda neco bylo udelano
  bool answer_noidle();//vrati, zda neco bylo udelano
  awwSystem();
  ~awwSystem();
  WNDPROC stdwndproc;
  void run();
  MenuItem *createmenuitem(Component *owner);
  Printer *createprinter();
  void quit();
  Font *createfont();
  Font *getsystemfont(bool canbeproportional=true);
  HWND gethandle() {return win;}
  string gettitle() {return getwndtext();}
  void settitle(string value) {WControl wc(gethandle());wc.settext(value.c_str());}
  LRESULT wndproc(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam);

  int showpopupmenu(string items,int x,int y);
  int showpopupmenu(string items);

  void createsimpletimer(SimpleTimerEvent *event,float secs);
  MidiOut *openmidiout();
  void setmainwindow(aws::Window *value) {if (value->canbemain()) mainwindow=value;}
  aws::Window *getmainwindow() {return mainwindow;}
  aws::Window *mainwindow;
  void createdwindow(aws::Window *win) {if (!mainwindow && win->canbemain()) mainwindow=win;}
  void closedwindow(aws::Window *win) {if (mainwindow==win) PostQuitMessage(0);}
  bool isterminated() {return terminated;}
  string getclipboardtext();
  void setclipboardtext(string value);
  void sleep(float secs);
  void sleepms(int ms);
  void showhint(string text,awwControl *parent);
  void hidehint(awwControl *parent);
  void minimize();
  float getppmmx();
  float getppmmy();
  OpenDialog *createopendialog();
  SaveDialog *createsavedialog();
  ColorDialog *createcolordialog();
  HCURSOR findcursor(Cursor type);

  Picture *createpicture();
//#delbeg [ABAZ]
//  zbuf::Bitmap *loadbitmap(const char *file,int colordepth);
//#delend [ABAZ]

//  int gettimerid() {if (acttimer==0) acttimer++;return acttimer++;}
  HANDLE getparentwin();
  WinFontMan fontman;
  int logpixelsx,logpixelsy;
private:
  HCURSOR wcr_arrow,wcr_cross,wcr_beam,wcr_wait,wcr_hsplit,wcr_vsplit,wcr_hand;
  Console *stdcon;
  awwHintWindow *hintwin;
  awwPrintDriver *prndrv;
  int popupresult;
  TCntVec<HANDLE> mousestack,mspom;
  void createhandle();
  void dispatch_mouse_enter_leave();
//  HWND handle;
  awwFont *varfont,*fixedfont;
  bool terminated;
  bool processmessage();//vrati, zda nejakou zpravu zpracovalo
  WNDPROC registersubclass(const char *classname,const char *subclassname);
//  UINT acttimer;
};

void throwlastwin32error();
inline void checkw32(bool res) {if (!res) throwlastwin32error();}
inline void checkw32i(int res) {if (res<=0) throwlastwin32error();}

}

#endif
