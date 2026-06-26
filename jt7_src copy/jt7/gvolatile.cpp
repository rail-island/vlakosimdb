#include <jlib.h>
#pragma hdrstop

#include "gvolatile.h"
#include "gameu.h"
#include "gview.h"

namespace jtyc {

TimeVolatile::TimeVolatile(double lifetime) {
  destroytime=game->gettime()+lifetime;
}
bool TimeVolatile::iterate(double deltatime) {
  work(deltatime);
  return game->gettime()>destroytime;
}
void BadVr::render(RenderContext &rc) {
  game->view->drawptcursor(x,y,clnyellow);
}
void BadSq::render(RenderContext &rc) {
  game->view->drawsqcursor(x,y,clnred);
}
ErrorMessage::ErrorMessage(string msg):TimeVolatile(4) {
  id=game->allochint(msg);
}
ErrorMessage::~ErrorMessage() {
  game->freehint(id);
}
InfoMessage::InfoMessage(string msg):TimeVolatile(4) {
  id=game->allochint(msg);
}
InfoMessage::~InfoMessage() {
  game->freehint(id);
}




}

//#pragma package(smart_init)
