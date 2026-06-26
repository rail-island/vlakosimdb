#ifndef awswintoolH
#define awswintoolH

namespace aws {

struct SDITool {
  string file;
  bool modified;

  SDITool() {modified=false;}

  void dosave() {
    if (file=="") dosaveas();
    else savefile(file);
  }
  virtual STRING getfilter()=0;
  bool dosaveas() {
    if (jsystem->getsavefn(getfilter(),basedir,file)) {
      dosave();return true;
    }
    return false;
  }
  void doload() {
    if (jsystem->getopenfn(getfilter(),basedir,file)) {
      loadfile(file);
      modified=false;
    }
  }
  void donew() {
    if (newfile()) file="";modified=false;
  }
  bool canexit() {
    return true;
  }
  virtual void savefile(STRING file)=0;
  virtual void loadfile(STRING file)=0;
  virtual bool newfile()=0;
};


}
#endif
 