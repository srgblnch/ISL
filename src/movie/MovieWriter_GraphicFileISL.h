//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
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
