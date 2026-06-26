#ifndef sdwmenuH
#define sdwmenuH

#include "sdwcontrol.h"

namespace sdw {
class sdwMenuItem;class ViewOpts;

struct sdwMenuBar {
  void paint(sdwCanvas cnv);
  void setmenu(sdwMenuItem *menu);
  int gethi();
  void countbar();
  void notifyremove(sdwMenuItem *menu);
  bool mousedown(int x,int y,MouseButton but);
  void mouseup(int x,int y,MouseButton but);
  void mousemove(int x,int y);
  void mouseleave();
  void repaint() {wantrepaint=true;}
  void resize(int aw,int ah) {w=aw;h=ah;}
  sdwMenuBar(sdwSystem *sys);
  void closemenu();
  void getrects(TCntVec<SDL_Rect> &rects);
  bool presskey(awsKey key);

  sdwMenuItem *menu;
  sdwSystem *sys;
  int w,h;
  bool wantrepaint;
};

struct sdwMenuItem:MenuItem {
  sdwMenuItem(Component *owner,sdwSystem *system);

  void settext(string value) {text=value;}
  string gettext() {return text;}
  bool getchecked() {return false;}
  void setchecked(bool value) {checked=value;}
  int childcnt() {return childs.count;}
  string getshortcut() {return shortcut_str;}
  void setshortcut(string shortcut);
  MenuItem *childat(int index) {return childs[index];}
  MenuItem *insertchild(int index);
  void notifyremove(Component *obj);
  sdwMenuItem *baritemat(int x,int y);
  sdwMenuItem *popupitemat(int x,int y);
  bool presskey(awsKey key);

  TCntVec<sdwMenuItem*> childs;
  string text;
  string shortcut_str;
  awsKey shortcut_key;
  bool checked;
  sdwSystem *sys;
  sdwMenuItem *parent;
  ViewOpts *vo;

  void countpopup(int x0,int y0);
  void countbar();
  void countpopupitemprops();
  void paintbar(sdwMenuBar *bar,sdwCanvas cnv);
  void paintpopup(sdwCanvas cnv) {drawpopup(sdwCanvas(cnv,popupx,popupy));}
  void drawpopup(sdwCanvas cnv);
  void barmousemove(sdwMenuBar *bar,int x,int y);
  bool barmousedown(sdwMenuBar *bar,int x,int y,MouseButton but);
  void barmouseup(sdwMenuBar *bar,int x,int y,MouseButton but);
  void popupmousemove(sdwMenuBar *bar,int x,int y);
  bool popupmousedown(sdwMenuBar *bar,int x,int y,MouseButton but);
  void popupmouseup(sdwMenuBar *bar,int x,int y,MouseButton but);
  void closemenu() {actchild=NULL;popupchild=NULL;for(int i=0;i<childs.count;i++) childs[i]->closemenu();}
  void bargetrects(sdwMenuBar *bar,TCntVec<SDL_Rect> &rects);

  void setsdp(string name,string value);
  string getsdp(string name) {return "";}
  bool supportsdp(string name) {return false;}

  bool counted;
  sdwMenuItem *actchild;//nad kterym je mys (zvyraznene(
  sdwMenuItem *popupchild;//vyskocene v okne
  int barx,bary,barw;//souradnice a sirka v MenuBar
  int itemtextw,shortcuttextw;//textwi(text0,textwi(shirtcut_str)
  int itemx,itemy,itemtextx,itemshortcutx;
  int popupx,popupy,popupw,popuph;

  bool buttonbar;
  int barx0,bary0;
  int menubarhi;
};


}

#endif
