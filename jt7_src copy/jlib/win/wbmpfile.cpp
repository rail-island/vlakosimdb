
/*
  bmp.c

  Decodes bmp images.

  ---------------------------------------------------
  Author:                Cinek

  Date:                  07/26/99
  Ver:                   0.11
  Updates:               initial

*/

#include <stdio.h>
#include <stdlib.h>
#include "wbmpfile.h"


static FILE *bmpfile;
static int bitsperpixel,bytesinline,
  width,height,bmpfilesize;
static RGBQuad *bmppalette;
static unsigned char *bmpdata;

/* prototypes */
unsigned char BMP_Clamp(double);
unsigned int BMP_ToBW(unsigned int);


static inline unsigned short int BMP_ReadWord(void)
{
  return fgetc(bmpfile)|(fgetc(bmpfile)<<8);
}
static inline unsigned int BMP_ReadLong(void)
{
  return BMP_ReadWord()|((unsigned int)BMP_ReadWord()<<16);
}

/* output size will be ignored */
int BMP_OpenFile(char *filename,char *imgdir,int owidth,int oheight)
{
  BitmapFileHeader bfh;
  BitmapInfo bi;
  int palettecolors,cnr,arraysize;

  width=height=0;
  bmpfile=NULL;
  if ((bmpfile=fopen(filename,"rb"))==NULL) return 0;

  fseek(bmpfile,0,SEEK_END);
  bmpfilesize=ftell(bmpfile);
  rewind(bmpfile);

  if ((bfh.bfType=BMP_ReadWord())!=BMP_SIGNATURE) return 0;
  bfh.bfSize=BMP_ReadLong();
  bfh.bfReserved[0]=BMP_ReadWord();
  bfh.bfReserved[1]=BMP_ReadWord();
  bfh.bfOffBits=BMP_ReadLong();

#ifdef VERBOSE
  fprintf(stderr,"bfOffBits: %d\n",bfh.bfOffBits);
#endif 

  palettecolors=(bfh.bfOffBits-54)/4; 
  /* minus sizeof(BitmapFileHeader) minus sizeof(BitmapInfoHeader) */

  bi.bmiHeader.biSize=BMP_ReadLong();
  width=bi.bmiHeader.biWidth=BMP_ReadLong();
  height=bi.bmiHeader.biHeight=BMP_ReadLong();
  bi.bmiHeader.biPlanes=BMP_ReadWord();
  bi.bmiHeader.biBitCount=BMP_ReadWord();
  bitsperpixel=bi.bmiHeader.biPlanes*bi.bmiHeader.biBitCount;
  switch (bitsperpixel) {
  case 1:
  case 4:
  case 8:
  case 24:
    break;
  default:
    return 0;
  }

  if ((bi.bmiHeader.biCompression=BMP_ReadLong())!=0) return 0;
  bi.bmiHeader.biSizeImage=BMP_ReadLong();
  bi.bmiHeader.biXPelsPerMeter=BMP_ReadLong();
  bi.bmiHeader.biYPelsPerMeter=BMP_ReadLong();
  bi.bmiHeader.biClrUsed=BMP_ReadLong();
  bi.bmiHeader.biClrImportant=BMP_ReadLong();

  if (palettecolors) {
    if ((bmppalette=(RGBQuad *)malloc(palettecolors*sizeof(RGBQuad)))==NULL)
      return 0;
    bi.bmiColors=bmppalette;

    for (cnr=0;cnr<palettecolors;cnr++) {
      bmppalette[cnr]=fgetc(bmpfile)|((long int)fgetc(bmpfile)<<8)|
	((long int)fgetc(bmpfile)<<16);
      fgetc(bmpfile);
    }
  }

  bytesinline=width*bitsperpixel/8;
  while (bytesinline&3) bytesinline++;

  arraysize=bmpfilesize-ftell(bmpfile);
  if ((bmpdata=(unsigned char *)malloc((size_t)arraysize))==NULL) {
    free(bmppalette);
    return 0;
  }

#ifdef VERBOSE
  fprintf(stderr,"Data offset: %ld\n",ftell(bmpfile));
  fprintf(stderr,"Data size/loaded: %d/%d\n",height*bytesinline,arraysize);
#endif 

  fread(bmpdata,1,(size_t)arraysize,bmpfile);
  return 1;
}

void BMP_CloseFile(void)
{
  if (bmppalette!=NULL) free(bmppalette);
  if (bmpdata!=NULL) free(bmpdata);

  if (bmpfile!=NULL) fclose(bmpfile);
  bmpfile=NULL;
}

unsigned char BMP_Clamp(double val)
{
  if (val<0) val=0;
  if (val>255) val=255;
  return (unsigned char)val;
}

unsigned int BMP_ToBW(unsigned int color)
{
  unsigned char y;

  y=BMP_Clamp(((color>>16)&0xFF)*0.333+((color>>8)&0xFF)*0.586+
	      (color&0xFF)*0.114);

  return (y<<16)|(y<<8)|y;
}

unsigned int BMP_GetPixel(int x,int y,int col)
{
  unsigned char *offs;
  unsigned int color;

  if ((x<0)||(x>=width)||(y<0)||(y>=height)) return 0x000000;

#ifdef VERBOSE
  fprintf(stderr,"Getting pixel: %dx%d\n",x,y);
#endif

  offs=bmpdata+(height-y-1)*bytesinline; /* BMP from bottom */
  
  switch (bitsperpixel) {
  case 1:
    color=bmppalette[(offs[x/8]>>(7-(x&7)))&1];
    break;
  case 4:
    color=bmppalette[(offs[x/2]>>(x&1 ? 0 : 4))&0x0F];
    break;
  case 8:
    color=bmppalette[offs[x]];
    break;
  case 24:
    color=offs[x*3]|(offs[x*3+1]<<8)|(offs[x*3+2]<<16);
    break;
  default:
    return 0;
  }

  if (col) return color;
  else return BMP_ToBW(color);
}

int BMP_GetWidth(void)
{
  return width;
}

int BMP_GetHeight(void)
{
  return height;
}
