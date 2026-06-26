#ifndef awwmenuH
#define awwmenuH

#include <aws/awsmenu.h>
#include "awwobject.h"

namespace aww {
using namespace aws;

struct awwMenuItem:MenuItem {
  HMENU gethandle() {if (!handle) counthandle();return handle;}
  awwMenuItem(awwMenuItem *parent);
  awwMenuItem(Component *owner,System *system);
  ~awwMenuItem() {deletemenu();}
  bool runcommand(int cid);
  bool keydown(awsKey key);
protected:
  string gettext() {return text;}
  void settext(string value) {text=value;}
  bool getchecked() {return checked;}
  void setchecked(bool value) {checked=value;}
  void setshortcut(string shortcut);
  string getshortcut() {return shortcut_str;}
  int childcnt() {return childs.count;}
  MenuItem *childat(int index) {return childs[index];}
  MenuItem *insertchild(int index);
private:
  static const int min_com=0xA00;
  awwMenuItem *parent;
  HMENU handle;
  string text;
  string shortcut_str;
  awsKey shortcut_key;
  int command;//CID, pokud==0, neni definovano

  TCntVec<awwMenuItem*> childs;

  static int getfreecom(awwMenuItem *thiz);
  string getmenutext() {string res=text;if (shortcut_key.valid()) res<<"\t"<<shortcut_str;return res;}
  bool hasreserved(int com);
  int getcommand() {if (!command) command=getfreecom(this);return command;}

  bool checked;
  void notifyremove(Component *comp);
  void append(awwMenuItem *child);
  void deletemenu() {if (handle) DestroyMenu(handle);handle=NULL;}
  void counthandle();
};

}

#endif
