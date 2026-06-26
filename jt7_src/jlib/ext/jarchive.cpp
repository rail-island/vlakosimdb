#include <jlib.h>
#pragma hdrstop

//#include <zip/ziparchive.h>
#include "jarchive.h"
#include <zlib/zlib.h>
#include <ziplib/ziplib.h>

namespace jlib {


Archive::Archive() {
  ar=NULL;
  reading=writing=false;
//  readonlyclear=false;
}
Archive::~Archive() {
  close();
}
void Archive::openreadonly(string file) {
/*  FILE *fr=fopen(file.c_str(),"rb");
  if (!fr) create(file);
  else {
    fclose(fr);
    close();
    ar=ziplib_Open(file.c_str(),ziplib_OPEN_R);
//    ar=new CZipArchive();
//    ar->Open(file.c_str(),CZipArchive::zipOpenReadOnly);
  }
  */
  close();
  ar=ziplib_Open(file.c_str(),ziplib_OPEN_R);
  if (!ar) {
    throw JError("Cannot open archive:"+file);
  }
//  if (!ar) readonlyclear=true;
}
void Archive::open(string file) {
  close();
  ar=ziplib_Open(file.c_str(),ziplib_OPEN_RW);
/*
FILE *fr=fopen(file.c_str(),"rb");
  if (!fr) create(file);
  else {
    fclose(fr);
    close();
    ar=new CZipArchive();
    ar->Open(file.c_str());
  }
  */
}
void Archive::create(string file) {
  close();
  remove(file.c_str());
  ar=ziplib_Open(file.c_str(),ziplib_OPEN_RW);
//  ar=new CZipArchive();
//  ar->Open(file.c_str(),CZipArchive::zipCreate);
}
void Archive::close() {
//  readonlyclear=false;
  if (!ar) return;
  closereadingfile();closewritingfile();
  ziplib_Close(ar);
//  ar->Close();
//  delete ar;
  ar=NULL;
}

void Archive::closereadingfile() {
  if (!reading) return;
  ziplib_CloseFile(ar);
//  ar->CloseFile();
  reading=false;
}

void Archive::closewritingfile() {
  if (!writing) return;
  ziplib_CloseFile(ar);
//  ar->CloseNewFile();
  writing=false;
}

void Archive::closefile() {
  if (reading) closereadingfile();
  else if (writing) closewritingfile();
  else throw JError("Archive::closefile():no file currctly open");
}

bool samefile(const char *f1,const char *f2) {
  while (*f1 && *f2) {
    if (*f1=='/' || *f1=='\\') {
      if (*f2!='/' && *f2!='\\') return false;
    }
    if (toupper(*f1)!=toupper(*f2)) return false;
    f1++,f2++;
  }
  return !*f1 && !*f2;
}

int Archive::findfile(string name) {
  if (!ar) return -1;
//  if (readonlyclear) return -1;
//  if (!ar) throw JError("Archive::rewrite():archive not opened");
  int cnt=count();
  for(int i=0;i<cnt;i++) {
    ziplib_File hdr;
    ziplib_Get(ar,i,&hdr);
//    CZipFileHeader ch;
//    ar->GetFileInfo(ch,i);
//    if (samefile(name.c_str(), ch.GetFileName())) return i;
    if (samefile(name.c_str(),hdr.name)) return i;
  }
  return -1;
//  return ar->FindFile(name.c_str());
}

int Archive::resetfile(string file) {
  if (reading || writing) throw JError("Archive::rewrite():file currectly open");
  if (!ar)
    throw JError("Archive::rewrite():archive not opened");
  int index=findfile(file);
  if (index<0) throw JError("Archive::resetfile():file not found in archive:"+file);
  ziplib_File hdr;
  ziplib_Get(ar,index,&hdr);
//  CZipFileHeader ch;
//  ar->GetFileInfo(ch,index);
//  ar->OpenFile(index);
  ziplib_OpenFile(ar,index,0);
  reading=true;
  return hdr.size;
//  return ch.m_uUncomprSize;
}

void Archive::rewritefile(string file) {
  if (reading || writing) throw JError("Archive::rewrite():file currectly open");
  if (!ar) throw JError("Archive::rewrite():archive not opened");

  int idx=findfile(file);                         
  if (idx>=0) ziplib_RemoveFile(ar,idx);
//   ar->DeleteFile(idx);

/*  CZipFileHeader ch;
  zerorec(ch);
  ch.SetFileName(file.c_str());
  ch.m_uMethod=1;
  ar->OpenNewFile(ch,-1);*/
  ziplib_OpenNewFile(ar,file.c_str(),-1);
  writing=true;
}

int Archive::count() {
  if (!ar) return 0;
  return ziplib_Count(ar);
//  return ar->GetCount();
}

void Archive::writedata(const void *buf,int size) {
  if (!writing) throw JError("Archive::writedata():no file currctly open for writing");
  ziplib_Write(ar,buf,size);
//  ar->WriteNewFile(buf,size);
}

int Archive::readdata(void *buf,int size) {
  if (!reading) throw JError("Archive::readdata():no file currctly open for reading");
  return ziplib_Read(ar,buf,size);
//  return ar->ReadFile(buf,size);
}

void Archive::get(int index,ArchiveRec &data) {
  if (index<0 || index>=count()) throw JError("Archive::get():index out of bounds");

  ziplib_File hdr;
  ziplib_Get(ar,index,&hdr);

  data.name=hdr.name;
  data.size=hdr.size;
/*
  CZipFileHeader ch;
  ar->GetFileInfo(ch,index);
  data.name=ch.GetFileName().c_str();
  data.size=ch.m_uUncomprSize;
*/
}

string Archive::loadtext(string file) {
  int size=resetfile(file);
  string res;
  char *buf=res.reservebuffer(size+1);
  readdata(buf,size);
  buf[size]=0;
  res.updatelen();
  closefile();
  return res;
}
void Archive::savetext(string file,string text) {
  rewritefile(file);
  writedata(text.c_str(),text.length());
  closefile();
}
void Archive::unpackfile(string file,string path) {
  char buffer[0x1000];
  FILE *fw=fopen(path.c_str(),"wb");
  if (!fw)
    throw JError("Cannot open file:"+path);
  int size=resetfile(file);
  while (size>0) {
    int r=readdata(buffer,sizeof(buffer));
    if (r<=0) break;
    size-=r;
    fwrite(buffer,1,r,fw);
  }
  fclose(fw);
  closefile();
}
void Archive::packfile(string file,string path) {
  char buffer[0x1000];
  FILE *fr=fopen(path.c_str(),"rb");
  if (!fr) throw JError("Cannot open file:"+path);
  rewritefile(file);
  while (true) {
    int r=fread(buffer,1,sizeof(buffer),fr);
    if (r<=0) break;
    writedata(buffer,r);
  }
  fclose(fr);
  closefile();
}
void Archive::erasefile(string file) {
  if (!ar) return;
  int index=findfile(file);
  if (index<0) throw JError("Archive::erasefile():file not found in archive:"+file);
  ziplib_RemoveFile(ar,index);
//  ar->DeleteFile(index);
}



void Archive::savexml(string file,XMLNode xml) {
  MEMSTROUT mso;
  xml.savetotext(mso);
  savetext(file,mso.str());
}

void Archive::loadxml(string file,XMLNode xml) {
  string text=loadtext(file);
  xml.loadfromtext(text);
}


}
/*

  CZipArchive zip;
  zip.Open(file);
  int cnt=zip.GetCount();
  printf("%d\n",cnt);
  for(int i=0;i<cnt;i++) {
    CZipFileHeader ch;
    zip.GetFileInfo(ch,i);
    zip.OpenFile(i);
//    zip.ReadFile(
    printf("%s size=%d\n",ch.GetFileName().c_str(),ch.m_uUncomprSize);
    char *buf=new char[ch.m_uUncomprSize+1];
    buf[ch.m_uUncomprSize]=0;
    int r=zip.ReadFile(buf,ch.m_uUncomprSize);
    puts(buf);
    delete buf;
    zip.CloseFile();
  }
  zip.Close();

  strcpy(l+1,"pok2.zip");
  CZipArchive zip2;
  zip2.Open(file,CZipArchive::zipCreate);

  CZipFileHeader ch;
  for(char *c=(char*)&ch;c<(char*)&ch+sizeof(ch);c++) *c=0;
  ch.SetFileName("pokus2");
//  ch.set

//  zip2.OpenNewFile(ch,-1,"pokus2");
  zip2.OpenNewFile(ch);
  zip2.WriteNewFile("pokus2_data",8);
  zip2.CloseNewFile();
  zip2.Close();
*/
