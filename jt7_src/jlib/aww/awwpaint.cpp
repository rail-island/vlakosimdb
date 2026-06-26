#include <winlib.h>
#pragma hdrstop

#include "awwpaint.h"
//#delbeg [ABAZ]
//#include <win/winblit.h>
//#delend [ABAZ]

namespace aww {

void awwBmp::resize() {
  if (hbmp) DeleteObject(hbmp);
  hbmp=NULL;
  if (bits<=0) {
    HDC sdc=GetDC(0);
    hbmp=CreateCompatibleBitmap(sdc,w,h);
    ReleaseDC(0,sdc);
  } else {
    hbmp=CreateBitmap(w,h,1,bits,NULL);
  }
  HDC dc=getdc();
  RECT r;
  r.left=0;r.top=0;r.right=w;r.bottom=h;
  HBRUSH br=CreateSolidBrush(0);
  FillRect(dc,&r,br);
  DeleteObject(br);
  releasedc(dc);
}

struct LoadedBitmap {
  HBITMAP bmp;
  BITMAPINFO *info;
};

static LoadedBitmap loadbitmap(string file) {
  BINRFILE fr(file.c_str());
  if (fr.eof) throw JError("File couldn't be open:"+file);
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  fr.rr(bfh);
  fr.rr(bih);
  int bits=bih.biBitCount;
  BITMAPINFO *bi=NULL;
  int pallen=0;
  if (bits==1) pallen=2;
  if (bits==4) pallen=16;
  if (bits==8) pallen=256;
  bi=(BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+pallen*sizeof(RGBQUAD));
  bi->bmiHeader=bih;
  if (pallen>0) fr.readdata((char*)bi+sizeof(BITMAPINFOHEADER),pallen*sizeof(RGBQUAD));
  void *initdata=fr.readtoeof();
  HDC sdc=GetDC(0);
  HBITMAP hbmp=CreateDIBitmap(sdc,&bih,CBM_INIT,initdata,bi,DIB_RGB_COLORS);
  ReleaseDC(0,sdc);
  free(initdata);
  LoadedBitmap res;
  res.bmp=hbmp;
  res.info=bi;
  return res;
}

/*
//#delbeg [ABAZ]
zbuf::Bitmap *awwPicture::loadbitmap(string file,int bits) {
  LoadedBitmap bmp=aww::loadbitmap(file);
  if (bits<0) bits=bmp.info->bmiHeader.biBitCount;
  if (bits==24) bits=32;
  int w=bmp.info->bmiHeader.biWidth;
  int h=bmp.info->bmiHeader.biHeight;
  zbuf::Bitmap *res=zbuf::creatergbbitmap(w,h,bits,false,4);
  HDC sdc=GetDC(0);
  HDC mdc=CreateCompatibleDC(sdc);
  ReleaseDC(0,sdc);
  HBITMAP oldbmp=SelectObject(mdc,bmp.bmp);
  if (bits==8 or bits==4) {
    int count=1<<bmp.info->bmiHeader.biBitCount;
    gr::Palette *pal=new gr::Palette(count);
    pal->copyfrom(bmp.info->bmiColors,true);
    wlib::readpalbmp(mdc,bmp.bmp,res,0,0,0,0,w,h,pal);
    res->setpalette(pal);
  } else {
    wlib::readrgbbmp(mdc,bmp.bmp,res,0,0,0,0,w,h);
  }
  SelectObject(mdc,oldbmp);
  DeleteDC(mdc);
  return res;
}
//#delend [ABAZ]
*/

void awwBmp::loadbmp(string file) {
  done();
  LoadedBitmap bmp=loadbitmap(file);
  hbmp=bmp.bmp;
  w=bmp.info->bmiHeader.biWidth;
  h=bmp.info->bmiHeader.biHeight;
  bits=bmp.info->bmiHeader.biBitCount;
  free(bmp.info);
}

void awwPicture::save(string file) {
  releasecanvas();
  int bytes=3;
  int alignedw=bmp.w;while (alignedw%4) alignedw++;
  int datasize=alignedw*bmp.h*bytes;
  void *buf=malloc(datasize);
  HDC dc=bmp.getdc();
  BITMAPINFO bmi;
  BITMAPINFOHEADER bih;
  BITMAPFILEHEADER bfh;

  zerorec(bmi);zerorec(bih);
  bih.biSize=sizeof(BITMAPINFOHEADER);
  bih.biWidth=bmp.w;
  bih.biHeight=bmp.h;
  bih.biPlanes=1;
  bih.biBitCount=bytes*8;
  bih.biCompression=BI_RGB;
  bih.biSizeImage=0;
  bmi.bmiHeader=bih;

  GetDIBits(dc,bmp.hbmp,0,bmp.h,buf,&bmi,DIB_RGB_COLORS);
  bmp.releasedc(dc);

  zerorec(bfh);
  bfh.bfType='B'+'M'*0x100;
  bfh.bfSize=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER)+datasize;
  bfh.bfOffBits=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);

  BINWFILE fw(file.c_str());
  fw.writedata(&bfh,sizeof(BITMAPFILEHEADER));
  fw.writedata(&bih,sizeof(BITMAPINFOHEADER));
  fw.writedata(buf,datasize);
  free(buf);
}

void vflipop(int *buf,int &w,int &h) {
  for(int y=0;y<h/2;y++) {
    int *l1=buf+(y*w),*l2=buf+(h-y-1)*w;
    for(int x=0;x<w;x++) exchange(l1[x],l2[x]);
  }
}

void hflipop(int *buf,int &w,int &h) {
  int *line=buf;
  for(int y=0;y<h;y++,line+=w) {
    for(int x=0;x<w/2;x++) exchange(line[x],line[w-x-1]);
  }
}
void invertop(int *buf,int &w,int &h) {
  int *act=buf;
  for(int y=0;y<h;y++)
    for(int x=0;x<w;x++,act++) {
    *act=makergb(0xFF-nativervalue(*act),0xFF-nativegvalue(*act),0xFF-nativebvalue(*act));
  }
}

void transpop(int *buf,int &w,int &h) {
  int *bakbuf=new int[w*h];
  for(int x=0;x<w;x++) for(int y=0;y<h;y++) bakbuf[(x*h)+y]=buf[(y*w)+x];
  memcpy(buf,bakbuf,sizeof(int)*w*h);
  delete []bakbuf;
  exchange(w,h);
}

void awwBmp::doop(void (*trop)(int *buf,int &w,int &h)) {
  BITMAPINFO bmi;
  zerorec(bmi);
  BITMAPINFOHEADER &bih=bmi.bmiHeader;
  bih.biSize=sizeof(BITMAPINFOHEADER);
  bih.biWidth=w;
  bih.biHeight=h;
  bih.biPlanes=1;
  bih.biBitCount=32;
  bih.biCompression=BI_RGB;
  bih.biSizeImage=0;
  HDC dc=getdc();
  int *buf=new int[w*h];
  GetDIBits(dc,hbmp,0,h,buf,&bmi,DIB_RGB_COLORS);
  trop(buf,w,h);
  if (w!=bih.biWidth || bih.biHeight!=h) {
    releasedc(dc);
    DeleteObject(hbmp);
    bih.biWidth=w;bih.biHeight=h;
    HDC sdc=GetDC(0);
    hbmp=CreateDIBitmap(sdc,&bih,CBM_INIT,buf,&bmi,DIB_RGB_COLORS);
    ReleaseDC(0,sdc);
  } else {
    SetDIBits(dc,hbmp,0,h,buf,&bmi,DIB_RGB_COLORS);
    releasedc(dc);
  }
  delete []buf;
}

void awwBmp::hflip() {
  doop(hflipop);
}
void awwBmp::invert() {
  doop(invertop);
}
void awwBmp::vflip() {
  doop(vflipop);
}
void awwBmp::transp() {
  doop(transpop);
}

void awwPicture::load(string afile) {
  releasecanvas();
  sFile file(afile);
  if (file.testext("bmp")) {
    bmp.done();
    bmp.loadbmp(file);
  }
}


Canvas awwPicture::getcanvas() {
  if (!canvas) {
    canvas=new awwCanvas(bmp.getdc());
  }
  return Canvas(&*canvas,0,0);
}
void awwPicture::releasecanvas() {
  if (canvas) {
    HDC dc=canvas->wdc.getdc();
    delete canvas;//uvolni se vsechny prostredky patrici canvasu
    canvas=NULL;
    bmp.releasedc(dc);
  }
}

/*
void awwPicture::blit(Canvas cnv) {
  xassert(canvas==NULL);
  if (awwCanvas *aww=dynamic_cast<awwCanvas*>(cnv.cnv.ptr())) bmp.drawto(aww->wdc.getdc(),cnv.dx,cnv.dy);
}
*/

void awwCanvas::setfont(Font *afont) {
  if (awwFont *f=dynamic_cast<awwFont*>(afont)) {
    wdc.setfont(f->gethandle());
    wdc.setfontcolor(afont->getcolor());
  }
}

//#delbeg [ABAZ]
//void awwCanvas::blitbmp(zbuf::Bitmap *bmp,int destx,int desty,int srcx,int srcy,int w,int h) {
//  wlib::blitbmp(wdc.getdc(),bmp,destx,desty,srcx,srcy,w,h);
//}
//#delend [ABAZ]

void awwCanvas::drawframe(const Rect &r,bool sunken) {
  RECT wr;convertrect(r,&wr);
//  int type=0;

  DrawFrameControl(wdc.getdc(),&wr,DFC_BUTTON,DFCS_BUTTONPUSH|(sunken?DFCS_PUSHED:0));

//  if (sunken) type=BDR_SUNKENINNER|BDR_RAISEDOUTER;else type=BDR_RAISEDINNER|BDR_SUNKENOUTER;
//  DrawEdge(wdc.getdc(),&wr,type,BF_RECT);
}

void awwCanvas::drawradio(int x,int y,bool checked) {
  int rad=6;
  RECT wr;wr.left=x-rad;wr.top=y-rad;wr.right=x+rad;wr.bottom=y+rad;
  DrawFrameControl(wdc.getdc(),&wr,DFC_BUTTON,DFCS_BUTTONRADIO|(checked?DFCS_CHECKED:0));
}

void awwCanvas::drawcheck(int x,int y,bool checked) {
  int rad=7;
  RECT wr;wr.left=x-rad;wr.top=y-rad;wr.right=x+rad;wr.bottom=y+rad;
  DrawFrameControl(wdc.getdc(),&wr,DFC_BUTTON,DFCS_BUTTONCHECK|(checked?DFCS_CHECKED:0));
}

void awwPaintControl::repainttomemdc() {
  HDC dc=bmp.getdc();
  RECT r;GetClientRect(win,&r);
  if (erasebkgnd) FillRect(dc,&r,GetStockObject(LTGRAY_BRUSH));
  repaintto(dc,NULL);
  bmp.releasedc(dc);
}

void awwPaintControl::repaint(const Rect &r) {
  if (!win) return;
  if (dblbuf==2) repainttomemdc();
  RECT wr;convertrect(r+Point(getcnvdx(),getcnvdy()),&wr);
  wr.right++;wr.bottom++;
  InvalidateRect(win,&wr,false);
}
void awwPaintControl::repaint() {
  if (!win) return;
  if (dblbuf==2) repainttomemdc();
  InvalidateRect(win,NULL,false);
}
void awwPaintControl::onsize(awwMessage &msg,int w,int h) {
  awwControl::onsize(msg,w,h);
  if (dblbuf==2) {
    bmp.resize(w,h);
    repainttomemdc();
  }
}
void awwPaintControl::updatedblbuf() {
  if (dblbuf==2) {
    bmp.resize(w,h);
    repainttomemdc();
  }
  if (dblbuf==0) bmp.done();
}

void awwPaintControl::repaintto(HDC dc,RECT *clip) {
  awsEvent msg;
  SetBkMode(dc,TRANSPARENT);
  awwCanvas cnv(dc);
  if (clip) cnv.clip=convertrect(*clip);//,cnv.clip.b.x++,cnv.clip.b.y++;
  HANDLE oldfont=SelectObject(dc,font->gethandle());
//  cnv.dx=getcnvdx();
//  cnv.dy=getcnvdy();
  pc->onpaint(Canvas(&cnv,getcnvdx(),getcnvdy()),msg);
  SelectObject(dc,oldfont);
}

void awwPaintControl::bufpaint(HDC dc,RECT &clip) {
  int clipw=clip.right-clip.left,cliph=clip.bottom-clip.top;
  RECT clip0;zerorec(clip0);clip0.right=clipw;clip0.bottom=cliph;
  bmp.wantsize(clipw,cliph);
  HDC mdc=bmp.getdc();
  HRGN rgn=CreateRectRgnIndirect(&clip0);
  SelectClipRgn(mdc,rgn);
  POINT pt;pt.x=pt.y=0;
  SetWindowOrgEx(mdc,clip.left,clip.top,&pt);
  if (erasebkgnd) FillRect(mdc,&clip,GetStockObject(LTGRAY_BRUSH));
  repaintto(mdc,&clip);
  SetWindowOrgEx(mdc,pt.x,pt.y,NULL);
  BitBlt(dc,clip.left,clip.top,clipw,cliph,mdc,0,0,SRCCOPY);
  DeleteObject(rgn);
  bmp.releasedc(mdc);
}

void awwPaintControl::onpaint(awwMessage &msg) {
  HDC dc;PAINTSTRUCT pi;
  dc=BeginPaint(win.handle,&pi);
//  if (pi.fErase) FillRect(dc,&pi.rcPaint,GetStockObject(LTGRAY_BRUSH));
  switch (dblbuf) {
    case 0:if (erasebkgnd) FillRect(dc,&pi.rcPaint,GetStockObject(LTGRAY_BRUSH));repaintto(dc,&pi.rcPaint);break;
    case 1:bufpaint(dc,pi.rcPaint);break;
    case 2:bmp.drawto(dc,0,0);break;
  }
  EndPaint(win.handle,&pi);
}
void awwPaintControl::createparams(CreateParams &pars) {
  awwControl::createparams(pars);
  pars.style|=WS_CHILD|WS_CLIPCHILDREN;
  strcpy(pars.classname,"awwWindow_nobrush");
//  strcpy(pars.classname,"awwWindow");
}

//bool _bitblt(RealCanvas *cnv,const Point &dest,const Rect &src,int rop);
//bool _stretchblt(RealCanvas *cnv,const Rect &dest,const Rect &src,int rop);

bool awwPicture::_bitblt(RealCanvas *cnv,const Point &dest,const Rect &src,int rop) {
  if (awwCanvas *aww=dynamic_cast<awwCanvas*>(cnv)) {
    HDC dc=bmp.getdc();
    BitBlt(aww->wdc.getdc(),dest.x,dest.y,src.size().w,src.size().h,dc,src.a.x,src.a.y,rop);
    bmp.releasedc(dc);
    return true;
  }
  return false;
}
bool awwPicture::_stretchblt(RealCanvas *cnv,const Rect &dest,const Rect &src,int rop) {
  if (awwCanvas *aww=dynamic_cast<awwCanvas*>(cnv)) {
    HDC dc=bmp.getdc();
    StretchBlt(aww->wdc.getdc(),dest.a.x,dest.a.y,dest.size().w,dest.size().h,
    dc,src.a.x,src.a.y,src.size().w,src.size().h,rop);
    bmp.releasedc(dc);
    return true;
  }
  return false;
}

bool awwPicture::dodraw(BltDrawOpts &o,RealCanvas *cnv) {
  releasecanvas();
  if (eqstr(o.rop,"copy")) return _bitblt(cnv,o.dest,o.src,SRCCOPY);
  if (eqstr(o.rop,"and")) return _bitblt(cnv,o.dest,o.src,SRCAND);
  return false;
}
bool awwPicture::dostretch(StretchDrawOpts &o,RealCanvas *cnv) {
  releasecanvas();
  if (eqstr(o.rop,"copy")) return _stretchblt(cnv,o.dest,o.src,SRCCOPY);
  if (eqstr(o.rop,"and")) return _stretchblt(cnv,o.dest,o.src,SRCAND);
  return false;
}
/*
void awwPicture::bitblt(Canvas cnv,Rect src,STRING rop,int rop_arg) {
  if (rop=="copy") _bitblt(cnv,src,SRCCOPY);
  if (rop=="and") _bitblt(cnv,src,SRCAND);
}
void awwPicture::stretchblt(Canvas cnv,Rect dest,Rect src,STRING rop,int rop_arg) {
  if (rop=="copy") _stretchblt(cnv,dest,src,SRCCOPY);
  if (rop=="and") _stretchblt(cnv,dest,src,SRCAND);
}
*/

void awwCanvas::repairrect(int &x1,int &y1,int &x2,int &y2) {
  if (x1>x2) exchange(x1,x2);
  if (y1>y2) exchange(y1,y2);
  x2++;y2++;
}

}


