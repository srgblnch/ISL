#ifndef _MOVIEWRITER_H_
#define _MOVIEWRITER_H_

#include <isl/ISLConfig.h>
#include <string>

namespace isl
{
  class Image;

  struct ISL_DECL IMovieWriter
  {
    virtual ~IMovieWriter();

    virtual void write_frame( const isl::Image& frame ) = 0;
  };


  struct ISL_DECL MovieConfig
  {
    std::string format;
    std::string file_basename;
    int bit_depth;
    double frame_rate;
  };

  class ISL_DECL MovieWriterFactory
  {
  public:
    static IMovieWriter* create( const MovieConfig& config );
  };
}

#endif
