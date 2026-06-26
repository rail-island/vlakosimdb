#ifndef gmanalH
#define gmanalH

namespace gram {

struct AnalError:JException {
  string msg;
  const char *pos;
  AnalError(string msg,const char *pos):msg(msg),pos(pos) {}
  string getmsg() {return msg;}
};

#pragma option push -w-
#pragma option push -vi

class Analyser;
struct CharSet {
  bool operator [](char c) {return yes[(unsigned char)c];}
  CharSet() {for(int i=0;i<0x100;i++) yes[i]=false;}
  void load(const char *c);
  void setinterval(int beg,int end,bool add) {for(int i=beg;i<=end;i++) yes[i]=add;}
  void setch(char ch,bool add) {yes[(unsigned char)ch]=add;}
  void stdload(const char *c);//c ve tvaru "abcd^e-z0-9\-" :-:interval,^:negace
  void setchs(const char *chs,bool add) {while (*chs) setch(*chs,add),chs++;}

  /**a konci ] nebo \0, -:interval,[ord]:znak, ^:prepinac negace
  pokud je jako prvni, berou se zpocatku vsechny znaky krome \0 
  */
  void regload(Analyser &a);
  void regload(STRING s);
  static char readchar(Analyser &a);
private:
  bool yes[0x100];
};
//load:eg:identifiers:#a..#z,#A..#Z,#0..#9,#_;whites:1..32,-#\n

struct Analyser {
  const char *act;
  void operator ++(int) {act++;}
  void operator --(int) {act--;}
//  void operator ++() {jmpwhite();}
//  void operator () () {jmpwhite();}
  void jcw();//jump C++ white chars(white chars+remarks)
  void jsp(char what=' ') {while (*act==what) act++;}
  char operator *() {return *act;}
  void operator +=(int num) {act+=num;}
  void operator -=(int num) {act-=num;}
//  Analyser operator [] (int index) {return Analyser(act+index);}
  explicit Analyser(const char *act=NULL):act(act) {}
  void operator =(const char *s) {act=s;}
  operator const char * () {return act;}
  bool jumpafter(const char *end);
  bool begins(const char *beg) {return strbegcmp(beg,act);}
  bool begjmp(const char *beg) {if (begins(beg)) {act+=strlen(beg);return true;}return false;}
  bool jmpidf(const char *beg) {int len=strlen(beg);if (begins(beg) && ! is_idf(act[len])) {act+=len;return true;}return false;}
  bool seekch(char ch='\n');
  void f_jump(const char *beg) {if (!begjmp(beg)) error(fmtstr("'%s' expected",beg));}
  int readint();
  int readto(char *buf,int size,const char *ends,bool jumplast);//vrati, na kolikaty z ends narazilo,
  //-1 pri konci vstupu, -2 pri malem bufferu
  string readto(const char *ends,bool jumplast);
  void readto(const char *ends,STROUT &fw,bool jumplast);
  void jmpwhite() {while (is_white(*act)) act++;}
  void jw() {jmpwhite();}
  bool readidf(char *buf,int bufsize=-1);
  string readidf();
  void f_readidf(char *buf,int bufsize=-1) {if (! readidf(buf,bufsize) ) error("Identifier expected");}
  bool isbody(char end) {return *act && *act!=end;}
  void f_char(char c) {if (*act!=c) error(fmtstr("%c expected",c));act++;}
  void readexpr(char *buf,int bufsize,char beg,char end);
  //vstup:prvni musi byt preskocena, posledni preskoci

  void roll(char end='\n') {while (*act && *act!=end) act++;if (*act) act++;}
  void roll(STROUT &fw,char end='\n') {while (*act && *act!=end) fw<<*act++;if (*act) act++;}
  void readline(char *buf,char end='\n');//+trimr
  void jumpchar(char end) {while (*act && *act!=end) act++;if (*act) act++;}
  void readcstring(char *buf,char end='"');//lisi se od readline, ze za end nepocita \z
  //bere v uvahu \n
  string readcstring(char end='"');
  void jumpcstring(char end='"');
  double readdouble(char *buf=NULL);

  void error(string msg="");

  bool is_digit() {return is_digit(*act);}
  bool is_alpha() {return is_alpha(*act);}
  bool is_alnum() {return is_alnum(*act);}
  bool is_idfbeg() {return is_idfbeg(*act);}
  bool is_idf() {return is_idf(*act);}
  bool is_white() {return is_white(*act);}
  bool is_xmlspecchar() {return is_xmlspecchar(*act);}
  bool is_xmlidf() {return is_xmlidf(*act);}
  bool is_xmlloadspec() {return is_xmlloadspec(*act);}

  static bool is_white(char c) {return whites[c];}// return (c && ((unsigned char)c)<=' ');}
  static bool is_digit(char c) {return digits[c];}//return c>='0' && c<='9';}
  static bool is_alpha(char c) {return alphas[c];}//return (c>='a' && c<='z') || (c>='A' && c<='Z');}
  static bool is_alnum(char c) {return digits[c];}//return isdigit(c) || isalpha(c);}
  static bool is_idfbeg(char c) {return idfbegs[c];}//return isalpha(c) || c=='_';}
  static bool is_idf(char c) {return idfs[c];}//return isdigit(c) || isidfbeg(c);}
  static bool is_xmlspecchar(char c) {return xmlspecs[c];}// " ' & < >
  static bool is_xmlidf(char c) {return xmlidfs[c];}
  static bool is_xmlloadspec(char c) {return xmlloadspecs[c];}// < > \0 &

  static void static_init();
private:
  static CharSet whites,alphas,digits,alnums,idfbegs,idfs,xmlspecs,xmlidfs,xmlloadspecs;
};

#pragma option pop
#pragma option pop

}

#endif
