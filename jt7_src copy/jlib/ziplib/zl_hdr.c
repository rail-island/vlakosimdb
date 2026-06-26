/*
 * This source file is part of the ziplib library
 * Author:Jan Prochazka, zputil@centrum.cz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 */


#include "zl_types.h"

static int ziplib_hdr_dummy=0;//to avoid Borland C++ Linker Error

int ziplib_free_header(FileHeader *hdr) {
  free(hdr->m_Name);
  return 0;
}

int ziplib_write_local_header_to_buffer(Archive *ar,FileHeader *hdr,char *buf) {
  WORD uFileNameSize = strlen(hdr->m_Name),uExtraFieldSize=0;

  memcpy(buf,FileHeader_gszLocalSignature,4);
  memcpy(buf+4,&hdr->m_uVersionNeeded,2);
  memcpy(buf+6,&hdr->m_uFlag,2);
  memcpy(buf+8,&hdr->m_uMethod,2);
  memcpy(buf+10,&hdr->m_uModTime,2);
  memcpy(buf+12,&hdr->m_uModDate,2);
  memcpy(buf+14,&hdr->m_uCrc32,4);
  memcpy(buf+18,&hdr->m_uComprSize,4);
  memcpy(buf+22,&hdr->m_uUncomprSize,4);
  memcpy(buf+26,&uFileNameSize,2);
  memcpy(buf+28,&uExtraFieldSize,2);
  memcpy(buf+30,hdr->m_Name,uFileNameSize);
  return 0;
}

int ziplib_count_local_header_size(Archive *ar,FileHeader *hdr) {
  WORD uFileNameSize = strlen(hdr->m_Name),uExtraFieldSize=0;
  DWORD iLocalSize = LOCALFILEHEADERSIZE + uExtraFieldSize + uFileNameSize;
  hdr->t_LocalHeaderSize=iLocalSize;
  return 0;
}

int ziplib_write_local_header(Archive *ar,FileHeader *hdr) {
  char *buf;

  ziplib_count_local_header_size(ar,hdr);
  buf=(char*)malloc(hdr->t_LocalHeaderSize);
  ziplib_write_local_header_to_buffer(ar,hdr,buf);
  ziplib_write(ar,buf,hdr->t_LocalHeaderSize);
  free(buf);

  return 0;

/*  WORD uFileNameSize = strlen(hdr->m_Name),uExtraFieldSize=0;
  DWORD iLocalSize = LOCALFILEHEADERSIZE + uExtraFieldSize + uFileNameSize;
  char *buf=(char*)malloc(iLocalSize);
  memcpy(buf,FileHeader_gszLocalSignature,4);
  memcpy(buf+4,&hdr->m_uVersionNeeded,2);
  memcpy(buf+6,&hdr->m_uFlag,2);
  memcpy(buf+8,&hdr->m_uMethod,2);
  memcpy(buf+10,&hdr->m_uModTime,2);
  memcpy(buf+12,&hdr->m_uModDate,2);
  memcpy(buf+14,&hdr->m_uCrc32,4);
  memcpy(buf+18,&hdr->m_uComprSize,4);
  memcpy(buf+22,&hdr->m_uUncomprSize,4);
  memcpy(buf+26,&uFileNameSize,2);
  memcpy(buf+28,&uExtraFieldSize,2);
  memcpy(buf+30,hdr->m_Name,uFileNameSize);
//  memcpy(buf+30+uFileNameSize,m_pExtraField,uExtraFieldSize);

  // possible disk change before writing to the file in the disk spanning mode
  // so write the local header first
  ziplib_write(ar,buf,iLocalSize);
  free(buf);
  hdr->m_uOffset=ziplib_tell(ar)-iLocalSize;
  hdr->t_LocalHeaderSize=iLocalSize;
  return 0;*/
}


int ziplib_read_local_header(Archive *ar,FileHeader *hdr) {
//int ziplib_read_local_header(Archive *ar,int index,WORD *iLocExtrFieldSize) {
  WORD iLocExtrFieldSize=0;
  char buf[LOCALFILEHEADERSIZE];
  int bIsDataDescr;
  WORD uFileNameSize;
  WORD uTemp;
//  FileHeader *hdr=&ar->headers[index];

  ziplib_read(ar,buf,LOCALFILEHEADERSIZE);
  if (memcmp(buf, FileHeader_gszLocalSignature, 4) != 0) return -1;

  bIsDataDescr = (((WORD)*(buf + 6)) & 8) != 0;

  uFileNameSize=strlen(hdr->m_Name);
//	uTemp;
  memcpy(&uTemp, buf+6, 2);
  if (( (uTemp & 0xf) != (hdr->m_uFlag & 0xf))
     ||(memcmp(buf + 8, &hdr->m_uMethod, 2) != 0)
     || (hdr->m_uMethod && (hdr->m_uMethod != Z_DEFLATED))
     || (memcmp(buf + 26, &uFileNameSize, 2) != 0))
       return -1;

// jeszcze mo¿naby porównaæ nazwy plików

//	if (!bIsDataDescr/* || !pStorage->IsSpanMode()*/)
//		if (!CheckCrcAndSizes(buf + 14))
//			return -1;

  memcpy(&iLocExtrFieldSize, buf + 28, 2);
  ziplib_seek(ar,uFileNameSize,SEEK_CUR);
//	pStorage->m_pFile->Seek(uFileNameSize, CZipAbstractFile::current);

  hdr->t_LocalHeaderSize=uFileNameSize+LOCALFILEHEADERSIZE+iLocExtrFieldSize;
  ziplib_seek(ar,iLocExtrFieldSize,SEEK_CUR);
  return 0;
}

