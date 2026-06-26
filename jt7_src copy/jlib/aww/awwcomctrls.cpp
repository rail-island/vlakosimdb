#include <winlib.h>
#pragma hdrstop

#define TCS_HOTTRACK 0x0040
#define ICC_BAR_CLASSES 0x00000004

#include "awwcomctrls.h"

namespace aww {

static bool InitCommonControl(int cc) {
  INITCOMMONCONTROLSEX icc;
  icc.dwSize=sizeof(icc);
  icc.dwICC=cc;
  bool res=InitCommonControlsEx(&icc);
  if (!res) InitCommonControls();
  return res;
}

void awwPageControl::createparams(CreateParams &pars) {
  InitCommonControl(ICC_TAB_CLASSES);
  awwControl::createparams(pars);
  pars.style|=WS_CHILD;
  strcpy(pars.classname,WC_TABCONTROL);
  pars.style|=TCS_HOTTRACK;
}

awwPageControl::awwPageControl(PageControl *pager):awwControl(pager) {
  active=-1;
}

void awwPageControl::addsheet(awwPageSheet *sheet) {
  pages.add(sheet);//makeindexes();
//  if (win) sheet->doinsert();
  if (win) makeitems();
}

void awwPageControl::changed() {
  for(int i=0;i<pages.count;i++) pages[i]->_hide();
  if (awwPageSheet *page=getactive()) page->_show();
  ((PageControl*)ctrl)->dochange();
}

void awwPageControl::onselchange(awwMessage &msg) {
  changed();
}

void awwPageControl::makeitems() {
  SendMessage(win,TCM_DELETEALLITEMS,0,0);
  for(int i=0;i<pages.count;i++) {
    TC_ITEM item;
    zerorec(item);
    item.mask=TCIF_TEXT;
    char text[0x100];strcpy(text,pages[i]->text.c_str());
    item.pszText=text;
    SendMessage(win,TCM_INSERTITEM,i,(LPARAM)&item);
  }
}

void awwPageControl::createwindow() {
  awwControl::createwindow();
  for(int i=0;i<pages.count;i++) pages[i]->recreatewnd();
  makeitems();
  SendMessage(win,TCM_SETCURSEL,0,0);
  changed();
}

void awwPageControl::onsize(awwMessage &msg,int w,int h) {
  changed();
}

void awwPageSheet::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  pars.style|=WS_CHILD;
  if (!_isactive) pars.style&=~WS_VISIBLE;
  strcpy(pars.classname,"awwWindow");
  pars.parent=pager->win;
}

awwPageSheet::awwPageSheet(PageControl *owner,PageSheet *apage):awwControl(apage),page(apage) {
  _isactive=false;
  pager=dynamic_cast<awwPageControl*>(owner->getreal());
  if (pager) {
    parent=pager;
    pager->addsheet(this);
  }
}

void awwPageSheet::_hide() {
  if (!_isactive) return;
  ShowWindow(win,SW_HIDE);
  _isactive=false;
}
void awwPageSheet::_show() {
  _isactive=true;
  RECT r;
  GetClientRect(pager->win,&r);
  SendMessage(pager->win,TCM_ADJUSTRECT,0,(LPARAM)&r);
  SetWindowPos(win,NULL,r.left,r.top,r.right-r.left+1,r.bottom-r.top+1,SWP_NOZORDER|SWP_SHOWWINDOW);
}

void awwTrackBar::createparams(CreateParams &pars) {
  InitCommonControl(ICC_BAR_CLASSES);
  awwControl::createparams(pars);
  strcpy(pars.classname,TRACKBAR_CLASS);
  pars.style|=TBS_AUTOTICKS|TBS_BOTTOM|WS_CHILD;
  if (vertical) pars.style|=TBS_VERT;else pars.style|=TBS_HORZ;
}

void awwTrackBar::changed() {
  if (win) {
    SendMessage(win,TBM_SETRANGEMIN,0,min);
    SendMessage(win,TBM_SETRANGEMAX,0,max);
    SendMessage(win,TBM_SETPOS,1,cur);
    SendMessage(win,TBM_SETTICFREQ,freq,1);
  }
}

void awwTrackBar::updatethis() {
  if (win) {
    cur=SendMessage(win,TBM_GETPOS,0,0);
  }
}

void awwTrackBar::maybeonchange() {
  int lastcur=cur;
  updatethis();
  if (lastcur!=cur && ctrl) ((TrackBar*)ctrl)->dochange();
}

void awwTrackBar::onhscroll(awwMessage &msg,int code,int pos,HWND scroll) {
  awwControl::onhscroll(msg,code,pos,scroll);
  maybeonchange();
}
void awwTrackBar::onvscroll(awwMessage &msg,int code,int pos,HWND scroll) {
  awwControl::onvscroll(msg,code,pos,scroll);
  maybeonchange();
}




}


