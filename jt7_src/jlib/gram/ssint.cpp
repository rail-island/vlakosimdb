#include <jlib.h>
#pragma hdrstop

#include "ssint.h"
#include <com/ptrto.h>

namespace ss {

static string read_idf(Analyser &a) {
  bstring res;
  while (a.is_idf() || *a=='$') {res<<*a;a++;}
  return res;
}
inline void jump_com(Analyser &a) {a.jcw();a.f_char(']');}
inline void jump_com(Analyser &a,const char *endcom) {
  a.jcw();a.f_jump(endcom);
  a.jcw();a.f_char(']');
}

struct IPushStr:Instruction {
  IPushStr(string str,Module *module):str(str) {}
  FnResult *work(Interpreter *ir) {ir->push(str);return NULL;}
private:
  string str;
};
struct IOutStr:Instruction {
  IOutStr(string str,Module *module):str(str) {}
  FnResult *work(Interpreter *ir) {ir->put(str);return NULL;}
private:
  string str;
};
struct IWriteTop:Instruction {
  Module *module;
  bool langtext;

  IWriteTop(Module *module,bool langtext=false):module(module),langtext(langtext) {}
  FnResult *work(Interpreter *ir) {
    string s=module->getvalue(ir->pop());
    if (langtext) s=lang_text_ch(s);
    ir->put(s);
    return NULL;
  }
private:
  string str;
};
struct IAdd:Instruction {
  Module *module;
  IAdd(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(ftos(left.fvalue()+right.fvalue()));
    return NULL;
  }
};
struct ISub:Instruction {
  Module *module;
  ISub(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(ftos(left.fvalue()-right.fvalue()));
    return NULL;
  }
};
struct IMul:Instruction {
  Module *module;
  IMul(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(ftos(left.fvalue()*right.fvalue()));
    return NULL;
  }
};
struct IDiv:Instruction {
  Module *module;
  IDiv(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(ftos(left.fvalue()/right.fvalue()));
    return NULL;
  }
};
struct IMod:Instruction {
  Module *module;
  IMod(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(ftos(round(left.fvalue())%round(right.fvalue())));
    return NULL;
  }
};
struct IConc:Instruction {
  Module *module;
  IConc(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(left+right);
    return NULL;
  }
};
struct IReturn:Instruction {
  Module *module;
  IReturn(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    return new FnResult(module->getvalue(ir->pop()));
  }
};
struct IRetOut:Instruction {
  Module *module;
  IRetOut(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    return new FnResult(ir->getout());
  }
};
struct IAssign:Instruction {
  Module *module;
  IAssign(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),lvalue=ir->pop();
    module->doassign(lvalue,right);
    return NULL;
  }
};
struct ILT:Instruction {
  Module *module;
  ILT(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(itos(left.fvalue()<right.fvalue()));
    return NULL;
  }
};
struct IGT:Instruction {
  Module *module;
  IGT(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(itos(left.fvalue()>right.fvalue()));
    return NULL;
  }
};
struct ILET:Instruction {
  Module *module;
  ILET(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(itos(left.fvalue()<right.fvalue()));
    return NULL;
  }
};
struct IGET:Instruction {
  Module *module;
  IGET(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(itos(left.fvalue()>=right.fvalue()));
    return NULL;
  }
};
struct IEQ:Instruction {
  Module *module;
  IEQ(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(itos(left==right));
    return NULL;
  }
};
struct INEQ:Instruction {
  Module *module;
  INEQ(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(itos(left!=right));
    return NULL;
  }
};
struct IAnd:Instruction {
  Module *module;
  IAnd(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(itos(module->tobool(left) && module->tobool(right)));
    return NULL;
  }
};
struct IOr:Instruction {
  Module *module;
  IOr(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string right=module->getvalue(ir->pop()),left=module->getvalue(ir->pop());
    ir->push(itos(module->tobool(left) || module->tobool(right)));
    return NULL;
  }
};
struct INot:Instruction {
  Module *module;
  INot(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string left=module->getvalue(ir->pop());
    ir->push(itos(!module->tobool(left)));
    return NULL;
  }
};
struct IDefer:Instruction {
  Module *module;
  IDefer(string str,Module *module):str(str),module(module) {}
  FnResult *work(Interpreter *ir) {
    string res=string("#[fld]")+str+"."+module->getvalue(ir->pop());
    ir->push(res);
    return NULL;
  }
private:
  string str;
};
struct IGetIndex:Instruction {
  Module *module;
  IGetIndex(int parcnt,Module *module):parcnt(parcnt),module(module) {}
  FnResult *work(Interpreter *ir) {
    Strings pars;
    ir->poppars(parcnt,pars);
    for(int i=0;i<pars.count;i++) pars[i]=module->getvalue(pars[i]);
    string array=ir->pop();
    ir->push(module->callmethod(module->getvalue(array),"getindex",pars));
    return NULL;
  }
private:
  int parcnt;
};
struct ICall:Instruction {
  Module *module;
  ICall(int parcnt,Module *module):parcnt(parcnt),module(module) {}
  FnResult *work(Interpreter *ir) {
    Strings pars;
    ir->poppars(parcnt,pars);
    for(int i=0;i<pars.count;i++) pars[i]=module->getvalue(pars[i]);
    string fn=ir->pop();
    string res=module->callobj(fn,pars);
    ir->push(res);
    return NULL;
  }
private:
  int parcnt;
};
struct IPostInc:Instruction {
  Module *module;
  IPostInc(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string op=ir->pop();
    string oldval=module->getvalue(op);
    string newval=ftos(oldval.fvalue()+1);
    module->doassign(op,newval);
    ir->push(oldval);
    return NULL;

  }
};
struct IPostDec:Instruction {
  Module *module;
  IPostDec(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string op=ir->pop();
    string oldval=module->getvalue(op);
    string newval=ftos(oldval.fvalue()-1);
    module->doassign(op,newval);
    ir->push(oldval);
    return NULL;
  }
};
struct IPop:Instruction {
  Module *module;
  IPop(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    ir->pop();
    return NULL;
  }
};
struct IRewrite:Instruction {
  Module *module;
  IRewrite(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    ir->clearout();
    return NULL;
  }
};
struct ISave:Instruction {
  Module *module;
  ISave(Module *module):module(module) {}
  FnResult *work(Interpreter *ir) {
    string str=ir->getout();
    string file=module->getvalue(ir->pop());
    savetextfile(sFile(module->globals->findvalue("outputdir"))/file,str.c_str());
    return NULL;
  }
};

struct IIf:Instruction {
  Module *module;
  IIf(Module *module):module(module),code(module),cond(module) {}
  void parse(Analyser &a,ParseOpts &po) {
    cond.parsecond(a,po);
    code.parseblock(a,po);
    a.jcw();
    if (a.jmpidf("else")) {
      els=new Block(module);
      els->parseblock(a,po);
    } else if (a.jmpidf("elif")) {
      elif=new IIf(module);
      elif->parse(a,po);
    }
  }
  void parsecm(Analyser &a,Program *program,ParseOpts &po) {
    cond.parseexpr(a,po);
    jump_com(a);
    code.parsecm(a,"",program,po);
    if (a.begjmp("elif")) {
      elif=new IIf(module);
      elif->parsecm(a,program,po);
    } else if (a.begjmp("else")) {
      jump_com(a);
      els=new Block(module);
      els->parsecm(a,"endif",program,po);
      jump_com(a,"endif");
    } else if (a.begjmp("endif")) {
      jump_com(a);
    } else a.error("else,elif or endif expected");
  }
  FnResult *work(Interpreter *ir) {
    IIf *act=this;
    while (act) {
      FnResult *res=act->cond.work(ir);
      if (res) return res;
      string cond=module->getvalue(ir->pop());
      if (module->tobool(cond)) {
        res=act->code.work(ir);
        return res;
      } else {
        if (act->elif) act=act->elif;else {
          if (act->els) {
            res=act->els->work(ir);
            return res;
          } else return NULL;
        }
      }
    }
    return NULL;
  }
  Block code;
  Block cond;
  PtrTo<IIf> elif;
  PtrTo<Block> els;
};
struct IWhile:Instruction {
  Module *module;
  IWhile(Module *module):module(module),code(module),cond(module) {}
  void parse(Analyser &a,ParseOpts &po) {
    cond.parsecond(a,po);
    code.parseblock(a,po);
  }
  void parsecm(Analyser &a,Program *program,ParseOpts &po) {
    cond.parsecond(a,po);
    jump_com(a);
    code.parsecm(a,"endwhile",program,po);
    jump_com(a,"endwhile");
  }
  FnResult *work(Interpreter *ir) {
    for(;;) {
      FnResult *res=cond.work(ir);
      if (res) return res;
      string cond=module->getvalue(ir->pop());
      if (!module->tobool(cond)) return NULL;
      res=code.work(ir);
      if (res) return res;
    }
    return NULL;
  }
  Block code;
  Block cond;
};
struct IRepeat:Instruction {//repeat nebo do-while cyklus
  Module *module;
  IRepeat(Module *module,bool isrepeat=false):module(module),code(module),cond(module),isrepeat(isrepeat) {}
  void parse(Analyser &a,ParseOpts &po) {
    code.parseblock(a,po);
    a.jcw();a.f_jump("while");
    cond.parsecond(a,po);
  }
  void parsecm(Analyser &a,Program *program,ParseOpts &po) {
    jump_com(a);
    code.parsecm(a,"repeat",program,po);
    a.f_jump("until");
    cond.parseexpr(a,po);
    jump_com(a);
  }
  FnResult *work(Interpreter *ir) {
    for(;;) {
      FnResult *res=code.work(ir);
      if (res) return res;
      res=cond.work(ir);
      if (res) return res;
      string cond=module->getvalue(ir->pop());
      if (isrepeat) {
        if (module->tobool(cond)) return NULL;
      } else {
        if (!module->tobool(cond)) return NULL;
      }
    }
    return NULL;
  }
  Block code;
  Block cond;
  bool isrepeat;
};
struct IFor:Instruction {
  Module *module;
  IFor(Module *module,string var):var(var),module(module),begin(module),end(module),code(module) {}
  void parse(Analyser &a,ParseOpts &po) {
    a.jcw();
    a.f_char('=');
    begin.parseexpr(a,po);
    a.jcw();
    a.f_jump("to");
    end.parseexpr(a,po);
    code.parseblock(a,po);
  }
  void parsecm(Analyser &a,Program *program,ParseOpts &po) {
    a.jcw();
    a.f_char('=');
    begin.parseexpr(a,po);
    a.jcw();
    a.f_jump("to");
    end.parseexpr(a,po);
    jump_com(a);
    code.parsecm(a,"endfor",program,po);
    jump_com(a,"endfor");
  }
  FnResult *work(Interpreter *ir) {
    FnResult *res=begin.work(ir);
    if (res) return res;
    double start=module->getvalue(ir->pop()).fvalue(),act=start;

    res=end.work(ir);
    if (res) return res;
    double stop=module->getvalue(ir->pop()).fvalue();

    while (act<=stop) {
      module->assignvar(var,ftos(act));
      res=code.work(ir);
      if (res) return res;
      act=act+1;
    }
    return NULL;
  }
  string var;
  Block begin,end,code;
};
struct IForEach:Instruction {
  Module *module;
  IForEach(Module *module,string var):var(var),module(module),array(module),code(module) {}
  void parse(Analyser &a,ParseOpts &po) {
    a.jcw();
    a.f_jump("in");
    array.parseexpr(a,po);
    code.parseblock(a,po);
  }
  void parsecm(Analyser &a,Program *program,ParseOpts &po) {
    a.jcw();
    a.f_jump("in");
    array.parseexpr(a,po);
    jump_com(a);
    code.parsecm(a,"endfor",program,po);
    jump_com(a,"endfor");
  }
  FnResult *work(Interpreter *ir) {
    FnResult *res=array.work(ir);
    if (res) return res;
    string ar=module->getvalue(ir->pop());

    Strings items;
    module->foreach(ar,items);
    for(int i=0;i<items.count;i++) {
      module->assignvar(var,items[i]);
      res=code.work(ir);
      if (res) return res;
    }
    return NULL;
  }
  string var;
  Block array,code;
};
struct IInclude:Instruction {
  Module *module;
  IInclude(Module *module):module(module),code(module) {}
  void parseincluded(string file,Program *program,ParseOpts &po) {
    string text;
    if (program->caninclude(file)) {
      string text=program->loadinclude(file);
      Analyser a(text.c_str());
      string last=program->setactfile(file);
      program->wasincluded(file);
      try {
        code.parsecm(a,NULL,program,po);
      } catch (AnalError &e) {
        program->throweanal(text,e);
      }
      program->restoreactfile(last);
    }
  }
  FnResult *work(Interpreter *ir) {
    return code.work(ir);
  }
  Block code;
};
/*
struct IFnDef:Instruction {
  Module *module;
  IFnDef(Module *module):module(module) {}
  void parse(Analyser &a) {
    fn=new Function(module);
    fn->parse(a);
    module->addsub(fn);
  }
  FnResult *work(Interpreter *ir) {
    module->addsub(fn);
    return NULL;
  }
private:
  RefBase<Function> fn;
};
*/

string Program::filename(string incname) {
  sFile path=actfile;
  path=path.getdir()/incname;
  return path;
}

bool Program::caninclude(string file) {
  if (included.search(filename(file).getupper())>=0) return false;
  return true;
}

string Program::loadinclude(string file) {
  file=filename(file);
  return loadtext(file.c_str());
}

void Program::throweanal(string text,AnalError &e) {
  CommonParseError pe;
  pe.init(text,e.getmsg(),e.pos-text.c_str(),actfile);
  throw pe;
}

void Program::compilecm(string file) {
  included.add(filename(file).getupper());
  actfile=file;
  string text=loadtext(file.c_str());
  Analyser a(text.c_str());
  try {
    ParseOpts po;
    main.parsecm(a,NULL,this,po);
  } catch (AnalError &e) {
    throweanal(text,e);
  }
  main.code.add(new IRetOut(module));
}

void Program::compile(string file) {
  string text=loadtext(file.c_str());
  Analyser a(text.c_str());
  ParseOpts po;
  main.parsecoms(a,po);
}

void Block::parsecmcom(Analyser &a,Program *program,ParseOpts &po) {
  a.f_char('#');a.f_char('[');
  if (a.begjmp("if")) {
    IIf *iif=new IIf(module);
    code.add(iif);
    iif->parsecm(a,program,po);
  } else if (a.begjmp("repeat")) {
    jump_com(a);
    IRepeat *irep=new IRepeat(module,true);
    code.add(irep);
    irep->parsecm(a,program,po);
  } else if (a.begjmp("define")) {
    RefBase<Function> fn=new Function(module);
    fn->parsecm(a,program,po);
    module->addsub(fn);
  } else if (a.begjmp("for")) {
    a.jcw();
    string var=read_idf(a);
    a.jcw();
    if (*a=='=') {
      IFor *ifor=new IFor(module,var);
      code.add(ifor);
      ifor->parsecm(a,program,po);
    } else {
      IForEach *ifor=new IForEach(module,var);
      code.add(ifor);
      ifor->parsecm(a,program,po);
    }
  } else if (a.begjmp("while")) {
    IWhile *wh=new IWhile(module);
    code.add(wh);
    wh->parsecm(a,program,po);
  }

}

void Block::parsecm(Analyser &a,const char *endcom,Program *program,ParseOpts &po) {
  #define flush \
    {string s=str.str();if (s!="") code.add(new IOutStr(s,module));str.clear();}

  MEMSTROUT str;
  while (*a) {
    if (*a==po.jumper && Analyser::is_white(a[1])) {
      a++;a.jmpwhite();
    } else if (*a==po.escape) {
      a++;if (*a) {
        if (*a=='n') str<<'\n';
        else if (*a=='s') str<<' ';
        else if (*a=='t') str<<'\t';
        else str<<*a;
        a++;
      }
    } else if (a.begins("#[endfor") || a.begins("#[endwhile") || a.begins("#[endif") ||
      a.begins("#[until") || a.begins("#[else") || a.begins("#[elif") || a.begins("#[enddef")) {
      if (endcom) {
        a+=2;
        if (*endcom && ! a.begins(endcom)) a.error(fmtstr("%s expected",endcom));
        flush
        return;
      } else {
        a.error("Unexpected end of structure");
      }
    } else if (*a=='#' && a[1]=='[') {
      flush
      parsecmcom(a,program,po);
    } else if (*a=='%' && a[1]=='[') {
      flush
      parsessincm_coms(a,po);
    } else if (*a=='&' && a[1]=='[') {
      flush
      a+=2;
      if (*a==']') {
        a++;
      } else if (a.begjmp("escape")) {
        a.jsp();
        if (*a==']') {
          po.escape='\0';
          a++;
        } else {
          po.escape=*a;
          a++;
          a.jsp();
          a.f_char(']');
        }
      } else if (a.begjmp("jumper")) {
        a.jsp();
        if (*a==']') {
          po.jumper='\0';
          a++;
        } else {
          po.jumper=*a;
          a++;
          a.jsp();
          a.f_char(']');
        }
      } else if (a.begjmp("rewrite]")) {
        code.add(new IRewrite(module));
      } else if (a.begjmp("include")) {
        a.jsp();
        a.f_char('"');
        string file=a.readcstring('"');
        jump_com(a);
        IInclude *inc=new IInclude(module);
        code.add(inc);
        inc->parseincluded(file,program,po);
      } else if (a.begjmp("save")) {
        parseexpr(a,po);
        jump_com(a);
        code.add(new ISave(module));
      } else if (a.begjmp("return]")) {
        code.add(new IRetOut(module));
      }
    } else if (*a=='@' && a[1]=='[') {
      flush
      a+=2;
      code.add(new IPushStr(string("#[ref]")+a.readidf(),module));
      int parcnt=0;
      while (a.jsp(),*a!=']') {
        parseexpr(a,po);
        parcnt++;
        a.jsp();
        if (*a==',') a++;
      }
      code.add(new ICall(parcnt,module));
      code.add(new IWriteTop(module));
      jump_com(a);
    } else if (*a=='$' && a[1]=='[') {
      flush
      a+=2;
      parseexpr(a,po);
      jump_com(a);
      code.add(new IWriteTop(module));
    } else if (*a=='$' && a[1]=='$' && a[2]=='[') {
      flush
      a+=3;
      parseexpr(a,po);
      jump_com(a);
      code.add(new IWriteTop(module,true));
    } else if (*a=='$' && a[1]=='$') {
      bstring name;
      a+=2;
      while (a.is_idf() || *a=='-') name<<*a,a++;
      str<<lang_text(name);
    } else {
      str<<*a;
      a++;
    }
  }
  flush
  #undef flush
}

string read_ss_string(Analyser &a,char end) {
  bstring res;
  while (*a && *a!=end) {
    if (*a=='\\') {
      a++;
      if (*a=='n') res<<'\n';
      else res<<*a;
      if (*a) a++;
    } else if (*a=='$' && a[1]=='$') {
      bstring name;
      a+=2;
      while (a.is_idf() || *a=='-') name<<*a,a++;
      res<<lang_text(name);
    } else res<<*a,a++;
  }
  if (*a) a++;
  return res;
}

void Block::parsehi(Analyser &a,ParseOpts &po) {
  a.jcw();
  if (*a=='(') {
    a++;
    parseexpr(a,po);
    a.jcw();
    a.f_char(')');
  } else if (a.is_idfbeg() || *a=='$') {
    code.add(new IPushStr(string("#[ref]")+read_idf(a),module));
  } else if (a.is_digit() || *a=='-') {
    char num[0x100];
    a.readdouble(num);
    code.add(new IPushStr(num,module));
  } else if (*a=='"') {
    a++;
    code.add(new IPushStr(read_ss_string(a,'"'),module));
/*    char str[0x1000];
    a.readcstring(str);
    code.add(new IPushStr(str,module));*/
  } else if (*a=='\'') {
    a++;
    code.add(new IPushStr(read_ss_string(a,'\''),module));
/*    char str[0x1000];
    a.readcstring(str,'\'');
    code.add(new IPushStr(str,module));*/
  } else {
    a.error("String, identifier, number or expression in parentheses expected");
  }
}

void Block::parsedefer(Analyser &a,ParseOpts &po) {
  parsehi(a,po);
  a.jcw();
  for(;;) {
    if (*a=='.') {
      a++;a.jcw();
      string right=read_idf(a);
      code.add(new IDefer(right,module));
    } else if (*a=='[') {
      a++;a.jcw();
      int parcnt=0;
      while (a.jcw(),*a!=']') {
        parseexpr(a,po);
        parcnt++;
        a.jcw();
        if (*a==',') a++,a.jcw();
      }
      a.jcw();a.f_char(']');a.jcw();
      code.add(new IGetIndex(parcnt,module));
    } else if (*a=='(') {
      a++;a.jcw();
      int parcnt=0;
      while (a.jcw(),*a!=')') {
        parseexpr(a,po);
        parcnt++;
        a.jcw();
        if (*a==',') a++,a.jcw();
      }
      a.jcw();a.f_char(')');a.jcw();
      code.add(new ICall(parcnt,module));
    } else break;
  }
  a.jcw();
  for(;;) {
    if (a.begjmp("++")) {
      code.add(new IPostInc(module));
    } else if (a.begjmp("--")) {
      code.add(new IPostDec(module));
    } else break;
    a.jcw();
  }
}


void Block::parsemul(Analyser &a,ParseOpts &po) {
  parsedefer(a,po);
  for(;;) {
    a.jcw();
    if (*a=='*') {
      a++;a.jcw();
      parsedefer(a,po);
      code.add(new IMul(module));
    } else if (*a=='/') {
      a++;a.jcw();
      parsedefer(a,po);
      code.add(new IDiv(module));
    } else if (*a=='%') {
      a++;a.jcw();
      parsedefer(a,po);
      code.add(new IMod(module));
    } else break;
  }
}

void Block::parseadd(Analyser &a,ParseOpts &po) {
  parsemul(a,po);
  for(;;) {
    a.jcw();
    if (*a=='+') {
      a++;a.jcw();
      parsemul(a,po);
      code.add(new IAdd(module));
    } else if (*a=='-') {
      a++;a.jcw();
      parsemul(a,po);
      code.add(new ISub(module));
    } else if (*a=='&' && a[1]!='&') {
      a++;a.jcw();
      parsemul(a,po);
      code.add(new IConc(module));
    } else break;
  }
}

void Block::parserel(Analyser &a,ParseOpts &po) {
  parseadd(a,po);
  for(;;) {
    a.jcw();
    if (a.begjmp("<=")) {
      a.jcw();
      parseadd(a,po);
      code.add(new ILET(module));
    } else if (a.begjmp(">=")) {
      a.jcw();
      parseadd(a,po);
      code.add(new IGET(module));
    } else if (a.begjmp("<")) {
      a.jcw();
      parseadd(a,po);
      code.add(new ILT(module));
    } else if (a.begjmp(">")) {
      a.jcw();
      parseadd(a,po);
      code.add(new IGT(module));
    } else if (a.begjmp("==")) {
      a.jcw();
      parseadd(a,po);
      code.add(new IEQ(module));
    } else if (a.begjmp("!=")) {
      a.jcw();
      parseadd(a,po);
      code.add(new INEQ(module));
    } else break;
  }
}

void Block::parsenot(Analyser &a,ParseOpts &po) {
  a.jcw();
  bool neg=false;
  if (a.begjmp("!") || a.jmpidf("not")) neg=true;
  parserel(a,po);
  if (neg) code.add(new INot(module));
}
void Block::parseand(Analyser &a,ParseOpts &po) {
  parsenot(a,po);
  for(;;) {
    a.jcw();
    if (a.jmpidf("and") || a.begjmp("&&")) {
      parsenot(a,po);
      code.add(new IAnd(module));
    } else break;
  }
}
void Block::parseor(Analyser &a,ParseOpts &po) {
  parseand(a,po);
  for(;;) {
    a.jcw();
    if (a.jmpidf("or") || a.begjmp("||")) {
      parseand(a,po);
      code.add(new IOr(module));
    } else break;
  }
}


void Block::parseblock(Analyser &a,ParseOpts &po) {
  a.jcw();a.f_char('{');a.jcw();
  parsecoms(a,po);
  a.jcw();
  a.f_char('}');
  a.jcw();
}

void Block::parsecoms(Analyser &a,ParseOpts &po) {
  while (a.jcw(),*a && *a!='}') parsecom(a,po);
}

void Block::parsessincm_coms(Analyser &a,ParseOpts &po) {
  a.f_char('%');a.f_char('[');
  while (a.jcw(),*a && *a!=']') parsecom(a,po);
  jump_com(a);
}

void Block::parsecond(Analyser &a,ParseOpts &po) {
  a.jcw();
  a.f_char('(');
  parseexpr(a,po);
  a.jcw();
  a.f_char(')');
}

void Block::parsecom(Analyser &a,ParseOpts &po) {
  a.jcw();
  if (a.jmpidf("if")) {
    IIf *iif=new IIf(module);
    code.add(iif);
    iif->parse(a,po);
  } else if (a.jmpidf("sub")) {
    RefBase<Function> fn=new Function(module);
    fn->parse(a,po);
    module->addsub(fn);
//    IFnDef *ifn=new IFnDef(module);
//    code.add(ifn);
//    ifn->parse(a);
  } else if (a.jmpidf("do")) {
    IRepeat *irep=new IRepeat(module);
    code.add(irep);
    irep->parse(a,po);
  } else if (a.jmpidf("while")) {
    IWhile *wh=new IWhile(module);
    code.add(wh);
    wh->parse(a,po);
  } else if (a.jmpidf("for")) {
    a.jcw();
    string var=read_idf(a);
    a.jcw();
    if (*a=='=') {
      IFor *ifor=new IFor(module,var);
      code.add(ifor);
      ifor->parse(a,po);
    } else {
      IForEach *ifor=new IForEach(module,var);
      code.add(ifor);
      ifor->parse(a,po);
    }
  } else if (a.jmpidf("return")) {
    a.jcw();
    if (*a==';' || *a=='}') {code.add(new IPushStr("",module));}
    parseexpr(a,po);
    code.add(new IReturn(module));
  } else if (*a=='{') {
    parseblock(a,po);
  } else {
    parseexpr(a,po);
    a.jcw();
    if (*a=='=') {
      a++;a.jcw();
      parseexpr(a,po);
      code.add(new IAssign(module));
    } else code.add(new IPop(module));
  }
  a.jcw();
  if (*a==';') a++,a.jcw();

}

void Function::parsecm(Analyser &a,Program *program,ParseOpts &po) {
  a.jsp();
  a.readidf(name,sizeof(name));
  a.jsp();
  while (a.jsp(),*a!=']') {
    parnames.add(a.readidf());
    a.jsp();if (*a==',') a++;
  }
  a.f_char(']');
  block.parsecm(a,"enddef",program,po);
  jump_com(a,"enddef");
  block.code.add(new IRetOut(module));
}

void Function::parse(Analyser &a,ParseOpts &po) {
  a.jcw();
  a.readidf(name,sizeof(name));
  a.jcw();
  a.f_char('(');
  while (a.jcw(),*a!=')') {
    parnames.add(a.readidf());
    a.jcw();if (*a==',') a++;
  }
  a.f_char(')');
  block.parseblock(a,po);
}

string Function::call(Strings &pars) {
  Interpreter ir;
  DEF_LOCALS
  int cnt=min(pars.count,parnames.count);
  for(int i=0;i<cnt;i++) module->locals->setvar(parnames[i],pars[i]);
  FnResult *res=block.work(&ir);
  if (res) return res->extractres();
  return locs.findvalue("result");
}

FnResult *Block::work(Interpreter *ir) {
  FnResult *res=NULL;
  for(int i=0;i<code.count;i++) {
    res=code[i]->work(ir);
    if (res) return res;
  }
  return NULL;
}

Program::Program(Module *module):module(module),main(module) {}

string Program::run() {
  Interpreter ir;
  FnResult *res=main.work(&ir);
  if (res) return res->extractres();
  return string("");
}


}

