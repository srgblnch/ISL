//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************

#pragma once

#include "MovieWriter.h"

class NexusTask;

namespace isl
{

  /*
   * Class for writing a movie as a Nexus file
   *
   */
  class MovieWriter_Nexus : public MovieWriter
  {
  public:
    MovieWriter_Nexus( const MovieConfig& config );
    virtual ~MovieWriter_Nexus();
    void write_frame( const isl::Image& frame );

  protected:
    void write_head(int width, int height);
  private:

    NexusTask* m_task;

    void*       m_nexusHandle;
    size_t      m_frameCount;
    bool        m_headWritten;
    int         m_shape[3];
  };

}
