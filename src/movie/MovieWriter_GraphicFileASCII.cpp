#include "MovieWriter_GraphicFileASCII.h"

#include <isl/Image.h>
#include "InternalErrorHandler.h"
#include <fstream>

namespace isl
{


  MovieWriter_GraphicFileASCII::MovieWriter_GraphicFileASCII( const MovieConfig& config )
    : MovieWriter_GraphicFile(config)
  {

  }

  MovieWriter_GraphicFileASCII::~MovieWriter_GraphicFileASCII()
  {
  }

  void MovieWriter_GraphicFileASCII::do_write_frame( const isl::Image& frame, std::string filename )
  {
    int i,j;

    CV_FUNCNAME( "MovieWriter_GraphicFileASCII::do_write_frame" );
    __BEGIN__;

    //- open the file as text
    std::ofstream file(filename.c_str());

    if (! file.is_open() )
      CV_ERROR(CV_StsError, "Unable to open file for writing"); 

    switch (frame.depth())
    {
    case isl::ISL_STORAGE_CHAR:
      {
        const char* row = static_cast<const char*>(frame.data());
        int step   = frame.row_byte_step();

        for (i = 0; i < frame.height(); i++)
        {
          for (j = 0; j < frame.width() - 1; j++)
          {
            file << row[j];
            file << ',';
          }
          file << row[j];

          row += step;
          if (i != frame.height() - 1)
            file << '\n';
        }
      }
      break;
    case isl::ISL_STORAGE_SHORT:
      {
        const short* row = static_cast<const short*>(frame.data());

        int step   = frame.row_byte_step();
        step /= sizeof(short);

        for (i = 0; i < frame.height(); i++)
        {
          for (j = 0; j < frame.width() - 1; j++)
          {
            file << row[j];
            file << ',';
          }
          file << row[j];

          row += step;
          if (i != frame.height() - 1)
            file << '\n';
        }
      }
      break;
    case isl::ISL_STORAGE_USHORT:
      {
        const unsigned short* row = static_cast<const unsigned short*>(frame.data());

        int step   = frame.row_byte_step();
        step /= sizeof(unsigned short);

        for (i = 0; i < frame.height(); i++)
        {
          for (j = 0; j < frame.width(); j++)
          {
            file << row[j];
            file << ',';
          }
          file << row[j];

          row += step;
          if (i != frame.height() - 1)
            file << '\n';
        }
      }
      break;
    }


    __END__;
    __ISL_CHECK_ERROR__;
  }

}
