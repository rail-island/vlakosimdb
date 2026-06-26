#ifndef jarchiveH
#define jarchiveH

#include <ext/xmlnode.h>

struct ziplib_Archive;

namespace jlib {

struct ArchiveRec {
  string name;
  int size;
  ArchiveRec() {size=0;}
};

struct Archive {
  Archive();
  ~Archive();
  int count();//pocet polozek

  ///otevre pro cteni i zapis neexistujici
  void create(string file);
  ///otevre existujici, pokud neexistuje, vytvori
  void open(string file);
  ///otevre existujici jen pro cteni
  void openreadonly(string file);

  void close();

  int resetfile(string file);//otevre existujici soubor v archivu pro cteni, vrati jeho velikost
  void rewritefile(string file);//otevre existujici soubor v archivu pro cteni
  void closefile();//zavre otevreny soubor (pro cteni nebo pro zapis)
  void writedata(const void *buf,int size);
  //vrati, kolik dat precetlo
  int readdata(void *buf,int size);
  int findfile(string name);
  void unpackfile(string file,string path);
  void packfile(string file,string path);
  void erasefile(string file);

  string loadtext(string file);
  void savetext(string file,string text);

  void get(int index,ArchiveRec &data);

  void savexml(string file,XMLNode xml);
  void loadxml(string file,XMLNode xml);

  //count==NULL:nic,*count<0:do count se vrati velikost pole,*count>=0:zkontroluje se velikost pole
  template <class T> T *loadarray(string file,int *count=NULL);
  template <class T> void savearray(string file,T *array,int count);
private:
//  CZipArchive *ar;
  //specialni hodnota, pokud je otevreno jen pro cteni a je prazdne
//  bool readonlyclear;
  ziplib_Archive *ar;
  bool reading;
  bool writing;

  void closereadingfile();
  void closewritingfile();
};

template <class T> inline T *Archive::loadarray(string file,int *count) {
  resetfile(file);
  int cnt=0;
  readdata(&cnt,sizeof(cnt));
  T *res=new T[cnt];
  readdata(res,cnt*sizeof(T));
  closefile();
  if (count) {
    if (*count>=0) {
      if (*count!=cnt) throw JError("Bad size of array");
    } else *count=cnt;
  }
  return res;
}
template <class T> inline void Archive::savearray(string file,T *array,int count) {
  rewritefile(file);
  writedata(&count,sizeof(count));
  writedata(array,count*sizeof(T));
  closefile();
}


}

#endif
