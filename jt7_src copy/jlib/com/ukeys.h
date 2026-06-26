#ifndef HUKEYS
#define HUKEYS

namespace jlib {

static void uchangesel(int key,int &act,int &fir,int viscnt,
  int count,int up,int down,int pgup,int pgdn, int home,int end)
{
    if (key==up) act--;
    if (key==down) act++;
    if (key==pgup) act-=viscnt;
    if (key==pgdn) act+=viscnt;
    if (key==home) act=0;
    if (key==end) act=count-1;

    if (act<0) act=0;
    if (act>=count) act=count-1;
    if (act<fir) fir=act;
    if (act-fir>=viscnt) fir=act-viscnt+1;
    if (fir<0) fir=0;
}

}

#endif
