#ifndef strutilsH
#define strutilsH

namespace jlib {

namespace str {

bool readto(const char *&c,char *buf,char end,int bufsize=-1);//precte c do buf,nepreskakuje nijak mezery,
//vraci,zda bylo dosazeno end,bufsize-velikost buf,<0:neomezena

bool readto(char *&c,char **buf,char end);//alokuje do buf

}

struct CHARANAL {
  char *act;
  void readtonojmp(char ch) {while (*act and *act!=ch) act++;}
  CHARANAL &operator <<(char ch) {readtonojmp(ch);if (*act==ch) act++;return *this;}
  void jumpoverfirst(const char *s);//najde prvni vyskyt s,skoci hned za nej nebo
  //na konec,pokud tam s neni
};

}

#endif
