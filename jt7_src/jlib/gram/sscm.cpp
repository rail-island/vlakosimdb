#include <jlib.h>
#pragma hdrstop

#include "sscm.h"
#include "ssmain.h"

namespace ss {

struct MacroReturn {};

void CharMixer::jumpcommand() {
  int level=0;
  if (*a=='#' && a[1]=='[') a+=2;
  while (*a) {
    if (*a=='[') {
      level++;
      a++;
    } else if (*a==']') {
      a++;
      if (level==0) return;
      level--;
    } else if (*a=='"') {
      a++;
      a.jumpcstring();
    } else if (*a=='\'') {
      a++;
      a.jumpcstring('\'');
    } else a++;
    a.jsp();
  }
}

void CharMixer::runrepeat() {

}
void CharMixer::runif() {

}

/*
void CharMixer::runto(const char *name,bool jmplastcom) {
  int level=0;
  while (*a) {
    if (iscommand()) {
      a+=2;
      if (a.begjmp("if") || a.begjmp("for") || a.begjmp("repeat")) {level++;}
      else if (a.begjmp(name)) {
        if (level==0) {if (jmplastcom) jumpcommand();return;}
        level--;
      } else if (a.begjmp("until") || a.begjmp("endfor") || a.begjmp("endif")) level--;
      if (level<0) a.error();
      jumpcommand();
    }
  }
}
*/

void CharMixer::jumpto(const char *name,bool jmplastcom,bool stoponifs) {
  int level=0;
  while (*a) {
    if (iscommand()) {
      a+=2;
      if (a.begjmp("if") || a.begjmp("for") || a.begjmp("repeat") || a.begjmp("while") || a.begjmp("define")) {level++;}
      else if (stoponifs && level==0 && (a.begins("else") || a.begins("elif"))) {
        if (jmplastcom) jumpcommand();return;
      } else if (*name && a.begjmp(name)) {
        if (level==0) {if (jmplastcom) jumpcommand();return;}
        level--;
      } else if (a.begjmp("until") || a.begjmp("endfor") || a.begins("endif") || a.begjmp("endwhile") || a.begjmp("enddef")) level--;
      if (level<0) {
        if (*name) a.error("Unexpected end of structure");
        if (jmplastcom) jumpcommand();
        return;
      }
      jumpcommand();
    } else a++;
  }
}

string CharMixer::evalexpr() {
  Runtime rt(module);
  rt.a=a;
  return rt.evalexpr();
}

bool CharMixer::evalcond() {
  Runtime rt(module);
  rt.a=a;
  bool res=rt.evalcond(false);
  jumpcommand();
  return res;
}

string MacDef::call(Strings &pars) {
  MEMSTROUT mso;
  DEF_LOCALS
  string lastfile=cm->actfile;
  for(int i=0;i<min(pars.count,args.count);i++) locs.setvar(args[i],pars[i]);
  cm->actfile=file;
  MEMSTROUT *last=cm->out;
  cm->out=&mso;
  try {
    cm->saferun(body,"enddef");
  } catch (MacroReturn) {
    //ok
  } catch(...) {
    cm->out=last;
    cm->actfile=lastfile;
    throw;
  }
  cm->out=last;
  cm->actfile=lastfile;
  return mso.str();
}

void MacDef::load(Analyser &a) {
  a.jsp();
  a.f_readidf(name);
  while (a.jsp(),*a!=']') {
    char par[0x100];a.f_readidf(par);
    args.add(par);
    a.jsp();
    if (*a==',') a++,a.jsp();
  }
  a.f_char(']');
  body=a;
}

void CharMixer::runcommand() {
  a+=2;
  if (a.begjmp("if")) {
    bool act=evalcond(),was=act;
    doifblock(act);
    for(;;) {
      if (a.begjmp("elif")) {
        act=evalcond();
        doifblock(act && ! was);
      } else if (a.begjmp("else")) {
        jumpcommand();
        doifblock( ! was);
      } else if (a.begjmp("endif")) {
        jumpcommand();
        break;
      } else a.error("else,elif or endif expected");
      was = was || act;
    }
  } else if (a.begjmp("repeat")) {
    a.jsp();a.f_char(']');
    Analyser begin=a;
//    Runtime rt(module);
    bool res=true;
    do {
      a=begin;
      runcoms("until");
      a.f_jump("until");
//      rt.a=a;
      res=evalcond();
//      res=rt.evalcond(false);
    } while (! res);
//    jumpcommand();
  } else if (a.begjmp("define")) {
    RefBase<MacDef> mac=new MacDef(this);
    mac->load(a);
    module->addsub(mac);
//    if (module->subs.searchtype(mac->name)<0) module->subs.add(mac);else delete mac;
    jumpto("enddef",true,false);
  } else if (a.begjmp("for")) {
    a.jcw();
    Runtime rt(module);rt.a=a;
    string idf=rt.readidf();
    rt.a.jcw();
    if (rt.a.begjmp("in")) {
      string expr=module->getvalue(rt.evalexpr());
      Strings items;
      module->foreach(expr,items);
      jumpcommand();
      Analyser beg=a;
      for(int i=0;i<items.count;i++) {
        module->locals->setvar(idf,items[i]);
        runcoms("endfor");
        a=beg;
      }
      jumpto("endfor",true,false);
    } else rt.a.error("in expected");
  } else if (a.begjmp("while")) {
    Analyser begin=a;
    for(;;) {
      a=begin;
//      Runtime rt(module);
//      rt.a=a;
//      bool res=rt.evalcond(false);
//      jumpcommand();
      bool res=evalcond();
      if (!res) {
        jumpto("endwhile",true,false);
        return;
      }
      runcoms("endwhile");
    }
//    jumpcommand();
  }
}

void CharMixer::runcoms(const char *endcom) {
  while (*a) {
    if (*a=='_' && Analyser::is_white(a[1])) {
      a++;a.jmpwhite();
    } else if (*a=='\\') {
      a++;if (*a) {
        if (*a=='n') *out<<'\n';
        else if (*a=='s') *out<<' ';
        else if (*a=='t') *out<<'\t';
        else *out<<*a;
        a++;
      }
    } else if (a.begins("#[endfor") || a.begins("#[endwhile") || a.begins("#[endif") ||
      a.begins("#[until") || a.begins("#[else") || a.begins("#[elif") || a.begins("#[enddef")) {
      if (endcom) {
        a+=2;
        if (*endcom && ! a.begins(endcom)) a.error(fmtstr("%s expected",endcom));
        return;
      } else {
        a.error("Unexpected end of structure");
      }
    } else if (iscommand()) {
      runcommand();
    } else if (*a=='%' && a[1]=='[') {
      a+=2;Runtime rt(module);rt.a=a;
      rt.run(']');
      jumpcommand();
    } else if (*a=='&' && a[1]=='[') {
      a+=2;
      if (*a==']') {
        a++;
      } else if (a.begjmp("rewrite]")) {
        out->clear();
      } else if (a.begjmp("mode")) {
        a++;
        if (a.begjmp("solid")) writewhites=false;
        else if (a.begjmp("copy")) writewhites=true;
        jumpcommand();
      } else if (a.begjmp("include")) {
        string file=evalexpr();
        runinclude(file);
        jumpcommand();
      } else if (a.begjmp("save")) {
        string file=evalexpr();
        string text_out=out->str();
        savetextfile(sFile(module->globals->findvalue("outputdir"))/file,text_out.c_str());
        jumpcommand();
      } else if (a.begjmp("return]")) {
        throw MacroReturn();
      }
    } else if (*a=='@' && a[1]=='[') {
      a+=2;
      char name[0x100];a.f_readidf(name);
      Runtime rt(module);rt.a=a;
      Strings pars;
      while (rt.a.jcw(),*rt.a!=']' && *rt.a) {
        string par=rt.evalexpr();
        rt.a.jcw();
        if (*rt.a==',') rt.a++,rt.a.jcw();
        pars.add(module->getvalue(par));
      }
      *out<<module->callsub(name,pars);
//      if (SubDef *sub=module->subs.searchvalue(name)) *out<<sub->call(pars);
//      else *out<<"Sub not found:"<<name<<"\n";
      jumpcommand();
    } else if (*a=='$' && a[1]=='[') {
      a+=2;Runtime rt(module);rt.a=a;
      *out<<module->getvalue(rt.evalexpr());
      jumpcommand();
    } else {
      if (writewhites || ! a.is_white()) *out<<*a;
      a++;
    }
  }
}

string CharMixer::runstr(const char *s) {
  a=s;
//  sw=stack;
  MEMSTROUT mso;
  out=&mso;
  runcoms();
  return mso.str(); 
}

void CharMixer::writeerrtext(const char *prgtext,AnalError &e) {
  int line,col;string errline;
  fmterror(prgtext,e.pos,line,col,errline,false);
  *out<<"Error:"<<actfile<<":"<<line<<"\n"<<e.msg<<"\n"<<errline;
  fmterror(prgtext,e.pos,line,col,errline,true);
  stderrout<<"Error:"<<actfile<<":"<<line<<"\n"<<e.msg<<"\n"<<errline<<
  "\n====================================================================\n";
  iserror=true;
}

void CharMixer::runfile(const char *file) {
  actfile=file;
  MEMSTROUT mso;
  out=&mso;
  inputdir=sFile(file).getdir();
  string str=loadtext(file);
  try {
    saferun(str.c_str(),NULL);
  } catch (AnalError &e) {
    CommonParseError pe;
    pe.init(str,e.msg,e.pos-str.c_str(),file);
    throw pe;
  }
  actfile="";
}


void CharMixer::saferun(const char *s,const char *end) {
  const char *lastpos=a;
  a=s;
  try {
    runcoms(end);
  } catch (...) {
//    writeerrtext(s,e);
    a=lastpos;
    throw;
  }
  a=lastpos;
}


const char *CharMixer::loadfile(string file) {
  string f=lowercase(file);
  if (const char *text=loadedfiles.findvar(f)) return text;
  string text=loadtext(f.c_str());
  loadedfiles._addvar(f,text);
//  checkloadedfile(loadedfiles.last()->name,loadedfiles.last()->value);
  return loadedfiles.last()->value.c_str();
}

void CharMixer::runinclude(string file) {
//  const char *lastpos=a;
  string last=actfile;
  actfile=inputdir/file;
  saferun(loadfile(actfile),NULL);
  actfile=last;
//  a=loadfile(inputdir/params[0]);
//  actfile=inputdir/params[0];
//  runcoms();
}



}


