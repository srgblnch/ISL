#ifndef _MOVIEWRITER_GRAPHICFILEASCII_H_
#define _MOVIEWRITER_GRAPHICFILEASCII_H_

#include "MovieWriter_GraphicFile.h"

namespace isl
{

  /*
   * Base class for writing a movie as independent files
   *
   * This class manages the file naming
   */
  class MovieWriter_GraphicFileASCII : public MovieWriter_GraphicFile
  {
  public:
    MovieWriter_GraphicFileASCII( const MovieConfig& config );
    ~MovieWriter_GraphicFileASCII();

  protected:
    void do_write_frame( const isl::Image& frame, std::string filename );
  };

}

#endif
