#include <winlib.h>
#pragma hdrstop

#include "awwmenu.h"
#include <win/w32keys.h>

namespace aww {

void awwMenuItem::notifyremove(Component *comp) {
  MenuItem::notifyremove(comp);
  childs.quickremove(dynamic_cast<awwMenuItem*>(comp));
}

awwMenuItem::awwMenuItem(awwMenuItem *parent):MenuItem(parent,parent->system),parent(parent) {
  handle=NULL;checked=false;command=0;
}
awwMenuItem::awwMenuItem(Component *owner,System *system):MenuItem(owner,system) {
  handle=NULL;checked=false;command=0;
  parent=NULL;
}


MenuItem *awwMenuItem::insertchild(int index) {
  awwMenuItem *added=new awwMenuItem(this);
  if (index<0 || index>=childs.count) childs.add(added);else childs.insert(index,added);
  return added;
}

int awwMenuItem::getfreecom(awwMenuItem *thiz) {
  while (thiz->parent) thiz=thiz->parent;
  int res=thiz->min_com;
  while (thiz->hasreserved(res)) res++;
  return res;
}
bool awwMenuItem::hasreserved(int com) {
  if (command==com) return true;
  for(int i=0;i<childs.count;i++) if (childs[i]->hasreserved(com)) return true;
  return false;
}

void awwMenuItem::setshortcut(string shortcut) {
//  char keynice[0x20];
  shortcut_key.fromstring(shortcut.c_str());
//  shortcut_key=wlib::strtoshortcut(shortcut.c_str());
  if (shortcut_key.valid()) shortcut_str=shortcut_key.tostring();
/*
    wlib::shortcuttostr(shortcut_key,keynice);
    shortcut_str=keynice;
  }*/
}

bool awwMenuItem::runcommand(int cid) {
  if (cid==command) {
    onclick();
    return true;
  }
  for(int i=0;i<childs.count;i++) if (childs[i]->runcommand(cid)) return true;
  return false;
}
bool awwMenuItem::keydown(awsKey key) {
  if (!key.valid()) return false;
  if (key.eqkey(shortcut_key)) {onclick();return true;}
  for(int i=0;i<childs.count;i++) if (childs[i]->keydown(key)) return true;
  return false;
}

void awwMenuItem::append(awwMenuItem *child) {
  HMENU popup=child->gethandle();
  if (popup) {
    AppendMenu(handle,MF_STRING|MF_POPUP,(UINT)popup,child->text.c_str());
  } else {
    if (child->text=="-") {
      AppendMenu(handle,MF_SEPARATOR,0,0);
    } else {
      int com=child->getcommand();
      AppendMenu(handle,MF_STRING,com,child->getmenutext().c_str());
    }
  }
}

void awwMenuItem::counthandle() {
  if (childs.count==0) {handle=NULL;return;}
  handle=CreateMenu();
  for(int i=0;i<childs.count;i++) {
    append(childs[i]);
  }
}


}


