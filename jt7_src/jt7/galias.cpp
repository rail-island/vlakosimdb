#include <jlib.h>
#pragma hdrstop

#include "galias.h"
#include <gram/gmanal.h>

namespace jtyc {

void Aliases::loadfile(string s,char mode) {
  gram::Analyser a(s.c_str());
  while (*a) {
    if (*a=='?') {
      a++;
      if (a.begins("loc")) mode='l';
      else if (a.begins("bld")) mode='b';
      else if (a.begins("bus") || a.begins("car")) mode='m';
      a.roll();
    } else {
      int size=0x100;
      char buf[0x100],*w=buf;
      while (!a.begins("==")) {
        if (*a=='\n') break;
        if (size<=1) break;
        *w++=*a;a++;
        size--;
      }
      *w=0;
      if (a.begjmp("==")) {
        int size2=0x100;
        char buf2[0x100],*w2=buf2;
        for(;;) {
          if (*a=='\n') break;
          if (size2<=1) break;
          *w2++=*a;a++;
          size2--;
        }
        *w2=0;
        trimr(buf2);
        ImageAlias *a=new ImageAlias();
        a->src=buf;a->dest=buf2;
        switch (mode) {
          case 'l':machines.add(a);break;
          case 'b':blds.add(a);break;
          case 'm':motocars.add(a);break;
        }
      }
      a.roll();
    }
  }//while
}

void Aliases::load() {
  TCntVec<FileRec> files;
  jsystem->listfiles(basedir/"objects"/"alias"/"*.*",files);
  for(int i=0;i<files.count;i++) {
    string s=loadtext(basedir/"objects"/"alias"/files[i].name);
    sFile sf=files[i].name;
    char mode='l';
    if (sf.testext("loc")) mode='l';
    if (sf.testext("bld")) mode='b';
    if (sf.testext("bus") || sf.testext("car")) mode='m';
    loadfile(s,mode);
  }
}


}


