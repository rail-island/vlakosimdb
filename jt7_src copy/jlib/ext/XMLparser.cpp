#include <jlib.h>
#pragma hdrstop

#include "XMLparser.h"
#include "XMLencoding.h"

namespace jlib {

void XMLPARSER::parsetag(XMLPARSERINTF *intf) {
  if (strbegcmp("<!--",act.act)) {act.jumpoverfirst("-->");return;}
  level++;
  act<<'<';
  str::readto(act.act,internalbuf,'>',sizeof(internalbuf));
  char *m=strchr(internalbuf,' ');
  char name[0x100];
  strecpy(name,internalbuf,m);
  xcheckstrlen(name);
  char end[0x100];sprintf(end,"</%s>",name);
  xcheckstrlen(end);
  if (intf) {
    wantvalue=false;
    XMLPARSERINTF *sub=intf->begintag(this,name);
    CHARANAL last=act;
    parselevel(sub,end);
    if (wantvalue) {
      MEMSTROUT mso;
      XMLenddecode(last.act,act.act,true,mso);
      intf->tagvalue(this,name,mso.str().c_str());
    }
    intf->endtag(this,name);
  } else parselevel(NULL,end);
  xxassert(strbegcmp(end,act.act),XMLERROR);
  act.act+=strlen(end);
  level--;
}

void XMLPARSER::parselevel(XMLPARSERINTF *intf,const char *end) {
  if (intf) intf->beginthis(this);
  while (true) {
    act.readtonojmp('<');
    if (strbegcmp(end,act.act)) break;
    parsetag(intf);
  }
  if (intf) intf->endthis(this);
}

void XMLPARSER::parse(const char *axml,XMLPARSERINTF *intf) {
  level=0;
  xml=(char*)axml;
  act.act=xml;
  act<<'<'<<'>';//preskocit hlavicku
  parsetag(intf);
}

}

#pragma package(smart_init)
