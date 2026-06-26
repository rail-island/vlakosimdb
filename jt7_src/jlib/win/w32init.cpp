#include <winlib.h>
#pragma hdrstop

#include "w32init.h"
#include "w32gr.h"
#include "w32dci.h"

namespace wlib {

void w32_init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  W_init(hInstance);
  stdobjs=new WStdGrObjs();
}
void w32_done() {
  delete stdobjs;
}


}

#pragma package(smart_init)
