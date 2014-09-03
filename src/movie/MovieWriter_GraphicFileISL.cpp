//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include "MovieWriter_GraphicFileISL.h"

#include <isl/Image.h>
#include "InternalErrorHandler.h"

namespace isl
{

  MovieWriter_GraphicFileISL::MovieWriter_GraphicFileISL( const MovieConfig& config )
    : MovieWriter_GraphicFile(config)
  {

  }

  MovieWriter_GraphicFileISL::~MovieWriter_GraphicFileISL()
  {

  }

  void MovieWriter_GraphicFileISL::do_write_frame( const isl::Image& frame, std::string filename )
  {
    CV_FUNCNAME( "MovieWriter_GraphicFileISL::do_write_frame" );
    __BEGIN__;

    if (frame.depth() == isl::ISL_STORAGE_UCHAR)
    {
      ISL_CALL( frame.save(filename.c_str()) );
    }
    else if (frame.depth() == isl::ISL_STORAGE_SHORT)
    {
      int bits_cut = config.bit_depth - 8;
      isl::Image conv_frame(frame.width(), frame.height(), isl::ISL_STORAGE_UCHAR);

      short* input;
      unsigned char* output;
      int input_step, output_step;

      CV_CALL( cvGetRawData(frame.get_ipl_image(),      (uchar**)&input,  &input_step) );
      input_step /= sizeof(input[0]);
      CV_CALL( cvGetRawData(conv_frame.get_ipl_image(), &output, &output_step) );
      output_step /= sizeof(output[0]);

      for (int i = 0; i < frame.height(); i++, input += input_step, output += output_step)
      {
        for (int j = 0; j < frame.width(); j++)
        {
          output[j] = (input[j] >> bits_cut);
        }
      }
      
      ISL_CALL( conv_frame.save(filename.c_str()) );
    }
    else if (frame.depth() == isl::ISL_STORAGE_USHORT)
    {
      int bits_cut = config.bit_depth - 8;
      isl::Image conv_frame(frame.width(), frame.height(), isl::ISL_STORAGE_UCHAR);

      unsigned short* input;
      unsigned char* output;
      int input_step, output_step;

      CV_CALL( cvGetRawData(frame.get_ipl_image(),      (uchar**)&input,  &input_step) );
      input_step /= sizeof(input[0]);
      CV_CALL( cvGetRawData(conv_frame.get_ipl_image(), &output, &output_step) );
      output_step /= sizeof(output[0]);

      for (int i = 0; i < frame.height(); i++, input += input_step, output += output_step)
      {
        for (int j = 0; j < frame.width(); j++)
        {
          output[j] = (input[j] >> bits_cut);
        }
      }

      IplImage* color_image = cvCreateImage( cvSize(frame.width(), frame.height()),
                                             IPL_DEPTH_8U,
                                             3);

      cvMerge(conv_frame.get_ipl_image(),
              conv_frame.get_ipl_image(),
              conv_frame.get_ipl_image(),
              0,
              color_image);

      //CV_CALL( cvWriteFrame(this->video_writer, color_image) );
      ISL_CALL( cvSaveImage(filename.c_str(), color_image) );

      CV_CALL( cvReleaseImage( &color_image ) );

    }
    __END__;
    __ISL_CHECK_ERROR__;
  }
}
