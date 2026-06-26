#include <jlib.h>
#pragma hdrstop

#include "sdwopts.h"
#include "sdwfont.h"
#include "sdwpaint.h"

namespace sdw {

int ViewOpts::menutextwi(string text) {if (font) return font->getwidth(text);return text.length()*8;}
int ViewOpts::menutexthi(string text) {if (font) return font->getheight();return 8;}
int ViewOpts::radiotextwi(string text) {if (font) return font->getwidth(text);return text.length()*8;}
int ViewOpts::edittextwi(string text) {if (font) return font->getwidth(text);return text.length()*8;}

void ViewOpts::setfont(sdwCommonFont *value) {
  font=value;
  int thi=font->getheight();

  wintitlehi=thi+2;
  menubarhi=thi+2;
  menuitemhi=thi;
  radiotitlehi=thi+2;
  radiofonthi=thi;
  edithi=thi+4;
}

ViewOpts::ViewOpts() {
  wintitlehi=10;
  winhborder=2;
  winbotborder=2;
  wintopborder=2;
  wintitleleft=10;
  menubarhi=10;
  menubarspace=10;
  popupmenuborder=3;
  shortcutspace=10;
  menuitemhi=8;
  radiotitlehi=10;
  radiobotspace=2;
  radiohborder=2;
  radioradius=3;
  radiofonthi=8;
  smallradioradius=1;
  radiospaceleft=7;
  radiospaceright=7;
  radiotitleleft=10;
  scrollbarborder=2;
  scrollboxborder=2;
  scrollbarsize=12;
  edithi=12;
  editborder=2;
  editcaretwi=1;
  winbuttonsize=8;
  winbuttonborder=1;
  trackbarsize=5;

  clbtnface=RGBColor(0xc0,0xc0,0xc0).toint();
//  cldesktop=RGBColor(0,0,0x40).toint();
  cldesktop=clnblack;
  clwinbgcolor=clbtnface;
  clwintitlebg=clnblue;
  clwintitletext=clnyellow;

  clmenubarbg=clnyellow;
  clmenubartext=clnblack;
  clmenubarselbg=clnlime;
  clmenubarseltext=clnblack;

  clmenupopupbg=clbtnface;
  clmenupopuptext=clnblack;
  clmenupopupselbg=clnnavy;
  clmenupopupseltext=clnwhite;
  clmenupopupframe=clnblack;

  clradioframe=clnblack;
  clradiotext=clnblack;
  clradio=clnblack;
  clradiosmall=clnblue;

  clscrollbarbg=clnnavy;
//  clscrollbarframe=RGBColor(0xc0,0xc0,0xc0).toint();
//  clscrollbar=RGBColor(0x80,0x80,0x80).toint();
  clscrollbar=clngreen;
  clscrollboxbg=RGBColor(0x0,0x0,0x40).toint();

  clrisedframein=RGBColor(0x80,0xff,0x80).toint();
  clrisedframeout=RGBColor(0x80,0xe0,0x80).toint();
  clrisedframebg=RGBColor(0x80,0xc0,0x80).toint();

  clsunkenframein=RGBColor(0x80,0xc0,0x80).toint();
  clsunkenframeout=RGBColor(0x80,0xe0,0x80).toint();
  clsunkenframebg=RGBColor(0x80,0xff,0x80).toint();

  cleditframe=clnblack;
  cleditbg=clnwhite;
  cledittext=clnblack;

  clwinbuttonbg=clngray;
  clwinclosebut=clnblack;

  cltrackbarbg=clngray;
  cltrackbardragger=clnyellow;

  carettime=0.4;
}

}

