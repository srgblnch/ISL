#ifndef _MOVIEWRITER_GRAPHICFILEISL_H_
#define _MOVIEWRITER_GRAPHICFILEISL_H_

#include "MovieWriter_GraphicFile.h"

namespace isl
{

  class MovieWriter_GraphicFileISL : public MovieWriter_GraphicFile
  {
  public:
    MovieWriter_GraphicFileISL( const MovieConfig& config );
    ~MovieWriter_GraphicFileISL();

  protected:
    void do_write_frame( const isl::Image& frame, std::string filename );
  };

}

#endif
