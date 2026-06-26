#ifndef atrajectoryH
#define atrajectoryH

#include <jmath/geom3d.h>

namespace jtyc {
using namespace jmath;

struct AbstractTrajectory {
  virtual fpoint3d at_count_pos(float dt)=0;
};

struct TrajectoryTranslation:AbstractTrajectory {
  AbstractTrajectory *traj;
  float dt;
  TrajectoryTranslation(AbstractTrajectory &traj,float dt):traj(&traj),dt(dt) {}
  fpoint3d at_count_pos(float dt) {return traj->at_count_pos(dt+this->dt);}
};


}

#endif
