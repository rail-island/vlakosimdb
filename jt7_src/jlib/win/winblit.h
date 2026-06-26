#ifndef winblitH
#define winblitH

//#delbeg [ABAZ]

#include <zbuf/bmpwrap.h>

namespace wlib {

//void blitbmp(HDC destdc,zbuf::ZBitmap *src);
void blitbmp(HDC destdc,zbuf::PixelMap *src);
void blitbmp(HDC destdc,int w,int h,int bits,void *data);

void blitbmp(HDC destdc,zbuf::Bitmap *src,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth);
void blitpalbmp(HDC destdc,zbuf::Bitmap *src,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth,gr::Palette *pal);

void readrgbbmp(HDC srcdc,zbuf::Bitmap *dest,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth);
void readrgbbmp(HDC srcdc,HBITMAP srcbmp,zbuf::Bitmap *dest,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth);

void readpalbmp(HDC srcdc,zbuf::Bitmap *dest,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth,gr::Palette *pal);
void readpalbmp(HDC srcdc,HBITMAP srcbmp,zbuf::Bitmap *dest,int xdest,int ydest,int xsrc,int ysrc,int rectw,int recth,gr::Palette *pal);

}

//#delend [ABAZ]

#endif
