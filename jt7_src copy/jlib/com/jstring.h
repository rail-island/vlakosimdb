#ifndef jstringH
#define jstringH

//#define STRINGCONTROL

//#define STRINGDESTROYCONTROL

#ifndef STRINGCONTROL
#pragma option push -w-
#pragma option push -vi
#endif

namespace jlib {

#ifdef STRINGCONTROL
#define _ctrl_str do_string_control()
#define _ctrl_other_str(_any_) _any_.do_string_control()
#else
#define _ctrl_str
#define _ctrl_other_str(_any_)
#endif

class string;
struct STRING {
  const char *s;
  bool operator ==(const char *s2) const {return !strcmp(s,s2);}
  bool operator !=(const char *s2) const {return strcmp(s,s2);}
  bool operator ==(STRING s2) const {return !strcmp(s,s2);}
  bool operator !=(STRING s2) const {return strcmp(s,s2);}
  bool operator ==(const string &s2) const;
  bool operator !=(const string &s2) const;
  operator const char *() const {return s;}
  STRING(const char *str) {s=str;}
  const char *c_str() const {return s;}
  char operator [] (int index) const {return c_str()[index];}
};

template<int SIZE> struct TString {
  char buf[SIZE+1];
  bool operator ==(STRING s2) const {return !strcmp(buf,s2);}
  bool operator !=(STRING s2) const {return strcmp(buf,s2);}
  operator const char *() const {return buf;}
  operator STRING () const {return STRING(buf);}
  const char *c_str() const {return buf;}
  void operator =(const char *src) {strcpy(buf,src);}
  TString() {*buf=0;}
};

#ifdef STRINGCONTROL
extern int dbg_string_count;
const int STRINGMAGIC=538435;
#endif

struct stringrecord {
  int len,capa,refcnt;
  //capa znamena,jak dlouhy retezec se do buf vejde
  #ifdef STRINGCONTROL
  int magic;int len2;
  #endif
  char *gets() {return reinterpret_cast<char*>(this+1);}
  void release() {
    refcnt--;if (refcnt==0) {
      #ifdef STRINGCONTROL
      dbg_string_count--;
      #endif
      free(this);
    }
  }
  void init(int alen,int acapa) {
    len=alen;capa=acapa;refcnt=1;
    #ifdef STRINGCONTROL
    len2=len;
    dbg_string_count++;
    magic=STRINGMAGIC;
    #endif
  }
};


class string;

struct lineiter {
  const char * operator *() {return aredata()?data:NULL;}
  bool aredata() {return wasdata;}
  void operator ++(int) {readnext();}
  ~lineiter() {if (rec) rec->release();}
private:
  void readnext() {
    wasdata=rec && *act;
    if (rec==NULL) return;
    char *w=data;
    int size=MAX;
    while (*act && *act!=separator && size) *w++=*act++,size--;
    if (*act==separator) act++;
    *w=0;
  }
  friend class string;
  static const int MAX=0xFFF;
  char data[MAX+1];
  stringrecord *rec;
  char *act;
  char separator;
  lineiter(string *src,char aseparator);
  bool wasdata;
};

#define string_class_defined

struct string {
  #ifdef STRINGCONTROL
  void do_string_control();
  #endif
  string() {buf=NULL;_ctrl_str;}
  const char *c_str() const {_ctrl_str;return buf?buf:"";}
  void clear();
/*  char *buffer() {
    unique();
    _ctrl_str;
    if (buf==NULL) initbuf("");
    return buf;
  }*/
  void unique() {
    _ctrl_str;
    if (buf) {
      REC *r=getrec();
      if (r->refcnt>1) {
        r->refcnt--;
        initbuf(r->gets());
      }
    }
  }
  int search(char c) {const char *pos=strchr(c_str(),c);return pos?pos-c_str():-1;}
  int search(char c,int begin) {const char *pos=strchr(c_str()+begin,c);return pos?pos-c_str()+begin:-1;}
  int searchstr(STRING s) {const char *pos=strstr(c_str(),s);return pos?pos-c_str():-1;}
  int searchstr(STRING s,int begin) {const char *pos=strstr(c_str()+begin,s);return pos?pos-c_str()+begin:-1;}
//  string copy(int index,int count);
  int ivalue() {return atoi(c_str());}
  double fvalue() {return atof(c_str());}
  bool replace(STRING oldstr,STRING newstr);
  int replaceall(STRING oldstr,STRING newstr);
  string stringat(int index,char separator);//vyuziva lineiter
  int itemcount(char separator);
  int searchinlines(const char *str,char separator);
  #ifdef STRINGCONTROL
  ~string();
  #else
  #ifdef STRINGDESTROYCONTROL
  ~string();
  #else
  ~string() {_ctrl_str;release();}
  #endif
  #endif
  void operator +=(char *str) {add(str);}
  void operator +=(string str) {add(str);}
  void operator +=(char ch) {add(ch);}
  int length() {_ctrl_str;return buf?getrec()->len:0;}
  void trimr();
  string gettrimed();
  string(const string &src) {_ctrl_other_str(src);buf=src.buf;if (buf) getrec()->refcnt++;}
  string(const char *src) {initbuf(src);_ctrl_str;}
  string(STRING s) {initbuf(s);_ctrl_str;}
  string(const unsigned char *src) {initbuf((char*)src);}
  operator STRING () {return STRING(c_str());}
  explicit string(char ch) {char s[2];s[0]=ch;s[1]=0;initbuf(s);}
  char operator [] (int index) const {return c_str()[index];}
  void operator =(const string &src) {
    _ctrl_str;_ctrl_other_str(src);
    if (buf) getrec()->release();
    buf=src.buf;
    if (buf) getrec()->refcnt++;
  }
//  void _init_size(int len) {initbuf(len);}//alokuje retezec s takovouto delkou
  //retezec musi byt prace zkostruovany
  bool match(string s2) {return strcmp(c_str(),s2.c_str())==0;}
  string & operator <<(const char *str) {add(str);return *this;}
  string & operator <<(char ch) {add(ch);return *this;}
  string & operator <<(string str) {add(str);return *this;}
  string & operator <<(int i) {char s[0x100];std::sprintf(s,"%d",i);add(s);return *this;}

  void operator /=(const char *str) {add(str);add('\n');}
  void operator /=(string str) {add(str);add('\n');}
  int linecnt() {int res=0;if (buf) for(char *c=buf;*c;c++) if (*c=='\n') res++;return res;}
  bool begins(string s) {return begins(s.c_str());}
  bool begins(const char *s);
  bool ends(string s) {return ends(s.c_str());}
  bool ends(const char *s);
  void sprintf(const char *format,...);
  void vsprintf(const char *format,va_list args);
  void insert(int pos,string what);
  void erase(int pos,int count);
  string copy(int pos,int count) {return string(buf,length(),pos,count);}
  void updatelen();
  char *reservebuffer(int len);
  char *reservebuffer(const char *src);
  void setlen(int newlen);//muze jen zmensit delku
  lineiter getlines(char separator='\n') {return lineiter(this,separator);}
  void makelower();
  void makeupper();
  string getlower() {string res(*this);res.makelower();return res;}
  string getupper() {string res(*this);res.makeupper();return res;}
private:
  char *buf;
  typedef stringrecord REC;
  friend class REC;friend class lineiter;
  static const int MINJMPCAPA=0x10;
  static const int SPRINTF_BUF_SIZE=0x1000;
  static int newcapa(int wanted) {int max1=(wanted*3)/2,max2=wanted+MINJMPCAPA;return max1>max2?max1:max2;}
  string(const char *src,int srclen,int pos,int count);
  void release() {_ctrl_str;if (buf) getrec()->release();}
  REC * wantcapa(int size);
  REC *getrec() {return reinterpret_cast<REC*>(buf)-1;}
  void initwithstr(char *s);
  REC *initbuf(int len) {return initbuf(len,len);}
  REC *initbuf(int capa,int len) {
    int x=newcapa(capa);
    REC *rec=(REC*)malloc(x+sizeof(REC)+1);
    rec->init(len,x);
    buf=rec->gets();
    *buf=0;
    return rec;
  }
  void initbuf(const char *s) {
    int len=strlen(s);
    REC *r=initbuf(len);
    strcpy(buf,s);
//    r->len=len;
    _ctrl_str;
  }
  string(string left,string right) {
    int l1=left.length(),l2=right.length();
    initbuf(l1+l2);
    strcpy(buf,left.c_str());
    strcpy(buf+l1,right.c_str());
    _ctrl_str;
  }
  void add(char ch);
  void add(const char *str);
  void add(const string &str);
  friend string operator +(string,string);
};

inline string operator +(string left,string right) {return string(left,right);}
inline bool operator ==(string left,string right){_ctrl_other_str(left);_ctrl_other_str(right); return strcmp(left.c_str(),right.c_str())==0;}
inline bool operator !=(string left,string right){_ctrl_other_str(left);_ctrl_other_str(right); return strcmp(left.c_str(),right.c_str());}
inline bool operator ==(string left,const char *right){_ctrl_other_str(left); return strcmp(left.c_str(),right)==0;}
inline bool operator !=(string left,const char *right){_ctrl_other_str(left); return strcmp(left.c_str(),right);}
inline bool operator ==(string left,STRING right){_ctrl_other_str(left); return strcmp(left.c_str(),right)==0;}
inline bool operator !=(string left,STRING right){_ctrl_other_str(left); return strcmp(left.c_str(),right);}
inline bool STRING::operator ==(const string &s2) const {return !strcmp(s,s2.c_str());}
inline bool STRING::operator !=(const string &s2) const {return strcmp(s,s2.c_str());}

inline lineiter::lineiter(string *src,char aseparator) {
  if (src->buf) {
    separator=aseparator;
    rec=src->getrec();
    rec->refcnt++;
    act=rec->gets();
    readnext();
  } else rec=NULL,act=NULL,*data=0,wasdata=false;
}

extern string emptystring;

template<int SIZE> struct TBufStringWriter {
  string value;char buf[SIZE+2];int pos;
  TBufStringWriter() {pos=0;}
  TBufStringWriter<SIZE> &operator <<(char ch) {
    buf[pos++]=ch;
    if (pos>=SIZE) flush();
    return *this;
  }
  TBufStringWriter<SIZE> &operator <<(string s) {
    flush();
    value<<s;
    return *this;
  }
  void flush() {
    buf[pos]=0;
    value<<buf;
    pos=0;
  }
  void clear() {pos=0;value="";}
  operator string () {flush();return value;}
};

typedef TBufStringWriter<128> bstring;


}

#ifndef STRINGCONTROL
#pragma option pop
#pragma option pop
#endif

#endif


