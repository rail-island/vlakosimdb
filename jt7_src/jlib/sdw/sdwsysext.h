#ifndef sdwsysextH
#define sdwsysextH

#include <zbuf/zbufwrap.h>
#include <sdl/sdl.h>

#ifndef USEBGIMG
#define NOBGBMP
#endif

namespace sdw {

struct DesktopOpts {
  int w,h;
};

struct SDLEventListener {
  virtual void handle_event(SDL_Event &event)=0;
};

struct sdwSystemExtension {
#ifndef NOBGBMP
  virtual void setbgimage(zbuf::PixelMap *bmp,zbuf::PixelFormat &pf)=0;
#endif
  virtual void getdesktopopts(DesktopOpts &opts)=0;
  virtual void repaintdesktop()=0;
  virtual void getmousepos(int *x,int *y)=0;
  virtual void setwantgl(bool value)=0;
  virtual void repaintgl()=0;
  ///x2==0:bere se do konce radku,x2<0:bere se minimalni sirka
  virtual void sethint(int hintid,int y,int x1,int x2,string msg)=0;
  virtual void closehint(int hintid)=0;
  virtual void set_listener(SDLEventListener *listener)=0;
};

}

#endif
