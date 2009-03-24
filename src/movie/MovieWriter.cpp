#include <isl/movie/MovieWriter.h>
#include "InternalErrorHandler.h"
#include "MovieWriter_FFmpeg.h"
#include "MovieWriter_GraphicFileISL.h"
#include "MovieWriter_GraphicFileRAS.h"
#include "MovieWriter_GraphicFileASCII.h"
#include <algorithm>

namespace isl
{

  IMovieWriter::~IMovieWriter()
  {
  }

  MovieWriter::MovieWriter( const MovieConfig& _config )
    : config(_config)
  {

  }

  MovieWriter::~MovieWriter()
  {
  }

}
