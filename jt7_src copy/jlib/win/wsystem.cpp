#include <winlib.h>
#pragma hdrstop

#include "wsystem.h"

namespace wlib {

WSystem *createwsystem() {return new WSystem();}

void initbasedir() {
  char buf[0x200];
  GetModuleFileName(0,buf,sizeof(buf));
  basedir=buf;basedir--;
}

struct WinMemBlock:JMemBlock {
  HLOCAL block;
  WinMemBlock(int size) {
    block=LocalAlloc(LMEM_MOVEABLE|LMEM_DISCARDABLE,size);
    xassert(block);
  }
  void *lock() {return LocalLock(block);}
  void unlock() {LocalUnlock(block);}
  int getsize() {return LocalSize(block);}
  void resize(int size) {block=LocalReAlloc(block,size,LMEM_MOVEABLE|LMEM_DISCARDABLE);xassert(block);}
  ~WinMemBlock() {LocalFree(block);}
  JMemBlock *clone() {
    int size=getsize();
    WinMemBlock *res=new WinMemBlock(size);
    void *dest=res->lock(),*src=lock();
    memcpy(dest,src,size);
    unlock();res->unlock();
    return res;
  }
};

JMemBlock *WSystem::tempblock(int size) {
//  return new WinMemBlock(size);
  return new StdMemBlock(size);
}

void WSystem::showinfo(string msg) {MessageBox(getactparent(),lang_text_ch(msg).c_str(),lang_text_ch("$Information").c_str(),MB_OK|MB_ICONINFORMATION);}
void WSystem::showerror(string msg) {MessageBox(getactparent(),lang_text_ch(msg).c_str(),lang_text_ch("$Error").c_str(),MB_OK|MB_ICONERROR);}
//void WSystem::showerror(string msg) {MessageBox(0,msg.c_str(),"Chyba",MB_OK|MB_ICONERROR);}
void WSystem::showwarning(string msg) {MessageBox(getactparent(),lang_text_ch(msg).c_str(),lang_text_ch("$Warning").c_str(),MB_OK|MB_ICONWARNING);}
string WSystem::ask(string question,string def) {
  return "";
}

bool WSystem::editvalues(Array<EditItem*> &items,string msg) {
  return false;
}

bool WSystem::makedir(jlib::STRING path) {
  return CreateDirectory(path,NULL);
}

void WSystem::copyfile(jlib::STRING oldname,jlib::STRING newname) {
  CopyFile(oldname,newname,false);
}

bool WSystem::getfilename(string initialdir,string filter,bool save,char buf[0x200]) {
  char flt[0x100],*w=flt;
  memmove(w,filter.c_str(),filter.length()),w+=filter.length();
  *w++=0;
  memmove(w,filter.c_str(),filter.length()),w+=filter.length();
  *w++=0;*w=0;
//  char flt[]="*.*\0*.*\0\0";
  *buf=0;
  OPENFILENAME o;
  char *c=reinterpret_cast<char*>(&o);
  for(int i=0;i<sizeof(o);i++,c++) *c=0;
  if (save) o.Flags=OFN_EXPLORER|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST|OFN_NOREADONLYRETURN;
  else o.Flags=OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOREADONLYRETURN;
  o.lStructSize=76;
  o.lpstrFilter=flt;
  o.lpstrFile=buf;
  o.nMaxFile=0x200;
  o.lpstrInitialDir=initialdir.c_str();
  o.lpstrDefExt="";
  o.hwndOwner=getactparent();
  return ((save && GetSaveFileName(&o)) || (!save && GetOpenFileName(&o)));
}

bool WSystem::getopenfn(string filter,string initialdir,string &res) {
  char buf[0x200];
  if (getfilename(initialdir,filter,false,buf)) {
    res=buf;return true;
  }
  return false;
}
bool WSystem::getsavefn(string filter,string initialdir,string &res) {
  char buf[0x200];
  if (getfilename(initialdir,filter,true,buf)) {
    string ext=sFile(filter).getext();
    res=sFile(buf)%ext.c_str();
    return true;
  }
  return false;
}

bool WSystem::edittext(string &text,int *position) {
  return false;
}

bool WSystem::dialogyn(string text) {
  return MessageBox(getactparent(),lang_text_ch(text).c_str(),lang_text_ch("$Question").c_str(),MB_YESNO)==IDYES;
}
int WSystem::dialogync(string text) {
  int res=MessageBox(getactparent(),lang_text_ch(text).c_str(),lang_text_ch("$Question").c_str(),MB_YESNOCANCEL);
  if (res==IDYES) return 0;
  if (res==IDNO) return 1;
  return 2;
}
bool WSystem::dialogar(string text) {
  return MessageBox(getactparent(),lang_text_ch(text).c_str(),lang_text_ch("$Question").c_str(),MB_YESNO)==IDRETRY;
}
bool WSystem::dialogoc(string text) {
  return MessageBox(getactparent(),lang_text_ch(text).c_str(),lang_text_ch("$Question").c_str(),MB_YESNO)==IDOK;
}

void WSystem::showwait(string msg) {
//  cblib::showwait();cblib::setwaittext(msg.c_str());
}
void WSystem::hidewait() {
//  cblib::hidewait();
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
  if (eqstr(fd.cFileName,".") || eqstr(fd.cFileName,"..")) return;
  FileRec &fr=files.add();
  strcpy(fr.name,fd.cFileName);
  fr.isdir=fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY;
  fr.time=0;
}

void WSystem::listfiles(jlib::STRING directory,TCntVec<FileRec> &files) {
  WIN32_FIND_DATA fd;
  HANDLE search=FindFirstFile(directory,&fd);
  if (search!=INVALID_HANDLE_VALUE) {
    waddfile(fd,files);
    while (FindNextFile(search,&fd)) waddfile(fd,files);
    FindClose(search);
  }
}

string WSystem::listfiles(jlib::STRING directory) {
  return w32list(directory,0);
}
string WSystem::listdirs(jlib::STRING directory) {
  return w32list(directory,FILE_ATTRIBUTE_DIRECTORY);
}


}

