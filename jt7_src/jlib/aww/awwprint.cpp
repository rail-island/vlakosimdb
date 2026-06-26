#include <winlib.h>
#pragma hdrstop

#include "awwprint.h"
#include "awwsystem.h"
#include <ext/hexstrm.h>
#include "awwfont.h"

namespace aww {

static int __stdcall awwAbortProc(HDC prn,int error) {
//  Application->ProcessMessages();
  return true;
//  return not winprn.aborted;
}



Canvas awwPrinter::getinfo() {
  wantic();
  return Canvas(&infocnv,0,0);
}
Canvas awwPrinter::beginpage() {
  checkprinting(true);
  checkw32i(StartPage(dc));
  return Canvas(&printcnv,0,0);
}
void awwPrinter::endpage() {
  checkprinting(true);
  checkw32i(EndPage(dc));
}
void awwPrinter::begindoc(STRING name,STRING file) {
  aborted=false;
  wantprn();
  checkprn();
  checkprinting(false);
  DOCINFO docinfo;
  wantdc();
  printing=true;
  zerorec(docinfo);
  docinfo.cbSize=sizeof(DOCINFO);
  docinfo.lpszDocName=name;
  docinfo.lpszOutput=file;
  if (file=="") docinfo.lpszOutput=NULL;
//  checkw32i(SetAbortProc(dc, (int(__stdcall*)()) awwAbortProc));
  checkw32i(SetAbortProc(dc,(ABORTPROC)awwAbortProc));
  checkw32i(StartDoc(dc, &docinfo));
//  beginpage();
}
void awwPrinter::enddoc() {
  checkprinting(true);
  if (not aborted) checkw32i(EndDoc(dc));
  printing=false;
  aborted=false;
}

void awwPrinter::loadprops(XMLNode xml) {
  string text=xml["win32.devmode"].svar("hexdump");
  if (text=="") {
    free_res();
    return;
  }
  HEXREAD hr(text);
  free_res();
  hr>>devmodesize;
  devmode=(DEVMODE*)malloc(devmodesize);
  short orient=DMORIENT_PORTRAIT;
  hr.readdata(devmode,devmodesize);
  if (xml.svar("orientation")=="landscape") orient=DMORIENT_LANDSCAPE;
  devmode->dmOrientation=orient;
  update();
}
void awwPrinter::saveprops(XMLNode xml) {
  xml.clear();
  xml.setname("printer");
  XMLNode ch=xml.add("win32.devmode");
  MEMHEXWRITE mhw;
  mhw.bytesperline=-1;
  mhw<<devmodesize;
  short orient=devmode->dmOrientation;
  if (orient==DMORIENT_PORTRAIT) xml.setvar("orientation","portrait");
  else xml.setvar("orientation","landscape");
  devmode->dmOrientation=DMORIENT_PORTRAIT;
  mhw.writedata(devmode,devmodesize);
  devmode->dmOrientation=orient;
  ch.setvar("hexdump",mhw.str());
}
int awwPrinter::getpgwi() {
  wantprn();
  return pagewi;
}
int awwPrinter::getpghi() {
  wantprn();
  return pagehi;
}
Font *awwPrinter::createprnfont(Font *src) {
  wantprn();
  if (awwFont *win=dynamic_cast<awwFont*>(src)) {
    awwFont *res=(awwFont*)win->clone();
    res->setpixelsperinch(logy);
    return res;
  }
  return src->clone();
}
awwPrinter::awwPrinter(Component *owner,awwSystem *system):Printer(owner,system),wsys(system) {
  init();
}
void awwPrinter::init() {
  dc=ic=NULL;hprn=NULL;devmode=NULL;devmodesize=0;*device=0;
  landscape=false;printing=false;
  pagewi=pagehi=0;
  logx=wsys->logpixelsx;
  logy=wsys->logpixelsy;
  koefx=koefy=1;
  mmkx=wsys->logpixelsx/25.4;
  mmky=wsys->logpixelsy/25.4;
  aborted=false;
}
void awwPrinter::free_prn() {
  if (hprn) ClosePrinter(hprn);
  if (dc) DeleteDC(dc);
  if (ic) DeleteDC(ic);
  printcnv.wdc.setdc(NULL);
  infocnv.wdc.setdc(NULL);
  hprn=NULL;dc=ic=NULL;
}
void awwPrinter::free_res() {
  if (hprn) ClosePrinter(hprn);
  if (devmode) free(devmode);
  if (dc) DeleteDC(dc);
  if (ic) DeleteDC(ic);
  printcnv.wdc.setdc(NULL);
  infocnv.wdc.setdc(NULL);
  init();
}

/*
inline void trimprnname(char * s) {
  while (*s==' ') s++;
  while (*s and *s!=',') s++;
  if (*s==',') *s=0;
}

string getdefprn() {
  char res[0x100];
  GetProfileString("windows","device","",res,0xFF);
  trimprnname(res);
  return string(res);
}
*/

void awwPrinter::wantdc() {
  if (dc) checkw32(DeleteDC(dc));
  checkw32(dc=CreateDC(NULL,device,NULL,devmode));
  printcnv.wdc.setdc(dc);
}

void awwPrinter::wantic() {
  wantprn();
  if (!ic) checkw32(ic=CreateIC(NULL,device,NULL,devmode));
  infocnv.wdc.setdc(ic);
}

void awwPrinter::firstupdate() {
  DEVMODE *olddev=devmode;int oldsize=devmodesize;
  devmodesize=DocumentProperties(0,hprn,device,NULL,NULL,0);
  devmode=(DEVMODE*)malloc(devmodesize);
  zeromem(devmode,devmodesize);
  if (olddev) memmove(devmode,olddev,min(devmodesize,oldsize));
  DocumentProperties(0,hprn,device,devmode,devmode,(olddev?DM_IN_BUFFER:0)|DM_OUT_BUFFER);
  update();
  free(olddev);
}

/*
bool awwPrinter::openprn(const char *prn) {
  strcpy(device,prn);
  if (OpenPrinter(device,&hprn,NULL)) {
    firstupdate();
    return true;
  } else {
    *device=0;hprn=0;return false;
  }
}
*/
bool awwPrinter::opendevmodeprn() {
  xassert(devmode);
  strcpy(device,(char*)devmode->dmDeviceName);
  if (OpenPrinter(device,&hprn,NULL)) {
    firstupdate();
    return true;
  } else {
    *device=0;hprn=0;return false;
  }
}
bool awwPrinter::opendefprn() {
  free_res();
  PAGESETUPDLG setup;
  zerorec(setup);
  setup.lStructSize=sizeof(setup);
  setup.Flags=PSD_RETURNDEFAULT;
  if (PageSetupDlg(&setup)) {
    DEVMODE *dev=(DEVMODE*)GlobalLock(setup.hDevMode);
    if (dev) {
      devmode=(DEVMODE*)malloc(dev->dmSize+dev->dmDriverExtra);
      devmodesize=dev->dmSize+dev->dmDriverExtra;
      memcpy(devmode,dev,devmodesize);
      GlobalUnlock(setup.hDevMode);
      opendevmodeprn();
      return true;
    }
  }
  return false;
}

void awwPrinter::detailsetup() {
  wantprn();
  DocumentProperties(0,hprn,device,devmode,devmode,DM_OUT_BUFFER|DM_IN_PROMPT|DM_IN_BUFFER);
  update();
}

void awwPrinter::setup() {
  wantprn();
  PAGESETUPDLG setup;
  zerorec(setup);
  setup.lStructSize=sizeof(setup);
  HANDLE dev=GlobalAlloc(GMEM_MOVEABLE,devmodesize);
  void *ptr=GlobalLock(dev);
  memcpy(ptr,devmode,devmodesize);
  GlobalUnlock(dev);
  setup.hDevMode=dev;
  if (PageSetupDlg(&setup)) {
    HANDLE dout=setup.hDevMode;
    ptr=GlobalLock(dout);
    if (ptr) {
      free(devmode);
      DEVMODE *dm=(DEVMODE*)ptr;
      devmodesize=dm->dmSize+dm->dmDriverExtra;
      devmode=(DEVMODE*)malloc(devmodesize);
      memcpy(devmode,dm,devmodesize);
      GlobalUnlock(dout);
      update();
    }
    GlobalFree(setup.hDevMode);
    GlobalFree(setup.hDevNames);
  }
  GlobalFree(dev);
}

void awwPrinter::allocprn() {
  if (devmode) opendevmodeprn();
  else opendefprn();
//  update();
}

void awwPrinter::update() {
  wantic();
  landscape=devmode->dmOrientation==DMORIENT_LANDSCAPE;
  checkprn();
  checkprinting(false);
  pagewi=GetDeviceCaps(ic,HORZRES);
  pagehi=GetDeviceCaps(ic,VERTRES);
  logx=GetDeviceCaps(ic,LOGPIXELSX);
  logy=GetDeviceCaps(ic,LOGPIXELSY);
  koefx=(double)wsys->logpixelsx/(double)logx;
  koefy=(double)wsys->logpixelsy/(double)logy;
  mmkx=logx/25.4;
  mmky=logy/25.4;
//  free_prn();
}

awwPrintDriver::awwPrintDriver(awwSystem *system):system(system) {
  prncnt=0;prns=NULL;
  enumprns();
}

void awwPrintDriver::enumprns() {
  free(prns);prns=NULL;prncnt=0;
  unsigned long bytes=0;
  EnumPrinters(PRINTER_ENUM_LOCAL,NULL,PRINTER_INFO_VERSION,NULL,0,&bytes,&prncnt);
  prns=(PRINTER_INFO_X*)malloc(bytes);
  if (not EnumPrinters(PRINTER_ENUM_LOCAL,NULL,PRINTER_INFO_VERSION,(uchar*)prns,bytes,&bytes,&prncnt)) return;
}


}



