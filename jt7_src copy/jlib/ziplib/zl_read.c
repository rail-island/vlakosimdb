/*
 * This source file is part of the ziplib library
 * Author:Jan Prochazka, zputil@centrum.cz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 */


#include "zl_types.h"


int ziplib_open_file(Archive *ar,int index,int flags) {
  WORD uMethod;
//  WORD uLocalExtraFieldSize;

  if (ziplib_valid_index(ar,index)<0) return -1;
  if (ar->opened) return -1;

  ziplib_seek(ar,ar->headers[index].m_uOffset,SEEK_SET);
  if (ziplib_read_local_header(ar,&ar->headers[index])) return -1;

  memset(&ar->info.m_stream,0,sizeof(z_stream));

  uMethod = ar->headers[index].m_uMethod;

  if ((uMethod != 0) &&(uMethod != Z_DEFLATED))
    return -1;
//    ThrowError(CZipException::badZipFile);

  if (uMethod == Z_DEFLATED) {
    int err;
    ar->info.m_stream.opaque =  0;
    err = inflateInit2(&ar->info.m_stream, -MAX_WBITS);
		//			* windowBits is passed < 0 to tell that there is no zlib header.
		//          * Note that in this case inflate *requires* an extra "dummy" byte
		//          * after the compressed stream in order to complete decompression and
		//          * return Z_STREAM_END.
    if (err) return -1;
  }
  ar->opened=&ar->headers[index];

  ar->info.m_uComprLeft = ar->headers[index].m_uComprSize;
  ar->info.m_uUncomprLeft = ar->headers[index].m_uUncomprSize;
  ar->info.m_uCrc32 = 0;
  ar->info.m_stream.total_out = 0;
  ar->info.m_stream.avail_in = 0;
  ar->m_iFileOpened = -1;/*extract*/
  return 0;
}

int ziplib_read_file(Archive *ar,void *pBuf,DWORD iSize) {
  DWORD iRead=0;
  int bForce;

  if (ar->m_iFileOpened != -1) return -1;
  if (!pBuf || !iSize) return -1;

  ar->info.m_stream.next_out = (Bytef*)pBuf;
  ar->info.m_stream.avail_out = iSize > ar->info.m_uUncomprLeft? ar->info.m_uUncomprLeft : iSize;


  // may happen when the file is 0 sized
  bForce = ar->info.m_stream.avail_out == 0 && ar->info.m_uComprLeft > 0;
  while (ar->info.m_stream.avail_out > 0 || (bForce && ar->info.m_uComprLeft > 0))
	{
		if ((ar->info.m_stream.avail_in == 0) &&
			(ar->info.m_uComprLeft > 0))
		{
			DWORD uToRead = ar->info.m_iBufferSize;
			if (ar->info.m_uComprLeft < uToRead)
				uToRead = ar->info.m_uComprLeft;

			if (uToRead == 0)
				return 0;

                        ziplib_read(ar,ar->info.m_pBuffer,uToRead);
                        
			ar->info.m_uComprLeft -= uToRead;

			ar->info.m_stream.next_in = (Bytef*)(char*)ar->info.m_pBuffer;
			ar->info.m_stream.avail_in = uToRead;
		}

		if (ar->opened->m_uMethod == 0)
		{
			DWORD uToCopy = ar->info.m_stream.avail_out < ar->info.m_stream.avail_in
				? ar->info.m_stream.avail_out : ar->info.m_stream.avail_in;

			memcpy(ar->info.m_stream.next_out, ar->info.m_stream.next_in, uToCopy);

			ar->info.m_uCrc32 = crc32(ar->info.m_uCrc32, ar->info.m_stream.next_out, uToCopy);

			ar->info.m_uUncomprLeft -= uToCopy;
			ar->info.m_stream.avail_in -= uToCopy;
			ar->info.m_stream.avail_out -= uToCopy;
			ar->info.m_stream.next_out += uToCopy;
			ar->info.m_stream.next_in += uToCopy;
            ar->info.m_stream.total_out += uToCopy;
			iRead += uToCopy;
		}
		else
		{
			DWORD uTotal = ar->info.m_stream.total_out;
			Bytef* pOldBuf =  ar->info.m_stream.next_out;
			int err = inflate(&ar->info.m_stream, Z_SYNC_FLUSH);
			DWORD uToCopy = ar->info.m_stream.total_out - uTotal;

			ar->info.m_uCrc32 = crc32(ar->info.m_uCrc32, pOldBuf, uToCopy);

			ar->info.m_uUncomprLeft -= uToCopy;
			iRead += uToCopy;

			if (err == Z_STREAM_END)
				return iRead;

                        if (err) return -1;
//			CheckForError(err);
		}
	}

	return iRead;
}

int ziplib_close_read_file(Archive *ar) {
  int result=0;
  int m_bIgnoreCRC=0;

  if (ar->m_iFileOpened!=-1) return -1;

  if (ar->info.m_uUncomprLeft == 0) {
    if (!m_bIgnoreCRC && ar->info.m_uCrc32 != ar->opened->m_uCrc32) {
      result=-1;
    }
  } else result=-1;

  if (ar->opened->m_uMethod == Z_DEFLATED) inflateEnd(&ar->info.m_stream);

  ar->opened=NULL;
  ar->m_iFileOpened=0;

  return result;
}

