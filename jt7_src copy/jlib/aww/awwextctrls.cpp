#include <winlib.h>
#pragma hdrstop

#include "awwextctrls.h"

namespace aww {

void _ScrollBar::onscroll(awwMessage &msg,int code,int apos,HWND hscroll) {
    switch (code) {
      case SB_LINEUP: scroll(pos-dpos*koef);break;
      case SB_LINEDOWN: scroll(pos+dpos*koef);break;
      case SB_PAGEUP: scroll(pos-page*koef);break;
      case SB_PAGEDOWN: scroll(pos+page*koef);break;
      case SB_THUMBPOSITION: scroll(apos*koef);break;
      case SB_THUMBTRACK: scroll(apos*koef);break;
      case SB_TOP: scroll(0);break;
      case SB_BOTTOM: scroll(sum);break;
      case SB_ENDSCROLL: {} break;
    }
//    if (code==SB_THUMBPOSITION) {
//      boxc->defproc(msg);msg.handled=true;
//      scroll(GetScrollPos(hwnd,barcode));
//      scroll(apos);
//    }
/*  if (sum>65535) {//32 bit
    if (code==SB_THUMBPOSITION) scroll(GetScrollPos(hwnd,barcode));
  } else {
    if (code==SB_THUMBTRACK or code==SB_THUMBPOSITION) scroll(apos);
  }*/
}

bool _ScrollBar::scroll(int apos) {
  if (apos+page>=sum) apos=sum-page;
  if (apos<0) apos=0;
  int oldpos=pos;
  pos=apos;
  if (pos==oldpos) return false;
  SetScrollPos(hwnd,barcode,pos/koef,true);
  if (hor) box->hscroll(-(pos-oldpos));
      else box->vscroll(-(pos-oldpos));
  return true;
//  if (hor) ScrollWindow(hwnd,-(pos-oldpos),0,NULL,NULL),box->onhscroll();
//      else ScrollWindow(hwnd,0,-(pos-oldpos),NULL,NULL),box->onvscroll();
}

void _ScrollBar::setrange(int apage,int asum) {
  koef=1;
  int smallpage=apage,smallsum=asum;
  while (smallsum>65535) smallpage/=2,smallsum/=2,koef*=2;
//  while (smallsum>100) smallpage/=2,smallsum/=2,koef*=2;
  page=apage;sum=asum;
  SCROLLINFO si;
  si.cbSize=sizeof(si);
  si.fMask=SIF_ALL;
  si.nMin=0;
  si.nMax=smallsum;
  si.nPage=smallpage;
  si.nPos=si.nTrackPos=pos;
  SetScrollInfo(hwnd,barcode,&si,true);
  int newpos=pos;
  if (newpos+page>sum) newpos=sum-page;
  if (newpos<0) newpos=0;
  scroll(newpos);
}

void awwScrollBox::hscroll(int dx) {
  ScrollWindow(win,dx,0,NULL,NULL);
  sb->dohscroll();
}
void awwScrollBox::vscroll(int dy) {
  ScrollWindow(win,0,dy,NULL,NULL);
  sb->dovscroll();
}

/*void awwScrollBox::onhscroll(awwMessage &msg,int code,int pos,HWND scroll) {
  if (code==SB_THUMBTRACK or code==SB_THUMBPOSITION) {
    SetScrollPos(win,SB_HORZ,pos,true);
    ScrollWindow(win,10,0,NULL,NULL);
  }
}
void awwScrollBox::onvscroll(awwMessage &msg,int code,int pos,HWND scroll) {
}*/

void awwScrollBox::countbounds() {
  inw=inh=0;
  for(int i=0;i<childs.count;i++) {
    Rect r(childs[i]->getpos(),childs[i]->getsize());
    if (r.b.x>=inw) inw=r.b.x;
    if (r.b.y>=inh) inh=r.b.y;
  }
}

void awwScrollBox::onsize(awwMessage &msg,int w,int h) {
  awwControl::onsize(msg,w,h);setbounds();
}

void awwScrollBox::setbounds() {
  Rect r=getclient();
  hor->setrange(r.size().w,inw);
  ver->setrange(r.size().h,inh);
}

static void makevis(int &pos,int len,int beg,int end) {
  if (end>pos+len) pos=end-len;
  if (beg<pos) pos=beg;
}

bool awwScrollBox::scrollinview(const Rect &r) {
  Size s=getclient().size();
  int newx=hor->pos,newy=ver->pos;
  makevis(newx,s.w,r.a.x,r.b.x);
  makevis(newy,s.h,r.a.y,r.b.y);
  bool res=false;
  res|=hor->scroll(newx);
  res|=ver->scroll(newy);
  return res;
}

void awwScrollBox::createwindow() {
  awwControl::createwindow();
  hor->hwnd=ver->hwnd=win;
  countbounds();setbounds();
}

void awwScrollBox::onerasebkgnd(awwMessage &msg) {
  awwControl::onerasebkgnd(msg);
  if (denyerasebkgnd) msg.handled=true;
//  if (hor->sum>hor->page || ver->sum>ver->page) msg.handled=true;
}

void awwScrollBox::setsdp(string name,string value) {
  if (name=="denyerasebkgnd") denyerasebkgnd=value.ivalue();
  else if (name=="flat") isflat=value.ivalue();
  else awwControl::setsdp(name,value);
}

void awwScrollBox::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  pars.style|=WS_CHILD|WS_HSCROLL|WS_VSCROLL;
  if (!isflat) pars.exstyle|=WS_EX_CLIENTEDGE;
  strcpy(pars.classname,"awwWindow");
}

void awwScrollBox::setscrollpos(Point pt) {
  hor->scroll(pt.x);
  ver->scroll(pt.y);
}

Point awwScrollBox::getscrollpos() {
  return Point(hor->pos,ver->pos);
}

/*
****************************ScrolledPainter********************************
*/

void awwScrolledPainter::onsize(awwMessage &msg,int w,int h) {
  awwControl::onsize(msg,w,h);setbounds();
}

void awwScrolledPainter::setbounds() {
  Rect r=getclient();
  hor->setrange(r.size().w,inw);
  ver->setrange(r.size().h,inh);
}

bool awwScrolledPainter::scrollinview(const Rect &r) {
  Size s=getclient().size();
  int newx=hor->pos,newy=ver->pos;
  makevis(newx,s.w,r.a.x,r.b.x);
  makevis(newy,s.h,r.a.y,r.b.y);
  bool res=false;
  res|=hor->scroll(newx);
  res|=ver->scroll(newy);
  return res;
}

void awwScrolledPainter::onmousewheel(awwMessage &msg,int x,int y,int delta,ShiftState shift) {
  awwPaintControl::onmousewheel(msg,x,y,delta,shift);
  ver->scroll(-delta);
}

void awwScrolledPainter::createwindow() {
  awwControl::createwindow();
  hor->hwnd=ver->hwnd=win;
  setbounds();
}

void awwScrolledPainter::onerasebkgnd(awwMessage &msg) {
  awwControl::onerasebkgnd(msg);msg.handled=true;
//  if (denyerasebkgnd) msg.handled=true;
//  if (hor->sum>hor->page || ver->sum>ver->page) msg.handled=true;
}

//void awwScrolledPainter::setsdp(string name,string value) {
//  if (name=="denyerasebkgnd") denyerasebkgnd=value.ivalue();
//  else if (name=="flat") isflat=value.ivalue();
//  else awwControl::setsdp(name,value);
//}

void awwScrolledPainter::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  pars.style|=WS_CHILD|WS_HSCROLL|WS_VSCROLL;
  if (!isflat) pars.exstyle|=WS_EX_CLIENTEDGE;
  strcpy(pars.classname,"awwWindow");
}

void awwScrolledPainter::setscrollpos(Point pt) {
  hor->scroll(pt.x);
  ver->scroll(pt.y);
}

Point awwScrolledPainter::getscrollpos() {
  return Point(hor->pos,ver->pos);
}

void awwScrolledPainter::updatedblbuf() {

}

void awwScrolledPainter::onsetfocus(awwMessage &msg) {
  awwPaintControl::onsetfocus(msg);
  updatecaret();
}
void awwScrolledPainter::onkillfocus(awwMessage &msg) {
  DestroyCaret();
  wincarethi=-1;
  awwPaintControl::onkillfocus(msg);
}

void awwScrolledPainter::updatecaret() {
  if (!win) return;
  if (caretsize>=0) {
    if (wincarethi!=caretsize) {
      if (wincarethi>=0) DestroyCaret();
      CreateCaret(win,NULL,1,caretsize);
      wincarethi=caretsize;
    }
    updatecaretpos();
  } else {
    if (wincarethi>=0) DestroyCaret();
    wincarethi=-1;
  }
}

void awwScrolledPainter::updatecaretpos() {
  if (!win) return;
  ShowCaret(win);
  SetCaretPos(caretx-hor->pos,carety-ver->pos);
}


awwScrolledPainter::awwScrolledPainter(ScrolledPainter *scrollbox):awwPaintControl(scrollbox) {
  hor=new _ScrollBar(this,true);ver=new _ScrollBar(this,false);
  inw=inh=0;sb=scrollbox;seterasebkgnd(false);isflat=false;
  caretx=carety=0;caretsize=-1;wincarethi=-1;
}

void awwScrolledPainter::onpaint(awwMessage &msg) {
  if (wincarethi>=0) HideCaret(win);
  awwPaintControl::onpaint(msg);
  if (wincarethi>=0) updatecaretpos();
}

void awwScrolledPainter::hscroll(int dx) {
  if (wincarethi>=0) HideCaret(win);
  sb->dohscroll();
  HDC dc=GetDC(win);
  Size s=getclientsize();
  if (dx<0) {
//    BitBlt(dc,0,0,s.w+dx,s.h,dc,-dx,0,SRCCOPY);
    BitBlt(dc,0,0,s.w+dx-1,s.h,dc,-dx,0,SRCCOPY);
    RECT r;r.left=s.w+dx-1;r.top=0;r.right=s.w+1;r.bottom=s.h;
    InvalidateRect(win,&r,false);
  } else {
    BitBlt(dc,dx,0,s.w-dx,s.h,dc,0,0,SRCCOPY);
    RECT r;r.left=0;r.top=0;r.right=dx;r.bottom=s.h;
    InvalidateRect(win,&r,false);
  }
  ReleaseDC(win,dc);
  if (wincarethi>=0) updatecaretpos();
}
void awwScrolledPainter::vscroll(int dy) {
  if (wincarethi>=0) HideCaret(win);
  sb->dovscroll();
  HDC dc=GetDC(win);
  Size s=getclientsize();
  if (dy<0) {
    BitBlt(dc,0,0,s.w,s.h+dy-1,dc,0,-dy,SRCCOPY);
//    BitBlt(dc,0,0,s.w,s.h+dy,dc,0,-dy,SRCCOPY);
    RECT r;r.left=0;r.top=s.h+dy-1;r.right=s.w;r.bottom=s.h+1;
    InvalidateRect(win,&r,true);
  } else {
    BitBlt(dc,0,dy,s.w,s.h-dy,dc,0,0,SRCCOPY);
    RECT r;r.left=0;r.top=0;r.right=s.w;r.bottom=dy;
    InvalidateRect(win,&r,true);
  }
  ReleaseDC(win,dc);
  if (wincarethi>=0) updatecaretpos();
//  sb->dovscroll();
//  InvalidateRect(win,NULL,true);
}

Size awwScrolledPainter::getclientsize() {
  if (win) {
    RECT r;
    GetClientRect(win,&r);
    return Size(r.right-r.left+1,r.bottom-r.top+1);
  }
  return getsize();
}


/*
****************************RadioGroup*************************************
*/

void awwRadioButton::onmousedown(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift) {
  awwTextControl::onmousedown(msg,x,y,button,shift);
  ismousedown=true;
}

void awwRadioButton::onmouseup(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift) {
  awwTextControl::onmouseup(msg,x,y,button,shift);
  if (x>=0 && x<w && y>=0 && y<h && ismousedown) {
    group->pressed(this);
  }
  ismousedown=false;
}

void awwRadioButton::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  strcpy(pars.classname,"BUTTON");
  pars.style|=WS_CHILD|BS_RADIOBUTTON|BS_VCENTER;
  pars.parent=group->win;
}

void awwRadioGroup::setcur(int index) {
  if (!win) {pushedindex=index;return;}
  if (index>=0 && index<buts.count) pressed(buts[index]);
  else pressed(NULL);
}

void awwRadioGroup::pressed(awwRadioButton *but) {
//  int idx=buts.search(but);
//  xassert(idx>=0);
  pushedindex=-1;
  for(int i=0;i<buts.count;i++) {
    if (buts[i]==but) {
      buts[i]->setchecked(true);
      pushedindex=i;
    } else buts[i]->setchecked(false);
  }
}

void awwRadioGroup::createwindow() {
  awwGroupBox::createwindow();
  updateitems();
  setcur(pushedindex);
}

void awwRadioGroup::ondestroywnd() {
  awwGroupBox::ondestroywnd();
  for(int i=0;i<buts.count;i++) delete buts[i];
  childs.clear();
  buts.clear();
}

Size awwRadioGroup::getbestsize() {
  Size res;
  int cnt1=1;
  for(lineiter it=items.getlines('\n');*it;it++) {
    int actw=getfont()->getwidth(*it);
    if (actw>res.w) res.w=actw;
    cnt1++;
  }
  res.w+=40;
  res.h=getfont()->getheight()*cnt1+10;
  return res;
}

void awwRadioGroup::updateitems() {
  if (!win) return;
  for(int i=0;i<buts.count;i++) delete buts[i];
  buts.clear();
  for(lineiter iter=items.getlines('\n');iter.aredata();iter++) {
    awwRadioButton *but=new awwRadioButton(this,group);
    but->setparent(this);
    but->settext(lang_text_ch(*iter));
    buts.add(but);
  }
//  recreatechilds();
  resized();
}

void awwRadioGroup::resized() {
  if (!win) return;
  if (buts.count==0) return;
  Rect r=getclient();
  int hi=r.size().h/buts.count;
  for(int i=0;i<buts.count;i++) {
    buts[i]->setpos(Point(r.left(),hi*i+r.top()),Size(r.size().w,hi));
  }
}

}


