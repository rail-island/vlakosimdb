#ifndef sdwsystemH
#define sdwsystemH

#include "sdwcontrol.h"
#include <aws/awssystem.h>
#include "sdwcontainer.h"
#include "sdwopts.h"
#include "sdwmenu.h"
#include "sdwsysext.h"
#include <com/ptrto.h>

namespace sdw {
class sdwWindow;

struct sdwHint {
  int id;
  int x,y,w,h;
  string msg;
  SDL_Rect getrect() {return SDLRect(x,y,w,h);}
  sdwHint(int id,string msg):id(id),msg(msg) {x=y=w=h=0;}
  void draw(const sdwCanvas &cnv);
  bool match(int aid) {return id==aid;}
};

struct sdwSystem:System,sdwSystemExtension {
  sdwSystem();
  ~sdwSystem();
  bool answer();//vrati, zda neco bylo udelano
  void run();
  void quit() {terminated=true;}
  bool isterminated() {return terminated;}
  void invalidate(Rect r);
  void invalidate() {invalidate(Rect(0,0,w,h));}
  void notifyremove(Component *obj);
  void removedwindow(sdwWindow *win);
  void settitle(string value) {title=value;SDL_WM_SetCaption(title.c_str(),"");}
  string gettitle() {return title;}
  void setsysmenu(MenuItem *menu);
  MenuItem *getsysmenu() {return sysmenu.menu;}
  void alert(string msg);
//  void setbgimage(zbuf::PixelMap *bmp,zbuf::PixelFormat &pf);
  void getdesktopopts(DesktopOpts &opts);
  int getdesktopw() {return w;}
  int getdesktoph() {return h;}
  void sethint(int hintid,int y,int x1,int x2,string msg);
  void closehint(int hintid);
  void set_listener(SDLEventListener *alistener) {listener=alistener;}

  bool init();

  RealWindow *createWindow(Window *win);
  RealRadioGroup *createRadioGroup(RadioGroup *grp);
  RealScrollBox *createScrollBox(ScrollBox *scrollbox);
  RealPaintControl *createPaintControl(PaintControl *painter);
  RealScrolledPainter *createScrolledPainter(ScrolledPainter *painter);
  RealLabel *createLabel(Label *label);
  RealWrapLabel *createWrapLabel(WrapLabel *label);
  RealEdit *createEdit(Edit *edit);
  MenuItem *createmenuitem(Component *owner);
  RealTrackBar *createTrackBar(TrackBar *trackbar);
  RealPlace *createPlace(Place *place);

  Picture *createpicture();

  ViewOpts *vo;

  void createdcontrol(sdwControl *ctrl) {controls.add(ctrl);}
  void removedcontrol(sdwControl *ctrl);
  void changedfocus();
  void activatewin(sdwWindow *win);
  void caretreset() {updatecaretpos(true);wantchangecaret=true;}
  int getw() {return w;}
  int geth() {return h-sysmenu.gethi();}
  void repaintdesktop() {invalidate();}
  void getmousepos(int *x,int *y) {SDL_GetMouseState(x,y);}
  void bringtofront(sdwWindow *win);
  Font *createfont();
  Font *getsystemfont(bool canbeproportional);
  void setwantgl(bool value);
  void repaintgl() {if (wantgl) wantrepaintgl=true;}
  void setoptionfile(string file);
#ifndef NOBGBMP
  void setbgimage(zbuf::PixelMap *bmp,zbuf::PixelFormat &pf);
#endif
private:
  bool processmessage();
  void dispatch(SDL_Event &event);
  void repaintifneeded();
  void keyevent(SDL_KeyboardEvent &event);
  void mouseevent(SDL_MouseButtonEvent &event);
  void mousemotion(SDL_MouseMotionEvent &event);
  void _hidecaret();
  void _showcaret();
  void setcaretpos(SDL_Rect rect,bool reset);
  void updatecaretpos(bool reset);
  void wantcaretdatasize(int w,int h);
  sdwControl *getkbfocus();
  void flip() {wantflip=true;}
  void resize(SDL_ResizeEvent &event);
  void activeevent(SDL_ActiveEvent &ev);
  void dorepaintgl();

  int w,h;
  bool wantgl;
  bool wantrepaintgl;
  string title;
  IObjArray<sdwHint> hints;
  ViewOpts viewopts;
  SDL_Surface *screen;
//  sdwControl *kbfocus;
  sdwWindow *focuswin;
  bool terminated;
  bool invalidated;
  bool wantflip;
  Rect invalidatedrect;
  TCntVec<sdwWindow*> windows;
  TCntVec<sdwControl*> controls;
  sdwContainer desktop;
  int bits;
  SDLEventListener *listener;

  int caretdataw,caretdatah;
  SDL_Surface *caretdata;
  int caretcolor;

  SDL_Rect caretpos,showcaretpos;
  bool visiblecaret;
  bool _visiblecaret;
  int carettime0;
  bool wantchangecaret;
  bool blinkcaret();//vrati, zda ma byt v tento cas videt
  void setvideo();

  bool menucapture;
  bool windowscapture;
  sdwMenuBar sysmenu;

#ifndef NOBGBMP
  bool paintedbgbmp;
  zbuf::PixelMap *bgbmp;
  zbuf::PixelFormat bgpf;
#endif
public:
  PtrTo<sdwCommonFont> sysfont,deffont;
};



}

#endif
