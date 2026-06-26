#include <jlib.h>
#pragma hdrstop

#include "gmess.h"
#include "gameu.h"
#include "gview.h"

//#pragma package(smart_init)

namespace jtyc {

void MChangeSeeAngle::process(Game *game,double deltatime) {
//  game->view->setseeangle(value);
}
void MChangeViewAngle::process(Game *game,double deltatime) {
//  game->view->setangle(value);
}

/*void MMouseMove::process(Game *game,double deltatime) {
  game->view->mousemove(x,y,deltatime);
  game->state->mmove(x,y,deltatime);
}*/

void MMouseDown::process(Game *game,double deltatime) {
//  game->state->mdown(right);
}
void MMouseUp::process(Game *game,double deltatime) {
//  game->state->mup(right);
}

void MResize::process(Game *game,double deltatime) {
  game->view->resize(x,y);
}

}