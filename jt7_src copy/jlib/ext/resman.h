#ifndef resmanH
#define resmanH

namespace jlib {

/*
template<class T,class S> struct ADAPTER {
  //T je datovy typ, s je typ pro ulozeni handle
  //funkce,ktere je treba definavat:
  void allochandle(const T &data,S &handle);
  void freehandle(S &handle);
  bool equalresources(const T &r1,const T &r2);
  int gethashcode(const T &data);
//tyto fce neni treba definovat pri odvozeni od jedne z nasl. dvou trid
  void addhandle(const S &handle);
  int getcount();
};
*/

template<class S> struct SIMPLERESOURCEBASE {
  void addhandle(const S &handle) {};
  int getcount() {return 0;}
};

template<class S> struct VECTORRESOURCEBASE {
  TCntVec<S> handles;
  void addhandle(const S &handle) {handles+=handle;};
  int getcount() {return handles.count;}
  //POZOR::handles jsou v opecnem poradi nez spojak v RESOURCEMANAGERu
};

template<class T,class S,class ADAPTER> struct RESOURCE {
  T data;
  S handle;
  ADAPTER *owner;
  int hashcode;
  RESOURCE *next;
  RESOURCE(ADAPTER *aowner,RESOURCE *anext,const T &adata,int ahashcode):data(adata) {
    next=anext;hashcode=ahashcode;
    owner=aowner;
    owner->allochandle(data,handle);
  }
  ~RESOURCE() {owner->freehandle(handle);}
};

template<class T,class S,class ADAPTER> struct RESOURCEMANAGER:ADAPTER {
  typedef RESOURCE<T,S,ADAPTER> ITEM;
  void clear() {
    ITEM *act=list;
    while (act) {
      ITEM *next=act->next;
      delete act;
      act=next;
    }
    list=NULL;
  }
  ITEM *list;
  S findres(const T &src,int *index=NULL) {//index je pocitan od konce,posledni prvek ma 1,predposledni 2...
    ITEM *act=list;
    int srchash=gethashcode(src);
    int actidx=0;
    while (act) {
      if (act->hashcode==srchash and equalresources(src,act->data)) {
        if (index) *index=getcount()-actidx-1;
        return act->handle;
      }
      act=act->next;
      actidx++;
    }
    list=new ITEM(this,list,src,srchash);
    addhandle(list->handle);
    if (index) *index=getcount()-1;
    return list->handle;
  }
  ~RESOURCEMANAGER() {clear();}
  RESOURCEMANAGER() {list=NULL;}
};


}
#endif
