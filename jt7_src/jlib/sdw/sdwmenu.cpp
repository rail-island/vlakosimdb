#include <jlib.h>
#pragma hdrstop

#include "sdwmenu.h"
#include "sdwsystem.h"
#include "sdwcontrol.h"

namespace sdw {

sdwMenuItem::sdwMenuItem(Component *owner,sdwSystem *system):MenuItem(owner,system) {
  sys=system;parent=dynamic_cast<sdwMenuItem*>(owner);
  actchild=NULL;popupchild=NULL;vo=sys->vo;
  buttonbar=false;barx0=0;bary0=0;
  menubarhi=vo->menubarhi;
}

bool sdwMenuItem::presskey(awsKey key) {
  if (key.eqkey(shortcut_key)) {
    onclick();
    return true;
  }
  for(int i=0;i<childs.count;i++) if (childs[i]->presskey(key)) return true;
  return false;
}

bool sdwMenuBar::presskey(awsKey key) {
  if (menu) return menu->presskey(key);
  return false;
}

sdwMenuBar::sdwMenuBar(sdwSystem *asys) {
  sys=asys;menu=NULL;wantrepaint=false;w=h=0;
}
int sdwMenuBar::gethi() {
  if (menu) return menu->menubarhi;
  return 0;
}
void sdwMenuBar::paint(sdwCanvas cnv) {
  wantrepaint=false;
  if (!menu) return;
  if (!menu->counted) menu->countbar();
  menu->paintbar(this,cnv);
}

void sdwMenuBar::notifyremove(sdwMenuItem *value) {
  if (value==menu) menu=NULL;
}
void fillcounted(sdwMenuItem *menu,bool value=false) {
  menu->counted=value;
  for(int i=0;i<menu->childs.count;i++) fillcounted(menu->childs[i]);
}
void sdwMenuBar::setmenu(sdwMenuItem *value) {
  menu=value;
  if (menu) fillcounted(menu);
}
void sdwMenuBar::mousemove(int x,int y) {
  if (menu) menu->barmousemove(this,x,y);
}
void sdwMenuBar::mouseup(int x,int y,MouseButton but) {
  if (menu) menu->barmouseup(this,x,y,but);
}
bool sdwMenuBar::mousedown(int x,int y,MouseButton but) {
  if (menu) {
    bool res=menu->barmousedown(this,x,y,but);
    if (!res) menu->closemenu();
    return res;
  }
  return false;
}
void sdwMenuBar::closemenu() {
  if (menu) menu->closemenu();
  wantrepaint=true;
}
void sdwMenuBar::getrects(TCntVec<SDL_Rect> &rects) {
  if (!menu) return;
  menu->bargetrects(this,rects);
}



sdwMenuItem *sdwMenuItem::baritemat(int x,int y) {
  if (y>=bary0 && y<menubarhi+bary0) {
    for(int i=0;i<childs.count;i++) {
      sdwMenuItem *child=childs[i];
      if (x>=child->barx && x<child->barx+child->barw) return child;
    }
  }
  return NULL;
}
sdwMenuItem *sdwMenuItem::popupitemat(int x,int y) {
  y-=vo->popupmenuborder;
  if (x>=0 && x<popupw) {
    for(int i=0;i<childs.count;i++) {
      sdwMenuItem *child=childs[i];
      if (y>=child->itemy && y<child->itemy+vo->menuitemhi) return child;
    }
  }
  return NULL;
}

bool sdwMenuItem::barmousedown(sdwMenuBar *bar,int x,int y,MouseButton but) {
  if (popupchild) return popupchild->popupmousedown(bar,x-popupchild->popupx,y-popupchild->popupy,but);
  else {
    sdwMenuItem *baritem=baritemat(x,y);
    if (!baritem) return false;
    popupchild=baritem;
    bar->repaint();
    return true;
  }
}
bool sdwMenuItem::popupmousedown(sdwMenuBar *bar,int x,int y,MouseButton but) {
  return popupitemat(x,y);
}
void sdwMenuItem::popupmouseup(sdwMenuBar *bar,int x,int y,MouseButton but) {
  sdwMenuItem *pitem=popupitemat(x,y);
  if (pitem) {
    if (but==mb_left) bar->closemenu();
    pitem->onclick();
  } else if (ev_click.get()) {
    bar->closemenu();
    onclick();
  }
}
void sdwMenuItem::barmouseup(sdwMenuBar *bar,int x,int y,MouseButton but) {
  if (popupchild) popupchild->popupmouseup(bar,x-popupchild->popupx,y-popupchild->popupy,but);
}
void sdwMenuBar::mouseleave() {
  if (!menu) return;
  if (menu->actchild) {
    menu->actchild=NULL;
    repaint();
  }
  if (menu->popupchild) {
    menu->popupchild=NULL;
    repaint();
  }
}


void sdwMenuItem::notifyremove(Component *obj) {
  MenuItem::notifyremove(obj);
  childs.remove(dynamic_cast<sdwMenuItem*>(obj));
  if (obj==parent) parent=NULL;
  if (obj==actchild) actchild=NULL;
  if (obj==popupchild) popupchild=NULL;
}

void sdwMenuItem::setshortcut(string shortcut) {
  shortcut_key.fromstring(shortcut.c_str());
  if (shortcut_key.valid()) shortcut_str=shortcut_key.tostring();
  else shortcut_str="",shortcut_key=awsKey();
}
void sdwMenuItem::countbar() {
  int x=barx0;
  for(int i=0;i<childs.count;i++) {
    sdwMenuItem *child=childs[i];
    child->barx=x;
    child->barw=vo->menutextwi(child->text);
    x+=child->barw+vo->menubarspace;
  }
}

void sdwMenuItem::countpopupitemprops() {
  itemtextw=vo->menutextwi(text);
  shortcuttextw=vo->menutextwi(shortcut_str);
}

void sdwMenuItem::countpopup(int x0,int y0) {
  popupx=x0;popupy=y0;
  int acty=vo->popupmenuborder;
  int textw=0,shortcutw=0;
  for(int i=0;i<childs.count;i++) {
    childs[i]->countpopupitemprops();
    if (childs[i]->itemtextw>textw) textw=childs[i]->itemtextw;
    if (childs[i]->shortcuttextw>shortcutw) shortcutw=childs[i]->shortcuttextw;
  }
  popupw=textw+2*vo->popupmenuborder;
  if (shortcutw>0) popupw+=shortcutw,popupw+=vo->shortcutspace;
  for(int i=0;i<childs.count;i++) {
    childs[i]->itemx=vo->popupmenuborder;
    childs[i]->itemy=acty;
    childs[i]->itemtextx=vo->popupmenuborder;
    childs[i]->itemshortcutx=vo->popupmenuborder+textw+vo->shortcutspace;
    acty+=vo->menuitemhi;
  }
  popuph=childs.count*vo->menuitemhi+2*vo->popupmenuborder;
}

MenuItem *sdwMenuItem::insertchild(int index) {
  sdwMenuItem *res=new sdwMenuItem(this,sys);
  if (index<0) childs.add(res);else childs.insert(index,res);
  return res;
}

void sdwMenuItem::drawpopup(sdwCanvas cnv) {
  if (childs.count==0) return;
  cnv.frect(0,0,popupw-1,popuph-1,vo->clmenupopupbg);
  cnv.rect(1,1,popupw-2,popuph-2,vo->clmenupopupframe);
  for(int i=0;i<childs.count;i++) {
    if (childs[i]->text=="-") {
      cnv.line(1,childs[i]->itemy+vo->menuitemhi/2,popupw-2,childs[i]->itemy+vo->menuitemhi/2,vo->clmenupopupframe);
    } else {
      int tcolor=vo->clmenupopuptext;
      if (childs[i]==actchild) {
        cnv.frect(childs[i]->itemx,childs[i]->itemy,
          childs[i]->itemx+popupw-2*vo->popupmenuborder,childs[i]->itemy+vo->menuitemhi,
          vo->clmenupopupselbg);
        tcolor=vo->clmenupopupseltext;
      }
      cnv.text(childs[i]->itemx,childs[i]->itemy,childs[i]->text,tcolor,sys->deffont);
      cnv.text(childs[i]->itemshortcutx,childs[i]->itemy,childs[i]->shortcut_str,tcolor,sys->deffont);
    }
  }
}

void sdwMenuItem::bargetrects(sdwMenuBar *bar,TCntVec<SDL_Rect> &rects) {
  if (buttonbar) {
    for(int i=0;i<childs.count;i++) {
      sdwMenuItem *child=childs[i];
      rects.add(SDLRect(child->barx,bary0,child->barw,menubarhi));
    }
  } else {
    rects.add(SDLRect(0,bary0,bar->w,menubarhi));
  }
  if (popupchild && popupchild->childcnt()>0) rects.add(SDLRect(popupchild->barx,bary0+menubarhi,popupchild->popupw,popupchild->popuph));
}

void sdwMenuItem::paintbar(sdwMenuBar *bar,sdwCanvas cnv) {
  if (!buttonbar) cnv.frect(0,bary0,bar->w,bary0+menubarhi,vo->clmenubarbg);
  for(int i=0;i<childs.count;i++) {
    sdwMenuItem *child=childs[i];
    int tcolor=vo->clmenubartext;
    if (actchild==child) {
      cnv.frect(child->barx,bary0,child->barx+child->barw-1,bary0+menubarhi,vo->clmenubarselbg);
      tcolor=vo->clmenubarseltext;
    } else {
      if (buttonbar) {
        cnv.frect(child->barx,bary0,child->barx+child->barw-1,bary0+menubarhi,vo->clmenubarbg);
      }
    }
    cnv.text(child->barx,bary0,child->text,tcolor,sys->deffont);
  }
  if (popupchild) {
    if (!popupchild->counted) popupchild->countpopup(popupchild->barx,bary0+menubarhi);
    popupchild->paintpopup(cnv);
  }
}
void sdwMenuItem::barmousemove(sdwMenuBar *bar,int x,int y) {
  sdwMenuItem *baritem=baritemat(x,y);
  if (baritem!=actchild) {
    actchild=baritem;
    bar->repaint();
  }
  if (popupchild && baritem && popupchild!=baritem) {
    popupchild=baritem;
    bar->repaint();
  }
  if (popupchild) {
    popupchild->popupmousemove(bar,x-popupchild->popupx,y-popupchild->popupy);
  }
}
void sdwMenuItem::popupmousemove(sdwMenuBar *bar,int x,int y) {
  sdwMenuItem *pitem=popupitemat(x,y);
  if (pitem!=actchild) {
    actchild=pitem;
    bar->repaint();
  }
  if (popupchild && pitem && popupchild!=pitem) {
    popupchild=pitem;
    bar->repaint();
  }
/*  if (popupchild) {
    popupchild->popupmousemove(bar,x-popupchild->popupx,y-popupchild->popupy);
  }*/
}
void sdwMenuItem::setsdp(string name,string value) {
  if (name=="buttonbar") buttonbar=value.ivalue();
  if (name=="barx0") barx0=value.ivalue();
  if (name=="bary0") bary0=value.ivalue();
}




}

