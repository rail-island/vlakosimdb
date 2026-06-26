#incldeu <winlib.h>
#pragma hdrstop

#include "w3derr.h"

namespace wlib {

string getwinerrormsg(int errcode) {
  LPVOID lpMsgBuf;
  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPTSTR) &lpMsgBuf,
    0,
    NULL
  );
  string res=(char*)lpMsgBuf;
  res.trimr();
  LocalFree( lpMsgBuf );
  return res;
}



}


