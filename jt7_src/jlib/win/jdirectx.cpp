#include <winlib.h>
#pragma hdrstop

#include "jdirectx.h"

namespace wlib {

void DirectDrawWrapper::free() {
  if(lpDD != NULL)
  {
    if(lpDDSPrimary != NULL)
    {
      lpDDSPrimary->Release();
      lpDDSPrimary = NULL;
    }
    lpDD->Release();
    lpDD = NULL;
  }
}

bool DirectDrawWrapper::init(HANDLE hwnd,int w,int h,int bits) {
  HRESULT ddrval;
  DDSURFACEDESC ddsd;
  DDSCAPS ddscaps;

  ddrval = DirectDrawCreate(NULL, &lpDD, NULL);
  if(ddrval == DD_OK)
  {
    // Get exclusive mode
    ddrval = lpDD->SetCooperativeLevel(hwnd,
      DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    if(ddrval == DD_OK)
    {
      ddrval = lpDD->SetDisplayMode(w,h,bits);
      if(ddrval == DD_OK)
      {
        // Create the primary surface with 1 back buffer
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                              DDSCAPS_FLIP |
                              DDSCAPS_COMPLEX;
        ddsd.dwBackBufferCount = 1;
        ddrval = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL);
        if(ddrval == DD_OK) {
          // Get a pointer to the back buffer
          ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
          ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps,
                                                &lpDDSBack);
          if(ddrval == DD_OK) return true;
        }
      }
    }
  }
  return false;
}

void DirectDrawWrapper::flip() {
  while(1)
  {
    HRESULT ddrval;
    ddrval = lpDDSPrimary->Flip(NULL, 0);

    if(ddrval == DD_OK)
      break;

    if(ddrval == DDERR_SURFACELOST)
    {
      ddrval = lpDDSPrimary->Restore();
      if(ddrval != DD_OK)
        break;
    }

    if(ddrval != DDERR_WASSTILLDRAWING)
      break;
  }
}


bool DirectDrawWrapper::getbackdc() {
  return lpDDSBack->GetDC(&backDC) == DD_OK;
}
void DirectDrawWrapper::releasebackdc() {
  lpDDSBack->ReleaseDC(backDC);
  backDC=NULL;
}

IDirectDrawSurface *DirectDrawWrapper::createbitmap(int w,int h) {
  if (!lpDD) return NULL;
  DDSURFACEDESC ddsd;
  IDirectDrawSurface *pdds;
  ZeroMemory(&ddsd,sizeof(ddsd));
  ddsd.dwSize=sizeof(ddsd);
  ddsd.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
  ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN;
  ddsd.dwWidth=w;
  ddsd.dwHeight=h;
  if (lpDD->CreateSurface(&ddsd,&pdds,NULL)!=DD_OK) return NULL;
  return pdds;
}
void DirectDrawWrapper::transparentblit(IDirectDrawSurface *surface,int destx,int desty,int srcx,int srcy,int w,int h) {
  RECT rcRect;
  rcRect.left = destx;
  rcRect.top = desty;
  rcRect.right = destx+w;
  rcRect.bottom = desty+h;
  lpDDSBack->BltFast(destx, desty, surface, &rcRect, DDBLTFAST_SRCCOLORKEY);
//  lpDDSBack->BltFast(destx, desty, surface, &rcRect, DDBLTFAST_NOCOLORKEY);
}

}
#pragma package(smart_init)

