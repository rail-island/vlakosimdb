#include <jlib.h>
#pragma hdrstop

#include "awsmodels.h"

namespace aws {

void ListModel::lm_update() {
  for(int i=0;i<lists.count;i++) lists[i]->changed_model();
}
void ListModel::lm_updatecur() {
  for(int i=0;i<lists.count;i++) lists[i]->changed_model_cur();
}
ListModel::~ListModel() {
  for(int i=0;i<lists.count;i++) lists[i]->deleted_model();
}
void ListImplementation::setmodel(ListModel *model) {
  if (list_model) list_model->deleted_list(this);
  list_model=model;
  if (list_model) list_model->add_list(this);
  changed_model();
}

void AbstractImplementation::setmodel(AbstractModel *model) {
  if (a_model) a_model->deleted_ctrl(this),a_model->release();
  a_model=model;
  if (a_model) a_model->add_ctrl(this),a_model->addref();
  changed_model();
}
AbstractModel::~AbstractModel() {
  for(int i=0;i<ctrls.count;i++) ctrls[i]->deleted_model();
}
void AbstractModel::model_update() {
  for(int i=0;i<ctrls.count;i++) ctrls[i]->changed_model();
}



}

