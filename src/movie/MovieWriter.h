#ifndef _MOVIEWRITER_INTERNAL_H_
#define _MOVIEWRITER_INTERNAL_H_

#include <isl/movie/MovieWriter.h>


namespace isl
{
  class MovieWriter : public IMovieWriter
  {
  public:
    MovieWriter( const MovieConfig& config );
    virtual ~MovieWriter();
    
  protected:
    MovieConfig config;
  };
}

#endif

