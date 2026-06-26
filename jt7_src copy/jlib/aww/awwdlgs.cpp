#include <winlib.h>
#pragma hdrstop

#include "awwdlgs.h"

namespace aww {

awwOpenDialog::awwOpenDialog(awwSystem *system):system(system) {
  multiselect=false;
}

static char *makewinfilter(string filter) {
  char *flt=new char[filter.length()+4];
  char *w=flt;
  const char *c=filter.c_str();
  while (*c) {
    if (*c=='~' || *c=='|') *w='\0';else *w=*c;
    w++;c++;
  }
  *w++=0;*w=0;
  return flt;
}

bool awwOpenDialog::execute() {
  char *flt=makewinfilter(lang_text_ch(filter));
  char buf[0x7FFF];
  *buf=0;
  strcpy(buf,file.c_str());
  OPENFILENAME o;
  zerorec(o);
  o.Flags=OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOREADONLYRETURN;
  if (multiselect) o.Flags|=OFN_ALLOWMULTISELECT;
  o.lStructSize=76;
  o.lpstrFilter=flt;
  o.lpstrFile=buf;
  o.nMaxFile=sizeof(buf);
  o.lpstrInitialDir=initialdir.c_str();
  o.lpstrDefExt="";
  o.hwndOwner=system->getparentwin();
  bool res=GetOpenFileName(&o);
  delete []flt;
  if (multiselect) {
    char *c=buf;
    bstring dir;
    while (*c) {
      dir<<*c;
      c++;
    }
    c++;
    if (*c) {
      MEMSTROUT mso;
      while (*c) {
        mso<<dir<<sFile::dirsep;
        while (*c) {
          mso<<*c;c++;
        }
        c++;
        mso<<'\n';
      }
      files=mso.str();
    } else {
      files=dir;
    }
  } else {
    file=buf;
  }
  return res;
}

awwSaveDialog::awwSaveDialog(awwSystem *system):system(system) {
}

bool awwSaveDialog::execute() {
  char *flt=makewinfilter(filter);
  char buf[0x7FFF];
  *buf=0;
  strcpy(buf,file.c_str());
  OPENFILENAME o;
  zerorec(o);
  o.Flags=OFN_EXPLORER|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST|OFN_NOREADONLYRETURN;
  o.lStructSize=76;
  o.lpstrFilter=flt;
  o.lpstrFile=buf;
  o.nMaxFile=sizeof(buf);
  o.lpstrInitialDir=initialdir.c_str();
  o.lpstrDefExt="";
  o.hwndOwner=system->getparentwin();
  bool res=GetSaveFileName(&o);
  delete []flt;
  file=buf;
  return res;
}

awwColorDialog::awwColorDialog(awwSystem *system):system(system) {
  color=gr::clnwhite;
}

bool awwColorDialog::execute() {
  static COLORREF custcolors[0x10];
  CHOOSECOLOR cc;
  zerorec(cc);
  cc.lStructSize=sizeof(cc);
  cc.hwndOwner=system->getparentwin();
  cc.rgbResult=color;
  cc.Flags=CC_RGBINIT|CC_FULLOPEN;
  cc.lpCustColors=custcolors;
  if (ChooseColor(&cc)) {
    color=cc.rgbResult;
    return true;
  }
  return false;
}



}

