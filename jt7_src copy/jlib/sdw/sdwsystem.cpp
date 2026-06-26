#include <jlib.h>
#pragma hdrstop

#include "sdwsystem.h"
#include "sdwwindow.h"
#include "sdwmenu.h"
#include "sdwradiogroup.h"
#include "sdwscroll.h"
#include "sdwpaint.h"
#include "sdwedit.h"
#include "sdwtrackbar.h"
#include <gr/comgr.h>
#include "sdwlabel.h"
#include <opengl.h>

namespace sdw {

static bool fullscreen=false;

extern string JSDL_ListFiles(jlib::STRING directory);
extern string JSDL_ListDirs(jlib::STRING directory);
extern void JSDL_ListFiles(jlib::STRING directory,TCntVec<FileRec> &files);
extern void JSDL_InitBaseDir(int argc,char **argv);
extern void JSDL_GetDisplaySize(int *w,int *h);

sdwSystem::sdwSystem():sysmenu(this) {
  bits=32;
  terminated=false;
  invalidated=false;
  screen=NULL;
  focuswin=NULL;
  w=h=0;vo=&viewopts;
  caretdataw=caretdatah=0;
  caretdata=NULL;
  visiblecaret=false;
  caretcolor=clnblack;
  wantflip=false;
  wantgl=false;
  wantrepaintgl=false;
  listener=NULL;
  #ifndef NOBGBMP
  bgbmp=NULL;
  paintedbgbmp=false;
  #endif
  _visiblecaret=false;
  wantchangecaret=false;
  menucapture=false;
  windowscapture=false;
  sysfont=new sdwSystemFont();
  deffont=new sdwSystemFont();
  sysopt->canwindow=true;
  sysopt->canedit=true;
  sysopt->canscrollbox=true;
  sysopt->canpaintcontrol=true;
  sysopt->canradiogroup=true;
  sysopt->canscrolledpainter=true;
  sysopt->cansysmenu=true;
  sysopt->canlabel=true;
  sysopt->canwraplabel=true;
  sysopt->checklistindsize=12;
//  setcaretpos(SDLRect(10,10,5,20));
}

bool sdwSystem::blinkcaret() {
  float d=(float)(clock()-carettime0)/(double)CLK_TCK;
  return (int)(d/vo->carettime)%2==0;
}

Picture *sdwSystem::createpicture() {
  return new sdwPicture();
}

void sdwSystem::setcaretpos(SDL_Rect rect,bool reset) {
  caretpos=rect;
  visiblecaret=true;
  wantcaretdatasize(caretpos.w,caretpos.h);
  if (reset) carettime0=clock();
  flip();
}
void sdwSystem::wantcaretdatasize(int w,int h) {
  if (w>caretdataw || h>caretdatah) {
    if (caretdata) SDL_FreeSurface(caretdata);
    caretdataw=w;caretdatah=h;
    caretdata=createSDLSurface(caretdataw,caretdatah);
  }
}


struct sdwJSystem:JSystem {
  sdwSystem *sys;
  string listfiles(jlib::STRING directory) {return JSDL_ListFiles(directory);}
  void listfiles(jlib::STRING directory,TCntVec<FileRec> &files) {JSDL_ListFiles(directory,files);}
  string listdirs(jlib::STRING directory) {return JSDL_ListDirs(directory);}
  sdwJSystem(sdwSystem *sys):sys(sys) {}
  void showinfo(string msg) {sys->alert(msg);}
  void showerror(string msg) {sys->alert(msg);}
  void showwarning(string msg) {sys->alert(msg);}
};

awsKey toawskey(SDL_keysym key) {
  awsKey res;
  res.ch=key.unicode&0x7F;
  res.key=toupper(res.ch);
  res.type=kt_char;
  switch (key.sym) {
    case SDLK_BACKSPACE:res.type=kt_bs;break;
    case SDLK_DELETE:res.type=kt_delete;break;
    case SDLK_ESCAPE:res.type=kt_esc;break;
    case SDLK_LEFT:res.type=kt_left;break;
    case SDLK_RIGHT:res.type=kt_right;break;
    case SDLK_UP:res.type=kt_up;break;
    case SDLK_DOWN:res.type=kt_down;break;
    case SDLK_PAGEDOWN:res.type=kt_pgdn;break;
    case SDLK_PAGEUP:res.type=kt_pgup;break;
    case SDLK_RETURN:res.type=kt_enter;break;
    case SDLK_HOME:res.type=kt_home;break;
    case SDLK_END:res.type=kt_end;break;
    case SDLK_F1:res.type=kt_f1;break;
    case SDLK_F2:res.type=kt_f2;break;
    case SDLK_F3:res.type=kt_f3;break;
    case SDLK_F4:res.type=kt_f4;break;
    case SDLK_F5:res.type=kt_f5;break;
    case SDLK_F6:res.type=kt_f6;break;
    case SDLK_F7:res.type=kt_f7;break;
    case SDLK_F8:res.type=kt_f8;break;
    case SDLK_F9:res.type=kt_f9;break;
    case SDLK_F10:res.type=kt_f10;break;
    case SDLK_F11:res.type=kt_f11;break;
    case SDLK_F12:res.type=kt_f12;break;
  }
  if (key.mod&KMOD_ALT) res.shift|=ks_alt;
  if (key.mod&KMOD_SHIFT) res.shift|=ks_shift;
  if (key.mod&KMOD_CTRL) res.shift|=ks_ctrl;
  if (key.mod&KMOD_LALT) res.shift.teststate|=kst_lalt;
  if (key.mod&KMOD_RALT) res.shift.teststate|=kst_ralt;
  if (key.mod&KMOD_LSHIFT) res.shift.teststate|=kst_lshift;
  if (key.mod&KMOD_RSHIFT) res.shift.teststate|=kst_rshift;
  if (key.mod&KMOD_LCTRL) res.shift.teststate|=kst_lctrl;
  if (key.mod&KMOD_RCTRL) res.shift.teststate|=kst_rctrl;
  return res;
}

void sdwHint::draw(const sdwCanvas &cnv) {
  cnv.frect(x,y,x+w-1,y+h-1,clnyellow);
  cnv.text(x,y,msg,clnblack,NULL);
}

void sdwSystem::alert(string msg) {
  if (wantgl) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  }
  sdwCanvas cnv(screen);
  cnv.setclip(SDLRect(0,0,w,h));
  cnv.frect(0,0,w-1,h-1,0);
  cnv.text(0,0,"Press any key to continue...",clnblue,NULL);
  int y=8;
  for(lineiter it=msg.getlines();*it;it++,y+=8) cnv.text(0,y,*it,clnwhite,NULL);
  if (wantgl) {
    SDL_UpdateRect(screen,0,0,w,h);
    SDL_GL_SwapBuffers();
  } else {
    SDL_Flip(screen);
  }
  SDL_Event event;
  while (SDL_WaitEvent(&event)) {
    if (event.type==SDL_KEYDOWN) break;
  }
  invalidate();
}

void sdwSystem::changedfocus() {
  updatecaretpos(true);
}

Font *sdwSystem::createfont() {
  return deffont->clone();
}
Font *sdwSystem::getsystemfont(bool canbeproportional) {
  return sysfont;
}

void sdwSystem::activatewin(sdwWindow *win) {
  focuswin=win;
  changedfocus();
}

sdwSystem::~sdwSystem() {
  if (screen) SDL_FreeSurface(screen);
  if (caretdata) SDL_FreeSurface(caretdata);
}
void sdwSystem::removedcontrol(sdwControl *ctrl) {
  controls.quickremove(ctrl);
  if (focuswin==ctrl) focuswin=NULL;
  for(int i=0;i<controls.count;i++) controls[i]->sdwnotifyremove(ctrl);
}
void sdwSystem::keyevent(SDL_KeyboardEvent &event) {
  sdwControl *kbfocus=getkbfocus();
  awsKey key=toawskey(event.keysym);
  if (sysmenu.presskey(key)) return;
  if (kbfocus) {
    if (event.type==SDL_KEYUP) kbfocus->keyup(key);
    if (event.type==SDL_KEYDOWN) kbfocus->keydown(key);
  } else {
  }
}
sdwControl *sdwSystem::getkbfocus() {
  if (focuswin) return focuswin->focused;
  return NULL;
}

ShiftState scanshiftstate() {
  ShiftState res;
  Uint8 *keystate = SDL_GetKeyState(NULL);
  if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT]) res|=ks_shift;
  if (keystate[SDLK_LALT] || keystate[SDLK_RALT]) res|=ks_alt;
  if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL]) res|=ks_ctrl;
  return res;
}

void sdwSystem::mouseevent(SDL_MouseButtonEvent &event) {
  shiftstate=scanshiftstate();
  bool down=event.type==SDL_MOUSEBUTTONDOWN;
  MouseButton but=mb_left;
  if (event.button==SDL_BUTTON_LEFT) but=mb_left;
  if (event.button==SDL_BUTTON_RIGHT) but=mb_right;
  if (event.button==SDL_BUTTON_MIDDLE) but=mb_middle;
  if (down) {
    menucapture=sysmenu.mousedown(event.x,event.y,but);
    if (!menucapture) windowscapture=desktop.mousedown(event.x,event.y,but,shiftstate);
    if (!windowscapture && !menucapture) ondesktopmousedown(event.x,event.y,but,shiftstate);
  } else {
    if (menucapture) sysmenu.mouseup(event.x,event.y,but);
    else if (windowscapture) desktop.mouseup(event.x,event.y,but,shiftstate);
    else ondesktopmouseup(event.x,event.y,but,shiftstate);
    menucapture=false;
    windowscapture=false;
  }
  if (sysmenu.wantrepaint) invalidate();
  shiftstate=ShiftState();
}

void sdwSystem::mousemotion(SDL_MouseMotionEvent &event) {
  shiftstate=scanshiftstate();
  desktop.mousemove(event.x,event.y,shiftstate);
  sysmenu.mousemove(event.x,event.y);
  ondesktopmousemove(event.x,event.y,shiftstate);
  if (sysmenu.wantrepaint) invalidate();
  shiftstate=ShiftState();
}

void sdwSystem::setvideo() {
  if (wantgl) screen=SDL_SetVideoMode(w,h,bits,SDL_HWSURFACE|SDL_OPENGLBLIT|SDL_RESIZABLE|(fullscreen?SDL_NOFRAME:0));
  else screen=SDL_SetVideoMode(w,h,bits,SDL_HWSURFACE|SDL_RESIZABLE|(fullscreen?SDL_NOFRAME:0)|SDL_DOUBLEBUF);
  if (fullscreen) SDL_WM_GrabInput(SDL_GRAB_OFF);else SDL_WM_GrabInput(SDL_GRAB_ON);
  glViewport(0,0,w,h);
  sysmenu.resize(w,h);
  invalidate();
}

void sdwSystem::resize(SDL_ResizeEvent &event) {
  w=event.w;h=event.h;
//  screen=SDL_SetVideoMode(w,h,32,SDL_HWSURFACE|SDL_FULLSCREEN);
  setvideo();
}

void sdwSystem::activeevent(SDL_ActiveEvent &ev) {
  if (ev.state==SDL_APPACTIVE) {
//    if (ev.gain) invalidate();
//    else SDL_WM_IconifyWindow();
  }
}

void sdwSystem::dispatch(SDL_Event &event) {
  switch (event.type) {
    case SDL_QUIT:terminated=true;break;
    case SDL_KEYDOWN:case SDL_KEYUP:keyevent(event.key);break;
    case SDL_MOUSEBUTTONDOWN:case SDL_MOUSEBUTTONUP:mouseevent(event.button);break;
    case SDL_MOUSEMOTION:mousemotion(event.motion);break;
    case SDL_VIDEORESIZE:resize(event.resize);break;
    case SDL_VIDEOEXPOSE:invalidate();break;
    case SDL_ACTIVEEVENT:activeevent(event.active);break;
  }
}
bool sdwSystem::processmessage() {
  char dummy1[0x20];
  SDL_Event event;
  char dummy2[0x20];
  zerorec(event);
  if (SDL_PollEvent(&event)) {
    if (listener) listener->handle_event(event);
    dispatch(event);
    return true;
  }
  return false;
}
bool sdwSystem::answer() {
  int res=0;
  for(;;) {
    try {
      while (processmessage()) res++;
      onidle();
      break;
    } catch (JException &error) {
      error.show();
    }
  }
  destroyplan();
  repaintifneeded();
  return res>0;
}
void sdwSystem::run() {
  while (!terminated) {
    answer();
  }
}
void sdwSystem::removedwindow(sdwWindow *win) {
  windows.remove(win);
  desktop.notifyremove(win);
}
void sdwSystem::notifyremove(Component *obj) {
  System::notifyremove(obj);
}
void sdwSystem::_hidecaret() {
  if (_visiblecaret) {
    SDL_Rect clip=SDLRect(0,0,w,h);
    SDL_SetClipRect(screen,&clip);
    SDL_Rect src=SDLRect(0,0,showcaretpos.w,showcaretpos.h),dest=showcaretpos;
    SDL_BlitSurface(caretdata,&src,screen,&dest);
    _visiblecaret=false;
  }
}
void sdwSystem::_showcaret() {
  if (!visiblecaret) return;
  if (!_visiblecaret) {
    SDL_Rect clip=SDLRect(0,0,w,h);
    SDL_SetClipRect(screen,&clip);
    SDL_Rect src=caretpos,dest=SDLRect(0,0,caretpos.w,caretpos.h);
    SDL_BlitSurface(screen,&src,caretdata,&dest);

    SDL_FillRect(screen,&caretpos,SDL_MapRGB(screen->format,nativervalue(caretcolor),
    nativegvalue(caretcolor),nativebvalue(caretcolor)));
    _visiblecaret=true;
    showcaretpos=caretpos;
  }
}

void sdwSystem::updatecaretpos(bool reset) {
  sdwControl *kbfocus=getkbfocus();
  if (!kbfocus) {
    visiblecaret=false;
    return;
  }
  SDL_Rect r=kbfocus->getcaret();
  Point pt=kbfocus->abspos();
  r.x+=pt.x;r.y+=pt.y;
  setcaretpos(r,reset);
}

void sdwSystem::dorepaintgl() {
  if (!wantgl) return;
  EventValue ev;
  ev.value="onDrawGLSurface()";
  donotify("ondrawgl",ev.get());
}

static void cliprect(SDL_Rect &r,int w,int h) {
  if (r.x<0) r.w+=r.x,r.x=0;
  if (r.y<0) r.h+=r.y,r.y=0;
  if (r.x+r.w>w) r.w=w-r.x;
  if (r.y+r.h>h) r.h=h-r.y;
}

void sdwSystem::setoptionfile(string file) {
  aws::System::setoptionfile(file);
  XMLNode opt=getopt();
  sdwBitmapFont *bf=new sdwBitmapFont();
  MainXMLNode xml;
  try {
    xml.loadfromfile(basedir/"data"/opt.svar("font_sizes"));
  } catch (...) {
    delete bf;
    return;
  }
  if (bf->eng->load(xml,basedir/"data"/opt.svar("font_images"))) {
    deffont=bf;
    vo->setfont(deffont);
  } else delete bf;
}

void sdwSystem::repaintifneeded() {
  sdwControl *kbfocus=getkbfocus();
  if (visiblecaret) {
    if (kbfocus) updatecaretpos(false);
    else visiblecaret=false;
  } else {
    if (kbfocus) updatecaretpos(true);
  }
  #ifndef NOBGBMP
  if (bgbmp) {
    SDL_Surface *tmps=SDL_CreateRGBSurfaceFrom(bgbmp->buffer(),bgbmp->getw(),bgbmp->geth(),
    bgbmp->bits(),bgbmp->pitch(),bgpf.rmask,bgpf.gmask,bgpf.bmask,bgpf.amask);
    SDL_BlitSurface(tmps,NULL,screen,NULL);
    SDL_FreeSurface(tmps);
    invalidated=true;
    invalidatedrect=Rect(0,0,w,h);
    paintedbgbmp=true;
  } else if (paintedbgbmp) {
    invalidated=true;
    invalidatedrect=Rect(0,0,w,h);
  }
  #endif
  bool blitgl=wantrepaintgl;
  bool repaintedgl=false;
  if (invalidated) {
    _hidecaret();
    invalidatedrect*=Rect(0,0,w,h);
    if (wantgl) {
      repaintedgl=true;
      dorepaintgl();
    } else {
      #ifndef NOBGBMP
      if (!bgbmp)
      #endif
      {
        SDL_Rect full=SDLRect(0,0,w,h);
        SDL_FillRect(screen,&full,vo->cldesktop);
      }
    }
//    sdwCanvas cnv0(screen,0,0);
    sdwCanvas cnv(screen,0,0);
    Rect &r=invalidatedrect;
    cnv.setclip(SDLRect(r.a.x,r.a.y,r.size().w,r.size().h));
    #ifndef NOBGBMP
    if (!bgbmp) cnv.frect(0,0,w-1,h-1,vo->cldesktop);
    #endif
    desktop.paint(cnv);
//    SDL_SetClipRect(screen,&full);
    sysmenu.paint(cnv);
    for(int i=0;i<hints.count;i++) hints[i]->draw(cnv);
    if (visiblecaret && blinkcaret()) _showcaret();
    if (wantgl) blitgl=true;
    if (!wantgl) SDL_Flip(screen);
  } else {
    if (wantchangecaret || _visiblecaret!=blinkcaret()) {
      if (_visiblecaret) _hidecaret();else _showcaret();
      wantflip=true;
    }
    if (wantflip && !wantgl) SDL_Flip(screen);
  }
  if (blitgl) {
    if (!wantrepaintgl) {
      glClearColor(0,0,0,1);
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
    if (!repaintedgl) dorepaintgl();
    TCntVec<SDL_Rect> update;
    sysmenu.getrects(update);
    for(int i=0;i<windows.count;i++) if (windows[i]->visible) update.add(windows[i]->getrect());
    for(int i=0;i<hints.count;i++) update.add(hints[i]->getrect());
    for(int i=0;i<update.count;i++) cliprect(update[i],w,h);
    glColor3f(1,1,1);
    SDL_UpdateRects(screen,update.count,update.buf);
    SDL_GL_SwapBuffers();
  }
  #ifndef NOBGBMP
  bgbmp=NULL;
  #endif
  wantrepaintgl=false;
  wantchangecaret=false;
  wantflip=false;
  invalidated=false;
}

void sdwSystem::setsysmenu(MenuItem *menu) {
  if (sysmenu.menu) sysmenu.menu->removehandlerall(this);
  sysmenu.setmenu(dynamic_cast<sdwMenuItem*>(menu));
  if (sysmenu.menu) sysmenu.menu->addhandlerall(this);
  invalidate();
}

//extern "C" void SDL_UninstallParachute(void);

void sdwSystem::setwantgl(bool value) {
  wantgl=value;
  setvideo();
}

bool sdwSystem::init() {
  if (SDL_Init(SDL_INIT_VIDEO)<0) return false;
  SDL_EnableUNICODE(true);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
  atexit(SDL_Quit);
//  screen=SDL_SetVideoMode(w=800,h=600,32,SDL_FULLSCREEN);
//  screen=SDL_SetVideoMode(w=800,h=600,32,SDL_HWSURFACE|SDL_FULLSCREEN);
//  SDL_Surface *real=SDL_GetVideoSurface();
  JSDL_GetDisplaySize(&w,&h);
  if (!fullscreen) w-=128,h-=128;
//  SDL_UninstallParachute();
  setvideo();
//  if (wantgl) screen=SDL_SetVideoMode(w,h,bits,SDL_HWSURFACE|SDL_OPENGLBLIT|SDL_RESIZABLE|SDL_NOFRAME);
//  else screen=SDL_SetVideoMode(w,h,bits,SDL_HWSURFACE|SDL_RESIZABLE|SDL_NOFRAME|SDL_DOUBLEBUF);
//  screen=SDL_SetVideoMode(w,h,bits,SDL_HWSURFACE|SDL_NOFRAME|SDL_DOUBLEBUF|SDL_RESIZABLE);
//  screen=SDL_SetVideoMode(w,h,bits,SDL_OPENGLBLIT|SDL_NOFRAME|SDL_RESIZABLE);
//  screen=SDL_SetVideoMode(w,h,32,SDL_SWSURFACE|SDL_RESIZABLE|SDL_NOFRAME);
//  sysmenu.resize(w,h);
//  invalidate();
  return true;
}

void sdwSystem::sethint(int hintid,int y,int x1,int x2,string msg) {
  closehint(hintid);
  sdwHint *hint=new sdwHint(hintid,msg);
  hint->x=x1;hint->y=y;hint->h=8;
  if (x2<0) hint->w=8*msg.length();
  else if (x2==0) hint->w=w-x1;
  else hint->w=x2-x1;
  hints.add(hint);
}
void sdwSystem::closehint(int hintid) {
  int idx=hints.psearchi(hintid);
  if (idx>=0) hints.erase(idx);
  invalidate();
}

void sdwSystem::getdesktopopts(DesktopOpts &opts) {
  opts.w=w;opts.h=h;
}

#ifndef NOBGBMP
void sdwSystem::setbgimage(zbuf::PixelMap *bmp,zbuf::PixelFormat &pf) {
  bgbmp=bmp;bgpf=pf;
}
#endif

MenuItem *sdwSystem::createmenuitem(Component *owner) {
  return new sdwMenuItem(owner,this);
}

RealWindow *sdwSystem::createWindow(Window *win) {
  sdwWindow *res=new sdwWindow(win,this);
  windows.add(res);
  desktop.notifyadd(res);
  return res;
}
RealRadioGroup *sdwSystem::createRadioGroup(RadioGroup *grp) {
  return new sdwRadioGroup(grp,this);
}
RealScrollBox *sdwSystem::createScrollBox(ScrollBox *scrollbox) {
  return new sdwScrollBox(scrollbox,this);
}
RealPaintControl *sdwSystem::createPaintControl(PaintControl *painter) {
  return new sdwPaintControl(painter,this);
}
RealScrolledPainter *sdwSystem::createScrolledPainter(ScrolledPainter *painter) {
  return new sdwScrolledPainter(painter,this);
}
RealEdit *sdwSystem::createEdit(Edit *edit) {
  return new sdwEdit(edit,this);
}
RealLabel *sdwSystem::createLabel(Label *label) {
  return new sdwLabel(label,this);
}
RealWrapLabel *sdwSystem::createWrapLabel(WrapLabel *label) {
  return new sdwWrapLabel(label,this);
}
RealTrackBar *sdwSystem::createTrackBar(TrackBar *trackbar) {
  return new sdwTrackBar(trackbar,this);
}
RealPlace *sdwSystem::createPlace(Place *place) {
  return new sdwControl(place,this);
}


void sdwSystem::bringtofront(sdwWindow *win) {desktop.bringtofront(win);}



void sdwSystem::invalidate(Rect r) {
  if (invalidated) {
    invalidatedrect+=r;
  } else {
    invalidated=true;
    invalidatedrect=r;
  }
}

}

extern "C" int SDL_main(int argc,char **argv) {
  sdw::JSDL_InitBaseDir(argc,argv);
  sdw::fullscreen=true;
  for(int i=0;i<argc;i++) if (eqstr(argv[i],"-w")) sdw::fullscreen=false;
//  basedir=argv[0];
//  basedir--;
  sdw::sdwSystem sys;
  init_lang();
  try {
    jsystem=new sdw::sdwJSystem(&sys);
    sys.init();
    sys.argc=argc-1;
    sys.argv=argv+1;
    aws::awsmain(&sys,argc-1,argv+1);
  } catch(aws::awsError &err) {
    jsystem->showerror(err.msg);
  }
}



