#ifndef gbusstopH
#define gbusstopH

#include "game0.h"
#include "gstatic.h"
#include "groad.h"
#include "gmtcom.h"
#include "mdijskr.h"

namespace jtyc {
class jtBusStop;

struct BusStop:Station {
  bool vertical;
  RoadItem *ri;
  float z;

  BusStop(int x,int y,RoadItem *ri,bool ver,float z);
  void xmlsave(XMLNode xml);
  BusStop(XMLNode xml);
  void notifyremove(GInstance *obj);
  void destroy();
  void render(RenderContext &rc);
  void rendercaptions(RenderContext &rc);
  string gettype();
  void settype(string value);
  bool canremove();
  void dooctalize();
  void dodelize();
  void clearvisibility();
  void ot_render(int id);
  void created();
  MWayElem getstartelem();
private:
  jtBusStop *type;
  bool visible;
};


}

#endif
