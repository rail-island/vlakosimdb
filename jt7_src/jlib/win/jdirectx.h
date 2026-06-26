#ifndef jdirectxH
#define jdirectxH

#include "ddraw.h"

namespace wlib {

//struct DirectXError {
//  char msg[0x100];
//  DirectXError(const char *amsg) {strcpy(msg,amsg);}
//};

struct DirectDrawWrapper {
  LPDIRECTDRAW lpDD;//DirectDraw object
  LPDIRECTDRAWSURFACE lpDDSPrimary;//DirectDraw primary surface
  LPDIRECTDRAWSURFACE lpDDSBack;//DirectDraw back surface
  HDC backDC;

  bool init(HANDLE hwnd,int w,int h,int bits);//hwnd:okno spojene s directx objektem
  void free();
  void flip();
  DirectDrawWrapper() {lpDD=NULL;lpDDSPrimary=NULL;lpDDSBack=NULL;backDC=NULL;}

  bool getbackdc();
  void releasebackdc();

  IDirectDrawSurface *createbitmap(int w,int h);
  void transparentblit(IDirectDrawSurface *surface,int destx,int desty,int srcx,int srcy,int w,int h);
};

}

#endif
