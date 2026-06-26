#ifndef HINFPOST
#define HINFPOST

#include <com/com0.h>
#include <com/strlist.h>
#include <com/arrays.h>

#define CHISOP 1
#define CHISALPHA 2
#define CHISNUM 4
#define CHISMAIN 8
#define CHISEND 8

#define OTBIN 'b'
#define OTPREFIX 'e'
#define OTPOSTFIX 't'

namespace jlib {

typedef int OPTYPE;//OTBIN,OTPREFIX,OTPOSTFIX

struct OPREC //struktura popisujici operator
{
  char name[16];//jmeno,nepr.: "==","and"...
  int pri;//priorita
  OPTYPE type;
  chrs getpriname() { return fmtstr("%c%s",type,name); }//jmeno pro postfixovy zapis
};
class OPS:public TCntVec<OPREC>
{
public:
  int findop(char *name,bool isprefix);
  void load(STRLIST &s);
  bool load(char *file);
};

inline OPREC makeoprec(char *aname,int apri,OPTYPE atype)
{
  OPREC r;strcpy(r.name,aname);r.pri=apri;r.type=atype;
  return r;
}
int inftopost(char *in,STRLIST &s,char chardefs[0x100],OPS &ops,char *error=NULL);
//prevede infix na postfix,in je vstupni retezec,s je vystup
//OUT:na jednom radku je,co se ma ulozit do zasobniku nebo co se ma provest
//(operator),rozpoznavaci je prvni znak

void fillchardefs(char chardefs[0x100],char *ops,char *mains,char *ends);//operatory ukoncene 0
//pouziva se k rozpoznani,ktery znak je operator,okoncovaci...
//pr.:ops="+-*/",mains="()",ends=";]"

#define ITOP 'o'
#define ITSTR 's'
#define ITINT 'n'
#define ITREAL 'r'
#define ITCHAR 'c'
#define ITIDF 'i'
#define ITFUNC 'f'

}

#endif
