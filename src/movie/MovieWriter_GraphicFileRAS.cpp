#include "MovieWriter_GraphicFileRAS.h"

#include <isl/Image.h>
#include "InternalErrorHandler.h"

namespace isl
{

  typedef struct _SunRaster
  {
    unsigned long MagicNumber;      /* Magic (identification) number */
    unsigned long Width;            /* Width of image in pixels */
    unsigned long Height;           /* Height of image in pixels */
    unsigned long Depth;            /* Number of bits per pixel */
    unsigned long Length;           /* Size of image data in bytes */
    unsigned long Type;             /* Type of raster file */
    unsigned long ColorMapType;     /* Type of color map */
    unsigned long ColorMapLength;   /* Size of the color map in bytes */
  } SunRasterHeader;

#define	RAS_MAGIC	0x59a66a95

  /* Sun supported ras_type's */
#define RT_OLD		0	/* Raw pixrect image in 68000 byte order */
#define RT_STANDARD	1	/* Raw pixrect image in 68000 byte order */
#define RT_BYTE_ENCODED	2	/* Run-length compression of bytes */
#define RT_FORMAT_RGB	3	/* XRGB or RGB instead of XBGR or BGR */
#define RT_FORMAT_TIFF	4	/* tiff <-> standard rasterfile */
#define RT_FORMAT_IFF	5	/* iff (TAAC format) <-> standard rasterfile */
#define RT_EXPERIMENTAL 0xffff	/* Reserved for testing */

  /* Sun registered ras_maptype's */
#define RMT_RAW		2
  /* Sun supported ras_maptype's */
#define RMT_NONE	0	/* ras_maplength is expected to be 0 */
#define RMT_EQUAL_RGB	1	/* red[ras_maplength/3],green[],blue[] */



  MovieWriter_GraphicFileRAS::MovieWriter_GraphicFileRAS( const MovieConfig& config )
    : MovieWriter_GraphicFile(config)
  {

  }

  MovieWriter_GraphicFileRAS::~MovieWriter_GraphicFileRAS()
  {
  }

  void MovieWriter_GraphicFileRAS::do_write_frame( const isl::Image& frame, std::string filename )
  {
    CV_FUNCNAME( "MovieWriter_GraphicFileRAS::do_write_frame" );
    __BEGIN__;

    __END__;
    __ISL_CHECK_ERROR__;
  }

}
