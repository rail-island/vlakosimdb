#ifndef awwprintH
#define awwprintH

#include "awwobject.h"
#include <aws/awsprint.h>
#include <win/w32gr.h>
#include "awwpaint.h"

namespace aww {
using namespace aws;
class awwSystem;

#define PRINTER_INFO_VERSION 2
#define PRINTER_INFO_X PRINTER_INFO_2

class awwPrinter;

struct awwPrintCanvas:awwCanvas {
  awwPrintCanvas():awwCanvas(NULL) {}
};


/*
HPRINTER je alokovane porad
*/
struct awwPrinter:Printer {
  awwPrintCanvas printcnv,infocnv;
  void loadprops(XMLNode xml);
  void saveprops(XMLNode xml);
  void begindoc(STRING name,STRING file="");
  void enddoc();
  Canvas getinfo();
  Canvas beginpage();
  void endpage();
  int getpgwi();
  int getpghi();
  void setup();
  void detailsetup();
  Font *createprnfont(Font *src);
  awwPrinter(Component *owner,awwSystem *system);
  ~awwPrinter() {free_res();}

  HANDLE gethandle() {wantprn();return hprn;}
  int getppmmx() {wantic();return mmkx;}
  int getppmmy() {wantic();return mmkx;}
private:
  awwSystem *wsys;
  char device[0x100];
  HDC dc,ic;
  HANDLE hprn;
  DEVMODE *devmode;
  int devmodesize;
  bool landscape;
  bool printing;
  int pagewi,pagehi;
  int logx,logy;
  double koefx,koefy;///<1, prnx*koefx=screenx
  double mmkx,mmky;/// pocet pixelu na milimetr (mm*mmkx=pocet pixelu)
  bool aborted;

  void wantprn() {if (!hprn) allocprn();}
  void checkprn() {xassert(hprn);}
  void checkprinting(bool value) {xassert(value==printing);}
//  bool openprn(const char *prn);//otevre tiskarnu prn
//  bool openprn();//otevre tiskarnu v devmode
  bool opendevmodeprn();//otevre tiskarnu z nacteneho devmode
  bool opendefprn();///otevre implicitni tiskarnu
  void firstupdate();///alokuje devmode,nastavi supportorientation, bere za vzor dev
  void wantdc();
  void wantic();
  void update();
  void free_res();///uvolni a vynuluje vsechny zdroje
  void free_prn();///uvolni objekt tiskarny(ten se pak znova otevre pomoci opendevmodeprn)
  void init();
  void allocprn();
};

/*
struct awwPrinter:Printer {
  awwPrintCanvas printcnv,infocnv;
  void loadprops(XMLNode xml);
  void saveprops(XMLNode xml);
  void begindoc(STRING name,STRING file="");
  void enddoc();
  Canvas getinfo();
  Canvas beginpage();
  void endpage();
  int getpgwi();
  int getpghi();
  void setup();
  void detailsetup();
  Font *createprnfont(Font *src);
  awwPrinter(Component *owner,awwSystem *system);
  ~awwPrinter() {free_res();}
  HANDLE gethandle() {wantprn();return hprn;}
  int getppmmx() {wantic();return mmkx;}
  int getppmmy() {wantic();return mmkx;}
private:
  awwSystem *wsys;
  char device[0x100];
  HDC dc,ic;
  HANDLE hprn;
  DEVMODE *devmode;
  int devmodesize;
  bool landscape;
  bool printing;
  int pagewi,pagehi;
  int logx,logy;
  double koefx,koefy;///<1, prnx*koefx=screenx
  double mmkx,mmky;/// pocet pixelu na milimetr (mm*mmkx=pocet pixelu)
  bool aborted;

  void wantprn() {if (!hprn) allocprn();}
  void checkprn() {xassert(hprn);}
  void checkprinting(bool value) {xassert(value==printing);}
//  bool openprn(const char *prn);//otevre tiskarnu prn
//  bool openprn();//otevre tiskarnu v devmode
  bool opendevmodeprn();//otevre tiskarnu z nacteneho devmode
  bool opendefprn();///otevre implicitni tiskarnu
  void firstupdate();///alokuje devmode,nastavi supportorientation, bere za vzor dev
  void wantdc();
  void wantic();
  void update();
  void free_res();///uvolni a vynuluje vsechny zdroje
  void free_prn();///uvolni objekt tiskarny(ten se pak znova otevre pomoci opendevmodeprn)
  void init();
  void allocprn();
};
*/

struct awwPrintDriver {
  void enumprns();
  awwPrintDriver(awwSystem *system);
  ~awwPrintDriver() {free(prns);}
  int cnt() {return prncnt;}
  PRINTER_INFO_X &operator [](int index) {return getprn(index);}
private:
  awwSystem *system;
  PRINTER_INFO_X *prns;
  unsigned long prncnt;
  PRINTER_INFO_X &getprn(int index) {xcheckindex(index,prncnt);return *(prns+index);}
};


}

#endif
