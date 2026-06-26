#ifndef awswindowH
#define awswindowH

#include "awscontrol.h"
#include "awsmenu.h" 

namespace aws {
class RealWindow;

enum WindowState {ws_minimized,ws_maximized,ws_normal};
enum WindowPosition {wp_default,wp_normal,wp_centered};


struct Window:ParentControl {

  Window(Component *owner,System *system);

  void setmenu(MenuItem *menu);

  property_i(state,WindowState)
  property_i(position,WindowPosition)
  property_i(text,string)
  property_i(clientsize,Size)
  property_i(style,string)

  RealWindow *getreal() {return (RealWindow*)realcontrol;}
  void loadprops(XMLNode xml,Form *form);

  void showmodal();
  void close();
  void denyclose();

  void onkeydown(awsKey key,awsEvent &event);
  virtual void onclose();
  virtual bool canbemain() {return true;}
private:
  string autoload;
  EventValue ev_close,ev_ok;bool closeonesc;
};

}

#endif
