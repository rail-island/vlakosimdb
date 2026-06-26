#include <winlib.h>
#pragma hdrstop

#include "awwsystem.h"
#include "awwcontrol.h"
#include "awwstdctrls.h"
#include "awwcomctrls.h"
#include "awwextctrls.h"
#include "awwwindow.h"
//#include <win/w32menu.h>
#include <win/wsystem.h>
#include "awwpaint.h"
#include <win/w32init.h>
//#include "w32bmps.h"
#include <win/w32keys.h>
#include "awwmenu.h"
#include "awwprint.h"
#include "awwhint.h"
#include "awwdlgs.h"

namespace aww {

static LRESULT CALLBACK awwObject_wndproc(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam) {
  awwObject *obj=ctrlfromhandle(hwin);
  return obj?obj->wndproc(hwin,msg,wparam,lparam):DefWindowProc(hwin,msg,wparam,lparam);
}

struct awwStdConsole:Console {
  void print(string s) {
    AllocConsole();
    HANDLE fout=GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD writted=0;
    WriteFile(fout,s.c_str(),s.length(),&writted,NULL);
  }
  string readln() {
    AllocConsole();
    HANDLE fin=GetStdHandle(STD_INPUT_HANDLE);
    char buf[0x1000];
    DWORD read=0;
    ReadFile(fin,buf,0xFFF,&read,NULL);
    buf[read]=0;
    trimr(buf);
    return string(buf);
  }
  void wait() {readln();}
  void close() {FreeConsole();}
  awwStdConsole(System *system):Console(system,system) {}
};

/*
void awwSystem::puts(string s) {
}
string awwSystem::gets() {
}
void awwSystem::showconsole() {
  AllocConsole();
}
void awwSystem::hideconsole() {
  FreeConsole();
}
*/

HCURSOR awwSystem::findcursor(Cursor type) {
  switch (type) {
    case cr_arrow:return wcr_arrow;
    case cr_cross:return wcr_cross;
    case cr_beam:return wcr_beam;
    case cr_wait:return wcr_wait;
    case cr_hsplit:return wcr_hsplit;
    case cr_vsplit:return wcr_vsplit;
    case cr_hand:return wcr_hand;
    case cr_default:return wcr_arrow;
  }
  return NULL;
}

awwSystem::awwSystem() {
//  acttimer=0;
  sysopt->canwindow=true;
  sysopt->canedit=true;
  sysopt->canlistbox=true;
  sysopt->canpackedlist=true;
  sysopt->canbutton=true;
  sysopt->canplace=true;
  sysopt->canpager=true;
  sysopt->canlabel=true;
  sysopt->cancheckbox=true;
  sysopt->canradiogroup=true;
  sysopt->cangroupbox=true;
  sysopt->canscrollbox=true;
  sysopt->canmemo=true;
  sysopt->canwraplabel=true;
  sysopt->canpaintcontrol=true;
  sysopt->canscrolledpainter=true;
  sysopt->cantrackbar=true;
  sysopt->cantimer=true;
  sysopt->canmenuitem=true;
  wcr_arrow=LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW));
  wcr_cross=LoadCursor(NULL,MAKEINTRESOURCE(IDC_CROSS));
  wcr_beam=LoadCursor(NULL,MAKEINTRESOURCE(IDC_IBEAM));
  wcr_wait=LoadCursor(NULL,MAKEINTRESOURCE(IDC_WAIT));
  wcr_hsplit=LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE));
  wcr_vsplit=LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENS));
  wcr_hand=LoadCursor(W_hInstance,"HANDGRAB");
  terminated=false;mainwindow=NULL;
  checkw32(registerwindowclass("awwWindow",awwObject_wndproc));
  checkw32(registerwindowclass("awwWindow_nobrush",awwObject_wndproc,"[nobrush]"));
//  editdefwndproc=registersubclass("EDIT","awwEdit");
//  windowdefwndproc=DefWindowProc;
  stdwndproc=awwObject_wndproc;
  fixedfont=new awwFont(new WinFont(&fontman,(HFONT)GetStockObject(ANSI_FIXED_FONT)),this);
  varfont=new awwFont(new WinFont(&fontman,(HFONT)GetStockObject(ANSI_VAR_FONT)),this);
  processedwin=NULL;
  createhandle();
  HDC maindc=GetDC(0);
  logpixelsx=GetDeviceCaps(maindc,LOGPIXELSX);
  logpixelsy=GetDeviceCaps(maindc,LOGPIXELSY);
  ReleaseDC(0,maindc);
  prndrv=new awwPrintDriver(this);
  stdcon=new awwStdConsole(this);
  hintwin=new awwHintWindow(this);
}

void awwSystem::showhint(string text,awwControl *parent) {
  hintwin->show(parent,text);
}
void awwSystem::hidehint(awwControl *parent) {
  hintwin->hide();
}


Printer *awwSystem::createprinter() {
  return new awwPrinter(this,this);
}

awwSystem::~awwSystem() {
  delete varfont;delete fixedfont;delete hintwin;
}

static VOID CALLBACK awwTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime) {
  ((Timer*)idEvent)->ontimer();
}

struct awwSimpleTimer {
  SimpleTimerEvent *event;
  awwSystem *system;
  awwSimpleTimer(SimpleTimerEvent *event,awwSystem *system):event(event),system(system) {}
  void run() {
    event->event_work();
    KillTimer(system->gethandle(),(UINT)this);
    delete this;
//    SimpleTimerEvent *event
  }
};

static VOID CALLBACK awwSimpleTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime) {
  ((awwSimpleTimer*)idEvent)->run();
}

struct awwTimer:Timer {
  bool ishandle;
  awwSystem *system;
  int milis;
  UINT htimer() {return (UINT)(Timer*)this;}
  void kill() {if (ishandle) KillTimer(system->gethandle(),htimer());ishandle=false;}
  void wanttimer() {
    if (!ishandle) SetTimer(system->gethandle(),htimer(),milis,(TIMERPROC)awwTimerProc),ishandle=true;
  }
  void setinterval(float secs) {milis=secs*1000;if (ishandle) restart();}
  float getinterval() {return milis/1000.0;}
  void stop() {kill();}
  void restart() {
    kill();wanttimer();
  }
  bool isrunning() {return ishandle;}
  awwTimer(Component *owner,awwSystem *system):Timer(owner,system),system(system) {ishandle=false;milis=1000;}
  ~awwTimer() {kill();}
};

Timer *awwSystem::createtimer(Component *owner) {
  return new awwTimer(owner,this);
}

void awwSystem::createsimpletimer(SimpleTimerEvent *event,float secs) {
  SetTimer(gethandle(),(UINT)(new awwSimpleTimer(event,this)),secs*1000,(TIMERPROC)awwSimpleTimerProc);
}

MenuItem *awwSystem::createmenuitem(Component *owner) {
  return new awwMenuItem(owner,this);
}

HANDLE awwSystem::getparentwin() {
  return processedwin?(HANDLE)processedwin->win:(HANDLE)win;
}

Font *awwSystem::createfont() {return new awwFont(varfont->wf,this);}
Font *awwSystem::getsystemfont(bool canbeproportional) {
  return canbeproportional?varfont:fixedfont;
}

static string getwinerrormsg(int errcode) {
  LPVOID lpMsgBuf;
  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPTSTR) &lpMsgBuf,
    0,
    NULL
  );
  string res=(char*)lpMsgBuf;
  res.trimr();
  LocalFree( lpMsgBuf );
  return res;
}
void throwlastwin32error() {
  throw awsError(getwinerrormsg(GetLastError()));
}

void awwSystem::quit() {
  PostMessage(win,WM_CLOSE,0,0);
}

/*
WNDPROC awwSystem::registersubclass(const char *classname,const char *subclassname) {
  WNDCLASS wc;
  checkw32(GetClassInfo(NULL,classname,&wc));
  wc.lpszClassName=subclassname;
  WNDPROC res=wc.lpfnWndProc;
  wc.lpfnWndProc=awwControl_wndproc;
  wc.hInstance=W_hInstance;
  checkw32(RegisterClass(&wc));
  return res;
}
*/

OpenDialog *awwSystem::createopendialog() {return new awwOpenDialog(this);}
SaveDialog *awwSystem::createsavedialog() {return new awwSaveDialog(this);}
ColorDialog *awwSystem::createcolordialog() {return new awwColorDialog(this);}

void awwSystem::createhandle() {
  WNDCLASS wc;
  zerorec(wc);
  wc.lpfnWndProc=awwObject_wndproc;
  wc.lpszClassName="awwSystem";
  wc.hInstance=W_hInstance;
  RegisterClass(&wc);
  win=CreateWindow(wc.lpszClassName,"",WS_VISIBLE|WS_POPUP|WS_CAPTION|WS_CLIPSIBLINGS|WS_SYSMENU|WS_MINIMIZEBOX,
  GetSystemMetrics(SM_CXSCREEN)/2,GetSystemMetrics(SM_CYSCREEN)/2,0,0,0,0,W_hInstance,NULL);
  objtohandle(win,this);
}

void awwSystem::minimize() {
  ShowWindow(win,SW_MINIMIZE);
}

const int POPUP_BEGIN=0x400;

int awwSystem::showpopupmenu(string items,int x,int y) {
  HMENU menu=CreatePopupMenu();
  int act=POPUP_BEGIN;
  for(lineiter it=items.getlines(';');*it;it++) {
    if (eqstr(*it,"-")) AppendMenu(menu,MF_SEPARATOR,0,0);
    else AppendMenu(menu,MF_STRING,act,*it),act++;
  }
  popupresult=-1;
  TrackPopupMenu(menu,0,x,y,0,win,NULL);
  answer_noidle();
  DestroyMenu(menu);
  return (popupresult<0)?(-1):(popupresult-POPUP_BEGIN);
//  while (popupresult<0) answer();
//  return popupresult-POPUP_BEGIN;
}
int awwSystem::showpopupmenu(string items) {
  POINT p;
  GetCursorPos(&p);
  return showpopupmenu(items,p.x,p.y);
}

RealWindow *awwSystem::createWindow(Window *win) {return new awwWindow(win);}
RealEdit *awwSystem::createEdit(Edit *edit) {return new awwEdit(edit);}
RealListBox *awwSystem::createListBox(ListBox *list) {return new awwListBox(list);}
RealPackedList *awwSystem::createPackedList(PackedList *packedlist) {return new awwPackedList(packedlist);}
RealButton *awwSystem::createButton(Button *button) {return new awwButton(button);}
RealPlace *awwSystem::createPlace(Place *place) {return new awwPlace(place);}
RealPageControl *awwSystem::createPageControl(PageControl *pager)  {return new awwPageControl(pager);}
RealPageSheet *awwSystem::createPageSheet(PageControl *owner,PageSheet *page) {return new awwPageSheet(owner,page);}
RealLabel *awwSystem::createLabel(Label *label) {return new awwLabel(label);}
RealCheckBox *awwSystem::createCheckBox(CheckBox *checkbox) {return new awwCheckBox(checkbox);}
RealRadioGroup *awwSystem::createRadioGroup(RadioGroup *radiogroup) {return new awwRadioGroup(radiogroup);}
RealGroupBox *awwSystem::createGroupBox(GroupBox *groupbox) {return new awwGroupBox(groupbox);}
RealScrollBox *awwSystem::createScrollBox(ScrollBox *scrollbox) {return new awwScrollBox(scrollbox);}
RealMemo *awwSystem::createMemo(Memo *memo) {return new awwMemo(memo);}
RealWrapLabel *awwSystem::createWrapLabel(WrapLabel *label) {return new awwWrapLabel(label);}
RealPaintControl *awwSystem::createPaintControl(PaintControl *paintcontrol) {return new awwPaintControl(paintcontrol);}
RealScrolledPainter *awwSystem::createScrolledPainter(ScrolledPainter *scrolledpainter) {return new awwScrolledPainter(scrolledpainter);}
RealTrackBar *awwSystem::createTrackBar(TrackBar *trackbar) {return new awwTrackBar(trackbar);}

bool awwSystem::processmessage() {
  MSG msg;
  if (PeekMessage(&msg,0,0,0,PM_REMOVE)) {
    if (msg.message==WM_QUIT) {terminated=true;return true;}
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return true;
  }
  return false;
}

string awwSystem::getclipboardtext() {
  string res;
  if (!OpenClipboard(win)) return res;
  HANDLE hmem=GetClipboardData(CF_TEXT);
  if (hmem) {
    char *s=(char*)GlobalLock(hmem);
    if (!s) {CloseClipboard();return res;}
    res=s;
    GlobalUnlock(hmem);
  }
  CloseClipboard();
  return res;
}

void awwSystem::setclipboardtext(string value) {
  if (!OpenClipboard(win)) return;
  HANDLE data=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,value.length()+1);
  void *ptr=GlobalLock(data);
  strcpy((char*)ptr,value.c_str());
  GlobalUnlock(data);
  SetClipboardData(CF_TEXT,data);
  CloseClipboard();
}

void awwSystem::dispatch_mouse_enter_leave() {
  POINT p;GetCursorPos(&p);
  HANDLE top=WindowFromPoint(p),act=top;
  mspom.clear();
  if (act and act!=gethandle()) {
    mspom.add(act);act=GetParent(act);
  }
  int outindex=mousestack.count-1,inindex=mspom.count-1;
  while (outindex>=0 && inindex>=0 && mousestack[outindex]==mspom[inindex]) outindex--,inindex--;
  for(int i=0;i<=outindex;i++) if (awwControl *ctrl=ctrlfromhandle(mousestack[i])) ctrl->onmouseleave();
  for(int i=0;i<=inindex;i++) if (awwControl *ctrl=ctrlfromhandle(mspom[i])) ctrl->onmouseenter();
  mousestack.assign(mspom);
}

void awwSystem::sleep(float secs) {
  int ms=secs*1000;
  if (ms>0) Sleep(ms);
}

void awwSystem::sleepms(int ms) {
  if (ms>0) Sleep(ms);
}

bool awwSystem::answer_noidle() {
  bool res=false;
  retry:
  try {
    while (processmessage()) res=true;
  } catch (JException &error) {
    error.show();
    goto retry;
  }
  dispatch_mouse_enter_leave();
  destroyplan();
  return res;
}

bool awwSystem::answer() {
  bool wasmsg=answer_noidle();
  if (!wasmsg) onidle();
  return wasmsg;
}

void awwSystem::run() {
  while (!terminated) answer();
}

float awwSystem::getppmmx() {
  return logpixelsx/25.4;
}
float awwSystem::getppmmy() {
  return logpixelsy/25.4;
}


/*
MenuBar *awwSystem::loadmenu(Component *owner,XMLNode xml) {
  awwMenuBar *res=new awwMenuBar(owner,this);
  res->load(xml);
  return res;
}
*/

LRESULT awwSystem::wndproc(HWND hwin,UINT nmsg,WPARAM wparam,LPARAM lparam) {
  awwMessage msg(hwin,nmsg,wparam,lparam);
  switch (nmsg) {
    case WM_CLOSE:PostQuitMessage(0);break;
    case WM_COMMAND:popupresult=LOWORD(wparam);break;
//    case WM_TIMER:((awwTimer*)lparam)->ontimer();msg.handled=true;break;
  }
  defproc(msg);
  return msg.res;
}

struct awwMidiOut:MidiOut {
  HMIDIOUT hmo;
  awwMidiOut() {hmo=NULL;lasterror=MMSYSERR_NOERROR;}
  int lasterror;
  bool open() {
    hmo=NULL;
    lasterror=midiOutOpen(&hmo,MIDI_MAPPER,NULL,0,CALLBACK_NULL);
//    lasterror=midiOutOpen(&hmo,0,NULL,0,CALLBACK_NULL);
    if (lasterror!=MMSYSERR_NOERROR) hmo=NULL;
    return hmo;
  }
  bool message(int status,int param1,int param2) {
    if (!hmo) {
      lasterror=MMSYSERR_INVALHANDLE;
      return false;
    }
    union{
      struct {
        unsigned int status:8,param1:8,param2:8,nil:8;
      };
      unsigned int value;
    } m;
    m.status=status;
    m.param1=param1;
    m.param2=param2;
    m.nil=0;
    lasterror=midiOutShortMsg(hmo,m.value);
    return lasterror==MMSYSERR_NOERROR;
  }
  void close() {
    lasterror=midiOutClose(hmo);
    hmo=NULL;
  }
  ~awwMidiOut() {close();}
};

MidiOut *awwSystem::openmidiout() {
  awwMidiOut *mo=new awwMidiOut();
  if (mo->open()) return mo;
  delete mo;
  return NULL;
}

Picture *awwSystem::createpicture() {return new awwPicture();}

//#delbeg [ABAZ]
//zbuf::Bitmap *awwSystem::loadbitmap(const char *file,int bits) {
//  return awwPicture::loadbitmap(file,bits);
/*  wlib::DIBitmap bmp;
  bmp.load(file);
  return bmp.tocbitmap(colordepth>0?colordepth:bmp.getbits());*/
//}
//#delend [ABAZ]


}

void anal_args(char *copy,TCntVec<char*> &args) {
  char *c=copy;
  while (*c) {
    jmpspaces(c);
    bool uv=false;
    if (*c=='"') c++,uv=true;
    args.add(c);
    if (uv) {
      while (*c && *c!='"') c++;
      if (*c) *c=0,c++;
    } else {
      while (*c && *c!=' ') c++;
      if (*c) *c=0,c++;
    }
  }
  args.add(NULL);
}

struct awwWinSystem:wlib::WSystem {
  aww::awwSystem *sys;
  HANDLE getactparent() {return sys->getparentwin();}
  awwWinSystem() {sys=NULL;}
};

void errormsg(const char *msg) {
  jsystem->showerror(msg);
}

//namespace System {
//  extern void __fastcall (*InitProc)();
//};

int winmain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  awwWinSystem *wsys=new awwWinSystem();
  char buf[0x200];
  GetModuleFileName(0,buf,sizeof(buf));
  basedir=buf;basedir--;

  showXErrorMessage=errormsg;

  wlib::w32_init(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
  init_lang();

  try {
    aww::init_awwobject_atom();
    aww::awwSystem sys;
    wsys->sys=&sys;
    TCntVec<char*> args;
    char *copy=strnew(lpCmdLine);
    anal_args(copy,args);
    sys.argc=args.count-1;
    sys.argv=args.buf;
    aws::awsmain(&sys,args.count-1,args.buf);
    aww::delete_awwobject_atom();
  } catch(aws::awsError &err) {
    jsystem->showerror(err.msg);
  }

  wlib::w32_done();
  return 0;
}

WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  try {
    return winmain(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
  } catch (jlib::JException &e) {
    MessageBox(0,e.getmsg().c_str(),"Fatal error",MB_OK | MB_ICONERROR);
  } catch (...) {
    MessageBox(0,"Fatal unknown error","Fatal error",MB_OK | MB_ICONERROR);
  }
}



