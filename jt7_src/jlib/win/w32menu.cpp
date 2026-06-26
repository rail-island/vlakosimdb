#include <winlib.h>
#pragma hdrstop

#include "w32menu.h"
#include "w32keys.h"

namespace wlib {

winMenuBar::~winMenuBar() {
  while (menus.count>0) DestroyMenu(menus.last()),menus.eraselast();
}

HMENU winMenuBar::makepopup(XMLNode xml) {
  HMENU res=CreateMenu();menus.add(menu);
  xassert(xml.hasname("popup"));
  for(int i=0;i<xml.count();i++) appendmenu(res,xml[i]);
  return res;
}
void winMenuBar::appendmenu(HMENU parent,XMLNode xml) {
  char text[0x100];
  strcpy(text,xml.cvar("text"));
  if (xml.hasname("do")) {
    int com=coms.count+combeg;comend++;
    coms.add(xml.cvar("command"));
    if (const char *key=xml.cvar("shortcut")) {
      char keynice[0x20];
      int keycode=strtoshortcut(key);
      if (keycode) {
        Accel &a=accels.add();
        a.key=keycode;
        a.com=com;
        shortcuttostr(a.key,keynice);
        sprintf(text,"%s\t%s",xml.cvar("text"),keynice);
      }
    }
    AppendMenu(parent,MF_STRING,com,text);
  }
  if (xml.hasname("popup")) {
    HMENU popup=makepopup(xml);
    AppendMenu(parent,MF_STRING|MF_POPUP,(UINT)popup,text);
  }
  if (xml.hasname("separator")) {
    AppendMenu(parent,MF_SEPARATOR,0,0);
  }
}

void winMenuBar::load(XMLNode xml,int menu_bar_command) {
  xassert(xml.hasname("menubar"));
  comend=combeg=menu_bar_command;
  menu=CreateMenu();menus.add(menu);
  for(int i=0;i<xml.count();i++) appendmenu(menu,xml[i]);
}

bool winMenuBar::keydown(int key) {
  bool res=false;
  for(int i=0;i<accels.count;i++) if (accels[i].key==key) runcommand(accels[i].com),res=true;
  return res;
}


}

#pragma package(smart_init)
