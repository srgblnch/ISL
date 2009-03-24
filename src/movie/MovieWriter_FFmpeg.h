#ifndef _MOVIEWRITER_FFMPEG_H_
#define _MOVIEWRITER_FFMPEG_H_

#include "MovieWriter.h"


namespace isl
{
  class MovieWriter_FFmpeg : public MovieWriter
  {
  public:
    MovieWriter_FFmpeg( const MovieConfig& config );
    virtual ~MovieWriter_FFmpeg();
    
    void write_frame( const isl::Image& frame );

  private:
    CvVideoWriter* video_writer;
  };
}

#endif

