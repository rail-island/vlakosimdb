#include <jlib.h>
#pragma hdrstop

#include "gmlang.h"
#include "gmexpr.h"
#include <com/jhash.cpp>

namespace gram {

template class IHashTable<ParsedTerm>;

TreeNode *Parser::allocnode() {
  TreeNode *res=(TreeNode*)allocate(sizeof(TreeNode));
  res->init();
  return res;
}
char *Parser::allocchar(const char *str) {
  int len=strlen(str);
  char *res=(char*)allocate(len+1);
  memcpy(res,str,len+1);
  return res;
}


ParserPos Parser::getpos() {
  ParserPos res;res.readpos=act;
  res.state=store();
//  res.allocpos=allocator.mark();
  return res;
}
void Parser::setpos(ParserPos pos) {
  act=pos.readpos;
  restore(pos.state);
//  allocator.release(pos.allocpos);
}

void Output::writestrout(TreeNode *node,STROUT &fw,TreeWriter &tw,STRING param) {
  fw<<"Node has not string output:"<<node->factor->term->name;
}
void Output::writexmlout(TreeNode *node,XMLNode xml,TreeWriter &tw) {
  xml.settext(string("Node has not XML output:")+node->factor->term->name);
}

inline int readcchar(const char *&c) {
  if (*c=='\\') {
    c++;
    int res=*c;
    if (*c) c++;
    return res;
  }
  return *c++;
}

/*
void Remark::load(const char *c) {
  Analyser a(c);
  nested=false;
  a.f_char('"');a.readcstring(beg);
  a.jsp();
  if (*a=='*') nested=true,a++,a.jsp();
  a.f_char('"');a.readcstring(end);
}
*/

void Remark::jump(Analyser &a) {
  a+=strlen(beg);
  a.jumpafter(end);
}

/*
bool Parser::jumpremark(Analyser &a) {
  if (actwhites) {
    return actwhites->jump(a);
  }
  return false;
/*  for(int i=0;i<remarks.count;i++) if (a.begins(remarks[i].beg)) {
    remarks[i].jump(a);
    return true;
  }
  return false;
}*/

void Whites::jmpwhite(Analyser &a) {
  retry:
  jmpchars(a);
  for(int i=0;i<rems.count;i++) if (a.begins(rems[i].beg)) {
    rems[i].jump(a);
    goto retry;
  }
}

void Parser::jmpwhite() {
  if (actwhites) actwhites->jmpwhite(act);
//  jmpwhitechars();
//  while (lang->jumpremark(act)) jmpwhitechars();
}

void Language::loadfiles() {
  for(int i=0;i<included.count;i++) {
    loading=included[i];
    string text=loadtext(included[i]);
    try {
      _load(text.c_str());
    } catch (AnalError &e) {
      CommonParseError pe;
      pe.init(text,e.msg,e.pos-text.c_str(),loading);
      throw pe;
    }
  }
  link();
}

/*
void Language::loadchars(Analyser &a) {
  while (*a && ! a.begins("[/chars]")) {
    char line[0x1000];
    a.readline(line);
    Analyser b(line);
    char idf[0x20];
    b.readidf(idf);
    if (*b!='=') {a.jumpchar('\n');continue;}
    b++;
    if (eqstr(idf,"idfs")) idfs.load(b);
    else if (eqstr(idf,"whites")) whites.load(b);
    else if (eqstr(idf,"idfbegs")) idfbegs.load(b);
    else if (eqstr(idf,"endw")) endwchars.load(b);
    else if (eqstr(idf,"remark")) {
      Remark &rem=remarks.add();
      rem.load(b);
    } else if (eqstr(idf,"keywords")) {
      while (b.jsp(),*b) {
        keywords.add(b.readto(" ",true));
      }
    }
  }
  a.jumpchar('\n');
}
*/

static void loadfactoritem(Analyser &a,FactorItem *fi) {
  a.jcw();
  *fi->varname=0;
  fi->vorbidden=false;
  if (*a==';') fi->skipwhites=false,a++,a.jcw();
  if (*a=='!') fi->vorbidden=true,a++,a.jcw();
  if (*a=='"' || *a=='\'') {
    char beg=*a;
    fi->fit=(beg=='"')?fit_word:fit_symbol;
    a++;
    a.readcstring(fi->name,beg);
  } else if (*a=='$') {
    a++;a.jcw();
    fi->fit=fit_eof;
  } else if (*a=='&') {//setwhites
    a++;a.jcw();
    fi->fit=fit_setwhites;
    a.readidf(fi->name);
    a.jcw();
  } else if (*a=='+') {//setendw
    a++;a.jcw();
    fi->fit=fit_setendw;
    a.readidf(fi->name);
    a.jcw();
  } else if (a.is_idfbeg()) {
    fi->fit=fit_term;
    a.readidf(fi->name);
    a.jcw();
    if (*a=='(') {
      a++;
      a.readline(fi->varname,')');
      xcheckstrlen(fi->varname);
      a.jcw();
    }
    if (*a==':') {
      a++;a.jcw();
      a.readidf(fi->typestr);
    } else strcpy(fi->typestr,fi->varname);
  } else a.error("factor item expected(identifier,+,&)");
  a.jcw();
}

void XMLCompoud::work(TreeNode *node,XMLNode xml,TreeWriter &tw) {
  XMLNode x=xml;
  if (simplesub) x=xml.add();
  if (*name) x.setname(name);
  for(int i=0;i<outs.count;i++) outs[i]->work(node,x,tw);
}

void XMLStrOutItem::work(TreeNode *node,XMLNode xml,TreeWriter &tw) {
  MEMSTROUT mso;
  out.writestrout(node,mso,tw,"");
  string val=mso.str().c_str();
  if (*name) xml.setvar(name,val);else xml.settext(val);
}

void XMLSimpleItem::work(TreeNode *node,XMLNode xml,TreeWriter &tw) {
  if (*name && *text) {
    xml.setvar(name,text);
  }
  if (! *name && *text) {
    xml.settext(text);
  }
  if (*name && ! *text) {
    xml.setname(name);
  }
}

void SubXMLOutItem::work(TreeNode *node,XMLNode xml,TreeWriter &tw) {
  TreeNode *child=node->searchchild(name);
  if (child) child->writeout(addsub?xml.add():xml,tw);
}

void XMLCompoud::load(Analyser &a) {
  a.jcw();
  while (a.isbody('|') && a.isbody('.') && a.isbody('>')) {
    if (*a=='<') {
      a++;
      XMLCompoud *child=new XMLCompoud();
      child->simplesub=true;
      if (a.is_idfbeg()) a.readidf(child->name);
      outs.add(child);
      child->load(a);
      a.jcw();
      a.f_char('>');
    } else if (*a=='=') {
      a++;
      a.jcw();
      char name[0x20];*name=0;
      if (a.is_idfbeg()) a.readidf(name),a.jcw();
      if (*a=='{') {//strout subitem
        a++;
        a.jcw();
        XMLStrOutItem *it=new XMLStrOutItem();
        outs.add(it);
        it->load(a);
        a.jcw();
        a.f_char('}');
        strcpy(it->name,name);
      } else {
        XMLSimpleItem *ti=new XMLSimpleItem();
        outs.add(ti);
        strcpy(ti->name,name);
        if (*a=='"') {
          a++;
          a.readcstring(ti->text);
        }
      }
    } else if (a.is_idfbeg() || *a=='+') {
      SubXMLOutItem *it=new SubXMLOutItem();
      outs.add(it);
      if (*a=='+') it->addsub=true,a++,a.jcw();
      a.f_readidf(it->name);
    } else {
      a.error("XML output item expected(=,+,identifier,<)");
    }
    a.jcw();
  }
}

void StrOutput::load(Analyser &a) {
  a.jcw();
  while (a.isbody('|') && a.isbody('.') && a.isbody('}')) {
    StrOutItem *out=addout();
    *out->param=0;
    if (*a=='"') {
      a++;
      a.readcstring(out->name,'"');
      out->type=ot_direct;
    } else if (a.is_idfbeg()) {
      a.f_readidf(out->name);
      out->type=ot_subnode;
      if (*a=='(') {
        a++;a.readcstring(out->param,')');
      }
    } else if (*a=='#') {
      a++;
      a.f_readidf(out->name);
      out->type=ot_string;
    } else if (*a=='$') {
      a++;
      a.f_readidf(out->name);
      out->type=ot_value;
    } else if (*a=='[') {
      a++;a.readcstring(out->name,']');
      out->type=ot_query;
    } else if (*a=='&') {
      a++;a.readcstring(out->name,':');
      out->type=eqstr(out->name,"break")?ot_break:ot_label;
    } else if (*a=='*') {
      a++;out->type=ot_default;
    } else if (*a=='!') {
      a++;
      *out->name=0;out->type=ot_break;
    } else
      a.error("string item expected");
    a.jcw();
  }
  a.jcw();
}

static void loadfactor(Analyser &a,Factor *f) {
  if (*a=='(') {
    a++;
    a.readexpr(f->expr,sizeof(f->expr),'(',')');
    a.jcw();
  }

/*  if (*a=='@') {//vyznamny terminal:identifikator,cislo...,nebo specialni,napr. bop
    a++;
    char type[0x20];
    a.readidf(type);
    a.jcw();
    if (eqstr(type,"id")) f->type=ft_id;
    else if (eqstr(type,"str")) f->type=ft_str;
    else if (eqstr(type,"num")) f->type=ft_num;
    else a.error();
  }*/
  a.jcw();
  while (a.isbody('|') && a.isbody('.') && a.isbody('=') && a.isbody('~')) {

    if (*a=='0') {
      a++;a.jcw();continue;
    }
    FactorItem *fi=f->additem();
    loadfactoritem(a,fi);
  }
  a.jcw();
  f->output=NULL;
  if (*a=='=') {
    a++;
    StrOutput *out=new StrOutput();
    f->output=out;
    out->load(a);
//    loadfactorout(a,f);
  } else if (*a=='~') {
    a++;
    XMLOutput *out=new XMLOutput();
    f->output=out;
    out->load(a);
  }
  if (*a=='|') a++,a.jcw();
}

bool RegexFactor::accept(STRING what) {
  for(int i=0;i<lists.count;i++) {
    if (lists[i]->items.search(what)>=0) return false;
  }
  return vorwrds.search(what)<=0;
}

bool RegexFactor::parse(Parser &p,TreeNode **res) {
  if (skipwhites) p.jmpwhite();
  string s;
  if (reg->doparse(p.act,s) && accept(s)) {
    *res=createnode(p);
    (*res)->text=p.allocchar(s.c_str());
    return true;
  }
  p.setlasterror(errname+" expected");
  return false;
/*    char idf[0x100];
    if (! p.isidf() ) {
      p.setlasterror("Identifier expected");
      return false;
    }
    p.readidf(idf);
    if (p.lang->keywords.search(idf)>=0) {
      p.setlasterror("Identifier expected");
      return false;
    }
    *res=createnode(p);
    (*res)->text=p.allocchar(idf);
//    res->after=p.getpos();
    return true;
*/
}
void RegexFactor::writestrout(TreeNode *node,STROUT &fw,TreeWriter &tw,STRING param) {
  if (quotedout) fw.writecstring(node->text,'"');
  else fw<<node->text;
}

void RegexFactor::link() {
  for(int i=0;i<vorlist.count;i++)
    if (NamedList *list=lang->lists.searchvalue(vorlist[i]))
      lists.add(list);
}

void RegexFactor::load(Analyser &a) {
  if (*a=='%') skipwhites=false;
  a++;//pres @ nebo %
//  a.f_char('@');
  reg=RegItem::load(a);
  a.f_char(';');
  a.jcw();
  if (*a=='{') {
    a++;
    errname=a.readcstring('}');
    a.jcw();
  }
  if (*a=='[') {
    a++;
    while (a.isbody(']')) {
      if (*a=='q') quotedout=true,a++;
      else a.error("] expected");
    }
    a.f_char(']');
    a.jcw();
  }
  while (a.isbody('.') && a.isbody('|')) {
    if (a.is_idf()) {
      char s[0x100];a.readidf(s);
      vorlist.add(s);
    } else if (*a=='"') {
      a++;
      char s[0x100];a.readcstring(s);
      vorwrds.add(s);
    } else a.error(". expected");
    a.jcw();
  }
  if (*a=='|') a++,a.jcw();
}

static void loadterm(Analyser &a,Term *t) {
  if (!a.begjmp(":=")) a.error(":= expected");
  a.jcw();

  while (a.isbody('.')) {
    a.jcw();
    if (*a=='@' || *a=='%') {//regex-factor
      RegexFactor *rf=t->addregexfactor();
      a.jcw();
      rf->load(a);
    } else {
      Factor *f=t->addfactor();
      a.jcw();
      loadfactor(a,f);
    }
  }
  if (*a=='.') a++,a.jcw();
}

void Whites::load(Analyser &a) {
  if (!a.begjmp(":=")) a.error(":= expected");
  a.jcw();
  while (a.isbody('.')) {
    if (*a=='\'' || *a=='"') {
      Remark &rem=rems.add();
      rem.load(a);
    } else if (*a=='[') {
      a++;
      whites.regload(a);
      a.f_char(']');
    } else a.error(". excpected");
    a.jcw();
  }
  a.f_char('.');
  a.jcw();
}

void Language::loadgram(Analyser &a) {
  a.jcw();
  while (*a) {
    a.jcw();
    if (*a=='{') {a.jumpafter("}");a.jcw();continue;}
    if (*a=='#') {//preprocesor
      a++;
      if (a.begjmp("if ")) {
        string var=a.readto(" ",true);
        string val=a.readto(":",true);
        string end=a.readto(";",true);
        if (vars.findvalue(var)==val) {
          a.jcw();continue;
        } else {
          a.jumpafter(("#."+end+";").c_str());
          a.jcw();continue;
        }
      }
      if (*a=='.') {a++;a.jumpafter(";");a.jcw();continue;}
      if (a.begjmp("include ")) {
        string file=a.readto(";",true);
        included.add((loading.getdir()/file).c_str());
        a.jcw();continue;
      }
      a.error("include, 'end; or if expected");
    }
    if (!a.is_idfbeg()) a.error("Identifier expeced");
    char idf[0x100];
    a.readidf(idf);
    a.jcw();
    if (eqstr(idf,"whites")) {
      a.readidf(idf);
      a.jcw();
      Whites *wh=addwhites(idf);
      wh->load(a);
    } else if (eqstr(idf,"endw")) {
      CharSet chs;
      a.readidf(idf);
      a.jcw();
      a.f_char(':');a.f_char('=');a.jcw();
      a.f_char('[');
      chs.regload(a);
      a.f_char(']');
      NamedCharSet *endw=new NamedCharSet();
      endw->set=chs;
      endw->name=idf;
      endws.add(endw);
      a.jcw();a.f_char('.');a.jcw();
    } else if (eqstr(idf,"list")) {
      a.readidf(idf);
      a.jcw();
      NamedList *list=new NamedList();
      lists.add(list);
      list->name=idf;
      a.jcw();
      a.f_char(':');a.f_char('=');a.jcw();
      while (a.isbody('.')) {
        if (*a=='"') {
          a++;list->items.add(a.readcstring('"'));
        } else if (a.is_idf()) {
          a.readidf(idf);
          list->items.add(idf);
        } else a.error("List item expected");
        a.jcw();
      }
      a.f_char('.');
    } else {
      Term *t=searchterm(idf);
      if (t) a.error(fmtstr("Redeclared term:%s",idf));
      t=addterm(idf);
      loadterm(a,t);
    }
  }
//  a.jumpchar('\n');
}

void Factor::link() {
  for(int k=0;k<items.count;k++) {
    FactorItem *fi=items[k];
    fi->type=lang->searchterm(fi->typestr);
    if (*fi->typestr && !fi->type)
      throw JError(fmtstr("Term not known:%s",fi->typestr));
  }
}

void Term::link() {
  for(int j=0;j<factors.count;j++) factors[j]->link();
}

void Language::link() {
  for(int i=0;i<terms.count;i++) terms[i]->link();
}

void TreeNode::_debugwrite(STROUT &fw,string prefix) {
/*  fw<<prefix;
  if (text) {
    fw<<"text:"<<text;
  } else {
    fw<<"factor:"<<factor->term->name;
//    if (item) fw<<";item:"<<item->name;
  }
  fw<<"\n";
  if (child) child->debugwrite(fw,prefix+"  ");*/
}

void TreeNode::debugwrite(STROUT &fw,string prefix) {
  fw<<prefix;
  if (text) {
    fw<<"text:"<<text;
  } else {
    fw<<"factor:"<<factor->term->name;
  }
  fw<<"\n";
  for(int i=0;i<childcnt;i++) {
    childs[i].node->debugwrite(fw,prefix+"  ");
  }
/*  TreeNode *act=this;
  while (act->next) act=act->next;
  while (act) {//projeti spojaku odzadu
    act->_debugwrite(fw,prefix);
    if (act==this) break;
    TreeNode *n=this;
    while (n->next!=act) n=n->next;
    act=n;
  }*/
}

void StrOutput::writestrout(TreeNode *node,STROUT &fw,TreeWriter &tw,STRING param) {
//  if (node->factor->type==ft_str || node->factor->type==ft_id || node->factor->type==ft_num) {
//    if (node->text) fw<<node->text;
//  } else {
    int start=0;
    if (*param) {
      while (start<outs.count) {
        StrOutItem *oi=outs[start];
        if ((oi->type==ot_default) || (oi->type==ot_label && eqstr(param,oi->name))) break;
        start++;
      }
    }
    for(int i=start;i<outs.count;i++) {
      StrOutItem *out=outs[i];
      switch (out->type) {
        case ot_direct:fw<<out->name;break;
        case ot_subnode:{
          TreeNode *child=node->searchchild(out->name);
          STRING p=out->param;
          if (p=="*") p=param;
          if (child) child->writeout(fw,tw,p);
          else fw<<"******unknown subnode:"<<out->name<<"\n";
        } break;
        case ot_value:{
          TreeNode *child=node->searchchild(out->name);
          if (child) fw<<child->ivalue;else fw<<"******unknown subnode:"<<out->name<<"\n";
        } break;
        case ot_string:{
          TreeNode *child=node->searchchild(out->name);
          MEMSTROUT mso;
          if (child) child->writeout(mso,tw,"");else fw<<"******unknown subnode:"<<out->name<<"\n";
          fw.writecstring(mso.str().c_str());
        } break;
        case ot_query:{
          tw.query(out->name,fw,node);
        } break;
        case ot_break:return;
      }
    }
//  }
}

struct TN_Vars:IVariables {
  TreeNode *node;
  int getivalue(const char *name) {
    return node->searchchildvalue(name);
/*    TreeNode *act=node->child;
    while (act) {
      if (act->factor && eqstr(name,act->item->varname)) return act->ivalue;
      act=act->next;
    }*/
    return 0;
  }
};

int TreeNode::evalexpr(const char *expr) {
  TN_Vars tnv;tnv.node=this;
  return gram::evalexpr(expr,tnv);
}

int TreeNode::searchchildvalue(STRING name) {
  for(int i=0;i<childcnt;i++) {
    TreeNode *act=childs[i].node;
    if (act->factor && eqstr(name,childs[i].item->varname)) return act->ivalue;
  }
  return 0;
}

TreeNode *TreeNode::searchchild(STRING name) {
  for(int i=0;i<childcnt;i++) {
    TreeNode *act=childs[i].node;
    if (act->factor && name==childs[i].item->name) return act;
  }
/*  TreeNode *act=child;
  while (act) {
    if (act->factor && name==act->item->name) return act;
    act=act->next;
  }*/
  return NULL;
}

bool Parser::jmpword(STRING word,bool mustbeword) {
  if (!act.begins(word)) return false;
  int len=strlen(word);
  if (mustbeword) {
    char ch=act[len];
    if (endwchars && ! (*endwchars)[ch]) return false;
  }
  act+=len;
  return true;
}

bool FactorItem::parse(Parser &p,TreeNode **res) {
  Analyser &a=p.act;
  if (fit==fit_symbol || fit==fit_word) {
    if (skipwhites) p.jmpwhite();
    if (p.jmpword(name,fit==fit_word)) {
      return true;
    } else {
      p.setlasterror(string(name)+" expected");
      #ifdef GMDEBUG
      p.fail_debug<<factor->term->name<<"["<<name<<"]  :"<<a<<"\n";
      #endif
      return false;
    }
  } else if (fit==fit_eof) {
    if (skipwhites) p.jmpwhite();
    if (*a) p.setlasterror("End of file expected");
    return *a==0;
  } else if (fit==fit_setwhites) {
    p.actwhites=p.lang->whites.searchvalue(name);
    return true;
  } else if (fit==fit_setendw) {
    p.endwchars=p.lang->searchendw(name);
    return true;
  } else if (fit==fit_term) {
    if (type) {
//      TreeNode *t=NULL;
//      if (skipwhites) p.jmpwhite();
      bool ok=type->parse(p,res);
//      if (*res) (*res)->item=this;
      return ok;
//      return type->parse(p,res);
//      if (type->parse(p)) {
//        *res=t;
//        t->item=this;
//      }
//      #ifdef GMDEBUG
//      if (!t) p.fail_debug<<factor->term->name<<"{"<<typestr<<"}  :"<<a<<"\n";
//      #endif
//      return t;
    } else {
      p.setlasterror("Unknown term:"+string(typestr));
      return false;
    }
  }
  return false;
}

bool Factor::parse_left_recur(Parser &p,TreeNode **res) {
//  TreeNode *res=term->parsenoLR(p);
//  if (!res) return NULL;
  if (!term->parsenoLR(p,res)) return false;

  for(;;) {
    Factor *fac=NULL;
    TreeNode *right=NULL;
    if (term->parseLRrest(p,&fac,&right)) {//posunuti stromu
      right->addchild(*res,fac->items[0]);
      right->ivalue=right->evalexpr(expr);
//      res->item=fac->items[0];
      *res=right;
    } else return res;
/*    if (term->parseLRrest(p,&fac,right)) {//posunuti stromu
      TreeNode *r=createnode(p);
      r->factor=fac;
      r->child=res;
      res->next=right;
      res->item=fac->items[0];
      r->ivalue=r->evalexpr(expr);
      res=r;
    } else return res;*/
  }

}

void ParsedTerm::init(const char *pos,const char *afterpos,Term *term,TreeNode *anode,bool aparsedok) {
  hashcode=0;next_hash=NULL;node=anode;after=afterpos;
  nodeid.term=term;
  nodeid.pos=pos;
  parsedok=aparsedok;
  hashcode=term->mixwith(pos);
}

TreeNode *AbstractFactor::createnode(Parser &p) {
  TreeNode *res=p.allocnode();
  res->init();
  res->factor=this;
  return res;
}

bool Factor::parseLRrest(Parser &p,TreeNode **res) {
  *res=createnode(p);
  for(int i=1;i<items.count;i++) {
    TreeNode *sub=NULL;
    if (items[i]->vorbidden) {
      ParserPos pos=p.getpos();
      if (items[i]->parse(p,&sub)) return false;
      p.setpos(pos);
    } else {
      if (! items[i]->parse(p,&sub)) return false;
      if (sub) {
        (*res)->addchild(sub,items[i]);
/*        if (res) {
          sub->next=res->next;
          res->next=sub;
        } else res=sub;*/
      }
    }
  }
  return true;
}

bool Factor::parse(Parser &p,TreeNode **res) {
  Analyser &a=p.act;

/*  if (type==ft_id) {
    p.jmpwhite();
    char idf[0x100];
    if (! p.isidf() ) {
      p.setlasterror("Identifier expected");
      return false;
    }
    p.readidf(idf);
    if (p.lang->keywords.search(idf)>=0) {
      p.setlasterror("Identifier expected");
      return false;
    }
    *res=createnode(p);
    (*res)->text=p.allocchar(idf);
//    res->after=p.getpos();
    return true;
  } else if (type==ft_str) {
    p.jmpwhite();
    if (! p.isstr() ) {
      p.setlasterror("String expected");
      return false;
    }
    string txt=p.readstr();
    *res=createnode(p);
    (*res)->text=p.allocchar(txt.c_str());
//    res->after=p.getpos();
    return true;
  } else if (type==ft_num) {
    p.jmpwhite();
    if (! p.isint() ) {
      p.setlasterror("Number expected");
      return false;
    }
    char num[0x100];
    a.readdouble(num);
    *res=createnode(p);
    (*res)->text=p.allocchar(num);
//    res->after=p.getpos();
    return true;
  } else*/
  if (isLR()) {
    return parse_left_recur(p,res);
  } else {
//    ParserState state=p.store();
    *res=createnode(p);
    for(int i=0;i<items.count;i++) {
      TreeNode *sub=NULL;
      if (items[i]->vorbidden) {
        ParserPos pos=p.getpos();
        if (items[i]->parse(p,&sub)) return false;
        p.setpos(pos);
      } else {
        if (! items[i]->parse(p,&sub)) return false;
        if (sub) (*res)->addchild(sub,items[i]);
      }
    }
    if (*expr) (*res)->ivalue=(*res)->evalexpr(expr);
//    res->after=p.getpos();
    return true;
  }
  return false;
}

ParsedTerm *Term::getpresult(const char *pos,const char *after,TreeNode *node,Parser &p,bool parsedok) {
  ParsedTerm *res=(ParsedTerm*)p.allocate(sizeof(ParsedTerm));
  if (!parsedok) node=NULL;
  res->init(pos,after,this,node,parsedok);
  return res;
}

bool Term::parse(Parser &p,TreeNode **r) {
  ParserPos orig=p.getpos();

  #ifdef GMDEBUG
  XMLRef run_parent=p.run_actchild;
  XMLNode running=run_parent.add("running");
  p.run_actchild=running.getnode();
  running.setname(name);
  running.setvar("begin",p.act.act);
  p.trun_debug<<"Running:"<<name<<":"<<p.act<<"\n";
  #endif

  if (ParsedTerm *res=p.parsedterms.search(mixwith(orig.readpos),NodeId(this,orig.readpos))) {
    p.act=res->after;
    *r=res->node;
    #ifdef GMDEBUG
    running.setvar("result",res->parsedok?"true":"false");
    running.setvar("method","search");
    running.setvar("end",p.act.act);
    p.trun_debug<<"Searched:"<<name<<"["<<(res->parsedok?"true":"false")<<"]"<<":"<<p.act<<"\n";
    p.run_actchild=run_parent;
    #endif
    return res->parsedok;
  }
  for(int i=0;i<factors.count;i++) {
    if (factors[i]->parse(p,r)) {
      ParsedTerm *res=getpresult(orig.readpos,p.act,*r,p,true);
      p.parsedterms.add(res);
      #ifdef GMDEBUG
      running.setvar("result","true");
      running.setvar("method","parse");
      running.setvar("end",p.act.act);
      p.trun_debug<<"Parsed:"<<name<<"["<<"true"<<"]"<<":"<<p.act<<"\n";
      p.run_actchild=run_parent;
      #endif
      return true;
    }
    p.setpos(orig);
  }
  ParsedTerm *res=this->getpresult(orig.readpos,p.act,NULL,p,false);
  p.parsedterms.add(res);
  #ifdef GMDEBUG
  running.setvar("result","false");
  running.setvar("method","parse");
  running.setvar("end",p.act.act);
  p.trun_debug<<"Parsed:"<<name<<"["<<"false"<<"]"<<":"<<p.act<<"\n";
  p.run_actchild=run_parent;
  #endif
  return false;
}

bool Term::parseLRrest(Parser &p,Factor **fac,TreeNode **res) {
  ParserPos orig=p.getpos();
  for(int i=0;i<factors.count;i++) if (factors[i]->isLR()) {
    if (((Factor*)factors[i])->parseLRrest(p,res)) {
      *fac=(Factor*)factors[i];
      return true;
    }
    p.setpos(orig);
  }
  return false;
}

bool Term::parsenoLR(Parser &p,TreeNode **res) {
  ParserPos orig=p.getpos();
  for(int i=0;i<factors.count;i++) if (! factors[i]->isLR()) {
    if (factors[i]->parse(p,res)) return true;
//    TreeNode *res=factors[i]->parse(p);
//    if (res) return res;
    p.setpos(orig);
  }
  return false;
}

string Parser::readstr() {
  char buf[0x1000];
  char beg=*act;act++;
  act.readcstring(buf,beg);
  return buf;
}

TreeNode *Parser::parse(const char *src,const char *term) {
  setsrc(src);
  Term *t=lang->searchterm(term);
//  try {
  TreeNode *node=NULL;
  #ifdef GMDEBUG
  run_actchild=run_debug;
  #endif
  bool ok=t->parse(*this,&node);
  if (!ok) error();
  return node;
/*  } catch (Analyser &error) {
//    CommonParseError pe;
    int line,col;
    string eline;
    fmterror(src,error.act,line,col,eline,true);
    jsystem->showerror(lasterror+"\n"+eline);
    return NULL;
  }*/
}


}


