#ifndef regexintfH
#define regexintfH

struct regexp;

namespace jlib {

struct StringOffset {
  int begin,end;
  StringOffset(int begin=0,int end=0):begin(begin),end(end) {}
  string get(string whole);
  string operator () (string whole) {return get(whole);}
  bool valid() {return begin>=0 && end>begin;}
  int length() {return end-begin;}
};

struct RegularExpr {
  void compile(STRING par_regex);
  void clear() {if (re) free(re);re=NULL;}
  bool run(string atext);
  RegularExpr() {re=NULL;}
  ~RegularExpr() {clear();}
  ///str():vrati nalezeny retezec, index>0:vrati uzavorkovany podretezec
  string str(int index=0) {return offset(index)(text);}
  StringOffset offset(int index=0);
  int subcount();//kolik podretezcu naslo
  ///replacestr je retezec s vyskyty \1,\2..., nahradi za nalezene podretezce
  string replace_with_subs(string replacestr);
  ///nahradi prvni vyskyt reg.vyrazu this retezcem replace_with
  string replace(string atext,string replace_with);
  ///nahradi vsechny vyskyty reg.vyrazu this retezcem replace_with
  string replaceall(string atext,string replace_with);
private:
  regexp *re;
  string text;
};

string makeexregex(string src);

}

#endif
