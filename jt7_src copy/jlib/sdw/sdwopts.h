#ifndef sdwoptsH
#define sdwoptsH

#include <gr/comgr.h>
#include <com/ptrto.h>

namespace sdw {
using namespace gr;

typedef unsigned int NativeColor;
class sdwCommonFont;

struct ViewOpts {
  int wintitlehi;
  int winhborder;
  int winbotborder;
  int wintopborder;
  int menubarhi;
  int wintitleleft;
  int menutextwi(string text);
  int menutexthi(string text);
  int menubarspace;
  int popupmenuborder;
  int shortcutspace;
  int menuitemhi;
  int radiotitlehi;
  int radiobotspace;
  int radiohborder;
  int radioradius;
  int radiofonthi;
  int smallradioradius;
  int radiospaceleft;
  int radiospaceright;//misto kolem radio-kolecka
  int radiotitleleft;
  int radiotextwi(string text);
  int scrollbarborder;
  int scrollboxborder;
  int scrollbarsize;
  int winbuttonsize;
  int winbuttonborder;

  int edittextwi(string text);
  int edithi;
  int editborder;
  int editcaretwi;

  int trackbarsize;

  NativeColor clbtnface;
  NativeColor cldesktop;

  NativeColor clwinbgcolor;
  NativeColor clwintitlebg;
  NativeColor clwintitletext;

  NativeColor clmenubarbg;
  NativeColor clmenubartext;
  NativeColor clmenubarselbg;
  NativeColor clmenubarseltext;

  NativeColor clmenupopupbg;
  NativeColor clmenupopuptext;
  NativeColor clmenupopupselbg;
  NativeColor clmenupopupseltext;
  NativeColor clmenupopupframe;

  NativeColor clradioframe;
  NativeColor clradiotext;

  NativeColor clradio;
  NativeColor clradiosmall;

  NativeColor clscrollbarbg;
  NativeColor clscrollbar;
  NativeColor clscrollboxbg;

  NativeColor clrisedframein;
  NativeColor clrisedframeout;
  NativeColor clrisedframebg;

  NativeColor clsunkenframein;
  NativeColor clsunkenframeout;
  NativeColor clsunkenframebg;

  NativeColor cleditframe;
  NativeColor cleditbg;
  NativeColor cledittext;

  NativeColor clwinbuttonbg;
  NativeColor clwinclosebut;

  NativeColor cltrackbarbg;
  NativeColor cltrackbardragger;

  float carettime;

  PtrTo<sdwCommonFont> font;

  ViewOpts();
  void setfont(sdwCommonFont *value);
};

}

#endif
