#include <jlib.h>
#pragma hdrstop

#include "editor.h"

namespace jlib {

void EDITOR::settext(const char *text) {
  lines.assignfrombuf(text);
  if (lines.count==0) lines+="";
  clearhistory();
  initposinfo();
}

void EDITOR::load(const char *file) {
  lines.load(file);
  if (lines.count==0) lines+="";
  clearhistory();
}

void EDITOR::undo() {
  if (inhistory) {
    if (acthistory>0) goprev();
  } else {
    fixact();
    inhistory=true;
    acthistory=history.count;
    goprev();
  }
}

void EDITOR::goprev() {
  xassert(acthistory>0);
  acthistory--;
  STATE pom;
  history[acthistory]->back(lastlines,laststate,lines,pom);
  loadstate(pom);
  saveactstate();
}
void EDITOR::gonext() {
  xassert(acthistory<history.count);
  STATE pom;
  history[acthistory]->forward(lastlines,laststate,lines,pom);
  loadstate(pom);
  acthistory++;
  saveactstate();
}

void EDITOR::redo() {
  if (not inhistory) return;
  if (acthistory<history.count) gonext();
}

void EDITOR::savestate(STATE &s) {
  s.actline=actline;s.isselected=isselected;
  s.topline=topline;s.leftcol=leftcol;s.actcol=actcol;
  s.selbeg=selbeg;s.selend=selend;
}

void EDITOR::loadstate(STATE &s) {
  actline=s.actline;isselected=s.isselected;
  topline=s.topline;leftcol=s.leftcol;actcol=s.actcol;
  selbeg=s.selbeg;selend=s.selend;
  repairints();
}

void EDITOR::clearhistory() {
  history.clear();
  inhistory=false;
  saveactstate();
}

void EDITOR::nohistory() {
  if (inhistory and acthistory<history.count) history.erase(acthistory,history.count-acthistory);
  inhistory=false;
}

void EDITOR::fixact() {
  if (inhistory) return;
  ACTION *a=new ACTION;history+=a;
  STATE pom;
  savestate(pom);
  a->assign(lastlines,laststate,lines,pom);
  saveactstate();
}

void EDITOR::ACTION::doaction(STRLIST &inlines,STATE &instate,STRLIST &outlines,STATE &outstate,R &src,R &tar) {
  outstate=tar.state;
  outlines.clear();
  outlines.add(inlines,0,begcnt);
  outlines.add(tar.s);
  outlines.add(inlines,begcnt+src.s.count,inlines.count);
}

void EDITOR::ACTION::assign(STRLIST &srcl,STATE &srcs,STRLIST &destl,STATE &dests) {
  begcnt=0,endcnt=0;
  for(int i=0;i<srcl.count and i<destl.count and eqstr(srcl[i],destl[i]);i++) begcnt++;
  if (begcnt<srcl.count and begcnt<destl.count)
    for(int i=1;i<=min(srcl.count,destl.count) and eqstr(srcl[srcl.count-i],destl[destl.count-i]);i++) endcnt++;
  src.s.clear();
  src.s.add(srcl,begcnt,srcl.count-endcnt);
  tar.s.clear();
  tar.s.add(destl,begcnt,destl.count-endcnt);
  src.state=srcs;tar.state=dests;
}

void EDITOR::assign(EDITOR &src) {
  lines.assign(src.lines);
  actline=src.actline;
  topline=src.topline;
  actcol=src.actcol;
  leftcol=src.leftcol;
  scr.resize(src.scr.wi,src.scr.hi);
  isselected=src.isselected;
  selstart=src.selstart;
  selact=src.selact;
  selbeg=src.selbeg;
  selend=src.selend;
  clearhistory();
}

string EDITOR::getsel() {
  if (isselected) {
    EDITPOS start=max(visbeg(),selbeg),stop=min(visend(),selend);
    MEMSTROUT mso;
    if (start.line<stop.line) {
      if (start.col<len(start.line)) mso<<lines[start.line]+start.col;
      mso<<"\n";
      for(int y=start.line+1;y<=stop.line-1;y++) mso<<lines[y]<<"\n";
      if (stop.col<len(stop.line)) {
        char *n=strnew(lines[stop.line]);
        n[stop.col]=0;
        mso<<n;
        free(n);
      } else mso<<lines[stop.line];
    } else if (start.col<len(start.line)) {
      if (stop.col<len(stop.line)) {
        char *n=strnew(lines[stop.line]);
        n[stop.col]=0;
        mso<<(n+start.col);
        free(n);
      } else mso<<lines[start.line]+start.col;
    }
    return mso.str();
  }
  return "";
}

void EDITOR::insert(const char *s) {
  char *c=(char*)s;
  while (*c) {
    char *begline=c;
    while (*c and *c!='\n') c++;
    if (*c=='\n') {
      *c=0;insatactline(begline);
      lines.insert(actline+1,lines[actline]+actcol+strlen(begline));
      lines[actline][actcol+strlen(begline)]=0;
      *c='\n';
      actline++;
      c++;actcol=0;
    } else insatactline(begline),actcol+=strlen(begline);
  }
  repairints();
  nohistory();
  fixact();
}

void EDITOR::erasesel() {
  if (not isselected) return;
  if (selend.line>selbeg.line) {
    lines.erase(selbeg.line+1,selend.line-selbeg.line-1);
    int l1=selbeg.line,l2=selbeg.line+1;
    int c1=selbeg.col,c2=selend.col;
    if (c2<len(l2)) {
      if (c1<len(l1)) lines[l1][c1]=0;
      insatline(lines[l2]+c2,l1,c1);
      lines.erase(l2);
    } else {
      lines.erase(l2);
      strdelete(lines[l1],c1,len(l1)-c1);
    }
  } else strdelete(lines[selbeg.line],selbeg.col,selend.col-selbeg.col);
  actline=selbeg.line;actcol=selbeg.col;
  isselected=false;
  repairints();
  nohistory();
  fixact();
}

int EDITOR::SCREEN::writeatline(int y,const char *c,ATTR a,int left) {
  while (left<0 and *c) left++,c++;
  if (left<0) return 0;
  ELEM *l=line(y);
  int x=left;
  int volne=wi-left;
  while (*c and volne>0) l[x++]=ELEM(*c++,a),volne--;
  return x;
}
void EDITOR::SCREEN::writeatline(int y,const char *s,ATTR a,int left,ELEM rest) {
  int x=writeatline(y,s,a,left);
  ELEM *l=_line(y);
  while(x<=wi) l[x++]=rest;
}

void EDITOR::setcurpos(int x,int y) {
  actline=topline+y;
  actcol=leftcol+x;
  repairints();
}

void EDITOR::insatline(const char *text,int line,int col) {
  int lt=strlen(text),actl=strlen(lines[line]);
  char *p=challoc(max(lt+actl+5,actcol+lt+5));//melo by stacit +1,ale pro jistotu
  strcpy(p,lines[line]);
  strinsert(p,text,col);
  lines.setallocatedbuf(line,p);
  nohistory();
  changedactline=true;
}

void EDITOR::keypress(char ch) {
  if (uchar(ch)>=' ') {
    erasesel();
    char ins[2];ins[0]=ch;ins[1]=0;
    insatactline(ins);
    actcol++;
    repairints();
    nohistory();
  }
}

void EDITOR::writetoscr() {
  for(int y=topline;y<topline+scr.hi;y++) scr.writeatline(y-topline,getline(y),stdattr,-leftcol,ELEM(' ',stdattr));
  showsel();
/*  ofstream f((basedir/"editor.txt").c_str());
  for(int y=0;y<scr.hi;y++) {
    for(int x=0;x<scr.wi;x++) f<<scr.at(x,y).c;
    f<<endl;
  }*/

}

void EDITOR::delatcursor(int cnt) {
  if (actcol<strlen(lines[actline])) strdelete(lines[actline],actcol,cnt);
  changedactline=true;
}

void EDITOR::showsel() {
  if (isselected) {
    EDITPOS start=max(visbeg(),selbeg),stop=min(visend(),selend);
    if (start.line<stop.line) {
      for(int x=max(start.col,leftcol);x<scr.wi+leftcol;x++) elemat(start.line,x).a=selattr;
      for(int y=start.line+1;y<=stop.line-1;y++) {
        for(int x=leftcol;x<scr.wi+leftcol;x++) elemat(y,x).a=selattr;
      }
      int end=min(scr.wi+leftcol,stop.col);
      for(int x=leftcol;x<end;x++) elemat(stop.line,x).a=selattr;
    } else {
      int beg=max(leftcol,start.col),end=min(scr.wi+leftcol,stop.col);
      for(int x=beg;x<end;x++) elemat(start.line,x).a=selattr;
    }
  }
}

void EDITOR::joinlines(int line1,int line2,int pos) {
  int l1=strlen(lines[line1]),l2=strlen(lines[line2]);
  char *p=challoc(pos+l2+1);
  strcpy(p,lines[line1]);
  strinsert(p,lines[line2],actcol);
  lines.erase(line2);
  lines.setallocatedbuf(line1,p);
  nohistory();
  fixact();
}

void EDITOR::outline() {
  trimr(lines[actline]);
  if (changedactline) fixact();
  changedactline=false;
}

void EDITOR::mousesetcurpos(int x,int y) {
  actline=topline+y;
  actcol=leftcol+x;
  if (lines.count==1 and actcol>len(0)) actcol=len(0);
  if (actline>=lines.count) {actline=lines.count-1;actcol=len(actline);}
  repairints();
}

void EDITOR::mousedown(int x,int y) {
  mousesetcurpos(x,y);
  selstart=actpos();selact=actpos();isselected=true;mouseselecting=true;setselbe();
}
bool EDITOR::mousemove(int x,int y) {
  if (mouseselecting) {
    mousesetcurpos(x,y);
    selact=actpos();
    setselbe();
    return true;
  }
  return false;
}
void EDITOR::mouseup(int x,int y) {
  mousesetcurpos(x,y);
  setselbe();
  mouseselecting=false;
}

void EDITOR::keydown(KEY key) {
  EDITPOS lastpos=actpos();
  if (key.key==kleft) actcol--;
  if (key.key==kright) actcol++;
  if (key.key==kup) outline(),actline--;
  if (key.key==kdown) outline(),actline++;
  if (key.key==khome) actcol=0;
  if (key.key==kend) actcol=strlen(lines[actline]);
  if (key.key==kpgup) outline(),actline-=(scr.hi-1);
  if (key.key==kpgdn) outline(),actline+=(scr.hi-1);
  if (key.key==kdel) {
    if (isselected) erasesel();
    else {
      if (actcol<strlen(lines[actline])) delatcursor(1);
      else if (actline<lines.count-1) joinlines(actline,actline+1,actcol);
    }
  }
  if (key.key==kbs) {
    if (actcol>0) actcol--,delatcursor(1);
    else if (actline>0) {
      actcol=strlen(lines[actline-1]);
      joinlines(actline-1,actline,strlen(lines[actline-1]));
      actline--;
    }
  }
  if (key.key==kenter) {
    erasesel();
    nohistory();
    fixact();
    if (actcol<strlen(lines[actline])) {
      lines.insert(actline+1,lines[actline]+actcol);lines[actline][actcol]=0;actline++;
    } else actline++,lines.insert(actline,"");
    actcol=0;
  }
  repairints();
  if (not key.shift) {if (isselected) fixact();isselected=false;}
  if (key.shift) {
    nohistory();
    if (isselected) {
      selact=actpos();
    } else {
      selstart=lastpos;
      selact=actpos();
      isselected=true;
      fixact();
    }
    setselbe();
  }
}

void EDITOR::setselbe() {
  if (selstart<selact) selbeg=selstart,selend=selact;else selbeg=selact,selend=selstart;
}

void EDITOR::initposinfo() {
  actline=0;topline=0;leftcol=0;actcol=0;isselected=false;inhistory=false;changedactline=false;
  mouseselecting=false;
}

EDITOR::EDITOR():selattr(0,15) {
  initposinfo();
  lines.add("");
}

void EDITOR::repairints() {
  if (actcol<0) actcol=0;
  if (actline<0) actline=0;
  if (actline>=lines.count) actline=lines.count-1;
  if (actcol<leftcol) leftcol=actcol;
  if (actcol>=leftcol+scr.wi) leftcol=actcol-scr.wi+1;
  if (actline<topline) topline=actline;
  if (actline>=topline+scr.hi) topline=actline-scr.hi+1;
}


}

#pragma package(smart_init)
