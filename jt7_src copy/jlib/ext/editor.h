#ifndef editorH
#define editorH

namespace jlib {


struct EDITOR {
  struct EDITPOS {
    int line,col;
    EDITPOS(int aline,int acol) {line=aline;col=acol;}
    EDITPOS() {line=-1;col=0;}
  };

  struct STATE {
    bool isselected;
    int actline,topline,leftcol,actcol;
    EDITPOS selbeg,selend;
    STATE() {isselected=false;actline=0;topline=0;leftcol=0;actcol=0;}
  };

  struct ACTION {//reprezentuje prechod od jednoho stavu k druhemu a zpet
    struct R {
      STRLIST s;
      STATE state;
    };
    R src,tar;
    int begcnt,endcnt;
    void assign(STRLIST &srclines,STATE &srcstate,STRLIST &destlines,STATE &deststate);
    void forward(STRLIST &inlines,STATE &instate,STRLIST &outlines,STATE &outstate)
      {doaction(inlines,instate,outlines,outstate,src,tar);}
    void back(STRLIST &inlines,STATE &instate,STRLIST &outlines,STATE &outstate)
      {doaction(inlines,instate,outlines,outstate,tar,src);}
    void doaction(STRLIST &inlines,STATE &instate,STRLIST &outlines,STATE &outstate,R &src,R &tar);
  };

  struct ATTR {
    uchar a;
    ATTR(int fg,int bg) {a=(bg<<4)|(fg&0xF);}
    ATTR() {a=15;}
    int bg() {return a>>4;}
    int fg() {return a&0xF;}
  };

  struct ELEM {
    char c;
    ATTR a;
    ELEM(char ac,ATTR aa) {c=ac;a=aa;}
    ELEM(char ac,int fg,int bg):a(fg,bg) {c=ac;}
    ELEM() {c=' ';}
  };

  struct KEY {
    bool shift;
    int key;
    char ch;
    KEY() {shift=false;key=0;ch=0;}
  };
  static const int kchar=0,kup=1,kdown=2,kleft=3,kright=4,kpgdn=5,kpgup=6,khome=7,kend=8,
  kdel=9,kbs=10,kenter=11;

  struct SCREEN {
    int wi,hi;
    ELEM *buf;
    SCREEN(int awi,int ahi) {buf=NULL;resize(awi,ahi);}
    void resize(int awi,int ahi) {if (buf) delete []buf;hi=ahi;wi=awi;buf=new ELEM[wi*hi];}
    void clear(ELEM e) {for(ELEM *act=buf,*end=buf+wi*hi;act<end;act++) *act=e;}
    ~SCREEN() {if (buf) delete []buf;}
    SCREEN() {buf=NULL;wi=0;hi=0;}
    ELEM &at(int x,int y) {xcheckindex(x,wi);xcheckindex(y,hi);return buf[y*wi+x];}
    ELEM &operator () (int x,int y) {return buf[y*wi+x];}
    ELEM *_line(int y) {return buf+(y*wi);}
    ELEM *line(int y) {xcheckindex(y,hi);return _line(y);}
    int writeatline(int y,const char *s,ATTR a,int left);//vypise retezec s na radku y od pozice left
    //vrati x-ovou souradnici za poslednim mistem,kam psalo
    void writeatline(int y,const char *s,ATTR a,int left,ELEM rest);
    //vola writeatline vyse+vyplni zpytekradky rest
  };

  void keydown(KEY key);
  void keypress(char ch);
  void setcurpos(int x,int y);

  void mousedown(int x,int y);
  bool mousemove(int x,int y);//vraci,zda je potreba prekreslit
  void mouseup(int x,int y);

  void settext(const char *text);

  int curx() {return actcol-leftcol;}
  int cury() {return actline-topline;}

  virtual void writetoscr();//prepise cast lines,ktera se ma zobrazit,do scr
  ATTR stdattr,selattr;
  char *getline(int y) {return y<lines.count?lines[y]:"";}

  ELEM &elemat(int line,int col) {return scr.at(col-leftcol,line-topline);}

  EDITOR();
  SCREEN scr;
  STRLIST lines;

  EDITPOS actpos() {return EDITPOS(actline,actcol);}
  EDITPOS visbeg() {return EDITPOS(topline,0);}
  EDITPOS visend() {return EDITPOS(topline+scr.hi-1,scr.wi-1);}

  string getsel();
  void insert(const char *s);
  void erasesel();
  virtual void assign(EDITOR &src);

  void clearhistory();

  void undo();
  void redo();

  void load(const char *file);

  void savestate(STATE &s);
  void loadstate(STATE &s);
protected:
  bool changedactline;
  void goprev();
  void gonext();
  void nohistory();//prerusi prochazeni historie
  bool inhistory;//zda se prochazi historie
  void fixact();//ulozi aktualni stav jako stav,kam se da vratit historii
  //vola se po vetsi zmene v textu,aby se ulozilo jako krok do historie
  STATE laststate;
  STRLIST lastlines;
  IObjArray<ACTION> history;
  int acthistory;//pozice v history,pokud inhistory==true

  void initposinfo();
  void saveactstate() {savestate(laststate);lastlines.assign(lines);}

  bool isselected;
  bool mouseselecting;
  int actline,topline,leftcol,actcol;
  EDITPOS selbeg,selend;

  int len(int lineindex) {return strlen(lines[lineindex]);}
  EDITPOS selstart,selact;//selstart je misto,odkad se zacalo oznacovat,sel act=actpos()
  void repairints();//opravi actcol,leftcol,topline,actline,aby bylo v povolenych mezich
  void delatcursor(int cnt);
  void insatactline(const char *text) {insatline(text,actline,actcol);}
  void insatline(const char *text,int line,int col);
  void joinlines(int line1,int line2,int pos);//spoji radky line1 a line2,
  //vymaze radku line2,radka line2 je pridana na pos-tou pozici radky line1
  //obycejne line2=line1+1
  void outline();//akyz se opusti actline
  void showsel();//zvyrazni vybranout cast textu(dostane atribut selattr)
  void mousesetcurpos(int x,int y);
  void setselbe();//nastavi selbeg,selend 
public:
  int gettop() {return topline;}
  int gethi() {return scr.hi;}
  int getleft() {return leftcol;}
  int getwi() {return scr.wi;}
};

inline bool operator ==(const EDITOR::EDITPOS &p1,const EDITOR::EDITPOS &p2) {
  return p1.line==p2.line and p1.col==p2.col;
}
inline bool operator <(const EDITOR::EDITPOS &p1,const EDITOR::EDITPOS &p2) {
  return p1.line<p2.line or (p1.line==p2.line and p1.col<p2.col);
}
inline bool operator <=(const EDITOR::EDITPOS &p1,const EDITOR::EDITPOS &p2) {return p1<p2 or p1==p2;}
inline bool operator >(const EDITOR::EDITPOS &p1,const EDITOR::EDITPOS &p2) {return p2<p1;}
inline bool operator >=(const EDITOR::EDITPOS &p1,const EDITOR::EDITPOS &p2) {return p1>p2 or p1==p2;}

}
#endif
