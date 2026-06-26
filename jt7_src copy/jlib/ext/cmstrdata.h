#ifndef cmstrdataH
#define cmstrdataH

namespace jlib {

//String array-uklada pole retezecu do retezce
struct SAERROR:JException {
  string getmsg() {return msg;}
  string msg;SAERROR(string amsg) {msg=amsg;}
};

string createSA(Strings &items);
void decodeSA(string array,Strings &out);
int SAcount(string array);
string SAitem(string array,int index);
void checkSA(string array) throw(SAERROR);
bool isSA(string array);


/*
struct SRecord {
  string s;
  SRecord(string s):s(s) {}
  SRecord() {s="#[Record]#";}
  string getprop(string what);
  void setprop(string prop,string value);
//  void add(SRecord right);
};
*/

string SRsetprop(string obj,string prop,string value);
string SRgetprop(string obj,string prop);
string SRcreate(VarList &vars);
void SRdecode(string obj,VarList &vars);

}

#endif
