#include "MovieWriter_GraphicFile.h"

#include <isl/Image.h>
#include "InternalErrorHandler.h"
#include <sstream>

namespace isl
{

  MovieWriter_GraphicFile::MovieWriter_GraphicFile( const MovieConfig& config )
    : MovieWriter(config),
      frame_count(0)
  {

  }

  MovieWriter_GraphicFile::~MovieWriter_GraphicFile()
  {

  }

  void MovieWriter_GraphicFile::write_frame( const isl::Image& frame)
  {
    CV_FUNCNAME( "MovieWriter_GraphicFile::write_frame" );
    __BEGIN__;

    ISL_CALL( this->do_write_frame( frame, this->next_filename() ) );
    this->frame_count++;

    __END__;
    __ISL_CHECK_ERROR__;
  }

  std::string MovieWriter_GraphicFile::next_filename( void )
  {
    std::ostringstream os;

    os << config.file_basename;
    os << "_";
    os.fill('0');
    os.width(4);
    os << this->frame_count;
    os << ".";
    os << config.format;
    
    return os.str();
  }

}
