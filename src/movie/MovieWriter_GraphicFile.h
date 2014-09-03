//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
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
