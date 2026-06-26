#ifndef gmlangH
#define gmlangH

#include <ext/qalloc.h>
#include "gmanal.h"
#include <ext/xmlnode.h>
#include <com/jhash.h>
#include "gmsexpr.h"

//#define GMDEBUG

namespace gram {

class Term;class TreeNode;
class Language;class Parser;
class Factor;
//class AbstractTerm;

struct Remark {
  bool nested;
  char beg[0x10],end[0x10];
//  void load(const char *c);
  Remark() {*beg=0;*end=0;nested=false;}
  void load(Analyser &a) {
    nested=(*a=='\'');
    char ch=*a;a++;
    a.readcstring(beg,ch);
    a.jcw();
    a.f_char(ch);
    a.readcstring(end,ch);
  }
  void jump(Analyser &a);//predoklad:strbegcmp(beg,c)
};

struct Whites {
  string name;
  Language *lang;
  TCntVec<Remark> rems;
  CharSet whites;
  void jmpchars(Analyser &a) {while (whites[*a]) a++;}
  void jmpwhite(Analyser &a);
  void load(Analyser &a);
  Whites(STRING name,Language *lang):name(name),lang(lang) {}
  bool match(STRING aname) {return name==aname;}
};

class Term;

/*
struct Translation {
  Factor *factor;
  Translation(Factor *factor):factor(factor) {}
  virtual ~Translation() {}
  virtual Translation *createnew(Factor *factor) {return new Translation(factor);}
  virtual void load(Analyser &a) {}
//  Translation(FactorItem *factor) {}
};
*/

enum FactorItemType {fit_symbol,fit_word,fit_term,fit_eof,fit_setwhites,fit_setendw};
//fit_symbol:pise se do ''
//fit_word:pise se do "",musi byt ukoncen endwchar
//fit_term:rekurzivni definice pomoci jineho termu
//fit_eof:konec vstupu, pise se $

struct FactorItem {
  FactorItemType fit;
  bool vorbidden;//zda tato FactorItem je zakazana=pri nacteni neuspech(zapisuje se s !)

  char name[0x20],typestr[0x20];
  char varname[0x20];//co je v zavorce za jmenem item

  Factor *factor;
  Term *type;
  bool skipwhites;

  bool parse(Parser &p,TreeNode **res);//vrati, zda byla node uspesne nactena
  bool match(STRING aname) {return aname==name;}
  FactorItem(Factor *factor):factor(factor) {
    skipwhites=true;type=NULL;*name=*typestr=*varname=0;
  }
};

class TreeWriter;
struct Output {
  virtual void writestrout(TreeNode *node,STROUT &fw,TreeWriter &tw,STRING param);
  virtual void writexmlout(TreeNode *node,XMLNode xml,TreeWriter &tw);
};

enum StrOutType {ot_direct,ot_subnode,ot_query,ot_value,ot_string,ot_label,ot_break,ot_default};
struct StrOutItem {
  StrOutType type;
  char name[0x40];//bud retezec na vystup nebo jmeno podstromu
  char param[0x20];
  StrOutItem() {*name=*param=0;type=ot_direct;}
};

struct StrOutput:Output {
  void writestrout(TreeNode *node,STROUT &fw,TreeWriter &tw,STRING param);
  void load(Analyser &a);
  StrOutItem *addout() {
    StrOutItem *out=new StrOutItem();
    outs.add(out);
    return out;
  }

  IObjArray<StrOutItem> outs;
};

struct XMLOutItem {
  virtual void work(TreeNode *node,XMLNode xml,TreeWriter &tw)=0;
};

struct XMLSimpleItem:XMLOutItem {
  XMLSimpleItem() {*name=*text=0;}
  void work(TreeNode *node,XMLNode xml,TreeWriter &tw);

  char text[0x100];
  char name[0x20];
};

struct XMLStrOutItem:XMLOutItem {
  void work(TreeNode *node,XMLNode xml,TreeWriter &tw);
  XMLStrOutItem() {*name=0;}
  void load(Analyser &a) {out.load(a);}

  StrOutput out;
  char name[0x20];
};

struct SubXMLOutItem:XMLOutItem {
  void work(TreeNode *node,XMLNode xml,TreeWriter &tw);
  SubXMLOutItem() {*name=0;addsub=false;}

  char name[0x20];
  bool addsub;
};

struct XMLCompoud:XMLOutItem {
  char name[0x20];
  bool simplesub;//<> primo v out

  void work(TreeNode *node,XMLNode xml,TreeWriter &tw);
  void load(Analyser &a);

  IObjArray<XMLOutItem> outs;
  XMLCompoud() {*name=0;simplesub=false;}
};

struct XMLOutput:Output {
  void writexmlout(TreeNode *node,XMLNode xml,TreeWriter &tw) {out.work(node,xml,tw);}
  void load(Analyser &a) {out.load(a);}

  XMLCompoud out;
};

//enum FactorType {ft_normal,ft_id,ft_str,ft_num};


struct AbstractFactor {
  Term *term;
  Language *lang;
  virtual bool isLR() {return false;}
  virtual bool parse(Parser &p,TreeNode **res)=0;
  virtual void link() {}
  virtual void writestrout(TreeNode *node,STROUT &fw,TreeWriter &tw,STRING param) {}
  virtual void writexmlout(TreeNode *node,XMLNode &xml,TreeWriter &tw) {}
  TreeNode *createnode(Parser &p);
  AbstractFactor(Term *term,Language *lang):term(term),lang(lang) {}
};

struct Factor:AbstractFactor {
  char expr[0x100];
//  Translation *tr;
  IObjArray<FactorItem> items;
  Output *output;
//  FactorType type;//0:standartni,1:id,2:string

  FactorItem *additem() {
    FactorItem *fi=new FactorItem(this);
    items.add(fi);
    return fi;
  }
  Factor(Term *term,Language *lang):AbstractFactor(term,lang) {*expr=0;output=NULL;}//tr=NULL;}
  ~Factor() {delete output;}
  FactorItem *searchitem(STRING name) {return items.searchvalue(name);}
  bool parse(Parser &p,TreeNode **res);
  bool parseLRrest(Parser &p,TreeNode **res);//parsing zbytku z leve rekurzivniho faktoru(predp:isLR())
  //vraci stroom jako parse, ale bez nejlevejsi TreeNode(jeou posunute o 1 doprava)
  bool isLR() {return items.count>1 && items[0]->type==term;}
  void link();
  override void writestrout(TreeNode *node,STROUT &fw,TreeWriter &tw,STRING param) {if (output) output->writestrout(node,fw,tw,param);}
  override void writexmlout(TreeNode *node,XMLNode &xml,TreeWriter &tw) {if (output) output->writexmlout(node,xml,tw);}

  char bop_left[0x20],bop_op[0x10],bop_right[0x20];
private:
  bool parse_left_recur(Parser &p,TreeNode **res);
};

class NamedList;
struct RegexFactor:AbstractFactor {
  RegItem *reg;
  STRLIST vorlist;
  STRLIST vorwrds;
  TCntVec<NamedList*> lists;
  string errname;
  bool quotedout;
  bool skipwhites;

  override bool parse(Parser &p,TreeNode **res);
  override void writestrout(TreeNode *node,STROUT &fw,TreeWriter &tw,STRING param);
  RegexFactor(Term *term,Language *lang):AbstractFactor(term,lang) {reg=NULL;quotedout=false;skipwhites=true;}
  ~RegexFactor() {delete reg;}
  bool accept(STRING what);
  void load(Analyser &a);
  void link();
};

class ParsedTerm;

/*
struct AbstractTerm {
  virtual bool parse(Parser &p,TreeNode **res)=0;
  virtual void link()=0;
};
*/

struct Term {
  char name[0x100];
  Language *lang;
  IObjArray<AbstractFactor> factors;
  Factor *addfactor() {Factor *f=new Factor(this,lang);factors.add(f);return f;}
  RegexFactor *addregexfactor() {RegexFactor *f=new RegexFactor(this,lang);factors.add(f);return f;}
  Term(const char *aname,Language *lang):lang(lang) {strcpy(name,aname);}
  bool match(const char *aname) {return eqstr(name,aname);}
  override bool parse(Parser &p,TreeNode **res);
  override void link();

//  Tree
  bool parsenoLR(Parser &p,TreeNode **res);//parsing ne leve rekurzivniho termu
  bool parseLRrest(Parser &p,Factor **fac,TreeNode **res);//parsing zbytku z leve rekurzivniho faktoru(prvniho, ktery padne)
  int mixwith(const char *pos) {return (int)this^(int)pos;}
  ParsedTerm *getpresult(const char *pos,const char *after,TreeNode *node,Parser &p,bool parsedok);
};

struct NamedCharSet {
  CharSet set;
  string name;
  bool match(STRING aname) {return name==aname;}
};

struct NamedList {
  STRLIST items;
  string name;
  bool match(STRING aname) {return name==aname;}
};

struct Language {
  VarList vars;
//  CharSet idfs,whites,endwchars,idfbegs;
//  STRLIST keywords;
//  TCntVec<Remark> remarks;
  IObjArray<Whites> whites;
  IObjArray<Term> terms;
  IObjArray<NamedCharSet> endws;
  IObjArray<NamedList> lists;
//  void load(const char *s,Translation *tr=NULL);//at the end delete tr is called
  void load(const char *s) {_load(s);link();}
  void loadfiles();
  void addfile(string file) {included.add(file);}

  Term *addterm(const char *name) {Term *t=new Term(name,this);terms.add(t);return t;}
  Whites *addwhites(const char *name) {Whites *w=new Whites(name,this);whites.add(w);return w;}
  void link();
  Term *searchterm(const char *name) {return terms.searchvalue(name);}
  CharSet *searchendw(STRING name) {NamedCharSet *res=endws.searchvalue(name);return res?&res->set:NULL;}
private:
//  void loadgram(Analyser &a,Translation *tr);
  STRLIST included;
  sFile loading;
  void loadgram(Analyser &a);
  void loadchars(Analyser &a);
  void _load(const char *s) {Analyser a(s);loadgram(a);}
};

struct TreeWriter {
  virtual void query(const char *query,STROUT &fw,TreeNode *node)=0;
};

struct ParserState {
  Whites *actwhites;
  CharSet *endw;
  ParserState() {actwhites=NULL;endw=NULL;}
};

struct ParserPos {
  const char *readpos;
  ParserState state;
  ParserPos() {readpos=NULL;}
//  StackAllocator::Pos allocpos;
};

struct NodeId {
  Term *term;
  const char *pos;
  bool operator ==(const NodeId &id) const {return id.term==term && id.pos==pos;}
  NodeId(Term *aterm,const char *apos) {term=aterm;pos=apos;}
};

struct TreeNode {
  void *writer_data;//nepouzite, muze vyuzit TreeWriter
  AbstractFactor *factor;
  Term *term;
//  FactorItem *item;
  char *text;//=NULL,pokud je terminal
  int ivalue;

//  TreeNode *child;
  void addchild(TreeNode *child,FactorItem *item) {childs[childcnt].node=child;childs[childcnt].item=item;childcnt++;}
//  TreeNode *next;
//  void init() {item=NULL;factor=NULL;text=NULL;child=NULL;next=NULL;ivalue=0;}
//  void init() {item=NULL;factor=NULL;text=NULL;child=NULL;next=NULL;writer_data=NULL;ivalue=0;hashcode=0;next_hash=NULL;after=NULL;}
  void init() {factor=NULL;text=NULL;writer_data=NULL;ivalue=0;childcnt=0;}

  TreeNode *searchchild(STRING name);
  int searchchildvalue(STRING name);

//  void writeout(STROUT &fw,TreeWriter &tw,STRING param="");
  void writeout(STROUT &fw,TreeWriter &tw,STRING param) {factor->writestrout(this,fw,tw,param);}// factor->output->writestrout(this,fw,tw,param);}
  void writeout(XMLNode xml,TreeWriter &tw) {factor->writexmlout(this,xml,tw);}//{if (factor->output) factor->output->writexmlout(this,xml,tw);}
  int evalexpr(const char *expr);

  void debugwrite(STROUT &fw,string prefix="");
  void _debugwrite(STROUT &fw,string prefix);

private:
  struct Child {
    TreeNode *node;
    FactorItem *item;
  };
  Child childs[0x10];
  int childcnt;
};

struct ParsedTerm {
  ParsedTerm *next_hash;
  int hashcode;
  NodeId nodeid;
  TreeNode *node;
  bool parsedok;
  const char *after;

  bool h_match(const NodeId &id) {return nodeid==id;}
  ParsedTerm *h_getnext() {return next_hash;}
  void h_setnext(ParsedTerm *n) {next_hash=n;}
  int h_hashcode() {return hashcode;}
  void init(const char *pos,const char *afterpos,Term *term,TreeNode *node,bool parsedok);
};

//typedef Analyser ParserPos;
//typedef const char * ParserPos;
//struct ParserPos {
//};

struct Parser {
//  void jmpwhitechars() {while (lang->whites[*act]) act++;}
//  void jmpwhite();//bere v uvahu i poznamky
  Parser() {lang=NULL;lasterrorpos=NULL;actwhites=NULL;endwchars=NULL;}//idfactor=(Factor*)1;keywordfactor=(Factor*)2;intfactor=(Factor*)3;}

  Language *lang;
  Analyser act;
  Whites *actwhites;
  CharSet *endwchars;
  #ifdef GMDEBUG
  MEMSTROUT fail_debug,trun_debug;
  MainXMLNode run_debug;
  XMLRef run_actchild;
  #endif
  TreeNode *allocnode();
  char *allocchar(const char *str);

  void jmpwhite();
  ParserState store() {ParserState res;res.actwhites=actwhites;res.endw=endwchars;return res;}
  void restore(const ParserState &state) {actwhites=state.actwhites;endwchars=state.endw;}
//  bool jumpremark();//vrati, zda tam skutecne byla poznamka,ktera byla preskocena
  void readidf(char *buf,int size) {act.readidf(buf,size);}// while (lang->idfs[*act]) *buf++=*act,act++;*buf=0;}
//  bool isidf() {return lang->idfbegs[*act];}
//  bool isint() {return act.isdigit();}
//  bool isstr() {return *act=='"' || *act=='\'';}
  string readstr();
  ParserPos getpos();
  void setpos(ParserPos pos);
//  Factor *idfactor,*keywordfactor,*intfactor;
  void error() {act=lasterrorpos;act.error(getlasterror());}
  void setsrc(const char *src) {act=src;}
  void setlang(Language *alang) {lang=alang;}
  bool jmpword(STRING word,bool mustbeword);

  TreeNode *parse(const char *src,const char *term);

  void setlasterror(STRING msg) {if (act>lasterrorpos) lasterror=msg,lasterrorpos=act;}
  string getlasterror() {return lasterror;}

//  void *allocate(int size) {return malloc(size);}
  void *allocate(int size) {return allocator.alloc(size);}
  void setaproxinputsize(int size) {parsedterms.setminhashsize(size);}
private:
  StackAllocator allocator;
  IHashTable<ParsedTerm> parsedterms;
  string lasterror;
  const char *lasterrorpos;
  friend class Term;
};

}

#endif
