#ifndef zbdefsH
#define zbdefsH

namespace zaba {

typedef double number_t;

class Interpreter;
class Interface;
class Runtime;
class zbObject;

struct zbError {
  zbError(string msg):msg(msg) {}
  string getmsg() {return msg;}
private:
  string msg;
};

struct InstanceCreator:RCObject {
  virtual zbObject *createInstance(Runtime *rt,STRING intf)=0;
};

zbObject *createNumber(number_t num,Runtime *rt);
zbObject *createString(string s,Runtime *rt);
zbObject *createVoid(Runtime *rt);
zbObject *createLogical(bool value,Runtime *rt);
zbObject *createMultiIndex(TCntVec<int> &items,Runtime *rt);

struct InterpretException:JException {
  string msg;
  string getmsg() {return msg;}
  InterpretException(string msg):msg(msg) {}
};

}

#endif
