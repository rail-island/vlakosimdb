/*
 * This source file is part of the ziplib library
 * Author:Jan Prochazka, zputil@centrum.cz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 */


#include "zl_types.h"
#include "ziplib.h"

#include <io.h>

int cd_buffersize=32768;
char CentralDir_gszSignature[]={0x50, 0x4b, 0x05, 0x06};

char FileHeader_gszSignature[] = {0x50, 0x4b, 0x01, 0x02};
char FileHeader_gszLocalSignature[] = {0x50, 0x4b, 0x03, 0x04};


int ziplib_read(Archive *ar,void *buf,int bytes) {
  return fread(buf,1,bytes,ar->zipfile);
}
int ziplib_write(Archive *ar,const void *buf,int bytes) {
  return fwrite(buf,1,bytes,ar->zipfile);
}
int ziplib_tell(Archive *ar) {
  return ftell(ar->zipfile);
}
int ziplib_seek(Archive *ar,int pos,int whence) {
  return fseek(ar->zipfile,pos,whence);
}
int ziplib_truncate(Archive *ar,int filesize) {
  int handle=fileno(ar->zipfile);
  return chsize(handle,filesize);
}
int ziplib_seek_end(Archive *ar) {
  return ziplib_seek(ar,ar->m_uOffset,SEEK_SET);
/*  if (ar->m_uEntriesNumber>0) {
    FileHeader *hdr=&ar->headers[ar->m_uEntriesNumber-1];
    return ziplib_seek(ar,hdr->m_uOffset+hdr->m_uComprSize+hdr->t_LocalHeaderSize,SEEK_SET);
  }
  else return ziplib_seek(ar,0,SEEK_SET);*/
}
void *ziplib_malloc_0(int size) {
  void *res=malloc(size);
  if (res) memset(res,0,size);
  return res;
}

int ziplib_valid_index(Archive* ar,int index) {
  if (!ar) ZL_ERROR(BAD_FILE_INDEX)
  if (index<0 || index>=ar->m_uEntriesNumber) ZL_ERROR(BAD_FILE_INDEX)
  return 0;
}

int ziplib_free_archive(Archive *ar) {
  int i;

  for(i=0;i<ar->m_uEntriesNumber;i++) ziplib_free_header(&ar->headers[i]);
  free(ar->headers);
  free(ar->info.m_pBuffer);
  free(ar);

  return 0;
}

int ziplib_close_file(Archive *ar) {
  if (ar->opened) {
    if (ar->m_iFileOpened<0) return ziplib_close_read_file(ar);
    else ziplib_close_new_file(ar);
  }
  ZL_ERROR(BAD_OPENED_FILE)
}

/*
int ziplib_index_ltop(Archive *ar,int index) {
  int i,res=0;
  for(i=0;i<ar->m_uEntriesNumber;i++) {
    if (!ar->headers[i].t_Removed) res++;
  }
  return res;
}
*/

/*
int ziplib_index_ltop(Archive *ar,int index) {
  int lindex=0,res=0;

  if (index<0) return -1;
  for(res=0;res<ar->m_uEntriesNumber;res++) {
    if (!ar->headers[res].t_Removed) lindex++;
    if (lindex>index) return res;
  }

  return -1;
}
*/



