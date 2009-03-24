#ifndef _MOVIEWRITER_GRAPHICFILE_H_
#define _MOVIEWRITER_GRAPHICFILE_H_

#include "MovieWriter.h"

namespace isl
{

  /*
   * Base class for writing a movie as independent files
   *
   * This class manages the file naming
   */
  class MovieWriter_GraphicFile : public MovieWriter
  {
  public:
    MovieWriter_GraphicFile( const MovieConfig& config );
    virtual ~MovieWriter_GraphicFile();

    void write_frame( const isl::Image& frame );

  protected:
    virtual void do_write_frame( const isl::Image& frame, std::string filename ) = 0;

  private:
    std::string next_filename( void );
    size_t      frame_count;
  };

}

#endif
