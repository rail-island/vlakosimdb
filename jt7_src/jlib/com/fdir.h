#ifndef HDIR
#define HDIR

#include <com/com0.h>

namespace jlib {


class STRLIST;

class TMPDIR
{
public:
  char tmpdir[L_tmpnam],tmppth[L_tmpnam+1];
  TMPDIR();
  ~TMPDIR();
  void readfiles(STRLIST &files,bool addpath=true);
  void add(STRLIST &buf,char *name);
  void addtozip(char *zipfile);
};

chrs extractfilename(char *file);
void extractfilename(char *file,char *outbuf);

void makepath(const char *dir,const char *file,const char *ext,char *path);
//makepath +dir,+file,+ext,-path-vytvori platnou cestu
void splitpath(const char *path,char *dir,char *file,char *ext);
//splitpath +path,-dir,-file,-ext
//rozdeli cestu,dir a ext muze byt NULL(pak se prida k file)
char *binloadstring(char *file);
void binsavestring(char *buf,char *file);
char *lastslash(char *in);
//vrati posledni dopredne nebo dozedne lomitko
char *floadstring(char *file);
void fsavestring(char *buf,char *file);

void tmpinit();
void gentmpdir(char *dir);
//vraci adresar s lomitkem nakonec
void gentmpfile(char *file);
void adddosbs(char *path);
//prida na konec dosslash,pokud tam neni '\\' nebo '/'

extern const char dosslash;
extern char deftmpdir[0x200];

}

#endif
