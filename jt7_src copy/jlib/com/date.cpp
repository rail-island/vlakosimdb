#include <jlib.h>
#pragma hdrstop

#include "date.h"

namespace jlib {


JDATE::JDATE(int day,int month,int year) {
  tm t;
  t.tm_sec=0;
  t.tm_min=0;
  t.tm_hour=0;
  t.tm_mday=day;
  t.tm_mon=month-1;
  t.tm_year=year-1900;
  t.tm_isdst=-1;
  secs=mktime(&t);
}

char *JDATE::c_str() {
  long se=secs;
  tm t=*localtime(&se);
  sprintf(s,"%d.%d.%d",t.tm_mday,t.tm_mon+1,t.tm_year+1900);
  return s;
}

}

#pragma package(smart_init)
