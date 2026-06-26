#include <winlib.h>
#pragma hdrstop

#include "sdw_win32.h"

namespace sdw {

void JSDL_GetDisplaySize(int *w,int *h) {
  HDC dc=GetDC(0);
  *w=GetDeviceCaps(dc,HORZRES);
  *h=GetDeviceCaps(dc,VERTRES);
  ReleaseDC(0,dc);
}

void JSDL_InitBaseDir(int argc,char **argv) {
  char buf[0x200];
  GetModuleFileName(0,buf,sizeof(buf));
  basedir=buf;basedir--;
}

static string w32list(STRING directory,int attr) {
  WIN32_FIND_DATA fd;
  HANDLE search=FindFirstFile(directory,&fd);
  string res;
  if (search!=INVALID_HANDLE_VALUE) {
    if ((fd.dwFileAttributes&attr)==attr) res<<fd.cFileName<<";";
    while (FindNextFile(search,&fd)) if ((fd.dwFileAttributes&attr)==attr) res<<fd.cFileName<<";";
    FindClose(search);
  }
  return res;
}

static void waddfile(WIN32_FIND_DATA &fd,TCntVec<FileRec> &files) {
  FileRec &fr=files.add();
  strcpy(fr.name,fd.cFileName);
  fr.isdir=fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY;
  fr.time=0;
}

void JSDL_ListFiles(jlib::STRING directory,TCntVec<FileRec> &files) {
  WIN32_FIND_DATA fd;
  HANDLE search=FindFirstFile(directory,&fd);
  if (search!=INVALID_HANDLE_VALUE) {
    waddfile(fd,files);
    while (FindNextFile(search,&fd)) waddfile(fd,files);
    FindClose(search);
  }
}

string JSDL_ListFiles(jlib::STRING directory) {
  return w32list(directory,0);
}
string JSDL_ListDirs(jlib::STRING directory) {
  return w32list(directory,FILE_ATTRIBUTE_DIRECTORY);
}

void JSDL_MessageBox(const char *msg) {
  MessageBox(0,msg,"",0);
}

}
