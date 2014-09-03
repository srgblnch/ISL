//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
#include <isl/calib/Calib.h>

#include "InternalErrorHandler.h"
#include "PerspEngine.h"
#include "TsaiEngine.h"
#include <iostream>

# define TIMER_DECL  int64 start_counter, end_counter;
# define TIMER_START { start_counter = cvGetTickCount(); }
# define TIMER_END { end_counter = cvGetTickCount(); }
# define TIMER_ELAPSED_MS (((double)(end_counter - start_counter)) / cvGetTickFrequency() * 1e-3)

namespace isl {

const double kDEFAULT_X_SPACING = 1.0f;
const double kDEFAULT_Y_SPACING = 1.0f;
const int    kDEFAULT_CALIB_TYPE = ISL_CALIB_PERSP;
const int    kDEFAULT_SMOOTH_KERNEL_WIDTH = 3;
const double kDEFAULT_ADTHRESH_WINDOW_SIZE = 0.3f;
const double kDEFAULT_ADTHRESH_OFFSET = 3.0f;
const int    kDEFAULT_DILATION_WIDTH = 3;
const int    kDEFAULT_DILATION_SHAPE = ISL_SHAPE_ELLIPSE;
const double kDEFAULT_MEDIAN_AREA_MAX_DEVIATION = 0.5f;
const int    kDEFAULT_CENTROID_ROUGH_EXTRACT_METHOD = ISL_BOUNDING_RECT_CENTER;
const int    kDEFAULT_REFINEMENT_METHOD = ISL_SQUARED_CENTROID;

Calib::Config::Config()
: x_spacing(kDEFAULT_X_SPACING),
  y_spacing(kDEFAULT_Y_SPACING),
  calib_type(kDEFAULT_CALIB_TYPE),
  smoothing_kernel_width(kDEFAULT_SMOOTH_KERNEL_WIDTH),
  adthresh_window_size_factor(kDEFAULT_ADTHRESH_WINDOW_SIZE),
  adthresh_offset(kDEFAULT_ADTHRESH_OFFSET),
  dilatation_width(kDEFAULT_DILATION_WIDTH),
  dilatation_shape(kDEFAULT_DILATION_SHAPE),
  median_area_max_deviation(kDEFAULT_MEDIAN_AREA_MAX_DEVIATION),
  centroid_rough_extraction(kDEFAULT_CENTROID_ROUGH_EXTRACT_METHOD),
  refinement_method(kDEFAULT_REFINEMENT_METHOD)
{
  ::memset(roi, 0, 4 * sizeof(Point2D<int>));
}

Calib::Config::~Config()
{
}

Calib::Calib()
: p_(0),
  e_(0)
{
}

Calib::~Calib()
{
  SAFE_DELETE(this->p_);
  SAFE_DELETE(this->e_);
}

void
Calib::learn(const Image& _image)
{
  TIMER_DECL;

  CV_FUNCNAME( "Calib::learn" );
  __BEGIN__;

  SAFE_DELETE(this->p_);
  ISL_CALL( this->p_ = new PatternAnalyzer );

  SAFE_DELETE(this->e_);

  if (this->config.calib_type == ISL_CALIB_PERSP)
  {
    ISL_CALL( this->e_ = new PerspEngine );
  }
  else if (this->config.calib_type == ISL_CALIB_TSAI)
  {
    ISL_CALL( this->e_ = new TsaiEngine );
  }
  else
  {
    CV_ERROR( CV_StsBadArg, "Wrong calibration type specified");
  }

  TIMER_START;
  ISL_CALL( this->p_->compute(_image, this->config) );
  ISL_CALL( this->e_->learn(*this->p_) );
  TIMER_END;
  std::cout << "learning time [ms] : " << TIMER_ELAPSED_MS << std::endl;

  __END__;
  
  if (cvGetErrStatus() < 0)
  {
    __ISL_DISABLE_ERROR__;
    SAFE_DELETE(this->p_);
    SAFE_DELETE(this->e_);
    __ISL_RESTORE_ERROR__;
  }
  __ISL_CHECK_ERROR__;
}

void
Calib::undistort(const Image& _src, Image& _dst, int _interp_type)
{
  TIMER_DECL;
  
  CV_FUNCNAME( "Calib::undistort" );
  __BEGIN__;

  TIMER_START;
  if (this->e_)
  {
    ISL_CALL( this->e_->undistort(_src, _dst, _interp_type) );
  }
  else
  {
    ISL_CALL( _src.convert(_dst) );
  }
  TIMER_END;
  std::cout << "undistort time [ms] : " << TIMER_ELAPSED_MS << std::endl;

  __END__;
  __ISL_CHECK_ERROR__;
}

void
Calib::draw_error_map(Image& _im, double _scale, double _intensity, int _thickness, int _line_type) const
{
  CV_FUNCNAME( "Calib::draw_error_map" );
  __BEGIN__;

  if (this->e_)
  {
    ISL_CALL( this->e_->draw_error_map(_im, _scale, _intensity, _thickness, _line_type) );
  }

  __END__;
  __ISL_CHECK_ERROR__;
}

void
Calib::draw_blobs(Image& _im, double _intensity, int _thickness, int _line_type) const
{
  CV_FUNCNAME( "Calib::draw_blobs" );
  __BEGIN__;

  if (this->p_)
  {
    ISL_CALL( this->p_->draw_blobs(_im, _intensity, _thickness, _line_type) );
  }

  __END__;
  __ISL_CHECK_ERROR__;
}

void
Calib::draw_delaunay(Image& _im, double _intensity, int _thickness, int _line_type) const
{
  CV_FUNCNAME( "Calib::draw_delaunay" );
  __BEGIN__;

  if (this->p_)
  {
    ISL_CALL( this->p_->draw_delaunay(_im, _intensity, _thickness, _line_type) );
  }

  __END__;
  __ISL_CHECK_ERROR__;
}

const Image&
Calib::preprocessed_image() const
{
  CV_FUNCNAME( "Calib::preprocessed_image" );
  __BEGIN__;
  
  if (!this->p_)
  {
    CV_ERROR(CV_StsNullPtr, "Calib::learn has not yet been called");
  }

  __END__;
  __ISL_CHECK_ERROR__;
  return( this->p_->preprocessed_image() );
}

double
Calib::x_mag_factor() const
{
  double ret = 0;
  if (this->e_)
    ret = this->e_->x_mag_factor();

  return ret;
}

double
Calib::y_mag_factor() const
{
  double ret = 0;
  if (this->e_)
    ret = this->e_->y_mag_factor();

  return ret;
}

double
Calib::model_error() const
{
  double ret = 0;
  if (this->e_)
    ret = this->e_->model_error();

  return ret;
}

}
