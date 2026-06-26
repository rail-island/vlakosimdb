#ifndef awwcomctrlsH
#define awwcomctrlsH

#include "awwcontrol.h"
#include "commctrl.h"

namespace aww {

class awwPageControl;

struct awwPageSheet:awwControl {
  awwPageSheet(PageControl *owner,PageSheet *page);
//  int index;
  string text;
  void settext(string atext) {text=atext;}
  string gettext() {return text;}
//  void doinsert();
  void createparams(CreateParams &pars);

  bool _isactive;
  void _hide();
  void _show();
  PageSheet *page;
protected:
  awwPageControl *pager;
};

struct awwPageControl:awwControl {
  awwPageControl(PageControl *pager);
  void addsheet(awwPageSheet *sheet);
  void getpages(TCntVec<PageSheet*> &res) {res.clear();for(int i=0;i<pages.count;i++) res.add(pages[i]->page);}
private:
  TCntVec<awwPageSheet*> pages;
//  void makeindexes() {for(int i=0;i<pages.count;i++) pages[i]->index=i;}
  void makeitems();
protected:
  int active;
//  void addchild(awwControl *child) {throw awsError("PageControl cannot have childs");}
  void createparams(CreateParams &pars);
  bool specialwndproc() {return true;}
//  void setfont(Font *afont);
  void createwindow();
  void onselchange(awwMessage &msg);
  void changed();
  void onsize(awwMessage &msg,int w,int h);
  PageSheet *add() {return 0;}
  awwPageSheet *getactive() {
    updateactive();
    if (active>=0) return pages[active];
    return NULL;
  }
  void updateactive() {
    if (!win) return;
    active=SendMessage(win,TCM_GETCURSEL,0,0);
  }
  void onmouseup(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift) {
    awwControl::onmouseup(msg,x,y,button,shift);
    onselchange(msg);
  }
  int getcur() {return active;}
  void setcur(int value) {active=value;if (win) SendMessage(win,TCM_SETCURSEL,active,0);changed();}
};

struct awwTrackBar:awwControl {
  awwTrackBar(TrackBar *bar):awwControl(bar) {cur=0;min=0;max=100;freq=10;vertical=false;}
  int getmin() {updatethis();return min;}
  void setmin(int value) {min=value;changed();}
  int getmax() {updatethis();return max;}
  void setmax(int value) {max=value;changed();}
  int getcur() {updatethis();return cur;}
  void setcur(int value) {cur=value;changed();}
  int getfrequency() {updatethis();return freq;}
  void setfrequency(int value) {freq=value;changed();}
  void onhscroll(awwMessage &msg,int code,int pos,HWND scroll);
  void onvscroll(awwMessage &msg,int code,int pos,HWND scroll);
  bool getvertical() {return vertical;}
  void setvertical(bool value) {vertical=value;}
private:
  int cur,min,max,freq;
  bool vertical;
  void changed();
  void updatethis();
  void maybeonchange();
protected:
  void createparams(CreateParams &pars);
  bool specialwndproc() {return true;}
  void createwindow() {awwControl::createwindow();changed();}
  void ondestroywnd() {awwControl::ondestroywnd();updatethis();}
};


}

#endif
