#ifndef w32keysH
#define w32keysH

#include <aws/awsdefs.h>

namespace wlib {
using namespace aws;

//int virtkeytokey(int virtkey,int lparam);//musi byt volano ze zpracovani WM_KEYDOWN

awsKey virtkeytokey(int virtkey,int lparam);//musi byt volano ze zpracovani WM_KEYDOWN

/*
int strtoshortcut(const char *s);
void shortcuttostr(int key,char buf[0x10]);
*/

}

#endif
