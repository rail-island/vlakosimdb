#include <jlib.h>
#pragma hdrstop

#include "awsmenu.h"
#include "awsreal.h"
#include "awsform.h"

namespace aws {


MenuItem::MenuItem(Component *owner,System *system):Component(owner,system) {

}

void MenuItem::addhandlerall(EventHandler *handler) {
  addhandler(handler);
  for(int i=0;i<childcnt();i++) childat(i)->addhandlerall(handler);
}
void MenuItem::removehandlerall(EventHandler *handler) {
  removehandler(handler);
  for(int i=0;i<childcnt();i++) childat(i)->removehandlerall(handler);
}

void MenuItem::onclick() {
  donotify("onclick",ev_click.get());
  if (comname!="") docommand(comname);
}

void MenuItem::loadprops(XMLNode xml,Form *form) {
  Component::loadprops(xml,form);
  if (form) addhandler(form);
  ev_click.load(xml,"onclick");
  comname=xml.svar("command");
  settext(lang_text_ch(xml.svar("text")));
  setchecked(xml.ivar("checked"));
  setshortcut(xml.svar("shortcut"));
  if (xml.getname()=="separator") settext("-");
  for(int i=0;i<xml.count();i++) {
    MenuItem *child=addchild();
    child->loadprops(xml[i],form);
  }
}


}

