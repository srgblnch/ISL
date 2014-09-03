//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/movie/MovieWriter.h>
#include "InternalErrorHandler.h"
#include "MovieWriter_FFmpeg.h"
#include "MovieWriter_GraphicFileISL.h"
#include "MovieWriter_GraphicFileRAS.h"
#include "MovieWriter_GraphicFileASCII.h"
#include "MovieWriter_Nexus.h"
#include <algorithm>

namespace isl
{

  IMovieWriter::~IMovieWriter()
  {
  }

  MovieWriter::MovieWriter( const MovieConfig& _config )
    : config(_config)
  {

  }

  MovieWriter::~MovieWriter()
  {
  }

}
