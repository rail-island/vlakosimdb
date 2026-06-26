/*
 * This source file is part of the ziplib library
 * Author:Jan Prochazka, zputil@centrum.cz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 */


#include "zl_types.h"
#include "ziplib.h"

#define FLUSHBUFFERSIZE 0x1000

int ziplib_remove_file(Archive* ar,int index) {
  if (!ar) ZL_ERROR(NULLP)
  if (ar->opened) ZL_ERROR(OPENED_FILE)
  if (ziplib_valid_index(ar,index)<0) return -1;
  ziplib_free_header(&ar->headers[index]);
  memmove(ar->headers+index,ar->headers+index+1,sizeof(FileHeader)*(ar->m_uEntriesNumber-index-1));
  ar->m_uEntriesNumber--;
/*  index=ziplib_index_ltop(ar,index);
  if (index<0) return -1;
  ar->headers[index].t_Removed=1;*/
  return 0;
}

typedef struct FlushBuffer {
  //size of buffer (capacity)
  int size;
  //data
  char *buf;
  //number of bytes in buffer
  int inbuf;
  //writing positon
  int writepos;
  //free bytes
  int avail;
} FlushBuffer;

void findpkpk(char *buf,int size) {
  while (size>=4) {
    if (buf[0]=='P' && buf[1]=='K' && buf[2]=='P' && buf[3]=='K') {
      int xxx=0;
    }
    size--,buf++;
  }
}

void ziplib_flush_buffer(Archive *ar,FlushBuffer *buffer) {
  int lastpos=ziplib_tell(ar);

  ziplib_seek(ar,buffer->writepos,SEEK_SET);
  ziplib_write(ar,buffer->buf,buffer->inbuf);
  buffer->writepos+=buffer->inbuf;
  buffer->avail=buffer->size;
  buffer->inbuf=0;

  ziplib_seek(ar,lastpos,SEEK_SET);
}

void ziplib_flush_move_data(Archive *ar,FlushBuffer *buffer,int size) {
  while (size>0) {
    int towrite=size;
    if (towrite>buffer->avail) towrite=buffer->avail;
    ziplib_read(ar,buffer->buf+buffer->inbuf,towrite);
    buffer->inbuf+=towrite;
    buffer->avail-=towrite;
  findpkpk(buffer->buf,buffer->inbuf);
    if (buffer->avail==0) ziplib_flush_buffer(ar,buffer);
    size-=towrite;
  }
}

int ziplib_flush(Archive *ar) {
  int i;
  int logwritepos=0;
  FlushBuffer fb;

  if (ar->opened) ZL_ERROR(OPENED_FILE)
  if (ar->t_OnDisk) return 0;

  fb.size=FLUSHBUFFERSIZE;
  fb.buf=malloc(fb.size);
  fb.writepos=0;
  fb.inbuf=0;
  fb.avail=fb.size;

  ziplib_seek(ar,0,SEEK_SET);

  for(i=0;i<ar->m_uEntriesNumber;i++) {
    FileHeader *hdr=&ar->headers[i];
    if (fb.writepos==hdr->m_uOffset && !hdr->t_ForceWrite) {//skip file
      ziplib_read_local_header(ar,hdr);
      fb.writepos+=hdr->t_LocalHeaderSize;
      logwritepos+=hdr->t_LocalHeaderSize;
      ziplib_seek(ar,hdr->m_uComprSize,SEEK_CUR);
      fb.writepos+=hdr->m_uComprSize;
      logwritepos+=hdr->m_uComprSize;
    } else {
      ziplib_seek(ar,hdr->m_uOffset,SEEK_SET);
      ziplib_read_local_header(ar,hdr);
      ziplib_count_local_header_size(ar,hdr);
      hdr->m_uOffset=logwritepos;

      if (fb.avail<hdr->t_LocalHeaderSize) ziplib_flush_buffer(ar,&fb);
      ziplib_write_local_header_to_buffer(ar,hdr,fb.buf+fb.inbuf);
  findpkpk(fb.buf,fb.inbuf);

      fb.inbuf+=hdr->t_LocalHeaderSize;fb.avail-=hdr->t_LocalHeaderSize;
      logwritepos+=hdr->t_LocalHeaderSize;

      ziplib_flush_move_data(ar,&fb,hdr->m_uComprSize);
      logwritepos+=hdr->m_uComprSize;
  findpkpk(fb.buf,fb.inbuf);

    }
  }
  findpkpk(fb.buf,fb.inbuf);
  ziplib_flush_buffer(ar,&fb);

  free(fb.buf);

  ar->m_uOffset=fb.writepos;
  
  ziplib_write_central_dir(ar);
  ar->t_OnDisk=1;

  return 0;
}
