#ifndef awwstdctrlsH
#define awwstdctrlsH

#include "awwcontrol.h"

namespace aww {

struct awwTextControl:awwControl {
  awwTextControl(Control *ctrl):awwControl(ctrl) {checked=false;}
  void settext(string atext);
  string gettext() {updatemytext();return makectext(text.c_str());}
  void setpos(Point pos,Size size);
  bool getchecked() {updatemychecked();return checked;}
  void setchecked(bool achecked);
protected:
  void updatemytext() {if (win) text=getwndtext();}
  string text;bool checked;
  bool specialwndproc() {return true;}
  void setfont(Font *afont);
  void createwindow();
  void ondestroywnd();
  void updatemypos();
  virtual void _repairsize(int neww,int newh) {}
  virtual void repairsize();

  void updatemychecked() {if (win) checked=WCheckBox(win).ischecked();}
};

struct awwEdit:awwTextControl {
  awwEdit(Edit *edit);
private:
  void repairsize();
protected:
  void onkeyup(awwMessage &msg,awsKey key);
  void onsetfocus(awwMessage &msg) {awwTextControl::onsetfocus(msg);selectall();}
  void createparams(CreateParams &pars);
//  void destroywnd() {updatemytext();awwTextControl::destroywnd();}
  Edit *edit;
};

struct awwLabel:awwTextControl {
  awwLabel(Label *label);
private:
  void _repairsize(int neww,int newh) {w=neww;h=newh;}
protected:
  void createparams(CreateParams &pars);
};

struct awwCheckBox:awwTextControl {
  awwCheckBox(CheckBox *label);
private:
  void _repairsize(int neww,int newh) {w=neww+20;h=newh;}
protected:
  void createparams(CreateParams &pars);
  void doclick() {((CheckBox*)ctrl)->doclick();}
  void onmouseup(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift);
};

struct awwGroupBox:awwTextControl {
  awwGroupBox(Control *groupbox):awwTextControl(groupbox) {}
private:
  void _repairsize(int neww,int newh) {}
protected:
  void getmargins(Margins &m);
  void createparams(CreateParams &pars);
  Rect getclient();
};

struct awwButton:awwControl {
  awwButton(Button *button):awwControl(button),button(button) {}
  void settext(string atext) {text=atext;if (win) win.settext(text.c_str());}
  string gettext() {updatemytext();return makectext(text.c_str());}
private:
  string text;
  Button *button;
  void updatemytext() {if (win) text=getwndtext();}
protected:
  void createparams(CreateParams &pars);
  bool specialwndproc() {return true;}
  void createwindow();
  void onmouseup(awwMessage &msg,int x,int y,MouseButton button,ShiftState shift);
};

struct awwMemo:awwControl {
  awwMemo(Memo *memo):awwControl(memo),memo(memo) {}
  void settext(string atext);
  string gettext() {updatemytext();return makectext(text.c_str());}
  void setcur(int index) {WMemo(win).setcur(index);}
  int getcur() {return WMemo(win).getcur();}
private:
  string text;
  Memo *memo;
  void updatemytext() {if (win) text=getwndtext();}
  void updatetext() {if (win) win.settext(text.c_str());}
  void onkeyup(awwMessage &msg,awsKey key);
protected:
  void createparams(CreateParams &pars);
  bool specialwndproc() {return true;}
  void createwindow() {awwControl::createwindow();updatetext();}
  void ondestroywnd() {updatemytext();awwControl::ondestroywnd();}
};

struct awwWrapLabel:awwControl {
  awwWrapLabel(WrapLabel *label):awwControl(label) {}
  void settext(string atext) {text=atext;updatetext();}
  string gettext() {return makectext(text.c_str());}
private:
  string text;
  void updatetext() {if (win) win.settext(text.c_str());}
protected:
  void createparams(CreateParams &pars);
  bool specialwndproc() {return true;}
  void createwindow() {awwControl::createwindow();updatetext();}
};


struct awwListControl:awwControl {
  int changecode;
  awwListControl(ListControl *list,int changecode):awwControl(list),changecode(changecode) {current=-1;}
  void setitems(string aitems) {items=aitems;updateitems();current=-1;}
  string getitems() {return items;}
  int getcur() {updatemycur();return current;}
  void setcur(int cur) {current=cur;updatecur();}
protected:
  int current;
  string items;
  virtual void updatemycur()=0;
  virtual void updatecur()=0;
  virtual void updateitems()=0;
protected:
  bool specialwndproc() {return true;}
  void createwindow();
  void dochange() {awsEvent msg;((ListControl*)ctrl)->onchange(msg);}
  void oncommand(awwMessage &msg,int id,int notifycode);
};

struct awwListBox:awwListControl {
  awwListBox(ListBox *list):awwListControl(list,LBN_SELCHANGE) {multiselect=false;multicol=false;colwi=0;}
  bool getmultiselect() {return multiselect;}
  void setmultiselect(bool value) {multiselect=value;}
  string getselected();
  void setselected(string value);
  void setsdp(string name,string value);
  string getsdp(string name);
  bool supportsdp(string name);
protected:
  TCntVec<int> selitems;
  bool multiselect;
  bool multicol;
  int colwi;

  void updatemysels();
  void updatesels();
  void updateitems();
  void createparams(CreateParams &pars);
  void createwindow();
  void updatemycur() {if (win) current=WListBox(win).getselected();}
  void updatecur() {if (win) WListBox(win).setselected(current);}
};

struct awwPackedList:awwListControl {
  awwPackedList(PackedList *list):awwListControl(list,CBN_SELCHANGE) {dropdowncount=8;}
protected:
  void updateitems();
  void createparams(CreateParams &pars);
  void updatemycur() {if (win) current=WPackList(win).getselected();}
  void updatecur() {if (win) WPackList(win).setselected(current);}
  int getdropdowncount() {return dropdowncount;}
  void setdropdowncount(int n) {dropdowncount=n;redropdown();}
  void createwindow() {awwListControl::createwindow();redropdown();}
  void redropdown();
  void updatepos() {setwindowpos(x,y,w,dropdownh);}
  void updatemypos();
protected:
  int dropdowncount;
private:
  int dropdownh;
};

struct awwPlace:awwControl {
  awwPlace(Control *place):awwControl(place) {}
protected:
  void createparams(CreateParams &pars);
};


}

#endif
