/*
 * This source file is part of the ziplib library
 * Author:Jan Prochazka, zputil@centrum.cz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 */


#include "zl_types.h"
#include "ziplib.h"

#ifndef DEF_MEM_LEVEL
#if MAX_MEM_LEVEL >= 8
#  define DEF_MEM_LEVEL 8
#else
#  define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#endif
#endif


int ziplib_write_file(Archive *ar,const void *pBuf,DWORD iSize) {
  FileHeader *hdr=ar->opened;

  if (iSize<=0) return 0;

  if (ar->m_iFileOpened!=1) ZL_ERROR(OPENED_FILE)

    ar->info.m_stream.next_in = (Bytef*)pBuf;
    ar->info.m_stream.avail_in = iSize;
    hdr->m_uCrc32 = crc32(hdr->m_uCrc32, (Bytef*)pBuf, iSize);


    while (ar->info.m_stream.avail_in > 0)
    {
        if (ar->info.m_stream.avail_out == 0)
        {
//			CryptEncodeBuffer();
           ziplib_write(ar,ar->info.m_pBuffer,ar->info.m_uComprLeft);
//			m_storage.Write(m_info.m_pBuffer, m_info.m_uComprLeft, false);
			ar->info.m_uComprLeft = 0;
            ar->info.m_stream.avail_out = ar->info.m_iBufferSize;
            ar->info.m_stream.next_out = (Bytef*)(char*)ar->info.m_pBuffer;
        }

        if (hdr->m_uMethod == Z_DEFLATED)
        {
            DWORD uTotal = ar->info.m_stream.total_out;
            int err = deflate(&ar->info.m_stream,  Z_NO_FLUSH);
            if (err) return -1;
            ar->info.m_uComprLeft += ar->info.m_stream.total_out - uTotal;
        }
        else
        {
            DWORD uToCopy = (ar->info.m_stream.avail_in < ar->info.m_stream.avail_out)
				? ar->info.m_stream.avail_in : ar->info.m_stream.avail_out;

			memcpy(ar->info.m_stream.next_out, ar->info.m_stream.next_in, uToCopy);

            ar->info.m_stream.avail_in -= uToCopy;
            ar->info.m_stream.avail_out -= uToCopy;
            ar->info.m_stream.next_in += uToCopy;
            ar->info.m_stream.next_out += uToCopy;
            ar->info.m_stream.total_in += uToCopy;
            ar->info.m_stream.total_out += uToCopy;
            ar->info.m_uComprLeft += uToCopy;
        }
    }

    return 0;
}


int ziplib_open_new_file(Archive *ar,const char *name,int iLevel) {
  FileHeader *hdr;

  if (ar->readonly) ZL_ERROR(READONLY)
  if (ar->opened) ZL_ERROR(OPENED_FILE)

  memset(&ar->info.m_stream,0,sizeof(z_stream));

  ziplib_seek_end(ar);

  if (ar->headerscapa==ar->m_uEntriesNumber) {
    ar->headerscapa+=32;
    ar->headers=realloc(ar->headers,ar->headerscapa*sizeof(FileHeader));
    memset(ar->headers+ar->m_uEntriesNumber,0,32*sizeof(FileHeader));
  }
  hdr=&ar->headers[ar->m_uEntriesNumber++];
  hdr->m_Name=(char*)malloc(strlen(name)+1);
  hdr->m_uOffset=ziplib_tell(ar);

  strcpy(hdr->m_Name,name);

  ar->opened=hdr;

  if (ziplib_prepare_header_data(hdr,iLevel)) return -1;

  ar->t_OnDisk=0;

  ziplib_write_local_header(ar,hdr);

  ar->info.m_uComprLeft = 0;
  ar->info.m_stream.avail_in = (uInt)0;
  ar->info.m_stream.avail_out = (uInt)ar->info.m_iBufferSize;
  ar->info.m_stream.next_out = (Bytef*)(char*)ar->info.m_pBuffer;
  ar->info.m_stream.total_in = 0;
  ar->info.m_stream.total_out = 0;

  if (hdr->m_uMethod == Z_DEFLATED) {
    int err;
    ar->info.m_stream.opaque = 0;
    err = deflateInit2(&ar->info.m_stream, iLevel,Z_DEFLATED, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY);
    if (err) return -1;
  }
  ar->m_iFileOpened = 1;//compress;
  return 0;
}

int ziplib_prepare_header_data(FileHeader *hdr,int iLevel) {
  hdr->m_uExternalAttr = 0;
  hdr->m_uModDate = hdr->m_uModTime = 0;
  hdr->m_uMethod = Z_DEFLATED;
  hdr->t_ForceWrite=1;

  memcpy(hdr->m_szSignature,FileHeader_gszSignature,4);
  hdr->m_uInternalAttr = 0;
  hdr->m_uVersionMadeBy = 0x14;
  hdr->m_uVersionNeeded = 0x14;// IsDirectory() ? 0xa : 0x14; // 1.0 or 2.0
//  hdr->m_uVersionNeeded = 0x14;// IsDirectory() ? 0xa : 0x14; // 1.0 or 2.0
//  ziplib_set_version(hdr,(WORD)(0x14));
  hdr->m_uCrc32 = 0;
  hdr->m_uComprSize = 0;
  hdr->m_uUncomprSize = 0;
  hdr->m_uDiskStart = 0;

  if (iLevel == 0) hdr->m_uMethod = 0;

  if ((hdr->m_uMethod != Z_DEFLATED) && (hdr->m_uMethod != 0))  hdr->m_uMethod = Z_DEFLATED;

  hdr->m_uFlag  = 0;
  if (hdr->m_uMethod == Z_DEFLATED)
		switch (iLevel)
		{
		case 1:
			hdr->m_uFlag  |= 6;
			break;
		case 2:
			hdr->m_uFlag  |= 4;
			break;
		case 8:
		case 9:
			hdr->m_uFlag  |= 2;
			break;
		}

/*
	if (bSpan || bEncrypted)
		m_uFlag  |= 8; // data descriptor present

	if (bEncrypted)
	{
		m_uComprSize = ZIPARCHIVE_ENCR_HEADER_LEN;	// encrypted header
		m_uFlag  |= 1;		// encrypted file
	}
*/

  return 0;
/*	return !(m_pszComment.GetSize() > USHRT_MAX || m_pszFileName.GetSize() > USHRT_MAX
		|| m_pExtraField.GetSize() > USHRT_MAX);
                */
}


int ziplib_close_new_file(Archive *ar) {
  int err = Z_OK;
  FileHeader *hdr=ar->opened;

  if (!ar->opened) return -1;
  if (ar->m_iFileOpened != 1) return -1;

  ar->info.m_stream.avail_in = 0;
  if (hdr->m_uMethod == Z_DEFLATED) {
    while (err == Z_OK) {
      DWORD uTotal;
      if (ar->info.m_stream.avail_out == 0) {
        ziplib_write(ar,ar->info.m_pBuffer,ar->info.m_uComprLeft);
        ar->info.m_uComprLeft = 0;
        ar->info.m_stream.avail_out = ar->info.m_iBufferSize;
        ar->info.m_stream.next_out = (Bytef*)(char*)ar->info.m_pBuffer;
      }
      uTotal = ar->info.m_stream.total_out;
      err = deflate(&ar->info.m_stream,  Z_FINISH);
      ar->info.m_uComprLeft += ar->info.m_stream.total_out - uTotal;
    }
  }
  if (err == Z_STREAM_END) err = Z_OK;
  if (err) return -1;


  if (ar->info.m_uComprLeft > 0) {
/*    if (ar->info.m_uComprLeft>200) {
      FILE *fw=fopen("c:/zipz.zip.gz","wb");
      fwrite(ar->info.m_pBuffer,1,ar->info.m_uComprLeft,fw);
      fclose(fw);
    }
    */

    ziplib_write(ar,ar->info.m_pBuffer,ar->info.m_uComprLeft);
  }
  if (hdr->m_uMethod == Z_DEFLATED) {
    err = deflateEnd(&ar->info.m_stream);
    if (err) return -1;
  }

  // it may be increased by the encrypted header size
  hdr->m_uComprSize += ar->info.m_stream.total_out;
  hdr->m_uUncomprSize = ar->info.m_stream.total_in;

//  m_centralDir.CloseNewFile();
//  return 0;
  ar->opened=NULL;
  ar->m_iFileOpened=0;
  ar->m_uOffset+=hdr->m_uComprSize+hdr->t_LocalHeaderSize;

//  ziplib_seek(ar,hdr->m_uOffset,SEEK_SET);
  //repaired header
//  ziplib_write_local_header(ar,hdr);
//  ar->info.ReleaseBuf();
//  EmptyPtrList();
  //now is necessary to update local

  return 0;
}


