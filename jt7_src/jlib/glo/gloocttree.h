#ifndef gloocttreeH
#define gloocttreeH

namespace glo {

class otNode;

/*
struct otObject {
  virtual void getcube(int id,float a[3],float b[3])=0;
};
*/

struct otItem {
//  otObject *obj;
  void *obj;
  int id;

  int priv_id;//index ukazujici na otCounter
  otItem(void *obj=NULL,int id=0):obj(obj),id(id) {priv_id=-1;}
  bool operator ==(const otItem &it) const {return it.obj==obj && it.id==id;}
};

struct otCounter {
  bool destroyed;
  bool added;
  int nextfree;
};

struct otClasifier {
  //vrati: -1:cube neni vubec viditelna,1:cube je cela viditelna,0:nevi se(cast viditelna, cast ne)
  virtual int visiblecube(float coords[3],float size)=0;
};

struct otProjector {
  virtual void project(float in[3],float out[2])=0;
};

typedef bool (*otRemoveFunc)(const otItem &item,void *data);

struct OctTree {
  OctTree();
  ~OctTree();
  otNode *add(otItem item,float a[3],float b[3],bool isdynamic=false);
  void remove(const otItem &item);
  void removegroup(void *obj);
  void removegroup(otRemoveFunc f,void *data=NULL);
  void getvisible(TCntVec<otItem> &res,otClasifier &cl);//vrati objekty, ktere mohout byt viditelne
  void init(float mins,float maxs,float p0[3]=NULL);
  void cleardynamics();
  void quickremove(const otItem &item,otNode *place);
private:
  TCntVec<otNode*> dyns;

  int getfree();
  void destroy(int i);

  otCounter *counters;
  otNode *emptychilds[2][2][2];
  int counterscapa;
  int firstfreecounter;
  float minsize;
  otNode *main;
  friend class otNode;
};

struct otNode {
  otNode *parent;
  otNode *(*childs)[2][2];
//  otNode *childs[2][2][2];
  float coords[3];
  float size;
  OctTree *tree;
  otNode *pnext[3];
  bool cannotnext[3];
  bool empty;//tento uzel i vsechny jeho deti jsou prazdne
  bool hasdynamic;//ma dynamic:toto nebo rodic
  unsigned char parpos[3];//pozice v rodici, 0 nebo 1
  short level;//hlavni uroven:0
  TCntVec<otItem> items;
  TCntVec<otItem> ditems;

  void add(const otItem &item,bool isdynamic);
  void remove(const otItem &item);
  void removegroup(void *obj);
  void removegroup(otRemoveFunc f,void *data);
  otNode *&child(unsigned char d[3]) {return childs[d[0]][d[1]][d[2]];}
  otNode *&operator [] (int d[3]) {return childs[d[0]][d[1]][d[2]];}
  otNode *forceat(float pos[3],float size);
  otNode *getchild(float pos[3]);//vrati dite s danou pozici
  otNode *createchild(unsigned char d[3]);//vytvori dite, pokud neexistuje
  otNode *countnext(int dir);//vrati (vytvori) dalsi otNode v danem smeru stejne velikosti
  otNode *next(int dir);
  void getvisible(TCntVec<otItem> &res,otClasifier &cl);
  void addall(TCntVec<otItem> &res);
  void quickremove(const otItem &item);
  void doremove(const otItem &item);
  void updateempty();
  void wantchilds();

  otNode(otNode *aparent);
  ~otNode();
};

int ot_map_visible_cube(otProjector &prj,float coords[3],float size);

}

#endif
