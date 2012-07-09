
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
