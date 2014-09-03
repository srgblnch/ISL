//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
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
