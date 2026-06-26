//---------------------------------------------------------------------------

#include <windows.h>
#pragma hdrstop

#include "w32dci.h"

HINSTANCE W_hInstance=NULL;

void W_init(HINSTANCE hInstance) {
  W_hInstance=hInstance;
}

bool registerwindowclass(const char *name,WINDOWPROC wndproc,const char *flags) {
  WNDCLASS wc;
  ZeroMemory(&wc,sizeof(wc));
  wc.hInstance=W_hInstance;
  wc.hCursor=LoadCursor(0,IDC_ARROW);
  wc.hbrBackground=GetStockObject(strstr(flags,"[nobrush]")?NULL_BRUSH:LTGRAY_BRUSH);
  wc.style=CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
  wc.lpfnWndProc=wndproc;
  wc.lpszClassName=name;
  return RegisterClass(&wc);
}

int counteditheight(HFONT font) {
  TEXTMETRIC metric;
  HDC dc=GetDC(0);
  HFONT oldfont=SelectObject(dc,font);
  GetTextMetrics(dc, &metric);
  SelectObject(dc,oldfont);
  ReleaseDC(0,dc);
  return metric.tmHeight+8*GetSystemMetrics(SM_CYBORDER);
}

void WControl::setvarfont() {
  SendMessage(handle,WM_SETFONT,(int)varfont(),MAKELPARAM(true,0));
}

WControl::WControl(WControl parent,int x,int y,int w,int h,int id,const char *wndclass,int style,int exstyle) {
  handle=CreateWindowEx(exstyle,wndclass,0,WS_CHILD|WS_VISIBLE|style,x,y,w,h,parent.handle,(HMENU)id,W_hInstance,NULL);
  setvarfont();
}

WEdit::WEdit(WControl parent,int x,int y,int w,int id) {
  handle=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL,x,y,w,counteditheight(varfont()),parent.handle,(HMENU)id,W_hInstance,NULL);
  setvarfont();
}

WMemo::WMemo(WControl parent,int x,int y,int w,int h,int id) {
  handle=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",0,WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL|ES_MULTILINE,x,y,w,counteditheight(varfont()),parent.handle,(HMENU)id,W_hInstance,NULL);
  setvarfont();
}

WCheckBox::WCheckBox(WControl parent,int x,int y,int w,int h,int id) {
  handle=CreateWindow("BUTTON",0,WS_VISIBLE|WS_CHILD|BS_CHECKBOX,x,y,w,h,parent.handle,(HMENU)id,W_hInstance,NULL);
  setvarfont();
}

int WControl::gettextlen() {
  return SendMessage(handle,WM_GETTEXTLENGTH,0,0);
}
void WControl::settext(const char *text) {
  SendMessage(handle,WM_SETTEXT,0,(LPARAM)text);
}
void WControl::gettext(char *buf,int size) {
  SendMessage(handle,WM_GETTEXT,size,(LPARAM)buf);
}

/*
WRadioGroup::WRadioGroup(WControl parent,int x,int y,int w,int h,const char *items) {
  int xs=5,ys=5;int h0=
  char buf[0x1000];strcpy(buf,items);
  char *c=buf;
  char *list[0x100],**lw=list;
  while (*c) {
    *lw++=c;
    while (*c && *c!=';') c++;
    if (*c==';') *c++=0;
  }
  count=lw-list;
  int elemhi=(h-2*ys)/count;
  group=WGroupBox(parent,x,y,w,h);
  for(int i=0;i<count;i++) {
    this->items[i]=WRadioButton(parent,x+xs,y+ys+i*elemhi,w-2*xs,elemhi,i);
    this->items[i].settext(list[i]);
  }
}
*/

#pragma package(smart_init)
