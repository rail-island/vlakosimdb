#include <jlib.h>
#pragma hdrstop

#include "gloocttree.h"

namespace glo {

#define make_for_childs \
child_op(childs[0][0][0])\
child_op(childs[0][0][1])\
child_op(childs[0][1][0])\
child_op(childs[0][1][1])\
child_op(childs[1][0][0])\
child_op(childs[1][0][1])\
child_op(childs[1][1][0])\
child_op(childs[1][1][1])

inline void copydirs(unsigned char dst[3],unsigned char src[3]) {
  dst[0]=src[0];
  dst[1]=src[1];
  dst[2]=src[2];
}

void OctTree::init(float mins,float size,float p0[3]) {
  minsize=mins;
  if (main) delete main;
  main=new otNode(NULL);
  main->tree=this;
  main->size=size;
  if (p0) memcpy(main->coords,p0,sizeof(float)*3);
}

OctTree::~OctTree() {
  if (main) delete main;
  if (counters) free(counters);
}

otNode::~otNode() {
  #define child_op(child) if (child) delete child;
  make_for_childs
  #undef child_op
  if (!tree || childs!=tree->emptychilds) delete []childs;
}

otNode::otNode(otNode *aparent) {
  memset(this,0,sizeof(*this));
  empty=true;
  parent=aparent;
  if (parent) {
    tree=parent->tree;
    level=parent->level+1;
    childs=tree->emptychilds;
  } else {
    childs=new otNode*[2][2][2];
    memset(childs,0,8*sizeof(otNode*));
  }
}

void otNode::wantchilds() {
  if (tree && childs==tree->emptychilds) {
    childs=new otNode*[2][2][2];
    memset(childs,0,8*sizeof(otNode*));
  }
}

otNode *otNode::createchild(unsigned char d[3]) {
  otNode *res=child(d);
  float s2=size/2;
  if (!res) {
    wantchilds();
    otNode *&r=child(d);
    r=new otNode(this);
    r->coords[0]=coords[0]+d[0]*s2;
    r->coords[1]=coords[1]+d[1]*s2;
    r->coords[2]=coords[2]+d[2]*s2;
//    res->parpos[0]=d[0];
//    res->parpos[1]=d[1];
//    res->parpos[2]=d[2];
    copydirs(r->parpos,d);
//    memcpy(res->parpos,d,sizeof(unsigned char)*3);
    r->size=s2;
    return r;
  }
  return res;
}

otNode *otNode::getchild(float pos[3]) {
  float s2=size/2;
  unsigned char d[3];
  d[0]=pos[0]>coords[0]+s2;
  d[1]=pos[1]>coords[1]+s2;
  d[2]=pos[2]>coords[2]+s2;
  otNode *res=createchild(d);
  return res;
}

otNode *otNode::forceat(float pos[3],float s) {
  otNode *act=this;
  while (act->size>s && act->size>=tree->minsize*2) {
    act=act->getchild(pos);
  }
  return act;
}

void otNode::add(const otItem &item,bool isdynamic) {
  if (isdynamic) {
    ditems.add(item);
    otNode *act=this;
    while (act && !act->hasdynamic) {
      tree->dyns.add(act);
      act->hasdynamic=true;
      act=act->parent;
    }
  } else {
    items.add(item);
    otNode *act=this;
    while (act && act->empty) {
      act->empty=false;
      act=act->parent;
    }
  }
}


otNode *otNode::next(int dir) {
  if (cannotnext[dir]) return NULL;
  if (!pnext[dir]) {
    pnext[dir]=countnext(dir);
    cannotnext[dir]=(pnext[dir]==NULL);
  }
  return pnext[dir];
}

static otNode *count_next(otNode *act,otNode **stack,int dir) {
  unsigned char d[3];
  int depth=0;

  while (act && act->parpos[dir]) {
    stack[depth++]=act;
    act=act->parent;
  }
  if (!act) return NULL;
  stack[depth]=act;
  act=act->parent;
  if (!act) return NULL;
//  d[0]=
//  memcpy(d,stack[depth]->parpos,3*sizeof(unsigned char));
  copydirs(d,stack[depth]->parpos);
  d[dir]=1;
  act=act->createchild(d);
  for(int i=1;i<=depth;i++) {
    copydirs(d,stack[depth-i]->parpos);
//    memcpy(d,stack[depth-i]->parpos,3*sizeof(unsigned char));
    d[dir]=0;
    act=act->createchild(d);
  }
  return act;
}

otNode *otNode::countnext(int dir) {
  otNode **stack=new otNode*[level+1];
  otNode *res=count_next(this,stack,dir);
  delete []stack;
  return res;
}

otNode *OctTree::add(otItem item,float A[3],float B[3],bool isdynamic) {
//  float a[3],b[3];
//  item.obj->getcube(item.id,a,b);
  item.priv_id=getfree();
//  counters[item.priv_id].destroyed=false;
  float a[3],b[3];
  memcpy(a,A,3*sizeof(float));
  memcpy(b,B,3*sizeof(float));
  if (a[0]>b[0]) exchange(a[0],b[0]);
  if (a[1]>b[1]) exchange(a[1],b[2]);
  if (a[2]>b[2]) exchange(a[2],b[1]);
  float s=jmax(b[0]-a[0],b[1]-a[1],b[2]-a[2]);
  otNode *act=main->forceat(a,s);
  act->add(item,isdynamic);
  int over[3];
  over[0]=b[0]>act->coords[0]+act->size;
  over[1]=b[1]>act->coords[1]+act->size;
  over[2]=b[2]>act->coords[2]+act->size;

  otNode *next[3]={NULL,NULL,NULL};
  if (over[0]) next[0]=act->next(0);
  if (over[1]) next[1]=act->next(1);
  if (over[2]) next[2]=act->next(2);

  otNode *inc[2][2][2];//inc[0][0][0] nas nezajima (jakoby true)
  memset(inc,0,sizeof(inc));
  inc[1][0][0]=next[0];
  inc[0][1][0]=next[1];
  inc[0][0][1]=next[2];
  if (next[0] && next[1]) inc[1][1][0]=next[0]->next(1);
  if (next[0] && next[2]) inc[1][0][1]=next[0]->next(2);
  if (next[1] && next[2]) inc[0][1][1]=next[1]->next(2);
  if (next[0] && inc[0][1][1]) inc[1][1][1]=inc[0][1][1]->next(0);

  if (inc[1][0][0]) inc[1][0][0]->add(item,isdynamic);
  if (inc[0][1][0]) inc[0][1][0]->add(item,isdynamic);
  if (inc[0][0][1]) inc[0][0][1]->add(item,isdynamic);
  if (inc[1][1][0]) inc[1][1][0]->add(item,isdynamic);
  if (inc[1][0][1]) inc[1][0][1]->add(item,isdynamic);
  if (inc[0][1][1]) inc[0][1][1]->add(item,isdynamic);
  if (inc[1][1][1]) inc[1][1][1]->add(item,isdynamic);
  return act;
}

void OctTree::cleardynamics() {
  for(int i=0;i<dyns.count;i++) {
    dyns[i]->hasdynamic=false;
    for(int j=0;j<dyns[i]->ditems.count;j++) {
      destroy(dyns[i]->ditems[j].priv_id);
//      counters[dyns[i]->ditems[j].priv_id].destroyed=true;
    }
    dyns[i]->ditems.clear();
  }
  dyns.clear();
}

void otNode::remove(const otItem &item) {
  if (empty) return;
  otItem *w=items.buf;
  for(int i=0;i<items.count;i++) {
    if (items[i].obj==item.obj && items[i].id==item.id) {
      tree->destroy(items[i].priv_id);
//      tree->counters[items[i].priv_id].destroyed=true;
    } else {
      *w++=items[i];
    }
  }
  items.count=w-items.buf;
  #define child_op(child) if (child) child->remove(item);
  make_for_childs
  #undef child_op
  empty=items.count==0;
  #define child_op(child) else if (child && !child->empty) empty=false;
  if (!empty) ;
  make_for_childs
  #undef child_op
}
void otNode::removegroup(void *obj) {
  if (empty) return;
  otItem *w=items.buf;
  for(int i=0;i<items.count;i++) {
    if (items[i].obj==obj) {
      tree->destroy(items[i].priv_id);
//      tree->counters[items[i].priv_id].destroyed=true;
    } else {
      *w++=items[i];
    }
  }
  items.count=w-items.buf;
  #define child_op(child) if (child) child->removegroup(obj);
  make_for_childs
  #undef child_op
  empty=items.count==0;
  #define child_op(child) else if (child && !child->empty) empty=false;
  if (!empty) ;
  make_for_childs
  #undef child_op
}
void otNode::removegroup(otRemoveFunc f,void *data) {
  if (empty) return;
  otItem *w=items.buf;
  for(int i=0;i<items.count;i++) {
    if (f(items[i],data)) {
      tree->destroy(items[i].priv_id);
//      tree->counters[items[i].priv_id].destroyed=true;
    } else {
      *w++=items[i];
    }
  }
  items.count=w-items.buf;
  #define child_op(child) if (child) child->removegroup(f,data);
  make_for_childs
  #undef child_op
  empty=items.count==0;
  #define child_op(child) else if (child && !child->empty) empty=false;
  if (!empty) ;
  make_for_childs
  #undef child_op
}


void OctTree::remove(const otItem &item) {
  main->remove(item);
}
void OctTree::removegroup(void *obj) {
  main->removegroup(obj);
}
void OctTree::removegroup(otRemoveFunc f,void *data) {
  main->removegroup(f,data);
}

void otNode::updateempty() {
  empty=items.count==0;
  #define child_op(child) else if (child && !child->empty) empty=false;
  if (!empty) ;
  make_for_childs
  #undef child_op
  if (empty && (parent && !parent->empty)) parent->updateempty();
}


void otNode::doremove(const otItem &item) {
  otItem *w=items.buf;
  for(int i=0;i<items.count;i++) {
    if (items[i].obj==item.obj && items[i].id==item.id) {
      tree->destroy(items[i].priv_id);
    } else {
      *w++=items[i];
    }
  }
  items.count=w-items.buf;
  updateempty();
}
void otNode::quickremove(const otItem &item) {
  doremove(item);

  otNode *inc[2][2][2];//inc[0][0][0] nas nezajima (jakoby true)
  memset(inc,0,sizeof(inc));
  inc[1][0][0]=pnext[0];
  inc[0][1][0]=pnext[1];
  inc[0][0][1]=pnext[2];
  if (pnext[0] && pnext[1]) inc[1][1][0]=pnext[0]->pnext[1];
  if (pnext[0] && pnext[2]) inc[1][0][1]=pnext[0]->pnext[2];
  if (pnext[1] && pnext[2]) inc[0][1][1]=pnext[1]->pnext[2];
  if (pnext[0] && inc[0][1][1]) inc[1][1][1]=inc[0][1][1]->pnext[0];

  if (inc[1][0][0]) inc[1][0][0]->doremove(item);
  if (inc[0][1][0]) inc[0][1][0]->doremove(item);
  if (inc[0][0][1]) inc[0][0][1]->doremove(item);
  if (inc[1][1][0]) inc[1][1][0]->doremove(item);
  if (inc[1][0][1]) inc[1][0][1]->doremove(item);
  if (inc[0][1][1]) inc[0][1][1]->doremove(item);
  if (inc[1][1][1]) inc[1][1][1]->doremove(item);

}
void OctTree::quickremove(const otItem &item,otNode *place) {
  if (place) place->quickremove(item);
}


//static int visited=0;
//static int addtime=0;

static void _add_item(otCounter *counters,TCntVec<otItem> &res,otItem &item) {
  if (counters[item.priv_id].added) return;
  counters[item.priv_id].added=true;
  res.add(item);
//  if (res.search(item)<0) res.add(item);
}

void otNode::getvisible(TCntVec<otItem> &res,otClasifier &cl) {
  if (empty && !hasdynamic) return;
//  visited++;
  int state=cl.visiblecube(coords,size);

  if (state<0) return;

//  addtime+=clock()-start;
  if (state>0) {
    addall(res);
    return;
  }

  for(int i=0;i<items.count;i++) _add_item(tree->counters,res,items[i]);
  for(int i=0;i<ditems.count;i++) _add_item(tree->counters,res,ditems[i]);
  #define child_op(child) if (child) child->getvisible(res,cl);
  make_for_childs
  #undef child_op
}

void otNode::addall(TCntVec<otItem> &res) {
  for(int i=0;i<items.count;i++) _add_item(tree->counters,res,items[i]);
  for(int i=0;i<ditems.count;i++) _add_item(tree->counters,res,ditems[i]);
  #define child_op(child) if (child) child->addall(res);
  make_for_childs
  #undef child_op
}

void OctTree::getvisible(TCntVec<otItem> &res,otClasifier &cl) {
  for(int i=0;i<counterscapa;i++) counters[i].added=false;
//  visited=0;addtime=0;
//  int start=clock();
  main->getvisible(res,cl);
//  int len=clock()-start;
//  visited=0;
}

OctTree::OctTree() {
  main=NULL;minsize=1;
  counters=NULL;
  counterscapa=0;
  firstfreecounter=-1;
  memset(emptychilds,0,8*sizeof(otNode*));
}

void OctTree::destroy(int i) {
  if (counters[i].destroyed) return;
  counters[i].destroyed=true;
  counters[i].nextfree=firstfreecounter;
  firstfreecounter=i;
}

int OctTree::getfree() {
  retry:
  if (firstfreecounter>=0) {
    int res=firstfreecounter;
    counters[res].destroyed=false;
    firstfreecounter=counters[res].nextfree;
    return res;
  }
//  for(int i=0;i<counterscapa;i++) if (counters[i].destroyed) return i;
  int old=counterscapa;
  counterscapa=max(counterscapa*2,32);
  counters=(otCounter*)realloc(counters,counterscapa*sizeof(otCounter));
  for(int i=old;i<counterscapa;i++) {
    counters[i].destroyed=true;
    counters[i].nextfree=i+1;
  }
  counters[counterscapa-1].nextfree=-1;//uzemneni
  firstfreecounter=old;
  goto retry;
//  return getfree();
//  return old;
}

int ot_map_visible_cube(otProjector &prj,float coords[3],float size) {
  float A[3]={coords[0],coords[1],coords[2]};
  float B[3]={coords[0],coords[1],coords[2]+size};
  float C[3]={coords[0],coords[1]+size,coords[2]};
  float D[3]={coords[0]+size,coords[1],coords[2]};
  float E[3]={coords[0]+size,coords[1]+size,coords[2]};
  float F[3]={coords[0]+size,coords[1],coords[2]+size};
  float G[3]={coords[0],coords[1]+size,coords[2]+size};
  float H[3]={coords[0]+size,coords[1]+size,coords[2]+size};
  float a[2],b[2],c[2],d[2],e[2],f[2],g[2],h[2];
  prj.project(A,a);
  prj.project(B,b);
  prj.project(C,c);
  prj.project(D,d);
  prj.project(E,e);
  prj.project(F,f);
  prj.project(G,g);
  prj.project(H,h);
  if (a[0]<-1 && b[0]<-1 && c[0]<-1 && d[0]<-1 && e[0]<-1 && f[0]<-1 && g[0]<-1 && h[0]<-1) return -1;
  if (a[1]<-1 && b[1]<-1 && c[1]<-1 && d[1]<-1 && e[1]<-1 && f[1]<-1 && g[1]<-1 && h[1]<-1) return -1;
  if (a[0]>1 && b[0]>1 && c[0]>1 && d[0]>1 && e[0]>1 && f[0]>1 && g[0]>1 && h[0]>1) return -1;
  if (a[1]>1 && b[1]>1 && c[1]>1 && d[1]>1 && e[1]>1 && f[1]>1 && g[1]>1 && h[1]>1) return -1;

  if (
       a[0]>-1 && b[0]>-1 && c[0]>-1 && d[0]>-1 && e[0]>-1 && f[0]>-1 && g[0]>-1 && h[0]>-1
    && a[1]>-1 && b[1]>-1 && c[1]>-1 && d[1]>-1 && e[1]>-1 && f[1]>-1 && g[1]>-1 && h[1]>-1
    && a[0]<1 && b[0]<1 && c[0]<1 && d[0]<1 && e[0]<1 && f[0]<1 && g[0]<1 && h[0]<1
    && a[1]<1 && b[1]<1 && c[1]<1 && d[1]<1 && e[1]<1 && f[1]<1 && g[1]<1 && h[1]<1
  ) return 1;
  return 0;
}

/*
///abcdv je ctyrboky jehlan s vrcholem v
int ot_perspective_visible_cube(float a[3],float b[3],float c[3],float d[3],float v[3],float coords[3],float size) {
  return 0;
}
*/

}

