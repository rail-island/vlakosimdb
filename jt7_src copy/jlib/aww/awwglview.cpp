#include <winlib.h>
#pragma hdrstop

#include "awwglview.h"
#include <opengl.h>

namespace aww {

awwGLView::awwGLView(GLView *glview):awwPlace(glview) {
  glv=glview;
  hdc=NULL;hglrc=NULL;
}

void awwGLView::wantdc() {
  if (hdc) return;
  hdc=GetDC(win.handle);

  int npfd;
  PIXELFORMATDESCRIPTOR pfd;

  zerorec(pfd);
  pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion=1;
  pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
  pfd.iPixelType=PFD_TYPE_RGBA;
  pfd.cColorBits=24;
  pfd.cDepthBits=32;
  pfd.iLayerType=PFD_MAIN_PLANE;
  npfd=ChoosePixelFormat(hdc,&pfd);
  SetPixelFormat(hdc,npfd,&pfd);
  DescribePixelFormat(hdc,npfd,sizeof(PIXELFORMATDESCRIPTOR),&pfd);
  hglrc=wglCreateContext(hdc);
//  wglMakeCurrent(hdc,hglrc);
}

void awwGLView::createwindow() {
  awwPlace::createwindow();
}
void awwGLView::ondestroywnd() {
  wglMakeCurrent(NULL,NULL) ;
  wglDeleteContext(hglrc);
  ReleaseDC(win.handle,hdc);
  hdc=NULL;hglrc=NULL;
  awwPlace::ondestroywnd();
}

void awwGLView::onpaint(awwMessage &msg) {
  awwPlace::onpaint(msg);
  HDC dc;PAINTSTRUCT pi;
  dc=BeginPaint(win.handle,&pi);
  EndPaint(win.handle,&pi);
  dorender();
}

void awwGLView::render() {
  InvalidateRect(win,NULL,false);
}

void awwGLView::dorender() {
  if (!hglrc) return;
  awsEvent ev;
  wglMakeCurrent(hdc,hglrc);
  glv->onpaintgl(ev);
  SwapBuffers(hdc);
  wglMakeCurrent(NULL,NULL);
}

void awwGLView::onsize(awwMessage &msg,int w,int h) {
  awwPlace::onsize(msg,w,h);
  wantdc();
  wglMakeCurrent(hdc,hglrc);
  glViewport(0,0,w,h);
  wglMakeCurrent(NULL,NULL);
}


}

aws::RealGLView *aws::createGLView(aws::System *system,GLView *glview) {
  return new aww::awwGLView(glview);
}

