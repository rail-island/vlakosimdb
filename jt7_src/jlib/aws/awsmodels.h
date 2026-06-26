#ifndef awsmodelsH
#define awsmodelsH

namespace aws {

class AbstractModel;

struct AbstractImplementation {
  AbstractImplementation() {a_model=NULL;}
  void deleted_model() {a_model=NULL;changed_model();}
  virtual void changed_model() {}
  ~AbstractImplementation() {setmodel(NULL);}
  void setmodel(AbstractModel *model);
  AbstractModel *getmodel() {return a_model;}
private:
  AbstractModel *a_model;
};

struct AbstractModel {
  TCntVec<AbstractImplementation*> ctrls;
  void model_update();
  void deleted_ctrl(AbstractImplementation *list) {ctrls.quickremove(list);}
  void add_ctrl(AbstractImplementation *list) {ctrls.add(list);}
  virtual ~AbstractModel();
  virtual void addref() {}
  virtual void release() {}
};


class ListModel;

struct ListImplementation {
  ListImplementation() {list_model=NULL;}
  void deleted_model() {list_model=NULL;changed_model();}
  virtual void changed_model() {}
  virtual void changed_model_cur() {}
  ~ListImplementation() {setmodel(NULL);}
  void setmodel(ListModel *model);
  ListModel *getmodel() {return list_model;}
private:
  ListModel *list_model;
};

struct ListModel {
  TCntVec<ListImplementation*> lists;
  virtual int lm_count()=0;
  virtual string lm_item(int index)=0;
  virtual bool lm_iscur() {return false;}
  virtual int lm_getcur() {return -1;}
  virtual void lm_setcur(int index) {}
  void lm_update();
  void lm_updatecur();
  void deleted_list(ListImplementation *list) {lists.quickremove(list);}
  void add_list(ListImplementation *list) {lists.add(list);}
  ~ListModel();
};


class LabelModel;

struct LabelImplementation:AbstractImplementation {
  virtual void changed_model() {}
  LabelModel *getmodel() {return (LabelModel*)AbstractImplementation::getmodel();}
};

struct LabelModel:AbstractModel {
  virtual string label_gettext(STRING model) {return "";}
};


}

#endif
