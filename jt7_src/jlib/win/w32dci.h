#ifndef w32dciH
#define w32dciH

/*
#ifndef defined_string_class
struct string {
  char s[0x100];
  string(const char *src) {strcpy(s,src);}
  const char *c_str() {return s;}
};
#endif
*/

inline HFONT varfont() {return GetStockObject(ANSI_VAR_FONT);}

struct WControl {
  HWND handle;
  WControl(HWND handle=0):handle(handle) {}
  WControl(WControl parent,int x,int y,int w,int h,int id,const char *wndclass,int style,int exstyle=0);
  int gettextlen();
  void settext(const char *text);
  void gettext(char *buf,int size);
  void setvarfont();
  void destroy() {if (handle) DestroyWindow(handle);handle=NULL;}
  void setprop(const char *name,void *data) {SetProp(handle,name,(HANDLE)data);}
  void setprop(ATOM atom,void *data) {SetProp(handle,MAKEINTATOM(atom),(HANDLE)data);}
  void *getprop(const char *name) {return (void*)GetProp(handle,name);}
  void *getprop(ATOM atom) {return (void*)GetProp(handle,MAKEINTATOM(atom));}
  void show() {ShowWindow(handle,SW_SHOW);}
  void hide() {ShowWindow(handle,SW_HIDE);}
  void enable() {EnableWindow(handle,true);}
  void disable() {EnableWindow(handle,false);}
  void update() {UpdateWindow(handle);}
  operator HWND () {return handle;}
  void setmodified(bool modified) {SendMessage(handle,EM_SETMODIFY,modified,0);}
  bool getmodified() {return SendMessage(handle,EM_GETMODIFY,0,0);}
};

typedef LRESULT CALLBACK (*WINDOWPROC)(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

bool registerwindowclass(const char *name,WINDOWPROC wndproc,const char *flags="");
//flags:[nobrush]:pozadi je NULL_BRUSH

struct WWindow:WControl {
  WWindow(HWND handle=NULL):WControl(handle) {}
  void maximize() {ShowWindow(handle,SW_MAXIMIZE);}
  void minimize() {ShowWindow(handle,SW_MINIMIZE);}
  void normalize() {ShowWindow(handle,SW_NORMAL);}
  void close() {if (handle) PostMessage(handle,WM_CLOSE,0,0);}
};

struct WEdit:WControl {
  WEdit(WControl parent,int x,int y,int w,int id);
};

template<class T> struct W_CheckAdapter:T {
  W_CheckAdapter(WControl parent,int x,int y,int w,int h,int id,const char *wndclass,int style,int exstyle=0)
    :T(parent,x,y,w,h,id,wndclass,style,exstyle)
  {}
  W_CheckAdapter(HWND handle=0):T(handle) {}
  bool ischecked() {return SendMessage(handle,BM_GETCHECK,0,0)==BST_CHECKED;}
  void setchecked(bool checked) {SendMessage(handle,BM_SETCHECK,checked?BST_CHECKED:BST_UNCHECKED,0);}
  void swap() {setchecked(!ischecked());}
};

struct WCheckBox:W_CheckAdapter<WControl> {
  WCheckBox(WControl parent,int x,int y,int w,int h,int id);
  WCheckBox(HWND handle):W_CheckAdapter<WControl>(handle) {}
};

struct WMemo:WControl {
  WMemo(HWND handle):WControl(handle) {}
  WMemo(WControl parent,int x,int y,int w,int h,int id);
  int getcur() {DWORD start,end;SendMessage(handle,EM_GETSEL,(WPARAM)&start,(LPARAM)&end);return start;}
  void setcur(int cur) {SendMessage(handle,EM_SETSEL,cur,cur);}
};

struct WGroupBox:WControl {
  WGroupBox(HWND handle):WControl(handle) {}
  WGroupBox(WControl parent,int x,int y,int w,int h,int id=-1):WControl(parent,x,y,w,h,id,"BUTTON",BS_GROUPBOX) {}
  WGroupBox() {}
};

struct WRadioButton:W_CheckAdapter<WControl> {
  WRadioButton(HWND handle):W_CheckAdapter<WControl>(handle) {}
  WRadioButton(WControl parent,int x,int y,int w,int h,int id):W_CheckAdapter<WControl>(parent,x,y,w,h,id,"BUTTON",BS_RADIOBUTTON) {}
  WRadioButton() {}
};

struct WListBox:WControl {
  WListBox(HWND handle):WControl(handle) {}
  WListBox(WControl parent,int x,int y,int w,int h,int id):WControl(parent,x,y,w,h,id,"LISTBOX",WS_VSCROLL,WS_EX_CLIENTEDGE) {}
  void add(const char *s) {SendMessage(handle,LB_ADDSTRING,0,(LPARAM)s);}
  void erase(int index) {SendMessage(handle,LB_DELETESTRING,index,0);}
  int count() {return SendMessage(handle,LB_GETCOUNT,0,0);}
  void clear() {SendMessage(handle,LB_RESETCONTENT,0,0);}
  void insert(int pos,const char *s) {SendMessage(handle,LB_INSERTSTRING,pos,(LPARAM)s);}
  int getselected() {return SendMessage(handle,LB_GETCURSEL,0,0);}
  void setselected(int index) {SendMessage(handle,LB_SETCURSEL,index,0);}
};

struct WCommonComboBox:WControl {
  WCommonComboBox(HWND handle):WControl(handle) {}
  WCommonComboBox(WControl parent,int x,int y,int w,int h,int id,int style):WControl(parent,x,y,w,h,id,"COMBOBOX",WS_VSCROLL|style,WS_EX_CLIENTEDGE) {}
  void add(const char *s) {SendMessage(handle,CB_ADDSTRING,0,(LPARAM)s);}
  void erase(int index) {SendMessage(handle,CB_DELETESTRING,index,0);}
  void clear() {SendMessage(handle,CB_RESETCONTENT,0,0);}
  int count() {return SendMessage(handle,CB_GETCOUNT,0,0);}
  void insert(int pos,const char *s) {SendMessage(handle,CB_INSERTSTRING,pos,(LPARAM)s);}
  int getselected() {return SendMessage(handle,CB_GETCURSEL,0,0);}
  void setselected(int index) {SendMessage(handle,CB_SETCURSEL,index,0);}
  int getlistitemheight() {return SendMessage(handle,CB_GETITEMHEIGHT,0,0);}
  int geteditheight() {return SendMessage(handle,CB_GETITEMHEIGHT,-1,0);}
};

struct WPackList:WCommonComboBox {
  WPackList(HWND handle):WCommonComboBox(handle) {}
  WPackList(WControl parent,int x,int y,int w,int h,int id):WCommonComboBox(parent,x,y,w,h,id,CBS_DROPDOWNLIST) {}
};

struct WButton:WControl {
  WButton(HWND handle):WControl(handle) {}
  WButton(WControl parent,int x,int y,int w,int h,int id):WControl(parent,x,y,w,h,id,"BUTTON",0) {}
  WButton() {}
};

struct WLabel:WControl {
  WLabel(HWND handle):WControl(handle) {}
  WLabel(WControl parent,int x,int y,int w,int h,int id=-1):WControl(parent,x,y,w,h,id,"STATIC",0) {}
  WLabel() {}
};

struct WCanvas {
  HDC dc;
  WCanvas(HDC dc):dc(dc) {}
  int getw() {return GetDeviceCaps(dc,HORZRES);}
  int geth() {return GetDeviceCaps(dc,VERTRES);}
  void release(HWND win) {ReleaseDC(win,dc);}
};

/*
struct WRadioGroup:WControl {
  WRadioGroup(WControl parent,int x,int y,int w,int h,const char *items);

  WGroupBox group;
  int count;
  WRadioButton items[0x20];
};
*/

extern HINSTANCE W_hInstance;

void W_init(HINSTANCE hInstance);
int counteditheight(HFONT font);


#endif
