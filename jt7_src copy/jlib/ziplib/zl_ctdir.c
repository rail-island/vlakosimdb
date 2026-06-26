/*
 * This source file is part of the ziplib library
 * Author:Jan Prochazka, zputil@centrum.cz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 */


#include "zl_types.h"
#include "ziplib.h"


int locate_central_dir(Archive *ar) {
  // maximum size of end of central dir record
  long uMaxRecordSize = 0xffff + CENTRALDIRSIZE;
  char *buf=malloc(cd_buffersize);
  long uPosInFile=0;
  int uRead=0;

  if (uMaxRecordSize>ar->filesize) uMaxRecordSize=ar->filesize;

  // backward reading
  while (uPosInFile < uMaxRecordSize) {
    int iToRead;
    int iActuallyRead;
    int i;

    uPosInFile = uRead + cd_buffersize;
    if (uPosInFile > uMaxRecordSize)
    uPosInFile = uMaxRecordSize;
    iToRead = uPosInFile - uRead;
    ziplib_seek(ar,-uPosInFile,SEEK_END);
    iActuallyRead=ziplib_read(ar,buf,iToRead);
    if (iActuallyRead != iToRead) {
      free(buf);
      ZL_ERROR(BAD_FILE)
    }
    // search from the very last bytes to prevent an error if inside archive
    // there are packed other arhives
    for (i = iToRead - 4; i >=0 ; i--)
      if (!memcmp((char*)buf + i, CentralDir_gszSignature, 4)) {
        free(buf);
        return ar->filesize - (uPosInFile - i);
      }
    uRead += iToRead - 3;
  }
  free(buf);
  ZL_ERROR(BAD_FILE)
}

int ziplib_read_header(Archive *ar,FileHeader *hdr) {
  WORD uFileNameSize, uCommentSize, uExtraFieldSize;
  char *buf=malloc(FILEHEADERSIZE);
  ziplib_read(ar,buf,FILEHEADERSIZE);
  memcpy(&hdr->m_szSignature,buf, 4);
  memcpy(&hdr->m_uVersionMadeBy,buf + 4, 2);
  memcpy(&hdr->m_uVersionNeeded,buf + 6, 2);
  memcpy(&hdr->m_uFlag,buf + 8, 2);
  memcpy(&hdr->m_uMethod,buf + 10, 2);
  memcpy(&hdr->m_uModTime,buf + 12, 2);
  memcpy(&hdr->m_uModDate,buf + 14, 2);
  memcpy(&hdr->m_uCrc32,buf + 16, 4);
  memcpy(&hdr->m_uComprSize,buf + 20, 4);
  memcpy(&hdr->m_uUncomprSize,buf + 24, 4);
  memcpy(&uFileNameSize,buf + 28, 2);
  memcpy(&uExtraFieldSize,buf + 30, 2);
  memcpy(&uCommentSize,buf + 32, 2);
  memcpy(&hdr->m_uDiskStart,buf + 34, 2);
  memcpy(&hdr->m_uInternalAttr,buf + 36, 2);
  memcpy(&hdr->m_uExternalAttr,buf + 38, 4);
  memcpy(&hdr->m_uOffset,buf + 42, 4);
  free(buf);
  if (memcmp(hdr->m_szSignature, FileHeader_gszSignature, 4) != 0) ZL_ERROR(BAD_FILE)
  hdr->t_ForceWrite=0;
//  hdr->t_ForceWrite=1;
  hdr->m_Name=(char*)malloc(uFileNameSize+1);
  ziplib_read(ar,hdr->m_Name,uFileNameSize);
  hdr->m_Name[uFileNameSize]=0;
  ziplib_seek(ar,uExtraFieldSize,SEEK_CUR);
  ziplib_seek(ar,uCommentSize,SEEK_CUR);
  return 0;
}



int ziplib_read_headers(Archive *ar) {
  int i;
  ziplib_seek(ar,ar->m_uOffset,SEEK_SET);
//m_pStorage->m_pFile->Seek(m_info.m_uOffset + m_info.m_uBytesBeforeZip, CZipAbstractFile::begin);

//	RemoveHeaders(); //just in case
  ar->headers=NEWA_CL(FileHeader,ar->m_uEntriesNumber);
  ar->headerscapa=ar->m_uEntriesNumber;
  for(i=0;i<ar->m_uEntriesNumber;i++) {
//    ziplib_FileHeader *hdr=NEW(ziplib_FileHeader);
//    head
    if (ziplib_read_header(ar,&ar->headers[i])) return -1;
  }
  return 0;
}

int read_central_dir(Archive *ar) {
  WORD uCommentSize;
  char *buf=malloc(CENTRALDIRSIZE);
  int uRead;

  ar->m_uCentrDirPos = locate_central_dir(ar);
  ziplib_seek(ar,ar->m_uCentrDirPos,SEEK_SET);

  uRead=ziplib_read(ar,buf,CENTRALDIRSIZE);

  if (uRead != CENTRALDIRSIZE) {
    free(buf);
    ZL_ERROR(BAD_FILE)
  }
  memcpy(&ar->m_Signature,buf,4);
  memcpy(&ar->m_uThisDisk,buf+4,2);
  memcpy(&ar->m_uDiskWithCD,buf+6,2);
  memcpy(&ar->m_uDiskEntriesNo,buf+8,2);
  memcpy(&ar->m_uEntriesNumber,buf+10,2);
  memcpy(&ar->m_uSize,buf+12,4);
  memcpy(&ar->m_uOffset,buf+16,4);
  memcpy(&uCommentSize,buf+20,2);
  free(buf);

//	m_pStorage->UpdateSpanMode(m_info.m_uThisDisk);
	// if m_uThisDisk is not zero, it is enough to say that it is a multi disk archive
//	ASSERT((!m_info.m_uThisDisk && (m_info.m_uEntriesNumber == m_info.m_uDiskEntriesNo) && !m_info.m_uDiskWithCD) || m_info.m_uThisDisk);



//	if (!m_pStorage->IsSpanMode() && !m_info.CheckIfOK_1())
//		ThrowError(CZipException::badZipFile);

  //skiping comment
  if (ziplib_seek(ar,uCommentSize,SEEK_CUR)<0) return -1;

  if (ziplib_read_headers(ar)<0) {
    ar->m_uEntriesNumber=0;
    if (ar->headers) free(ar->headers);
    ar->headers=NULL;
    ar->headerscapa=0;
    return -1;
  }
  return 0;
}


int ziplib_write_central_dir(Archive *ar) {
  if (ar->t_OnDisk) return 0;
  if (ziplib_seek_end(ar)) return -1;

  if (ziplib_write_headers(ar)) return -1;

  if (ziplib_write_central_end(ar)) return -1;
  ar->t_OnDisk=1;

  return 0;

}

DWORD ziplib_write_header(Archive *ar,FileHeader *hdr) {
  WORD uFileNameSize = strlen(hdr->m_Name),uCommentSize = 0,uExtraFieldSize = 0;
  DWORD iSize = FILEHEADERSIZE + uFileNameSize + uCommentSize + uExtraFieldSize;
  char *buf=(char*)malloc(iSize);
  int result;

  memcpy(buf,&hdr->m_szSignature,4);
  memcpy(buf + 4, &hdr->m_uVersionMadeBy, 2);
  memcpy(buf + 6, &hdr->m_uVersionNeeded, 2);
  memcpy(buf + 8, &hdr->m_uFlag, 2);
  memcpy(buf + 10, &hdr->m_uMethod, 2);
  memcpy(buf + 12, &hdr->m_uModTime, 2);
  memcpy(buf + 14, &hdr->m_uModDate, 2);
  memcpy(buf + 16, &hdr->m_uCrc32, 4);
  memcpy(buf + 20, &hdr->m_uComprSize, 4);
  memcpy(buf + 24, &hdr->m_uUncomprSize, 4);
  memcpy(buf + 28, &uFileNameSize, 2);
  memcpy(buf + 30, &uExtraFieldSize, 2);
  memcpy(buf + 32, &uCommentSize, 2);
  memcpy(buf + 34, &hdr->m_uDiskStart, 2);
  memcpy(buf + 36, &hdr->m_uInternalAttr, 2);
  memcpy(buf + 38, &hdr->m_uExternalAttr, 4);
  memcpy(buf + 42, &hdr->m_uOffset, 4);
  memcpy(buf + 46, hdr->m_Name, uFileNameSize);

/*
  if (uExtraFieldSize) memcpy(buf + 46 + uFileNameSize, m_pExtraField, uExtraFieldSize);

	if (uCommentSize)
		memcpy(buf + 46 + uFileNameSize + uExtraFieldSize, m_pszComment, uCommentSize);

	pStorage->Write(buf, iSize, true);
	return iSize;
  */
  result=ziplib_write(ar,buf,iSize);
  free(buf);
  return result;
}


int ziplib_write_headers(Archive *ar) {
  int i;
  ar->m_uOffset=ziplib_tell(ar);
//  ziplib_seek(ar,ar->m_uOffset,SEEK_SET);
//m_pStorage->m_pFile->Seek(m_info.m_uOffset + m_info.m_uBytesBeforeZip, CZipAbstractFile::begin);

//	RemoveHeaders(); //just in case
//  ar->headers=NEWA(FileHeader,ar->m_uEntriesNumber);
//  ar->headerscapa=ar->m_uEntriesNumber;
  for(i=0;i<ar->m_uEntriesNumber;i++) {
//    ziplib_FileHeader *hdr=NEW(ziplib_FileHeader);
//    head
    if (ziplib_write_header(ar,&ar->headers[i])<0) return -1;
  }
  return 0;
}

DWORD ziplib_write_central_end(Archive *ar) {
  DWORD uSize = CENTRALDIRSIZE;
  char *buf=(char*)malloc(uSize);
  WORD uCommentSize = 0;
  int result;

  ar->m_uSize=ziplib_tell(ar)-ar->m_uOffset;

  ar->m_uDiskEntriesNo=ar->m_uEntriesNumber;

  memcpy(buf, CentralDir_gszSignature, 4);
  memcpy(buf + 4, &ar->m_uThisDisk, 2);
  memcpy(buf + 6, &ar->m_uDiskWithCD, 2);
  memcpy(buf + 8, &ar->m_uDiskEntriesNo, 2);
  memcpy(buf + 10, &ar->m_uEntriesNumber, 2);
  memcpy(buf + 12, &ar->m_uSize, 4);
  memcpy(buf + 16, &ar->m_uOffset, 4);
  memcpy(buf + 20, &uCommentSize, 2);
//  memcpy(buf + 22, m_pszComment, uCommentSize);
  result=ziplib_write(ar,buf,uSize);
  free(buf);
  ziplib_truncate(ar,ziplib_tell(ar));
  if (result) return result;
  return uSize;
}

