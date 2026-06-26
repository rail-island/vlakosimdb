#ifndef backinfoH
#define backinfoH

/**
BACKINFO slouzi pro uschovani zpetne informace pro interfacy(napr. INIFILEI) o objektech,
ktere toto interface vyuzivaji. Pri destrukci objektu, ktery implementuje interface,
pak muzi zaslat zpravu vsem objektum, ktere interface vyuzivaji.
*/

namespace jlib {

struct BACKINFO {
  struct REC {
    void *param,*info;
    void (*ondestroy)(void *param,void *info);
    REC *next;
    REC(REC *anext,void *aparam,void *ainfo,void (*aondestroy)(void *param,void *info)) {
      next=anext;param=aparam;info=ainfo;ondestroy=aondestroy;
    }
  };
  REC *first;
  void *find(void *param) {
    REC *act=first;
    while (act) {
      if (act->param==param) return act->info;
      act=act->next;
    }
    return NULL;
  }
  BACKINFO() {first=NULL;}
  ~BACKINFO() {
    REC *act=first;
    while (act) {
      act->ondestroy(act->param,act->info);
      REC *last=act;
      act=act->next;
      delete last;
    }
  }
  void add(void *param,void *info,void (*ondestroy)(void *param,void *info)) {
    first=new REC(first,param,info,ondestroy);
  }
  bool erase(void *param) {
    REC *act=first;
    REC *last=NULL;
    while (act) {
      REC *next=act->next;
      if (act->param==param) {
        delete act;
        if (last==NULL) first=next;else last->next=next;
        return true;
      }
      last=act;
      act=next;
    }
    return false;
  }
};

}

#endif
