/*
 * This source file is part of the ziplib library
 * Author:Jan Prochazka, zputil@centrum.cz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 */

#ifndef zl_typesH
#define zl_typesH

#include <zlib/zlib.h>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


#define CENTRALDIRSIZE	22
#define FILEHEADERSIZE	46
#define LOCALFILEHEADERSIZE	30


#define NEW_CL(class_n) (class_n*)ziplib_malloc_0(sizeof(class_n))
#define DELETE_CL(object_n) free(object_n)
#define NEWA_CL(class_n,count_n) (class_n*)ziplib_malloc_0(sizeof(class_n)*count_n)

#define ZL_ERROR(code_n) {ziplib_error=ziplib_E_##code_n;return -1;}

extern int cd_buffersize;
extern char CentralDir_gszSignature[];

extern char FileHeader_gszSignature[];
extern char FileHeader_gszLocalSignature[];


typedef unsigned long DWORD;
typedef unsigned short WORD;

typedef struct FileHeader {
  char m_szSignature[4];			///< central file header signature
  WORD m_uVersionMadeBy;			///< version made by and system compatibility
  WORD m_uVersionNeeded;			///< version needed to extract
  WORD m_uFlag;					///< general purpose bit flag
  WORD m_uMethod;					///< compression method
  WORD m_uModTime;				///< last mod file time
  WORD m_uModDate;				///< last mod file date
  DWORD m_uCrc32;					///< crc-32
  DWORD m_uComprSize;				///< compressed size
  DWORD m_uUncomprSize;			///< uncompressed size
  //filename length                 2 bytes
  //WORD m_uFileNameSize;
  //extra field length              2 bytes
  //WORD m_uExtraFieldSize;
  //file comment length             2 bytes
  //WORD m_uCommentSize;
  WORD m_uDiskStart;				///< disk number start
  WORD m_uInternalAttr;			///< internal file attributes
  DWORD m_uExternalAttr;			///< external file attributes
  DWORD m_uOffset;				///< relative offset of local header
  char *m_Name;

  int t_LocalHeaderSize;
  int t_Removed;

  int t_ForceWrite;//is changed, must be written
} FileHeader;

//structure contains data nedde to comunicate with zlib
typedef struct InternalInfo {
  z_stream m_stream;/*zlib library data stream*/
  DWORD m_uUncomprLeft;/*bytes left to uncompress*/
  DWORD m_uComprLeft;/*bytes left to decompress*/
  DWORD m_uCrc32;/*crc32 file control value*/
  DWORD m_iBufferSize;/*size of m_pBuffer*/
  char *m_pBuffer;
} InternalInfo;

typedef struct Archive {
  FILE *zipfile;

  char m_Signature[4];
  unsigned long filesize;
  DWORD m_uCentrDirPos;	/*the position of the beginning of the central directory (as located by #Locate*/
  DWORD m_uBytesBeforeZip;/*The number of bytes before the actual zip archive in a file. It is non-zero for self-extracting archives*/
  WORD m_uThisDisk;/*number of the disk with the central directory end record (the number of disks in the disk-spanning archive)*/
  WORD m_uDiskWithCD;/*number of the disk with the start of the central directory*/
  WORD m_uDiskEntriesNo;/*total number of entries in the central dir on this disk*/
  WORD m_uEntriesNumber;/*total number of entries in the central dir*/
  DWORD m_uSize;/*size of the central directory (valid only if #m_bOnDisk is \c true; use #GetSize instead)*/
  DWORD m_uOffset;/*offset of start of central directory with respect to the starting disk number
		(as written in the central directory record);
		valid only if t_OnDisK is \c true*/
//  char m_bOnDisk;/*\c true if the central directory is physically present in the archive*/

  int headerscapa;
  int t_OnDisk;

  char m_iFileOpened;

  InternalInfo info;

  FileHeader *headers;

  FileHeader *opened;

  int readonly;
} Archive;

extern int read_central_dir(Archive *ar);
extern int ziplib_read_headers(Archive *ar);
extern int ziplib_read_header(Archive *ar,FileHeader *hdr);
extern int locate_central_dir(Archive *ar);
//extern int ziplib_less_index(Archive *ar,int index);
extern int ziplib_read_file(Archive *ar,void *pBuf,DWORD iSize);
extern int ziplib_open_file(Archive *ar,int index,int flags);
extern int ziplib_read_local_header(Archive *ar,FileHeader *hdr);
extern int ziplib_open_new_file(Archive *ar,const char *name,int iLevel);
extern int ziplib_prepare_header_data(FileHeader *hdr,int iLevel);
extern int ziplib_write_file(Archive *ar,const void *pBuf,DWORD iSize);
extern int ziplib_flush(Archive *ar);
extern int ziplib_write_local_header(Archive *ar,FileHeader *hdr);
extern int ziplib_write_headers(Archive *ar);
extern int ziplib_write_central_dir(Archive *ar);
extern DWORD ziplib_write_central_end(Archive *ar);
extern int ziplib_close_new_file(Archive *ar);
extern int ziplib_close_read_file(Archive *ar);
extern int ziplib_close_file(Archive *ar);
extern int ziplib_open_new_file(Archive *ar,const char *name,int iLevel);
extern int ziplib_write_file(Archive *ar,const void *pBuf,DWORD iSize);
extern int ziplib_seek_end(Archive *ar);
extern void *ziplib_malloc_0(int size);
extern int ziplib_remove_file(Archive* ar,int index);
//extern int ziplib_index_ltop(Archive *ar,int index);
extern int ziplib_count_local_header_size(Archive *ar,FileHeader *hdr);
extern int ziplib_write_local_header_to_buffer(Archive *ar,FileHeader *hdr,char *buf);
extern int ziplib_valid_index(Archive* ar,int index);
extern int ziplib_free_header(FileHeader *hdr);
extern int ziplib_free_archive(Archive *ar);

//IO functions
int ziplib_read(Archive *ar,void *buf,int bytes);
int ziplib_write(Archive *ar,const void *buf,int bytes);
int ziplib_tell(Archive *ar);
int ziplib_seek(Archive *ar,int pos,int whence);
int ziplib_truncate(Archive *ar,int filesize);



extern int ziplib_error;


#endif
