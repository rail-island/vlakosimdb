#ifndef awsmenuH
#define awsmenuH

#include "awscomponent.h"

namespace aws {

struct MenuItem:Component {
  void loadprops(XMLNode xml,Form *form);
  MenuItem(Component *owner,System *system);

  virtual void settext(string value)=0;
  virtual string gettext()=0;
  virtual bool getchecked()=0;
  virtual void setchecked(bool value)=0;
  virtual int childcnt()=0;
  virtual string getshortcut()=0;
  virtual void setshortcut(string shortcut)=0;
  virtual MenuItem *childat(int index)=0;
  virtual MenuItem *insertchild(int index)=0;//naplatny index:dite je pridano na konec

  MenuItem *addchild() {return insertchild(-1);}

  void addhandlerall(EventHandler *handler);//pridat handler i pro deti
  void removehandlerall(EventHandler *handler);
protected:
  EventValue ev_click;
  string comname;
  void onclick();
};

}

#endif
