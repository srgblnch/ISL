#include <isl/movie/MovieWriter.h>
#include "InternalErrorHandler.h"
//#include "MovieWriter_FFmpeg.h"
#include "MovieWriter_GraphicFileISL.h"
#include "MovieWriter_Nexus.h"
//#include "MovieWriter_GraphicFileRAS.h"
#include "MovieWriter_GraphicFileASCII.h"
#include <algorithm>

namespace isl
{

  IMovieWriter* MovieWriterFactory::create( const MovieConfig& _config )
  {
    IMovieWriter* mvw = 0;
    CV_FUNCNAME( "MovieWriterFactory::create" );
    __BEGIN__;

    MovieConfig config;
    config.format = _config.format;
    std::transform(_config.format.begin(),
                   _config.format.end(),
                   config.format.begin(),
                   ::tolower); 
    config.file_basename = _config.file_basename;
    config.bit_depth     = _config.bit_depth;
    config.frame_rate    = _config.frame_rate;


    /*if (config.format == "mpg"
        || config.format == "avi")
    {
      mvw = new MovieWriter_FFmpeg(config);
    }*/

    if ((config.format == "jpg")
         || (config.format == "png")
         || (config.format == "tif")
         || (config.format == "bmp")
         || (config.format == "dib")
         || (config.format == "pgm"))
    {
      ISL_CALL( mvw = new MovieWriter_GraphicFileISL(config) );
    }

    if ((config.format == "nxs")
     || (config.format == "xml"))
    {
      ISL_CALL( mvw = new MovieWriter_Nexus(config) );
    }

    /*
    if (format_lower == "ras")
    {
      mvw = new MovieWriter_GraphicFileRAS(config);
    }
    */

    if (config.format == "txt")
    {
      ISL_CALL( mvw = new MovieWriter_GraphicFileASCII(config) );
    }
    
    /*if (config.format == "mat")
    {

    }*/

    __END__;
    __ISL_CHECK_ERROR__;
    return mvw;
  }

}
