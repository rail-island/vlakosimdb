#ifndef w32menuH
#define w32menuH

#include <ext/xmlnode.h>

namespace wlib {

struct MenuHandler {
  virtual void run_menu_command(const char *com)=0;
};

struct winMenuBar {
  void load(XMLNode xml,int menu_bar_command=0xA00);
  HMENU getmenu() {return menu;}
  winMenuBar() {handler=NULL;menu=0;combeg=comend=0;}
  bool runcommand(int com) {
    if (handler and com>=combeg and com<comend) {handler->run_menu_command(coms[com-combeg]);return true;}
    return false;
  }
  bool keydown(int key);
  ~winMenuBar();
  MenuHandler *handler;
private:
  struct Accel {
    int key;
    int com;
  };
  TCntVec<Accel> accels;
  TCntVec<HMENU> menus;
  STRLIST coms;
  HMENU menu;
  int combeg,comend;
  HMENU makepopup(XMLNode item);
  void appendmenu(HMENU parent,XMLNode item);
};

}

#endif
