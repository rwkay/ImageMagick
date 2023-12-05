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


static Image *ReadASEImage(const ImageInfo *image_info, ExceptionInfo *exception)
{
  return NULL;
} // end ReadBMPImage


static MagickBooleanType WriteASEImage(const ImageInfo *image_info,Image *image, ExceptionInfo *exception)
{
  return MagickTrue;
} // end WriteBMPImage


static MagickBooleanType IsASE(const unsigned char *magick,const size_t length)
{
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
