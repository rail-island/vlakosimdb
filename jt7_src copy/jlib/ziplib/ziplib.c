/*
 * This source file is part of the ziplib library
 * Author:Jan Prochazka, zputil@centrum.cz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 */

#include "ziplib.h"


#include "zl_types.h"

int ziplib_error=0;

int zl_EXPORT ziplib_Error() {
  return ziplib_error;
}
const char* zl_EXPORT ziplib_ErrorMsg(int code) {
  switch (code) {
    case ziplib_E_NO_ERROR:return "No error";
    case ziplib_E_BAD_FILE:return "Bad zip file";
    case ziplib_E_OPENED_FILE:return "Archive has opened file";
    case ziplib_E_BAD_OPENED_FILE:return "Archive has not opened file or file is opened in bad mode";
    case ziplib_E_BAD_FILE_INDEX:return "Index of file out of bounds";
    case ziplib_E_READONLY:return "Archive is opened only for read access";
    case ziplib_E_ZLIB:return "zlib compression library error";
    case ziplib_E_NULLP:return "null pointer error";
  }
  return "Unknown error";
}

Archive *ziplib_open(const char *file,int flags) {
  int pos;
  Archive *ar=NEW_CL(Archive);
  ar->readonly=flags==ziplib_OPEN_R;
  ar->t_OnDisk=1;

  if (ar->readonly) {
    ar->zipfile=fopen(file,"rb");
  } else {
    FILE *fr=fopen(file,"rb");
    if (fr) fclose(fr);
    if (fr) {
      ar->zipfile=fopen(file,"rb+");
    } else {//create new archive
      ar->zipfile=fopen(file,"wb+");
      ar->t_OnDisk=0;

      ar->m_uSize=0;
      ar->m_uOffset=0;
      ar->m_uDiskEntriesNo=ar->m_uEntriesNumber=0;
      memcpy(ar->m_Signature,CentralDir_gszSignature,4);
      ar->m_uThisDisk=0;
      ar->m_uDiskWithCD=0;
    }
  }
  if (!ar->zipfile) {
    DELETE_CL(ar);
    return NULL;
  }
  ar->opened=NULL;
  ar->info.m_iBufferSize=1024;
  ar->info.m_pBuffer=malloc(ar->info.m_iBufferSize);
  ziplib_seek(ar,0,SEEK_END);
  ar->filesize=ziplib_tell(ar);
  ziplib_seek(ar,0,SEEK_SET);
  read_central_dir(ar);

  return ar;
}



int ziplib_close(Archive *ar)  {
  if (!ar) return -1;
  if (ar->opened) ziplib_close_file(ar);
  ziplib_flush(ar);
  fclose(ar->zipfile);
  ziplib_free_archive(ar);
  return 0;
}

ziplib_Archive * zl_EXPORT ziplib_Open(const char *file,int flags) {
  return (ziplib_Archive *)ziplib_open(file,flags);
}
int zl_EXPORT ziplib_OpenFile(ziplib_Archive *ar,int index,int flags) {
  return ziplib_open_file((Archive*)ar,index,flags);
}
int zl_EXPORT ziplib_Close(ziplib_Archive *ar) {
  return ziplib_close((Archive*)ar);
}
int zl_EXPORT ziplib_Read(ziplib_Archive *ar,void *buffer,int bytes) {
  return ziplib_read_file((Archive*)ar,buffer,bytes);
}
int zl_EXPORT ziplib_Write(ziplib_Archive *ar,const void *buffer,int bytes) {
  return ziplib_write_file((Archive*)ar,buffer,bytes);
}
int zl_EXPORT ziplib_OpenNewFile(ziplib_Archive *ar,const char *name,int compressionLevel) {
  return ziplib_open_new_file((Archive*)ar,name,compressionLevel);
}
int zl_EXPORT ziplib_Flush(ziplib_Archive *ar) {
  return ziplib_flush((Archive*)ar);
}
int zl_EXPORT ziplib_RemoveFile(ziplib_Archive *ar,int index) {
  return ziplib_remove_file((Archive*)ar,index);
}
int zl_EXPORT ziplib_CloseFile(ziplib_Archive *ar) {
  return ziplib_close_file((Archive*)ar);
}
int zl_EXPORT ziplib_Get(ziplib_Archive *_ar,int index,ziplib_File *file) {
  Archive *ar=(Archive*)_ar;
  FileHeader *hdr=&ar->headers[index];

  if (!ar) return -1;
  if (ziplib_valid_index(ar,index)<0) return -1;

  file->size=hdr->m_uUncomprSize;
  file->name=hdr->m_Name;
  return 0;
}

int zl_EXPORT ziplib_Count(ziplib_Archive *_ar) {
  Archive *ar=(Archive*)_ar;
  
  if (!ar) return -1;
  return ar->m_uEntriesNumber;
}

