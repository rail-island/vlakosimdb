#ifndef awwfontH
#define awwfontH

#include "awwcontrol.h"
#include <aws/awsgr.h>

namespace aww {

inline int fonthash(LOGFONT &lf) {return xorhash(&lf,sizeof(LOGFONT));}

template<class T,class S> struct WinResMan {
  TCntVec<T*> res;
  T *search(const S &data,int hashcode) {
    for(int i=0;i<res.count;i++) if (res[i]->hashcode==hashcode && res[i]->match(data)) return res[i];
    return NULL;
  }
  void add(T *obj) {
    res.add(obj);
  }
  void erased(T *obj) {
    res.quickremove(obj);
  }
  WinResMan() {}
};

class WinFontMan;

struct WinFont:RCObject {
  LOGFONT lf;
  int hashcode;
  WinFontMan *manager;
  bool own;

  HFONT gethandle() {if (!handle) handle=CreateFontIndirect(&lf);return handle;}
  WinFont(WinFontMan *man,HFONT handle);
  WinFont(WinFontMan *man,const LOGFONT &font);
  ~WinFont();
  void update() {GetObject(handle,sizeof(LOGFONT),&lf);hashcode=fonthash(lf);}
  int getheight();
  int getwidth(string text);
  int getheight(RealCanvas *cnv);
  int getwidth(RealCanvas *cnv,string text);
  bool match(const LOGFONT &f) {return memcmp(&lf,&f,sizeof(f))==0;}
private:
  HFONT handle;
};

struct WinFontMan:WinResMan<WinFont,LOGFONT> {
  WinFontMan() {}
};

struct awwFont:aws::Font {
  awwSystem *system;
  int pixelsperinch;
  int color;
  RefBase<WinFont> wf;
  int getheight() {return wf->getheight();}
  int getwidth(string text) {return wf->getwidth(text);}
  int getheight(RealCanvas *cnv) {return wf->getheight(cnv);}
  int getwidth(RealCanvas *cnv,string text) {return wf->getwidth(cnv,text);}
  void assign(Font *src) {
    awwFont *f=dynamic_cast<awwFont*>(src);
    if (f) {
      wf=f->wf;
      color=f->color;
      pixelsperinch=f->pixelsperinch;
    }
  }
  HFONT gethandle() {return wf->gethandle();}
  awwFont(WinFont *wf,awwSystem *system);
  bool edit();
  void setwf(LOGFONT &f);
  Font *clone() {awwFont *res=new awwFont(NULL,system);res->assign(this);return res;}

  void setpixelsperinch(int pixs) {LOGFONT f=wf->lf;f.lfWidth=0;f.lfHeight*=(float)pixs/(float)pixelsperinch;pixelsperinch=pixs;setwf(f);}
//properties
  virtual int getsize() {return -MulDiv(wf->lf.lfHeight,72,pixelsperinch);}
  virtual void setsize(int size) {LOGFONT f=wf->lf;f.lfWidth=0;f.lfHeight=-MulDiv(size,pixelsperinch,72);setwf(f);}
  virtual int getcolor() {return color;}
  virtual void setcolor(int acolor) {color=acolor;}
  virtual string getname() {return wf->lf.lfFaceName;}
  virtual void setname(string name) {LOGFONT f=wf->lf;memchr(f.lfFaceName,sizeof(f.lfFaceName),0);strcpy(f.lfFaceName,name.c_str());setwf(f);}
  virtual bool getunderline() {return wf->lf.lfUnderline;}
  virtual void setunderline(bool underline) {LOGFONT f=wf->lf;f.lfUnderline=underline;setwf(f);}
  virtual bool getbold() {return wf->lf.lfWeight>=FW_BOLD;}
  virtual void setbold(bool bold) {LOGFONT f=wf->lf;f.lfWeight=(bold?FW_BOLD:FW_NORMAL);setwf(f);}
  virtual bool getitalic() {return wf->lf.lfItalic;}
  virtual void setitalic(bool italic) {LOGFONT f=wf->lf;f.lfItalic=italic;setwf(f);}
  virtual bool getstrikeout() {return wf->lf.lfStrikeOut;}
  virtual void setstrikeout(bool strikeout) {LOGFONT f=wf->lf;f.lfStrikeOut=strikeout;setwf(f);}
};

}

#endif
