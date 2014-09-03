//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
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

