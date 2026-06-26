#ifndef convutilsH
#define convutilsH

namespace jlib {


int anytoi(char *&c,char beg,char end);//beg je prvni pismeno abecedy,end posledni(pro DECIMAl beg==0,end==9)
//precte toto cislo, pokud je tam '-',crati -1

inline int dectoi(char *&c) {return anytoi(c,'0','9');}
inline int abctoi(char *&c) {return anytoi(c,'a','z');}
inline int ABCtoi(char *&c) {return anytoi(c,'A','Z');}

inline int aBtoi(char *&c) {if (*c>='a' && *c<='z') return abctoi(c);return ABCtoi(c);}

void itoany(char *&w,int num,char beg,char end);//opacna konverze

inline void itodec(char *&w,int num) {itoany(w,num,'0','9');}
inline void itoabc(char *&w,int num) {itoany(w,num,'a','z');}
inline void itoABC(char *&w,int num) {itoany(w,num,'A','Z');}

inline void itoaB(char *&w,int num,bool big) {if (big) itoABC(w,num);else itoabc(w,num);} 

}

#endif
