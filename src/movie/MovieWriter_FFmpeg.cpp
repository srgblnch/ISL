//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************

// THIS CLASS IS EXPERIMENTAL - DO NOT COMPILE


#include "MovieWriter_FFmpeg.h"

#include <isl/Image.h>
#include "InternalErrorHandler.h"

namespace isl
{

  MovieWriter_FFmpeg::MovieWriter_FFmpeg( const MovieConfig& config )
    : MovieWriter(config),
      video_writer(0)
  {
  }

  MovieWriter_FFmpeg::~MovieWriter_FFmpeg()
  {
    CV_FUNCNAME( "MovieWriter_FFmpeg::~MovieWriter_FFmpeg" );
    __BEGIN__;

    if (this->video_writer)
      CV_CALL( cvReleaseVideoWriter(&this->video_writer) );
    
    __END__;
    
    if (cvGetErrStatus() < 0)
    {
      __ISL_DISABLE_ERROR__;
      __ISL_RESTORE_ERROR__;
    }

    __ISL_CHECK_ERROR__;
  }

  void MovieWriter_FFmpeg::write_frame( const isl::Image& frame )
  {
    CV_FUNCNAME( "MovieWriter_FFmpeg::write_frame" );
    __BEGIN__;


    /*
    FFV1 : A lossless video codec based on arithmetic coding developed in the open source ffmpeg project.
    FMP4 : The default MPEG4 format used by tool mencoder. DirectShow filters supporting the codec are available here.
    XVID : Codec is available in source form from XVID web site.
    PIM1 : MPEG-1 based codec
    */

    if (this->video_writer == 0)
    {
      std::string filename = config.file_basename + "." + config.format;

      int fourcc = (config.format == "mpg") ? CV_FOURCC('P','I','M','1') : CV_FOURCC('X','V','I','D');

      CvVideoWriter* vw = 0;
      CV_CALL( vw = cvCreateVideoWriter(filename.c_str(), 
                                        fourcc,
                                        this->config.frame_rate,
                                        cvSize(frame.width(), frame.height()),
                                        0) );
      this->video_writer = vw;
    }

    if (frame.depth() == isl::ISL_STORAGE_UCHAR)
    {
      CV_CALL( cvWriteFrame(this->video_writer, frame.get_ipl_image()) );
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
      
      CV_CALL( cvWriteFrame(this->video_writer, conv_frame.get_ipl_image()) );
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

      CV_CALL( cvWriteFrame(this->video_writer, conv_frame.get_ipl_image()) );

    }
    __END__;
    __ISL_CHECK_ERROR__;

  }

}

