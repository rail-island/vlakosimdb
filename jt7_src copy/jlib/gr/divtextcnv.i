#ifndef devtextcnvI
#define devtextcnvI

#define STDCALL __stdcall

class IPANEGRP;
struct IPANE {
  BACKINFO bi;
  virtual bool STDCALL isfirst()=0;
  virtual bool STDCALL islast()=0;
  virtual int STDCALL gethi()=0;
  virtual IPANEGRP * STDCALL getparent()=0;
};

struct IPANEGRP {
  BACKINFO bi;
  virtual IPANE * STDCALL getpane(int index)=0;
  virtual int STDCALL panecount()=0;
  virtual int STDCALL sheetCount(int pageHi)=0;
};

struct ILOGPAGE {
  BACKINFO bi;
  virtual bool STDCALL addPane(IPANE *pane)=0;
  virtual bool STDCALL canaddPane(IPANE *pane)=0;
  virtual int STDCALL getHi()=0;

  virtual bool STDCALL isblocked()=0;
  virtual bool STDCALL isfreebegin()=0;
  virtual bool STDCALL isfreeend()=0;

  virtual bool STDCALL isleft()=0;

  virtual void STDCALL addPaneGrp(IPANEGRP *panes)=0;
  virtual int STDCALL getExtraPageCount(IPANEGRP *panes)=0;

  virtual int STDCALL getPaneCount()=0;
  virtual IPANE * STDCALL getPane(int index)=0;

  virtual void STDCALL erasePane(int index,int count)=0;
  virtual bool STDCALL insertPane(int index,IPANE *pane)=0;

};

struct ILOGPAGES {
  BACKINFO bi;
  virtual ILOGPAGE * STDCALL addPage()=0;
  virtual ILOGPAGE * STDCALL getPage(int index)=0;
  virtual int STDCALL getpagehi()=0;
  virtual ILOGPAGE *last()=0;
  virtual int pageCount()=0;
};

#endif