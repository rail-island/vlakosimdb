/*
 * This source file is part of the ziplib library
 * Author:Jan Prochazka, zputil@centrum.cz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 */

#ifndef ziplibH
#define ziplibH

#define zl_EXPORT __export __stdcall

#define ziplib_E_NO_ERROR 0
#define ziplib_E_BAD_FILE 1
#define ziplib_E_OPENED_FILE 2
#define ziplib_E_BAD_OPENED_FILE 3
#define ziplib_E_BAD_FILE_INDEX 4
#define ziplib_E_READONLY 5
#define ziplib_E_ZLIB 6
#define ziplib_E_NULLP 7

#ifdef __cplusplus
extern "C" {
#endif

#define ziplib_OPEN_R 0
#define ziplib_OPEN_RW 1

typedef struct ziplib_Archive {
  char first_byte;//struct cannot be empty
} ziplib_Archive;

typedef struct ziplib_File {
  const char *name;
  int size;
} ziplib_File;

extern ziplib_Archive * zl_EXPORT ziplib_Open(const char *file,int flags);
extern int zl_EXPORT ziplib_Close(ziplib_Archive *ar);
extern int zl_EXPORT ziplib_OpenFile(ziplib_Archive *ar,int index,int flags);
extern int zl_EXPORT ziplib_Read(ziplib_Archive *ar,void *buffer,int bytes);
extern int zl_EXPORT ziplib_CloseFile(ziplib_Archive *ar);
extern int zl_EXPORT ziplib_Write(ziplib_Archive *ar,const void *buffer,int bytes);
extern int zl_EXPORT ziplib_OpenNewFile(ziplib_Archive *ar,const char *name,int compressionLevel);
extern int zl_EXPORT ziplib_Flush(ziplib_Archive *ar);
extern int zl_EXPORT ziplib_RemoveFile(ziplib_Archive *ar,int index);
extern int zl_EXPORT ziplib_Get(ziplib_Archive *ar,int index,ziplib_File *file);
extern int zl_EXPORT ziplib_Count(ziplib_Archive *ar);
extern int zl_EXPORT ziplib_Error();
extern const char* zl_EXPORT ziplib_ErrorMsg(int code);

extern int ziplib_error;

#ifdef __cplusplus
} //extern "C"
#endif

#endif
