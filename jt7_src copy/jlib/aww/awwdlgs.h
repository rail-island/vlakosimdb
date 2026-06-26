#ifndef awwdlgsH
#define awwdlgsH

#include "awwsystem.h"

namespace aww {

struct awwOpenDialog:OpenDialog {
  awwOpenDialog(awwSystem *system);
  string getfile() {return file;}
  void setfile(string value) {file=value;}
  bool getmultiselect() {return multiselect;}
  void setmultiselect(bool value) {multiselect=value;}
  bool execute();
  string getfiles() {return files;}
  string getinitialdir() {return initialdir;}
  void setinitialdir(string value) {initialdir=value;}
  string getfilter() {return filter;}
  void setfilter(string value) {filter=value;}
private:
  string file;
  string files;
  string filter;
  string initialdir;
  bool multiselect;
  awwSystem *system;
};

struct awwSaveDialog:SaveDialog {
  awwSaveDialog(awwSystem *system);
  string getfile() {return file;}
  void setfile(string value) {file=value;}
  bool execute();
  string getinitialdir() {return initialdir;}
  void setinitialdir(string value) {initialdir=value;}
  string getfilter() {return filter;}
  void setfilter(string value) {filter=value;}
private:
  awwSystem *system;
  string filter;
  string file;
  string initialdir;
};

struct awwColorDialog:ColorDialog {
  awwColorDialog(awwSystem *system);
  bool execute();
  int getcolor() {return color;}
  void setcolor(int value) {color=value;}
private:
  int color;
  awwSystem *system;
};


}

#endif
