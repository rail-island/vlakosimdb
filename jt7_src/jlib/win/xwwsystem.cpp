#include <winlib.h>
#pragma hdrstop

#include "jwswinsys.h"
#include <win/winblit.h>
#include <win/wsystem.h>

namespace wlib {
using namespace jws;

//static HDC hmaindc;
//static HWND hmainwin;

void WinSystem::repaint() {
  _wantrepaint=true;
//  PostMessage(hmainwin,WM_PAINT,0,0);
}
void WinSystem::exit() {
  PostQuitMessage(0);
}
WinSystem *jwswinsys=NULL;

static Key translatekey(int vk) {
  switch ((short)vk) {
    case VK_ESCAPE:return Key(Key::esc);
    case VK_BACK:return Key(Key::bs);
    case VK_RETURN:return Key(Key::enter);
    case VK_DELETE:return Key(Key::del);
    case VK_INSERT:return Key(Key::insert);
    case VK_LEFT:return Key(Key::left);
    case VK_RIGHT:return Key(Key::right);
    case VK_UP:return Key(Key::up);
    case VK_DOWN:return Key(Key::down);
    case VK_HOME:return Key(Key::home);
    case VK_END:return Key(Key::end);
  }
  return Key(Key::none);
}

int WinSystem::wndproc(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam) {
  switch (msg) {
    case WM_CLOSE:PostMessage(hwin,WM_QUIT,0,0);break;
    case WM_PAINT:if (dorepainting) {
//      repaintdesktop();
      bmpcnv.bmp->clear();
      desktop->repaintdesktop(bmpcnv);
      wlib::blitbmp(hmaindc,bmpcnv.bmp);
    } break;
    case WM_LBUTTONDOWN:desktop->mdown(LOWORD(lparam),HIWORD(lparam),mbleft);break;
    case WM_LBUTTONUP:desktop->mup(LOWORD(lparam),HIWORD(lparam),mbleft,false);break;
    case WM_RBUTTONDOWN:desktop->mdown(LOWORD(lparam),HIWORD(lparam),mbright);break;
    case WM_RBUTTONUP:desktop->mup(LOWORD(lparam),HIWORD(lparam),mbright,false);break;
    case WM_MOUSEMOVE:if (processmousemove) desktop->mmove(LOWORD(lparam),HIWORD(lparam),false);break;
    case WM_KEYDOWN:{
      Key k=translatekey(wparam);
      if (k.type!=Key::none) keydown(k);
    } break;
    case WM_CHAR:keydown(Key(Key::ascii,wparam));break;
    default:DefWindowProc(hwin,msg,wparam,lparam);break;
  }
  return 1;
}

static LRESULT CALLBACK wndproc(HWND hwin,UINT msg,WPARAM wparam,LPARAM lparam) {
  return jwswinsys->wndproc(hwin,msg,wparam,lparam);
};

}

using namespace wlib;

void WinSystem::displaychanged() {
  ShowWindow(hmainwin,SW_MAXIMIZE);
  int dispw=GetDeviceCaps(hmaindc,HORZRES),disph=GetDeviceCaps(hmaindc,VERTRES);
  desktop->setsize(dispw,disph);
  bmpcnv.setsize(dispw,disph);
  System::displaychanged();
}

WinSystem::WinSystem(void *hInstance) {
  jwsystem=this;
  GZREAD fr(basedir/"arial8.fnt");
  font=new zbuf::RasterFont(fr);
  initflags();

  WNDCLASS wc;zerorec(wc);
  wc.lpszClassName="jwsMainClass";
  wc.lpfnWndProc=wlib::wndproc;
  wc.hbrBackground=reinterpret_cast<void*>(COLOR_BTNFACE);
  wc.hCursor=LoadCursor(0,IDC_ARROW);

  RegisterClass(&wc);
  hmainwin=CreateWindow("jwsMainClass","jwsApplication",WS_POPUP,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,0,0,hInstance,NULL);
  hmaindc=GetDC(hmainwin);

  int dispw=GetDeviceCaps(hmaindc,HORZRES),disph=GetDeviceCaps(hmaindc,VERTRES);
  desktop->setsize(dispw,disph);
  bmpcnv.setsize(dispw,disph);
}


WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  jsystem=createwsystem();
  char buf[0x200];
  GetModuleFileName(0,buf,sizeof(buf));
  basedir=buf;basedir--;

  jwswinsys=new WinSystem(hInstance);

  jws_init();

  ShowWindow(jwswinsys->hmainwin,SW_MAXIMIZE);
  UpdateWindow(jwswinsys->hmainwin);

  jwswinsys->messageloop();
  ShowWindow(jwswinsys->hmainwin,SW_HIDE);
  jws_done();
  delete jwswinsys;
  delete jsystem;
  return 0;
}


void WinSystem::messageloop() {
  MSG msg;
  while (!_terminated) {
    if (_wantrepaint) {_wantrepaint=false;SendMessage(hmainwin,WM_PAINT,0,0);}
    bool handled=false;
    if (PeekMessage(&msg,0,0,0,PM_REMOVE)) {
      if (msg.message!=WM_PAINT) handled=true;
      if (msg.message==WM_QUIT) jwswinsys->_terminated=true;else {
        if (msg.message==WM_KEYDOWN and translatekey(msg.wParam).type==Key::none) TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    if (!handled) {
      if (!jwsystem->processmessages()) if (jwswinsys->onidle) jwswinsys->onidle();
    }
  }
}




//#pragma package(smart_init)
