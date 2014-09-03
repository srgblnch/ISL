//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
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

