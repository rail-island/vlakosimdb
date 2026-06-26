#include <jlib.h>
#pragma hdrstop

#include "charmixer.h"
#include "cmstrdata.h"
namespace jlib {

#define RUNDEBUG

CMARRAY *CHARMIXER::findarray(string name) {
  for(CMELEM **e=elemw-1;e>=elemstack;e--) if (CMARRAY *res=(*e)->findarray(name)) return res;
  return NULL;
}

string CHARMIXER::hasitems(string var) {
  if (CMARRAY *array=findarray(var)) {
    return btos(array->items.count>0);
  }
  return btos(false);
}

string CHARMIXER::ftos(double value) {
  char s[0x100];
  sprintf(s,"%f",value);
  if (strchr(s,'.')) {
    char *e=strend(s)-1;
    while (*e=='0') e--;
    if (*e!='.') e++;
    *e=0;
  }
  return string(s);
}


string CHARMIXER::calland(Strings &pars) {
  for(int i=0;i<pars.count;i++) if (not stob(pars[i])) return btos(false);
  return btos(true);
}
string CHARMIXER::callor(Strings &pars) {
  for(int i=0;i<pars.count;i++) if (stob(pars[i])) return btos(true);
  return btos(false);
}
string CHARMIXER::callsum(Strings &pars) {
  double res=0;
  for(int i=0;i<pars.count;i++) res+=atof(pars[i].c_str());
  return ftos(res);
}
string CHARMIXER::callmul(Strings &pars) {
  double res=1;
  for(int i=0;i<pars.count;i++) res*=atof(pars[i].c_str());
  return ftos(res);
}

bool CHARMIXER::stob(string s) {
  if (s=="true") return true;
  if (s=="false") return false;
  throw CMERROR(fmtstr("'%s' neni logicka hodnota",s.c_str()));
}

string CHARMIXER::callmacro(string name,Strings &params) {
  int idx=macros.searchtype(name.c_str());
  if (idx>=0) {
    #ifdef RUNDEBUG
    eprintf("Call macro:%s\n",name.c_str());
    #endif
    STATE st=getstate();
    MACRO &m=*macros[idx];
    VarList locs;
    locals=&locs;
    for(int i=0;i<min(m.params.count,params.count);i++)
      locs.setvar(m.params[i],params[i]);
    MEMSTROUT res;
    act=m.beg,end=m.end;
    out=&res;
    run();
    setstate(st);
    #ifdef RUNDEBUG
    eprintf("Return macro:%s\n",name.c_str());
    #endif
    return res.str();
  }
  return string("");
}

string CHARMIXER::call(string fn,Strings &params) {
  if (fn=="cat") {
    MEMSTROUT res;
    for(int i=0;i<params.count;i++) res<<params[i];
    return res.str();
  }
  if (fn=="lower") return lowercase(params[0]);
  if (fn=="upper") return uppercase(params[0]);
  if (fn=="array") return createSA(params);
  if (fn=="item") return SAitem(params[0],atoi(params[1].c_str()));
  if (fn=="count") return itos(SAcount(params[0]));
  if (fn=="lt") return btos(atoi(params[0].c_str())<atoi(params[1].c_str()));
  if (fn=="gt") return btos(atoi(params[0].c_str())>atoi(params[1].c_str()));
  if (fn=="eq") return btos(params[0]==params[1]);
  if (fn=="neq") return btos(params[0]!=params[1]);
  if (fn=="and") return calland(params);
  if (fn=="or") return calland(params);
  if (fn=="not") return btos(not stob(params[0]));
  if (fn=="sum") return callsum(params);
  if (fn=="mul") return callmul(params);
  if (fn=="fdiv") return ftos(atof(params[0].c_str())/atof(params[1].c_str()));
  if (fn=="div") return itos(atoi(params[0].c_str())/atoi(params[1].c_str()));
  if (fn=="mod") return itos(atoi(params[0].c_str())%atoi(params[1].c_str()));
  if (fn=="sub") return ftos(atof(params[0].c_str())-atof(params[1].c_str()));
  if (fn=="minus") return ftos(-atof(params[0].c_str()));
  if (fn=="hasitems") return hasitems(params[0]);
  if (fn=="defined") return btos(isdefined(params[0]));
  if (fn=="arraysize") {if (CMARRAY *a=findarray(params[0])) return itos(a->items.count);return itos(0);}
  if (fn=="filesize") return itos(getfilesize(outputdir/params[0].c_str()));
  if (fn=="round") return itos(round(atof(params[0].c_str())));
  if (customfuncs) return customfuncs(customparam,fn,params);
  return string("");
}

CMVAR *CHARMIXER::getvar(string name) {
  if (CMVAR *res=locals->searchvalue(name)) return res;
  if (CMVAR *res=globals.searchvalue(name)) return res;

  return NULL;
}

string CHARMIXER::getvalue(string name) {
  if (name=="item" and festack.count>0) return festack.last()->items[festack.last()->index];
  int up_level=0;
  char *c=(char*)name.c_str();
  while (*c=='.') c++,up_level++;
  if (up_level==0) {
    if (const char *value=locals->findvar(name)) return string(value);
    if (const char *value=globals.findvar(name)) return string(value);
  }
  for(CMELEM **e=elemw-1-up_level;e>=elemstack;e--) if (const char *value=(*e)->vars.findvar(c)) return string(value);
  return unknownvar(name);
}

bool CHARMIXER::isdefined(string what) {
  if (what=="last") {
    if (arrayw<=arraystack) {fatalerror("Testovani last mimo cyklus");return false;}
    return arrayw[-1].index==arrayw[-1].array->items.count-1;
  }
  if (what=="first") {
    if (arrayw<=arraystack) {fatalerror("Testovani first mimo cyklus");return false;}
    return arrayw[-1].index==0;
  }
  for(CMELEM **e=elemstack;e<elemw;e++) {
    if ((*e)->vars.searchvalue(what)) return true;
  }
  if (globals.searchvalue(what)) return true;
  if (locals->searchvalue(what)) return true;
  return false;
}

void CHARMIXER::jumptonextcom() {
  while (data()) {
    if (*act=='\\') {act++;if (data()) act++;}
    else if (isremark()) {act+=2;jmpnestedremark();}
    else if (iscommand()) {act+=2;return;}
    else act++;
  }
}

bool CHARMIXER::structurejump(const char *endcom,bool stoponelse) {
  int level=0;
  while (data()) {
    jumptonextcom();
    char *begcom=act;
    string com=readto(" ]",false);
    jumpto(']');
    if (com=="for" or com=="ifdef" or com=="ifndef" or com=="while" or com=="if" or com=="define") level++;else
    if (com=="endfor" or com=="endif" or com=="enddef" or com=="endwhile") {
      if (level) level--;else {
        if (not (com==endcom)) fatalerrorfmt("Spatne ukonceni bloku(ma byt %s,je %s)",endcom,com);
        return false;
      }
    } else if (level==0 and stoponelse) {
      if (com=="else") return false;
      if (com=="elif") {act=begcom;return true;}
    }
  }
  fatalerror("Neukonceny blok");
  return false;
}

void CHARMIXER::fatalerror(const char *msg) {
  *out<<"[FATAL ERROR:"<<msg<<"]";
  throw CMERROR(msg);
}
void CHARMIXER::fatalerrorfmt(const char *msg,...) {
  va_list args;
  va_start(args,msg);
  char s[0x1000];
  vsprintf(s,msg,args);
  va_end(args);
  fatalerror(s);
}

void CHARMIXER::jmpnestedremark() {
  int level=1;
  while (level and data()) {
    if (*act=='\\' and data(1)) act+=2;else
    if (*act=='#') {
      act++;
      if (*act==':') level++,act++;else if (*act=='.') level--,act++;
    } else act++;
  }
  if (level) fatalerror("neukoncena poznamka");
}

string CHARMIXER::readbrackets(char beg,char end) {
  if (*act!=beg) return fmtstr("[CHYBA:zde ma byt znak '%c']",beg);
  act++;
  char res[0x100],*rw=res;
  int size=0xFF;int level=0;
  while ( (level>0 or *act!=end) and data() and size) {
    if (*act==beg) level++;
    *rw++=*act++,size--;
  }
  if (*act==end) act++;else fatalerrorfmt("zde ma byt znak '%c']",end);
  *rw=0;
  return string(res);
}
string CHARMIXER::readto(const char *end,bool jumplastchar) {
  char res[0x100],*rw=res;
  int size=0xFF;
  while ( strchr(end,*act)==NULL and data() and size) *rw++=*act++,size--;
  if (strchr(end,*act)==NULL) fatalerrorfmt("zde ma byt jeden ze znaku '%s'",end);else if (jumplastchar) act++;
  *rw=0;
  return string(res);
}

string CHARMIXER::readandrunfunc(bool ismacro) {
  string fn=readexpr("(");
  if (*act=='(') act++;
  Strings params;
  while (*act!=')') {
    string par=readexpr("),");
    params.add(par.c_str());
    if (*act==',') act++;
  }
  if (*act==')') act++;

  if (ismacro) return callmacro(fn,params);
  return call(fn,params);
}

string CHARMIXER::readexpr(const char *end) {
  if (*act=='$') {act++;return getvalue(readto(end,false));}//promenna
  if (*act=='%') {act++;return readandrunfunc(false);}//funkce
  if (*act=='@') {act++;return readandrunfunc(true);}//makro
  if (*act=='"') {
    MEMSTROUT mso;
    act++;
    while (data()) {
      if (*act=='\\' and data(1)) mso<<STROUT::CHAR(act[1]),act+=2;
      else if (*act=='"') break;
      else mso<<STROUT::CHAR(*act),act++;
    }
    if (*act=='"') act++;
    return mso.str();
  }
  return readto(end,false);//obycejny retezec
}

void CHARMIXER::jumptocondition() {
  while (structurejump("endif",true)) {//bylo nalezeno elif
    Strings params;
    string com=readcomandparams(params);
    xassert(com=="elif");
    if (stob(params[0])) return;
  }
}
void CHARMIXER::runifdef(Strings &params,bool yes) {
  bool def=isdefined(params[0]);
  if (yes!=def)// def and not yes or not def and yes
    jumptocondition();
}
void CHARMIXER::runif(Strings &params) {
  if (not stob(params[0])) jumptocondition();
}
void CHARMIXER::runinclude(Strings &params) {
  STATE st=getstate();
  char *included=loadfile(inputdir/params[0]);
  act=included;end=strend(included);
  actfile=inputdir/params[0];
  run();
  setstate(st);
}
void CHARMIXER::rundefine(Strings &params) {
  MACRO *mac=new MACRO;
  mac->name=params[0];
  for(int i=1;i<params.count;i++) mac->params.add(params[i]);
//  mac->params.add(params,1,params.count);
  mac->beg=act;
  mac->file=actfile;
  #ifdef RUNDEBUG
  eprintf("Define:%s\n",mac->name.c_str());
  #endif
  structurejump("enddef",false);
  mac->end=act;
  macros.add(mac);
}
void CHARMIXER::runforeach(Strings &params) {
  FEREC *fe=new FEREC;
  decodeSA(params[0],fe->items);
  if (fe->items.count==0) {
    delete fe;
    structurejump("endforeach",false);
  } else {
    fe->beg=act;
    festack.add(fe);
  }
}
void CHARMIXER::runendforeach() {
  if (festack.count>0) {
    if (++festack.last()->index>=festack.last()->items.count) {
      festack.eraselast(1);
    } else act=festack.last()->beg;
  } else fatalerror("ukonceni neexistujiciho cyklu foreach");
}
void CHARMIXER::runfor(Strings &params) {
  CMARRAY *array=findarray(params[0]);
  if (array) {
    *arrayw++=AREC(array,this);
    if (array->items.count==0) {
      structurejump("endfor",false);
      arrayw--;
    } else {
      *elemw++=array->items[0];
    }
  } else fatalerrorfmt("Pole nenalezeno:%s",params[0]);
}
void CHARMIXER::runendfor() {
  if (arrayw>arraystack) {
    arrayw[-1].index++;
    if (arrayw[-1].index<arrayw[-1].array->items.count) {
      jump(arrayw[-1].beg);
      elemw[-1]=arrayw[-1].array->items[arrayw[-1].index];
    } else arrayw--,elemw--;
  } else fatalerror("ukonceni neexistujiciho cyklu for");
}

void CHARMIXER::runwhile(char *combegin,Strings &params) {
  *whilew++=combegin;
  if (not stob(params[0])) {
    structurejump("endwhile",false);
    whilew--;
  }
}
void CHARMIXER::runendwhile() {
  if (whilew>whilestack) {
    whilew--;
    act=*whilew;
  } else fatalerror("ukonceni neexistujiciho cyklu while");
}

string CHARMIXER::readcomandparams(Strings &params) {
  string res=readto(" ]",false);
  if (*act==' ') act++;
  while (*act!=']' and *act) {
    string par=readexpr("],");
    params.add(par);
    if (*act==',') act++;
  }
  if (*act==']') act++;
  return res;
}

void CHARMIXER::readlangstrs(Strings &strs) {
  while (*act!=']') {
    strs.add(readto("]|",false));
    if (*act=='|') act++;
  }
  if (*act==']') act++;
}

void CHARMIXER::runcom(char *commandbegin) {
  Strings params;
  string com=readcomandparams(params);
  if (com=="ifdef") runifdef(params,true);
  else if (com=="ifndef") runifdef(params,false);
  else if (com=="else") structurejump("endif",false);
  else if (com=="for") runfor(params);
  else if (com=="endfor") runendfor();
  else if (com=="if") runif(params);
  else if (com=="elif") structurejump("endif",false);
  else if (com=="while") runwhile(commandbegin,params);
  else if (com=="endwhile") runendwhile();
  else if (com=="save") savetextfile(outputdir/params[0],out->str().c_str());
  else if (com=="rewrite") out->clear();
  else if (com=="include") runinclude(params);
  else if (com=="define") rundefine(params);
  else if (com=="setoutdir") outputdir=prgdir/params[0];
  else if (com=="setindir") inputdir=prgdir/params[0];
  else if (com=="mkdir") mkdir(prgdir/params[0]);
  else if (com=="setglobal") globals.setvar(params[0],params[1]);
  else if (com=="setlocal") locals->setvar(params[0],params[1]);
  else if (com=="foreach") runforeach(params);
  else if (com=="endforeach") runendforeach();
  else if (com=="editglobals") {if (not doedit(&globals,params)) throw EditAborted();}
  else if (com=="editlocals") {if (not doedit(locals,params)) throw EditAborted();}
  else if (com=="inc") {CMVAR *v=getvar(params[0]);if (v) v->value=itos(atoi(v->value.c_str())+1);}
  else if (com=="dec") {CMVAR *v=getvar(params[0]);if (v) v->value=itos(atoi(v->value.c_str())-1);}
}

bool CHARMIXER::doedit(VarList *vars,Strings &pars) {
  if (pars.count==0) return false;
  IObjArray<JSystem::EditItem> items;
  int cnt=(pars.count-1)/2;
  string *strs=new string[cnt];

  for(int i=0;i<cnt;i++) {
    strs[i]=vars->findvalue(pars[1+i*2]);
    items.add(new JSystem::EditItem('s',strs+i,pars[2+i*2],""));
  }
  bool res=jsystem->editvalues(items,pars[0]);
  if (res) for(int i=0;i<cnt;i++) vars->setvar(pars[1+i*2],strs[i]);
  delete []strs;
  return res;
}

void CHARMIXER::run() {
  while (data()) {
    if (*act=='\\') {act++;if (data()) *out<<STROUT::CHAR(*act),act++;} //prejet znak
    else if (*act=='$' and act[1]=='(') {act+=2;*out<<readexpr(")");if (*act==')') act++;}
    else if (*act=='$' and act[1]=='[') {act++;*out<<getvalue(readbrackets('[',']'));}
    else if (*act=='%' and act[1]=='[') {act+=2;Strings pars;string name=readcomandparams(pars);*out<<call(name,pars);}
    else if (*act=='@' and act[1]=='[') {act+=2;Strings pars;string name=readcomandparams(pars);*out<<callmacro(name,pars);}
    else if (*act=='^' and act[1]=='[') {act+=2;Strings strs;readlangstrs(strs);string v=getvalue("lang");int i=atoi(v.c_str());if (i>=0 and i<strs.count) *out<<strs[i];}
    else if (*act=='_' and uchar(act[1])<=' ') {act++;while (data() and uchar(*act)<=' ') act++;}
    else if (iscommand()) {act+=2;runcom(act-2);}
    else if (isremark()) {act++;jmpnestedremark();}
    else *out<<STROUT::CHAR(*act++);
  }
}

static void _cec(char *buf,Strings &stack,string com) {
  if (stack.count==0) {
  };
}

void CHARMIXER::checkloadedfile(string name,string value) {
  char *buf=(char*)value.c_str();
  STATE st=getstate();
  act=buf;end=strend(buf);
  Strings stack;
  while (data()) {
    jumptonextcom();
    string com=readto(" ]",false);
    jumpto(']');
    if (com=="if" or com=="ifdef" or com=="ifndef" or com=="foreach" or com=="for" or
    com=="define") stack.add(com);
    if (com=="endif") _cec(buf,stack,"endif");
  }
  setstate(st);
}

char * CHARMIXER::loadfile(string file) {
  string f=lowercase(file);
  if (char *text=(char*)loadedfiles.findvar(f.c_str())) return text;
  string text=loadtext(f.c_str());
  loadedfiles._addvar(f,text);
  checkloadedfile(loadedfiles.last()->name,loadedfiles.last()->value);
  return (char*)loadedfiles.last()->value.c_str();
}

void CHARMIXER::runscript(const char *file,CMELEM *base,bool catcherrors) {
  VarList locs;
  locals=&locs;
  elemw=elemstack;*elemw++=base;arrayw=arraystack;whilew=whilestack;
  prgdir=outputdir=inputdir=sFile(file).getdir();
  MEMSTROUT mso;
  out=&mso;
  act=loadfile(file);
  end=strend(act);
  try {
    run();
  } catch(CMERROR &e) {
    savetextfile(prgdir/"error",e.msg.c_str());
    savetextfile(prgdir/"erroroutput",mso.str().c_str());
    if (not catcherrors) throw;
  } catch(SAERROR &e) {
    savetextfile(prgdir/"error",e.msg.c_str());
    savetextfile(prgdir/"erroroutput",mso.str().c_str());
    if (not catcherrors) throw;
  } catch(EditAborted) {
    return;
  }
}

void CHARMIXER::jumpto(char end) {
  while (*act!=end and data()) {
    if (*act=='\\' and act[1]) act+=2;else act++;
  }
  if (*act==end and data()) act++;
}

}
#pragma package(smart_init)
