#ifndef _MOVIEWRITER_GRAPHICFILERAW_H_
#define _MOVIEWRITER_GRAPHICFILERAW_H_

#include "MovieWriter_GraphicFile.h"

namespace isl
{

  /*
   * Base class for writing a movie as independent files
   *
   * This class manages the file naming
   */
  class MovieWriter_GraphicFileRAS : public MovieWriter_GraphicFile
  {
  public:
    MovieWriter_GraphicFileRAS( const MovieConfig& config );
    ~MovieWriter_GraphicFileRAS();

  protected:
    void do_write_frame( const isl::Image& frame, std::string filename );
  };

}

#endif
