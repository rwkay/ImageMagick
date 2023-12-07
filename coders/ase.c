#include "MagickCore/studio.h"
#include "MagickCore/blob.h"
#include "MagickCore/blob-private.h"
#include "MagickCore/cache.h"
#include "MagickCore/colormap-private.h"
#include "MagickCore/color-private.h"
#include "MagickCore/colormap.h"
#include "MagickCore/colorspace.h"
#include "MagickCore/colorspace-private.h"
#include "MagickCore/exception.h"
#include "MagickCore/exception-private.h"
#include "MagickCore/image.h"
#include "MagickCore/image-private.h"
#include "MagickCore/list.h"
#include "MagickCore/log.h"
#include "MagickCore/magick.h"
#include "MagickCore/memory_.h"
#include "MagickCore/monitor.h"
#include "MagickCore/monitor-private.h"
#include "MagickCore/option.h"
#include "MagickCore/pixel-accessor.h"
#include "MagickCore/profile.h"
#include "MagickCore/quantum-private.h"
#include "MagickCore/static.h"
#include "MagickCore/string_.h"
#include "MagickCore/module.h"
#include "MagickCore/transform.h"

typedef struct _ASEHeader
{
  uint32_t filesize;
  uint16_t magic;
  uint16_t frames;
  uint16_t width;
  uint16_t height;
  uint16_t bitsperpixel;
  uint32_t flags;
  uint16_t speed;
  uint32_t zero1;
  uint32_t zero2;
  uint8_t transparentIndex;
  uint8_t ignore1, ignore2, ignore3;
  uint16_t numcolours;
  uint8_t pixelwidth;
  uint8_t pixelheight;
  int16_t xposgrid;
  int16_t yposgrid;
  uint16_t gridwidth;
  uint16_t gridheight;
} ASEHeader;


static Image *ReadASEImage(const ImageInfo *image_info, ExceptionInfo *exception)
{
  Image
    *image;

  MagickBooleanType
    status;

  ASEHeader header;
  uint32_t currsize;
  uint32_t framesize;
  uint16_t framemagic;
  MagickOffsetType curroffset;



  printf( "ReadASE :: image_info = %p, Exception = %p\n", image_info, exception );
  /*
    Open image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickCoreSignature);
  if (IsEventLogging() != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",
      image_info->filename);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickCoreSignature);
  image=AcquireImage(image_info,exception);
  image->columns=0;
  image->rows=0;
  status=OpenBlob(image_info,image,ReadBinaryBlobMode,exception);
  if (status == MagickFalse)
    {
      image=DestroyImageList(image);
      return((Image *) NULL);
    }

  header.filesize = ReadBlobLSBLong(image);
  header.magic = ReadBlobLSBShort(image);
  header.frames = ReadBlobLSBShort(image);
  header.width = ReadBlobLSBShort(image);
  header.height = ReadBlobLSBShort(image);
  header.bitsperpixel = ReadBlobLSBShort(image);
  header.flags = ReadBlobLSBLong(image);
  header.speed = ReadBlobLSBShort(image);
  header.zero1 = ReadBlobLSBLong(image);
  header.zero2 = ReadBlobLSBLong(image);
  header.transparentIndex = ReadBlobByte(image);
  header.ignore1 = ReadBlobByte(image);
  header.ignore2 = ReadBlobByte(image);
  header.ignore3 = ReadBlobByte(image);
  header.numcolours = ReadBlobLSBShort(image);
  header.pixelwidth = ReadBlobByte(image);
  header.pixelheight = ReadBlobByte(image);
  header.xposgrid = ReadBlobLSBShort(image);
  header.yposgrid = ReadBlobLSBShort(image);
  header.gridwidth = ReadBlobLSBShort(image);
  header.gridheight = ReadBlobLSBShort(image);

  printf( "filesize = %d\n", header.filesize);
  printf( "magic = %04x\n", header.magic);
  printf( "frames = %d\n", header.frames);
  printf( "width = %d\n", header.width);
  printf( "height = %d\n", header.height);
  printf( "bitsperpixel = %d\n", header.bitsperpixel);
  printf( "flags = %08x\n", header.flags);
  printf( "speed = %d\n", header.speed);
  printf( "transparentindex = %d\n", header.transparentIndex);
  printf( "numcolours = %d\n", header.numcolours);
  printf( "pixelwidth = %d\n", header.pixelwidth);
  printf( "pixelheight = %d\n", header.pixelheight);
  printf( "xposgrid = %d\n", header.xposgrid);
  printf( "yposgrid = %d\n", header.yposgrid);
  printf( "gridwidth = %d\n", header.gridwidth);
  printf( "gridheight = %d\n", header.gridheight);

  SeekBlob(image, 128, SEEK_SET);

  currsize = header.filesize - 128;
  while( currsize > 0) {

    curroffset = TellBlob( image );

    // get all the frames
    framesize = ReadBlobLSBLong(image);
    framemagic = ReadBlobLSBShort(image);
    printf( "framesize = %d\n", framesize);
    printf( "framemagic = %04x\n", framemagic);


    // next frame
    SeekBlob(image, curroffset+framesize, SEEK_SET);
    currsize -= framesize;
  } // end while

  return NULL;
} // end ReadASEImage


static MagickBooleanType WriteASEImage(const ImageInfo *image_info,Image *image, ExceptionInfo *exception)
{
  printf( "WriteASE :: image_info = %p, image = %p, Exception = %p\n", image_info, image, exception );
  return MagickTrue;
} // end WriteASEImage


static MagickBooleanType IsASE(const unsigned char *magick,const size_t length)
{
  printf( "IsASE :: magick = %p, len = %ld\n", magick, length );
  return(MagickFalse);
} // end IsASE


ModuleExport size_t RegisterASEImage(void)
{
  MagickInfo
    *entry;

  entry=AcquireMagickInfo("ASE","ASE","Animated Sprite Editor file format");
  entry->decoder=(DecodeImageHandler *) ReadASEImage;
  entry->encoder=(EncodeImageHandler *) WriteASEImage;
  entry->magick=(IsImageFormatHandler *) IsASE;
  entry->flags^=CoderAdjoinFlag;
  entry->flags|=CoderDecoderSeekableStreamFlag;
  entry->mime_type=ConstantString("application/octet-stream");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}

ModuleExport void UnregisterASEImage(void)
{
  (void) UnregisterMagickInfo("ASE");
}
