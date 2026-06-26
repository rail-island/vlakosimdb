#ifndef langmanH
#define langmanH

namespace jlib {

struct LanguageManager {
  string get(string name);
  string get_upper(string name);
  string getdef(string name);
  void loadfromtext(sFile dir,STRING text);
  void loadfromfile(string file);
  string mapmodify(string str);

  VarList items;
  string charmap;
  string langid;
};

extern LanguageManager *langman;

string lang_text(string msg);
string lang_text_ch(string msg,char spec='$');

void init_lang();
void done_lang();
void load_lang_file(string file);

/*
inline string lang_text(string msg) {return msg;}
inline string lang_text_ch(string msg,char spec='$') {return msg;}

inline void init_lang() {}
inline void done_lang() {}
inline void load_lang_file(string file) {}
*/

}

#endif
